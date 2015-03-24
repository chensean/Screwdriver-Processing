#ifndef RTR_TM_CLIENT_HPP
#define RTR_TM_CLIENT_HPP

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

	class __declspec(dllexport) rtr_tm_client
		:private boost::noncopyable
	{
	public:
		rtr_tm_client(const std::string& ip, const std::string& folder);
		~rtr_tm_client(void);
		void start();
		void stop();
	private:
		struct rtr_tm_client_imp_t;
		boost::shared_ptr<rtr_tm_client_imp_t> imp_;
	};
}
#endif // RTR_TM_CLIENT_HPP

