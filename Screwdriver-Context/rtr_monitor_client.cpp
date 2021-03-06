#include "rtr_monitor_client.h"
#include <SDKDDKVer.h>
#define BOOST_ALL_DYN_LINK
#include <boost/make_shared.hpp>
# pragma warning( push )
# pragma warning(disable:4913)
#include <boost/thread/thread.hpp>
# pragma warning( pop )
#include <boost/asio.hpp>
# pragma warning( push )
#pragma warning(disable:4512)
#pragma warning(disable:4244)
#include <boost/assign.hpp>
# pragma warning( pop )
#include <boost/function.hpp>
#include "rtr_data_parser.h"
using namespace boost::assign;

const uint16_t RTR_MONITOR_PORT = 3000;
const size_t BUFFER_MAX_SIZE = 8 * 1024 * 1024;
using boost::asio::ip::tcp;

namespace screwdriver
{
	typedef boost::shared_ptr<tcp::socket> sock_ptr;

	struct rtr_monitor_client::rtr_monitor_client_imp_t
	{
		rtr_monitor_client_imp_t(const std::string& ip, parse_data_fun_t parser_fun)
			: ep_(boost::asio::ip::address::from_string(ip), RTR_MONITOR_PORT),
			  sp_(new tcp::socket(ios_)),
			  read_buffer_(BUFFER_MAX_SIZE),
			  rtr_data_parser_(new rtr_data_parser(parser_fun))
		{
			cmd_ += 1234567890 , 20 , 0 , 0x5001 - 1234567890;
			std::transform(cmd_.begin(), cmd_.end(), cmd_.begin(), boost::function<int(int)>(htonl));
		}

		boost::asio::io_service ios_;
		tcp::endpoint ep_;
		sock_ptr sp_;
		boost::shared_ptr<boost::thread> thread_;
		std::vector<uint8_t> read_buffer_;
		boost::shared_ptr<rtr_data_parser> rtr_data_parser_;
		std::vector<int> cmd_;
	};


	rtr_monitor_client::rtr_monitor_client(const std::string& ip, parse_data_fun_t parser_fun)
		:imp_(new rtr_monitor_client_imp_t(ip, parser_fun))
	{
	}

	rtr_monitor_client::~rtr_monitor_client(void)
	{
	}

	bool rtr_monitor_client::connect_server()
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

	void rtr_monitor_client::start()
	{
		if (!connect_server()) return;

		imp_->thread_ = boost::make_shared<boost::thread>(
			[=]()
			{
				while (!boost::this_thread::interruption_requested())
				{
					try
					{
						send_data(imp_->cmd_);
						boost::system::error_code ec;
						size_t bytes_transferred = imp_->sp_->read_some(boost::asio::buffer(imp_->read_buffer_), ec);
						if (!ec)
						{
							imp_->rtr_data_parser_->receive(imp_->read_buffer_.begin(), imp_->read_buffer_.begin() + bytes_transferred);
						}
						else
						{
							std::cerr << "���մ���" << ec.message() << std::endl;
							imp_->sp_->close();
							break;
						}
					}
					catch (std::exception& e)
					{
						std::cerr << e.what() << std::endl;
						imp_->sp_->close();
						break;
					}
				}
			}
		);
	}

	void rtr_monitor_client::stop()
	{
		if (imp_->sp_->is_open())
		{
			boost::system::error_code ignored_ec;
			imp_->sp_->shutdown(tcp::socket::shutdown_both, ignored_ec);
		}
		if (imp_->thread_)
		{
			imp_->thread_->join();
			imp_->thread_.reset();
		}
		imp_->sp_->close();
	}

	void rtr_monitor_client::send_data(const std::vector<int32_t>& data)
	{
		if (imp_->sp_->is_open())
		{
			boost::system::error_code ec;
			imp_->sp_->write_some(boost::asio::buffer(data), ec);
		}
	}
}
