#ifndef RTR_TM_CLIENT_HPP
#define RTR_TM_CLIENT_HPP

#if defined(_MSC_VER)
# pragma once
#endif
#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>


namespace screwdriver
{
	typedef boost::shared_ptr<std::vector<uint8_t>> tm_data_ptr;
	typedef std::function<void(const tm_data_ptr&)> parse_data_fun_t;

	class rtr_tm_client
		:private boost::noncopyable
	{
	public:
		rtr_tm_client(const std::string& ip, uint16_t port, int tm_channel, const parse_data_fun_t& fun);
		~rtr_tm_client(void);
		void start();
		void stop();
	private:
		struct rtr_tm_client_imp_t;
		boost::shared_ptr<rtr_tm_client_imp_t> imp_;
	};
}
#endif // RTR_TM_CLIENT_HPP

