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

		TEST_METHOD(GetFoo)
		{
			App::FooPattern::RegisterMethods();
			Assert::AreEqual(4ull, Microsoft::UIA::TestOnly_RemoteOperationCount());

			auto fooPatternInstance = winrt::make<App::FooPattern>();
			int getFooCallCount = 0;
			fooPatternInstance.as<App::FooPattern>()->OnGetFoo = [&getFooCallCount]() { ++getFooCallCount; };

			Microsoft::UIA::RemoteOperationContext context;
			context.SetOperand(0, fooPatternInstance);

			Microsoft::UIA::CallRemoteOperationExtension(getFooGuid, context, { 0 });

			Assert::AreEqual(1, getFooCallCount);

			App::FooPattern::UnregisterMethods();
			Assert::AreEqual(0ull, Microsoft::UIA::TestOnly_RemoteOperationCount());
		}

		TEST_METHOD(SetFoo)
		{
			App::FooPattern::RegisterMethods();
			Assert::AreEqual(4ull, Microsoft::UIA::TestOnly_RemoteOperationCount());

			auto fooPatternInstance = winrt::make<App::FooPattern>();

			std::vector<std::wstring> arguments;
			fooPatternInstance.as<App::FooPattern>()->OnSetFoo = [&arguments](const std::wstring& val) { arguments.push_back(val); };

			Microsoft::UIA::RemoteOperationContext context;
			context.SetOperand(0, fooPatternInstance);
			context.SetOperand(1, winrt::box_value(L"string cheese"));

			Microsoft::UIA::CallRemoteOperationExtension(setFooGuid, context, { 0, 1 });

			Assert::AreEqual(1ull, arguments.size());
			Assert::AreEqual(L"string cheese", arguments[0].c_str());

			Assert::AreEqual(L"string cheese", fooPatternInstance.as<App::FooPattern>()->GetFoo());

			App::FooPattern::UnregisterMethods();
			Assert::AreEqual(0ull, Microsoft::UIA::TestOnly_RemoteOperationCount());
		}

	private:
		static inline GUID getFooGuid{ 0xe6b31052, 0x4a7e, 0x4dad, {0xb3, 0x41, 0x85, 0x56, 0x09, 0xf4, 0x22, 0x32} };
		static inline GUID setFooGuid{ 0x4426d571, 0x240c, 0x47bc, {0x8d, 0x5a, 0x51, 0xf2, 0x97, 0x4b, 0x4b, 0xeb} };
	};
}
