#ifndef PARAMETER_HPP
#define PARAMETER_HPP

#if defined(_MSC_VER)
# pragma once
#endif
#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/signals2/signal.hpp>
#include "Screwdriver-Infrastructure.h"


namespace screwdriver
{
	class parameter;
	typedef boost::shared_ptr<parameter> parameter_ptr;
	typedef boost::signals2::signal<void(const parameter_ptr&)> parameter_charged_signal_t;
	typedef parameter_charged_signal_t::slot_type parameter_charged_slot_t;

	class SCREWDRIVER_INFRASTRUCTURE_API parameter
		:public boost::enable_shared_from_this<parameter>,
		 private boost::noncopyable
	{
	public:
		explicit parameter(const std::string& name);
		~parameter(void);
		std::string get_name() const;
		double get_time() const;
		double get_value() const;
		void set_value(double time, double val);
		boost::signals2::connection connect_val_charged_signal(const parameter_charged_slot_t& slot);
	private:
		struct parameter_imp_t;
		boost::shared_ptr<parameter_imp_t> imp_;
	};
}
#endif // PARAMETER_HPP

