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

			Assert::AreEqual(7ull, guids.size());
			Assert::AreEqual(getBoolGuid, guids[0]);
			Assert::AreEqual(setBoolGuid, guids[1]);
			Assert::AreEqual(getIntGuid, guids[2]);
			Assert::AreEqual(setIntGuid, guids[3]);
			Assert::AreEqual(clearStringGuid, guids[4]);
			Assert::AreEqual(setStringGuid, guids[5]);
			Assert::AreEqual(getStringGuid, guids[6]);

			guids.clear();

			App::FooPattern::UnregisterMethods<TestRegistrar>();

			Assert::AreEqual(7ull, guids.size());
			Assert::AreEqual(getBoolGuid, guids[0]);
			Assert::AreEqual(setBoolGuid, guids[1]);
			Assert::AreEqual(getIntGuid, guids[2]);
			Assert::AreEqual(setIntGuid, guids[3]);
			Assert::AreEqual(clearStringGuid, guids[4]);
			Assert::AreEqual(setStringGuid, guids[5]);
			Assert::AreEqual(getStringGuid, guids[6]);
		}

		TEST_METHOD(GetString)
		{
			App::FooPattern::RegisterMethods();
			Assert::AreEqual(7ull, Microsoft::UIA::TestOnly_RemoteOperationCount());

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
		}

		TEST_METHOD(ClearString)
		{
			App::FooPattern::RegisterMethods();

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
		}

		TEST_METHOD(GetInt)
		{
			App::FooPattern::RegisterMethods();

			auto fooPatternInstance = winrt::make<App::FooPattern>();

			// set the value to be able to verify the return value of the 'GetInt' call
			fooPatternInstance.as<App::FooPattern>()->SetInt(23);

			int getIntCallCount = 0;
			fooPatternInstance.as<App::FooPattern>()->OnGetInt = [&getIntCallCount]() { ++getIntCallCount; };

			Microsoft::UIA::RemoteOperationContext context;
			context.SetOperand(0, fooPatternInstance);

			Microsoft::UIA::CallRemoteOperationExtension(getIntGuid, context, { 0, 1 });

			Assert::AreEqual(1, getIntCallCount);

			Assert::AreEqual(23, winrt::unbox_value<int>(context.GetOperand(1)));

			App::FooPattern::UnregisterMethods();
		}

		TEST_METHOD(SetInt)
		{
			App::FooPattern::RegisterMethods();

			auto fooPatternInstance = winrt::make<App::FooPattern>();

			std::vector<int> setIntArguments;
			fooPatternInstance.as<App::FooPattern>()->OnSetInt = [&setIntArguments](int val) { setIntArguments.push_back(val); };

			Microsoft::UIA::RemoteOperationContext context;
			context.SetOperand(0, fooPatternInstance);
			context.SetOperand(1, winrt::box_value(23));

			Microsoft::UIA::CallRemoteOperationExtension(setIntGuid, context, { 0, 1 });

			Assert::AreEqual(1ull, setIntArguments.size());
			Assert::AreEqual(23, setIntArguments[0]);

			Assert::AreEqual(23, fooPatternInstance.as<App::FooPattern>()->GetInt());

			App::FooPattern::UnregisterMethods();
		}

		TEST_METHOD(GetBool)
		{
			App::FooPattern::RegisterMethods();

			auto fooPatternInstance = winrt::make<App::FooPattern>();

			// set the value to be able to verify the return value of the 'GetBool' call
			fooPatternInstance.as<App::FooPattern>()->SetBool(true);

			int getBoolCallCount = 0;
			fooPatternInstance.as<App::FooPattern>()->OnGetBool = [&getBoolCallCount]() { ++getBoolCallCount; };

			Microsoft::UIA::RemoteOperationContext context;
			context.SetOperand(0, fooPatternInstance);

			Microsoft::UIA::CallRemoteOperationExtension(getBoolGuid, context, { 0, 1 });

			Assert::AreEqual(1, getBoolCallCount);

			Assert::IsTrue(winrt::unbox_value<bool>(context.GetOperand(1)));

			App::FooPattern::UnregisterMethods();
		}

		TEST_METHOD(SetBool)
		{
			App::FooPattern::RegisterMethods();

			auto fooPatternInstance = winrt::make<App::FooPattern>();

			std::vector<bool> setBoolArguments;
			fooPatternInstance.as<App::FooPattern>()->OnSetBool = [&setBoolArguments](bool val) { setBoolArguments.push_back(val); };

			Microsoft::UIA::RemoteOperationContext context;
			context.SetOperand(0, fooPatternInstance);
			context.SetOperand(1, winrt::box_value(true));

			Microsoft::UIA::CallRemoteOperationExtension(setBoolGuid, context, { 0, 1 });

			Assert::AreEqual(1ull, setBoolArguments.size());
			Assert::IsTrue(setBoolArguments[0]);

			Assert::IsTrue(fooPatternInstance.as<App::FooPattern>()->GetBool());

			App::FooPattern::UnregisterMethods();
		}

	private:
		static inline GUID setBoolGuid{ 0x25fb1199, 0xdb6f, 0x4349, {0x86, 0xaa, 0x43, 0x6d, 0x37, 0x6a, 0x68, 0x43} };
		static inline GUID getBoolGuid{ 0xa9487d87, 0x8935, 0x49ad, {0x94, 0x73, 0xfa, 0xdc, 0xe0, 0xbf, 0xa9, 0x74} };
		static inline GUID setIntGuid{ 0x921a5d67, 0x9a8f, 0x4c38, {0xb6, 0x76, 0x5c, 0x8c, 0x2d, 0x44, 0xef, 0x18} };
		static inline GUID getIntGuid{ 0x85e4d90e, 0xa804, 0x4a45, {0xa0, 0xe2, 0x3f, 0x57, 0x2f, 0x5c, 0xcf, 0xa9} };
		static inline GUID getStringGuid{ 0xe6b31052, 0x4a7e, 0x4dad, {0xb3, 0x41, 0x85, 0x56, 0x09, 0xf4, 0x22, 0x32} };
		static inline GUID setStringGuid{ 0x4426d571, 0x240c, 0x47bc, {0x8d, 0x5a, 0x51, 0xf2, 0x97, 0x4b, 0x4b, 0xeb} };
		static inline GUID clearStringGuid{ 0x13e7a41d, 0xbad4, 0x4ab2, {0x80, 0x6b, 0xe5, 0x71, 0x28, 0x30, 0x26, 0x84} };
		// static inline GUID Guid{ 0x, 0x, 0x, {0x, 0x, 0x, 0x, 0x, 0x, 0x, 0x} };
	};
}
