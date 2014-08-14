#include "tm_parameter_manager.h"

namespace TM
{
	tm_parameter_manager::tm_parameter_manager(void)
	{
	}


	tm_parameter_manager::~tm_parameter_manager(void)
	{
	}

	TM::tm_parameter_ptr tm_parameter_manager::get_tm_parameter( const std::string& name )
	{
		return parameter_container_.find(name)!=parameter_container_.end() ? parameter_container_[name] : tm_parameter_ptr(nullptr);
	}

	void tm_parameter_manager::add_tm_parameter( const std::string& name,const tm_parameter_ptr& param )
	{
		parameter_container_[name]=param;
	}

	void tm_parameter_manager::remove_tm_parameter( const std::string& name )
	{
		parameter_container_.erase(name);
	}

}