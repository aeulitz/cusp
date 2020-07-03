// TMP
// compile-time counter: https://stackoverflow.com/questions/6166337/does-c-support-compile-time-counters
// VC++ is non-compliant in this case - explicit specializations have to be at namespace scope. C++03, §14.7.3/2:

#pragma once

#include <iostream>

#include "UIA.h"

namespace uia = Microsoft::UIA;

#define A02_PATTERN_METHOD(methodNumber, guid) \
	template<> struct ChainLinkTraits<methodNumber> { static const char* Guid = guid; };

namespace Approach02
{


//constexpr int NextI()
//{
//
//}
//constexpr int LastI()
//{
//
//}

	template<int I>
	struct ChainLinkTraits
	{
	};

	template<int I>
	struct ChainLink
	{
		inline static void Register()
		{
			// std::cout << "Register " << guid << std::endl;
			std::cout << "Register " << ChainLinkTraits<I>::Guid << std::endl;

			if constexpr (I > 0) ChainLink<I - 1>::Register();
		}
	};

	//template<>
	//struct ChainLink<1>
	//{
	//};
	//
	//template<>
	//struct ChainLink<2>
	//{
	//	inline static void Register()
	//	{
	//		std::cout << "CL2::Register" << std::endl;
	//		ChainLink<1>::Register();
	//	}
	//};

	template<class TDerived>
	class PatternBase
	{
	public:
		template<int I> struct SomeTemplate {};

		template<int I>
		struct NestedChainLinkTraits
		{
		};

		template<int I>
		struct NestedChainLink
		{
			inline static void Register()
			{
				// std::cout << "Register " << guid << std::endl;
				std::cout << "Register " << NestedChainLinkTraits<I>::Guid << std::endl;

				if constexpr (I > 0) ChainLink<I - 1>::Register();
			}
		};
		static void Register()
		{
			// ChainLink<TDerived::MethodCount - 1>::Register();
		}
	};

	class FooPattern : public PatternBase<FooPattern>
	{
	public:

		// template<> struct SomeTemplate<1> {};

		// template<> struct PatternBase::NestedChainLink<1> {};

		// A02_PATTERN_METHOD(0, "abc123")
		// Problem: E0551 class "Approach01::ChainLinkTraits<0>" cannot be defined in the current scope
		const char* GetFoo()
		{
			return m_foo.c_str();
		}

		// A02_PATTERN_METHOD(1, "abc124")
		const char* SetFoo(const char* val)
		{
			m_foo = val;
		}

		static const int MethodCount = 2;
	private:
		std::string m_foo;
	};

	void Main()
	{
		FooPattern::Register();
	}
}








