#include "sfid_minor_frame.h"
#include "tm_parameter.h"
#include "base_tm_parameter.h"
#include "bits_tm_parameter.h"

namespace screwdriver
{
	struct sfid_minor_frame::sfid_minor_frame_imp_t
	{
		sfid_minor_frame_imp_t(uint32_t sfid_pos)
			:sfid_pos_(sfid_pos)
		{}
		uint32_t sfid_pos_;
		TM::tm_parameter_ptr sfid_tm_parameter_;
	};
	sfid_minor_frame::sfid_minor_frame(uint32_t sfid_pos,uint32_t sfid_bits, uint32_t bit_start_idx)
		:imp_(new sfid_minor_frame_imp_t(sfid_pos))
	{
		if (sfid_bits%8==0)
		{
			std::string type="parameter"+boost::lexical_cast<std::string>(sfid_bits);
			if (sfid_bits>8)
			{
				type="big_endian_"+type;
			}
			imp_->sfid_tm_parameter_=TM::create_base_tm_parameter(type,"sfid");			
		}
		else
		{
			std::string type="bits8_parameter"+boost::lexical_cast<std::string>(sfid_bits);
			if (sfid_bits>8 && sfid_bits<16)
			{
				type="big_endian_bits16_parameter"+boost::lexical_cast<std::string>(sfid_bits);
			}
			else if (sfid_bits>16 && sfid_bits<32)
			{
				type="big_endian_bits32_parameter"+boost::lexical_cast<std::string>(sfid_bits);
			}
			imp_->sfid_tm_parameter_=TM::create_bits_tm_parameter(type,"sfid",bit_start_idx);				
		}
	}


	sfid_minor_frame::~sfid_minor_frame(void)
	{
	}

	int32_t sfid_minor_frame::get_minor_frame_id(const tm_data_ptr& tm_data, uint32_t offset)
	{
		if (imp_->sfid_tm_parameter_)
		{
			imp_->sfid_tm_parameter_->read_form_buffer(*tm_data,offset+imp_->sfid_pos_);
			return boost::numeric_cast<int32_t>(imp_->sfid_tm_parameter_->get_code());
		}
		return 0;
	}
}
