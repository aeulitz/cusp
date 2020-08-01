#pragma once

#include "CustomPatternBase.h"

namespace App
{
	struct BarPattern : public CustomPatternBase<BarPattern>
	{
		CUSTOM_PATTERN_METHOD(0, GetBool, "bd11eb15-2696-4e92-a350-872a4276e447")
		bool GetBool()
		{
			if (OnGetBool) OnGetBool();
			return m_bool;
		}

		CUSTOM_PATTERN_METHOD(1, GetInt, "b6801851-50a9-493d-97f5-cd955f4be086")
		int GetInt()
		{
			if (OnGetInt) OnGetInt();
			return m_int;
		}

		CUSTOM_PATTERN_METHOD(2, SetBoolInt, "d8705c7f-68b2-435f-86e3-6a46a7d7f4ac")
		void SetBoolInt(bool boolVal, int intVal)
		{
			if (OnSetBoolInt) OnSetBoolInt(boolVal, intVal);
			m_bool = boolVal;
			m_int = intVal;
		}

		//
		// for testing
		//
		std::function<void(void)> OnGetBool = nullptr;
		std::function<void(void)> OnGetInt = nullptr;
		std::function<void(bool, int)> OnSetBoolInt = nullptr;

	private:
		bool m_bool = false;
		int m_int = 0;
		float m_float = 0.0f;
		double m_double = 0.0;
		std::wstring m_string;
	};
}
#pragma once
