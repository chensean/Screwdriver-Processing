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
	typedef std::function<void(const tm_data_ptr&)> parse_data_fun_t;

	class rtr_data_parser
		:private boost::noncopyable
	{
	public:
		explicit rtr_data_parser(const parse_data_fun_t& fun=nullptr);
		~rtr_data_parser(void);
		void start();
		void stop();
		void receive(iterator_type, iterator_type);
		void set_parse_data_fun(const parse_data_fun_t& fun);
	private:
		struct rtr_data_parser_imp_t;
		boost::shared_ptr<rtr_data_parser_imp_t> imp_;
	};
}
#endif // RTR_DATA_PARSER_HPP

