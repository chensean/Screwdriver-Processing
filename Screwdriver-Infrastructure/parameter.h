#ifndef PARAMETER_HPP
#define PARAMETER_HPP

#if defined(_MSC_VER)
# pragma once
#endif
#define BOOST_ALL_DYN_LINK
#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/signals2/signal.hpp>
#include "Screwdriver-Infrastructure.h"
#include "parameter_value.h"


namespace screwdriver
{
	class parameter;
	typedef boost::shared_ptr<parameter> parameter_ptr;
	typedef boost::shared_ptr<parameter_value> parameter_value_ptr;
	typedef boost::signals2::signal<void(const parameter_value_ptr&)> parameter_charged_signal_t;
	typedef parameter_charged_signal_t::slot_type parameter_charged_slot_t;

	class
		SCREWDRIVER_INFRASTRUCTURE_API parameter
		:public boost::enable_shared_from_this<parameter>,
		 private boost::noncopyable
	{
	public:
		explicit parameter(const std::string& name, uint32_t bits_count);
		~parameter(void);
		std::string get_name() const;
		uint32_t get_bits_count() const;
		void set_value(double time, double val, uint64_t code);
		boost::signals2::connection connect_val_charged_signal(const parameter_charged_slot_t& slot);
	private:
		struct parameter_imp_t;
		boost::shared_ptr<parameter_imp_t> imp_;
	};
}
#endif // PARAMETER_HPP

