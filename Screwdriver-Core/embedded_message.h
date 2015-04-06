#ifndef EMBEDDED_MESSAGE_HPP
#define EMBEDDED_MESSAGE_HPP

#if defined(_MSC_VER)
# pragma once
#endif

#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/signals2.hpp>

namespace TM
{
	class data_buffer;	
	typedef boost::shared_ptr<std::vector<uint8_t>> tm_data_ptr;	
	typedef boost::signals2::signal<void()> embedded_message_extract_signal_t;

	class __declspec(dllexport) embedded_message
		:private boost::noncopyable
	{
	public:
		embedded_message(uint32_t message_length, std::vector<uint8_t> synchro_word);
		~embedded_message(void);
		boost::shared_ptr<data_buffer> get_data_buffer() const;
		void add_data(const std::vector<uint8_t>& data, uint32_t start_idx, uint32_t length);
		void parse();
		boost::signals2::connection connect_embedded_message_extract_signal(const embedded_message_extract_signal_t::slot_type& slot);

	private:
		struct embedded_message_imp_t;
		boost::shared_ptr<embedded_message_imp_t> imp_;
	};

	typedef boost::shared_ptr<embedded_message> embedded_message_ptr;
}
#endif // EMBEDDED_MESSAGE_HPP

