#pragma once

#include "CustomPatternBase.h"
#include "UIA.h"

namespace App
{
	//[uuid(AFDB9683 - F18A - 4B85 - 90D1 - B6158DAFA46C)]
	//__declspec(uuid("c29e6e8c-47e5-4405-95a8-706d4d0d95b3"))
	struct FooPattern : public CustomPatternBase<FooPattern>
	{
		//CUSTOM_PATTERN_METHOD(0, GetFoo, "e6b31052-4a7e-4dad-b341-855609f42232")
		template<class TVisitor>
		static constexpr void RegisterMethod(std::integral_constant<int, 0> n)
		{
			// Can we bind the pattern type to the visitor type to make it available here without
			// having to specify it as a macro parameter (again), resulting in a usage like
			//     TVisitor::PatternType
			// ?
			// Or could it be a direct type parameter of the RegisterMethod?

			TVisitor::Visit(n, Guid::StringToGuid("e6b31052-4a7e-4dad-b341-855609f42232"));

			Microsoft::UIA::AddRemoteOperationExtension(
				Guid::StringToGuid("e6b31052-4a7e-4dad-b341-855609f42232"),
				1, // not sure what it is/how it is used
				[](Microsoft::UIA::RemoteOperationContext& context, const std::vector<Microsoft::UIA::OperandId>& operandIds)
				{
					auto _this = static_cast<FooPattern*>(context.GetOperand(operandIds[0]).Get());
					auto methodPointer = &FooPattern::GetFoo;
					auto retVal = (_this->*methodPointer)();
				});




			if constexpr (n > 0) RegisterMethod<TVisitor>(std::integral_constant<int, n - 1>{});
		}
		template<class TVisitor>
		static constexpr void UnregisterMethod(std::integral_constant<int, 0> n)
		{
			Microsoft::UIA::RemoveRemoteOperationExtension(Guid::StringToGuid("e6b31052-4a7e-4dad-b341-855609f42232"));

			if constexpr (n > 0) UnregisterMethod<TVisitor>(std::integral_constant<int, n - 1>{});

		}
		const char* GetFoo()
		{
			return m_foo.c_str();
		}

		CUSTOM_PATTERN_METHOD(1, "4426d571-240c-47bc-8d5a-51f2974b4beb")
		void SetFoo(const char* val)
		{
			m_foo = val;
		}

		CUSTOM_PATTERN_METHOD(2, "c583a435-e0c1-4ba9-be2d-6b8d714c7918")
		void AppendFoo(const char* val)
		{
			m_foo += val;
		}

		CUSTOM_PATTERN_METHOD(3, "13e7a41d-bad4-4ab2-806b-e57128302684")
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
