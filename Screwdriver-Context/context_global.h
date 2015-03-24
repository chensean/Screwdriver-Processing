#ifndef CONTEXT_GLOBAL_HPP
#define CONTEXT_GLOBAL_HPP

#if defined(_MSC_VER)
# pragma once
#endif

#include <string>

namespace screwdriver
{
	__declspec(dllexport) void create_rtr_tm_client(const std::string& ip, const std::string& folder);
	__declspec(dllexport) void start_rtr_tm();
	__declspec(dllexport) void stop_rtr_tm();
}

#endif // CONTEXT_GLOBAL_HPP

