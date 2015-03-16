#include "sub_buffer.h"


namespace TM
{
	struct sub_buffer::sub_buffer_imp_t
	{
		uint32_t data_length;
	};

	sub_buffer::sub_buffer(void)
		:imp_(new sub_buffer_imp_t)
	{
		imp_->data_length=0;
	}


	sub_buffer::~sub_buffer(void)
	{
	}

	uint32_t sub_buffer::get_length() const
	{
		return imp_->data_length;
	}

	void sub_buffer::add_data( const std::vector<uint8_t>& data ,uint32_t start_idx)
	{

	}

}