#ifndef BITS_TM_PARAMETER_HPP
#define BITS_TM_PARAMETER_HPP

#include <boost/lexical_cast.hpp>
#include "tm_parameter.h"
#include "extract_policy.h"

namespace TM
{
	template
	<
		int BIT_COUNT
		, typename container_type=uint8_t
		, template<int> class ReadPolicy = big_endian_uint_extract
	>
	class bits_tm_parameter
		:public ReadPolicy<std::numeric_limits<container_type>::digits>
		 ,public tm_parameter
		 ,private boost::noncopyable
	{
	public:
				typedef boost::mpl::int_<BIT_COUNT> i_bits_t;
		typedef typename boost::mpl::if_<boost::mpl::greater<i_bits_t, i32_t>, uint64_t, typename boost::mpl::if_<boost::mpl::greater<i_bits_t, i16_t>, uint32_t, typename boost::mpl::if_<boost::mpl::greater<i_bits_t, i8_t>, uint16_t, uint8_t>::type>::type>::type code_type;

		bits_tm_parameter(const std::string& name, uint32_t bit_start_idx = 0,  primary_conversion_fun_t fun1 = nullptr, secondary_conversion_fun_t fun2 = nullptr, text_conversion_fun_t fun3 = nullptr)
			:name_(name)
			 ,bit_start_idx_(bit_start_idx)
			 ,time_(0)
			 , data_(sizeof(container_type))
			 , extraction_val_(0)
			 ,primary_conversion_fun_(fun1)
			 ,secondary_conversion_fun_(fun2)
			 ,text_conversion_fun_(fun3)
		{
			static_assert(boost::is_integral<container_type>::value,"ContainerT is not integral");
		}

		~bits_tm_parameter(void)
		{
		}

		int32_t get_bits_count() const
		{
			return BIT_COUNT;
		}

		virtual std::string get_name() const
		{
			return name_;
		}
		
		virtual double get_val() const
		{
			double val = extraction_val_;
			if (primary_conversion_fun_ != nullptr)
			{
				val = primary_conversion_fun_(val);
			}
			if (secondary_conversion_fun_ != nullptr)
			{
				val = secondary_conversion_fun_(val);
			}
			return val;
		}

		virtual double get_time() const
		{
			return time_;
		}

		virtual void set_time(double time)
		{
			time_ = time;
		}
		
		virtual double get_extraction_val() const
		{
			return extraction_val_;
		}

		virtual std::vector<uint8_t> get_data() const
		{
			return data_;
		}
		
		virtual void read_form_buffer(const std::vector<uint8_t>& buffer, uint32_t start_idx)
		{
			container_type code = extract_val(buffer, start_idx);
			extraction_val_ = utilities::get_bits_value<BIT_COUNT, code_type>(code, bit_start_idx_);
			std::copy(buffer.begin() + start_idx, buffer.begin() + start_idx + sizeof(container_type), data_.begin());
			val_charged_(this);
		}
		

		virtual std::string get_val_text() const
		{
			if (text_conversion_fun_ != nullptr)
			{
				return text_conversion_fun_(get_val());
			}
			return boost::lexical_cast<std::string>(get_val());
		}
		
		virtual void set_primary_conversion(primary_conversion_fun_t primary_conversion)
		{
			primary_conversion_fun_ = primary_conversion;
		}

		virtual void set_secondary_conversion(secondary_conversion_fun_t secondary_conversion)
		{
			secondary_conversion_fun_ = secondary_conversion;
		}

		virtual void set_text_conversion(text_conversion_fun_t text_conversion)
		{
			text_conversion_fun_ = text_conversion;
		}
		
		virtual boost::signals2::connection connect_val_charged_signal(const val_charged_slot_t& slot)
		{
			return val_charged_.connect(slot);
		}
	private:
		std::string name_;
		uint32_t bit_start_idx_;
		double time_;
		std::vector<uint8_t> data_;
		double extraction_val_;
		primary_conversion_fun_t primary_conversion_fun_;
		secondary_conversion_fun_t secondary_conversion_fun_;
		text_conversion_fun_t text_conversion_fun_;
		val_charged_signal_t val_charged_;
	};

