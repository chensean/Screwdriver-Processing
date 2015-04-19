#ifndef TEST_CONTEXT_HPP
#define TEST_CONTEXT_HPP

#if defined(_MSC_VER)
# pragma once
#endif

#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <loki/Singleton.h>
#include <boost/property_tree/exceptions.hpp>
#include <boost/property_tree/ptree.hpp>
#include "tm_parameter.h"

namespace screwdriver
{
	class test_context
		:private boost::noncopyable
	{
	public:
		friend Loki::CreateUsingNew<test_context>;
		void start_rtr_tm();
		void stop_rtr_tm();
		void start_save_file();
		void stop_save_file();
		void set_save_folder(const std::string& folder);
		void load_irig_config(const std::string& file_name);
	private:
		test_context(void);
		~test_context(void);
		void create_raw_data(const std::string& name);
		void config_irig_frame(const boost::property_tree::ptree& pt);
		void create_rtr_tm_client(const std::string& ip, uint16_t port, int tm_channel);
		void config_data_source(boost::property_tree::ptree& pt);
		TM::tm_parameter_ptr create_tm_parameter(const boost::property_tree::ptree::value_type& parameter_pt, uint32_t word_length);
		void create_parameter_proxy(const TM::tm_parameter_ptr& tm_param_ptr);
		void set_parameter_conversion(const boost::property_tree::ptree::value_type& parameter_pt, const TM::tm_parameter_ptr& tm_param_ptr);
		void config_frame_parameter_position(const boost::property_tree::ptree::value_type& parameter_pt, const TM::tm_parameter_ptr& tm_param_ptr, uint32_t minor_frames, uint32_t bytes, const std::string& sync_location);
		void config_frame_parameters(boost::property_tree::ptree& pt, uint32_t word_length, uint32_t minor_frames, const std::string& sync_location);
		void create_embedded_messages(boost::property_tree::ptree& pt);
		void config_embedded_messages_position(boost::property_tree::ptree& pt, uint32_t word_length, const std::string& sync_location);
		std::vector<uint8_t> get_sync_pattern_array(const std::string& sync_pattern_string, uint32_t sync_pattern_bytes);
	private:
		struct test_context_imp_t;
		boost::shared_ptr<test_context_imp_t> imp_;
	};

	typedef Loki::SingletonHolder<test_context, Loki::CreateUsingNew, Loki::DeletableSingleton> single_test_context;
}
#endif // TEST_CONTEXT_HPP

