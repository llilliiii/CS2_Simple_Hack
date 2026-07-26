#include "present.h"
#include "cs2/settings.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

// Initialise the variables
PresentFn oPresent = nullptr;

bool init = false;

HWND window = nullptr;
WNDPROC oWndProc = nullptr;

ID3D11Device* pDevice = nullptr;
ID3D11DeviceContext* pContext = nullptr;
ID3D11RenderTargetView* mainRenderTargetView = nullptr;

// Function prototype
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT __stdcall WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, msg, wParam, lParam);
}

void InitImGui() {
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);
}

// The hook function for present
HRESULT initHkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!init) {
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice))) {
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
			InitImGui();
			init = true;
		}
	}
	return S_OK;
}

HRESULT hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	initHkPresent(pSwapChain, SyncInterval, Flags);

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (Settings::menuVisible)
	{
		ImGui::Begin("Debug Menu", &Settings::menuVisible);

		ImGui::Checkbox(
			"Enable Aimbot",
			&Settings::AimbotEnabled
		);

		ImGui::Text(
			"Status: %s",
			Settings::AimbotEnabled
			? "Enabled"
			: "Disabled"
		);

		ImGui::Checkbox(
			"Enable Hiding",
			&Settings::Hiding
		);

		ImGui::Text(
			"Status: %s",
			Settings::Hiding
			? "Enabled"
			: "Disabled"
		);

		ImGui::End();
	}

	ImGui::Render();
	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return oPresent(pSwapChain, SyncInterval, Flags);
}
