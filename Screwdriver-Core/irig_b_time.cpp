#include "irig_b_time.h"
#include "data_buffer.h"

namespace TM
{
	struct irig_b_time::irig_b_time_imp_t
	{
		irig_b_time_imp_t()
			:time_(0),
			 data_buffer_ptr_(new data_buffer)
		{
		}

		double time_;
		data_buffer_ptr data_buffer_ptr_;
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
	}
}
