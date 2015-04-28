#include "asynchronous_tcp_client.h"
#include <SDKDDKVer.h>
#define BOOST_ALL_DYN_LINK
# pragma warning( push )
# pragma warning(disable:4913)
#include <boost/thread/thread.hpp>
# pragma warning( pop )
#include <boost/asio.hpp>
#include <boost/make_shared.hpp>

class asynchronous_tcp_client;
const size_t BUFFER_MAX_SIZE = 8 * 1024 * 1024;
using boost::asio::ip::tcp;

namespace screwdriver
{
	typedef boost::shared_ptr<tcp::socket> sock_ptr;

	struct asynchronous_tcp_client::asynchronous_tcp_client_imp_t
	{
		asynchronous_tcp_client_imp_t(const std::string& ip, uint16_t port, parser_fun_t parser_fun)
			: ep_(boost::asio::ip::address::from_string(ip), port),
			  sp_(new tcp::socket(ios_)),
			  read_buffer_(BUFFER_MAX_SIZE),
			  parser_fun_(parser_fun)
		{
		}

		boost::asio::io_service ios_;
		tcp::endpoint ep_;
		sock_ptr sp_;
		boost::shared_ptr<boost::thread> thread_;
		std::vector<uint8_t> read_buffer_;
		parser_fun_t parser_fun_;
	};


	asynchronous_tcp_client::asynchronous_tcp_client(const std::string& ip, uint16_t port, parser_fun_t parser_fun)
		:imp_(new asynchronous_tcp_client_imp_t(ip, port, parser_fun))

	{
	}

	asynchronous_tcp_client::~asynchronous_tcp_client(void)
	{
	}

	void asynchronous_tcp_client::start_receive()
	{
		imp_->sp_->async_read_some(boost::asio::buffer(imp_->read_buffer_),
		                           boost::bind(&asynchronous_tcp_client::handle_receive, this,
		                                       boost::asio::placeholders::error,
		                                       boost::asio::placeholders::bytes_transferred));
	}

	bool asynchronous_tcp_client::connect_server()
	{
		try
		{
			boost::system::error_code ec;
			imp_->sp_->connect(imp_->ep_, ec);
			if (ec)
			{
				std::cerr << ec.message() << std::endl;
				return false;
			}
		}
		catch (std::exception& e)
		{
			std::cerr << e.what() << std::endl;
			return false;
		}
		return true;
	}

	void asynchronous_tcp_client::start()
	{
		imp_->sp_ = boost::make_shared<tcp::socket>(imp_->ios_);
		if (!connect_server()) return;
		start_receive();
		imp_->thread_ = boost::make_shared<boost::thread>(boost::bind(&boost::asio::io_service::run, &imp_->ios_));
	}

	void asynchronous_tcp_client::stop()
	{
		if (imp_->sp_ && imp_->sp_->is_open())
		{
			boost::system::error_code ignored_ec;
			imp_->sp_->shutdown(tcp::socket::shutdown_both, ignored_ec);
			imp_->sp_->close();
			imp_->sp_.reset();
		}
		if (!imp_->ios_.stopped())
		{
			imp_->ios_.stop();
			imp_->ios_.reset();
		}
	}

	void asynchronous_tcp_client::send_data(const std::vector<int32_t>& data)
	{
		if (imp_->sp_->is_open())
		{
			boost::system::error_code ec;
			imp_->sp_->write_some(boost::asio::buffer(data), ec);
			if (ec)
			{
				std::cerr << ec.message() << std::endl;
				stop();
			}
		}
	}

	void asynchronous_tcp_client::send_data(const std::vector<uint8_t>& data)
	{
		if (imp_->sp_->is_open())
		{
			boost::system::error_code ec;
			imp_->sp_->write_some(boost::asio::buffer(data), ec);
			if (ec)
			{
				std::cerr << ec.message() << std::endl;
				stop();
			}
		}
	}

	void asynchronous_tcp_client::async_send_data(const data_ptr& data)
	{
		if (imp_->sp_->is_open())
		{
			imp_->sp_->async_write_some(boost::asio::buffer(*data),
			                            boost::bind(&asynchronous_tcp_client::handle_send, this,
			                                        data,
			                                        boost::asio::placeholders::error,
			                                        boost::asio::placeholders::bytes_transferred));
		}
	}

	void asynchronous_tcp_client::handle_receive(const boost::system::error_code& e, std::size_t bytes_transferred)
	{
		if (!e)
		{
			if (bytes_transferred > 0 && imp_->parser_fun_ != nullptr)
			{
				imp_->parser_fun_(imp_->read_buffer_.begin(), imp_->read_buffer_.begin() + bytes_transferred);
			}
			start_receive();
		}
		else
		{
			//stop();
		}
	}

	void asynchronous_tcp_client::handle_send(const data_ptr& data, const boost::system::error_code& e, std::size_t bytes_transferred)
	{
		if (e)
		{
			//stop();
		}
	}
}
