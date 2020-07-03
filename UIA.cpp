#include "UIA.h"

#include <cassert>
#include <unordered_map>

template<>
struct std::hash<GUID>
{
	size_t operator()(GUID guid) const
	{
		// typedef struct _GUID {
		// 	unsigned long  Data1;
		// 	unsigned short Data2;
		// 	unsigned short Data3;
		// 	unsigned char  Data4[8];
		// } GUID;

		return
			guid.Data1 ^
			((guid.Data2 << 16) | guid.Data3) ^
			reinterpret_cast<uint32_t>(guid.Data4) ^
			reinterpret_cast<uint32_t>(guid.Data4 + 4);
	}
};

namespace Microsoft::UIA
{
	wrl::ComPtr<IInspectable> RemoteOperationContext::GetOperand(OperandId operandId) const
	{
		return m_operands[operandId];
	}

	void RemoteOperationContext::SetOperand(OperandId operandId, wrl::ComPtr<IInspectable> operandValue)
	{
		m_operands[operandId] = operandValue;
	}

	static std::unordered_map<GUID, Callback> callbackRegistry;


	void AddRemoteOperationExtension(GUID guid, int n, Callback&& callback)
	{
		// REVIEW: What is 'n'? Do we need to store it?

		const auto[it, success] = callbackRegistry.emplace(guid, std::move(callback));
		assert(success);
	}

	void CallRemoteOperationExtension(
		GUID guid,
		RemoteOperationContext& context,
		const std::vector<OperandId>& operands)
	{
		callbackRegistry[guid](context, operands);
	}
}