	typedef bits_tm_parameter<1> bits8_parameter1;
	typedef bits_tm_parameter<2> bits8_parameter2;
	typedef bits_tm_parameter<3> bits8_parameter3;
	typedef bits_tm_parameter<4> bits8_parameter4;
	typedef bits_tm_parameter<5> bits8_parameter5;
	typedef bits_tm_parameter<6> bits8_parameter6;
	typedef bits_tm_parameter<7> bits8_parameter7;
	typedef bits_tm_parameter<2,uint16_t> big_endian_bits16_parameter2;
	typedef bits_tm_parameter<3,uint16_t> big_endian_bits16_parameter3;
	typedef bits_tm_parameter<4,uint16_t> big_endian_bits16_parameter4;
	typedef bits_tm_parameter<5,uint16_t> big_endian_bits16_parameter5;
	typedef bits_tm_parameter<6,uint16_t> big_endian_bits16_parameter6;
	typedef bits_tm_parameter<7,uint16_t> big_endian_bits16_parameter7;
	typedef bits_tm_parameter<8,uint16_t> big_endian_bits16_parameter8;
	typedef bits_tm_parameter<9,  uint16_t> big_endian_bits16_parameter9;
	typedef bits_tm_parameter<10, uint16_t> big_endian_bits16_parameter10;
	typedef bits_tm_parameter<11,uint16_t> big_endian_bits16_parameter11;
	typedef bits_tm_parameter<12,uint16_t> big_endian_bits16_parameter12;
	typedef bits_tm_parameter<13,uint16_t> big_endian_bits16_parameter13;
	typedef bits_tm_parameter<14,uint16_t> big_endian_bits16_parameter14;
	typedef bits_tm_parameter<15,uint16_t> big_endian_bits16_parameter15;
	typedef bits_tm_parameter<10,uint32_t> big_endian_bits32_parameter10;
	typedef bits_tm_parameter<11,uint32_t> big_endian_bits32_parameter11;
	typedef bits_tm_parameter<12,uint32_t> big_endian_bits32_parameter12;
	typedef bits_tm_parameter<13,uint32_t> big_endian_bits32_parameter13;
	typedef bits_tm_parameter<14,uint32_t> big_endian_bits32_parameter14;
	typedef bits_tm_parameter<15,uint32_t> big_endian_bits32_parameter15;
	typedef bits_tm_parameter<16,uint32_t> big_endian_bits32_parameter16;
	typedef bits_tm_parameter<17,uint32_t> big_endian_bits32_parameter17;
	typedef bits_tm_parameter<18,uint32_t> big_endian_bits32_parameter18;
	typedef bits_tm_parameter<19,uint32_t> big_endian_bits32_parameter19;
	typedef bits_tm_parameter<20,uint32_t> big_endian_bits32_parameter20;
	typedef bits_tm_parameter<21,uint32_t> big_endian_bits32_parameter21;
	typedef bits_tm_parameter<22,uint32_t> big_endian_bits32_parameter22;
	typedef bits_tm_parameter<23,uint32_t> big_endian_bits32_parameter23;
	typedef bits_tm_parameter<24,uint32_t> big_endian_bits32_parameter24;
	typedef bits_tm_parameter<25,uint32_t> big_endian_bits32_parameter25;
	typedef bits_tm_parameter<26,uint32_t> big_endian_bits32_parameter26;
	typedef bits_tm_parameter<27,uint32_t> big_endian_bits32_parameter27;
	typedef bits_tm_parameter<28,uint32_t> big_endian_bits32_parameter28;
	typedef bits_tm_parameter<29,uint32_t> big_endian_bits32_parameter29;
	typedef bits_tm_parameter<30,uint32_t> big_endian_bits32_parameter30;
	typedef bits_tm_parameter<31,uint32_t> big_endian_bits32_parameter31;

