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
	typedef boost::shared_ptr<std::vector<uint8_t>> data_ptr;
	typedef std::vector<uint8_t>::iterator iterator_type;
	typedef std::function<void(iterator_type, iterator_type)> parser_fun_t;

	class __declspec(dllexport) rtr_monitor_client
		:private boost::noncopyable
	{
	public:
		rtr_monitor_client(const std::string& ip, parser_fun_t parser_fun);
		~rtr_monitor_client(void);
		void start();
		void stop();
	private:
		bool connect_server();
	private:
		struct rtr_monitor_client_imp_t;
		boost::shared_ptr<rtr_monitor_client_imp_t> imp_;
	};
}
#endif // RTR_MONITOR_CLIENT_HPP

