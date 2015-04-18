#include "infrastructure_manager.h"
#include "work_queue.hpp"
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include "object_manager.h"

namespace screwdriver
{
	typedef object_manager<parameter> parameter_manager;
	typedef object_manager<raw_data> raw_data_manager;


	void run_while_not_stopped(work_queue& queue)
	{
		work_queue::task_type task;
		while (task = queue.pop_task())
		{
			task();
		}
	}

	struct infrastructure_manager::infrastructure_manager_imp_t
	{
		infrastructure_manager_imp_t()
			:parameter_manager_(new parameter_manager),
			 raw_data_manager_(new raw_data_manager)
		{
		}

		boost::shared_ptr<parameter_manager> parameter_manager_;
		boost::shared_ptr<raw_data_manager> raw_data_manager_;
		work_queue parameter_work_queue_;
		work_queue raw_data_work_queue_;
	};

	infrastructure_manager::infrastructure_manager(void)
		:imp_(new infrastructure_manager_imp_t)
	{
	}

	infrastructure_manager::~infrastructure_manager(void)
	{
	}

	parameter_ptr infrastructure_manager::get_parameter(const std::string& name)
	{
		return imp_->parameter_manager_->get_object(name);
	}

	void infrastructure_manager::add_parameter(const std::string& name, const parameter_ptr& obj_ptr)
	{
		imp_->parameter_manager_->add_object(name, obj_ptr);
	}

	void infrastructure_manager::remove_parameter(const std::string& name)
	{
		imp_->parameter_manager_->remove_object(name);
	}

	std::vector<parameter_ptr> infrastructure_manager::get_all_parameters()
	{
		return imp_->parameter_manager_->get_all_objects();
	}

	raw_data_ptr infrastructure_manager::get_raw_data(const std::string& name)
	{
		return imp_->raw_data_manager_->get_object(name);
	}

	void infrastructure_manager::add_raw_data(const std::string& name, const raw_data_ptr& obj_ptr)
	{
		imp_->raw_data_manager_->add_object(name, obj_ptr);
	}

	void infrastructure_manager::remove_raw_data(const std::string& name)
	{
		imp_->raw_data_manager_->remove_object(name);
	}

	std::vector<raw_data_ptr> infrastructure_manager::get_all_raw_datas()
	{
		return imp_->raw_data_manager_->get_all_objects();
	}

	void infrastructure_manager::start()
	{
		boost::thread raw_data_notify_thread(boost::bind(
			&run_while_not_stopped, boost::ref(imp_->raw_data_work_queue_)
		));
		boost::thread parameter_notify_thread(boost::bind(
			&run_while_not_stopped, boost::ref(imp_->parameter_work_queue_)
		));
	}

	void infrastructure_manager::stop()
	{
		imp_->raw_data_work_queue_.stop();
		imp_->parameter_work_queue_.stop();
	}

	void infrastructure_manager::push_task_to_parameter_work_queue(const task_type& task)
	{
		imp_->parameter_work_queue_.push_task(task);
	}

	void infrastructure_manager::push_task_to_raw_data_work_queue(const task_type& task)
	{
		imp_->raw_data_work_queue_.push_task(task);
	}
}
