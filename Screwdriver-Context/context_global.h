#ifndef CONTEXT_GLOBAL_HPP
#define CONTEXT_GLOBAL_HPP

#if defined(_MSC_VER)
# pragma once
#endif

#include <string>
#include "parameter.h"

namespace screwdriver
{
	__declspec(dllexport) void create_rtr_tm_client(const std::string& ip, const std::string& folder);
	__declspec(dllexport) void start_rtr_tm();
	__declspec(dllexport) void stop_rtr_tm();
	__declspec(dllexport) void load_irig_config(const std::string& file_name);
	__declspec(dllexport) parameter_ptr get_parameter(const std::string& name);
	__declspec(dllexport) std::vector<parameter_ptr> get_all_parameters();
}

#endif // CONTEXT_GLOBAL_HPP

