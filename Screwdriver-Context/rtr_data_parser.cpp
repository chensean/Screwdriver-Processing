#include "rtr_data_parser.h"
#define BOOST_ALL_DYN_LINK
#include <boost/make_shared.hpp>
# pragma warning( push )
# pragma warning(disable:4913)
#include <boost/thread/thread.hpp>
# pragma warning( pop )
#include "bounded_buffer.h"
#include "digit_utilities.h"
#include "save_file.h"

const size_t BOUNDED_BUFFER_SIZE = 8 * 1024 * 1024;
const size_t PARSER_BUFFER_SIZE = 8 * 1024 * 1024;
const int HEAD = 1234567890;
const int TAIL = -1234567890;
const uint32_t TIME_OFFSET = 3;
const uint32_t FRAME_OFFSET = 14;

namespace screwdriver
{
	typedef boost::circular_buffer<uint8_t> parser_buffer_type;

	struct rtr_data_parser::rtr_data_parser_imp_t
	{
		explicit rtr_data_parser_imp_t(const std::string& folder)
			: head_(utilities::big_endian_val_to_array(HEAD)),
			  tail_(utilities::big_endian_val_to_array(TAIL)),
			  tm_data_buffer_(BOUNDED_BUFFER_SIZE),
			  parser_buffer_(PARSER_BUFFER_SIZE),
			  save_file_(folder, "RAW_")
		{
		}

		std::vector<uint8_t> head_;
		std::vector<uint8_t> tail_;
		bounded_buffer<uint8_t> tm_data_buffer_;
		parser_buffer_type parser_buffer_;
		boost::shared_ptr<boost::thread> thread_;
		save_file save_file_;
		parse_frame_fun_t parse_frame_data_fun_;
		parse_frame_fun_t parse_frame_time_fun_;
	};

	rtr_data_parser::rtr_data_parser(const std::string& folder)
		:imp_(new rtr_data_parser_imp_t(folder))
	{
	}

	rtr_data_parser::~rtr_data_parser(void)
	{
	}

	void rtr_data_parser::save2file(tm_data_ptr data_ptr)
	{
		imp_->save_file_.receive(data_ptr);
	}

	void rtr_data_parser::start()
	{
		imp_->save_file_.start();
		imp_->thread_ = boost::make_shared<boost::thread>(
			[=]()
			{
				while (!boost::this_thread::interruption_requested() || imp_->tm_data_buffer_.is_not_empty())
				{
					auto data = imp_->tm_data_buffer_.pop_front_array();
					std::copy(data->begin(), data->end(), std::back_inserter(imp_->parser_buffer_));
					auto head = std::search(imp_->parser_buffer_.begin(), imp_->parser_buffer_.end(), imp_->head_.begin(), imp_->head_.end());
					size_t index = head - imp_->parser_buffer_.begin();
					while (head != imp_->parser_buffer_.end())
					{
						auto tail = std::search(head, imp_->parser_buffer_.end(), imp_->tail_.begin(), imp_->tail_.end());
						if (tail != imp_->parser_buffer_.end())
						{
							tm_data_ptr data_ptr(new std::vector<unsigned char>(head, tail + imp_->tail_.size()));
							save2file(data_ptr);
							if (imp_->parse_frame_time_fun_)
							{
								imp_->parse_frame_time_fun_(data_ptr, TIME_OFFSET * sizeof(int32_t));
							}
							if (imp_->parse_frame_data_fun_)
							{
								imp_->parse_frame_data_fun_(data_ptr, FRAME_OFFSET * sizeof(int32_t));
							}
							index = tail - imp_->parser_buffer_.begin() + imp_->tail_.size();
						}
						else
						{
							break;
						}
						head = std::search(tail + imp_->tail_.size(), imp_->parser_buffer_.end(), imp_->head_.begin(), imp_->head_.end());
					}
					imp_->parser_buffer_.erase(imp_->parser_buffer_.begin(), imp_->parser_buffer_.begin() + index);
				}
			});
	}

	void rtr_data_parser::stop()
	{
		if (imp_->thread_)
		{
			imp_->thread_->interrupt();
			imp_->thread_->join();
			imp_->thread_.reset();
		}
	}

	void rtr_data_parser::receive(iterator_type begin, iterator_type end)
	{
		imp_->tm_data_buffer_.push_back_array(begin, end);
	}

	void rtr_data_parser::set_parse_frame_data_fun(parse_frame_fun_t fun)
	{
		imp_->parse_frame_data_fun_ = fun;
	}

	void rtr_data_parser::set_parse_frame_time_fun(parse_frame_fun_t fun)
	{
		imp_->parse_frame_time_fun_ = fun;
	}
}
