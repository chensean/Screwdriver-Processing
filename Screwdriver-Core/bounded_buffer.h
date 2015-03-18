#ifndef BOUNDED_BUFFER_HPP
#define BOUNDED_BUFFER_HPP

#if defined(_MSC_VER)
# pragma once
#endif
# pragma warning( push )
# pragma warning(disable:4913)
#include <boost/circular_buffer.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/thread.hpp>
# pragma warning( pop )
#include <boost/call_traits.hpp>
#include <boost/progress.hpp>
#include <boost/bind.hpp>

template <class T>
class bounded_buffer
{
public:

	typedef boost::circular_buffer<T> container_type;
	typedef typename container_type::size_type size_type;
	typedef typename container_type::value_type value_type;
	typedef typename std::vector<T>::iterator iterator_type;
	typedef typename boost::call_traits<value_type>::param_type param_type;

	explicit bounded_buffer(size_type capacity) : container_(capacity)
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
		not_empty_.wait(lock, boost::bind(&bounded_buffer<value_type>::is_not_empty, this));
		*pItem = container_.back();
		container_.pop_back();
		lock.unlock();
		//not_full_.notify_one();
	}


	void push_back_array(iterator_type begin, iterator_type end)
	{
		boost::mutex::scoped_lock lock(mutex_);
		//not_full_.wait(lock, boost::bind(&bounded_buffer_space_optimized<value_type>::is_not_full, this));
		std::copy(begin, end, back_inserter(container_));
		lock.unlock();
		not_empty_.notify_one();
	}

	boost::shared_ptr<std::vector<value_type>> pop_front_array()
	{
		boost::mutex::scoped_lock lock(mutex_);
		not_empty_.wait(lock, boost::bind(&bounded_buffer<value_type>::is_not_empty, this));
		boost::shared_ptr<std::vector<value_type>> item_array(new std::vector<value_type>(container_.size()));
		std::copy(container_.begin(), container_.end(), item_array->begin());
		container_.clear();
		lock.unlock();
		return item_array;
	}

	size_t size()
	{
		boost::mutex::scoped_lock lock(mutex_);
		return container_.size();
	}

	void clear()
	{
		boost::mutex::scoped_lock lock(mutex_);
		container_.clear();
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
	bounded_buffer(const bounded_buffer&);
	bounded_buffer& operator =(const bounded_buffer&);
private:
	container_type container_;
	boost::mutex mutex_;
	boost::condition not_empty_;
	//boost::condition not_full_;
};

#endif // BOUNDED_BUFFER_HPP

