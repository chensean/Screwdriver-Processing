#pragma once
#include <vector>
#include <bitset>
#include <limits>
#include <iomanip>
#include <algorithm>

namespace utilities
{

	template <int BITCOUNT, typename T1, typename T2> void set_bits_value(T1& source_data, size_t start_idx, T2 val)
	{
		std::bitset<std::numeric_limits<T1>::digits> source_bitset(static_cast<int>(source_data));
		std::bitset<BITCOUNT> valueBitset(static_cast<int>(val));
		for (int i = 0; i<BITCOUNT; ++i)
		{
			source_bitset[startIdx + i] = valueBitset[i];
		}
		source_data = static_cast<T1>(source_bitset.to_ulong());
	}

	template <int BITCOUNT, typename T1, typename T2>  T1 get_bits_value(T2 source_data, size_t startIdx)
	{
		std::bitset<BITCOUNT> codeBitset;
		codeBitset.set();
		return static_cast<T1>((source_data >> startIdx)&codeBitset.to_ulong());
	}

	template <typename T1, typename T2> void set_single_bit(T1& source_data, size_t index, T2 val)
	{
		std::bitset<std::numeric_limits<T1>::digits> source_bitset(static_cast<int>(source_data));
		source_bitset[index] = val == 1;
		source_data = static_cast<T1>(source_bitset.to_ulong());
	}

	template <typename T> bool get_single_bit(T source_data, size_t index)
	{
		std::bitset<std::numeric_limits<T>::digits> source_bitset(source_data);
		return index<std::numeric_limits<T>::digits && source_bitset[index];
	}

	template <typename T, typename U> T array_to_little_endian_val(const std::vector<U>& charArray, int startIndex)
	{
		T retVal = 0;
		if (charArray.size() >= startIndex + sizeof(T) / sizeof(U))
		{
			memcpy(&retVal, charArray.data()+startIndex, sizeof(T));
		}
		return retVal;
	}

	template <typename T, typename U> T array_to_big_endian_val(const std::vector<U>& charArray, int startIndex)
	{
		T retVal = 0;
		if (charArray.size() >= startIndex + sizeof(T) / sizeof(U))
		{
			std::vector<U> data(sizeof(T) / sizeof(U), 0);
			std::reverse_copy(charArray.begin() + startIndex, charArray.begin() + startIndex + data.size(), data.begin());
			retVal = array_to_little_endian_val<T>(data, 0);
		}
		return retVal;
	}

	template <typename T> std::vector<unsigned char> big_endian_val_to_array(T val)
	{
		std::vector<unsigned char> data = little_endian_val_to_array(val);
		std::reverse(data.begin(), data.end());
		return data;
	}

	template <typename T> std::vector<unsigned char> little_endian_val_to_array(T val)
	{
		std::vector<unsigned char> buf(sizeof(T), 0);
		memcpy(buf.data(), &val, sizeof(T));
		return buf;
	}

	template <typename T> T reverse_endian(T val)
	{
		auto buf = little_endian_val_to_array(val);
		return array_to_big_endian_val<T>(buf, 0);
	}

	template <typename T1, typename T2> T1 men_copy_convert(T2 val)
	{
		static_assert(sizeof(T1)==sizeof(T2),"type memory size is not equal");
		T1 retVal;
		memcpy(&retVal, &val, sizeof(T1));
		return retVal;
	}
}