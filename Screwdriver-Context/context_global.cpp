#include "context_global.h"
#include "test_context.h"
#include "infrastructure_global.h"

namespace screwdriver
{
	void start_rtr_tm()
	{
		start_task();
		single_test_context::Instance().start_rtr_tm();
	}

	void stop_rtr_tm()
	{
		single_test_context::Instance().stop_rtr_tm();
		stop_task();
	}

	void start_save_file()
	{
		single_test_context::Instance().start_save_file();
	}

	void stop_save_file()
	{
		single_test_context::Instance().stop_save_file();
	}

	void set_save_folder(const std::string& folder)
	{
		single_test_context::Instance().set_save_folder(folder);
	}

	void load_irig_config(const std::string& file_name)
	{
		single_test_context::Instance().load_irig_config(file_name);
	}
}
