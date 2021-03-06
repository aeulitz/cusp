#include "BarPattern.h"
#include "DummyElement.h"
#include "FooPattern.h"

#include <CppUnitTest.h>
#include <winrt/Windows.Foundation.h>

#include <functional>

namespace CuspTest
{
	using namespace Microsoft::VisualStudio::CppUnitTestFramework;

	static GUID FooPattern_Guid{ 0x2bd720b1, 0xc433, 0x4292, {0x83, 0x14, 0xd4, 0xcc, 0xb8, 0xb7, 0xfa, 0xe7} };
	static GUID FooPattern_GetBool_Guid{ 0xa9487d87, 0x8935, 0x49ad, {0x94, 0x73, 0xfa, 0xdc, 0xe0, 0xbf, 0xa9, 0x74} };
	static GUID FooPattern_SetBool_Guid{ 0x25fb1199, 0xdb6f, 0x4349, {0x86, 0xaa, 0x43, 0x6d, 0x37, 0x6a, 0x68, 0x43} };
	static GUID FooPattern_GetInt_Guid{ 0x85e4d90e, 0xa804, 0x4a45, {0xa0, 0xe2, 0x3f, 0x57, 0x2f, 0x5c, 0xcf, 0xa9} };
	static GUID FooPattern_SetInt_Guid{ 0x921a5d67, 0x9a8f, 0x4c38, {0xb6, 0x76, 0x5c, 0x8c, 0x2d, 0x44, 0xef, 0x18} };
	static GUID FooPattern_GetFloat_Guid{ 0x71667878, 0x2e43, 0x4901, {0xaf, 0x9c, 0x0e, 0xb1, 0x39, 0xf7, 0x94, 0x39} };
	static GUID FooPattern_SetFloat_Guid{ 0x4d111a79, 0x4b56, 0x4a23, {0xae, 0xfa, 0xa5, 0x6b, 0x51, 0x33, 0x7f, 0x09} };
	static GUID FooPattern_GetDouble_Guid{ 0x643bd34c, 0xc22c, 0x4255, {0x92, 0xe7, 0xe2, 0xed, 0x4a, 0x10, 0x47, 0x82} };
	static GUID FooPattern_SetDouble_Guid{ 0x16c06415, 0x52f3, 0x4317, {0x8f, 0xfc, 0xa2, 0xd4, 0xb3, 0x9c, 0xdd, 0xfb} };
	static GUID FooPattern_GetString_Guid{ 0xe6b31052, 0x4a7e, 0x4dad, {0xb3, 0x41, 0x85, 0x56, 0x09, 0xf4, 0x22, 0x32} };
	static GUID FooPattern_SetString_Guid{ 0x4426d571, 0x240c, 0x47bc, {0x8d, 0x5a, 0x51, 0xf2, 0x97, 0x4b, 0x4b, 0xeb} };
	static GUID FooPattern_ClearString_Guid{ 0x13e7a41d, 0xbad4, 0x4ab2, {0x80, 0x6b, 0xe5, 0x71, 0x28, 0x30, 0x26, 0x84} };

	static GUID BarPattern_Guid{ 0x500b9f32, 0x17ee, 0x4540, {0xb9, 0xdf, 0x6d, 0xda, 0x4f, 0x8e, 0x83, 0x3d} };
	static GUID BarPattern_GetBool_Guid{ 0xbd11eb15, 0x2696, 0x4e92, {0xa3, 0x50, 0x87, 0x2a, 0x42, 0x76, 0xe4, 0x47} };
	static GUID BarPattern_GetInt_Guid{ 0xb6801851, 0x50a9, 0x493d, {0x97, 0xf5, 0xcd, 0x95, 0x5f, 0x4b, 0xe0, 0x86} };
	static GUID BarPattern_SetBoolInt_Guid{ 0xd8705c7f, 0x68b2, 0x435f, {0x86, 0xe3, 0x6a, 0x46, 0xa7, 0xd7, 0xf4, 0xac} };

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

