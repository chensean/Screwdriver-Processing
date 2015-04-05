#ifndef EXTRACT_POLICY_HPP
#define EXTRACT_POLICY_HPP

#if defined(_MSC_VER)
# pragma once
#endif

#include <boost/mpl/if.hpp>
#include <boost/mpl/comparison.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include "digit_utilities.h"

namespace TM
{
	typedef boost::mpl::int_<std::numeric_limits<uint8_t>::digits> i8_t;
	typedef boost::mpl::int_<std::numeric_limits<uint16_t>::digits> i16_t;
	typedef boost::mpl::int_<std::numeric_limits<uint32_t>::digits> i32_t;
	typedef boost::mpl::int_<std::numeric_limits<uint64_t>::digits> i64_t;


	template <int BIT_COUNT>
	struct big_endian_uint_extract
	{
		typedef boost::mpl::int_<BIT_COUNT> i_bits_t;
		typedef typename boost::mpl::if_<boost::mpl::greater<i_bits_t, i32_t>, uint64_t, typename boost::mpl::if_<boost::mpl::greater<i_bits_t, i16_t>, uint32_t, uint16_t>::type>::type val_type;

		static val_type extract_val(const std::vector<uint8_t>& buffer, uint32_t start_idx)
		{
			static_assert(BIT_COUNT % std::numeric_limits<uint8_t>::digits == 0,"bits is not bytes");
			return utilities::array_to_big_endian_val<val_type>(buffer, start_idx);
		}

	protected:
		~big_endian_uint_extract(void)
		{
		}
	};

	template <>
	struct big_endian_uint_extract<std::numeric_limits<uint8_t>::digits>
	{
		static uint8_t extract_val(const std::vector<uint8_t>& buffer, uint32_t start_idx)
		{
			return start_idx < buffer.size() ? buffer[start_idx] : 0;
		}

	protected:
		~big_endian_uint_extract(void)
		{
		}
	};

	template <>
	struct big_endian_uint_extract<std::numeric_limits<uint8_t>::digits * 3>
	{
		static uint32_t extract_val(const std::vector<uint8_t>& buffer, uint32_t start_idx)
		{
			if (start_idx + 3 < buffer.size())
			{
				std::vector<uint8_t> data(3);
				std::copy(buffer.begin() + start_idx, buffer.begin() + start_idx + 3, data.begin());
				data.insert(data.begin(), 0);
				return utilities::array_to_big_endian_val<uint32_t>(data, 0);
			}
			return 0;
		}

	protected:
		~big_endian_uint_extract(void)
		{
		}
	};

	template <int BIT_COUNT>
	struct little_endian_uint_extract
	{
		typedef boost::mpl::int_<BIT_COUNT> i_bits_t;
		typedef typename boost::mpl::if_<boost::mpl::greater<i_bits_t, i32_t>, uint64_t, typename boost::mpl::if_<boost::mpl::greater<i_bits_t, i16_t>, uint32_t, uint16_t>::type>::type val_type;

		static val_type extract_val(const std::vector<uint8_t>& buffer, uint32_t start_idx)
		{
			static_assert(BIT_COUNT % std::numeric_limits<uint8_t>::digits == 0,"bits is not bytes");
			return utilities::array_to_little_endian_val<val_type>(buffer, start_idx);
		}

	protected:
		~little_endian_uint_extract(void)
		{
		}
	};

	template <>
	struct little_endian_uint_extract<std::numeric_limits<uint8_t>::digits>
	{
		static uint8_t extract_val(const std::vector<uint8_t>& buffer, uint32_t start_idx)
		{
			return start_idx < buffer.size() ? buffer[start_idx] : 0;
		}

	protected:
		~little_endian_uint_extract(void)
		{
		}
	};

	template <>
	struct little_endian_uint_extract<std::numeric_limits<uint8_t>::digits * 3>
	{
		static uint32_t extract_val(const std::vector<uint8_t>& buffer, uint32_t start_idx)
		{
			if (start_idx + 3 < buffer.size())
			{
				std::vector<uint8_t> data(3);
				std::copy(buffer.begin() + start_idx, buffer.begin() + start_idx + 3, data.begin());
				data.push_back(0);
				return utilities::array_to_little_endian_val<uint32_t>(data, 0);
			}
			return 0;
		}

