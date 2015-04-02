#ifndef TM_PARAMETER_MANAGER_HPP
#define TM_PARAMETER_MANAGER_HPP

#if defined(_MSC_VER)
# pragma once
#endif

#include <boost/unordered_map.hpp>
#include "tm_parameter.h"

namespace TM
{
	class __declspec(dllexport) tm_parameter_manager
		:private boost::noncopyable
	{
	public:
		tm_parameter_manager(void);
		~tm_parameter_manager(void);
		tm_parameter_ptr get_tm_parameter(const std::string& name);
		void add_tm_parameter(const std::string& name, const tm_parameter_ptr& param);
		void remove_tm_parameter(const std::string& name);
	private:
		typedef boost::unordered_map<std::string, tm_parameter_ptr> parameter_container_t;
		parameter_container_t parameter_container_;
	};
}

#endif // TM_PARAMETER_MANAGER_HPP

