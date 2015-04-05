#include "polynomial_conversion.h"

struct polynomial_conversion::polynomial_conversion_imp_t
{
	std::vector<double> coefficient;
};

polynomial_conversion::polynomial_conversion(const std::vector<double>& coefficient)
	:imp_(new polynomial_conversion_imp_t)
{
	imp_->coefficient = coefficient;
}


polynomial_conversion::~polynomial_conversion(void)
{
}

double polynomial_conversion::convert(double pre_val)
{
	double val = 0.;
	for (size_t i = 0; i < imp_->coefficient.size(); ++i)
	{
		val += imp_->coefficient[i] * pow(pre_val, i);
	}
	return val;
}

