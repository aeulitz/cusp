#pragma once

#include "CustomPatternBase.h"

namespace App
{
	CUSTOM_PATTERN(FooPattern, "2bd720b1-c433-4292-8314-d4ccb8b7fae7")
	{
		CUSTOM_PATTERN_METHOD(0, SetBool, "25fb1199-db6f-4349-86aa-436d376a6843")
		void SetBool(bool val)
		{
			// example of a 'void(bool)' method
			OnSetBool(val);
		}

		CUSTOM_PATTERN_METHOD(1, GetBool, "a9487d87-8935-49ad-9473-fadce0bfa974")
		bool GetBool()
		{
			// example of a 'bool(void)' method
			return OnGetBool();
		}

		CUSTOM_PATTERN_METHOD(2, SetInt, "921a5d67-9a8f-4c38-b676-5c8c2d44ef18")
		void SetInt(int val)
		{
			// example of a 'void(int)' method
			OnSetInt(val);
		}

		CUSTOM_PATTERN_METHOD(3, GetInt, "85e4d90e-a804-4a45-a0e2-3f572f5ccfa9")
		int GetInt()
		{
			// example of an 'int(void)' method
			return OnGetInt();
		}

		CUSTOM_PATTERN_METHOD(4, SetFloat, "4d111a79-4b56-4a23-aefa-a56b51337f09")
		void SetFloat(float val)
		{
			// example of a 'void(float)' method
			OnSetFloat(val);
		}

		CUSTOM_PATTERN_METHOD(5, GetFloat, "71667878-2e43-4901-af9c-0eb139f79439")
		float GetFloat()
		{
			// example of a 'float(void)' method
			return OnGetFloat();
		}

		CUSTOM_PATTERN_METHOD(6, SetDouble, "16c06415-52f3-4317-8ffc-a2d4b39cddfb")
		void SetDouble(double val)
		{
			// example of a 'void(double)' method
			OnSetDouble(val);
		}

		CUSTOM_PATTERN_METHOD(7, GetDouble, "643bd34c-c22c-4255-92e7-e2ed4a104782")
		double GetDouble()
		{
			// example of a 'double(void)' method
			return OnGetDouble();
		}

		CUSTOM_PATTERN_METHOD(8, SetString, "4426d571-240c-47bc-8d5a-51f2974b4beb")
		void SetString(const std::wstring& val)
		{
			// example of a 'void(const std::wstring&)' method
			OnSetString(val);
		}

		CUSTOM_PATTERN_METHOD(9, GetString, "e6b31052-4a7e-4dad-b341-855609f42232")
		const wchar_t* GetString()
		{
			// example of a 'const wchar_t*()' method
			return OnGetString();
		}

		CUSTOM_PATTERN_METHOD(10, ClearString, "13e7a41d-bad4-4ab2-806b-e57128302684")
		void ClearString()
		{
			// example of a 'void(void)' method
			OnClearString();
		}

		//
		// for testing
		//
		std::function<void(bool)> OnSetBool = nullptr;
		std::function<bool()> OnGetBool = nullptr;
		std::function<void(int)> OnSetInt = nullptr;
		std::function<int()> OnGetInt = nullptr;
		std::function<void(float)> OnSetFloat = nullptr;
		std::function<float()> OnGetFloat = nullptr;
		std::function<void(double)> OnSetDouble = nullptr;
		std::function<double()> OnGetDouble = nullptr;
		std::function<void(const std::wstring&)> OnSetString = nullptr;
		std::function<const wchar_t*()> OnGetString = nullptr;
		std::function<void()> OnClearString = nullptr;
	};
}
