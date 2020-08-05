#pragma once

#include "CustomPatternBase.h"

namespace App
{
	CUSTOM_PATTERN(BarPattern, "500b9f32-17ee-4540-b9df-6dda4f8e833d")
	{
		BarPattern(IInspectable element) : Super(element) {}

		CUSTOM_PATTERN_METHOD(0, GetBool, "bd11eb15-2696-4e92-a350-872a4276e447")
		bool GetBool()
		{
			return OnGetBool();
		}

		CUSTOM_PATTERN_METHOD(1, GetInt, "b6801851-50a9-493d-97f5-cd955f4be086")
		int GetInt()
		{
			return OnGetInt();
		}

		CUSTOM_PATTERN_METHOD(2, SetBoolInt, "d8705c7f-68b2-435f-86e3-6a46a7d7f4ac")
		void SetBoolInt(bool boolVal, int intVal)
		{
			OnSetBoolInt(boolVal, intVal);
		}

		//
		// for testing
		//
		std::function<bool(void)> OnGetBool = nullptr;
		std::function<int(void)> OnGetInt = nullptr;
		std::function<void(bool, int)> OnSetBoolInt = nullptr;
	};
}
#pragma once