	TEST_CLASS(FooPatternTests)
	{
	public:
		TEST_METHOD(PatternRegistration)
		{
			std::vector<GUID> guids;
			TestRegistrar::OnRegister = TestRegistrar::OnUnregister = [&guids](const GUID& guid) { guids.push_back(guid); };

			App::FooPattern::Register<TestRegistrar>();

			Assert::AreEqual(11ull, guids.size());
			Assert::AreEqual(FooPattern_ClearString_Guid, guids[0]);
			Assert::AreEqual(FooPattern_GetString_Guid, guids[1]);
			Assert::AreEqual(FooPattern_SetString_Guid, guids[2]);
			Assert::AreEqual(FooPattern_GetDouble_Guid, guids[3]);
			Assert::AreEqual(FooPattern_SetDouble_Guid, guids[4]);
			Assert::AreEqual(FooPattern_GetFloat_Guid, guids[5]);
			Assert::AreEqual(FooPattern_SetFloat_Guid, guids[6]);
			Assert::AreEqual(FooPattern_GetInt_Guid, guids[7]);
			Assert::AreEqual(FooPattern_SetInt_Guid, guids[8]);
			Assert::AreEqual(FooPattern_GetBool_Guid, guids[9]);
			Assert::AreEqual(FooPattern_SetBool_Guid, guids[10]);

			guids.clear();

			App::FooPattern::Unregister<TestRegistrar>();

			Assert::AreEqual(11ull, guids.size());
			Assert::AreEqual(FooPattern_ClearString_Guid, guids[0]);
			Assert::AreEqual(FooPattern_GetString_Guid, guids[1]);
			Assert::AreEqual(FooPattern_SetString_Guid, guids[2]);
			Assert::AreEqual(FooPattern_GetDouble_Guid, guids[3]);
			Assert::AreEqual(FooPattern_SetDouble_Guid, guids[4]);
			Assert::AreEqual(FooPattern_GetFloat_Guid, guids[5]);
			Assert::AreEqual(FooPattern_SetFloat_Guid, guids[6]);
			Assert::AreEqual(FooPattern_GetInt_Guid, guids[7]);
			Assert::AreEqual(FooPattern_SetInt_Guid, guids[8]);
			Assert::AreEqual(FooPattern_GetBool_Guid, guids[9]);
			Assert::AreEqual(FooPattern_SetBool_Guid, guids[10]);
		}

		TEST_METHOD(GetBool)
		{
			App::FooPattern::Register();

			auto element = winrt::make<DummyElement>();
			auto fooPatternProvider = winrt::make<App::FooPattern>(element);

			int getBoolCallCount = 0;
			fooPatternProvider.as<App::FooPattern>()->OnGetBool = [&getBoolCallCount]()
			{
				++getBoolCallCount;
				return true;
			};

			{
				// client actions

				Microsoft::UIA::RemoteOperationContext context;

				// retrieve pattern
				context.SetOperand(0, element);
				Microsoft::UIA::CallRemoteOperationExtension(FooPattern_Guid, context, { 0, 1 });
				auto fooPatternClient = context.GetOperand(1);

				// call pattern method
				context.SetOperand(0, fooPatternClient);
				Microsoft::UIA::CallRemoteOperationExtension(FooPattern_GetBool_Guid, context, { 0, 1 });

				Assert::IsTrue(winrt::unbox_value<bool>(context.GetOperand(1)));
			}

			Assert::AreEqual(1, getBoolCallCount);

			App::FooPattern::Unregister();
		}

		TEST_METHOD(SetBool)
		{
			App::FooPattern::Register();

			auto element = winrt::make<DummyElement>();
			auto fooPatternProvider = winrt::make<App::FooPattern>(element);

			std::vector<bool> setBoolArguments;
			fooPatternProvider.as<App::FooPattern>()->OnSetBool =
				[&setBoolArguments](bool val)
			{
				setBoolArguments.push_back(val);
			};

			{
				// client actions

				Microsoft::UIA::RemoteOperationContext context;

				// retrieve pattern
				context.SetOperand(0, element);
				Microsoft::UIA::CallRemoteOperationExtension(FooPattern_Guid, context, { 0, 1 });
				auto fooPatternClient = context.GetOperand(1);

				// call pattern method
				context.SetOperand(0, fooPatternClient);
				context.SetOperand(1, winrt::box_value(true));

				Microsoft::UIA::CallRemoteOperationExtension(FooPattern_SetBool_Guid, context, { 0, 1 });
			}

			Assert::AreEqual(1ull, setBoolArguments.size());

			App::FooPattern::Unregister();
		}

		TEST_METHOD(GetInt)
		{
			App::FooPattern::Register();

			auto element = winrt::make<DummyElement>();
			auto fooPatternProvider = winrt::make<App::FooPattern>(element);

			int getIntCallCount = 0;
			fooPatternProvider.as<App::FooPattern>()->OnGetInt =
				[&getIntCallCount]()
				{
					++getIntCallCount;
					return 23;
				};

			{
				// client actions

				Microsoft::UIA::RemoteOperationContext context;

				// retrieve pattern
				context.SetOperand(0, element);
				Microsoft::UIA::CallRemoteOperationExtension(FooPattern_Guid, context, { 0, 1 });
				auto fooPatternClient = context.GetOperand(1);

				// call pattern method
				context.SetOperand(0, fooPatternClient);
				Microsoft::UIA::CallRemoteOperationExtension(FooPattern_GetInt_Guid, context, { 0, 1 });

				Assert::AreEqual(23, winrt::unbox_value<int>(context.GetOperand(1)));
			}

			Assert::AreEqual(1, getIntCallCount);
			App::FooPattern::Unregister();
		}

		TEST_METHOD(SetInt)
		{
			App::FooPattern::Register();

			auto element = winrt::make<DummyElement>();
			auto fooPatternProvider = winrt::make<App::FooPattern>(element);

			std::vector<int> setIntArguments;
			fooPatternProvider.as<App::FooPattern>()->OnSetInt =
				[&setIntArguments]
				(int val)
				{
					setIntArguments.push_back(val);
				};

			{
				// client actions

				Microsoft::UIA::RemoteOperationContext context;

				// retrieve pattern
				context.SetOperand(0, element);
				Microsoft::UIA::CallRemoteOperationExtension(FooPattern_Guid, context, { 0, 1 });
				auto fooPatternClient = context.GetOperand(1);

				// call pattern method
				context.SetOperand(0, fooPatternClient);
				context.SetOperand(1, winrt::box_value(23));
				Microsoft::UIA::CallRemoteOperationExtension(FooPattern_SetInt_Guid, context, { 0, 1 });
			}

			Assert::AreEqual(1ull, setIntArguments.size());
			Assert::AreEqual(23, setIntArguments[0]);

			App::FooPattern::Unregister();
		}

		TEST_METHOD(GetFloat)
		{
			App::FooPattern::Register();

			auto element = winrt::make<DummyElement>();
			auto fooPatternProvider = winrt::make<App::FooPattern>(element);

			int getFloatCallCount = 0;
			fooPatternProvider.as<App::FooPattern>()->OnGetFloat =
				[&getFloatCallCount]()
				{
					++getFloatCallCount;
					return 1.234f;
				};

			{
				// client actions

				Microsoft::UIA::RemoteOperationContext context;

				// retrieve pattern
				context.SetOperand(0, element);
				Microsoft::UIA::CallRemoteOperationExtension(FooPattern_Guid, context, { 0, 1 });
				auto fooPatternClient = context.GetOperand(1);

				// call pattern method
				context.SetOperand(0, fooPatternClient);
				Microsoft::UIA::CallRemoteOperationExtension(FooPattern_GetFloat_Guid, context, { 0, 1 });
				Assert::AreEqual(1.234f, winrt::unbox_value<float>(context.GetOperand(1)));
			}

			Assert::AreEqual(1, getFloatCallCount);

			App::FooPattern::Unregister();
		}

		TEST_METHOD(SetFloat)
		{
			App::FooPattern::Register();

			auto element = winrt::make<DummyElement>();
			auto fooPatternProvider = winrt::make<App::FooPattern>(element);

			std::vector<float> setFloatArguments;
			fooPatternProvider.as<App::FooPattern>()->OnSetFloat = [&setFloatArguments](float val) { setFloatArguments.push_back(val); };

			{
				// client actions

				Microsoft::UIA::RemoteOperationContext context;

				// retrieve pattern
				context.SetOperand(0, element);
				Microsoft::UIA::CallRemoteOperationExtension(FooPattern_Guid, context, { 0, 1 });
				auto fooPatternClient = context.GetOperand(1);

				// call pattern method
				context.SetOperand(0, fooPatternClient);
				context.SetOperand(1, winrt::box_value(1.234f));
				Microsoft::UIA::CallRemoteOperationExtension(FooPattern_SetFloat_Guid, context, { 0, 1 });
			}

			Assert::AreEqual(1ull, setFloatArguments.size());
			Assert::AreEqual(1.234f, setFloatArguments[0]);

			App::FooPattern::Unregister();
		}

		TEST_METHOD(GetDouble)
		{
			App::FooPattern::Register();

			auto element = winrt::make<DummyElement>();
			auto fooPatternProvider = winrt::make<App::FooPattern>(element);

			int getDoubleCallCount = 0;
			fooPatternProvider.as<App::FooPattern>()->OnGetDouble =
				[&getDoubleCallCount]()
				{
					++getDoubleCallCount;
					return 1.234;
				};

			{
				// client actions

				Microsoft::UIA::RemoteOperationContext context;

				// retrieve pattern
				context.SetOperand(0, element);
				Microsoft::UIA::CallRemoteOperationExtension(FooPattern_Guid, context, { 0, 1 });
				auto fooPatternClient = context.GetOperand(1);

				// call pattern method
				context.SetOperand(0, fooPatternClient);
				Microsoft::UIA::CallRemoteOperationExtension(FooPattern_GetDouble_Guid, context, { 0, 1 });
				Assert::AreEqual(1.234, winrt::unbox_value<double>(context.GetOperand(1)));
			}

			Assert::AreEqual(1, getDoubleCallCount);

			App::FooPattern::Unregister();
		}

		TEST_METHOD(SetDouble)
		{
			App::FooPattern::Register();

			auto element = winrt::make<DummyElement>();
			auto fooPatternProvider = winrt::make<App::FooPattern>(element);

			std::vector<double> setDoubleArguments;
			fooPatternProvider.as<App::FooPattern>()->OnSetDouble =
				[&setDoubleArguments]
				(double val)
				{
					setDoubleArguments.push_back(val);
				};

			{
				// client actions

				Microsoft::UIA::RemoteOperationContext context;

				// retrieve pattern
				context.SetOperand(0, element);
				Microsoft::UIA::CallRemoteOperationExtension(FooPattern_Guid, context, { 0, 1 });
				auto fooPatternClient = context.GetOperand(1);

				// call pattern method
				context.SetOperand(0, fooPatternClient);
				context.SetOperand(1, winrt::box_value(1.234));
				Microsoft::UIA::CallRemoteOperationExtension(FooPattern_SetDouble_Guid, context, { 0, 1 });
			}

			Assert::AreEqual(1ull, setDoubleArguments.size());
			Assert::AreEqual(1.234, setDoubleArguments[0]);

			App::FooPattern::Unregister();
		}

		TEST_METHOD(GetString)
		{
			App::FooPattern::Register();
			Assert::AreEqual(12ull, Microsoft::UIA::TestOnly_RemoteOperationCount());

			auto element = winrt::make<DummyElement>();
			auto fooPatternProvider = winrt::make<App::FooPattern>(element);

			int getStringCallCount = 0;
			fooPatternProvider.as<App::FooPattern>()->OnGetString =
				[&getStringCallCount]()
				{
					++getStringCallCount;
					return L"pool noodle";
				};

			{
				// client actions

				Microsoft::UIA::RemoteOperationContext context;

				// retrieve pattern
				context.SetOperand(0, element);
				Microsoft::UIA::CallRemoteOperationExtension(FooPattern_Guid, context, { 0, 1 });
				auto fooPatternClient = context.GetOperand(1);

				// call pattern method
				context.SetOperand(0, fooPatternClient);
				Microsoft::UIA::CallRemoteOperationExtension(FooPattern_GetString_Guid, context, { 0, 1 });
				Assert::AreEqual(L"pool noodle", winrt::unbox_value<winrt::hstring>(context.GetOperand(1)).c_str());
			}

			Assert::AreEqual(1, getStringCallCount);

			App::FooPattern::Unregister();
			Assert::AreEqual(0ull, Microsoft::UIA::TestOnly_RemoteOperationCount());
		}

		TEST_METHOD(SetString)
		{
			App::FooPattern::Register();

			auto element = winrt::make<DummyElement>();
			auto fooPatternProvider = winrt::make<App::FooPattern>(element);

			std::vector<std::wstring> setStringArguments;
			fooPatternProvider.as<App::FooPattern>()->OnSetString =
				[&setStringArguments]
				(const std::wstring& val)
				{
					setStringArguments.push_back(val);
				};

			{
				// client actions

				Microsoft::UIA::RemoteOperationContext context;

				// retrieve pattern
				context.SetOperand(0, element);
				Microsoft::UIA::CallRemoteOperationExtension(FooPattern_Guid, context, { 0, 1 });
				auto fooPatternClient = context.GetOperand(1);

				// call pattern method
				context.SetOperand(0, fooPatternClient);
				context.SetOperand(1, winrt::box_value(L"string cheese"));
				Microsoft::UIA::CallRemoteOperationExtension(FooPattern_SetString_Guid, context, { 0, 1 });
			}

			Assert::AreEqual(1ull, setStringArguments.size());
			Assert::AreEqual(L"string cheese", setStringArguments[0].c_str());

			App::FooPattern::Unregister();
		}

		TEST_METHOD(ClearString)
		{
			App::FooPattern::Register();

			auto element = winrt::make<DummyElement>();
			auto fooPatternProvider = winrt::make<App::FooPattern>(element);

			int clearStringCallCount = 0;
			fooPatternProvider.as<App::FooPattern>()->OnClearString = [&clearStringCallCount]() { ++clearStringCallCount; };

			{
				// client actions

				Microsoft::UIA::RemoteOperationContext context;

				// retrieve pattern
				context.SetOperand(0, element);
				Microsoft::UIA::CallRemoteOperationExtension(FooPattern_Guid, context, { 0, 1 });
				auto fooPatternClient = context.GetOperand(1);

				// call pattern method
				context.SetOperand(0, fooPatternClient);
				Microsoft::UIA::CallRemoteOperationExtension(FooPattern_ClearString_Guid, context, { 0 });
			}

			Assert::AreEqual(1, clearStringCallCount);

			App::FooPattern::Unregister();
		}

		TEST_METHOD(MultipleInstances)
		{
			App::FooPattern::Register();

			auto element1 = winrt::make<DummyElement>();
			auto fooPatternProvider1 = winrt::make<App::FooPattern>(element1);
			int setBoolCallCount1 = 0;
			fooPatternProvider1.as<App::FooPattern>()->OnSetBool = [&setBoolCallCount1](bool) { ++setBoolCallCount1; };

			auto element2 = winrt::make<DummyElement>();
			auto fooPatternProvider2 = winrt::make<App::FooPattern>(element2);
			int setBoolCallCount2 = 0;
			fooPatternProvider2.as<App::FooPattern>()->OnSetBool = [&setBoolCallCount2](bool) { ++setBoolCallCount2; };

			{
				// client actions

				Microsoft::UIA::RemoteOperationContext context;

				// retrieve pattern
				context.SetOperand(0, element1);
				Microsoft::UIA::CallRemoteOperationExtension(FooPattern_Guid, context, { 0, 1 });
				auto fooPatternClient1 = context.GetOperand(1);

				context.SetOperand(0, fooPatternClient1);
				context.SetOperand(1, winrt::box_value(true));
				Microsoft::UIA::CallRemoteOperationExtension(FooPattern_SetBool_Guid, context, { 0, 1 });
			}

			Assert::AreEqual(1, setBoolCallCount1);
			Assert::AreEqual(0, setBoolCallCount2);

			App::FooPattern::Unregister();
		}

		TEST_METHOD(DeadInstance)
		{
			App::FooPattern::Register();

			auto element = winrt::make<DummyElement>();

			{
				auto fooPattern = winrt::make<App::FooPattern>(element);
				// only strong ref to FooPattern instance expires
			}

			{
				// client actions

				Microsoft::UIA::RemoteOperationContext context;

				// retrieve pattern
				context.SetOperand(0, element);
				Microsoft::UIA::CallRemoteOperationExtension(FooPattern_Guid, context, { 0, 1 });
				Assert::IsFalse((bool)context.GetOperand(1));
			}


			App::FooPattern::Unregister();
		}
	};

