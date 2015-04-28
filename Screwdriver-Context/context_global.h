#ifndef CONTEXT_GLOBAL_HPP
#define CONTEXT_GLOBAL_HPP

#if defined(_MSC_VER)
# pragma once
#endif

#include <string>

namespace screwdriver
{
	__declspec(dllexport) bool is_running();
	__declspec(dllexport) bool is_saving();
	__declspec(dllexport) bool connect_rtr_tm();
	__declspec(dllexport) bool disconnect_rtr_tm();
	__declspec(dllexport) void start_rtr_tm();
	__declspec(dllexport) void stop_rtr_tm();
	__declspec(dllexport) void start_rtr_monitor();
	__declspec(dllexport) void stop_rtr_monitor();
	__declspec(dllexport) void start_save_file(const std::string& folder);
	__declspec(dllexport) void stop_save_file();
	__declspec(dllexport) void load_irig_config(const std::string& file_name);
}

#endif // CONTEXT_GLOBAL_HPP

