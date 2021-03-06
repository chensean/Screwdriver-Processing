#include "parameter_position_table.h"

namespace TM
{
	typedef std::pair<tm_parameter_ptr, uint32_t> parameter_position_t;
	typedef std::vector<parameter_position_t> parameter_container_t;

	struct parameter_position_table::parameter_position_table_imp_t
	{
		parameter_position_table_imp_t()
			:delta_time_(0)
		{
		}

		double delta_time_;
		parameter_container_t parameters;
	};

	parameter_position_table::parameter_position_table(void)
		:imp_(new parameter_position_table_imp_t)
	{
	}


	parameter_position_table::~parameter_position_table(void)
	{
	}

	void parameter_position_table::add_parameter(const tm_parameter_ptr& param, uint32_t start_idx)
	{
		imp_->parameters.push_back(std::make_pair(param, start_idx));
	}

	void parameter_position_table::read_from_buffer(const std::vector<uint8_t>& data)
	{
		read_from_buffer(data, 0);
	}

	void parameter_position_table::read_from_buffer(const std::vector<uint8_t>& data, uint32_t offset)
	{
		std::for_each(imp_->parameters.begin(), imp_->parameters.end()
		              , [&data,offset](const parameter_position_t& param_pos)
		              {
			              param_pos.first->read_form_buffer(data, offset + param_pos.second);
		              });
	}

	void parameter_position_table::set_delta_time(double delta_time)
	{
		imp_->delta_time_ = delta_time;
	}

	void parameter_position_table::set_time(double time)
	{
		std::for_each(imp_->parameters.begin(), imp_->parameters.end()
		              , [=,&time](const parameter_position_t& param_pos)
		              {
			              param_pos.first->set_time(time + param_pos.second * imp_->delta_time_);
		              });
	}
}
