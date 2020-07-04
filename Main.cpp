#include "FooPattern.h"

#include <iostream>

int main()
{
	App::FooPattern::RegisterMethods();

	std::cout << "method count: " << RegisterMethodCount<App::FooPattern, CustomPatternBase<App::FooPattern>::MethodRegistrar>::Get() << std::endl;

	std::cout << "end\n";
}
