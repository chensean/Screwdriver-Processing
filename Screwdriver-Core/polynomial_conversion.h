#ifndef POLYNOMIAL_CONVERSION_HPP
#define POLYNOMIAL_CONVERSION_HPP

#if defined(_MSC_VER)
# pragma once
#endif

#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

class __declspec(dllexport) polynomial_conversion
	:private boost::noncopyable
{
public:
	explicit polynomial_conversion(const std::vector<double>& coefficient);
	~polynomial_conversion(void);
	double convert(double pre_val);
private:
	struct polynomial_conversion_imp_t;
	boost::shared_ptr<polynomial_conversion_imp_t> imp_;
};

#endif // POLYNOMIAL_CONVERSION_HPP

