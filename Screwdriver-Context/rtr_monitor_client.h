#ifndef RTR_MONITOR_CLIENT_HPP
#define RTR_MONITOR_CLIENT_HPP

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

	class __declspec(dllexport) rtr_monitor_client
		:private boost::noncopyable
	{
	public:
		rtr_monitor_client(const std::string& ip, parse_data_fun_t parser_fun);
		~rtr_monitor_client(void);
		void start();
		void stop();
		void send_data(const std::vector<int32_t>& data);
	private:
		bool connect_server();
	private:
		struct rtr_monitor_client_imp_t;
		boost::shared_ptr<rtr_monitor_client_imp_t> imp_;
	};
}
#endif // RTR_MONITOR_CLIENT_HPP

