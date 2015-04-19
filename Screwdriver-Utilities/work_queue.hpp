#ifndef WORK_QUEUE_HPP
#define WORK_QUEUE_HPP
#define BOOST_ALL_DYN_LINK

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)



#include <deque>
#include <boost/function.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/condition_variable.hpp>

class work_queue
{
public:
	typedef boost::function<void()> task_type;

private:
	std::deque<task_type> tasks_;
	boost::mutex mutex_;
	boost::condition_variable cond_;
	bool is_stopped_;

public:
	work_queue()
		: is_stopped_(false)
	{
	}

	void stop()
	{
		boost::unique_lock<boost::mutex> lock(mutex_);
		is_stopped_ = true;
		lock.unlock();
		cond_.notify_all();
	}

	void push_task(const task_type& task)
	{
		boost::unique_lock<boost::mutex> lock(mutex_);
		if (is_stopped_)
		{
			return;
		}
		tasks_.push_back(task);
		lock.unlock();
		cond_.notify_one();
	}

	task_type pop_task()
	{
		boost::unique_lock<boost::mutex> lock(mutex_);
		while (tasks_.empty())
		{
			if (is_stopped_)
			{
				return task_type();
			}
			cond_.wait(lock);
		}

		task_type ret = tasks_.front();
		tasks_.pop_front();

		return ret;
	}
};

#endif // WORK_QUEUE_HPP

