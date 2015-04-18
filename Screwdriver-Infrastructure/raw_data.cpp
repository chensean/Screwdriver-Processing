#define SCREWDRIVER_INFRASTRUCTURE_COMPILATION
#include "raw_data.h"
#include "bounded_buffer_space_optimized.h"
#include "infrastructure_manager.h"

namespace screwdriver
{
	const size_t BUFFER_MAX_SIZE = 1000;

	struct raw_data::raw_data_imp_t
	{
		raw_data_imp_t(const std::string& name)
			:name_(name)
			 ,raw_data_buffer_(BUFFER_MAX_SIZE)
		{
		}

		std::string name_;
		bounded_buffer_space_optimized<data_ptr> raw_data_buffer_;
		raw_data_charged_signal_t raw_data_charged_;
	};


	raw_data::raw_data(const std::string& name)
		:imp_(new raw_data_imp_t(name))
	{
	}

	raw_data::~raw_data(void)
	{
	}

	std::string raw_data::get_name() const
	{
		return imp_->name_;
	}

	void raw_data::set_data(const data_ptr& data)
	{
		single_infrastructure_manager::Instance().push_task_to_raw_data_work_queue(
			[=]()
			{
				imp_->raw_data_charged_(data);
			}
		);
	}


	boost::signals2::connection raw_data::connect_data_charged_signal(const raw_data_charged_slot_t& slot)
	{
		return imp_->raw_data_charged_.connect(slot);
	}
}
