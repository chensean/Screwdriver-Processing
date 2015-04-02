#include "context_global.h"
#include "test_context.h"

namespace screwdriver
{
	void create_rtr_tm_client(const std::string& ip, const std::string& folder)
	{
		single_test_context::Instance().create_rtr_tm_client(ip,folder);
	}

	void start_rtr_tm()
	{
		single_test_context::Instance().start_rtr_tm();
	}

	void stop_rtr_tm()
	{
		single_test_context::Instance().stop_rtr_tm();
	}

	void load_irig_config(const std::string& file_name)
	{
		single_test_context::Instance().load_irig_config(file_name);
	}
}
