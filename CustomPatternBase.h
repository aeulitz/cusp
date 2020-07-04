#pragma once

#include <iostream>
#include <type_traits>

// Learnings:
// - If a constexpr function prints to std::out, it causes
//     error C3615: constexpr function '...' cannot result in a constant expression
//   Is that (one of) the reasons Vladimir uses the visitor pattern?

template<class TPattern, class TVisitor>
struct RegisterMethodCount
{
	template<int N>
	static auto HasMethod(std::integral_constant<int, N> i)
		-> decltype(TPattern::template RegisterMethod<TVisitor>(i), std::true_type{});

	template<int N>
	static auto HasMethod(...)->std::false_type;

	template<int N = 0>
	static constexpr int Get()
	{
		if constexpr (decltype(HasMethod<N + 1>(std::integral_constant<int, N + 1>{}))::value)
			return Get<N + 1>();
		else
			return N + 1;
	}
};

template<class TDerived>
struct CustomPatternBase
{
	struct MethodRegistrar
	{
		static void Visit(int n)
		{
			std::cout << "register " << n << std::endl;
		}
	};

	static constexpr void RegisterMethods()
	{
		std::integral_constant<int, RegisterMethodCount<TDerived, MethodRegistrar>::Get() - 1> n;

		TDerived::template RegisterMethod<MethodRegistrar>(n);
	}
};
