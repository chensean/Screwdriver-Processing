#pragma once
#include "tm_parameter.h"

class slot_test
{
public:
	slot_test(){}
	~slot_test(){}
	double Val_f() const { return val_f_; }
	std::string Val_string() const { return val_string_; }
	void receive_signal(TM::tm_parameter* param)
	{
		assert(param!=nullptr);
		val_f_=param->get_val_f();
		val_string_=param->get_val_text();
	}
private:
	std::string val_string_;
	double val_f_;
};