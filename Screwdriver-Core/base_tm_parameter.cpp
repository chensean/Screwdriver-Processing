#include "base_tm_parameter.h"

namespace TM
{

	using namespace Loki;
	typedef SingletonHolder
	<
		Factory<tm_parameter, std::string,Seq<const std::string&>>,
		CreateUsingNew,
		DeletableSingleton
	>
	base_tm_parameter_factory;

	bool regist_base_tm_parameter(const std::string& tm_parameter_type, base_tm_parameter_creator creator)
	{
		return base_tm_parameter_factory::Instance().Register(tm_parameter_type,creator);
	}

	bool unregist_base_tm_parameter(const std::string& tm_parameter_type)
	{
		return base_tm_parameter_factory::Instance().Unregister(tm_parameter_type);
	}

	tm_parameter_ptr create_base_tm_parameter(const std::string& tm_parameter_type,const std::string& tm_parameter_name)
	{
		return tm_parameter_ptr(base_tm_parameter_factory::Instance().CreateObject(tm_parameter_type,tm_parameter_name));
	}
}

namespace
{
	const bool registered1=TM::regist_base_tm_parameter("parameter8",&TM::create_with_name<TM::parameter8>::create);
	const bool registered2=TM::regist_base_tm_parameter("double_parameter8",&TM::create_with_name<TM::double_parameter8>::create);
	const bool registered3=TM::regist_base_tm_parameter("big_endian_parameter16",&TM::create_with_name<TM::big_endian_parameter16>::create);
	const bool registered4=TM::regist_base_tm_parameter("big_endian_double_parameter16",&TM::create_with_name<TM::big_endian_double_parameter16>::create);
	const bool registered5=TM::regist_base_tm_parameter("big_endian_parameter24",&TM::create_with_name<TM::big_endian_parameter24>::create);
	const bool registered6=TM::regist_base_tm_parameter("big_endian_parameter32",&TM::create_with_name<TM::big_endian_parameter32>::create);
	const bool registered7=TM::regist_base_tm_parameter("big_endian_float_parameter32",&TM::create_with_name<TM::big_endian_float_parameter32>::create);
	const bool registered8=TM::regist_base_tm_parameter("big_endian_double_parameter32",&TM::create_with_name<TM::big_endian_double_parameter32>::create);
	const bool registered9=TM::regist_base_tm_parameter("big_endian_parameter64",&TM::create_with_name<TM::big_endian_parameter64>::create);
	const bool registered10=TM::regist_base_tm_parameter("big_endian_double_parameter64",&TM::create_with_name<TM::big_endian_double_parameter64>::create);
	const bool registered11=TM::regist_base_tm_parameter("little_endian_parameter16",&TM::create_with_name<TM::little_endian_parameter16>::create);
	const bool registered12=TM::regist_base_tm_parameter("little_endian_double_parameter16",&TM::create_with_name<TM::little_endian_double_parameter16>::create);
	const bool registered13=TM::regist_base_tm_parameter("little_endian_parameter24",&TM::create_with_name<TM::little_endian_parameter24>::create);
	const bool registered14=TM::regist_base_tm_parameter("little_endian_double_parameter32",&TM::create_with_name<TM::little_endian_double_parameter32>::create);
	const bool registered15=TM::regist_base_tm_parameter("little_endian_float_parameter32",&TM::create_with_name<TM::little_endian_float_parameter32>::create);
	const bool registered16=TM::regist_base_tm_parameter("little_endian_parameter64",&TM::create_with_name<TM::little_endian_parameter64>::create);
	const bool registered17=TM::regist_base_tm_parameter("little_endian_double_parameter64",&TM::create_with_name<TM::little_endian_double_parameter64>::create);
}			   
