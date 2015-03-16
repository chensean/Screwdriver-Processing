#include "data_buffer.h"


namespace TM
{
	typedef std::pair<uint32_t,tm_parameter_ptr> parameter_position_t;
	typedef std::map<uint32_t,tm_parameter_ptr> parameter_container_t;
	typedef std::pair<uint32_t,sub_buffer_ptr> sub_buffer_position_t;
	typedef std::map<uint32_t,sub_buffer_ptr> sub_buffer_container_t;
	struct data_buffer::data_buffer_imp_t
	{
		parameter_container_t parameters;
		sub_buffer_container_t sub_buffers;
	};

	data_buffer::data_buffer(void)
		:imp_(new data_buffer_imp_t)
	{
	}


	data_buffer::~data_buffer(void)
	{
	}

	void data_buffer::add_parameter( const tm_parameter_ptr& param,uint32_t start_idx )
	{
		imp_->parameters[start_idx]=param;
	}

	void data_buffer::add_sub_buffer( const sub_buffer_ptr& sub_buffer,uint32_t start_idx )
	{
		imp_->sub_buffers[start_idx]=sub_buffer;
	}


	void data_buffer::update_buffer( const std::vector<uint8_t>& data )
	{
		std::for_each(imp_->parameters.begin(),imp_->parameters.end()
			,[&data](const parameter_position_t& param_pos)
			{
				param_pos.second->read_form_buffer(data,param_pos.first);
		});
		std::for_each(imp_->sub_buffers.begin(),imp_->sub_buffers.end()
			,[&data](const sub_buffer_position_t& sub_buffer_pos)
		{
			sub_buffer_pos.second->add_data(data,sub_buffer_pos.first);
		});
	}

}