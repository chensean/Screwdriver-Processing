#ifndef RAW_DATA_HPP
#define RAW_DATA_HPP

#if defined(_MSC_VER)
# pragma once
#endif
#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/signals2/signal.hpp>


namespace screwdriver
{
	class raw_data;
	typedef boost::shared_ptr<raw_data> raw_data_ptr;
	typedef boost::shared_ptr<std::vector<uint8_t>> data_ptr;
	typedef boost::signals2::signal<void(const data_ptr&)> raw_data_charged_signal_t;
	typedef raw_data_charged_signal_t::slot_type raw_data_charged_slot_t;

	class __declspec(dllexport) raw_data
		:public boost::enable_shared_from_this<raw_data>,
		 private boost::noncopyable
	{
	public:
		explicit raw_data(const std::string& name);
		~raw_data(void);
		std::string get_name() const;
		void set_data(const data_ptr& data);
		boost::signals2::connection connect_val_charged_signal(const raw_data_charged_slot_t& slot);
	private:
		struct raw_data_imp_t;
		boost::shared_ptr<raw_data_imp_t> imp_;
	};
}
#endif // RAW_DATA_HPP

