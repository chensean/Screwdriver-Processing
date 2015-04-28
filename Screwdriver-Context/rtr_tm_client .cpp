#include "rtr_tm_client.h"
#include <SDKDDKVer.h>
#include <functional>
#define BOOST_ALL_DYN_LINK
# pragma warning( push )
#pragma warning(disable:4512)
#pragma warning(disable:4244)
#include <boost/assign.hpp>
# pragma warning( pop )
#include <boost/bind.hpp>
#include <boost/make_shared.hpp>
# pragma warning( push )
# pragma warning(disable:4913)
#include <boost/thread/thread.hpp>
# pragma warning( pop )
#include <boost/asio.hpp>
# pragma warning( push )
#pragma warning(disable:4512)
#pragma warning(disable:4244)
#include "rtr_data_parser.h"
#include "digit_utilities.h"

using namespace boost::assign;

const size_t BUFFER_MAX_SIZE = 8 * 1024 * 1024;
const uint16_t RTR_TM_PORT = 3070;

using boost::asio::ip::tcp;

namespace screwdriver
{
	typedef boost::shared_ptr<tcp::socket> sock_ptr;

	struct rtr_tm_client::rtr_tm_client_imp_t
	{
		rtr_tm_client_imp_t(const std::string& ip, uint16_t port, int tm_channel, const parse_data_fun_t& fun)
			:ep_(boost::asio::ip::address::from_string(ip), port),
			 sp_(new tcp::socket(ios_)),
			 read_buffer_(BUFFER_MAX_SIZE),
			 rtr_data_parser_(new rtr_data_parser(fun)),
			 is_connected_(false)
		{
			start_command_ += 1234567890 , 64 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -1234567890;
			start_command_[3] = tm_channel;
			std::transform(start_command_.begin(), start_command_.end(), start_command_.begin(), std::ptr_fun(utilities::reverse_endian<int32_t>));
			stop_command_ += 1234567890 , 64 , 0 , 0 , 0 , 0x80 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -1234567890;
			stop_command_[3] = tm_channel;
			std::transform(stop_command_.begin(), stop_command_.end(), stop_command_.begin(), std::ptr_fun(utilities::reverse_endian<int32_t>));
		}

		boost::asio::io_service ios_;
		tcp::endpoint ep_;
		sock_ptr sp_;
		boost::shared_ptr<boost::thread> thread_;
		std::vector<uint8_t> read_buffer_;
		boost::shared_ptr<rtr_data_parser> rtr_data_parser_;
		bool is_connected_;
		std::vector<int32_t> start_command_;
		std::vector<int32_t> stop_command_;
	};


	rtr_tm_client::rtr_tm_client(const std::string& ip, uint16_t port, int tm_channel, const parse_data_fun_t& fun)
		:imp_(new rtr_tm_client_imp_t(ip, port, tm_channel, fun))

	{
	}

	rtr_tm_client::~rtr_tm_client(void)
	{
	}

	bool rtr_tm_client::connect_server()
	{
		if (!imp_->is_connected_)
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
			imp_->is_connected_ = true;
			imp_->rtr_data_parser_->start();
			imp_->thread_ = boost::make_shared<boost::thread>(
				[=]()
				{
					while (!boost::this_thread::interruption_requested())
					{
						boost::this_thread::interruption_point();
						try
						{
							boost::system::error_code ec;
							size_t bytes_transferred = imp_->sp_->read_some(boost::asio::buffer(imp_->read_buffer_), ec);
							if (!ec)
							{
								imp_->rtr_data_parser_->receive(imp_->read_buffer_.begin(), imp_->read_buffer_.begin() + bytes_transferred);
							}
							else
							{
								std::cerr << "½ÓÊÕCRTÒ£²â´íÎó£¡" << ec.message() << std::endl;
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
		return true;
	}

	bool rtr_tm_client::disconnect_server()
	{
		if (imp_->is_connected_)
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
			imp_->rtr_data_parser_->stop();

			if (imp_->thread_)
			{
				imp_->thread_->interruption_requested();
			}

			imp_->is_connected_ = false;
		}
		return true;
	}

	void rtr_tm_client::start()
	{
		if (!imp_->is_connected_) return;
		send_data(imp_->start_command_);
	}

	void rtr_tm_client::stop()
	{
		send_data(imp_->stop_command_);
		//imp_->rtr_data_parser_->stop();

		//if (imp_->thread_)
		//{
		//	imp_->thread_->interruption_requested();
		//}
	}

	void rtr_tm_client::send_data(const std::vector<int32_t>& data)
	{
		if (imp_->sp_->is_open())
		{
			boost::system::error_code ec;
			imp_->sp_->write_some(boost::asio::buffer(data), ec);
		}
	}
}
