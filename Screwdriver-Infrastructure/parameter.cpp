#define SCREWDRIVER_INFRASTRUCTURE_COMPILATION
#include "parameter.h"
#include "infrastructure_manager.h"


namespace screwdriver
{
	struct parameter::parameter_imp_t
	{
		parameter_imp_t(const std::string& name)
			:name_(name),
			 value_(0),
			 time_(0)
		{
		}

		std::string name_;
		double value_;
		double time_;
		parameter_charged_signal_t parameter_charged_;
	};


	parameter::parameter(const std::string& name)
		:imp_(new parameter_imp_t(name))
	{
	}

	parameter::~parameter(void)
	{
	}

	std::string parameter::get_name() const
	{
		return imp_->name_;
	}

	double parameter::get_time() const
	{
		return imp_->time_;
	}

	double parameter::get_value() const
	{
		return imp_->value_;
	}

	void parameter::set_value(double time, double val)
	{
		imp_->time_ = time;
		imp_->value_ = val;
		single_infrastructure_manager::Instance().push_task_to_parameter_work_queue(
			[=]()
			{
				imp_->parameter_charged_(shared_from_this());
			}
		);
	}

	boost::signals2::connection parameter::connect_val_charged_signal(const parameter_charged_slot_t& slot)
	{
		return imp_->parameter_charged_.connect(slot);
	}
}
