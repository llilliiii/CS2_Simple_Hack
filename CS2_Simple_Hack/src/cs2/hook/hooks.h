#pragma once

#define PRESENT_PATTERN "48 89 6C 24 10 56 57 41 54"
#define CREATEMOVE_PATTERN "48 8B C4 4C 89 40 ? 48 89 48 ? 55 53 41 54"

namespace hook
{
	bool Init();
	void Shutdown();
	bool SetupPresentHook();
	bool SetupCreateMoveHook();
}