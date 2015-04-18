#ifndef TASK_PROCESSOR_HPP
#define TASK_PROCESSOR_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)


#include <boost/thread/thread.hpp>
#include <boost/asio/io_service.hpp>

namespace detail
{
	template <class T>
	struct task_wrapped
	{
	private:
		T task_unwrapped_;

	public:
		explicit task_wrapped(const T& task_unwrapped)
			: task_unwrapped_(task_unwrapped)
		{
		}

		void operator()() const
		{
			// resetting interruption
			try
			{
				boost::this_thread::interruption_point();
			}
			catch (const boost::thread_interrupted&)
			{
			}

			try
			{
				// Executing task
				task_unwrapped_();
			}
			catch (const std::exception& e)
			{
				std::cerr << "Exception: " << e.what() << '\n';
			}
			catch (const boost::thread_interrupted&)
			{
				std::cerr << "Thread interrupted\n";
			}
			catch (...)
			{
				std::cerr << "Unknown exception\n";
			}
		}
	};

	template <class T>
	inline task_wrapped<T> make_task_wrapped(const T& task_unwrapped)
	{
		return task_wrapped<T>(task_unwrapped);
	}
} // namespace detail


class tasks_processor: private boost::noncopyable
{
protected:
	boost::asio::io_service ios_;
	boost::asio::io_service::work work_;

	tasks_processor()
		: ios_()
		  , work_(ios_)
	{
	}

public:

	template <class T>
	inline void push_task(const T& task_unwrapped)
	{
		ios_.post(detail::make_task_wrapped(task_unwrapped));
	}

	void start()
	{
		ios_.run();
	}

	void stop()
	{
		ios_.stop();
	}

	void start_multiple(std::size_t threads_count = 0)
	{
		if (!threads_count)
		{
			threads_count = (std::max)(static_cast<int>(
				                           boost::thread::hardware_concurrency()), 1
			);
		}


		boost::thread_group tg;
		for (std::size_t i = 0; i < threads_count; ++i)
		{
			tg.create_thread(boost::bind(&boost::asio::io_service::run, boost::ref(ios_)));
		}

		tg.join_all();
	}
}; // tasks_processor


#endif // TASK_PROCESSOR_HPP

