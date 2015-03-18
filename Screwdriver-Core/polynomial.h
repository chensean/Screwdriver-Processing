#ifndef POLYNOMIAL_HPP
#define POLYNOMIAL_HPP

#if defined(_MSC_VER)
# pragma once
#endif

#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

class __declspec(dllexport) polynomial
	:private boost::noncopyable
{
public:
	explicit polynomial(const std::vector<double>& coefficient);
	~polynomial(void);
	double code_to_val(uint64_t code);
private:
	struct polynomial_imp_t;
	boost::shared_ptr<polynomial_imp_t> imp_;
};

#endif // POLYNOMIAL_HPP

