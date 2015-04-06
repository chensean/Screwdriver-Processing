#ifndef RTR_DATA_PARSER_HPP
#define RTR_DATA_PARSER_HPP

#if defined(_MSC_VER)
# pragma once
#endif
#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace screwdriver
{
	typedef boost::shared_ptr<std::vector<uint8_t>> tm_data_ptr;
	typedef std::vector<uint8_t>::iterator iterator_type;
	typedef std::function<void(const tm_data_ptr&, uint32_t)> parse_frame_fun_t;

	class __declspec(dllexport) rtr_data_parser
		:private boost::noncopyable
	{
	public:
		rtr_data_parser(const std::string& folder);
		~rtr_data_parser(void);
		void save2file(tm_data_ptr data_ptr);
		void start();
		void stop();
		void receive(iterator_type, iterator_type);
		void set_parse_frame_data_fun(parse_frame_fun_t fun);
		void set_parse_frame_time_fun(parse_frame_fun_t fun);
	private:
		struct rtr_data_parser_imp_t;
		boost::shared_ptr<rtr_data_parser_imp_t> imp_;
	};
}
#endif // RTR_DATA_PARSER_HPP

