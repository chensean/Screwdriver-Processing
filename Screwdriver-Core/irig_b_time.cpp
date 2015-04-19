#include "irig_b_time.h"
#include "data_buffer.h"
#include "base_tm_parameter.h"
#include "string_utilities.h"

namespace TM
{
	struct irig_b_time::irig_b_time_imp_t
	{
		irig_b_time_imp_t()
			:time_(0),
			 data_buffer_ptr_(new data_buffer)
		{
			secondary_conversion_fun_t secondary_conversion_fun = [](double val)
				{
					uint16_t uint16_val = boost::numeric_cast<uint16_t>(val);
					std::string val_string = utilities::to_hex_string(uint16_val);
					return boost::lexical_cast<double>(val_string);
				};
			day_parameter_ptr_ = create_base_tm_parameter("big_endian_uint_parameter16", "d");
			day_parameter_ptr_->set_secondary_conversion(secondary_conversion_fun);
			data_buffer_ptr_->add_parameter(day_parameter_ptr_, 1);
			hour_parameter_ptr_ = create_base_tm_parameter("big_endian_uint_parameter8", "h");
			hour_parameter_ptr_->set_secondary_conversion(secondary_conversion_fun);
			data_buffer_ptr_->add_parameter(hour_parameter_ptr_, 3);
			minute_parameter_ptr_ = create_base_tm_parameter("big_endian_uint_parameter8", "m");
			minute_parameter_ptr_->set_secondary_conversion(secondary_conversion_fun);
			data_buffer_ptr_->add_parameter(minute_parameter_ptr_, 4);
			second_parameter_ptr_ = create_base_tm_parameter("big_endian_uint_parameter8", "s");
			second_parameter_ptr_->set_secondary_conversion(secondary_conversion_fun);
			data_buffer_ptr_->add_parameter(second_parameter_ptr_, 5);
			millisecond_parameter_ptr_ = create_base_tm_parameter("big_endian_uint_parameter16", "ms");
			millisecond_parameter_ptr_->set_secondary_conversion(secondary_conversion_fun);
			data_buffer_ptr_->add_parameter(millisecond_parameter_ptr_, 6);
		}

		double time_;
		data_buffer_ptr data_buffer_ptr_;
		tm_parameter_ptr day_parameter_ptr_;
		tm_parameter_ptr hour_parameter_ptr_;
		tm_parameter_ptr minute_parameter_ptr_;
		tm_parameter_ptr second_parameter_ptr_;
		tm_parameter_ptr millisecond_parameter_ptr_;
		time_charged_signal_t time_charged_;
	};

	irig_b_time::irig_b_time(void)
		:imp_(new irig_b_time_imp_t)
	{
	}


	irig_b_time::~irig_b_time(void)
	{
	}

	double irig_b_time::get_time() const
	{
		return imp_->time_;
	}

	void irig_b_time::read_form_buffer(const std::vector<uint8_t>& buffer, uint32_t start_idx)
	{
		imp_->data_buffer_ptr_->read_from_buffer(buffer, start_idx);
		imp_->time_ = imp_->day_parameter_ptr_->get_val() * 24 * 60 * 60 + imp_->hour_parameter_ptr_->get_val() * 60 * 60 + imp_->minute_parameter_ptr_->get_val() * 60 + imp_->second_parameter_ptr_->get_val() + imp_->millisecond_parameter_ptr_->get_val() / 1000;
		imp_->time_charged_(imp_->time_);
	}

	boost::signals2::connection irig_b_time::connect_time_charged_signal(const time_charged_slot_t& slot)
	{
		return imp_->time_charged_.connect(slot);
	}
}
