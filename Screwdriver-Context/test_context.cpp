#include "test_context.h"
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/make_shared.hpp>
#include "rtr_tm_client.h"
#include "rtr_monitor_client.h"
#include "irig_frame.h"
#include "sfid_minor_frame.h"
#include "fcc_minor_frame.h"
#include "string_utilities.h"
#include "digit_utilities.h"
#include "base_tm_parameter.h"
#include "bits_tm_parameter.h"
#include "linear_conversion.h"
#include "polynomial_conversion.h"
#include "tm_time.h"
#include "object_manager.h"

namespace screwdriver
{
	
	typedef object_manager<parameter> parameter_manager;
	typedef object_manager<raw_data> raw_data_manager;
	typedef object_manager<TM::tm_parameter> tm_parameter_manager;
	struct test_context::test_context_imp_t
	{
		test_context_imp_t()
			:parameter_manager_(new parameter_manager),
			 raw_data_manager_(new raw_data_manager),
			 tm_parameter_manager_(new tm_parameter_manager)
		{
		}

		boost::shared_ptr<parameter_manager> parameter_manager_;
		boost::shared_ptr<raw_data_manager> raw_data_manager_;
		boost::shared_ptr<tm_parameter_manager> tm_parameter_manager_;
		boost::shared_ptr<rtr_tm_client> rtr_tm_client_;
		boost::shared_ptr<rtr_monitor_client> rtr_monitor_client_;
		boost::shared_ptr<irig_frame> irig_frame_;
		TM::tm_time_ptr tm_time_;
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
		imp_->rtr_tm_client_ = boost::make_shared<rtr_tm_client>(ip, folder);
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
		auto bytes = word_length / std::numeric_limits<uint8_t>::digits;
		auto minor_frame_words = irig_pt.get<uint32_t>("<xmlattr>.MinorFrameWords");
		auto minor_frames = irig_pt.get<uint32_t>("<xmlattr>.MinorFrames");
		auto frame_mode = irig_pt.get<std::string>("<xmlattr>.FrameMode");
		auto sfid_bits = irig_pt.get<uint32_t>("<xmlattr>.SFIDBits");
		auto sfid_position = irig_pt.get<uint32_t>("<xmlattr>.SFIDPosition");
		auto lsb_bit = irig_pt.get<uint32_t>("<xmlattr>.SFIDLSBBit");
		auto sync_location = irig_pt.get<std::string>("<xmlattr>.SyncLocation");
		auto sync_pattern_string = irig_pt.get<std::string>("<xmlattr>.SyncPattern");
		auto sync_pattern = utilities::string_to_uinteger_base16<uint16_t>(sync_pattern_string);
		auto sync_pattern_array = utilities::big_endian_val_to_array(sync_pattern);
		get_minor_frame_id_fun_t get_minor_frame_id_fun;
		if (frame_mode == "FCC")
		{
			uint32_t sync_pos = sync_location == "Begin" ? 0 : (minor_frame_words - 1) * bytes;
			boost::shared_ptr<fcc_minor_frame> fcc_minor_frame_ptr(new fcc_minor_frame(sync_pos, minor_frames, sync_pattern_array));
			get_minor_frame_id_fun = boost::bind(&fcc_minor_frame::get_minor_frame_id, fcc_minor_frame_ptr, _1, _2);
		}
		else
		{
			boost::shared_ptr<sfid_minor_frame> sfid_minor_frame_ptr(new sfid_minor_frame(sfid_position, sfid_bits, lsb_bit));
			get_minor_frame_id_fun = boost::bind(&sfid_minor_frame::get_minor_frame_id, sfid_minor_frame_ptr, _1, _2);
		}

		imp_->irig_frame_ = boost::make_shared<irig_frame>(bit_rate, minor_frame_words * word_length / std::numeric_limits<uint8_t>::digits, minor_frames, get_minor_frame_id_fun);
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

