#pragma once

#include <winrt/Windows.Foundation.h>
using namespace winrt::Windows::Foundation;

struct DummyElement : public winrt::implements<DummyElement, IInspectable>{};
