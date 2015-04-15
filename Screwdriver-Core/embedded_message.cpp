#include <boost/circular_buffer.hpp>
#include "embedded_message.h"
#include "data_buffer.h"

namespace TM
{
	struct embedded_message::embedded_message_imp_t
	{
		typedef boost::circular_buffer<uint8_t> buffer_type;

		embedded_message_imp_t(uint32_t message_length, std::vector<uint8_t> synchro_word):
			message_length_(message_length),
			message_buffer_(message_length * 10),
			synchro_word_(synchro_word),
			message_(message_length),
			data_buffer_(new data_buffer)
		{
		}

		uint32_t message_length_;
		buffer_type message_buffer_;
		std::vector<uint8_t> synchro_word_;
		std::vector<uint8_t> message_;
		boost::shared_ptr<data_buffer> data_buffer_;
		embedded_message_extract_signal_t embedded_message_extract_signal_;
	};

	embedded_message::embedded_message(uint32_t message_length, std::vector<uint8_t> synchro_word)
		:imp_(new embedded_message_imp_t(message_length, synchro_word))
	{
	}


	embedded_message::~embedded_message(void)
	{
	}

	boost::shared_ptr<data_buffer> embedded_message::get_data_buffer() const
	{
		return imp_->data_buffer_;
	}

	void embedded_message::add_data(const std::vector<uint8_t>& data, uint32_t start_idx, uint32_t length)
	{
		if (start_idx + length <= data.size())
		{
			std::copy(data.begin() + start_idx, data.begin() + start_idx + length, std::back_inserter(imp_->message_buffer_));
		}
	}

	void embedded_message::parse()
	{
		if (imp_->message_buffer_.size() >= imp_->message_length_)
		{
			auto iter = std::search(imp_->message_buffer_.begin(), imp_->message_buffer_.end(), imp_->synchro_word_.begin(), imp_->synchro_word_.end());

			if (iter != imp_->message_buffer_.end() && std::distance(iter, imp_->message_buffer_.end()) >= imp_->message_length_)
			{
				std::copy(iter, iter + imp_->message_length_, imp_->message_.begin());
				imp_->message_buffer_.erase(imp_->message_buffer_.begin(), iter + imp_->message_length_);
				imp_->data_buffer_->read_from_buffer(imp_->message_);
				tm_data_ptr data_ptr(new std::vector<unsigned char>(imp_->message_.begin(), imp_->message_.end()));
				imp_->embedded_message_extract_signal_(data_ptr);
			}
		}
	}

	boost::signals2::connection embedded_message::connect_embedded_message_extract_signal(const embedded_message_extract_signal_t::slot_type& slot)
	{
		return imp_->embedded_message_extract_signal_.connect(slot);
	}
}
