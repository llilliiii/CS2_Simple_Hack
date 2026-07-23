#pragma once
#include <d3d11.h>

typedef HRESULT(__stdcall* PresentFn)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);

extern PresentFn oPresent;

HRESULT hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);