// Static Runtime List

#pragma once

#include <string>
#include <vector>

#define A03_PATTERN_METHOD(guid) MethodRegistrar{guid}

namespace Approach03
{
	struct RegistrationRecord
	{
		RegistrationRecord(const char* guid) : m_guid{ guid } {}
		const char* m_guid;
	};

	template<class TDerived>
	class PatternBase
	{
	public:
		struct MethodRegistrar {
			MethodRegistrar(const char* guid)
			{
				registrationRecords.emplace(guid);
			}
		};

		static void Register()
		{
			for (auto rr : registrationRecords)
			{
				std::cout << "register " << rr.m_guid << std::endl;
			}
		}

		static std::vector<RegistrationRecord> registrationRecords;
	};

	class FooPattern : public PatternBase<FooPattern>
	{
	public:
		// A03_PATTERN_METHOD("abc123")
		// Problem: We'd have to create a member variable here to get the ctor executed.
		const char* GetFoo()
		{
			return m_foo.c_str();
		}

		// A03_PATTERN_METHOD("abc124")
		const char* SetFoo(const char* val)
		{
			m_foo = val;
		}

		const int MethodCount = 2;

	private:
		std::string m_foo;
	};

}
