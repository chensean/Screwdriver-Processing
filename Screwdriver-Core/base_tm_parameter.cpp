#include "base_tm_parameter.h"
#include <loki/Singleton.h>
#include <loki/Factory.h>

namespace TM
{
	typedef base_tm_parameter<8> uint_parameter8;
	typedef base_tm_parameter<8, big_endian_int_extract> int_parameter8;
	typedef base_tm_parameter<16> big_endian_uint_parameter16;
	typedef base_tm_parameter<16, big_endian_int_extract> big_endian_int_parameter16;
	typedef base_tm_parameter<24> big_endian_uint_parameter24;
	typedef base_tm_parameter<32> big_endian_uint_parameter32;
	typedef base_tm_parameter<32, big_endian_int_extract> big_endian_int_parameter32;
	typedef base_tm_parameter<64> big_endian_uint_parameter64;
	typedef base_tm_parameter<64, big_endian_int_extract> big_endian_int_parameter64;
	typedef base_tm_parameter<32, big_endian_float_extract> big_endian_float_parameter32;
	typedef base_tm_parameter<64, big_endian_double_extract> big_endian_double_parameter64;

	typedef base_tm_parameter<16, little_endian_uint_extract> little_endian_uint_parameter16;
	typedef base_tm_parameter<16, little_endian_int_extract> little_endian_int_parameter16;
	typedef base_tm_parameter<32, little_endian_uint_extract> little_endian_uint_parameter32;
	typedef base_tm_parameter<32, little_endian_int_extract> little_endian_int_parameter32;
	typedef base_tm_parameter<64, little_endian_uint_extract> little_endian_uint_parameter64;
	typedef base_tm_parameter<64, little_endian_int_extract> little_endian_int_parameter64;
	typedef base_tm_parameter<32, little_endian_float_extract> little_endian_float_parameter32;
	typedef base_tm_parameter<64, little_endian_double_extract> little_endian_double_parameter64;

	using namespace Loki;
	typedef SingletonHolder<
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
	const bool registered1=TM::regist_base_tm_parameter("uint_parameter8",&TM::create_with_name<TM::uint_parameter8>::create);
	const bool registered2=TM::regist_base_tm_parameter("int_parameter8",&TM::create_with_name<TM::int_parameter8>::create);
	const bool registered3=TM::regist_base_tm_parameter("big_endian_uint_parameter16",&TM::create_with_name<TM::big_endian_uint_parameter16>::create);
	const bool registered4=TM::regist_base_tm_parameter("big_endian_int_parameter16",&TM::create_with_name<TM::big_endian_int_parameter16>::create);
	const bool registered5=TM::regist_base_tm_parameter("big_endian_uint_parameter24",&TM::create_with_name<TM::big_endian_uint_parameter24>::create);
	const bool registered6=TM::regist_base_tm_parameter("big_endian_uint_parameter32",&TM::create_with_name<TM::big_endian_uint_parameter32>::create);
	const bool registered7=TM::regist_base_tm_parameter("big_endian_int_parameter32",&TM::create_with_name<TM::big_endian_int_parameter32>::create);
	const bool registered8=TM::regist_base_tm_parameter("big_endian_uint_parameter64",&TM::create_with_name<TM::big_endian_uint_parameter64>::create);
	const bool registered9=TM::regist_base_tm_parameter("big_endian_int_parameter64",&TM::create_with_name<TM::big_endian_int_parameter64>::create);
	const bool registered10=TM::regist_base_tm_parameter("big_endian_double_parameter64",&TM::create_with_name<TM::big_endian_double_parameter64>::create);
	const bool registered11=TM::regist_base_tm_parameter("big_endian_float_parameter32",&TM::create_with_name<TM::big_endian_float_parameter32>::create);
	const bool registered12=TM::regist_base_tm_parameter("big_endian_double_parameter64",&TM::create_with_name<TM::big_endian_double_parameter64>::create);
	const bool registered13=TM::regist_base_tm_parameter("little_endian_uint_parameter16",&TM::create_with_name<TM::little_endian_uint_parameter16>::create);
	const bool registered14=TM::regist_base_tm_parameter("little_endian_int_parameter16",&TM::create_with_name<TM::little_endian_int_parameter16>::create);
	const bool registered15=TM::regist_base_tm_parameter("little_endian_uint_parameter32",&TM::create_with_name<TM::little_endian_uint_parameter32>::create);
	const bool registered16=TM::regist_base_tm_parameter("little_endian_int_parameter32",&TM::create_with_name<TM::little_endian_int_parameter32>::create);
	const bool registered17=TM::regist_base_tm_parameter("little_endian_uint_parameter64",&TM::create_with_name<TM::little_endian_uint_parameter64>::create);
	const bool registered18=TM::regist_base_tm_parameter("little_endian_int_parameter64",&TM::create_with_name<TM::little_endian_int_parameter64>::create);
	const bool registered19=TM::regist_base_tm_parameter("little_endian_float_parameter32",&TM::create_with_name<TM::little_endian_float_parameter32>::create);
	const bool registered20=TM::regist_base_tm_parameter("little_endian_double_parameter64",&TM::create_with_name<TM::little_endian_double_parameter64>::create);
}			   
