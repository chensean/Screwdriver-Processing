#ifndef INFRASTRUCTURE_MANAGER_HPP
#define INFRASTRUCTURE_MANAGER_HPP

#if defined(_MSC_VER)
# pragma once
#endif

#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <loki/Singleton.h>
#include "parameter.h"
#include "raw_data.h"

namespace screwdriver
{
	typedef boost::function<void()> task_type;

	class infrastructure_manager
		:private boost::noncopyable
	{
	public:
		friend Loki::CreateUsingNew<infrastructure_manager>;
		parameter_ptr get_parameter(const std::string& name);
		void add_parameter(const std::string& name, const parameter_ptr& obj_ptr);
		void remove_parameter(const std::string& name);
		std::vector<parameter_ptr> get_all_parameters();
		raw_data_ptr get_raw_data(const std::string& name);
		void add_raw_data(const std::string& name, const raw_data_ptr& obj_ptr);
		void remove_raw_data(const std::string& name);
		std::vector<raw_data_ptr> get_all_raw_datas();
		void start_task();
		void stop_task();
		void push_task_to_parameter_work_queue(const task_type& task);
		void push_task_to_raw_data_work_queue(const task_type& task);
	private:
		infrastructure_manager(void);
		~infrastructure_manager(void);
	private:
		struct infrastructure_manager_imp_t;
		boost::shared_ptr<infrastructure_manager_imp_t> imp_;
	};

	typedef Loki::SingletonHolder<infrastructure_manager, Loki::CreateUsingNew, Loki::DeletableSingleton> single_infrastructure_manager;
}
#endif // INFRASTRUCTURE_MANAGER_HPP

