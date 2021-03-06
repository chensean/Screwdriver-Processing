#include "save_data_file.h"
#define BOOST_ALL_DYN_LINK
#include <fstream>
# pragma warning( push )
# pragma warning(disable:4913)
#include <boost/thread/thread.hpp>
# pragma warning( pop )
#include <boost/config/warning_disable.hpp>
# pragma warning( push )
#pragma warning(disable:4005)
#define BOOST_FILESYSTEM_NO_DEPRECATED
#include <boost/filesystem.hpp>
#include <boost/filesystem/operations.hpp>
# pragma warning( pop )
#include <boost/algorithm/string.hpp>
#include <boost/make_shared.hpp>
#include "bounded_buffer_space_optimized.h"
#include "string_utilities.h"

namespace fs = boost::filesystem;
const size_t BUFFER_MAX_SIZE = 1000;
const size_t FILE_SIZE_MAX = 100 * 1024 * 1024;

namespace screwdriver
{
	struct save_data_file::save_data_file_imp_t
	{
		explicit save_data_file_imp_t(const std::string& prefix)
			: prefix_(prefix),
			  tm_data_buffer_(BUFFER_MAX_SIZE)
		{
		}

		std::ofstream file_stream_;
		std::string data_folder_;
		std::string prefix_;
		std::string save_path_;
		boost::shared_ptr<boost::thread> thread_;
		bounded_buffer_space_optimized<tm_data_ptr> tm_data_buffer_;
	};


	save_data_file::save_data_file(const std::string& prefix)
		:imp_(new save_data_file_imp_t(prefix))
	{
	}

	save_data_file::~save_data_file(void)
	{
	}

	void save_data_file::start(const std::string& folder)
	{
		imp_->data_folder_ = folder;
		open_file();
		imp_->thread_ = boost::make_shared<boost::thread>(
			[=]()
			{
				while (!boost::this_thread::interruption_requested() || imp_->tm_data_buffer_.is_not_empty())
				{
					tm_data_ptr data;
					imp_->tm_data_buffer_.pop_back(&data);
					save2file(data);
				}
			});
	}

	void save_data_file::stop()
	{
		if (imp_->thread_)
		{
			imp_->thread_->interrupt();
			imp_->thread_->join();
			imp_->thread_.reset();
			close_file();
		}
	}

	void save_data_file::receive(const tm_data_ptr& data)
	{
		if (imp_->thread_)
		{
			imp_->tm_data_buffer_.push_front(data);
		}
	}

	void save_data_file::split_file()
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

	std::string save_data_file::get_time_file_name()
	{
		std::string time_string = utilities::get_current_time_string();
		boost::replace_all(time_string, ":", "-");
		std::string file_name(imp_->prefix_);
		file_name += "(";
		file_name += time_string;
		file_name += ")";
		file_name += ".dat";
		return file_name;
	}


	void save_data_file::open_file()
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

	void save_data_file::close_file()
	{
		imp_->file_stream_.clear();
		imp_->file_stream_.close();
		fs::path p(imp_->save_path_);
		if (exists(p) && is_empty(p))
		{
			remove(p);
		}
	}

	void save_data_file::save2file(const tm_data_ptr& data)
	{
		if (!data->empty() && imp_->file_stream_.is_open())
		{
			imp_->file_stream_.write(reinterpret_cast<const char*>(data->data()), data->size());
			auto current_time = utilities::get_current_time_string();
			imp_->file_stream_ << current_time;
			imp_->file_stream_.flush();
			split_file();
		}
	}
}
