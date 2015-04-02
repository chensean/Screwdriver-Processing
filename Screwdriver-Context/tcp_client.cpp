#include "tcp_client.h"
#include <SDKDDKVer.h>
#define BOOST_ALL_DYN_LINK
# pragma warning( push )
# pragma warning(disable:4913)
#include <boost/thread/thread.hpp>
# pragma warning( pop )
#include <boost/asio.hpp>

class tcp_client;
const size_t BUFFER_MAX_SIZE = 8 * 1024 * 1024;
using boost::asio::ip::tcp;

namespace screwdriver
{
	typedef boost::shared_ptr<tcp::socket> sock_ptr;

	struct tcp_client::tcp_client_imp_t
	{
		tcp_client_imp_t(const std::string& ip, uint16_t port, parser_fun_t parser_fun)
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


	tcp_client::tcp_client(const std::string& ip, uint16_t port, parser_fun_t parser_fun)
		:imp_(new tcp_client_imp_t(ip, port, parser_fun))

	{
	}

	tcp_client::~tcp_client(void)
	{
	}

	void tcp_client::start_receive()
	{
		imp_->sp_->async_read_some(boost::asio::buffer(imp_->read_buffer_),
		                           boost::bind(&tcp_client::handle_receive, this,
		                                       boost::asio::placeholders::error,
		                                       boost::asio::placeholders::bytes_transferred));
	}

	bool tcp_client::connect_server()
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

	void tcp_client::start()
	{
		if (!connect_server()) return;
		start_receive();
		imp_->thread_ = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&boost::asio::io_service::run, &imp_->ios_)));
	}

	void tcp_client::stop()
	{
		if (imp_->sp_->is_open())
		{
			boost::system::error_code ignored_ec;
			imp_->sp_->shutdown(tcp::socket::shutdown_both, ignored_ec);
			imp_->sp_->close();
		}
		if (!imp_->ios_.stopped())
		{
			imp_->ios_.stop();
			imp_->ios_.reset();
		}
	}

	void tcp_client::send_data(const std::vector<int32_t>& data)
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

	void tcp_client::send_data(const std::vector<uint8_t>& data)
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

	void tcp_client::async_send_data(const data_ptr& data)
	{
		if (imp_->sp_->is_open())
		{
			imp_->sp_->async_write_some(boost::asio::buffer(*data),
			                            boost::bind(&tcp_client::handle_send, this,
			                                        data,
			                                        boost::asio::placeholders::error,
			                                        boost::asio::placeholders::bytes_transferred));
		}
	}

	void tcp_client::handle_receive(const boost::system::error_code& e, std::size_t bytes_transferred)
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
			stop();
		}
	}

	void tcp_client::handle_send(const data_ptr& data, const boost::system::error_code& e, std::size_t bytes_transferred)
	{
		if (e)
		{
			stop();
		}
	}
}
