#pragma once

// if a constexpr function prints to std::out, it causes
// error C3615: constexpr function '...' cannot result in a constant expression
// Is that (one of) the reasons Vladimir uses the visitor pattern?

#include <iostream>

namespace Approach04
{
	struct Printer
	{
		static void Visit(int n)
		{
			std::cout << "register " << n << std::endl;
		}
	};

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
	struct PatternBase
	{
		static constexpr void RegisterMethods()
		{
			std::integral_constant<int, RegisterMethodCount<TDerived, Printer>::Get() - 1> n;

			TDerived::template RegisterMethod<Printer>(n);
		}
	};

	struct FooPattern : public PatternBase<FooPattern>
	{
		template<class TVisitor>
		static constexpr void RegisterMethod(std::integral_constant<int, 0> n)
		{
			TVisitor::Visit(n);
			if constexpr (n > 0) RegisterMethod<TVisitor>(std::integral_constant<int, n - 1>{});
		}
		const char* GetFoo()
		{
			return m_foo.c_str();
		}

		template<class TVisitor>
		// static constexpr void RegisterMethod(std::integral_constant<int, RegisterMethodCount<FooPattern, TVisitor>::Get()> n)
		static constexpr void RegisterMethod(std::integral_constant<int, 1> n)
		{
			TVisitor::Visit(n);
			if constexpr (n > 0) RegisterMethod<TVisitor>(std::integral_constant<int, n - 1>{});
		}
		void SetFoo(const char* val)
		{
			m_foo = val;
		}

		template<class TVisitor>
		static constexpr void RegisterMethod(std::integral_constant<int, 2> n)
		{
			TVisitor::Visit(n);
			if constexpr (n > 0) RegisterMethod<TVisitor>(std::integral_constant<int, n - 1>{});
		}
		void AppendFoo(const char* val)
		{
			m_foo += val;
		}

		template<class TVisitor>
		static constexpr void RegisterMethod(std::integral_constant<int, 3> n)
		{
			TVisitor::Visit(n);
			if constexpr (n > 0) RegisterMethod<TVisitor>(std::integral_constant<int, n - 1>{});
		}
		void ClearFoo()
		{
			m_foo.clear();
		}

	private:
		std::string m_foo;
	};



#if 0
	template <int I>
	using ReactAttributeId = std::integral_constant<int, I>;

	template <class TModule>
	struct ReactMemberInfoIterator {
		template <int StartIndex, class TVisitor>
		constexpr void ForEachMember(TVisitor& visitor) noexcept {
			ForEachMember<StartIndex>(visitor, static_cast<std::make_index_sequence<10>*>(nullptr));
		}

		template <int I, class TVisitor>
		constexpr void GetMemberInfo(TVisitor& visitor) noexcept {
			if constexpr (decltype(HasGetReactMemberAttribute(visitor, ReactAttributeId<I>{}))::value) {
				TModule::template GetReactMemberAttribute<TModule>(visitor, ReactAttributeId<I>{});
			}
		}

	private:
		template <class TVisitor, int I>
		static auto HasGetReactMemberAttribute(TVisitor& visitor, ReactAttributeId<I> id)
			-> decltype(TModule::template GetReactMemberAttribute<TModule>(visitor, id), std::true_type{});
		static auto HasGetReactMemberAttribute(...)->std::false_type;

		// Visit members in groups of 10 to avoid deep recursion.
		template <size_t StartIndex, class TVisitor, size_t... I>
		constexpr void ForEachMember(TVisitor& visitor, std::index_sequence<I...>*) noexcept {
			if constexpr (decltype(HasGetReactMemberAttribute(visitor, ReactAttributeId<StartIndex>{}))::value) {
				(GetMemberInfo<StartIndex + I>(visitor), ...);
				ForEachMember<StartIndex + sizeof...(I)>(visitor, static_cast<std::make_index_sequence<10>*>(nullptr));
			}
		}
	};
#endif



	void Main()
	{
		// std::integral_constant<int, RegisterMethodCount<FooPattern, Printer>::Get() - 1> n;
		// FooPattern::RegisterMethod<Printer>(n);

		FooPattern::RegisterMethods();

		std::cout << "method count: " << RegisterMethodCount<FooPattern, Printer>::Get() << std::endl;

	}
}
