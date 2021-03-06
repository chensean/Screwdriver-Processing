#include "stdafx.h"
#include "CppUnitTest.h"
#include <CppUnitTestAssert.h>
#include <boost/assign.hpp>
#include <boost/lexical_cast.hpp>
#include "bits_tm_parameter.h"
#include "polynomial_conversion.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace boost::assign;

namespace UnitTestCore
{
	TEST_CLASS(test_bits_tm_parameter)
	{
	public:

		TEST_METHOD(Test_bits8_parameter1)
		{
			Logger::WriteMessage("Test bits8_parameter1");
			TM::bits8_parameter1 tm("tm1", 1);
			std::vector<unsigned char> buf;
			buf += 0xaa , 0xbb , 0xcc;
			tm.read_form_buffer(buf, 0);
			int val = boost::numeric_cast<uint8_t>(tm.get_val());
			Assert::AreEqual(1, val);
			Assert::AreEqual(std::string("1"), tm.get_val_text());
		}

		TEST_METHOD(Test_bits8_parameter2)
		{
			Logger::WriteMessage("Test bits8_parameter2");
			TM::bits8_parameter2 tm("tm1", 4);
			std::vector<unsigned char> buf;
			buf += 0xaa , 0xbb , 0xcc;
			tm.read_form_buffer(buf, 1);
			int val = boost::numeric_cast<uint8_t>(tm.get_val());
			Assert::AreEqual(3, val);
			Assert::AreEqual(std::string("3"), tm.get_val_text());
		}

		TEST_METHOD(Test_Create_bits8_parameter2)
		{
			Logger::WriteMessage("Test Create bits8_parameter2");
			auto tm = TM::create_bits_tm_parameter("bits8_parameter2", "tm1", 4);
			std::vector<unsigned char> buf;
			buf += 0xaa , 0xbb , 0xcc;
			tm->read_form_buffer(buf, 1);
			int val = boost::numeric_cast<uint8_t>(tm->get_val());
			Assert::AreEqual(3, val);
			Assert::AreEqual(std::string("3"), tm->get_val_text());
		}

		TEST_METHOD(Test_bits8_parameter3)
		{
			Logger::WriteMessage("Test bits8_parameter3");
			TM::bits8_parameter3 tm("tm1", 5);
			std::vector<unsigned char> buf;
			buf += 0xaa , 0xbb , 0xcc;
			tm.read_form_buffer(buf, 1);
			int val = boost::numeric_cast<uint8_t>(tm.get_val());
			Assert::AreEqual(5, val);
			Assert::AreEqual(std::string("5"), tm.get_val_text());
		}

		TEST_METHOD(Test_bits8_parameter4)
		{
			Logger::WriteMessage("Test bits8_parameter4");
			TM::bits8_parameter4 tm("tm1", 4);
			std::vector<unsigned char> buf;
			buf += 0xaa , 0xbb , 0xcc;
			tm.read_form_buffer(buf, 2);
			int val = boost::numeric_cast<uint8_t>(tm.get_val());
			Assert::AreEqual(12, val);
			Assert::AreEqual(std::string("12"), tm.get_val_text());
		}


		TEST_METHOD(Test_bits8_parameter5)
		{
			Logger::WriteMessage("Test bits8_parameter5");
			TM::bits8_parameter5 tm("tm1", 0);
			std::vector<unsigned char> buf;
			buf += 0xaa , 0xbb , 0xcc;
			tm.read_form_buffer(buf, 1);
			int val = boost::numeric_cast<uint8_t>(tm.get_val());
			Assert::AreEqual(27, val);
			Assert::AreEqual(std::string("27"), tm.get_val_text());
		}

		TEST_METHOD(Test_bits8_parameter6)
		{
			Logger::WriteMessage("Test bits8_parameter6");
			TM::bits8_parameter6 tm("tm1", 0);
			std::vector<unsigned char> buf;
			buf += 0xaa , 0xbb , 0xcc;
			tm.read_form_buffer(buf, 1);
			int val = boost::numeric_cast<uint8_t>(tm.get_val());
			Assert::AreEqual(27 + 32, val);
			Assert::AreEqual(std::string("59"), tm.get_val_text());
		}

		TEST_METHOD(Test_bits8_parameter7)
		{
			Logger::WriteMessage("Test bits8_parameter7");
			TM::bits8_parameter7 tm("tm1", 1);
			std::vector<unsigned char> buf;
			buf += 0xaa , 0xbb , 0xcc;
			tm.read_form_buffer(buf, 1);
			int val = boost::numeric_cast<uint8_t>(tm.get_val());
			Assert::AreEqual(93, val);
			Assert::AreEqual(std::string("93"), tm.get_val_text());
		}

		TEST_METHOD(Test_big_endian_bits16_parameter2)
		{
			Logger::WriteMessage("Test big_endian_bits16_parameter2");
			TM::big_endian_bits16_parameter2 tm("tm1", 4);
			std::vector<unsigned char> buf;
			buf += 0xaa , 0xbb , 0xcc;
			tm.read_form_buffer(buf, 0);
			int val = boost::numeric_cast<uint8_t>(tm.get_val());
			Assert::AreEqual(3, val);
			Assert::AreEqual(std::string("3"), tm.get_val_text());
		}

		TEST_METHOD(Test_big_endian_bits16_parameter2_1)
		{
			Logger::WriteMessage("Test big_endian_bits16_parameter2");
			TM::big_endian_bits16_parameter2 tm("tm1", 7);
			std::vector<unsigned char> buf;
			buf += 0xaa , 0xbb , 0xcc;
			tm.read_form_buffer(buf, 0);
			int val = boost::numeric_cast<uint8_t>(tm.get_val());
			Assert::AreEqual(1, val);
			Assert::AreEqual(std::string("1"), tm.get_val_text());
		}

		TEST_METHOD(Test_big_endian_bits16_parameter3)
		{
			Logger::WriteMessage("Test big_endian_bits16_parameter3");
			TM::big_endian_bits16_parameter3 tm("tm1", 5);
			std::vector<unsigned char> buf;
			buf += 0xaa , 0xbb , 0xcc;
			tm.read_form_buffer(buf, 0);
			int val = boost::numeric_cast<uint8_t>(tm.get_val());
			Assert::AreEqual(5, val);
			Assert::AreEqual(std::string("5"), tm.get_val_text());
		}

		TEST_METHOD(Test_big_endian_bits16_parameter3_1)
		{
			Logger::WriteMessage("Test big_endian_bits16_parameter3");
			TM::big_endian_bits16_parameter3 tm("tm1", 7);
			std::vector<unsigned char> buf;
			buf += 0xaa , 0xbb , 0xcc;
			tm.read_form_buffer(buf, 0);
			int val = boost::numeric_cast<uint8_t>(tm.get_val());
			Assert::AreEqual(5, val);
			Assert::AreEqual(std::string("5"), tm.get_val_text());
		}

		TEST_METHOD(Test_big_endian_bits16_parameter4)
		{
			Logger::WriteMessage("Test big_endian_bits16_parameter4");
			TM::big_endian_bits16_parameter4 tm("tm1", 4);
			std::vector<unsigned char> buf;
			buf += 0xaa , 0xbb , 0xcc;
			tm.read_form_buffer(buf, 1);
			int val = boost::numeric_cast<uint8_t>(tm.get_val());
			Assert::AreEqual(12, val);
			Assert::AreEqual(std::string("12"), tm.get_val_text());
		}

		TEST_METHOD(Test_big_endian_bits16_parameter4_1)
		{
			Logger::WriteMessage("Test big_endian_bits16_parameter4");
			TM::big_endian_bits16_parameter4 tm("tm1", 5);
			std::vector<unsigned char> buf;
			buf += 0xaa , 0xbb , 0xcc;
			tm.read_form_buffer(buf, 1);
			int val = boost::numeric_cast<uint8_t>(tm.get_val());
			Assert::AreEqual(14, val);
			Assert::AreEqual(std::string("14"), tm.get_val_text());
		}

		TEST_METHOD(Test_big_endian_bits16_parameter5)
		{
			Logger::WriteMessage("Test big_endian_bits16_parameter5");
			TM::big_endian_bits16_parameter5 tm("tm1", 0);
			std::vector<unsigned char> buf;
			buf += 0xaa , 0xbb , 0xcc;
			tm.read_form_buffer(buf, 0);
			int val = boost::numeric_cast<uint8_t>(tm.get_val());
			Assert::AreEqual(27, val);
			Assert::AreEqual(std::string("27"), tm.get_val_text());
		}

		TEST_METHOD(Test_big_endian_bits16_parameter5_1)
		{
			Logger::WriteMessage("Test big_endian_bits16_parameter5");
			TM::big_endian_bits16_parameter5 tm("tm1", 5);
			std::vector<unsigned char> buf;
			buf += 0xaa , 0xbb , 0xcc;
			tm.read_form_buffer(buf, 0);
			int val = boost::numeric_cast<uint8_t>(tm.get_val());
			Assert::AreEqual(21, val);
			Assert::AreEqual(std::string("21"), tm.get_val_text());
		}

		TEST_METHOD(Test_big_endian_bits16_parameter6)
		{
			Logger::WriteMessage("Test big_endian_bits16_parameter6");
			TM::big_endian_bits16_parameter6 tm("tm1", 0);
			std::vector<unsigned char> buf;
			buf += 0xaa , 0xbb , 0xcc;
			tm.read_form_buffer(buf, 0);
			int val = boost::numeric_cast<uint8_t>(tm.get_val());
			Assert::AreEqual(27 + 32, val);
			Assert::AreEqual(std::string("59"), tm.get_val_text());
		}

		TEST_METHOD(Test_big_endian_bits16_parameter6_2)
		{
			Logger::WriteMessage("Test big_endian_bits16_parameter6");
			TM::big_endian_bits16_parameter6 tm("tm1", 4);
			std::vector<unsigned char> buf;
			buf += 0xaa , 0xbb , 0xcc;
			tm.read_form_buffer(buf, 0);
			int val = boost::numeric_cast<uint8_t>(tm.get_val());
			Assert::AreEqual(43, val);
			Assert::AreEqual(std::string("43"), tm.get_val_text());
		}

		TEST_METHOD(Test_big_endian_bits16_parameter7)
		{
			Logger::WriteMessage("Test big_endian_bits16_parameter7");
			TM::big_endian_bits16_parameter7 tm("tm1", 1);
			std::vector<unsigned char> buf;
			buf += 0xaa , 0xbb , 0xcc;
			tm.read_form_buffer(buf, 0);
			int val = boost::numeric_cast<uint8_t>(tm.get_val());
			Assert::AreEqual(93, val);
			Assert::AreEqual(std::string("93"), tm.get_val_text());
		}

		TEST_METHOD(Test_big_endian_bits16_parameter7_1)
		{
			Logger::WriteMessage("Test big_endian_bits16_parameter7");
			TM::big_endian_bits16_parameter7 tm("tm1", 3);
			std::vector<unsigned char> buf;
			buf += 0xaa , 0xbb , 0xcc;
			tm.read_form_buffer(buf, 0);
			int val = boost::numeric_cast<uint8_t>(tm.get_val());
			Assert::AreEqual(0x57, val);
			Assert::AreEqual(std::string("87"), tm.get_val_text());
		}

		TEST_METHOD(Test_big_endian_bits16_parameter8)
		{
			Logger::WriteMessage("Test big_endian_bits16_parameter8");
			TM::big_endian_bits16_parameter8 tm("tm1", 4);
			std::vector<unsigned char> buf;
			buf += 0xaa , 0xbb , 0xcc;
			tm.read_form_buffer(buf, 0);
			int val = boost::numeric_cast<uint8_t>(tm.get_val());
			Assert::AreEqual(0xab, val);
			Assert::AreEqual(std::string("171"), tm.get_val_text());
		}

		TEST_METHOD(Test_big_endian_bits16_parameter10)
		{
			Logger::WriteMessage("Test big_endian_bits16_parameter10");
			TM::big_endian_bits16_parameter10 tm("tm1", 0);
			std::vector<unsigned char> buf;
			buf += 0xaa , 0xbb , 0xcc;
			tm.read_form_buffer(buf, 0);
			int val = boost::numeric_cast<uint16_t>(tm.get_val());
			Assert::AreEqual(0x2bb, val);
			Assert::AreEqual(std::string("699"), tm.get_val_text());
		}

		TEST_METHOD(Test_big_endian_bits32_parameter15)
		{
			Logger::WriteMessage("Test big_endian_bits32_parameter15");
			TM::big_endian_bits32_parameter15 tm("tm1", 0);
			std::vector<unsigned char> buf;
			buf += 0 , 0xaa , 0xbb , 0xcc;
			tm.read_form_buffer(buf, 0);
			int val = boost::numeric_cast<uint16_t>(tm.get_val());
			Assert::AreEqual(0x3bcc, val);
			Assert::AreEqual(std::string("15308"), tm.get_val_text());
		}

		TEST_METHOD(Test_big_endian_bits32_parameter25)
		{
			Logger::WriteMessage("Test big_endian_bits32_parameter25");
			TM::big_endian_bits32_parameter25 tm("tm1", 3);
			std::vector<unsigned char> buf;
			buf += 0 , 0xaa , 0xbb , 0xcc , 0xdd;
			tm.read_form_buffer(buf, 1);
			int val = boost::numeric_cast<uint32_t>(tm.get_val());
			Assert::AreEqual(0x157799B, val);
			Assert::AreEqual(std::string("22509979"), tm.get_val_text());
		}

		TEST_METHOD(Test_big_endian_bits16_double_parameter10)
		{
			Logger::WriteMessage("Test big_endian_bits16_double_parameter10");
			std::vector<double> coefficient;
			coefficient += -20.15 , 0.02 , 0.01;
			std::shared_ptr<polynomial_conversion> pl(new polynomial_conversion(coefficient));
			TM::big_endian_bits16_parameter10 tm("tm1"
			                                     , 3
			                                     , boost::bind(&polynomial_conversion::convert, pl, _1));
			std::vector<unsigned char> buf;
			buf += 0 , 0xaa , 0xbb , 0xcc , 0xdd;
			tm.read_form_buffer(buf, 1);
			double val = tm.get_val();
			Assert::AreEqual(1163.2, val);
			Assert::AreEqual(std::string("1163.2"), tm.get_val_text());
		}


		TEST_METHOD(Test_little_endian_bits16_parameter5)
		{
			Logger::WriteMessage("Test little_endian_bits16_parameter5");
			TM::little_endian_bits16_parameter5 tm("tm1", 5);
			std::vector<unsigned char> buf;
			buf += 0xaa , 0xbb , 0xcc;
			tm.read_form_buffer(buf, 0);
			int val = boost::numeric_cast<uint8_t>(tm.get_val());
			Assert::AreEqual(29, val);
			Assert::AreEqual(std::string("29"), tm.get_val_text());
		}

		TEST_METHOD(Test_little_endian_bits32_parameter18)
		{
			Logger::WriteMessage("Test little_endian_bits32_parameter18");
			TM::little_endian_bits32_parameter18 tm("tm1", 2);
			std::vector<unsigned char> buf;
			buf += 0 , 0xaa , 0xbb , 0xcc;
			tm.read_form_buffer(buf, 0);
			int val = boost::numeric_cast<uint32_t>(tm.get_val());
			Assert::AreEqual(0x2EA80, val);
			Assert::AreEqual(std::string("191104"), tm.get_val_text());
		}
	};
}
