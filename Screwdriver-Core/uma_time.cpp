#include "uma_time.h"
#include "data_buffer.h"
#include "base_tm_parameter.h"

namespace TM
{
	struct uma_time::uma_time_imp_t
	{
		uma_time_imp_t()
			:time_(0),
			 data_buffer_ptr_(new data_buffer)
		{
			second_parameter_ptr_ = create_base_tm_parameter("big_endian_uint_parameter32", "s");
			if (data_buffer_ptr_)
			{
				data_buffer_ptr_->add_parameter(second_parameter_ptr_, 0);
			}

			millisecond_parameter_ptr_ = create_base_tm_parameter("big_endian_uint_parameter16", "ms");
			data_buffer_ptr_->add_parameter(millisecond_parameter_ptr_, 4);
		}

		double time_;
		data_buffer_ptr data_buffer_ptr_;
		tm_parameter_ptr second_parameter_ptr_;
		tm_parameter_ptr millisecond_parameter_ptr_;
		time_charged_signal_t time_charged_;
	};

	uma_time::uma_time(void)
		:imp_(new uma_time_imp_t)
	{
	}


	uma_time::~uma_time(void)
	{
	}

	double uma_time::get_time() const
	{
		return imp_->time_;
	}

	void uma_time::read_form_buffer(const std::vector<uint8_t>& buffer, uint32_t start_idx)
	{
		imp_->data_buffer_ptr_->read_from_buffer(buffer, start_idx);
		imp_->time_ = (imp_->second_parameter_ptr_->get_val() * 10 + imp_->millisecond_parameter_ptr_->get_val() / 1000) / 1000;
		imp_->time_charged_(imp_->time_);
	}

	boost::signals2::connection uma_time::connect_time_charged_signal(const time_charged_slot_t& slot)
	{
		return imp_->time_charged_.connect(slot);
	}
}
