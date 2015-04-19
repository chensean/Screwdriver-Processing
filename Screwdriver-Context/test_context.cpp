#include "test_context.h"
#include <iostream>
#define SCREWDRIVER_INFRASTRUCTURE_LINK_DYNAMIC
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
#include "parameter.h"
#include "infrastructure_global.h"
#include "save_data_file.h"
#include "code0_time.h"

namespace screwdriver
{
	const std::string RTR_DATA_STRING = "rtr_data";
	const std::string MINOR_FRAME_DATA_STRING = "minor_frame_data";
	const uint32_t RTR_TIME_OFFSET = 3;
	const uint32_t RTR_FRAME_OFFSET = 14;
	typedef object_manager<TM::tm_parameter> tm_parameter_manager;
	typedef object_manager<TM::embedded_message> embedded_message_manager;
	typedef object_manager<save_data_file> save_data_file_manager;

	struct test_context::test_context_imp_t
	{
		test_context_imp_t()
			:tm_parameter_manager_(new tm_parameter_manager),
			 embedded_message_manager_(new embedded_message_manager),
			 save_data_file_manager_(new save_data_file_manager),
			 tm_time_(new TM::code0_time)
		{
		}

		boost::shared_ptr<tm_parameter_manager> tm_parameter_manager_;
		boost::shared_ptr<embedded_message_manager> embedded_message_manager_;
		boost::shared_ptr<save_data_file_manager> save_data_file_manager_;
		boost::shared_ptr<rtr_tm_client> rtr_tm_client_;
		boost::shared_ptr<rtr_monitor_client> rtr_monitor_client_;
		boost::shared_ptr<irig_frame> irig_frame_;
		TM::tm_time_ptr tm_time_;
		std::string save_folder_;
	};

	test_context::test_context(void)
		:imp_(new test_context_imp_t)
	{
	}

	test_context::~test_context(void)
	{
	}