		if (!imp_->irig_frame_)
		{
			return;
		}
		auto irig_pt = pt.get_child("CFG.ACQ_SYS.SOURCE.IRIG.IRIGFrameStruc");
		auto word_length = irig_pt.get<uint32_t>("<xmlattr>.WordLength");
		auto bytes = word_length / std::numeric_limits<uint8_t>::digits;
		auto minor_frames = irig_pt.get<uint32_t>("<xmlattr>.MinorFrames");
		auto sync_location = irig_pt.get<std::string>("<xmlattr>.SyncLocation");
		auto parameters = pt.get_child("CFG.ACQ_SYS.SOURCE.Parameters");
		std::for_each(parameters.begin(), parameters.end(),
		              [=](ptree::value_type parameter_pt)
		              {
			              auto name = parameter_pt.second.get<std::string>("<xmlattr>.IDENT");

			              auto format_type = parameter_pt.second.get<std::string>("Extraction.<xmlattr>.FormatType");
			              auto nb_words = parameter_pt.second.get<uint32_t>("Extraction.<xmlattr>.NbWords");
			              auto nb_bits = parameter_pt.second.get<uint32_t>("Extraction.<xmlattr>.NbBits");
			              auto lsb_pos_bit = parameter_pt.second.get<uint32_t>("Extraction.<xmlattr>.Lsbposbit");
			              TM::tm_parameter_ptr tm_param_ptr;
			              if (nb_words * word_length == nb_bits)
			              {
				              std::string type = boost::to_lower_copy(format_type) + "_parameter" + boost::lexical_cast<std::string>(nb_bits);
				              if (nb_bits > 8)
				              {
					              type = "big_endian_" + type;
				              }
				              tm_param_ptr = TM::create_base_tm_parameter(type, name);
			              }
			              else
			              {
				              std::string type = "bits" + boost::lexical_cast<std::string>(nb_words * word_length) + "_parameter" + boost::lexical_cast<std::string>(nb_bits);
				              if (nb_words * word_length > 8)
				              {
					              type = "big_endian_" + type;
				              }
				              tm_param_ptr = TM::create_bits_tm_parameter(type, name, lsb_pos_bit);
			              }
			              if (tm_param_ptr)
			              {
				              parameter_ptr param_ptr(new parameter(name));
				              imp_->parameter_manager_->add_object(name, param_ptr);
				              tm_param_ptr->connect_val_charged_signal(TM::val_charged_slot_t(
					              [param_ptr](TM::tm_parameter* tm_param)
					              {
						              param_ptr->set_value(tm_param->get_time(), tm_param->get_val());
					              }));
				              auto A0 = parameter_pt.second.get<double>("Conversion.<xmlattr>.A0");
				              auto A1 = parameter_pt.second.get<double>("Conversion.<xmlattr>.A1");
				              if (A0 != 0 || A1 != 1)
				              {
					              std::shared_ptr<linear_conversion> conversion(new linear_conversion(A1, A0));
					              tm_param_ptr->set_primary_conversion(boost::bind(&linear_conversion::convert, conversion, _1));
				              }
				              auto conv_type = parameter_pt.second.get<std::string>("Conversion.<xmlattr>.ConvType");
				              if (conv_type == "Liner")
				              {
					              auto B0 = parameter_pt.second.get<double>("Conversion.<xmlattr>.B0");
					              auto B1 = parameter_pt.second.get<double>("Conversion.<xmlattr>.B1");
					              std::shared_ptr<linear_conversion> conversion(new linear_conversion(B1, B0));
					              tm_param_ptr->set_secondary_conversion(boost::bind(&linear_conversion::convert, conversion, _1));
				              }
				              else if (conv_type == "Parabolic")
				              {
					              std::vector<double> coefficient;
					              for (size_t i = 0; i < 3; ++i)
					              {
						              auto B = parameter_pt.second.get<double>("Conversion.<xmlattr>.B" + boost::lexical_cast<std::string>(i));
						              coefficient.push_back(B);
					              }
					              std::shared_ptr<polynomial_conversion> conversion(new polynomial_conversion(coefficient));
					              tm_param_ptr->set_secondary_conversion(boost::bind(&polynomial_conversion::convert, conversion, _1));
				              }
				              else if (conv_type == "Polynomial")
				              {
					              std::vector<double> coefficient;
					              for (size_t i = 0; i < 9; ++i)
					              {
						              auto B = parameter_pt.second.get<double>("Conversion.<xmlattr>.B" + boost::lexical_cast<std::string>(i));
						              coefficient.push_back(B);
					              }
					              std::shared_ptr<polynomial_conversion> conversion(new polynomial_conversion(coefficient));
					              tm_param_ptr->set_secondary_conversion(boost::bind(&polynomial_conversion::convert, conversion, _1));
				              }

				              auto comm_type = parameter_pt.second.get<std::string>("Gernal.<xmlattr>.CommType");
				              auto major_pos = parameter_pt.second.get<uint32_t>("Gernal.<xmlattr>.MajorPos");
				              auto position = parameter_pt.second.get<uint32_t>("Gernal.<xmlattr>.Position");
				              auto recurence = parameter_pt.second.get<uint32_t>("Gernal.<xmlattr>.Recurence");
				              if (sync_location != "Begin")
				              {
					              position -= 1;
				              }
				              if (comm_type == "Com")
				              {
					              for (uint32_t i = 0; i < minor_frames; ++i)
					              {
						              imp_->irig_frame_->add_parameter(tm_param_ptr, i, position * bytes);
					              }
				              }
				              else if (comm_type == "SubCom")
				              {
					              for (uint32_t i = major_pos; i < minor_frames; i += recurence)
					              {
						              imp_->irig_frame_->add_parameter(tm_param_ptr, i, position * bytes);
					              }
				              }
				              else if (comm_type == "SuperCom")
				              {
					              for (uint32_t i = 0; i < minor_frames; ++i)
					              {
						              for (uint32_t j = 0; j < minor_frames; j += recurence)
						              {
							              imp_->irig_frame_->add_parameter(tm_param_ptr, i, (position + j) * bytes);
						              }
					              }
				              }
				              else if (comm_type == "Free")
				              {
					              auto positions = parameter_pt.second.get_child("Gernal");
					              std::for_each(positions.begin(), positions.end(),
					                            [=](ptree::value_type pos_pt)
					                            {
						                            if (pos_pt.first == "Pos")
						                            {
							                            auto free_major_pos = pos_pt.second.get<uint32_t>("<xmlattr>.MajorPos");
							                            auto free_minor_pos = pos_pt.second.get<uint32_t>("<xmlattr>.MinorPos");
							                            if (sync_location != "Begin")
							                            {
								                            free_minor_pos -= 1;
							                            }
							                            imp_->irig_frame_->add_parameter(tm_param_ptr, free_major_pos, free_minor_pos * bytes);
						                            }
					                            });
				              }
			              }
		              });
	}

	parameter_ptr test_context::get_parameter(const std::string& name)
	{
		return imp_->parameter_manager_->get_object(name);
	}

	std::vector<parameter_ptr> test_context::get_all_parameters()
	{
		return imp_->parameter_manager_->get_all_objects();
	}

	raw_data_ptr test_context::get_raw_data(const std::string& name)
	{
		return imp_->raw_data_manager_->get_object(name);
	}

	std::vector<raw_data_ptr> test_context::get_all_raw_datas()
	{
		return imp_->raw_data_manager_->get_all_objects();
	}
}
