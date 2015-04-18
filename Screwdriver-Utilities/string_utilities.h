#ifndef STRING_UTILITIES_HPP
#define STRING_UTILITIES_HPP

#if defined(_MSC_VER)
# pragma once
#endif
#define BOOST_ALL_DYN_LINK
#include <string>
#include <vector>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/lexical_cast.hpp>

const std::string SEPARATOR = "[[:space:]]+|,|£¬|;|£»|¡¢|\r\n";

namespace utilities
{
	__declspec(dllexport) std::string get_current_time_string();
	__declspec(dllexport) std::vector<std::string> separate_string_by_separator(const std::string& str, const std::string& separator = SEPARATOR);

	template <typename IntegerType>
	IntegerType string_to_uinteger_base16(const std::string& str)
	{
		try
		{
			return boost::numeric_cast<IntegerType>(stoull(str, nullptr, 16));
		}
		catch (boost::bad_numeric_cast&)
		{
			return IntegerType();
		}
	}

	template <typename IntegerType>
	std::vector<IntegerType> string_to_integer_array(const std::string& str, const std::string& separator = SEPARATOR)
	{
		std::vector<IntegerType> data;
		if (!str.empty())
		{
			auto dataStringList = separate_string_by_separator(str, separator);
			std::transform(dataStringList.begin(), dataStringList.end(), back_inserter(data),
			               [](const std::string& data_str)->IntegerType

			               
			               {
				               try
				               {
					               return boost::lexical_cast<IntegerType>(data_str);
				               }
				               catch (boost::bad_lexical_cast&)
				               {
					               return IntegerType();
				               }
			               }
			);
		}
		return data;
	}

	template <typename IntegerType>
	std::vector<IntegerType> string_to_uinteger_array_base16(const std::string& str, const std::string& separator = SEPARATOR)
	{
		std::vector<IntegerType> data;
		if (!str.empty())
		{
			auto dataStringList = separate_string_by_separator(str, separator);
			std::transform(dataStringList.begin(), dataStringList.end(), back_inserter(data),
			               [](const std::string& data_str)
			               {
				               return string_to_uinteger_base16<IntegerType>(data_str);
			               }
			);
		}
		return data;
	}
}

#endif // STRING_UTILITIES_HPP

