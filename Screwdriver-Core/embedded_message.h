#ifndef EMBEDDED_MESSAGE_HPP
#define EMBEDDED_MESSAGE_HPP

#if defined(_MSC_VER)
# pragma once
#endif

#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace TM
{
	class data_buffer;

	class embedded_message
		:private boost::noncopyable
	{
	public:
		embedded_message(uint32_t message_length, std::vector<uint8_t> synchro_word);
		~embedded_message(void);
		boost::shared_ptr<data_buffer> get_data_buffer() const;
		void add_data(const std::vector<uint8_t>& data, uint32_t start_idx, uint32_t length);
		void parse();
	private:
		struct sub_buffer_imp_t;
		boost::shared_ptr<sub_buffer_imp_t> imp_;
	};

	typedef boost::shared_ptr<embedded_message> sub_buffer_ptr;
}
#endif // EMBEDDED_MESSAGE_HPP

