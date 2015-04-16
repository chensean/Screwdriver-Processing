#ifndef LINEAR_CONVERSION_HPP
#define LINEAR_CONVERSION_HPP

#if defined(_MSC_VER)
# pragma once
#endif

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

class __declspec(dllexport) linear_conversion
	:private boost::noncopyable
{
public:
	explicit linear_conversion(double a1, double a0);
	~linear_conversion(void);
	double convert(double pre_val);
private:
	struct linear_conversion_imp_t;
	boost::shared_ptr<linear_conversion_imp_t> imp_;
};

#endif // LINEAR_CONVERSION_HPP

