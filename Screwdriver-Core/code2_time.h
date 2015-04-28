#ifndef CODE2_TIME_HPP
#define CODE2_TIME_HPP

#if defined(_MSC_VER)
# pragma once
#endif

#include "tm_time.h"


namespace TM
{
	class __declspec(dllexport) code2_time :
		public tm_time
	{
	public:
		code2_time(void);
		~code2_time(void);
		virtual double get_time() const override;
		virtual void read_form_buffer(const std::vector<uint8_t>& buffer, uint32_t start_idx) override;
		virtual boost::signals2::connection connect_time_charged_signal(const time_charged_slot_t& slot) override;
	private:
		struct code2_time_imp_t;
		boost::shared_ptr<code2_time_imp_t> imp_;
	};
}

#endif // CODE2_TIME_HPP

