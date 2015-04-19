#ifndef TM_TIME_HPP
#define TM_TIME_HPP

#if defined(_MSC_VER)
# pragma once
#endif

#include <vector>
#include <boost/cstdint.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/signals2/signal.hpp>
#include "../Screwdriver-Infrastructure/parameter.h"


namespace TM
{
	class tm_time;
	typedef boost::shared_ptr<tm_time> tm_time_ptr;
	typedef boost::signals2::signal<void(double)> time_charged_signal_t;
	typedef time_charged_signal_t::slot_type time_charged_slot_t;

	class __declspec(dllexport) tm_time : public boost::enable_shared_from_this<tm_time>
	{
	public:
		tm_time(void)
		{
		}

		virtual ~tm_time(void)
		{
		}

		virtual double get_time() const = 0;
		virtual void read_form_buffer(const std::vector<uint8_t>& buffer, uint32_t start_idx) = 0;
		virtual boost::signals2::connection connect_time_charged_signal(const time_charged_slot_t& slot) =0;
	};
}

#endif // TM_TIME_HPP

