#include "context_global.h"
#include "test_context.h"
#include "infrastructure_global.h"

namespace screwdriver
{
	bool is_running()
	{
		return single_test_context::Instance().is_running();
	}

	bool is_saving()
	{
		return single_test_context::Instance().is_saving();
	}

	bool connect_rtr_tm()
	{
		return single_test_context::Instance().connect_rtr_tm();
	}

	bool disconnect_rtr_tm()
	{
		return single_test_context::Instance().disconnect_rtr_tm();
	}

	void start_rtr_tm()
	{
		single_test_context::Instance().start_rtr_tm();
	}

	void stop_rtr_tm()
	{
		single_test_context::Instance().stop_rtr_tm();
	}

	void start_rtr_monitor()
	{
		single_test_context::Instance().start_rtr_monitor();
	}

	void stop_rtr_monitor()
	{
		single_test_context::Instance().stop_rtr_monitor();
	}

	void start_save_file(const std::string& folder)
	{
		single_test_context::Instance().start_save_file(folder);
	}

	void stop_save_file()
	{
		single_test_context::Instance().stop_save_file();
	}

	void load_irig_config(const std::string& file_name)
	{
		single_test_context::Instance().load_irig_config(file_name);
	}
}
