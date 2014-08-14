#ifndef BITS_TM_PARAMETER_HPP
#define BITS_TM_PARAMETER_HPP

#include <boost/lexical_cast.hpp>
#include "tm_parameter.h"
#include "read_policy.h"

namespace TM
{
	template
		<
		int BIT_COUNT
		,typename value_type
		,typename container_type=uint8_t
		,template<int> class ReadPolicy = big_endian_read
		>
	class bits_tm_parameter 
		:public ReadPolicy<std::numeric_limits<container_type>::digits>
		,public tm_parameter
		,private boost::noncopyable
	{
	public:
		typedef  boost::mpl::int_<BIT_COUNT> i_bits_t;
		typedef typename boost::mpl::if_< boost::mpl::greater<i_bits_t,i32_t>,uint64_t,typename boost::mpl::if_<boost::mpl::greater<i_bits_t,i16_t>,uint32_t,typename boost::mpl::if_<boost::mpl::greater<i_bits_t,i8_t>,uint16_t,uint8_t>::type >::type >::type code_type;
		typedef std::function<value_type(code_type)>  code_to_val_fun_t;
		typedef std::function< std::string(code_type)> code_to_val_text_fun_t;
		bits_tm_parameter(const std::string& name,uint32_t bit_start_idx=0,code_to_val_fun_t fun1=nullptr,code_to_val_text_fun_t fun2=nullptr) 
			:name_(name)
			,bit_start_idx_(bit_start_idx)
			,time_(0)
			, code_(0)
			, data_(sizeof(container_type))
			,code_to_val_fun_(fun1)
			,code_to_val_text_fun_(fun2)
		{
			static_assert(boost::is_integral<container_type>::value,"ContainerT is not integral");
		}

		~bits_tm_parameter(void){}
		int32_t get_bits_count() const{ return BIT_COUNT; }
		virtual std::string get_name() const { return name_; }
		virtual tm_val_type get_val() const 
		{
			if (code_to_val_fun_ != nullptr)
			{
				return tm_val_type(code_to_val_fun_(code_));
			}
			return tm_val_type(boost::numeric_cast<value_type>(code_)); 
		}

		virtual double get_val_f() const 
		{
			if (code_to_val_fun_ != nullptr)
			{
				return boost::numeric_cast<double>(code_to_val_fun_(code_));
			}
			return boost::numeric_cast<double>(code_); 
		}

		virtual double get_time() const { return time_; }
		virtual void set_time(double time){ time_ = time; }
		virtual std::vector<uint8_t> get_data() const { return data_; }
		virtual uint64_t get_code() const { return code_; }
		virtual void read_form_buffer(const std::vector<uint8_t>& buffer, uint32_t start_idx)
		{
			container_type code = read_code(buffer, start_idx);
			code_=utilities::get_bits_value<BIT_COUNT,code_type>(code,bit_start_idx_);
			std::copy(buffer.begin() + start_idx, buffer.begin() + start_idx + sizeof(container_type), data_.begin());
			code_charged_(this);
		}

		virtual std::string get_val_text() const
		{
			if (code_to_val_text_fun_!=nullptr)
			{
				return code_to_val_text_fun_(code_);
			}
			return boost::lexical_cast<std::string>(get_code());
		}

		virtual boost::signals2::connection connect_signal(const code_charged_signal_t::slot_type& slot)
		{
			return code_charged_.connect(slot);
		}

	private:
		std::string name_;
		uint32_t bit_start_idx_;
		double time_;
		std::vector<uint8_t> data_;
		code_type code_;
		code_to_val_fun_t code_to_val_fun_;
		code_to_val_text_fun_t code_to_val_text_fun_;
		code_charged_signal_t code_charged_;
	};

