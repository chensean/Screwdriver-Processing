#include "irig_frame.h"

namespace screwdriver
{
	typedef std::pair<int32_t, tm_data_ptr> fcc_buffer_frame;

	struct irig_frame::irig_frame_imp_t
	{
		irig_frame_imp_t(uint64_t bit_rate, uint32_t minor_frame_bytes, uint32_t minor_frames,get_minor_frame_id_fun_t get_minor_frame_id_fun)
			:bit_rate_(bit_rate),
			 minor_frame_bytes_(minor_frame_bytes),
			 minor_frames_(minor_frames),
			 get_minor_frame_id_fun_(get_minor_frame_id_fun)
		{
		}

		uint64_t bit_rate_;
		uint32_t minor_frame_bytes_;
		uint32_t minor_frames_;
		get_minor_frame_id_fun_t get_minor_frame_id_fun_;
		std::vector<fcc_buffer_frame> fcc_buffer_frame_list_;
		std::map<uint32_t, boost::shared_ptr<TM::data_buffer>> data_buffer_map_;
	};

	irig_frame::irig_frame(uint64_t bit_rate, uint32_t minor_frame_bytes, uint32_t minor_frames,get_minor_frame_id_fun_t get_minor_frame_id_fun):
		imp_(new irig_frame_imp_t(bit_rate,minor_frame_bytes,minor_frames,get_minor_frame_id_fun))
	{
	}

	irig_frame::~irig_frame(void)
	{
	}

	void irig_frame::parse_frame_data(int32_t minor_frame_id, const tm_data_ptr& data_ptr, uint32_t offset)
	{
		if (imp_->data_buffer_map_.find(minor_frame_id) != imp_->data_buffer_map_.end())
		{
			imp_->data_buffer_map_[minor_frame_id]->read_from_buffer(*data_ptr, offset);
		}
	}

	void irig_frame::parse_frame(const tm_data_ptr& data_ptr, uint32_t offset)
	{
		int32_t minor_frame_id = imp_->get_minor_frame_id_fun_ ? imp_->get_minor_frame_id_fun_(data_ptr,offset) : 0;
		if (minor_frame_id < 0)
		{
			std::for_each(imp_->fcc_buffer_frame_list_.begin(), imp_->fcc_buffer_frame_list_.end(),
			              [](fcc_buffer_frame buffer_frame)
			              {
				              --buffer_frame.first;
			              });
			imp_->fcc_buffer_frame_list_.push_back(std::make_pair(minor_frame_id, data_ptr));
		}
		else
		{
			if (imp_->fcc_buffer_frame_list_.size() > 0 && minor_frame_id == 0)
			{
				std::for_each(imp_->fcc_buffer_frame_list_.begin(), imp_->fcc_buffer_frame_list_.end(),
				              [=,&offset](fcc_buffer_frame buffer_frame)
				              {
					              parse_frame_data(buffer_frame.first + imp_->minor_frames_, data_ptr,offset);
				              });
			}

			parse_frame_data(minor_frame_id, data_ptr,offset);
		}
	}

	void irig_frame::add_parameter(const TM::tm_parameter_ptr& param, uint32_t minor_frame_id, uint32_t start_idx)
	{
		if (imp_->data_buffer_map_.find(minor_frame_id) != imp_->data_buffer_map_.end())
		{
			imp_->data_buffer_map_[minor_frame_id]->add_parameter(param, start_idx);
		}
	}

	void irig_frame::add_sub_buffer(const TM::sub_buffer_ptr& sub_buffer, uint32_t minor_frame_id, uint32_t start_idx, uint32_t length)
	{
		if (imp_->data_buffer_map_.find(minor_frame_id) != imp_->data_buffer_map_.end())
		{
			imp_->data_buffer_map_[minor_frame_id]->add_sub_buffer(sub_buffer, start_idx, length);
		}
	}
}
