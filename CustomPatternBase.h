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

using namespace winrt::Windows::Foundation;

#define CUSTOM_PATTERN_METHOD(N, METHODNAME, GUID)                                                       \
	template<class TPattern, class TRegistrar>                                                           \
	static constexpr void RegisterMethod(std::integral_constant<int, N> n)                               \
	{                                                                                                    \
		TRegistrar::Register(Guid::StringToGuid(GUID), &TPattern::METHODNAME);                           \
		if constexpr (n > 0) RegisterMethod<TPattern, TRegistrar>(std::integral_constant<int, n - 1>{}); \
	}                                                                                                    \
	template<class TRegistrar>                                                                           \
	static constexpr void UnregisterMethod(std::integral_constant<int, N> n)                             \
	{                                                                                                    \
		TRegistrar::Unregister(Guid::StringToGuid(GUID));                                                \
		if constexpr (n > 0) UnregisterMethod<TRegistrar>(std::integral_constant<int, n - 1>{});         \
	}                                                                                                    \

template<class TPattern, class TRegistrar>
struct RegisterMethodCount
{
	template<int N>
	static auto HasMethod(std::integral_constant<int, N> i)
		-> decltype(TPattern::template RegisterMethod<TPattern, TRegistrar>(i), std::true_type{});

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
TTarget Unbox(const IInspectable& val)
{
	return winrt::unbox_value<TTarget>(val);
}

template <>
std::wstring Unbox<std::wstring>(const IInspectable& val)
{
	winrt::hstring s = winrt::unbox_value<winrt::hstring>(val);
	return s.c_str();
}

template<class T>
struct UnboxMapping
{
	using TargetType = T;
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

	template<class TReturn>
	static constexpr void Invoke(TReturn(TPattern::* methodPointer)(), Microsoft::UIA::RemoteOperationContext& context, const std::vector<Microsoft::UIA::OperandId>& operandIds)
	{
		TPattern* _this = context.GetOperand(operandIds[0]).as<TPattern>().get();
		if constexpr (std::is_void<TReturn>::value)
		{
			(_this->*methodPointer)();
		}
		else
		{
			TReturn retVal = (_this->*methodPointer)();
			context.SetOperand(operandIds[1], winrt::box_value(retVal));
		}
	}

	template<class TReturn, class TArg0>
	static constexpr void Invoke(TReturn(TPattern::* methodPointer)(TArg0), Microsoft::UIA::RemoteOperationContext& context, const std::vector<Microsoft::UIA::OperandId>& operandIds)
	{
		TPattern* _this = context.GetOperand(operandIds[0]).as<TPattern>().get();
		if constexpr (std::is_void<TReturn>::value)
		{
			(_this->*methodPointer)(Unbox<UnboxMapping<TArg0>::TargetType>(context.GetOperand(operandIds[1])));
		}
		else
		{
			TReturn retVal = (_this->*methodPointer)(Unbox<UnboxMapping<TArg0>::TargetType>(context.GetOperand(operandIds[1])));
			context.SetOperand(operandIds[2], winrt::box_value(retVal));
		}
	}
};

template<class TPattern>
struct CustomPatternBase : public winrt::implements<TPattern, IInspectable>
{
	struct MethodRegistrar
	{
		template<class TMethodPointer>
		static constexpr void Register(const GUID& guid, TMethodPointer methodPointer)
		{
			Microsoft::UIA::AddRemoteOperationExtension(
				guid,
				1, // not sure what it is/how it is used
				[methodPointer](Microsoft::UIA::RemoteOperationContext& context, const std::vector<Microsoft::UIA::OperandId>& operandIds)
				{
					MethodInvoker<TPattern>::Invoke(methodPointer, context, operandIds);
				});
		}

		static constexpr void Unregister(const GUID& guid)
		{
			Microsoft::UIA::RemoveRemoteOperationExtension(guid);
		}
	};

	template<class TRegistrar = MethodRegistrar>
	static constexpr void RegisterMethods()
	{
		std::integral_constant<int, RegisterMethodCount<TPattern, TRegistrar>::Get() - 1> n;

		TPattern::template RegisterMethod<TPattern, TRegistrar>(n);
	}

	template<class TRegistrar = MethodRegistrar>
	static constexpr void UnregisterMethods()
	{
		std::integral_constant<int, RegisterMethodCount<TPattern, TRegistrar>::Get() - 1> n;

		TPattern::template UnregisterMethod<TRegistrar>(n);
	}
};
