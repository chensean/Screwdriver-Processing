#ifndef PARAMETER_MANAGER_HPP
#define PARAMETER_MANAGER_HPP

#if defined(_MSC_VER)
# pragma once
#endif
#include "parameter.h"

namespace screwdriver
{
	class __declspec(dllexport) parameter_manager
		:private boost::noncopyable
	{
	public:
		parameter_manager(void);
		~parameter_manager(void);
		parameter_ptr get_parameter(const std::string& name);
		void add_parameter(const std::string& name, const parameter_ptr& param);
		void remove_parameter(const std::string& name);
		std::vector<parameter_ptr> get_all_parameters();
	private:
		struct parameter_manager_imp_t;
		boost::shared_ptr<parameter_manager_imp_t> imp_;
	};
}

#endif // PARAMETER_MANAGER_HPP

