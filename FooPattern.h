#pragma once

#include "CustomPatternBase.h"

namespace App
{
	struct FooPattern : public CustomPatternBase<FooPattern>
	{
		//CUSTOM_PATTERN_METHOD(0, "abc123")
		template<class TVisitor>
		static constexpr void RegisterMethod(std::integral_constant<int, 0> n)
		{
			TVisitor::Visit(n, "abc123");
			if constexpr (n > 0) RegisterMethod<TVisitor>(std::integral_constant<int, n - 1>{});
		}
		const char* GetFoo()
		{
			return m_foo.c_str();
		}

		CUSTOM_PATTERN_METHOD(1, "abc123")
		void SetFoo(const char* val)
		{
			m_foo = val;
		}

		CUSTOM_PATTERN_METHOD(2, "abc123")
		void AppendFoo(const char* val)
		{
			m_foo += val;
		}

		CUSTOM_PATTERN_METHOD(3, "abc123")
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
}
