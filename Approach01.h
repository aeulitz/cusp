#define INITGUID

#pragma once

#include "UIA.h"

namespace uia = Microsoft::UIA;
namespace wrl = Microsoft::WRL;

#define A01_GET_METHOD_REGISTRAR_NAME(className, methodName, suffix) className##methodName##suffix

#define A01_PATTERN(patternClass, patternGuid) \
	static_assert(sizeof(patternGuid) == 37, "GUID string must have the following format: \"XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX\""); \
	class __declspec(uuid(patternGuid)) patternClass; \

#define A01_PATTERN_METHOD(patternClass, methodName, methodGuid)                                   \
	struct A01_GET_METHOD_REGISTRAR_NAME(patternClass, methodName, Registrar) {                    \
		inline static void RegisterMethod() {                                                      \
			/* TODO: how to chain previous RegisterMethod? */                                      \
		}                                                                                          \
	};                                                                                             \

namespace Approach01
{
	DEFINE_GUID(NamePattern_Guid, 0x7bca1cb8, 0xd74c, 0x486a, 0x94, 0x75, 0xac, 0xc4, 0xaa, 0xa7, 0x37, 0x80);
	DEFINE_GUID(NamePattern_GetName_Guid, 0x7a9068b7, 0xd16a, 0x443b, 0x9e, 0xc9, 0xbe, 0xb5, 0xac, 0xf0, 0xdb, 0xd7);
	DEFINE_GUID(NamePattern_SetName_Guid, 0xa3632090, 0x5424, 0x4ac4, 0x89, 0x7b, 0xd5, 0xf6, 0x2d, 0x86, 0xac, 0x64);

	template<class TDerived>
	class CustomPatternBase : public wrl::RuntimeClass<IInspectable>
	{
	public:
		static void Register(wrl::ComPtr<TDerived> patternInstance)
		{
			uia::AddRemoteOperationExtension(
				__uuidof(TDerived),
				0, // TODO: figure out what this parameter is
				[patternInstance](uia::RemoteOperationContext& context, const std::vector<uia::OperandId>& operandIds)
				{
					// TODO: associate with an element
					context.SetOperand(operandIds[1], patternInstance);
				});

			// IDEA:
			// CUSTOM_PATTERN_METHOD macro expanding to unique sub-class?
			// CustomPatternMethodRegistrar<TDerived>.RegisterMethods();



		}

		// Idea:
		// If possible, object ctor that adds to static list registry list?

		// An idea - I don't like it because it causes O(number_of_methods) at runtime.
		// static void DispatchMemberCall(uia::RemoteOperationContext& context, const std::vector<uia::OperandId>& operandIds)
		// {
		//
		// }

	private:

		// pointer to member
		// bool (Window::* pfnwSC)( const char * ) = &Window::SetCaption;
		//                 ^^^^^^ variable (not type)
		// (pwChildWindow->*pfnwSC)( szCaptionBase );
		//template<>
		//static CallMethod(uia::RemoteOperationContext& context, const std::vector<uia::OperandId>& operandIds)
		//{
		//	(static_cast<TDerived*>(context.GetOperand(operandIds[0]))->*PTM)();
		//}
	};

	A01_PATTERN(NamePattern, "7bca1cb8-d74c-486a-9475-acc4aaa73780")
	class NamePattern : public CustomPatternBase<NamePattern>
	{
	public:
		A01_PATTERN_METHOD(NamePattern, GetName, "7a9068b7-d16a-443b-9ec9-beb5acf0dbd7")
		std::string GetName() const
		{
			return m_name;
		}

		A01_PATTERN_METHOD(NamePattern, SetName, "a3632090-5424-4ac4-897b-d5f62d86ac64")
		void SetName(const char* value)
		{
			m_name = value;
		}

	private:
		std::string m_name;
	};

	void InitializeProvider()
	{
		wrl::ComPtr<NamePattern> np = wrl::Make<NamePattern>();
		NamePattern::Register(np);
	}

	void SimulateClient()
	{
#pragma region Get the pattern

		uia::RemoteOperationContext context1;
		std::vector<uia::OperandId> operandIds1{ 0, 1 };
		uia::CallRemoteOperationExtension(NamePattern_Guid, context1, operandIds1);

#pragma endregion

#pragma region Call a method on the pattern

		uia::RemoteOperationContext context2;
		std::vector<uia::OperandId> operandIds2{ 0, 1 };

		// transfer the pattern's 'this' pointer
		context2.SetOperand(operandIds2[0], context1.GetOperand(operandIds1[1]));

		uia::CallRemoteOperationExtension(NamePattern_GetName_Guid, context2, operandIds2);

#pragma endregion
	}

	void Main()
	{
		InitializeProvider();
		SimulateClient();
	}
}
