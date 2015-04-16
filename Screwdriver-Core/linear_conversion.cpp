#include "linear_conversion.h"

struct linear_conversion::linear_conversion_imp_t
{
	linear_conversion_imp_t(double a1, double a0)
		: a1_(a1),
		  a0_(a0)
	{
	}

	double a1_;
	double a0_;
};


linear_conversion::linear_conversion(double a1, double a0)
	:imp_(new linear_conversion_imp_t(a1, a0))
{
}

linear_conversion::~linear_conversion(void)
{
}

double linear_conversion::convert(double pre_val)
{
	return imp_->a1_ * pre_val + imp_->a0_;
}
