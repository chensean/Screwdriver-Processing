#ifndef SAVE_TEXT_FILE_HPP
#define SAVE_TEXT_FILE_HPP

#if defined(_MSC_VER)
# pragma once
#endif

#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include "parameter.h"

namespace screwdriver
{
	class save_text_file
		:private boost::noncopyable
	{
	public:
		explicit save_text_file(const std::string& prefix);
		~save_text_file(void);
		void start(const std::string& folder);
		void stop();
		void receive(const parameter_value_ptr& param_val);
	private:
		void split_file();
		std::string get_time_file_name();
		void open_file();
		void close_file();
		void save2file(const parameter_value_ptr& param_val);
	private:
		struct save_text_file_imp_t;
		boost::shared_ptr<save_text_file_imp_t> imp_;
	};

	typedef boost::shared_ptr<save_text_file> save_text_file_ptr;
}
#endif // SAVE_TEXT_FILE_HPP

