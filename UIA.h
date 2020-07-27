#pragma once

#include <guiddef.h>
#include <wrl.h>

#include <functional>
#include <vector>

namespace wrl = Microsoft::WRL;

namespace Microsoft::UIA
{
	using OperandId = int;

	class RemoteOperationContext
	{
	public:
		wrl::ComPtr<IInspectable> GetOperand(OperandId operandId) const;
		void SetOperand(OperandId operandId, wrl::ComPtr<IInspectable> operandValue);

	private:
		const size_t MaxOperands = 10;
		std::vector<wrl::ComPtr<IInspectable>> m_operands{ MaxOperands };
	};

	using Callback = std::function<void(RemoteOperationContext& context, const std::vector<OperandId>&)>;

	void AddRemoteOperationExtension(GUID guid, int n, Callback&& callback);
	void CallRemoteOperationExtension(
		GUID guid,
		RemoteOperationContext& context,
		const std::vector<OperandId>& operands);
}
