#ifndef SAFE_SINGLETON_HPP
#define SAFE_SINGLETON_HPP

#if defined(_MSC_VER)
# pragma once
#endif

#include <boost/noncopyable.hpp>
#define BOOST_THREAD_DYN_LINK
#include <boost/thread/once.hpp>

template <typename T>
class safe_singleton
	:private boost::noncopyable
{
public:
	typedef T real_type;

	static real_type& instance()
	{
		boost::call_once(flag_, init);
		return init();
	}

private:
	static real_type& init()
	{
		static real_type obj;
		return obj;
	}

	static boost::once_flag flag_;
};

template <typename T>
boost::once_flag safe_singleton<T>::flag_ = BOOST_ONCE_INIT;

#endif // SAFE_SINGLETON_HPP

