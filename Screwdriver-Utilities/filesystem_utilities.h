#ifndef STRING_UTILITIES_HPP
#define STRING_UTILITIES_HPP

#if defined(_MSC_VER)
# pragma once
#endif
#define BOOST_ALL_DYN_LINK
# pragma warning( push )
#pragma warning(disable:4005)
#define BOOST_FILESYSTEM_NO_DEPRECATED
#include <boost/filesystem.hpp>
# pragma warning( pop )
#include <boost/optional.hpp>
namespace fs = boost::filesystem;

namespace utilities
{
	__declspec(dllexport) boost::optional<fs::path> get_path(const fs::path& current_path, const std::string& sub_dir);
	__declspec(dllexport) boost::optional<fs::path> get_directorty_path(const std::string& sub_dir);
}

#endif // STRING_UTILITIES_HPP

