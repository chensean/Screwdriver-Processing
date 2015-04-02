#include "test_context.h"
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include "rtr_tm_client.h"
#include "rtr_monitor_client.h"
#include "tm_parameter_manager.h"
#include "irig_frame.h"
#include "sfid_minor_frame.h"
#include "fcc_minor_frame.h"
#include "string_utilities.h"
#include "digit_utilities.h"
#include "base_tm_parameter.h"
#include "bits_tm_parameter.h"

namespace screwdriver
{
	struct test_context::test_context_imp_t
	{
		test_context_imp_t()
			:tm_parameter_manager_(new TM::tm_parameter_manager)
		{
		}

		boost::shared_ptr<TM::tm_parameter_manager> tm_parameter_manager_;
		boost::shared_ptr<rtr_tm_client> rtr_tm_client_;
		boost::shared_ptr<rtr_monitor_client> rtr_monitor_client_;
		boost::shared_ptr<irig_frame> irig_frame_;
	};

	test_context::test_context(void)
		:imp_(new test_context_imp_t)
	{
	}

	test_context::~test_context(void)
	{
	}

	void test_context::create_rtr_tm_client(const std::string& ip, const std::string& folder)
	{
		stop_rtr_tm();
		imp_->rtr_tm_client_ = boost::shared_ptr<rtr_tm_client>(new rtr_tm_client(ip, folder));
	}

	void test_context::start_rtr_tm()
	{
		if (imp_->rtr_tm_client_)
		{
			imp_->rtr_tm_client_->start();
		}
	}

	void test_context::stop_rtr_tm()
	{
		if (imp_->rtr_tm_client_)
		{
			imp_->rtr_tm_client_->stop();
		}
	}

	void test_context::config_irig_frame(const boost::property_tree::ptree& pt)
	{
		auto irig_pt = pt.get_child("CFG.ACQ_SYS.SOURCE.IRIG.IRIGFrameStruc");
		auto bit_rate = irig_pt.get<uint64_t>("<xmlattr>.BitRate");
		auto word_length = irig_pt.get<uint32_t>("<xmlattr>.WordLength");
		auto minor_frame_words = irig_pt.get<uint32_t>("<xmlattr>.MinorFrameWords");
		auto minor_frames = irig_pt.get<uint32_t>("<xmlattr>.MinorFrames");
		auto frame_mode = irig_pt.get<std::string>("<xmlattr>.FrameMode");
		auto sfid_bits = irig_pt.get<uint32_t>("<xmlattr>.SFIDBits");
		auto sfid_position = irig_pt.get<uint32_t>("<xmlattr>.SFIDPosition");
		auto lsb_bit = irig_pt.get<uint32_t>("<xmlattr>.SFIDLSBBit");
		auto sync_location = irig_pt.get<uint32_t>("<xmlattr>.SyncLocation");
		auto sync_pattern_string = irig_pt.get<std::string>("<xmlattr>.SyncPattern");
		auto sync_pattern = utilities::string_to_uinteger_base16<uint16_t>(sync_pattern_string);
		auto sync_pattern_array = utilities::big_endian_val_to_array(sync_pattern);
		get_minor_frame_id_fun_t get_minor_frame_id_fun;
		if (frame_mode == "FCC")
		{
			boost::shared_ptr<fcc_minor_frame> fcc_minor_frame_ptr(new fcc_minor_frame(sync_location, minor_frames, sync_pattern_array));
			get_minor_frame_id_fun = boost::bind(&fcc_minor_frame::get_minor_frame_id, fcc_minor_frame_ptr, _1, _2);
		}
		else
		{
			boost::shared_ptr<sfid_minor_frame> sfid_minor_frame_ptr(new sfid_minor_frame(sfid_position, sfid_bits, lsb_bit));
			get_minor_frame_id_fun = boost::bind(&sfid_minor_frame::get_minor_frame_id, sfid_minor_frame_ptr, _1, _2);
		}

		imp_->irig_frame_ = boost::shared_ptr<irig_frame>(new irig_frame(bit_rate, minor_frame_words * word_length / std::numeric_limits<uint8_t>::digits, minor_frames, get_minor_frame_id_fun));
	}

	void test_context::load_irig_config(const std::string& file_name)
	{
		using namespace boost::property_tree;
		ptree pt;
		try
		{
			read_xml(file_name, pt);
		}
		catch (xml_parser_error& e)
		{
			std::cout << "xml_parser_error! path:" << file_name << std::endl;
			std::cout << e.message() << std::endl;
			return;
		}
		config_irig_frame(pt);


		auto irig_pt = pt.get_child("CFG.ACQ_SYS.SOURCE.IRIG.IRIGFrameStruc");
		auto word_length = irig_pt.get<uint32_t>("<xmlattr>.WordLength");
		auto bytes = word_length / std::numeric_limits<uint8_t>::digits;
		auto minor_frames = irig_pt.get<uint32_t>("<xmlattr>.MinorFrames");
		auto sync_location = irig_pt.get<uint32_t>("<xmlattr>.SyncLocation");
		auto parameter = pt.get_child("CFG.ACQ_SYS.SOURCE.Parameters");
		std::for_each(parameter.begin(), parameter.end(),
		              [=](ptree::value_type parameter_pt)
		              {
			              auto name = parameter_pt.second.get<std::string>("<xmlattr>.IDENT");

			              auto format_type = parameter_pt.second.get<std::string>("Extraction.<xmlattr>.FormatType");
			              auto nb_words = parameter_pt.second.get<uint32_t>("Gernal.<xmlattr>.NbWords");
			              auto nb_bits = parameter_pt.second.get<uint32_t>("Gernal.<xmlattr>.NbBits");
			              auto lsb_pos_bit = parameter_pt.second.get<uint32_t>("Gernal.<xmlattr>.Lsbposbit");
			              TM::tm_parameter_ptr parameter_ptr;
			              if (nb_words * word_length == nb_bits)
			              {
				              std::string type = "double_parameter" + boost::lexical_cast<std::string>(nb_bits);
				              if (nb_bits > 8)
				              {
					              type = "big_endian_" + type;
				              }
				              parameter_ptr = TM::create_base_tm_parameter(type, name);
			              }
			              else
			              {
				              std::string type = "bits" + boost::lexical_cast<std::string>(nb_words * word_length) + "_double_parameter" + boost::lexical_cast<std::string>(nb_bits);
				              if (nb_words * word_length > 8)
				              {
					              type = "big_endian_" + type;
				              }
				              parameter_ptr = TM::create_bits_tm_parameter(type, name,lsb_pos_bit);
			              }

			              auto comm_type = parameter_pt.second.get<std::string>("Gernal.<xmlattr>.CommType");
			              auto major_pos = parameter_pt.second.get<uint32_t>("Gernal.<xmlattr>.MajorPos");
			              auto position = parameter_pt.second.get<uint32_t>("Gernal.<xmlattr>.Position");
		              });
	}
}
