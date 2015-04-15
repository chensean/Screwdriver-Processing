#include "parameter_manager.h"
#include <boost/unordered_map.hpp>

namespace screwdriver
{
	typedef boost::unordered_map<std::string, parameter_ptr> parameter_container_t;
		
	struct parameter_manager::parameter_manager_imp_t
	{
		parameter_container_t parameter_container_;
	};

	parameter_manager::parameter_manager(void)
		:imp_(new parameter_manager_imp_t)
	{
	}


	parameter_manager::~parameter_manager(void)
	{
	}

	parameter_ptr parameter_manager::get_parameter(const std::string& name)
	{
		return imp_->parameter_container_.find(name) != imp_->parameter_container_.end() ? imp_->parameter_container_[name] : parameter_ptr(nullptr);
	}

	void parameter_manager::add_parameter(const std::string& name, const parameter_ptr& param)
	{
		imp_->parameter_container_[name] = param;
	}

	void parameter_manager::remove_parameter(const std::string& name)
	{
		imp_->parameter_container_.erase(name);
	}

	std::vector<parameter_ptr> parameter_manager::get_all_parameters()
	{
		std::vector<parameter_ptr> parameters(imp_->parameter_container_.size());
		std::transform(imp_->parameter_container_.begin(),imp_->parameter_container_.end(),parameters.begin()
			,[](const std::pair<std::string, parameter_ptr>& parameter_pair)
		{
			return parameter_pair.second;
		});
		return parameters;
	}
}
