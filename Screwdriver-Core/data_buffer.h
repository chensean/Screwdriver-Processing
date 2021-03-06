#ifndef DATA_BUFFER_HPP
#define DATA_BUFFER_HPP

#if defined(_MSC_VER)
# pragma once
#endif

#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include "tm_parameter.h"
#include "embedded_message.h"

namespace TM
{
	class data_buffer;
	typedef boost::shared_ptr<data_buffer> data_buffer_ptr;

	class __declspec(dllexport) data_buffer
		:private boost::noncopyable
	{
	public:
		data_buffer(void);
		~data_buffer(void);
		void add_parameter(const tm_parameter_ptr& param, uint32_t start_idx);
		void add_embedded_message(const embedded_message_ptr& embedded_message, uint32_t start_idx, uint32_t length);
		void read_from_buffer(const std::vector<uint8_t>& data);
		void read_from_buffer(const std::vector<uint8_t>& data, uint32_t offset);
		void set_delta_time(double delta_time);
		void set_time(double time);
	private:
		struct data_buffer_imp_t;
		boost::shared_ptr<data_buffer_imp_t> imp_;
	};
}
#endif // DATA_BUFFER_HPP

