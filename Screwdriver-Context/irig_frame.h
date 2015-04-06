#ifndef IRIG_FRAME_HPP
#define IRIG_FRAME_HPP

#if defined(_MSC_VER)
# pragma once
#endif
#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <data_buffer.h>

namespace screwdriver
{
	typedef boost::shared_ptr<std::vector<uint8_t>> tm_data_ptr;
	typedef std::function<int32_t(const tm_data_ptr&, uint32_t)> get_minor_frame_id_fun_t;

	class irig_frame
		:private boost::noncopyable
	{
	public:
		irig_frame(uint64_t bit_rate, uint32_t minor_frame_bytes, uint32_t minor_frames, get_minor_frame_id_fun_t get_minor_frame_id_fun);
		~irig_frame(void);
		void parse_frame(const tm_data_ptr& data_ptr, uint32_t offset);
		void add_parameter(const TM::tm_parameter_ptr& param, uint32_t minor_frame_id, uint32_t start_idx);
		void add_sub_buffer(const TM::embedded_message_ptr& sub_buffer, uint32_t minor_frame_id, uint32_t start_idx, uint32_t length);
	private:
		void parse_frame_data(int32_t minor_frame_id, const tm_data_ptr& data_ptr, uint32_t offset);
	private:
		struct irig_frame_imp_t;
		boost::shared_ptr<irig_frame_imp_t> imp_;
	};
}
#endif // IRIG_FRAME_HPP

