#pragma once
#include <functional>
#include <boost/cstdint.hpp>
#include <boost/noncopyable.hpp>
#include <boost/variant.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/comparison.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/lexical_cast.hpp>
#include "digit_utilities.h"

namespace TM
{
	typedef boost::variant<int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t,float, double> tm_val_type;
	class __declspec(dllexport) tm_parameter : public std::enable_shared_from_this<tm_parameter>
	{
	public:
		tm_parameter(void){}
		virtual ~tm_parameter(void){}
		virtual std::string get_name() const =0;
		virtual double get_time() const = 0;
		virtual void set_time(double time)  = 0;
		virtual tm_val_type get_val() const = 0;
		virtual std::string get_val_text() const =0;
		virtual std::vector<uint8_t> get_data() const = 0;
		virtual uint64_t get_code() const = 0;
		virtual void read_form_buffer(const std::vector<uint8_t>& buffer,uint32_t start_idx,uint32_t bit_idx=0) = 0;
	};
	typedef boost::shared_ptr<tm_parameter>  tm_parameter_ptr;

	typedef  boost::mpl::int_<std::numeric_limits<uint8_t>::digits> i8_t;
	typedef  boost::mpl::int_<std::numeric_limits<uint16_t>::digits> i16_t;
	typedef  boost::mpl::int_<std::numeric_limits<uint32_t>::digits> i32_t;
	typedef  boost::mpl::int_<std::numeric_limits<uint64_t>::digits> i64_t;


	template <int BIT_COUNT>
	struct big_endian_read
	{
		typedef  boost::mpl::int_<BIT_COUNT> i_bits_t;
		typedef typename boost::mpl::if_< boost::mpl::greater<i_bits_t,i32_t>,uint64_t,typename boost::mpl::if_<boost::mpl::greater<i_bits_t,i16_t>,uint32_t,uint16_t>::type >::type val_type;

		static val_type read_code(const std::vector<uint8_t>& buffer, uint32_t start_idx)
		{
			static_assert(BIT_COUNT%std::numeric_limits<uint8_t>::digits==0,"bits is not bytes");
			return utilities::array_to_big_endian_val<val_type>(buffer, start_idx);
		}

	protected:
		~big_endian_read(void){}
	};

	template <>
	struct big_endian_read < std::numeric_limits<uint8_t>::digits >
	{
		static uint8_t read_code(const std::vector<uint8_t>& buffer, uint32_t start_idx)
		{
			return start_idx<buffer.size() ? buffer[start_idx] : 0;
		}

	protected:
		~big_endian_read(void){}
	};

	template <>
	struct big_endian_read<std::numeric_limits<uint8_t>::digits*3>
	{
		static uint32_t read_code(const std::vector<uint8_t>& buffer, uint32_t start_idx)
		{
			if (start_idx + 3<buffer.size())
			{
				std::vector<uint8_t> data(3);
				std::copy(buffer.begin() + start_idx, buffer.begin() + start_idx+3, data.begin());
				data.insert(data.begin(),0);
				return utilities::array_to_big_endian_val<uint32_t>(data, 0);
			}
			return 0;
		}

	protected:
		~big_endian_read(void){}
	};

	template <int BIT_COUNT>
	struct little_endian_read
	{
		typedef  boost::mpl::int_<BIT_COUNT> i_bits_t;
		typedef typename boost::mpl::if_< boost::mpl::greater<i_bits_t,i32_t>,uint64_t,typename boost::mpl::if_<boost::mpl::greater<i_bits_t,i16_t>,uint32_t,uint16_t>::type >::type val_type;

		static val_type read_code(const std::vector<uint8_t>& buffer, uint32_t start_idx)
		{
			static_assert(BIT_COUNT%std::numeric_limits<uint8_t>::digits==0,"bits is not bytes");
			return utilities::array_to_little_endian_val<val_type>(buffer, start_idx);
		}
		