	typedef bits_tm_parameter<2,  uint16_t, little_endian_uint_extract> little_endian_bits16_parameter2;
	typedef bits_tm_parameter<3,  uint16_t, little_endian_uint_extract> little_endian_bits16_parameter3;
	typedef bits_tm_parameter<4,  uint16_t, little_endian_uint_extract> little_endian_bits16_parameter4;
	typedef bits_tm_parameter<5,  uint16_t, little_endian_uint_extract> little_endian_bits16_parameter5;
	typedef bits_tm_parameter<6,  uint16_t, little_endian_uint_extract> little_endian_bits16_parameter6;
	typedef bits_tm_parameter<7,  uint16_t, little_endian_uint_extract> little_endian_bits16_parameter7;
	typedef bits_tm_parameter<8,  uint16_t, little_endian_uint_extract> little_endian_bits16_parameter8;
	typedef bits_tm_parameter<9,  uint16_t, little_endian_uint_extract> little_endian_bits16_parameter9;
	typedef bits_tm_parameter<10, uint16_t, little_endian_uint_extract> little_endian_bits16_parameter10;
	typedef bits_tm_parameter<11, uint16_t, little_endian_uint_extract> little_endian_bits16_parameter11;
	typedef bits_tm_parameter<12, uint16_t, little_endian_uint_extract> little_endian_bits16_parameter12;
	typedef bits_tm_parameter<13, uint16_t, little_endian_uint_extract> little_endian_bits16_parameter13;
	typedef bits_tm_parameter<14, uint16_t, little_endian_uint_extract> little_endian_bits16_parameter14;
	typedef bits_tm_parameter<15, uint16_t, little_endian_uint_extract> little_endian_bits16_parameter15;
	typedef bits_tm_parameter<10, uint32_t, little_endian_uint_extract> little_endian_bits32_parameter10;
	typedef bits_tm_parameter<11, uint32_t, little_endian_uint_extract> little_endian_bits32_parameter11;
	typedef bits_tm_parameter<12, uint32_t, little_endian_uint_extract> little_endian_bits32_parameter12;
	typedef bits_tm_parameter<13, uint32_t, little_endian_uint_extract> little_endian_bits32_parameter13;
	typedef bits_tm_parameter<14, uint32_t, little_endian_uint_extract> little_endian_bits32_parameter14;
	typedef bits_tm_parameter<15, uint32_t, little_endian_uint_extract> little_endian_bits32_parameter15;
	typedef bits_tm_parameter<16, uint32_t, little_endian_uint_extract> little_endian_bits32_parameter16;
	typedef bits_tm_parameter<17, uint32_t, little_endian_uint_extract> little_endian_bits32_parameter17;
	typedef bits_tm_parameter<18, uint32_t, little_endian_uint_extract> little_endian_bits32_parameter18;
	typedef bits_tm_parameter<19, uint32_t, little_endian_uint_extract> little_endian_bits32_parameter19;
	typedef bits_tm_parameter<20, uint32_t, little_endian_uint_extract> little_endian_bits32_parameter20;
	typedef bits_tm_parameter<21, uint32_t, little_endian_uint_extract> little_endian_bits32_parameter21;
	typedef bits_tm_parameter<22, uint32_t, little_endian_uint_extract> little_endian_bits32_parameter22;
	typedef bits_tm_parameter<23, uint32_t, little_endian_uint_extract> little_endian_bits32_parameter23;
	typedef bits_tm_parameter<24, uint32_t, little_endian_uint_extract> little_endian_bits32_parameter24;
	typedef bits_tm_parameter<25, uint32_t, little_endian_uint_extract> little_endian_bits32_parameter25;
	typedef bits_tm_parameter<26, uint32_t, little_endian_uint_extract> little_endian_bits32_parameter26;
	typedef bits_tm_parameter<27, uint32_t, little_endian_uint_extract> little_endian_bits32_parameter27;
	typedef bits_tm_parameter<28, uint32_t, little_endian_uint_extract> little_endian_bits32_parameter28;
	typedef bits_tm_parameter<29, uint32_t, little_endian_uint_extract> little_endian_bits32_parameter29;
	typedef bits_tm_parameter<30, uint32_t, little_endian_uint_extract> little_endian_bits32_parameter30;
	typedef bits_tm_parameter<31, uint32_t, little_endian_uint_extract> little_endian_bits32_parameter31;

    template <class T> struct create_with_name_and_position
    {
        static T* create(const std::string& name, uint32_t bit_start_idx)
        {
	        return new T(name,bit_start_idx);
        }        
    };

	typedef boost::function<tm_parameter*(const std::string&,uint32_t)> bits_tm_parameter_creator;
	bool regist_bits_tm_parameter(const std::string& tm_parameter_type,bits_tm_parameter_creator creator);
	bool unregist_bits_tm_parameter(const std::string& tm_parameter_type);
	__declspec(dllexport) tm_parameter_ptr create_bits_tm_parameter(const std::string& tm_parameter_type,const std::string& tm_parameter_name, uint32_t bit_start_idx);
}

#endif // BITS_TM_PARAMETER_HPP