	void test_context::create_raw_data(const std::string& name)
	{
		raw_data_ptr raw_data_p(new raw_data(name));
		add_raw_data(raw_data_p->raw_data::get_name(), raw_data_p);
		save_data_file_ptr save_data_file_p(new save_data_file(imp_->save_folder_, name));
		imp_->save_data_file_manager_->add_object(name, save_data_file_p);
		raw_data_p->raw_data::connect_data_charged_signal(raw_data_charged_slot_t(&save_data_file::receive, save_data_file_p, _1));
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

	void test_context::start_save_file()
	{
		auto save_files = imp_->save_data_file_manager_->get_all_objects();
		std::for_each(save_files.begin(), save_files.end(), boost::bind(&save_data_file::start, _1));
	}


	void test_context::stop_save_file()
	{
		auto save_files = imp_->save_data_file_manager_->get_all_objects();
		std::for_each(save_files.begin(), save_files.end(), boost::bind(&save_data_file::stop, _1));
	}

	void test_context::set_save_folder(const std::string& folder)
	{
		imp_->save_folder_ = folder;
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

		config_data_source(pt);

		config_irig_frame(pt);

		if (!imp_->irig_frame_)
		{
			return;
		}
		auto irig_pt = pt.get_child("CFG.ACQ_SYS.SOURCE.IRIG.IRIGFrameStruc");
		auto word_length = irig_pt.get<uint32_t>("<xmlattr>.WordLength");
		auto minor_frames = irig_pt.get<uint32_t>("<xmlattr>.MinorFrames");
		auto sync_location = irig_pt.get<std::string>("<xmlattr>.SyncLocation");

		config_frame_parameters(pt, word_length, minor_frames, sync_location);

		create_embedded_messages(pt);

		config_embedded_messages_position(pt, word_length, sync_location);
	}

	std::vector<uint8_t> test_context::get_sync_pattern_array(const std::string& sync_pattern_string, uint32_t sync_pattern_bytes)
	{
		auto sync_pattern = utilities::string_to_uinteger_base16<uint32_t>(sync_pattern_string);
		auto sync_pattern_array = utilities::big_endian_val_to_array(sync_pattern);
		if (sync_pattern_array.size() > sync_pattern_bytes)
		{
			sync_pattern_array.erase(sync_pattern_array.begin(), sync_pattern_array.begin() + (sync_pattern_array.size() - sync_pattern_bytes));
		}
		return sync_pattern_array;
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
		auto sync_pattern_array = get_sync_pattern_array(sync_pattern_string, bytes);
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
		auto minor_frame_bytes = minor_frame_words * bytes;
		imp_->irig_frame_ = boost::make_shared<irig_frame>(bit_rate, minor_frame_bytes, minor_frames, get_minor_frame_id_fun);

		if (imp_->tm_time_)
		{
			imp_->tm_time_->connect_time_charged_signal(TM::time_charged_slot_t(&irig_frame::set_minor_frame_time, imp_->irig_frame_, _1));
		}
		raw_data_ptr rtr_raw_data_p = get_raw_data(RTR_DATA_STRING);
		auto frame_offset = boost::numeric_cast<uint32_t>(RTR_FRAME_OFFSET * sizeof(int));
		if (rtr_raw_data_p)
		{
			rtr_raw_data_p->connect_data_charged_signal(raw_data_charged_slot_t(&irig_frame::parse_frame, imp_->irig_frame_, _1, frame_offset));
		}

		create_raw_data(MINOR_FRAME_DATA_STRING);
		auto minor_frame_raw_data_p = get_raw_data(MINOR_FRAME_DATA_STRING);
		if (minor_frame_raw_data_p)
		{
			rtr_raw_data_p->connect_data_charged_signal(raw_data_charged_slot_t(
				[=](const data_ptr& data)
				{
					data_ptr minor_frame_data_ptr(new std::vector<uint8_t>(data->begin() + frame_offset, data->begin() + frame_offset + minor_frame_bytes));
					minor_frame_raw_data_p->set_data(minor_frame_data_ptr);
				}));
		}
	}

	void test_context::create_rtr_tm_client(const std::string& ip, uint16_t port, int tm_channel)
	{
		create_raw_data(RTR_DATA_STRING);
		auto raw_data_p = get_raw_data(RTR_DATA_STRING);
		if (raw_data_p)
		{
			imp_->rtr_tm_client_ = boost::make_shared<rtr_tm_client>(ip, tm_channel, port, boost::bind(&raw_data::set_data, raw_data_p, _1));
			if (imp_->tm_time_)
			{
				auto time_offset = boost::numeric_cast<uint32_t>(RTR_TIME_OFFSET * sizeof(int));
				raw_data_p->connect_data_charged_signal(raw_data_charged_slot_t([=](const data_ptr& data_ptr)
					{
						imp_->tm_time_->read_form_buffer(*data_ptr, time_offset);
					}));
			}
		}
	}

	void test_context::config_data_source(boost::property_tree::ptree& pt)
	{
		auto source_pt = pt.get_child("CFG.ACQ_SYS.SOURCE");
		auto type = source_pt.get<std::string>("<xmlattr>.TYPE");
		if (type == "CORTEX_TM")
		{
			auto ip = source_pt.get<std::string>("<xmlattr>.IPAddr");
			auto port = source_pt.get<uint16_t>("<xmlattr>.Port");
			auto channel = source_pt.get<int>("<xmlattr>.TMChannel");
			create_rtr_tm_client(ip, port, channel);
		}
	}

	TM::tm_parameter_ptr test_context::create_tm_parameter(const boost::property_tree::ptree::value_type& parameter_pt, uint32_t word_length)
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
		return tm_param_ptr;
	}

	void test_context::create_parameter_proxy(const TM::tm_parameter_ptr& tm_param_ptr)
	{
		parameter_ptr param_ptr(new parameter(tm_param_ptr->get_name()));
		add_parameter(param_ptr->get_name(), param_ptr);
		tm_param_ptr->connect_val_charged_signal(TM::val_charged_slot_t(
			[param_ptr](TM::tm_parameter* tm_param)
			{
				param_ptr->set_value(tm_param->get_time(), tm_param->get_val());
			}));
	}

	void test_context::set_parameter_conversion(const boost::property_tree::ptree::value_type& parameter_pt, const TM::tm_parameter_ptr& tm_param_ptr)
	{
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
	}

