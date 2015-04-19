#include "rtr_tm_client.h"
#include <functional>
# pragma warning( push )
#pragma warning(disable:4512)
#pragma warning(disable:4244)
#include <boost/assign.hpp>
# pragma warning( pop )
#include <boost/bind.hpp>
#include "tcp_client.h"
#include "rtr_data_parser.h"
#include "digit_utilities.h"

using namespace boost::assign;

const size_t BUFFER_MAX_SIZE = 8 * 1024 * 1024;
const uint16_t RTR_TM_PORT = 3070;

namespace screwdriver
{
	struct rtr_tm_client::rtr_tm_client_imp_t
	{
		rtr_tm_client_imp_t(const std::string& ip, uint16_t port, int tm_channel, const parse_data_fun_t& fun)
			:rtr_data_parser_(new rtr_data_parser(fun)),
			 tcp_client_(new tcp_client(ip, port, boost::bind(&rtr_data_parser::receive, rtr_data_parser_, _1, _2)))
		{
			start_command_ += 1234567890 , 64 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -1234567890;
			start_command_[3] = tm_channel;
			std::transform(start_command_.begin(), start_command_.end(), start_command_.begin(), std::ptr_fun(utilities::reverse_endian<int32_t>));
			stop_command_ += 1234567890 , 64 , 0 , 0 , 0 , 0x80 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , -1234567890;
			stop_command_[3] = tm_channel;
			std::transform(stop_command_.begin(), stop_command_.end(), stop_command_.begin(), std::ptr_fun(utilities::reverse_endian<int32_t>));
		}

		boost::shared_ptr<rtr_data_parser> rtr_data_parser_;
		boost::shared_ptr<tcp_client> tcp_client_;
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

	void rtr_tm_client::start()
	{
		imp_->rtr_data_parser_->start();
		imp_->tcp_client_->start();
		imp_->tcp_client_->send_data(imp_->start_command_);
	}

	void rtr_tm_client::stop()
	{
		imp_->tcp_client_->send_data(imp_->stop_command_);
		imp_->rtr_data_parser_->stop();
		imp_->tcp_client_->stop();
	}
}
