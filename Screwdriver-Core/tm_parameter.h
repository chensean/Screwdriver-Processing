#pragma once
#include <functional>
#include <boost/cstdint.hpp>
#include <boost/noncopyable.hpp>
#include <boost/variant.hpp>
#include <boost/mpl/if.hpp>
#include "digit_utilities.h"

namespace TM
{
	typedef boost::variant<int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t,float, double, std::string> tm_val_type;
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
		virtual void read_form_buffer(const std::vector<uint8_t>& buffer,uint32_t start_idx) = 0;
	};
	typedef boost::shared_ptr<tm_parameter>  tm_parameter_ptr;



	template <int BYTE_COUNT>
	struct big_endian_read
	{
		typedef typename boost::mpl::if_c<BYTE_COUNT==8,uint64_t,typename boost::mpl::if_c<BYTE_COUNT==2,uint16_t,uint32_t>::type >::type val_type;

		static uint64_t read_code(const std::vector<uint8_t>& buffer, uint32_t start_idx)
		{
			return utilities::array_to_big_endian_val<val_type>(buffer, start_idx);
		}

	protected:
		~big_endian_read(void){}
	};

	template <>
	struct big_endian_read < 1 >
	{
		static uint64_t read_code(const std::vector<uint8_t>& buffer, uint32_t start_idx)
		{
			return start_idx<buffer.size() ? buffer[start_idx] : 0;
		}

	protected:
		~big_endian_read(void){}
	};

	template <>
	struct big_endian_read<3>
	{
		static uint64_t read_code(const std::vector<uint8_t>& buffer, uint32_t start_idx)
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

	template <int BYTE_COUNT>
	struct little_endian_read
	{
		typedef typename boost::mpl::if_c<BYTE_COUNT==8,uint64_t,typename boost::mpl::if_c<BYTE_COUNT==2,uint16_t,uint32_t>::type >::type val_type;

		static uint64_t read_code(const std::vector<uint8_t>& buffer, uint32_t start_idx)
		{
			return utilities::array_to_little_endian_val<val_type>(buffer, start_idx);
		}

	protected:
		~little_endian_read(void){}
	};

	template <>
	struct little_endian_read < 1 >
	{
		static uint64_t read_code(const std::vector<uint8_t>& buffer, uint32_t start_idx)
		{
			return start_idx < buffer.size() ? buffer[start_idx] : 0;
		}

	protected:
		~little_endian_read(void){}
	};

	template <>
	struct little_endian_read<3>
	{
		static uint64_t read_code(const std::vector<uint8_t>& buffer, uint32_t start_idx)
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
		int BYTE_COUNT
		,typename ValT
		,template<int> class ReadPolicy = big_endian_read
		>
	class base_tm_parameter 
		:public ReadPolicy<BYTE_COUNT>
		,public tm_parameter
		,private boost::noncopyable
	{
	public:
		typedef std::function<ValT(uint64_t)>  code_to_val_fun_t;
		typedef std::function< std::string(uint64_t)> code_to_val_text_fun_t;
		base_tm_parameter(const std::string& name,code_to_val_fun_t fun1=nullptr,code_to_val_text_fun_t fun2=nullptr) 
			:name_(name)
			,time_(0)
			, code_(0)
			, data_(BYTE_COUNT)
			,code_to_val_fun_(fun1)
			,code_to_val_text_fun_(fun2)
		{
		}

		~base_tm_parameter(void){}
		int32_t get_byte_count() const{ return BYTE_COUNT; }
		virtual std::string get_name() const { return name_; }
		virtual tm_val_type get_val() const 
		{
			if (code_to_val_fun_ != nullptr)
			{
				return tm_val_type(code_to_val_fun_(code_));
			}
			return tm_val_type(static_cast<ValT>(code_)); 
		}

		virtual double get_time() const { return time_; }
		virtual void set_time(double time){ time_ = time; }
		virtual std::vector<uint8_t> get_data() const { return data_; }
		virtual uint64_t get_code() const { return code_; }
		virtual void read_form_buffer(const std::vector<uint8_t>& buffer, uint32_t start_idx)
		{
			code_ = read_code(buffer, start_idx);
			std::copy(buffer.begin() + start_idx, buffer.begin() + start_idx + BYTE_COUNT, data_.begin());
		}

		virtual std::string get_val_text() const
		{
			if (code_to_val_text_fun_!=nullptr)
			{
				return code_to_val_text_fun_(code_);
			}
			return std::to_string(code_);
		}

	private:
		std::string name_;
		double time_;
		std::vector<uint8_t> data_;
		uint64_t code_;
		code_to_val_fun_t code_to_val_fun_;
		code_to_val_text_fun_t code_to_val_text_fun_;
	};


	typedef base_tm_parameter<1, uint8_t> parameter1;
	typedef base_tm_parameter<1, double> double_parameter1;
	typedef base_tm_parameter<2, uint16_t> big_endian_parameter2;
	typedef base_tm_parameter<2, double> big_endian_double_parameter2;
	typedef base_tm_parameter<3, uint32_t> big_endian_parameter3;
	typedef base_tm_parameter<4, uint32_t> big_endian_parameter4;
	typedef base_tm_parameter<4, float> big_endian_float_parameter4;
	typedef base_tm_parameter<4, double> big_endian_double_parameter4;
	typedef base_tm_parameter<8, uint64_t> big_endian_parameter8;
	typedef base_tm_parameter<8, double> big_endian_double_parameter8;
	typedef base_tm_parameter<2, uint16_t, little_endian_read> little_endian_parameter2;
	typedef base_tm_parameter<2, double, little_endian_read> little_endian_double_parameter2;
	typedef base_tm_parameter<3, uint32_t, little_endian_read> little_endian_parameter3;
	typedef base_tm_parameter<4, double, little_endian_read> little_endian_double_parameter4;
	typedef base_tm_parameter<4, float, little_endian_read> little_endian_float_parameter4;
	typedef base_tm_parameter<8, uint64_t, little_endian_read> little_endian_parameter8;
	typedef base_tm_parameter<8, double, little_endian_read> little_endian_double_parameter8;
}