#pragma once

#include <guiddef.h>
#include "CppUnitTest.h"

#include <string>

namespace
{
	// helper array to convert a hexadecimal ASCII character into an unsigned char value
	constexpr const unsigned char H2U[256] = {
		// 16 rows x 16 cols
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0,
		0, 10, 11, 12, 13, 14, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 10, 11, 12, 13, 14, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	};

	inline wchar_t ToHexDigit(uint8_t nibble)
	{
		return nibble < 10 ? L'0' + nibble : L'a' + (nibble - 10);
	}
}

namespace Guid
{
	/// To represent a GUID string without curly braces: "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXX"
	typedef char GuidString[37];

	/// Converts string to a GUID at compile time. Expected format: "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXX"
	constexpr GUID StringToGuid(const GuidString& g) noexcept
	{
		return
		{
			static_cast<unsigned long>((H2U[g[0]] << 28) | (H2U[g[1]] << 24) | (H2U[g[2]] << 20) | (H2U[g[3]] << 16) | (H2U[g[4]] << 12) | (H2U[g[5]] << 8) | (H2U[g[6]] << 4) | H2U[g[7]]),
			static_cast<unsigned short>((H2U[g[9]] << 12) | (H2U[g[10]] << 8) | (H2U[g[11]] << 4) | H2U[g[12]]),
			static_cast<unsigned short>((H2U[g[14]] << 12) | (H2U[g[15]] << 8) | (H2U[g[16]] << 4) | H2U[g[17]]),
			{
				static_cast<unsigned char>((H2U[g[19]] << 4) | H2U[g[20]]),
				static_cast<unsigned char>((H2U[g[21]] << 4) | H2U[g[22]]),
				static_cast<unsigned char>((H2U[g[24]] << 4) | H2U[g[25]]),
				static_cast<unsigned char>((H2U[g[26]] << 4) | H2U[g[27]]),
				static_cast<unsigned char>((H2U[g[28]] << 4) | H2U[g[29]]),
				static_cast<unsigned char>((H2U[g[30]] << 4) | H2U[g[31]]),
				static_cast<unsigned char>((H2U[g[32]] << 4) | H2U[g[33]]),
				static_cast<unsigned char>((H2U[g[34]] << 4) | H2U[g[35]])
			}
		};
	}
}

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	// needed by the CppUnitTestFramework for the proper display of error messages
	template<> inline std::wstring ToString<GUID>(const GUID& g)
	{
		std::wstring s;
		return s
			.append(1, ToHexDigit((g.Data1 >> (7 * 4)) & 0x0F))
			.append(1, ToHexDigit((g.Data1 >> (6 * 4)) & 0x0F))
			.append(1, ToHexDigit((g.Data1 >> (5 * 4)) & 0x0F))
			.append(1, ToHexDigit((g.Data1 >> (4 * 4)) & 0x0F))
			.append(1, ToHexDigit((g.Data1 >> (3 * 4)) & 0x0F))
			.append(1, ToHexDigit((g.Data1 >> (2 * 4)) & 0x0F))
			.append(1, ToHexDigit((g.Data1 >> (1 * 4)) & 0x0F))
			.append(1, ToHexDigit((g.Data1 >> (0 * 4)) & 0x0F))
			.append(1, L'-')
			.append(1, ToHexDigit((g.Data2 >> (3 * 4)) & 0x0F))
			.append(1, ToHexDigit((g.Data2 >> (2 * 4)) & 0x0F))
			.append(1, ToHexDigit((g.Data2 >> (1 * 4)) & 0x0F))
			.append(1, ToHexDigit((g.Data2 >> (0 * 4)) & 0x0F))
			.append(1, L'-')
			.append(1, ToHexDigit((g.Data3 >> (3 * 4)) & 0x0F))
			.append(1, ToHexDigit((g.Data3 >> (2 * 4)) & 0x0F))
			.append(1, ToHexDigit((g.Data3 >> (1 * 4)) & 0x0F))
			.append(1, ToHexDigit((g.Data3 >> (0 * 4)) & 0x0F))
			.append(1, L'-')
			.append(1, ToHexDigit((g.Data4[0] >> 4) & 0x0F))
			.append(1, ToHexDigit((g.Data4[0] >> 0) & 0x0F))
			.append(1, ToHexDigit((g.Data4[1] >> 4) & 0x0F))
			.append(1, ToHexDigit((g.Data4[1] >> 0) & 0x0F))
			.append(1, L'-')
			.append(1, ToHexDigit((g.Data4[2] >> 4) & 0x0F))
			.append(1, ToHexDigit((g.Data4[2] >> 0) & 0x0F))
			.append(1, ToHexDigit((g.Data4[3] >> 4) & 0x0F))
			.append(1, ToHexDigit((g.Data4[3] >> 0) & 0x0F))
			.append(1, ToHexDigit((g.Data4[4] >> 4) & 0x0F))
			.append(1, ToHexDigit((g.Data4[4] >> 0) & 0x0F))
			.append(1, ToHexDigit((g.Data4[5] >> 4) & 0x0F))
			.append(1, ToHexDigit((g.Data4[5] >> 0) & 0x0F))
			.append(1, ToHexDigit((g.Data4[6] >> 4) & 0x0F))
			.append(1, ToHexDigit((g.Data4[6] >> 0) & 0x0F))
			.append(1, ToHexDigit((g.Data4[7] >> 4) & 0x0F))
			.append(1, ToHexDigit((g.Data4[7] >> 0) & 0x0F));
	}
}
