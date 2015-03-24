#ifndef TCP_CLIENT_HPP
#define TCP_CLIENT_HPP

#if defined(_MSC_VER)
# pragma once
#endif
#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#define BOOST_SYSTEM_DYN_LINK
#include <boost/system/error_code.hpp>

namespace screwdriver
{
	typedef boost::shared_ptr<std::vector<uint8_t>> data_ptr;
	typedef std::vector<uint8_t>::iterator iterator_type;
	typedef std::function<void(iterator_type, iterator_type)> parser_fun_t;

	class __declspec(dllexport) tcp_client
		:private boost::noncopyable
	{
	public:
		tcp_client(const std::string& ip, uint16_t port, parser_fun_t parser_fun);
		~tcp_client(void);
		void start();
		void stop();
		void send_data(const std::vector<int32_t>& data);
		void send_data(const std::vector<uint8_t>& data);
		void async_send_data(const data_ptr& data);
	private:
		void start_receive();
		bool connect_server();
		void handle_receive(const boost::system::error_code& e, std::size_t bytes_transferred);
		void handle_send(const data_ptr& data, const boost::system::error_code& e, std::size_t bytes_transferred);
	private:
		struct tcp_client_imp_t;
		boost::shared_ptr<tcp_client_imp_t> imp_;
	};
}
#endif // TCP_CLIENT_HPP

