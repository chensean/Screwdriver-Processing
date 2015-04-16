#include "tm_parameter_manager.h"
#include <boost/unordered_map.hpp>

namespace TM
{
	typedef boost::unordered_map<std::string, tm_parameter_ptr> parameter_container_t;

	struct tm_parameter_manager::tm_parameter_manager_imp_t
	{
		parameter_container_t parameter_container_;
	};

	tm_parameter_manager::tm_parameter_manager(void)
		:imp_(new tm_parameter_manager_imp_t)
	{
	}


	tm_parameter_manager::~tm_parameter_manager(void)
	{
	}

	TM::tm_parameter_ptr tm_parameter_manager::get_tm_parameter(const std::string& name)
	{
		return imp_->parameter_container_.find(name) != imp_->parameter_container_.end() ? imp_->parameter_container_[name] : tm_parameter_ptr(nullptr);
	}

	void tm_parameter_manager::add_tm_parameter(const std::string& name, const tm_parameter_ptr& param)
	{
		imp_->parameter_container_[name] = param;
	}

	void tm_parameter_manager::remove_tm_parameter(const std::string& name)
	{
		imp_->parameter_container_.erase(name);
	}
}
