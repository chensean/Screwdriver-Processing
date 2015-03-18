#ifndef BOUNDED_BUFFER_HPP
#define BOUNDED_BUFFER_HPP

#if defined(_MSC_VER)
# pragma once
#endif
# pragma warning( push )
# pragma warning(disable:4913)
# pragma warning(disable:4172)
#include <boost/circular_buffer.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/thread.hpp>
# pragma warning( pop )
#include <boost/call_traits.hpp>
#include <boost/progress.hpp>
#include <boost/bind.hpp>

template <class T>
class bounded_buffer_space_optimized
{
public:

	typedef boost::circular_buffer_space_optimized<T> container_type;
	typedef typename container_type::size_type size_type;
	typedef typename container_type::value_type value_type;
	typedef typename boost::call_traits<value_type>::param_type param_type;

	explicit bounded_buffer_space_optimized(size_type capacity) : container_(capacity)
	{
	}

	void push_front(param_type item)
	{
		boost::mutex::scoped_lock lock(mutex_);
		//not_full_.wait(lock, boost::bind(&bounded_buffer_space_optimized<value_type>::is_not_full, this));
		container_.push_front(item);
		lock.unlock();
		not_empty_.notify_one();
	}

	void pop_back(value_type* pItem)
	{
		boost::mutex::scoped_lock lock(mutex_);
		not_empty_.wait(lock, boost::bind(&bounded_buffer_space_optimized<value_type>::is_not_empty, this));
		*pItem = container_.back();
		container_.pop_back();
		lock.unlock();
		//not_full_.notify_one();
	}

	void clear()
	{
		boost::mutex::scoped_lock lock(mutex_);
		container_.clear();
	}

	size_t size()
	{
		boost::mutex::scoped_lock lock(mutex_);
		return container_.size();
	}

	bool is_not_empty() const
	{
		return container_.size() > 0;
	}

	bool is_not_full() const
	{
		return container_.size() < container_.capacity();
	}

private:
	bounded_buffer_space_optimized(const bounded_buffer_space_optimized&); // Disabled copy constructor
	bounded_buffer_space_optimized& operator =(const bounded_buffer_space_optimized&); // Disabled assign operator
private:
	container_type container_;
	boost::mutex mutex_;
	boost::condition not_empty_;
	//boost::condition not_full_;
};

#endif // BOUNDED_BUFFER_HPP

