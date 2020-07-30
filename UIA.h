#pragma once

#include <guiddef.h>
#include <winrt/Windows.Foundation.h>

#include <functional>
#include <vector>

using namespace winrt::Windows::Foundation;

namespace Microsoft::UIA
{
	using OperandId = int;

	class RemoteOperationContext
	{
	public:
		IInspectable GetOperand(OperandId operandId) const;
		void SetOperand(OperandId operandId, IInspectable operandValue);

	private:
		const size_t MaxOperands = 10;
		std::vector<IInspectable> m_operands{ MaxOperands };
	};

	using Callback = std::function<void(RemoteOperationContext& context, const std::vector<OperandId>&)>;

	void AddRemoteOperationExtension(const GUID& guid, int n, Callback&& callback);
	void CallRemoteOperationExtension(
		const GUID& guid,
		RemoteOperationContext& context,
		const std::vector<OperandId>& operands);
	void RemoveRemoteOperationExtension(const GUID& guid);

	size_t TestOnly_RemoteOperationCount();
}
