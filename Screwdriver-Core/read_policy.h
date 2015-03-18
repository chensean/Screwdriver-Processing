#ifndef READ_POLICY_HPP
#define READ_POLICY_HPP

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
	struct big_endian_read
	{
		typedef boost::mpl::int_<BIT_COUNT> i_bits_t;
		typedef typename boost::mpl::if_<boost::mpl::greater<i_bits_t, i32_t>, uint64_t, typename boost::mpl::if_<boost::mpl::greater<i_bits_t, i16_t>, uint32_t, uint16_t>::type>::type val_type;

		static val_type read_code(const std::vector<uint8_t>& buffer, uint32_t start_idx)
		{
			static_assert(BIT_COUNT % std::numeric_limits<uint8_t>::digits == 0,"bits is not bytes");
			return utilities::array_to_big_endian_val<val_type>(buffer, start_idx);
		}

	protected:
		~big_endian_read(void)
		{
		}
	};

	template <>
	struct big_endian_read<std::numeric_limits<uint8_t>::digits>
	{
		static uint8_t read_code(const std::vector<uint8_t>& buffer, uint32_t start_idx)
		{
			return start_idx < buffer.size() ? buffer[start_idx] : 0;
		}

	protected:
		~big_endian_read(void)
		{
		}
	};

	template <>
	struct big_endian_read<std::numeric_limits<uint8_t>::digits * 3>
	{
		static uint32_t read_code(const std::vector<uint8_t>& buffer, uint32_t start_idx)
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
		~big_endian_read(void)
		{
		}
	};

	template <int BIT_COUNT>
	struct little_endian_read
	{
		typedef boost::mpl::int_<BIT_COUNT> i_bits_t;
		typedef typename boost::mpl::if_<boost::mpl::greater<i_bits_t, i32_t>, uint64_t, typename boost::mpl::if_<boost::mpl::greater<i_bits_t, i16_t>, uint32_t, uint16_t>::type>::type val_type;

		static val_type read_code(const std::vector<uint8_t>& buffer, uint32_t start_idx)
		{
			static_assert(BIT_COUNT % std::numeric_limits<uint8_t>::digits == 0,"bits is not bytes");
			return utilities::array_to_little_endian_val<val_type>(buffer, start_idx);
		}

	protected:
		~little_endian_read(void)
		{
		}
	};

	template <>
	struct little_endian_read<std::numeric_limits<uint8_t>::digits>
	{
		static uint8_t read_code(const std::vector<uint8_t>& buffer, uint32_t start_idx)
		{
			return start_idx < buffer.size() ? buffer[start_idx] : 0;
		}

	protected:
		~little_endian_read(void)
		{
		}
	};

	template <>
	struct little_endian_read<std::numeric_limits<uint8_t>::digits * 3>
	{
		static uint32_t read_code(const std::vector<uint8_t>& buffer, uint32_t start_idx)
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
		~little_endian_read(void)
		{
		}
	};
}

#endif // READ_POLICY_HPP

