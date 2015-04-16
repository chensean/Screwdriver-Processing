#include "bits_tm_parameter.h"
#include <loki/Singleton.h>
#include <loki/Factory.h>

namespace TM
{
	using namespace Loki;
	typedef SingletonHolder
	<
		Factory<tm_parameter, std::string, Seq<const std::string&, uint32_t>>,
		CreateUsingNew,
		DeletableSingleton
	>
	bits_tm_parameter_factory;


	bool regist_bits_tm_parameter(const std::string& tm_parameter_type, bits_tm_parameter_creator creator)
	{
		return bits_tm_parameter_factory::Instance().Register(tm_parameter_type, creator);
	}

	bool unregist_bits_tm_parameter(const std::string& tm_parameter_type)
	{
		return bits_tm_parameter_factory::Instance().Unregister(tm_parameter_type);
	}

	tm_parameter_ptr create_bits_tm_parameter(const std::string& tm_parameter_type, const std::string& tm_parameter_name, uint32_t bit_start_idx)
	{
		return tm_parameter_ptr(bits_tm_parameter_factory::Instance().CreateObject(tm_parameter_type, tm_parameter_name, bit_start_idx));
	}
}

namespace
{
	const bool registered1 = TM::regist_bits_tm_parameter("bits8_parameter1", &TM::create_with_name_and_position<TM::bits8_parameter1>::create);
	const bool registered2 = TM::regist_bits_tm_parameter("bits8_parameter2", &TM::create_with_name_and_position<TM::bits8_parameter2>::create);
	const bool registered3 = TM::regist_bits_tm_parameter("bits8_parameter3", &TM::create_with_name_and_position<TM::bits8_parameter3>::create);
	const bool registered4 = TM::regist_bits_tm_parameter("bits8_parameter4", &TM::create_with_name_and_position<TM::bits8_parameter4>::create);
	const bool registered5 = TM::regist_bits_tm_parameter("bits8_parameter5", &TM::create_with_name_and_position<TM::bits8_parameter5>::create);
	const bool registered6 = TM::regist_bits_tm_parameter("bits8_parameter6", &TM::create_with_name_and_position<TM::bits8_parameter6>::create);
	const bool registered7 = TM::regist_bits_tm_parameter("bits8_parameter7", &TM::create_with_name_and_position<TM::bits8_parameter7>::create);
	const bool registered8 = TM::regist_bits_tm_parameter("big_endian_bits16_parameter2", &TM::create_with_name_and_position<TM::big_endian_bits16_parameter2>::create);
	const bool registered9 = TM::regist_bits_tm_parameter("big_endian_bits16_parameter3", &TM::create_with_name_and_position<TM::big_endian_bits16_parameter3>::create);
	const bool registered10 = TM::regist_bits_tm_parameter("big_endian_bits16_parameter4", &TM::create_with_name_and_position<TM::big_endian_bits16_parameter4>::create);
	const bool registered11 = TM::regist_bits_tm_parameter("big_endian_bits16_parameter5", &TM::create_with_name_and_position<TM::big_endian_bits16_parameter5>::create);
	const bool registered12 = TM::regist_bits_tm_parameter("big_endian_bits16_parameter6", &TM::create_with_name_and_position<TM::big_endian_bits16_parameter6>::create);
	const bool registered13 = TM::regist_bits_tm_parameter("big_endian_bits16_parameter7", &TM::create_with_name_and_position<TM::big_endian_bits16_parameter7>::create);
	const bool registered14 = TM::regist_bits_tm_parameter("big_endian_bits16_parameter8", &TM::create_with_name_and_position<TM::big_endian_bits16_parameter8>::create);
	const bool registered15 = TM::regist_bits_tm_parameter("big_endian_bits16_parameter9", &TM::create_with_name_and_position<TM::big_endian_bits16_parameter9>::create);
	const bool registered16 = TM::regist_bits_tm_parameter("big_endian_bits16_parameter10", &TM::create_with_name_and_position<TM::big_endian_bits16_parameter10>::create);
	const bool registered17 = TM::regist_bits_tm_parameter("big_endian_bits16_parameter11", &TM::create_with_name_and_position<TM::big_endian_bits16_parameter11>::create);
	const bool registered18 = TM::regist_bits_tm_parameter("big_endian_bits16_parameter12", &TM::create_with_name_and_position<TM::big_endian_bits16_parameter12>::create);
	const bool registered19 = TM::regist_bits_tm_parameter("big_endian_bits16_parameter13", &TM::create_with_name_and_position<TM::big_endian_bits16_parameter13>::create);
	const bool registered20 = TM::regist_bits_tm_parameter("big_endian_bits16_parameter14", &TM::create_with_name_and_position<TM::big_endian_bits16_parameter14>::create);
	const bool registered21 = TM::regist_bits_tm_parameter("big_endian_bits16_parameter15", &TM::create_with_name_and_position<TM::big_endian_bits16_parameter15>::create);
	const bool registered22 = TM::regist_bits_tm_parameter("big_endian_bits32_parameter10", &TM::create_with_name_and_position<TM::big_endian_bits32_parameter10>::create);
	const bool registered23 = TM::regist_bits_tm_parameter("big_endian_bits32_parameter11", &TM::create_with_name_and_position<TM::big_endian_bits32_parameter11>::create);
	const bool registered24 = TM::regist_bits_tm_parameter("big_endian_bits32_parameter12", &TM::create_with_name_and_position<TM::big_endian_bits32_parameter12>::create);
	const bool registered25 = TM::regist_bits_tm_parameter("big_endian_bits32_parameter13", &TM::create_with_name_and_position<TM::big_endian_bits32_parameter13>::create);
	const bool registered26 = TM::regist_bits_tm_parameter("big_endian_bits32_parameter14", &TM::create_with_name_and_position<TM::big_endian_bits32_parameter14>::create);
	const bool registered27 = TM::regist_bits_tm_parameter("big_endian_bits32_parameter15", &TM::create_with_name_and_position<TM::big_endian_bits32_parameter15>::create);
	const bool registered28 = TM::regist_bits_tm_parameter("big_endian_bits32_parameter16", &TM::create_with_name_and_position<TM::big_endian_bits32_parameter16>::create);
	const bool registered29 = TM::regist_bits_tm_parameter("big_endian_bits32_parameter17", &TM::create_with_name_and_position<TM::big_endian_bits32_parameter17>::create);
	const bool registered30 = TM::regist_bits_tm_parameter("big_endian_bits32_parameter18", &TM::create_with_name_and_position<TM::big_endian_bits32_parameter18>::create);
	const bool registered31 = TM::regist_bits_tm_parameter("big_endian_bits32_parameter19", &TM::create_with_name_and_position<TM::big_endian_bits32_parameter19>::create);
	const bool registered32 = TM::regist_bits_tm_parameter("big_endian_bits32_parameter20", &TM::create_with_name_and_position<TM::big_endian_bits32_parameter20>::create);
	const bool registered33 = TM::regist_bits_tm_parameter("big_endian_bits32_parameter21", &TM::create_with_name_and_position<TM::big_endian_bits32_parameter21>::create);
	const bool registered34 = TM::regist_bits_tm_parameter("big_endian_bits32_parameter22", &TM::create_with_name_and_position<TM::big_endian_bits32_parameter22>::create);
	const bool registered35 = TM::regist_bits_tm_parameter("big_endian_bits32_parameter23", &TM::create_with_name_and_position<TM::big_endian_bits32_parameter23>::create);
	const bool registered36 = TM::regist_bits_tm_parameter("big_endian_bits32_parameter24", &TM::create_with_name_and_position<TM::big_endian_bits32_parameter24>::create);
	const bool registered37 = TM::regist_bits_tm_parameter("big_endian_bits32_parameter25", &TM::create_with_name_and_position<TM::big_endian_bits32_parameter25>::create);
	const bool registered38 = TM::regist_bits_tm_parameter("big_endian_bits32_parameter26", &TM::create_with_name_and_position<TM::big_endian_bits32_parameter26>::create);
	const bool registered39 = TM::regist_bits_tm_parameter("big_endian_bits32_parameter27", &TM::create_with_name_and_position<TM::big_endian_bits32_parameter27>::create);
	const bool registered40 = TM::regist_bits_tm_parameter("big_endian_bits32_parameter28", &TM::create_with_name_and_position<TM::big_endian_bits32_parameter28>::create);
	const bool registered41 = TM::regist_bits_tm_parameter("big_endian_bits32_parameter29", &TM::create_with_name_and_position<TM::big_endian_bits32_parameter29>::create);
	const bool registered42 = TM::regist_bits_tm_parameter("big_endian_bits32_parameter30", &TM::create_with_name_and_position<TM::big_endian_bits32_parameter30>::create);
	const bool registered43 = TM::regist_bits_tm_parameter("big_endian_bits32_parameter31", &TM::create_with_name_and_position<TM::big_endian_bits32_parameter31>::create);
	const bool registered45 = TM::regist_bits_tm_parameter("little_endian_bits16_parameter2", &TM::create_with_name_and_position<TM::little_endian_bits16_parameter2>::create);
	const bool registered46 = TM::regist_bits_tm_parameter("little_endian_bits16_parameter3", &TM::create_with_name_and_position<TM::little_endian_bits16_parameter3>::create);
	const bool registered47 = TM::regist_bits_tm_parameter("little_endian_bits16_parameter4", &TM::create_with_name_and_position<TM::little_endian_bits16_parameter4>::create);
	const bool registered48 = TM::regist_bits_tm_parameter("little_endian_bits16_parameter5", &TM::create_with_name_and_position<TM::little_endian_bits16_parameter5>::create);
	const bool registered49 = TM::regist_bits_tm_parameter("little_endian_bits16_parameter6", &TM::create_with_name_and_position<TM::little_endian_bits16_parameter6>::create);
	const bool registered50 = TM::regist_bits_tm_parameter("little_endian_bits16_parameter7", &TM::create_with_name_and_position<TM::little_endian_bits16_parameter7>::create);
	const bool registered51 = TM::regist_bits_tm_parameter("little_endian_bits16_parameter8", &TM::create_with_name_and_position<TM::little_endian_bits16_parameter8>::create);
	const bool registered52 = TM::regist_bits_tm_parameter("little_endian_bits16_parameter9", &TM::create_with_name_and_position<TM::little_endian_bits16_parameter9>::create);
	const bool registered53 = TM::regist_bits_tm_parameter("little_endian_bits16_parameter10", &TM::create_with_name_and_position<TM::little_endian_bits16_parameter10>::create);
	const bool registered54 = TM::regist_bits_tm_parameter("little_endian_bits16_parameter11", &TM::create_with_name_and_position<TM::little_endian_bits16_parameter11>::create);
	const bool registered55 = TM::regist_bits_tm_parameter("little_endian_bits16_parameter12", &TM::create_with_name_and_position<TM::little_endian_bits16_parameter12>::create);
	const bool registered56 = TM::regist_bits_tm_parameter("little_endian_bits16_parameter13", &TM::create_with_name_and_position<TM::little_endian_bits16_parameter13>::create);
	const bool registered57 = TM::regist_bits_tm_parameter("little_endian_bits16_parameter14", &TM::create_with_name_and_position<TM::little_endian_bits16_parameter14>::create);
	const bool registered58 = TM::regist_bits_tm_parameter("little_endian_bits16_parameter15", &TM::create_with_name_and_position<TM::little_endian_bits16_parameter15>::create);
	const bool registered59 = TM::regist_bits_tm_parameter("little_endian_bits32_parameter10", &TM::create_with_name_and_position<TM::little_endian_bits32_parameter10>::create);
	const bool registered60 = TM::regist_bits_tm_parameter("little_endian_bits32_parameter11", &TM::create_with_name_and_position<TM::little_endian_bits32_parameter11>::create);
	const bool registered61 = TM::regist_bits_tm_parameter("little_endian_bits32_parameter12", &TM::create_with_name_and_position<TM::little_endian_bits32_parameter12>::create);
	const bool registered62 = TM::regist_bits_tm_parameter("little_endian_bits32_parameter13", &TM::create_with_name_and_position<TM::little_endian_bits32_parameter13>::create);
	const bool registered63 = TM::regist_bits_tm_parameter("little_endian_bits32_parameter14", &TM::create_with_name_and_position<TM::little_endian_bits32_parameter14>::create);
	const bool registered64 = TM::regist_bits_tm_parameter("little_endian_bits32_parameter15", &TM::create_with_name_and_position<TM::little_endian_bits32_parameter15>::create);
	const bool registered65 = TM::regist_bits_tm_parameter("little_endian_bits32_parameter16", &TM::create_with_name_and_position<TM::little_endian_bits32_parameter16>::create);
	const bool registered66 = TM::regist_bits_tm_parameter("little_endian_bits32_parameter17", &TM::create_with_name_and_position<TM::little_endian_bits32_parameter17>::create);
	const bool registered67 = TM::regist_bits_tm_parameter("little_endian_bits32_parameter18", &TM::create_with_name_and_position<TM::little_endian_bits32_parameter18>::create);
	const bool registered68 = TM::regist_bits_tm_parameter("little_endian_bits32_parameter19", &TM::create_with_name_and_position<TM::little_endian_bits32_parameter19>::create);
	const bool registered69 = TM::regist_bits_tm_parameter("little_endian_bits32_parameter20", &TM::create_with_name_and_position<TM::little_endian_bits32_parameter20>::create);
	const bool registered70 = TM::regist_bits_tm_parameter("little_endian_bits32_parameter21", &TM::create_with_name_and_position<TM::little_endian_bits32_parameter21>::create);
	const bool registered71 = TM::regist_bits_tm_parameter("little_endian_bits32_parameter22", &TM::create_with_name_and_position<TM::little_endian_bits32_parameter22>::create);
	const bool registered72 = TM::regist_bits_tm_parameter("little_endian_bits32_parameter23", &TM::create_with_name_and_position<TM::little_endian_bits32_parameter23>::create);
	const bool registered73 = TM::regist_bits_tm_parameter("little_endian_bits32_parameter24", &TM::create_with_name_and_position<TM::little_endian_bits32_parameter24>::create);
	const bool registered74 = TM::regist_bits_tm_parameter("little_endian_bits32_parameter25", &TM::create_with_name_and_position<TM::little_endian_bits32_parameter25>::create);
	const bool registered75 = TM::regist_bits_tm_parameter("little_endian_bits32_parameter26", &TM::create_with_name_and_position<TM::little_endian_bits32_parameter26>::create);
	const bool registered76 = TM::regist_bits_tm_parameter("little_endian_bits32_parameter27", &TM::create_with_name_and_position<TM::little_endian_bits32_parameter27>::create);
	const bool registered77 = TM::regist_bits_tm_parameter("little_endian_bits32_parameter28", &TM::create_with_name_and_position<TM::little_endian_bits32_parameter28>::create);
	const bool registered78 = TM::regist_bits_tm_parameter("little_endian_bits32_parameter29", &TM::create_with_name_and_position<TM::little_endian_bits32_parameter29>::create);
	const bool registered79 = TM::regist_bits_tm_parameter("little_endian_bits32_parameter30", &TM::create_with_name_and_position<TM::little_endian_bits32_parameter30>::create);
	const bool registered80 = TM::regist_bits_tm_parameter("little_endian_bits32_parameter31", &TM::create_with_name_and_position<TM::little_endian_bits32_parameter31>::create);
}
