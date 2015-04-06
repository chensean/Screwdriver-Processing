#ifndef SFID_MINOR_FRAME_HPP
#define SFID_MINOR_FRAME_HPP

#if defined(_MSC_VER)
# pragma once
#endif
#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace screwdriver
{
	typedef boost::shared_ptr<std::vector<uint8_t>> tm_data_ptr;

	class sfid_minor_frame
		:private boost::noncopyable
	{
	public:
		sfid_minor_frame(uint32_t sfid_pos, uint32_t sfid_bits, uint32_t bit_start_idx = 0);
		~sfid_minor_frame(void);
		int32_t get_minor_frame_id(const tm_data_ptr& tm_data, uint32_t offset);
	private:
		struct sfid_minor_frame_imp_t;
		boost::shared_ptr<sfid_minor_frame_imp_t> imp_;
	};
}
#endif // SFID_MINOR_FRAME_HPP

