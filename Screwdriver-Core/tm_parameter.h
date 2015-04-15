#ifndef TM_PARAMETER_HPP
#define TM_PARAMETER_HPP

#if defined(_MSC_VER)
# pragma once
#endif

#include <boost/cstdint.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/signals2.hpp>


namespace TM
{
	class tm_parameter;
	typedef boost::shared_ptr<tm_parameter> tm_parameter_ptr;
	typedef boost::signals2::signal<void(tm_parameter*)> val_charged_signal_t;
	typedef val_charged_signal_t::slot_type val_charged_slot_t;
	typedef std::function<double(double)> primary_conversion_fun_t;
	typedef std::function<double(double)> secondary_conversion_fun_t;
	typedef std::function<std::string(double)> text_conversion_fun_t;

	class __declspec(dllexport) tm_parameter : public boost::enable_shared_from_this<tm_parameter>
	{
	public:
		tm_parameter(void)
		{
		}

		virtual ~tm_parameter(void)
		{
		}

		virtual std::string get_name() const =0;
		virtual double get_time() const = 0;
		virtual void set_time(double time) = 0;
		virtual double get_val() const = 0;
		virtual double get_extraction_val() const = 0;
		virtual std::string get_val_text() const =0;
		virtual std::vector<uint8_t> get_data() const = 0;
		virtual boost::signals2::connection connect_val_charged_signal(const val_charged_slot_t& slot) = 0;
		virtual void read_form_buffer(const std::vector<uint8_t>& buffer, uint32_t start_idx) = 0;
		virtual void set_primary_conversion(primary_conversion_fun_t primary_conversion) = 0;
		virtual void set_secondary_conversion(secondary_conversion_fun_t secondary_conversion) = 0;
		virtual void set_text_conversion(text_conversion_fun_t text_conversion) = 0;
	};

}

#endif // TM_PARAMETER_HPP

