#ifndef INFRASTRUCTURE_GLOBAL_HPP
#define INFRASTRUCTURE_GLOBAL_HPP

#if defined(_MSC_VER)
# pragma once
#endif

#include <string>
#include "Screwdriver-Infrastructure.h"
#include "parameter.h"
#include "raw_data.h"

namespace screwdriver
{
	SCREWDRIVER_INFRASTRUCTURE_API parameter_ptr get_parameter(const std::string& name);
	SCREWDRIVER_INFRASTRUCTURE_API void add_parameter(const std::string& name, const parameter_ptr& obj_ptr);
	SCREWDRIVER_INFRASTRUCTURE_API void remove_parameter(const std::string& name);
	SCREWDRIVER_INFRASTRUCTURE_API std::vector<parameter_ptr> get_all_parameters();
	SCREWDRIVER_INFRASTRUCTURE_API raw_data_ptr get_raw_data(const std::string& name);
	SCREWDRIVER_INFRASTRUCTURE_API void add_raw_data(const std::string& name, const raw_data_ptr& obj_ptr);
	SCREWDRIVER_INFRASTRUCTURE_API void remove_raw_data(const std::string& name);
	SCREWDRIVER_INFRASTRUCTURE_API std::vector<raw_data_ptr> get_all_raw_datas();
}

#endif // INFRASTRUCTURE_GLOBAL_HPP

