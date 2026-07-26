#include "hooks.h"
#include "minhook/include/MinHook.h"
#include "core/mem/mem.h"

#include "present.h"
#include "createmove.h"

bool hook::Init() {
	// Initialize the hook
	if (MH_Initialize() != MH_OK) {
		return false;
	}

	if (!SetupPresentHook()) {
		return false;
	}

	if (!SetupCreateMoveHook()) {
		return false;
	}

	MH_EnableHook(MH_ALL_HOOKS);

	return true;
}

void hook::Shutdown() {
	// Shutdown the hook
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize();
}

bool hook::SetupPresentHook() {
	// Setup the Present hook
	uintptr_t presentAddress = Mem::PatternScan(PRESENT_PATTERN, "GameOverlayRenderer64.dll");
	if (!presentAddress)
		return false;

	if (MH_CreateHook(reinterpret_cast<LPVOID>(presentAddress), &hkPresent, reinterpret_cast<LPVOID*>(&oPresent)) != MH_OK)
		return false;

	return true;
}

bool hook::SetupCreateMoveHook() {
	// Setup the CreateMove hook
	uintptr_t createmoveAddress = Mem::PatternScan(CREATEMOVE_PATTERN, "client.dll");
	if (!createmoveAddress)
		return false;

	if (MH_CreateHook(reinterpret_cast<LPVOID>(createmoveAddress), &hkCreateMove, reinterpret_cast<LPVOID*>(&oCreateMove)) != MH_OK)
		return false;

	return true;
}