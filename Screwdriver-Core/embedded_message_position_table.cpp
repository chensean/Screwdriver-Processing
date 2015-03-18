#include "embedded_message_position_table.h"

namespace TM
{
	typedef std::tuple<sub_buffer_ptr, uint32_t, uint32_t> sub_buffer_position_t;
	typedef std::vector<sub_buffer_position_t> sub_buffer_container_t;

	struct embedded_message_position_table::sub_buffer_position_table_imp_t
	{
		sub_buffer_container_t sub_buffers;
	};

	embedded_message_position_table::embedded_message_position_table(void)
	{
	}


	embedded_message_position_table::~embedded_message_position_table(void)
	{
	}

	void embedded_message_position_table::add_sub_buffer(const sub_buffer_ptr& sub_buffer, uint32_t start_idx, uint32_t length)
	{
		imp_->sub_buffers.push_back(std::make_tuple(sub_buffer, start_idx, length));
	}


	void embedded_message_position_table::read_from_buffer(const std::vector<uint8_t>& data)
	{
		read_from_buffer(data, 0);
	}

	void embedded_message_position_table::read_from_buffer(const std::vector<uint8_t>& data, uint32_t offset)
	{
		std::for_each(imp_->sub_buffers.begin(), imp_->sub_buffers.end()
		              , [&data,offset](const sub_buffer_position_t& sub_buffer_pos)
		              {
			              std::get<0>(sub_buffer_pos)->add_data(data, std::get<1>(sub_buffer_pos) + offset, std::get<2>(sub_buffer_pos));
		              });
	}
}