	TEST_CLASS(BarPatternTests)
	{
	public:
		TEST_METHOD(PatternRegistration)
		{
			std::vector<GUID> guids;
			TestRegistrar::OnRegister = TestRegistrar::OnUnregister = [&guids](const GUID& guid) { guids.push_back(guid); };

			App::BarPattern::Register<TestRegistrar>();

			Assert::AreEqual(3ull, guids.size());
			Assert::AreEqual(BarPattern_SetBoolInt_Guid, guids[0]);
			Assert::AreEqual(BarPattern_GetInt_Guid, guids[1]);
			Assert::AreEqual(BarPattern_GetBool_Guid, guids[2]);

			guids.clear();

			App::BarPattern::Unregister<TestRegistrar>();

			Assert::AreEqual(3ull, guids.size());
			Assert::AreEqual(BarPattern_SetBoolInt_Guid, guids[0]);
			Assert::AreEqual(BarPattern_GetInt_Guid, guids[1]);
			Assert::AreEqual(BarPattern_GetBool_Guid, guids[2]);
		}

		TEST_METHOD(SetBoolInt)
		{
			App::BarPattern::Register();

			auto element = winrt::make<DummyElement>();
			auto barPatternInstance = winrt::make<App::BarPattern>(element);

			std::vector<std::pair<bool, int>> setBoolIntArguments;
			barPatternInstance.as<App::BarPattern>()->OnSetBoolInt =
				[&setBoolIntArguments](bool boolVal, int intVal)
				{
					setBoolIntArguments.emplace_back(boolVal, intVal);
				};

			Microsoft::UIA::RemoteOperationContext context;
			context.SetOperand(0, barPatternInstance);
			context.SetOperand(1, winrt::box_value(true));
			context.SetOperand(2, winrt::box_value(23));

			Microsoft::UIA::CallRemoteOperationExtension(BarPattern_SetBoolInt_Guid, context, { 0, 1, 2 });

			Assert::AreEqual(1ull, setBoolIntArguments.size());
			Assert::IsTrue(setBoolIntArguments[0].first);
			Assert::AreEqual(23, setBoolIntArguments[0].second);

			App::BarPattern::Unregister();
		}
	};
}
