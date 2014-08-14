#include "data_buffer.h"


namespace TM
{
	typedef std::pair<uint32_t,tm_parameter_ptr> parameter_position_t;
	typedef std::map<uint32_t,tm_parameter_ptr> parameter_container_t;
	struct data_buffer::data_buffer_imp_t
	{
		parameter_container_t parameters;
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

	void data_buffer::update_buffer( const std::vector<uint8_t>& buffer )
	{
		std::for_each(imp_->parameters.begin(),imp_->parameters.end()
			,[&buffer](const parameter_position_t& param_pos)
			{
				param_pos.second->read_form_buffer(buffer,param_pos.first);
		});
	}

}