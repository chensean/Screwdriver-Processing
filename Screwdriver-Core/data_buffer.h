#ifndef DATA_BUFFER_HPP
#define DATA_BUFFER_HPP

#if defined(_MSC_VER)
# pragma once
#endif

#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include "tm_parameter.h"

namespace TM
{
	class data_buffer
	{
	public:
		data_buffer(void);
		~data_buffer(void);
		void add_parameter(const tm_parameter_ptr& param,uint32_t start_idx);
		void update_buffer(const std::vector<uint8_t>& buffer);
	private:
		struct data_buffer_imp_t;
		boost::shared_ptr<data_buffer_imp_t> imp_;
	};
}
#endif // DATA_BUFFER_HPP
