#ifndef TM_PARAMETER_HPP
#define TM_PARAMETER_HPP

#if defined(_MSC_VER)
# pragma once
#endif

#include <boost/cstdint.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/variant.hpp>
#include <boost/signals2.hpp>
#include <loki/Factory.h>
#include <loki/Functor.h>
#include <loki/Sequence.h>


namespace TM
{
	typedef boost::variant<int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t, float, double> tm_val_type;
	class tm_parameter;
	typedef boost::shared_ptr<tm_parameter> tm_parameter_ptr;
	typedef boost::signals2::signal<void(tm_parameter*)> code_charged_signal_t;
	typedef std::function<std::string(tm_parameter*)> to_val_text_fun_t;

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
		virtual tm_val_type get_val() const = 0;
		virtual double get_val_f() const = 0;
		virtual std::string get_val_text() const =0;
		virtual std::vector<uint8_t> get_data() const = 0;
		virtual uint64_t get_code() const = 0;
		virtual boost::signals2::connection connect_signal(const code_charged_signal_t::slot_type& slot) = 0;
		virtual void read_form_buffer(const std::vector<uint8_t>& buffer, uint32_t start_idx) = 0;
	};

}

#endif // TM_PARAMETER_HPP

