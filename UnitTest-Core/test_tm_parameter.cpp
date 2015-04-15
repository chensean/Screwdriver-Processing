#include "stdafx.h"
#include "CppUnitTest.h"
#include <CppUnitTestAssert.h>
#include <boost/assign.hpp>
#include <boost/lexical_cast.hpp>
#include "tm_parameter.h"
#include "base_tm_parameter.h"
#include "polynomial_conversion.h"
#include "linear_conversion.h"
#include "slot_test.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace boost::assign;
using namespace TM;
namespace UnitTestCore
{
	TEST_CLASS(test_tm_parameter)
	{
	public:

		TEST_METHOD(Test_create_uint_parameter8)
		{
			Logger::WriteMessage("Test Create uint_parameter8");
			auto tm =TM::create_base_tm_parameter("uint_parameter8","tm1");
			boost::shared_ptr<slot_test> slot(new slot_test);
			tm->connect_val_charged_signal(val_charged_slot_t(&slot_test::receive_signal,slot.get(),_1).track(slot));
			std::vector<unsigned char> buf;
			buf+=0xaa,0xbb,0xcc;
			tm->read_form_buffer(buf, 0);
			int val = boost::numeric_cast<uint8_t>(tm->get_val());

			Assert::AreEqual(0xaa,val );
			Assert::AreEqual( std::string("170"),tm->get_val_text());
			
			Assert::AreEqual(170.,slot->Val_f() );
			Assert::AreEqual( std::string("170"),slot->Val_string());
			tm->read_form_buffer(buf, 1);
			Assert::AreEqual(187.,slot->Val_f() );
			Assert::AreEqual( std::string("187"),slot->Val_string());
			slot.reset();
			tm->read_form_buffer(buf, 0);
			val = boost::numeric_cast<uint8_t>(tm->get_val());

			Assert::AreEqual(0xaa,val );
			Assert::AreEqual( std::string("170"),tm->get_val_text());
		}

		TEST_METHOD(Test_uint_parameter8_primary_conversion)
		{
			Logger::WriteMessage("Test Create uint_parameter8 primary_conversion");
			auto tm =TM::create_base_tm_parameter("uint_parameter8","tm1");
			tm->set_primary_conversion([](double code){return code*0.02;});
			tm->set_text_conversion([](double val){return std::to_string(val);});
			std::vector<unsigned char> buf;
			buf+=0xaa,0xbb,0xcc;
			tm->read_form_buffer(buf, 0);
			double val = tm->get_val();
			Assert::AreEqual(170*0.02,val);
			//Assert::AreEqual( std::string("3.4"),tm->get_val_text());
		}

		TEST_METHOD(Test_uint_parameter8_polynomial)
		{
			Logger::WriteMessage("Test Create uint_parameter8 polynomial");
			auto tm =TM::create_base_tm_parameter("uint_parameter8","tm1");
			std::vector<double> coefficient;
			coefficient += 0.1, 0.02,0.01;
			std::shared_ptr<polynomial_conversion> pl(new polynomial_conversion(coefficient));
			tm->set_secondary_conversion(boost::bind(&polynomial_conversion::convert,pl,_1));
			std::vector<unsigned char> buf;
			buf+=0xaa,0xbb,0xcc;
			tm->read_form_buffer(buf, 0);
			double val = tm->get_val();
			Assert::AreEqual(170*170*0.01+0.02*170+0.1,val);
			Assert::AreEqual(boost::lexical_cast<std::string>(170*170*0.01+0.02*170+0.1) ,tm->get_val_text());
			double code=0xaa;
			Assert::AreEqual(code,tm->get_extraction_val());
		}

		TEST_METHOD(Test_big_endian_uint_parameter16)
		{
			Logger::WriteMessage("Test Create big_endian_uint_parameter16");
			auto tm =TM::create_base_tm_parameter("big_endian_uint_parameter16","tm1");
			std::vector<unsigned char> buf;
			buf+=0xaa,0xbb,0xcc;
			tm->read_form_buffer(buf, 0);
			int val = boost::numeric_cast<uint16_t>(tm->get_val());
			Assert::AreEqual(val, 0xaabb);
		}

		TEST_METHOD(Test_big_endian_uint_parameter16_liner_conversion)
		{
			Logger::WriteMessage("Test Create big_endian_uint_parameter16 liner_conversion");
			auto tm =TM::create_base_tm_parameter("big_endian_uint_parameter16","tm1");
			std::shared_ptr<linear_conversion> conversion(new linear_conversion(0.05,-3.2));
			tm->set_primary_conversion(boost::bind(&linear_conversion::convert,conversion,_1));

			std::vector<unsigned char> buf;
			buf+=0xaa,0xbb,0xcc;
			tm->read_form_buffer(buf, 0);
			int extraction_val = boost::numeric_cast<uint16_t>(tm->get_extraction_val());
			Assert::AreEqual(extraction_val, 0xaabb);
			Assert::AreEqual(2182.15, tm->get_val());
		}

		TEST_METHOD(Test_little_endian_uint_parameter16_liner_conversion)
		{
			Logger::WriteMessage("Test Create little_endian_uint_parameter16 liner_conversion");
			auto tm =TM::create_base_tm_parameter("little_endian_uint_parameter16","tm1");
			std::shared_ptr<linear_conversion> conversion(new linear_conversion(0.05,-3.2));
			tm->set_primary_conversion(boost::bind(&linear_conversion::convert,conversion,_1));

			std::vector<unsigned char> buf;
			buf+=0xaa,0xbb,0xcc;
			tm->read_form_buffer(buf, 0);
			int extraction_val = boost::numeric_cast<uint16_t>(tm->get_extraction_val());
			Assert::AreEqual(extraction_val, 0xbbaa);
			Assert::AreEqual(2398.9, tm->get_val());
		}

		TEST_METHOD(Test_big_endian_int_parameter16_liner_conversion)
		{
			Logger::WriteMessage("Test Create big_endian_int_parameter16 liner_conversion");
			auto tm =TM::create_base_tm_parameter("big_endian_int_parameter16","tm1");
			std::shared_ptr<linear_conversion> pl(new linear_conversion(0.05,-3.2));
			tm->set_primary_conversion(boost::bind(&linear_conversion::convert,pl,_1));

			std::vector<unsigned char> buf;
			buf+=0xaa,0xbb,0xcc;
			tm->read_form_buffer(buf, 0);
			int extraction_val = boost::numeric_cast<int16_t>(tm->get_extraction_val());
			Assert::AreEqual(extraction_val, -21829);
			Assert::AreEqual(-1094.65, tm->get_val());
		}

		TEST_METHOD(Test_big_endian_uint_parameter24)
		{
			Logger::WriteMessage("Test Create big_endian_uint_parameter24");
			auto tm =TM::create_base_tm_parameter("big_endian_uint_parameter24","tm1");
			std::vector<unsigned char> buf;
			buf+=0xaa,0xbb,0xcc,0xdd,0xee;
			tm->read_form_buffer(buf, 1);
			int val = boost::numeric_cast<uint32_t>(tm->get_val());
			Assert::AreEqual(val, 0xbbccdd);
		}

		TEST_METHOD(Test_big_endian_double_parameter64)
		{
			Logger::WriteMessage("Test Create big_endian_double_parameter64");
			auto tm =TM::create_base_tm_parameter("big_endian_double_parameter64","tm1");
			tm->set_text_conversion([](double val){return std::to_string(val);});
			double expVal=-3.156;
			std::vector<unsigned char> buf(8);
			memcpy(buf.data(),&expVal,buf.size());
			std::reverse(buf.begin(),buf.end());
			tm->read_form_buffer(buf, 0);
			double val =tm->get_val();
			Assert::AreEqual(expVal,val,0.0001);
			Assert::AreEqual( std::to_string(expVal),tm->get_val_text());
		}


		TEST_METHOD(Test_little_endian_double_parameter64)
		{
			Logger::WriteMessage("Test Create little_endian_double_parameter64");
			auto tm =TM::create_base_tm_parameter("little_endian_double_parameter64","tm1");
			tm->set_text_conversion([](double val){return std::to_string(val);});
			double expVal=-3.156;
			std::vector<unsigned char> buf(8);
			memcpy(buf.data(),&expVal,buf.size());
			tm->read_form_buffer(buf, 0);
			double val =tm->get_val();
			Assert::AreEqual(expVal,val,0.0001);
			Assert::AreEqual( std::to_string(expVal),tm->get_val_text());
		}
	};
}