#ifndef STRING_UTILITIES_HPP
#define STRING_UTILITIES_HPP

#if defined(_MSC_VER)
# pragma once
#endif
#define BOOST_ALL_DYN_LINK
#include <string>

namespace utilities
{
	__declspec(dllexport) std::string get_current_time_string();
}

#endif // STRING_UTILITIES_HPP

