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

		// CUSTOM_PATTERN_METHOD(1, SetString, "4426d571-240c-47bc-8d5a-51f2974b4beb")
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

		CUSTOM_PATTERN_METHOD(2, ClearString, "13e7a41d-bad4-4ab2-806b-e57128302684")
		void ClearString()
		{
			// example of a 'void(void)' method
			if (OnClearString) OnClearString();
			m_string.clear();
		}

		CUSTOM_PATTERN_METHOD(3, SetInt, "921a5d67-9a8f-4c38-b676-5c8c2d44ef18")
		void SetInt(int val)
		{
			// example of a 'void(int)' method
			if (OnSetInt) OnSetInt(val);
			m_int = val;
		}

		CUSTOM_PATTERN_METHOD(4, GetInt, "85e4d90e-a804-4a45-a0e2-3f572f5ccfa9")
		int GetInt()
		{
			// example of an 'int(void)' method
			if (OnGetInt) OnGetInt();
			return m_int;
		}

		CUSTOM_PATTERN_METHOD(5, SetBool, "25fb1199-db6f-4349-86aa-436d376a6843")
		void SetBool(bool val)
		{
			// example of a 'void(bool)' method
			if (OnSetBool) OnSetBool(val);
			m_bool = val;
		}

		CUSTOM_PATTERN_METHOD(6, GetBool, "a9487d87-8935-49ad-9473-fadce0bfa974")
		bool GetBool()
		{
			// example of an 'bool(void)' method
			if (OnGetBool) OnGetBool();
			return m_bool;
		}

		CUSTOM_PATTERN_METHOD(7, SetFloat, "4d111a79-4b56-4a23-aefa-a56b51337f09")
		void SetFloat(float val)
		{
			// example of a 'void(float)' method
			if (OnSetFloat) OnSetFloat(val);
			m_float = val;
		}

		CUSTOM_PATTERN_METHOD(8, GetFloat, "71667878-2e43-4901-af9c-0eb139f79439")
		float GetFloat()
		{
			// example of an 'float(void)' method
			if (OnGetFloat) OnGetFloat();
			return m_float;
		}

		CUSTOM_PATTERN_METHOD(9, SetDouble, "16c06415-52f3-4317-8ffc-a2d4b39cddfb")
		void SetDouble(double val)
		{
			// example of a 'void(double)' method
			if (OnSetDouble) OnSetDouble(val);
			m_double = val;
		}

		CUSTOM_PATTERN_METHOD(10, GetDouble, "643bd34c-c22c-4255-92e7-e2ed4a104782")
		double GetDouble()
		{
			// example of an 'double(void)' method
			if (OnGetDouble) OnGetDouble();
			return m_double;
		}

		//
		// for testing
		//
		std::function<void(bool)> OnSetBool = nullptr;
		std::function<void()> OnGetBool = nullptr;
		std::function<void(int)> OnSetInt = nullptr;
		std::function<void()> OnGetInt = nullptr;
		std::function<void(float)> OnSetFloat = nullptr;
		std::function<void()> OnGetFloat = nullptr;
		std::function<void(double)> OnSetDouble = nullptr;
		std::function<void()> OnGetDouble = nullptr;
		std::function<void(const std::wstring&)> OnSetString = nullptr;
		std::function<void()> OnGetString = nullptr;
		std::function<void()> OnClearString = nullptr;

	private:
		bool m_bool;
		int m_int;
		float m_float;
		double m_double;
		std::wstring m_string;
	};
}
