#pragma once

#include "GuidUtils.h"
#include "UIA.h"

#include <winrt/Windows.Foundation.h>

#include <mutex>
#include <stack>

#define CUSTOM_PATTERN(PATTERN, PATTERNGUID)                                                       \
	struct PATTERN;                                                                                \
	template<> struct GuidHolder<PATTERN>                                                          \
	{                                                                                              \
		static constexpr GUID value = Guid::StringToGuid(PATTERNGUID);                             \
	};                                                                                             \
	struct PATTERN : public CustomPatternBase<PATTERN>                                             \

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

using namespace winrt::Windows::Foundation;

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

	template<class TReturn, class TArg0, class TArg1>
	static constexpr void Invoke(TReturn(TPattern::* methodPointer)(TArg0, TArg1), Microsoft::UIA::RemoteOperationContext& context, const std::vector<Microsoft::UIA::OperandId>& operandIds)
	{
		TPattern* _this = context.GetOperand(operandIds[0]).as<TPattern>().get();
		if constexpr (std::is_void<TReturn>::value)
		{
			(_this->*methodPointer)(
				Unbox<UnboxMapping<TArg0>::TargetType>(context.GetOperand(operandIds[1])),
				Unbox<UnboxMapping<TArg1>::TargetType>(context.GetOperand(operandIds[2])));
		}
		else
		{
			TReturn retVal = (_this->*methodPointer)(
				Unbox<UnboxMapping<TArg0>::TargetType>(context.GetOperand(operandIds[1])),
				Unbox<UnboxMapping<TArg0>::TargetType>(context.GetOperand(operandIds[2])));
			context.SetOperand(operandIds[3], winrt::box_value(retVal));
		}
	}
};

template<class TPattern> struct GuidHolder {};

template<class TPattern>
struct CustomPatternBase : public winrt::implements<TPattern, IInspectable>
{
	using Super = CustomPatternBase;

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

	CustomPatternBase(IInspectable element) : m_elementWeak{ element }
	{
		std::lock_guard instancesLock{ instancesMutex };
		instances.push_back(this->get_weak());
	}

	virtual ~CustomPatternBase()
	{
		std::stack<int> deadInstances;

		std::lock_guard instancesLock{ instancesMutex };

		for (int i = 0; i < instances.size(); ++i)
		{
			auto instance = instances[i].get();
			if (!instance || instance == this->get_strong()) deadInstances.push(i);
		}

		for (;!deadInstances.empty(); deadInstances.pop())
		{
			instances.erase(instances.cbegin() + deadInstances.top());
		}
	}

	template<class TRegistrar = MethodRegistrar>
	static constexpr void Register()
	{
		Microsoft::UIA::AddRemoteOperationExtension(
			GuidHolder<TPattern>::value,
			1,
			[](Microsoft::UIA::RemoteOperationContext& context, const std::vector<Microsoft::UIA::OperandId>& operandIds)
			{
				auto element = context.GetOperand(operandIds[0]);
				IInspectable pattern = FindPattern(element);
				if (pattern) context.SetOperand(operandIds[1], pattern);
			});

		std::integral_constant<int, RegisterMethodCount<TPattern, TRegistrar>::Get() - 1> n;
		TPattern::template RegisterMethod<TPattern, TRegistrar>(n);
	}

	template<class TRegistrar = MethodRegistrar>
	static constexpr void Unregister()
	{
		Microsoft::UIA::RemoveRemoteOperationExtension(GuidHolder<TPattern>::value);
		std::integral_constant<int, RegisterMethodCount<TPattern, TRegistrar>::Get() - 1> n;
		TPattern::template UnregisterMethod<TRegistrar>(n);
	}

	static IInspectable FindPattern(IInspectable element)
	{
		IInspectable pattern{ nullptr };
		std::stack<int> deadInstances;

		std::lock_guard instancesLock{ instancesMutex };

		for (int i = 0; i < instances.size(); ++i)
		{
			auto instance = instances[i].get();

			if (instance)
			{
				auto instanceElement = instance->m_elementWeak.get();
				if (instanceElement && instanceElement == element)
				{
					pattern = instance.as<IInspectable>();
				}
			}
			else
			{
				deadInstances.push(i);
			}
		}

		for (; !deadInstances.empty(); deadInstances.pop())
		{
			instances.erase(instances.cbegin() + deadInstances.top());
		}

		return pattern;
	}

	winrt::weak_ref<IInspectable> m_elementWeak;

	static std::mutex instancesMutex;
	static std::vector<winrt::weak_ref<TPattern>> instances;
};

template<class TPattern>
std::mutex CustomPatternBase<TPattern>::instancesMutex;

template<class TPattern>
std::vector<winrt::weak_ref<TPattern>> CustomPatternBase<TPattern>::instances;
