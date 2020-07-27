#include "FooPattern.h"

#include <cassert>
#include <iostream>
#include <functional>


namespace TestAssert
{
	template<class TExpected, class TActual>
	void AreEqual(TExpected expected, TActual actual)
	{
		assert(expected == actual);
	}
}

struct TestRegistrar
{
	static void Visit(int n, const char* guid)
	{
		TestRegistrar::OnRegister(n, guid);
	}
	static std::function<void(int n, const char* guid)> OnRegister;
};

std::function<void(int n, const char* guid)> TestRegistrar::OnRegister = [](int, const char*) {};

void TestFooPatternRegistration()
{
	std::vector<const char*> guids;
	TestRegistrar::OnRegister = [&guids](int n, const char* guid) {
		guids.push_back(guid);
	};

	App::FooPattern::RegisterMethods<TestRegistrar>();

	TestAssert::AreEqual(4, guids.size());
}



int main()
{
	TestFooPatternRegistration();
	App::FooPattern::RegisterMethods();

	std::cout << "method count: " << RegisterMethodCount<App::FooPattern, CustomPatternBase<App::FooPattern>::MethodRegistrar>::Get() << std::endl;

	std::cout << "end\n";
}
