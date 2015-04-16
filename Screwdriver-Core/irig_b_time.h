#ifndef IRIG_B_TIME_HPP
#define IRIG_B_TIME_HPP

#if defined(_MSC_VER)
# pragma once
#endif

#include "tm_time.h"


namespace TM
{
	class __declspec(dllexport) irig_b_time :
		public tm_time
	{
	public:
		irig_b_time(void);
		~irig_b_time(void);
		virtual double get_time() const override;
		virtual void read_form_buffer(const std::vector<uint8_t>& buffer, uint32_t start_idx) override;
	private:
		struct irig_b_time_imp_t;
		boost::shared_ptr<irig_b_time_imp_t> imp_;
	};
}

#endif // IRIG_B_TIME_HPP

