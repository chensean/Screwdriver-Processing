#ifndef BASE_TM_PARAMETER_HPP
#define BASE_TM_PARAMETER_HPP

#if defined(_MSC_VER)
# pragma once
#endif

#include <boost/lexical_cast.hpp>
#include "tm_parameter.h"
#include "extract_policy.h"

namespace TM
{
	template
	<
		int BIT_COUNT
		, template<int> class ReadPolicy = big_endian_uint_extract
	>
	class base_tm_parameter
		:public ReadPolicy<BIT_COUNT>
		 ,public tm_parameter
		 ,private boost::noncopyable
	{
	public:

		base_tm_parameter(const std::string& name, primary_conversion_fun_t fun1 = nullptr, secondary_conversion_fun_t fun2 = nullptr, text_conversion_fun_t fun3 = nullptr)
			:name_(name)
			 ,time_(0)
			 , data_(get_bytes_count())
			 , extraction_val_(0)
			 ,primary_conversion_fun_(fun1)
			 ,secondary_conversion_fun_(fun2)
			 ,text_conversion_fun_(fun3)
		{
			static_assert(BIT_COUNT % std::numeric_limits<uint8_t>::digits == 0,"bits is not bytes");
		}

		~base_tm_parameter(void)
		{
		}

		virtual uint32_t get_bits_count() const override
		{
			return BIT_COUNT;
		}

		uint32_t get_bytes_count() const
		{
			return BIT_COUNT / std::numeric_limits<uint8_t>::digits;
		}

		virtual std::string get_name() const override
		{
			return name_;
		}

		virtual double get_val() const override
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


		virtual uint64_t get_code() const override
		{
			typedef boost::mpl::int_<BIT_COUNT> i_bits_t;
			typedef typename boost::mpl::if_<boost::mpl::greater<i_bits_t, i32_t>, uint64_t, typename boost::mpl::if_<boost::mpl::greater<i_bits_t, i16_t>, uint32_t, typename boost::mpl::if_<boost::mpl::greater<i_bits_t, i8_t>, uint16_t, uint8_t>::type>::type>::type code_type;
			return utilities::array_to_big_endian_val<code_type>(data_, 0);
		}

		virtual double get_extraction_val() const override
		{
			return extraction_val_;
		}

		virtual double get_time() const override
		{
			return time_;
		}

		virtual void set_time(double time) override
		{
			time_ = time;
		}

		virtual std::vector<uint8_t> get_data() const override
		{
			return data_;
		}

		virtual void set_primary_conversion(primary_conversion_fun_t primary_conversion) override
		{
			primary_conversion_fun_ = primary_conversion;
		}

		virtual void set_secondary_conversion(secondary_conversion_fun_t secondary_conversion) override
		{
			secondary_conversion_fun_ = secondary_conversion;
		}

		virtual void set_text_conversion(text_conversion_fun_t text_conversion) override
		{
			text_conversion_fun_ = text_conversion;
		}

		virtual void read_form_buffer(const std::vector<uint8_t>& buffer, uint32_t start_idx) override
		{
			extraction_val_ = boost::numeric_cast<double>(extract_val(buffer, start_idx)) ;
			std::copy(buffer.begin() + start_idx, buffer.begin() + start_idx + get_bytes_count(), data_.begin());
			val_charged_(this);
		}

		virtual std::string get_val_text() const override
		{
			if (text_conversion_fun_ != nullptr)
			{
				return text_conversion_fun_(get_val());
			}
			return boost::lexical_cast<std::string>(get_val());
		}

		virtual boost::signals2::connection connect_val_charged_signal(const val_charged_slot_t& slot) override
		{
			return val_charged_.connect(slot);
		}

	private:
		std::string name_;
		double time_;
		std::vector<uint8_t> data_;
		double extraction_val_;
		primary_conversion_fun_t primary_conversion_fun_;
		secondary_conversion_fun_t secondary_conversion_fun_;
		text_conversion_fun_t text_conversion_fun_;
		val_charged_signal_t val_charged_;
	};


	template <class T>
	struct create_with_name
	{
		static T* create(const std::string& name)
		{
			return new T(name);
		}
	};

	typedef boost::function<tm_parameter*(const std::string&)> base_tm_parameter_creator;
	bool regist_base_tm_parameter(const std::string& tm_parameter_type, base_tm_parameter_creator creator);
	bool unregist_base_tm_parameter(const std::string& tm_parameter_type);
	__declspec(dllexport) tm_parameter_ptr create_base_tm_parameter(const std::string& tm_parameter_type, const std::string& tm_parameter_name);
}

#endif // BASE_TM_PARAMETER_HPP

