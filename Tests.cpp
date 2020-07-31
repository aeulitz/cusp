#include "FooPattern.h"

#include <CppUnitTest.h>
#include <winrt/Windows.Foundation.h>

#include <functional>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

struct TestRegistrar
{
	template<class TMethodPointer>
	static constexpr void Register(const GUID& guid, TMethodPointer methodPointer)
	{
		TestRegistrar::OnRegister(guid);
	}

	static inline void Unregister(const GUID& guid)
	{
		TestRegistrar::OnUnregister(guid);
	}

	static std::function<void(const GUID& guid)> OnRegister;
	static std::function<void(const GUID& guid)> OnUnregister;
};

std::function<void(const GUID& guid)> TestRegistrar::OnRegister = [](const GUID&) {};
std::function<void(const GUID& guid)> TestRegistrar::OnUnregister = [](const GUID&) {};

namespace UnitTest1
{
	TEST_CLASS(FooPatternTests)
	{
	public:

		TEST_METHOD(PatternRegistration)
		{
			std::vector<GUID> guids;
			TestRegistrar::OnRegister = TestRegistrar::OnUnregister = [&guids](const GUID& guid) { guids.push_back(guid); };

			App::FooPattern::RegisterMethods<TestRegistrar>();

			Assert::AreEqual(4ull, guids.size());
			Assert::AreEqual(GUID{ 0x13e7a41d, 0xbad4, 0x4ab2, {0x80, 0x6b, 0xe5, 0x71, 0x28, 0x30, 0x26, 0x84} }, guids[0]);
			Assert::AreEqual(GUID{ 0xc583a435, 0xe0c1, 0x4ba9, {0xbe, 0x2d, 0x6b, 0x8d, 0x71, 0x4c, 0x79, 0x18} }, guids[1]);
			Assert::AreEqual(GUID{ 0x4426d571, 0x240c, 0x47bc, {0x8d, 0x5a, 0x51, 0xf2, 0x97, 0x4b, 0x4b, 0xeb} }, guids[2]);
			Assert::AreEqual(GUID{ 0xe6b31052, 0x4a7e, 0x4dad, {0xb3, 0x41, 0x85, 0x56, 0x09, 0xf4, 0x22, 0x32} }, guids[3]);

			guids.clear();

			App::FooPattern::UnregisterMethods<TestRegistrar>();

			Assert::AreEqual(4ull, guids.size());
			Assert::AreEqual(GUID{ 0x13e7a41d, 0xbad4, 0x4ab2, {0x80, 0x6b, 0xe5, 0x71, 0x28, 0x30, 0x26, 0x84} }, guids[0]);
			Assert::AreEqual(GUID{ 0xc583a435, 0xe0c1, 0x4ba9, {0xbe, 0x2d, 0x6b, 0x8d, 0x71, 0x4c, 0x79, 0x18} }, guids[1]);
			Assert::AreEqual(GUID{ 0x4426d571, 0x240c, 0x47bc, {0x8d, 0x5a, 0x51, 0xf2, 0x97, 0x4b, 0x4b, 0xeb} }, guids[2]);
			Assert::AreEqual(GUID{ 0xe6b31052, 0x4a7e, 0x4dad, {0xb3, 0x41, 0x85, 0x56, 0x09, 0xf4, 0x22, 0x32} }, guids[3]);
		}

		TEST_METHOD(GetString)
		{
			App::FooPattern::RegisterMethods();
			Assert::AreEqual(4ull, Microsoft::UIA::TestOnly_RemoteOperationCount());

			auto fooPatternInstance = winrt::make<App::FooPattern>();

			// set the value to be able to verify the return value of the 'GetString' call
			fooPatternInstance.as<App::FooPattern>()->SetString(L"pool noodle");

			int getStringCallCount = 0;
			fooPatternInstance.as<App::FooPattern>()->OnGetString = [&getStringCallCount]() { ++getStringCallCount; };

			Microsoft::UIA::RemoteOperationContext context;
			context.SetOperand(0, fooPatternInstance);

			Microsoft::UIA::CallRemoteOperationExtension(getStringGuid, context, { 0, 1 });

			Assert::AreEqual(1, getStringCallCount);

			Assert::AreEqual(L"pool noodle", winrt::unbox_value<winrt::hstring>(context.GetOperand(1)).c_str());

			App::FooPattern::UnregisterMethods();
			Assert::AreEqual(0ull, Microsoft::UIA::TestOnly_RemoteOperationCount());
		}

		TEST_METHOD(SetString)
		{
			App::FooPattern::RegisterMethods();
			Assert::AreEqual(4ull, Microsoft::UIA::TestOnly_RemoteOperationCount());

			auto fooPatternInstance = winrt::make<App::FooPattern>();

			std::vector<std::wstring> setStringArguments;
			fooPatternInstance.as<App::FooPattern>()->OnSetString = [&setStringArguments](const std::wstring& val) { setStringArguments.push_back(val); };

			Microsoft::UIA::RemoteOperationContext context;
			context.SetOperand(0, fooPatternInstance);
			context.SetOperand(1, winrt::box_value(L"string cheese"));

			Microsoft::UIA::CallRemoteOperationExtension(setStringGuid, context, { 0, 1 });

			Assert::AreEqual(1ull, setStringArguments.size());
			Assert::AreEqual(L"string cheese", setStringArguments[0].c_str());

			Assert::AreEqual(L"string cheese", fooPatternInstance.as<App::FooPattern>()->GetString());

			App::FooPattern::UnregisterMethods();
			Assert::AreEqual(0ull, Microsoft::UIA::TestOnly_RemoteOperationCount());
		}

		TEST_METHOD(ClearString)
		{
			App::FooPattern::RegisterMethods();
			Assert::AreEqual(4ull, Microsoft::UIA::TestOnly_RemoteOperationCount());

			auto fooPatternInstance = winrt::make<App::FooPattern>();

			// set the value to be able to verify the effect of the 'ClearString' call
			fooPatternInstance.as<App::FooPattern>()->SetString(L"meatballs");

			int clearStringCallCount = 0;
			fooPatternInstance.as<App::FooPattern>()->OnClearString = [&clearStringCallCount]() { ++clearStringCallCount; };

			Microsoft::UIA::RemoteOperationContext context;
			context.SetOperand(0, fooPatternInstance);

			Microsoft::UIA::CallRemoteOperationExtension(clearStringGuid, context, { 0 });

			Assert::AreEqual(1, clearStringCallCount);
			Assert::AreEqual(L"", fooPatternInstance.as<App::FooPattern>()->GetString());

			App::FooPattern::UnregisterMethods();
			Assert::AreEqual(0ull, Microsoft::UIA::TestOnly_RemoteOperationCount());
		}

	private:
		static inline GUID getStringGuid{ 0xe6b31052, 0x4a7e, 0x4dad, {0xb3, 0x41, 0x85, 0x56, 0x09, 0xf4, 0x22, 0x32} };
		static inline GUID setStringGuid{ 0x4426d571, 0x240c, 0x47bc, {0x8d, 0x5a, 0x51, 0xf2, 0x97, 0x4b, 0x4b, 0xeb} };
		static inline GUID clearStringGuid{ 0x13e7a41d, 0xbad4, 0x4ab2, {0x80, 0x6b, 0xe5, 0x71, 0x28, 0x30, 0x26, 0x84} };
	};
}
