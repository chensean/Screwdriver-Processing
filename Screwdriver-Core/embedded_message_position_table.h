#ifndef EMBEDDED_MESSAGE_POSITION_TABLE_HPP
#define EMBEDDED_MESSAGE_POSITION_TABLE_HPP

#if defined(_MSC_VER)
# pragma once
#endif

#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include "embedded_message.h"

namespace TM
{
	class __declspec(dllexport) embedded_message_position_table
		:private boost::noncopyable
	{
	public:
		embedded_message_position_table(void);
		~embedded_message_position_table(void);
		void add_sub_buffer(const sub_buffer_ptr& sub_buffer, uint32_t start_idx, uint32_t length);
		void read_from_buffer(const std::vector<uint8_t>& data);
		void read_from_buffer(const std::vector<uint8_t>& data, uint32_t offset);
	private:
		struct sub_buffer_position_table_imp_t;
		boost::shared_ptr<sub_buffer_position_table_imp_t> imp_;
	};
}

#endif // EMBEDDED_MESSAGE_POSITION_TABLE_HPP

