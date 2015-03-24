#include "test_context.h"
#include "rtr_tm_client.h"
#include "rtr_monitor_client.h"
#include "tm_parameter_manager.h"

namespace screwdriver
{
	struct test_context::test_context_imp_t
	{
		boost::shared_ptr<rtr_tm_client> rtr_tm_client_;
		boost::shared_ptr<rtr_monitor_client> rtr_monitor_client_;
		boost::shared_ptr<TM::tm_parameter_manager> tm_parameter_manager_;
	};

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

	test_context::test_context(void)
		:imp_(new test_context_imp_t)
	{
	}


	test_context::~test_context(void)
	{
	}
}
