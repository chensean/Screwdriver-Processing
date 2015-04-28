#include "code3_time.h"
#include "data_buffer.h"
#include "base_tm_parameter.h"

namespace TM
{
	struct code3_time::code3_time_imp_t
	{
		code3_time_imp_t()
			:time_(0),
			 data_buffer_ptr_(new data_buffer)
		{
			second_parameter_ptr_ = create_base_tm_parameter("big_endian_uint_parameter32", "s");
			if (data_buffer_ptr_)
			{
				data_buffer_ptr_->add_parameter(second_parameter_ptr_, 0);
			}

			microsecond_parameter_ptr_ = create_base_tm_parameter("big_endian_uint_parameter32", "ms");
			data_buffer_ptr_->add_parameter(microsecond_parameter_ptr_, 4);
		}

		double time_;
		data_buffer_ptr data_buffer_ptr_;
		tm_parameter_ptr second_parameter_ptr_;
		tm_parameter_ptr microsecond_parameter_ptr_;
		time_charged_signal_t time_charged_;
	};

	code3_time::code3_time(void)
		:imp_(new code3_time_imp_t)
	{
	}


	code3_time::~code3_time(void)
	{
	}

	double code3_time::get_time() const
	{
		return imp_->time_;
	}

	void code3_time::read_form_buffer(const std::vector<uint8_t>& buffer, uint32_t start_idx)
	{
		imp_->data_buffer_ptr_->read_from_buffer(buffer, start_idx);
		imp_->time_ = imp_->second_parameter_ptr_->get_val() + imp_->microsecond_parameter_ptr_->get_val() / 1e6;
		imp_->time_charged_(imp_->time_);
	}

	boost::signals2::connection code3_time::connect_time_charged_signal(const time_charged_slot_t& slot)
	{
		return imp_->time_charged_.connect(slot);
	}
}
