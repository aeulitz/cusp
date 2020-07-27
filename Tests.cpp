#include "FooPattern.h"
#include "CppUnitTest.h"
#include <functional>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

struct TestRegistrar
{
	static void Visit(int n, const char* guid)
	{
		TestRegistrar::OnRegister(n, guid);
	}
	static std::function<void(int n, const char* guid)> OnRegister;
};

std::function<void(int n, const char* guid)> TestRegistrar::OnRegister = [](int, const char*) {};

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:

		TEST_METHOD(TestFooPatternRegistration)
		{
			std::vector<const char*> guids;
			TestRegistrar::OnRegister = [&guids](int n, const char* guid) {
				guids.push_back(guid);
			};

			App::FooPattern::RegisterMethods<TestRegistrar>();

			Assert::AreEqual(static_cast<size_t>(4), guids.size());
		}
	};
}