	protected:
		~little_endian_read(void){}
	};

	template <>
	struct little_endian_read <std::numeric_limits<uint8_t>::digits >
	{
		static uint8_t read_code(const std::vector<uint8_t>& buffer, uint32_t start_idx)
		{
			return start_idx < buffer.size() ? buffer[start_idx] : 0;
		}

	protected:
		~little_endian_read(void){}
	};

	template <>
	struct little_endian_read<std::numeric_limits<uint8_t>::digits*3 >
	{
		static uint32_t read_code(const std::vector<uint8_t>& buffer, uint32_t start_idx)
		{
			if (start_idx + 3 < buffer.size())
			{
				std::vector<uint8_t> data(3);
				std::copy(buffer.begin() + start_idx, buffer.begin() + start_idx+3, data.begin());
				data.push_back(0);
				return utilities::array_to_little_endian_val<uint32_t>(data, 0);
			}
			return 0;
		}

	protected:
		~little_endian_read(void){}
	};


	template
		<
		int BIT_COUNT
		,typename value_type
		,template<int> class ReadPolicy = big_endian_read
		>
	class base_tm_parameter 
		:public ReadPolicy<BIT_COUNT>
		,public tm_parameter
		,private boost::noncopyable
	{
	public:
		typedef  boost::mpl::int_<BIT_COUNT> i_bits_t;
		typedef typename boost::mpl::if_< boost::mpl::greater<i_bits_t,i32_t>,uint64_t,typename boost::mpl::if_<boost::mpl::greater<i_bits_t,i16_t>,uint32_t,typename boost::mpl::if_<boost::mpl::greater<i_bits_t,i8_t>,uint16_t,uint8_t>::type >::type >::type code_type;

		typedef std::function<value_type(code_type)>  code_to_val_fun_t;
		typedef std::function< std::string(code_type)> code_to_val_text_fun_t;
		base_tm_parameter(const std::string& name,code_to_val_fun_t fun1=nullptr,code_to_val_text_fun_t fun2=nullptr) 
			:name_(name)
			,time_(0)
			, code_(0)
			, data_(get_bytes_count())
			,code_to_val_fun_(fun1)
			,code_to_val_text_fun_(fun2)
		{
			static_assert(BIT_COUNT%std::numeric_limits<uint8_t>::digits==0,"bits is not bytes");
		}

		~base_tm_parameter(void){}
		int32_t get_bits_count() const{ return BIT_COUNT; }
		int32_t get_bytes_count() const{ return  BIT_COUNT/std::numeric_limits<uint8_t>::digits;  }
		virtual std::string get_name() const { return name_; }
		virtual tm_val_type get_val() const 
		{
			if (code_to_val_fun_ != nullptr)
			{
				return tm_val_type(code_to_val_fun_(code_));
			}
			return tm_val_type(boost::numeric_cast<value_type>(code_)); 
		}

		virtual double get_time() const { return time_; }
		virtual void set_time(double time){ time_ = time; }
		virtual std::vector<uint8_t> get_data() const { return data_; }
		virtual uint64_t get_code() const { return code_; }
		virtual void read_form_buffer(const std::vector<uint8_t>& buffer, uint32_t start_idx,uint32_t bit_idx=0)
		{
			code_ = read_code(buffer, start_idx);
			std::copy(buffer.begin() + start_idx, buffer.begin() + start_idx + get_bytes_count(), data_.begin());
		}

		virtual std::string get_val_text() const
		{
			if (code_to_val_text_fun_!=nullptr)
			{
				return code_to_val_text_fun_(code_);
			}
			return boost::lexical_cast<std::string>(get_code());
		}

	private:
		std::string name_;
		double time_;
		std::vector<uint8_t> data_;
		code_type code_;
		code_to_val_fun_t code_to_val_fun_;
		code_to_val_text_fun_t code_to_val_text_fun_;
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
}