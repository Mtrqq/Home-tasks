#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\TDDTask\StringCalculatorLib\StringCalculator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{		
	TEST_CLASS(StringCalculatorTests)
	{
	public:
		
		TEST_METHOD(Shoud_Be_0_When_Empty)
		{
			Assert::AreEqual(0, Add(""));
		}

		TEST_METHOD(Should_Be_3_When_1_3)
		{
			Assert::AreEqual(4, Add("1,3"));
		}

		TEST_METHOD(Should_Be_25_When_5v5)
		{
			Assert::AreEqual(25, Add("5,5,5,5,5"));
		}

		TEST_METHOD(Should_Be_22_When_2v11)
		{
			Assert::AreEqual(22, Add("2,2,2,2,2,2,2,2,2,2,2"));
		}

		TEST_METHOD(Should_Be_15_With_Different_Delimiters)
		{
			Assert::AreEqual(15, Add("5,5\n5"));
		}

		TEST_METHOD(Should_Be_44_With_Custom_Delimiters)
		{
			Assert::AreEqual(44, Add("//[g][,][;]\n15g15,7;7"));
		}

		TEST_METHOD(Shoud_Be_75_With_Regex_Special_Symbols)
		{
			Assert::AreEqual(75, Add("//[.][*]\n5.5.5.5*2*1.52"));
		}

		TEST_METHOD(Should_Be_2_With_More_Than_Thousand)
		{
			Assert::AreEqual(2, Add("//[,][+]\n1 + 100000, 1"));
		}

		TEST_METHOD(Should_Be_50_With_Multiple_Character_Delimiters)
		{
			Assert::AreEqual(50, Add("//[qqq][,,,,,,,,]\n1qqq40,,,,,,,,9"));
		}

		TEST_METHOD(Shoud_Throw_With_Ambiguos_Delimiters)
		{
			auto lambda = [] {Add("//[**][***]\n2**3***4***1"); };
			Assert::ExpectException<std::regex_error>(lambda);
		}

		TEST_METHOD(Should_Throw_When_Invalid_Input)
		{
			auto lambda = [] {Add("//[2][3]\n-12-13-15310"); };
			Assert::ExpectException<std::invalid_argument>(lambda);
		}
	

	};
}