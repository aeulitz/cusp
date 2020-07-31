#pragma once

#include "CustomPatternBase.h"

namespace App
{
	//[uuid(AFDB9683 - F18A - 4B85 - 90D1 - B6158DAFA46C)]
	//__declspec(uuid("c29e6e8c-47e5-4405-95a8-706d4d0d95b3"))
	struct FooPattern : public CustomPatternBase<FooPattern>
	{
		//CUSTOM_PATTERN_METHOD(0, GetString, "e6b31052-4a7e-4dad-b341-855609f42232")
		template<class TPattern, class TRegistrar>
		static constexpr void RegisterMethod(std::integral_constant<int, 0> n)
		{
			TRegistrar::Register(Guid::StringToGuid("e6b31052-4a7e-4dad-b341-855609f42232"), &TPattern::GetString);
			if constexpr (n > 0) RegisterMethod<TPattern, TRegistrar>(std::integral_constant<int, n - 1>{});
		}
		template<class TRegistrar>
		static constexpr void UnregisterMethod(std::integral_constant<int, 0> n)
		{
			TRegistrar::Unregister(Guid::StringToGuid("e6b31052-4a7e-4dad-b341-855609f42232"));
			if constexpr (n > 0) UnregisterMethod<TRegistrar>(std::integral_constant<int, n - 1>{});
		}
		const wchar_t* GetString()
		{
			if (OnGetString) OnGetString();
			return m_string.c_str();
		}

		// CUSTOM_PATTERN_METHOD(1, SetFoo, "4426d571-240c-47bc-8d5a-51f2974b4beb")
		template<class TPattern, class TRegistrar>
		static constexpr void RegisterMethod(std::integral_constant<int, 1> n)
		{
			TRegistrar::Register(Guid::StringToGuid("4426d571-240c-47bc-8d5a-51f2974b4beb"), &TPattern::SetString);
			if constexpr (n > 0) RegisterMethod<TPattern, TRegistrar>(std::integral_constant<int, n - 1>{});
		}
		template<class TRegistrar>
		static constexpr void UnregisterMethod(std::integral_constant<int, 1> n)
		{
			TRegistrar::Unregister(Guid::StringToGuid("4426d571-240c-47bc-8d5a-51f2974b4beb"));
			if constexpr (n > 0) UnregisterMethod<TRegistrar>(std::integral_constant<int, n - 1>{});
		}
		void SetString(const std::wstring& val)
		{
			if (OnSetString) OnSetString(val);
			m_string = val;
		}

		CUSTOM_PATTERN_METHOD(2, AppendString, "c583a435-e0c1-4ba9-be2d-6b8d714c7918")
		void AppendString(const std::wstring& val)
		{
			m_string += val;
		}

		CUSTOM_PATTERN_METHOD(3, ClearString, "13e7a41d-bad4-4ab2-806b-e57128302684")
		void ClearString()
		{
			if (OnClearString) OnClearString();
			m_string.clear();
		}

		//
		// for testing
		//

		std::function<void()> OnGetString = nullptr;
		std::function<void(const std::wstring&)> OnSetString = nullptr;
		std::function<void()> OnClearString = nullptr;

	private:
		std::wstring m_string;
	};
}