	protected:
		~little_endian_uint_extract(void)
		{
		}
	};

	template <int BIT_COUNT>
	struct big_endian_int_extract
	{
		typedef boost::mpl::int_<BIT_COUNT> i_bits_t;
		typedef typename boost::mpl::if_<boost::mpl::greater<i_bits_t, i32_t>, int64_t, typename boost::mpl::if_<boost::mpl::greater<i_bits_t, i16_t>, int32_t, int16_t>::type>::type val_type;

		static val_type extract_val(const std::vector<uint8_t>& buffer, uint32_t start_idx)
		{
			static_assert(BIT_COUNT % std::numeric_limits<uint8_t>::digits == 0,"bits is not bytes");
			return utilities::array_to_big_endian_val<val_type>(buffer, start_idx);
		}

	protected:
		~big_endian_int_extract(void)
		{
		}
	};

	template <>
	struct big_endian_int_extract<std::numeric_limits<int8_t>::digits>
	{
		static int8_t extract_val(const std::vector<int8_t>& buffer, uint32_t start_idx)
		{
			return start_idx < buffer.size() ? buffer[start_idx] : 0;
		}

	protected:
		~big_endian_int_extract(void)
		{
		}
	};
	
	template <int BIT_COUNT>
	struct little_endian_int_extract
	{
		typedef boost::mpl::int_<BIT_COUNT> i_bits_t;
		typedef typename boost::mpl::if_<boost::mpl::greater<i_bits_t, i32_t>, int64_t, typename boost::mpl::if_<boost::mpl::greater<i_bits_t, i16_t>, int32_t, int16_t>::type>::type val_type;

		static val_type extract_val(const std::vector<uint8_t>& buffer, uint32_t start_idx)
		{
			static_assert(BIT_COUNT % std::numeric_limits<uint8_t>::digits == 0,"bits is not bytes");
			return utilities::array_to_little_endian_val<val_type>(buffer, start_idx);
		}

	protected:
		~little_endian_int_extract(void)
		{
		}
	};

	template <>
	struct little_endian_int_extract<std::numeric_limits<int8_t>::digits>
	{
		static uint8_t extract_val(const std::vector<uint8_t>& buffer, uint32_t start_idx)
		{
			return start_idx < buffer.size() ? buffer[start_idx] : 0;
		}

	protected:
		~little_endian_int_extract(void)
		{
		}
	};
	
	template <int BIT_COUNT>
	struct big_endian_float_extract
	{

		static float extract_val(const std::vector<uint8_t>& buffer, uint32_t start_idx)

		{
			static_assert(BIT_COUNT == 32,"bits is not float");
			return utilities::array_to_big_endian_val<float>(buffer, start_idx);
		}

	protected:
		~big_endian_float_extract(void)
		{
		}
	};

	template <int BIT_COUNT>
	struct big_endian_double_extract
	{

		static double extract_val(const std::vector<uint8_t>& buffer, uint32_t start_idx)
		{
			static_assert(BIT_COUNT == 64,"bits is not double");
			return utilities::array_to_big_endian_val<double>(buffer, start_idx);
		}

	protected:
		~big_endian_double_extract(void)
		{
		}
	};

	
	template <int BIT_COUNT>
	struct little_endian_float_extract
	{

		static float extract_val(const std::vector<uint8_t>& buffer, uint32_t start_idx)
		{
			static_assert(BIT_COUNT == 32,"bits is not float");
			return utilities::array_to_little_endian_val<float>(buffer, start_idx);
		}

	protected:
		~little_endian_float_extract(void)
		{
		}
	};

	template <int BIT_COUNT>
	struct little_endian_double_extract
	{

		static double extract_val(const std::vector<uint8_t>& buffer, uint32_t start_idx)
		{
			static_assert(BIT_COUNT == 64,"bits is not double");
			return utilities::array_to_little_endian_val<double>(buffer, start_idx);
		}

	protected:
		~little_endian_double_extract(void)
		{
		}
	};
}

#endif // EXTRACT_POLICY_HPP

