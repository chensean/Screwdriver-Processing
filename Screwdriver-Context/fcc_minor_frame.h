#ifndef FCC_MINOR_FRAME_HPP
#define FCC_MINOR_FRAME_HPP

#if defined(_MSC_VER)
# pragma once
#endif
#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace screwdriver
{
	typedef boost::shared_ptr<std::vector<uint8_t>> tm_data_ptr;

	class fcc_minor_frame
		:private boost::noncopyable
	{
	public:
		fcc_minor_frame(uint32_t sync_location, uint32_t minor_frames, const std::vector<uint8_t>& frame_sync_pattern);
		~fcc_minor_frame(void);
		int32_t get_minor_frame_id(const tm_data_ptr& tm_data, uint32_t offset);
	private:
		struct fcc_minor_frame_imp_t;
		boost::shared_ptr<fcc_minor_frame_imp_t> imp_;
	};
}
#endif // FCC_MINOR_FRAME_HPP

