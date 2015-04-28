#define SCREWDRIVER_INFRASTRUCTURE_COMPILATION
#include "parameter.h"
#include "infrastructure_manager.h"

namespace screwdriver
{
	struct parameter::parameter_imp_t
	{
		parameter_imp_t(const std::string& name, uint32_t bits_count)
			:name_(name)
			 , bits_count_(bits_count)
		{
		}

		std::string name_;
		uint32_t bits_count_;
		parameter_charged_signal_t parameter_charged_;
	};


	parameter::parameter(const std::string& name, uint32_t bits_count)
		:imp_(new parameter_imp_t(name, bits_count))
	{
	}

	parameter::~parameter(void)
	{
	}

	std::string parameter::get_name() const
	{
		return imp_->name_;
	}

	uint32_t parameter::get_bits_count() const
	{
		return imp_->bits_count_;
	}

	void parameter::set_value(double time, double val, uint64_t code)
	{
		parameter_value_ptr value_ptr(new parameter_value(imp_->name_, time, val, code));
		single_infrastructure_manager::Instance().push_task_to_parameter_work_queue(
			[=]()
			{
				imp_->parameter_charged_(value_ptr);
			}
		);
	}

	boost::signals2::connection parameter::connect_val_charged_signal(const parameter_charged_slot_t& slot)
	{
		return imp_->parameter_charged_.connect(slot);
	}
}
