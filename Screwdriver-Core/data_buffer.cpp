#include "data_buffer.h"
#include "parameter_position_table.h"
#include "embedded_message_position_table.h"

namespace TM
{
	struct data_buffer::data_buffer_imp_t
	{
		data_buffer_imp_t():
			parameter_position_table_(new parameter_position_table),
			sub_buffer_position_table_(new embedded_message_position_table)
		{
		}

		boost::shared_ptr<parameter_position_table> parameter_position_table_;
		boost::shared_ptr<embedded_message_position_table> sub_buffer_position_table_;
	};

	data_buffer::data_buffer(void)
		:imp_(new data_buffer_imp_t)
	{
	}


	data_buffer::~data_buffer(void)
	{
	}

	void data_buffer::add_parameter(const tm_parameter_ptr& param, uint32_t start_idx)
	{
		imp_->parameter_position_table_->add_parameter(param, start_idx);
	}

	void data_buffer::add_embedded_message(const embedded_message_ptr& embedded_message, uint32_t start_idx, uint32_t length)
	{
		imp_->sub_buffer_position_table_->add_sub_buffer(embedded_message, start_idx, length);
	}


	void data_buffer::read_from_buffer(const std::vector<uint8_t>& data)
	{
		read_from_buffer(data, 0);
	}

	void data_buffer::read_from_buffer(const std::vector<uint8_t>& data, uint32_t offset)
	{
		imp_->parameter_position_table_->read_from_buffer(data, offset);
		imp_->sub_buffer_position_table_->read_from_buffer(data, offset);
	}

	void data_buffer::set_delta_time(double delta_time)
	{
		imp_->parameter_position_table_->set_delta_time(delta_time);
	}

	void data_buffer::set_time(double time)
	{
		imp_->parameter_position_table_->set_time(time);
	}
}
