#include "polynomial.h"

struct polynomial::polynomial_imp_t
{
	std::vector<double> coefficient;
};

polynomial::polynomial(const std::vector<double>& coefficient)
	:imp_(new polynomial_imp_t)
{
	imp_->coefficient = coefficient;
}


polynomial::~polynomial(void)
{
}

double polynomial::code_to_val(uint64_t code)
{
	double val = 0.;
	for (size_t i = 0; i < imp_->coefficient.size(); ++i)
	{
		val += imp_->coefficient[i] * pow(code, i);
	}
	return val;
}
