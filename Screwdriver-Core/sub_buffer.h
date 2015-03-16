#ifndef SUB_BUFFER_HPP
#define SUB_BUFFER_HPP

#if defined(_MSC_VER)
# pragma once
#endif

#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace TM
{
	class sub_buffer
		:private boost::noncopyable
	{
	public:
		sub_buffer();
		~sub_buffer(void);
		uint32_t get_length() const;
		void add_data(const std::vector<uint8_t>& data,uint32_t start_idx);
	private:
		struct sub_buffer_imp_t;
		boost::shared_ptr<sub_buffer_imp_t> imp_;
	};
	typedef boost::shared_ptr<sub_buffer> sub_buffer_ptr;
}
#endif // SUB_BUFFER_HPP


