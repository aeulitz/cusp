#pragma once

#include "GuidUtils.h"
#include "UIA.h"

#include <winrt/Windows.Foundation.h>

#include <iostream>
#include <type_traits>

// Learnings:
// - If a constexpr function prints to std::out, it causes
//     error C3615: constexpr function '...' cannot result in a constant expression
//   Is that (one of) the reasons Vladimir uses the visitor pattern?
// - interesting article: https://www.drdobbs.com/cpp/extracting-function-parameter-and-return/240000586

namespace wrl = Microsoft::WRL;

#define CUSTOM_PATTERN_METHOD(N, GUID)                                                             \
	template<class TVisitor>                                                                       \
	static constexpr void RegisterMethod(std::integral_constant<int, N> n)                         \
	{                                                                                              \
		TVisitor::Visit(n, Guid::StringToGuid(GUID));                                              \
		if constexpr (n > 0) RegisterMethod<TVisitor>(std::integral_constant<int, n - 1>{});       \
	}                                                                                              \
	template<class TVisitor>                                                                       \
	static constexpr void UnregisterMethod(std::integral_constant<int, N> n)                       \
	{                                                                                              \
		if constexpr (n > 0) UnregisterMethod<TVisitor>(std::integral_constant<int, n - 1>{});     \
	}                                                                                              \

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

template<class TTarget>
TTarget Unbox(const wrl::ComPtr<IInspectable>& val)
{
}

template <>
std::wstring Unbox<std::wstring>(const wrl::ComPtr<IInspectable>& val)
{
	wrl::ComPtr val1 = val;

	winrt::hstring s = winrt::unbox_value<winrt::hstring>(winrt::Windows::Foundation::IInspectable{ val1.Detach(), winrt::take_ownership_from_abi });
	return s.c_str();
}

template<class T>
struct UnboxMapping
{
};

template<>
struct UnboxMapping<const std::wstring&>
{
	// REVIEW: there's probably a better way to drop 'const &' from a type
	using TargetType = std::wstring;
};

template<class TPattern>
struct MethodInvoker
{
	template<class TReturn /*, class TArg*/>
	static TReturn Invoke(TReturn(TPattern::* methodPointer)(/*TArg*/), const Microsoft::UIA::RemoteOperationContext& context, const std::vector<Microsoft::UIA::OperandId>& operandIds)
	{
		auto _this = static_cast<TPattern*>(context.GetOperand(operandIds[0]).Get());
		// stick return value into context rather than returning it?
		return (_this->*methodPointer)();

	}

	template<class TReturn, class TArg0>
	static TReturn Invoke(TReturn(TPattern::* methodPointer)(TArg0), const Microsoft::UIA::RemoteOperationContext& context, const std::vector<Microsoft::UIA::OperandId>& operandIds)
	{
		auto _this = static_cast<TPattern*>(context.GetOperand(operandIds[0]).Get());
		// stick return value into context rather than returning it?
		return (_this->*methodPointer)(Unbox<UnboxMapping<TArg0>::TargetType>(context.GetOperand(operandIds[1]).Get()));
	}
};

template<class TDerived>
struct CustomPatternBase : public wrl::RuntimeClass<IInspectable>
{
	struct MethodRegistrar
	{
		static void Visit(int n, const GUID& guid)
		{
			Microsoft::UIA::AddRemoteOperationExtension(
				guid,
				1, // not sure what it is/how it is used
				[](Microsoft::UIA::RemoteOperationContext& context, const std::vector<Microsoft::UIA::OperandId>& operandIds)
				{
					wrl::ComPtr<TDerived> _this;
					context.GetOperand(operandIds[0]).As<TDerived>(&_this);

				});
		}
	};

	template<class TVisitor = MethodRegistrar>
	static constexpr void RegisterMethods()
	{
		std::integral_constant<int, RegisterMethodCount<TDerived, TVisitor>::Get() - 1> n;

		TDerived::template RegisterMethod<TVisitor>(n);
	}

	template<class TVisitor = MethodRegistrar>
	static constexpr void UnregisterMethods()
	{
		std::integral_constant<int, RegisterMethodCount<TDerived, TVisitor>::Get() - 1> n;

		TDerived::template UnregisterMethod<TVisitor>(n);
	}
};
