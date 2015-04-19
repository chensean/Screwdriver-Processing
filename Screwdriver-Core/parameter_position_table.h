#ifndef PARAMETER_POSITION_TABLE_HPP
#define PARAMETER_POSITION_TABLE_HPP

#if defined(_MSC_VER)
# pragma once
#endif

#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include "tm_parameter.h"

namespace TM
{
	class __declspec(dllexport) parameter_position_table
		:private boost::noncopyable
	{
	public:
		parameter_position_table(void);
		~parameter_position_table(void);
		void add_parameter(const tm_parameter_ptr& param, uint32_t start_idx);
		void read_from_buffer(const std::vector<uint8_t>& data);
		void read_from_buffer(const std::vector<uint8_t>& data, uint32_t offset);
		void set_delta_time(double delta_time);
		void set_time(double time);
	private:
		struct parameter_position_table_imp_t;
		boost::shared_ptr<parameter_position_table_imp_t> imp_;
	};
}

#endif // PARAMETER_POSITION_TABLE_HPP

