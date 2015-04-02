#ifndef BASE_TM_PARAMETER_HPP
#define BASE_TM_PARAMETER_HPP

#if defined(_MSC_VER)
# pragma once
#endif

#include <boost/lexical_cast.hpp>
#include "tm_parameter.h"
#include "read_policy.h"

namespace TM
{
	template
	<
		int BIT_COUNT
		, typename value_type
		, template<int> class ReadPolicy = big_endian_read
	>
	class base_tm_parameter
		:public ReadPolicy<BIT_COUNT>
		 ,public tm_parameter
		 ,private boost::noncopyable
	{
	public:
		typedef boost::mpl::int_<BIT_COUNT> i_bits_t;
		typedef typename boost::mpl::if_<boost::mpl::greater<i_bits_t, i32_t>, uint64_t, typename boost::mpl::if_<boost::mpl::greater<i_bits_t, i16_t>, uint32_t, typename boost::mpl::if_<boost::mpl::greater<i_bits_t, i8_t>, uint16_t, uint8_t>::type>::type>::type code_type;

		typedef std::function<value_type(code_type)> code_to_val_fun_t;

		base_tm_parameter(const std::string& name, code_to_val_fun_t fun1 = nullptr, to_val_text_fun_t fun2 = nullptr)
			:name_(name)
			 ,time_(0)
			 , code_(0)
			 , data_(get_bytes_count())
			 ,code_to_val_fun_(fun1)
			 ,to_val_text_fun_(fun2)
		{
			static_assert(BIT_COUNT % std::numeric_limits<uint8_t>::digits == 0,"bits is not bytes");
		}

		~base_tm_parameter(void)
		{
		}

		int32_t get_bits_count() const
		{
			return BIT_COUNT;
		}

		int32_t get_bytes_count() const
		{
			return BIT_COUNT / std::numeric_limits<uint8_t>::digits;
		}

		virtual std::string get_name() const
		{
			return name_;
		}

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

		virtual double get_time() const
		{
			return time_;
		}

		virtual void set_time(double time)
		{
			time_ = time;
		}

		virtual std::vector<uint8_t> get_data() const
		{
			return data_;
		}

		virtual uint64_t get_code() const
		{
			return code_;
		}

		virtual void read_form_buffer(const std::vector<uint8_t>& buffer, uint32_t start_idx)
		{
			code_ = read_code(buffer, start_idx);
			std::copy(buffer.begin() + start_idx, buffer.begin() + start_idx + get_bytes_count(), data_.begin());
			code_charged_(this);
		}

		virtual std::string get_val_text() const
		{
			if (to_val_text_fun_ != nullptr)
			{
				return to_val_text_fun_((tm_parameter*)this);
			}
			return boost::lexical_cast<std::string>(get_val_f());
		}

		virtual boost::signals2::connection connect_signal(const code_charged_signal_t::slot_type& slot)
		{
			return code_charged_.connect(slot);
		}

	private:
		std::string name_;
		double time_;
		std::vector<uint8_t> data_;
		code_type code_;
		code_to_val_fun_t code_to_val_fun_;
		to_val_text_fun_t to_val_text_fun_;
		code_charged_signal_t code_charged_;
	};


	typedef base_tm_parameter<8, uint8_t> parameter8;
	typedef base_tm_parameter<8, double> double_parameter8;
	typedef base_tm_parameter<16, uint16_t> big_endian_parameter16;
	typedef base_tm_parameter<16, double> big_endian_double_parameter16;
	typedef base_tm_parameter<24, uint32_t> big_endian_parameter24;
	typedef base_tm_parameter<32, uint32_t> big_endian_parameter32;
	typedef base_tm_parameter<32, float> big_endian_float_parameter32;
	typedef base_tm_parameter<32, double> big_endian_double_parameter32;
	typedef base_tm_parameter<64, uint64_t> big_endian_parameter64;
	typedef base_tm_parameter<64, double> big_endian_double_parameter64;
	typedef base_tm_parameter<16, uint16_t, little_endian_read> little_endian_parameter16;
	typedef base_tm_parameter<16, double, little_endian_read> little_endian_double_parameter16;
	typedef base_tm_parameter<24, uint32_t, little_endian_read> little_endian_parameter24;
	typedef base_tm_parameter<32, double, little_endian_read> little_endian_double_parameter32;
	typedef base_tm_parameter<32, float, little_endian_read> little_endian_float_parameter32;
	typedef base_tm_parameter<64, uint64_t, little_endian_read> little_endian_parameter64;
	typedef base_tm_parameter<64, double, little_endian_read> little_endian_double_parameter64;
	
    template <class T> struct create_with_name
    {
        static T* create(const std::string& name)
        {
	        return new T(name);
        }        
    };

	typedef boost::function<tm_parameter*(const std::string&)> base_tm_parameter_creator;
	bool regist_base_tm_parameter(const std::string& tm_parameter_type,base_tm_parameter_creator creator);
	bool unregist_base_tm_parameter(const std::string& tm_parameter_type);
	__declspec(dllexport) tm_parameter_ptr create_base_tm_parameter(const std::string& tm_parameter_type,const std::string& tm_parameter_name);
}

#endif // BASE_TM_PARAMETER_HPP