	typedef bits_tm_parameter<1, uint8_t> bits8_parameter1;
	typedef bits_tm_parameter<2, uint8_t> bits8_parameter2;
	typedef bits_tm_parameter<3, uint8_t> bits8_parameter3;
	typedef bits_tm_parameter<4, uint8_t> bits8_parameter4;
	typedef bits_tm_parameter<5, uint8_t> bits8_parameter5;
	typedef bits_tm_parameter<6, uint8_t> bits8_parameter6;
	typedef bits_tm_parameter<7, uint8_t> bits8_parameter7;
	typedef bits_tm_parameter<2, uint8_t, uint16_t> big_endian_bits16_parameter2;
	typedef bits_tm_parameter<3, uint8_t, uint16_t> big_endian_bits16_parameter3;
	typedef bits_tm_parameter<4, uint8_t, uint16_t> big_endian_bits16_parameter4;
	typedef bits_tm_parameter<5, uint8_t, uint16_t> big_endian_bits16_parameter5;
	typedef bits_tm_parameter<6, uint8_t, uint16_t> big_endian_bits16_parameter6;
	typedef bits_tm_parameter<7, uint8_t, uint16_t> big_endian_bits16_parameter7;
	typedef bits_tm_parameter<8,  uint8_t, uint16_t> big_endian_bits16_parameter8;
	typedef bits_tm_parameter<9, uint16_t, uint16_t> big_endian_bits16_parameter9;
	typedef bits_tm_parameter<10, uint16_t, uint16_t> big_endian_bits16_parameter10;
	typedef bits_tm_parameter<11, uint16_t, uint16_t> big_endian_bits16_parameter11;
	typedef bits_tm_parameter<12, uint16_t, uint16_t> big_endian_bits16_parameter12;
	typedef bits_tm_parameter<13, uint16_t, uint16_t> big_endian_bits16_parameter13;
	typedef bits_tm_parameter<14, uint16_t, uint16_t> big_endian_bits16_parameter14;
	typedef bits_tm_parameter<15, uint16_t, uint16_t> big_endian_bits16_parameter15;
	typedef bits_tm_parameter<10, uint16_t, uint32_t> big_endian_bits32_parameter10;
	typedef bits_tm_parameter<11, uint16_t, uint32_t> big_endian_bits32_parameter11;
	typedef bits_tm_parameter<12, uint16_t, uint32_t> big_endian_bits32_parameter12;
	typedef bits_tm_parameter<13, uint16_t, uint32_t> big_endian_bits32_parameter13;
	typedef bits_tm_parameter<14, uint16_t, uint32_t> big_endian_bits32_parameter14;
	typedef bits_tm_parameter<15, uint16_t, uint32_t> big_endian_bits32_parameter15;
	typedef bits_tm_parameter<16, uint16_t, uint32_t> big_endian_bits32_parameter16;
	typedef bits_tm_parameter<17, uint32_t, uint32_t> big_endian_bits32_parameter17;
	typedef bits_tm_parameter<18, uint32_t, uint32_t> big_endian_bits32_parameter18;
	typedef bits_tm_parameter<19, uint32_t, uint32_t> big_endian_bits32_parameter19;
	typedef bits_tm_parameter<20, uint32_t, uint32_t> big_endian_bits32_parameter20;
	typedef bits_tm_parameter<21, uint32_t, uint32_t> big_endian_bits32_parameter21;
	typedef bits_tm_parameter<22, uint32_t, uint32_t> big_endian_bits32_parameter22;
	typedef bits_tm_parameter<23, uint32_t, uint32_t> big_endian_bits32_parameter23;
	typedef bits_tm_parameter<24, uint32_t, uint32_t> big_endian_bits32_parameter24;
	typedef bits_tm_parameter<25, uint32_t, uint32_t> big_endian_bits32_parameter25;
	typedef bits_tm_parameter<26, uint32_t, uint32_t> big_endian_bits32_parameter26;
	typedef bits_tm_parameter<27, uint32_t, uint32_t> big_endian_bits32_parameter27;
	typedef bits_tm_parameter<28, uint32_t, uint32_t> big_endian_bits32_parameter28;
	typedef bits_tm_parameter<29, uint32_t, uint32_t> big_endian_bits32_parameter29;
	typedef bits_tm_parameter<30, uint32_t, uint32_t> big_endian_bits32_parameter30;
	typedef bits_tm_parameter<31, uint32_t, uint32_t> big_endian_bits32_parameter31;
	typedef bits_tm_parameter<10, double, uint16_t> big_endian_bits16_double_parameter10;

