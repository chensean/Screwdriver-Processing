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
#include "parameter.h"

namespace screwdriver
{
	class test_context
		:private boost::noncopyable
	{
	public:
		friend Loki::CreateUsingNew<test_context>;
		void create_rtr_tm_client(const std::string& ip, const std::string& folder);
		void start_rtr_tm();
		void stop_rtr_tm();
		void load_irig_config(const std::string& file_name);
		parameter_ptr get_parameter(const std::string& name);
		std::vector<parameter_ptr> get_all_parameters();
	private:
		test_context(void);
		~test_context(void);
		void config_irig_frame(const boost::property_tree::ptree& pt);
	private:
		struct test_context_imp_t;
		boost::shared_ptr<test_context_imp_t> imp_;
	};

	typedef Loki::SingletonHolder<test_context, Loki::CreateUsingNew, Loki::DeletableSingleton> single_test_context;
}
#endif // TEST_CONTEXT_HPP

