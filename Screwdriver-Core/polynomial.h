#pragma once
#include <memory>
#include <vector>
#include <boost/noncopyable.hpp>

class __declspec(dllexport) polynomial
	:private boost::noncopyable
{
public:
	explicit polynomial(const std::vector<double>& coefficient);
	~polynomial(void); 
	double code_to_val(uint64_t code);
private:
	struct polynomial_imp;
	std::shared_ptr<polynomial_imp> imp_;
};

