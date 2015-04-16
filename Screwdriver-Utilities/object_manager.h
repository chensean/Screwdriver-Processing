#ifndef OBJECT_MANAGER_HPP
#define OBJECT_MANAGER_HPP

#if defined(_MSC_VER)
# pragma once
#endif
#include <string>
#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>

template <class T>
class object_manager
	:private boost::noncopyable
{
public:
	typedef boost::shared_ptr<T> object_ptr;
	typedef boost::unordered_map<std::string, object_ptr> object_container_t;

	object_manager(void)
	{
	}

	~object_manager(void)
	{
	}

	object_ptr get_object(const std::string& name)
	{
		return object_container_.find(name) != object_container_.end() ? object_container_[name] : object_ptr(nullptr);
	}

	void add_object(const std::string& name, const object_ptr& param)
	{
		object_container_[name] = param;
	}

	void remove_object(const std::string& name)
	{
		object_container_.erase(name);
	}

	std::vector<object_ptr> get_all_objects()
	{
		std::vector<object_ptr> objects(object_container_.size());
		std::transform(object_container_.begin(), object_container_.end(), objects.begin()
		               , [](const std::pair<std::string, object_ptr>& object_pair)
		               {
			               return object_pair.second;
		               });
		return objects;
	}

private:
	object_container_t object_container_;
};

#endif // OBJECT_MANAGER_HPP

