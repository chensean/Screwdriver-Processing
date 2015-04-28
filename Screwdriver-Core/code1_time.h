#ifndef CODE1_TIME_HPP
#define CODE1_TIME_HPP

#if defined(_MSC_VER)
# pragma once
#endif

#include "tm_time.h"


namespace TM
{
	class __declspec(dllexport) code1_time :
		public tm_time
	{
	public:
		code1_time(void);
		~code1_time(void);
		virtual double get_time() const override;
		virtual void read_form_buffer(const std::vector<uint8_t>& buffer, uint32_t start_idx) override;
		virtual boost::signals2::connection connect_time_charged_signal(const time_charged_slot_t& slot) override;
	private:
		struct code1_time_imp_t;
		boost::shared_ptr<code1_time_imp_t> imp_;
	};
}

#endif // CODE1_TIME_HPP