	typedef bits_tm_parameter<2, uint8_t, uint16_t, little_endian_read> little_endian_bits16_parameter2;
	typedef bits_tm_parameter<3, uint8_t, uint16_t, little_endian_read> little_endian_bits16_parameter3;
	typedef bits_tm_parameter<4, uint8_t, uint16_t, little_endian_read> little_endian_bits16_parameter4;
	typedef bits_tm_parameter<5, uint8_t, uint16_t, little_endian_read> little_endian_bits16_parameter5;
	typedef bits_tm_parameter<6, uint8_t, uint16_t, little_endian_read> little_endian_bits16_parameter6;
	typedef bits_tm_parameter<7, uint8_t, uint16_t, little_endian_read> little_endian_bits16_parameter7;
	typedef bits_tm_parameter<8, uint8_t, uint16_t, little_endian_read> little_endian_bits16_parameter8;
	typedef bits_tm_parameter<9, uint16_t, uint16_t, little_endian_read> little_endian_bits16_parameter9;
	typedef bits_tm_parameter<10, uint16_t, uint16_t, little_endian_read> little_endian_bits16_parameter10;
	typedef bits_tm_parameter<11, uint16_t, uint16_t, little_endian_read> little_endian_bits16_parameter11;
	typedef bits_tm_parameter<12, uint16_t, uint16_t, little_endian_read> little_endian_bits16_parameter12;
	typedef bits_tm_parameter<13, uint16_t, uint16_t, little_endian_read> little_endian_bits16_parameter13;
	typedef bits_tm_parameter<14, uint16_t, uint16_t, little_endian_read> little_endian_bits16_parameter14;
	typedef bits_tm_parameter<15, uint16_t, uint16_t, little_endian_read> little_endian_bits16_parameter15;
	typedef bits_tm_parameter<10, uint16_t, uint32_t, little_endian_read> little_endian_bits32_parameter10;
	typedef bits_tm_parameter<11, uint16_t, uint32_t, little_endian_read> little_endian_bits32_parameter11;
	typedef bits_tm_parameter<12, uint16_t, uint32_t, little_endian_read> little_endian_bits32_parameter12;
	typedef bits_tm_parameter<13, uint16_t, uint32_t, little_endian_read> little_endian_bits32_parameter13;
	typedef bits_tm_parameter<14, uint16_t, uint32_t, little_endian_read> little_endian_bits32_parameter14;
	typedef bits_tm_parameter<15, uint16_t, uint32_t, little_endian_read> little_endian_bits32_parameter15;
	typedef bits_tm_parameter<16, uint16_t, uint32_t, little_endian_read> little_endian_bits32_parameter16;
	typedef bits_tm_parameter<17, uint32_t, uint32_t, little_endian_read> little_endian_bits32_parameter17;
	typedef bits_tm_parameter<18, uint32_t, uint32_t, little_endian_read> little_endian_bits32_parameter18;
	typedef bits_tm_parameter<19, uint32_t, uint32_t, little_endian_read> little_endian_bits32_parameter19;
	typedef bits_tm_parameter<20, uint32_t, uint32_t, little_endian_read> little_endian_bits32_parameter20;
	typedef bits_tm_parameter<21, uint32_t, uint32_t, little_endian_read> little_endian_bits32_parameter21;
	typedef bits_tm_parameter<22, uint32_t, uint32_t, little_endian_read> little_endian_bits32_parameter22;
	typedef bits_tm_parameter<23, uint32_t, uint32_t, little_endian_read> little_endian_bits32_parameter23;
	typedef bits_tm_parameter<24, uint32_t, uint32_t, little_endian_read> little_endian_bits32_parameter24;
	typedef bits_tm_parameter<25, uint32_t, uint32_t, little_endian_read> little_endian_bits32_parameter25;
	typedef bits_tm_parameter<26, uint32_t, uint32_t, little_endian_read> little_endian_bits32_parameter26;
	typedef bits_tm_parameter<27, uint32_t, uint32_t, little_endian_read> little_endian_bits32_parameter27;
	typedef bits_tm_parameter<28, uint32_t, uint32_t, little_endian_read> little_endian_bits32_parameter28;
	typedef bits_tm_parameter<29, uint32_t, uint32_t, little_endian_read> little_endian_bits32_parameter29;
	typedef bits_tm_parameter<30, uint32_t, uint32_t, little_endian_read> little_endian_bits32_parameter30;
	typedef bits_tm_parameter<31, uint32_t, uint32_t, little_endian_read> little_endian_bits32_parameter31;
}

#endif // BITS_TM_PARAMETER_HPP