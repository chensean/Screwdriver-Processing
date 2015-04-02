#include "string_utilities.h"
#include <boost/config/warning_disable.hpp>
# pragma warning( push )
# pragma warning(disable:4005)
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
# pragma warning( pop )
#include <boost/algorithm/string/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/regex_fwd.hpp>

using namespace boost::posix_time;
using namespace boost::gregorian;


const unsigned int TIME_STRING_LENGTH = 23;

namespace utilities
{
	std::string get_current_time_string()
	{
		using namespace boost;
		ptime currentTime = microsec_clock::local_time();
		auto timeString = to_iso_extended_string(currentTime);
		replace_all(timeString, "T", " ");
		if (timeString.length() < TIME_STRING_LENGTH)
		{
			timeString += ".000";
		}
		timeString = timeString.substr(0, TIME_STRING_LENGTH);
		return timeString;
	}
	std::vector<std::string> separate_string_by_separator(const std::string& str,const std::string& separator/*=SEPARATOR*/)
	{
		std::vector<std::string> strList;
		boost::split_regex(strList,str,boost::regex(separator));
		strList.erase(std::remove(strList.begin(),strList.end(),""),strList.end());
		return strList;		
	}
}
