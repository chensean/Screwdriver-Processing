#define SCREWDRIVER_INFRASTRUCTURE_COMPILATION
#include "infrastructure_global.h"
#include "infrastructure_manager.h"

namespace screwdriver
{
	parameter_ptr get_parameter(const std::string& name)
	{
		return single_infrastructure_manager::Instance().get_parameter(name);
	}

	void add_parameter(const std::string& name, const parameter_ptr& obj_ptr)
	{
		single_infrastructure_manager::Instance().add_parameter(name, obj_ptr);
	}

	void remove_parameter(const std::string& name)
	{
		single_infrastructure_manager::Instance().remove_parameter(name);
	}

	std::vector<parameter_ptr> get_all_parameters()
	{
		return single_infrastructure_manager::Instance().get_all_parameters();
	}

	raw_data_ptr get_raw_data(const std::string& name)
	{
		return single_infrastructure_manager::Instance().get_raw_data(name);
	}

	void add_raw_data(const std::string& name, const raw_data_ptr& obj_ptr)
	{
		single_infrastructure_manager::Instance().add_raw_data(name, obj_ptr);
	}

	void remove_raw_data(const std::string& name)
	{
		single_infrastructure_manager::Instance().remove_raw_data(name);
	}

	std::vector<raw_data_ptr> get_all_raw_datas()
	{
		return single_infrastructure_manager::Instance().get_all_raw_datas();
	}

	void start_task()
	{
		single_infrastructure_manager::Instance().start_task();
	}

	void stop_task()
	{
		single_infrastructure_manager::Instance().stop_task();
	}
}
