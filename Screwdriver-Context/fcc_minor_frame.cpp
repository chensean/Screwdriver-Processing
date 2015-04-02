#include "fcc_minor_frame.h"


namespace screwdriver
{
	struct fcc_minor_frame::fcc_minor_frame_imp_t
	{
		fcc_minor_frame_imp_t(uint32_t sync_location, uint32_t minor_frames, const std::vector<uint8_t>& frame_sync_pattern)
			:id_(-1),
			 sync_location_(sync_location),
			 minor_frames_(minor_frames),
			 frame_sync_pattern_(frame_sync_pattern),
			 frame_sync_pattern_fcc_(frame_sync_pattern.size())
		{
			std::transform(frame_sync_pattern_.begin(), frame_sync_pattern_.end(), frame_sync_pattern_fcc_.begin(),
			               [](uint8_t val)
			               {
				               return ~val;
			               });
		}

		int32_t id_;
		uint32_t sync_location_;
		uint32_t minor_frames_;
		std::vector<uint8_t> frame_sync_pattern_;
		std::vector<uint8_t> frame_sync_pattern_fcc_;
	};

	fcc_minor_frame::fcc_minor_frame(uint32_t sync_location, uint32_t minor_frames, const std::vector<uint8_t>& frame_sync_pattern)
		:imp_(new fcc_minor_frame_imp_t(sync_location, minor_frames, frame_sync_pattern))
	{
	}


	fcc_minor_frame::~fcc_minor_frame(void)
	{
	}

	int32_t fcc_minor_frame::get_minor_frame_id(const tm_data_ptr& tm_data, uint32_t offset)
	{
		if (std::equal(imp_->frame_sync_pattern_fcc_.begin(), imp_->frame_sync_pattern_fcc_.end(), tm_data->begin() + offset + imp_->sync_location_))
		{
			imp_->id_ = 0;
		}
		else if (std::equal(imp_->frame_sync_pattern_.begin(), imp_->frame_sync_pattern_.end(), tm_data->begin() + offset + imp_->sync_location_))
		{
			if (imp_->id_ >= 0)
			{
				imp_->id_ = (imp_->id_ + 1) % imp_->minor_frames_;
			}
		}
		else
		{
			return imp_->minor_frames_;
		}
		return imp_->id_;
	}
}