	void test_context::config_frame_parameter_position(const boost::property_tree::ptree::value_type& parameter_pt, const TM::tm_parameter_ptr& tm_param_ptr, uint32_t minor_frames, uint32_t bytes, const std::string& sync_location)
	{
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
			              [=](boost::property_tree::ptree::value_type pos_pt)
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


	void test_context::config_frame_parameters(boost::property_tree::ptree& pt, uint32_t word_length, uint32_t minor_frames, const std::string& sync_location)
	{
		auto parameters_pt = pt.get_child("CFG.ACQ_SYS.SOURCE.Parameters");
		auto bytes = word_length / std::numeric_limits<uint8_t>::digits;
		std::for_each(parameters_pt.begin(), parameters_pt.end(),
		              [=](boost::property_tree::ptree::value_type parameter_pt)
		              {
			              auto tm_param_ptr = create_tm_parameter(parameter_pt, word_length);
			              if (tm_param_ptr)
			              {
				              create_parameter_proxy(tm_param_ptr);
				              set_parameter_conversion(parameter_pt, tm_param_ptr);
				              config_frame_parameter_position(parameter_pt, tm_param_ptr, minor_frames, bytes, sync_location);
			              }
		              });
	}

	void test_context::create_embedded_messages(boost::property_tree::ptree& pt)
	{
		auto source_pt = pt.get_child("CFG.ACQ_SYS.SOURCE");
		std::for_each(source_pt.begin(), source_pt.end(),
		              [=](boost::property_tree::ptree::value_type embedded_pt)
		              {
			              if (embedded_pt.first == "SOURCE")
			              {
				              auto type = embedded_pt.second.get<std::string>("<xmlattr>.TYPE");
				              if (type == "EMBEDDED_MSG")
				              {
					              auto name = embedded_pt.second.get<std::string>("<xmlattr>.NAME");
					              auto EmLength = embedded_pt.second.get<uint32_t>("Definition.<xmlattr>.EmLength");
					              auto SyncBits = embedded_pt.second.get<uint32_t>("Definition.<xmlattr>.SyncBits");
					              auto sync_pattern_string = embedded_pt.second.get<std::string>("Definition.<xmlattr>.SyncPattern");
					              auto sync_pattern_bytes = SyncBits / std::numeric_limits<uint8_t>::digits;
					              auto sync_pattern_array = get_sync_pattern_array(sync_pattern_string, sync_pattern_bytes);
					              TM::embedded_message_ptr embedded_message_ptr(new TM::embedded_message(EmLength, sync_pattern_array));
					              imp_->embedded_message_manager_->add_object(name, embedded_message_ptr);

					              auto parameters_pt = embedded_pt.second.get_child("Parameters");

					              std::for_each(parameters_pt.begin(), parameters_pt.end(),
					                            [=](boost::property_tree::ptree::value_type parameter_pt)
					                            {
						                            auto tm_param_ptr = create_tm_parameter(parameter_pt, std::numeric_limits<uint8_t>::digits);
						                            if (tm_param_ptr)
						                            {
							                            create_parameter_proxy(tm_param_ptr);
							                            set_parameter_conversion(parameter_pt, tm_param_ptr);
							                            auto position = parameter_pt.second.get<uint32_t>("Gernal.<xmlattr>.Position");
							                            embedded_message_ptr->get_data_buffer()->add_parameter(tm_param_ptr, position);
						                            }
					                            });
				              }
			              }
		              });
	}

	void test_context::config_embedded_messages_position(boost::property_tree::ptree& pt, uint32_t word_length, const std::string& sync_location)
	{
		auto bytes = word_length / std::numeric_limits<uint8_t>::digits;
		auto embeddeds_pt = pt.get_child("CFG.ACQ_SYS.SOURCE.Embedded");

		std::for_each(embeddeds_pt.begin(), embeddeds_pt.end(),
		              [=](boost::property_tree::ptree::value_type embedded_pt)
		              {
			              auto name = embedded_pt.second.get<std::string>("<xmlattr>.NAME");
			              auto embedded_message_ptr = imp_->embedded_message_manager_->get_object(name);
			              std::for_each(embedded_pt.second.begin(), embedded_pt.second.end(),
			                            [=](boost::property_tree::ptree::value_type pos_pt)
			                            {
				                            if (pos_pt.first == "Pos")
				                            {
					                            auto free_major_pos = pos_pt.second.get<uint32_t>("<xmlattr>.MajorPos");
					                            auto free_minor_pos = pos_pt.second.get<uint32_t>("<xmlattr>.MinorPos");
					                            if (sync_location != "Begin")
					                            {
						                            free_minor_pos -= 1;
					                            }
					                            imp_->irig_frame_->add_embedded_message(embedded_message_ptr, free_major_pos, free_minor_pos * bytes, bytes);
				                            }
			                            });
		              });
	}
}
