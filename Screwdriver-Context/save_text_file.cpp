#include "save_text_file.h"
#define BOOST_ALL_DYN_LINK
#include <fstream>
# pragma warning( push )
# pragma warning(disable:4913)
#include <boost/thread/thread.hpp>
# pragma warning( pop )
# pragma warning( push )
#pragma warning(disable:4005)
#define BOOST_FILESYSTEM_NO_DEPRECATED
#include <boost/filesystem.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
# pragma warning( pop )
#include <boost/make_shared.hpp>
#include "bounded_buffer_space_optimized.h"
#include "string_utilities.h"

namespace fs = boost::filesystem;
const size_t BUFFER_MAX_SIZE = 1000;
const size_t FILE_SIZE_MAX = 100 * 1024 * 1024;

using namespace boost::posix_time;
using namespace boost::gregorian;

namespace screwdriver
{
	struct save_text_file::save_text_file_imp_t
	{
		explicit save_text_file_imp_t(const std::string& prefix)
			: prefix_(prefix),
			  tm_param_buffer_(BUFFER_MAX_SIZE),
			  start_time_((time_from_string("2015-01-01 00:00:00.000")))
		{
		}

		std::ofstream file_stream_;
		std::string data_folder_;
		std::string prefix_;
		std::string save_path_;
		boost::shared_ptr<boost::thread> thread_;
		bounded_buffer_space_optimized<parameter_value_ptr> tm_param_buffer_;
		ptime start_time_;
	};


	save_text_file::save_text_file(const std::string& prefix)
		:imp_(new save_text_file_imp_t(prefix))
	{
	}

	save_text_file::~save_text_file(void)
	{
	}

	void save_text_file::start(const std::string& folder)
	{
		imp_->data_folder_ = folder;
		open_file();
		imp_->thread_ = boost::make_shared<boost::thread>(
			[=]()
			{
				while (!boost::this_thread::interruption_requested() || imp_->tm_param_buffer_.is_not_empty())
				{
					parameter_value_ptr parameter_value;
					imp_->tm_param_buffer_.pop_back(&parameter_value);
					save2file(parameter_value);
				}
			});
	}

	void save_text_file::stop()
	{
		if (imp_->thread_)
		{
			imp_->thread_->interrupt();
			imp_->thread_->join();
			imp_->thread_.reset();
			close_file();
		}
	}

	void save_text_file::receive(const parameter_value_ptr& param_val)
	{
		if (imp_->thread_)
		{
			imp_->tm_param_buffer_.push_front(param_val);
		}
	}


	void save_text_file::split_file()
	{
		try
		{
			fs::path p(imp_->save_path_);
			if (exists(p) && is_regular_file(p) && file_size(p) > FILE_SIZE_MAX)
			{
				close_file();
				open_file();
			}
		}
		catch (const fs::filesystem_error&)
		{
		}
	}

	std::string save_text_file::get_time_file_name()
	{
		std::string time_string = utilities::get_current_time_string();
		boost::replace_all(time_string, ":", "-");
		std::string file_name(imp_->prefix_);
		file_name += "(";
		file_name += time_string;
		file_name += ")";
		file_name += ".txt";
		return file_name;
	}


	void save_text_file::open_file()
	{
		if (imp_->data_folder_.empty())
		{
			return;
		}
		fs::path p(imp_->data_folder_);
		if (!exists(p))
		{
			return;
		}
		imp_->save_path_ = imp_->data_folder_ + "/" + get_time_file_name();
		imp_->file_stream_.open(imp_->save_path_.c_str(), std::ios::binary);
	}

	void save_text_file::close_file()
	{
		imp_->file_stream_.clear();
		imp_->file_stream_.close();
		fs::path p(imp_->save_path_);
		if (exists(p) && is_empty(p))
		{
			remove(p);
		}
	}

	void save_text_file::save2file(const parameter_value_ptr& param)
	{
		if (imp_->file_stream_.is_open())
		{
			time_duration td = microseconds(boost::numeric_cast<uint64_t>(param->get_time() * 1e6));
			ptime time = imp_->start_time_ + td;
			auto time_string = to_iso_extended_string(time);
			imp_->file_stream_ << time_string << ": " << param->get_value() << ": " << utilities::to_hex_string(param->get_code()) << "\r\n";
			split_file();
		}
	}
}
