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
		void config_irig_frame(const boost::property_tree::ptree& pt);
		void create_rtr_tm_client(const std::string& ip);
	private:
		struct test_context_imp_t;
		boost::shared_ptr<test_context_imp_t> imp_;
	};

	typedef Loki::SingletonHolder<test_context, Loki::CreateUsingNew, Loki::DeletableSingleton> single_test_context;
}
#endif // TEST_CONTEXT_HPP

