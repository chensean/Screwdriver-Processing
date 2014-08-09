#include "stdafx.h"
#include "CppUnitTest.h"
#include <CppUnitTestAssert.h>
#include <boost/assign.hpp>
#include <boost/lexical_cast.hpp>
#include "tm_parameter.h"
#include "polynomial.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace boost::assign;
using namespace std::placeholders;

namespace UnitTestCore
{		
	TEST_CLASS(UnitTest1)
	{
	public:

		TEST_METHOD(TestParameter1)
		{
			Logger::WriteMessage("Test parameter1");

			TM::parameter1 tm("tm1");
			std::vector<unsigned char> buf;
			buf+=0xaa,0xbb,0xcc;
			tm.read_form_buffer(buf, 0);
			int val = boost::get<uint8_t>(tm.get_val());
			Assert::AreEqual(0xaa,val );
			Assert::AreEqual( std::string("170"),tm.get_val_text());
		}

		TEST_METHOD(TestDoubleParameter1)
		{
			Logger::WriteMessage("Test double_parameter1");

			TM::double_parameter1 tm("tm1"
				,[](uint64_t code){return code*0.02;}
			,[](uint64_t code){return std::to_string(code*0.02);});
			std::vector<unsigned char> buf;
			buf+=0xaa,0xbb,0xcc;
			tm.read_form_buffer(buf, 0);
			double val = boost::get<double>(tm.get_val());
			Assert::AreEqual(170*0.02,val);
			//Assert::AreEqual( std::string("3.4"),tm.get_val_text());
		}

		TEST_METHOD(TestDoubleParameter1_polynomial)
		{
			Logger::WriteMessage("Test double_parameter1 polynomial");
			std::vector<double> coefficient;
			coefficient += 0.1, 0.02,0.01;
			std::shared_ptr<polynomial> pl(new polynomial(coefficient));
			TM::double_parameter1 tm("tm1"
				,std::bind(&polynomial::code_to_val,pl,_1)
				,[&pl](uint64_t code){return boost::lexical_cast<std::string>(pl->code_to_val(code));});
			std::vector<unsigned char> buf;
			buf+=0xaa,0xbb,0xcc;
			tm.read_form_buffer(buf, 0);
			double val = boost::get<double>(tm.get_val());
			Assert::AreEqual(170*170*0.01+0.02*170+0.1,val);
			Assert::AreEqual(boost::lexical_cast<std::string>(170*170*0.01+0.02*170+0.1) ,tm.get_val_text());
			uint64_t code=0xaa;
			Assert::AreEqual(code,tm.get_code());
		}

		TEST_METHOD(TestParameter2)
		{
			Logger::WriteMessage("Test big_endian_parameter2");

			TM::big_endian_parameter2 tm("tm2");
			std::vector<unsigned char> buf;
			buf+=0xaa,0xbb,0xcc;
			tm.read_form_buffer(buf, 0);
			int val = boost::get<uint16_t>(tm.get_val());
			Assert::AreEqual(val, 0xaabb);
		}

		TEST_METHOD(TestDoubleParameter8)
		{
			Logger::WriteMessage("Test double_parameter8");

			TM::big_endian_double_parameter8 tm("tm1"
				,&utilities::men_copy_convert<double,uint64_t>
				,[](uint64_t code){return std::to_string(utilities::men_copy_convert<double>(code));});
			double expVal=-3.156;
			std::vector<unsigned char> buf(8);
			memcpy(buf.data(),&expVal,buf.size());
			std::reverse(buf.begin(),buf.end());
			tm.read_form_buffer(buf, 0);
			double val = boost::get<double>(tm.get_val());
			Assert::AreEqual(expVal,val,0.0001);
			Assert::AreEqual( std::to_string(expVal),tm.get_val_text());
		}

	};
}