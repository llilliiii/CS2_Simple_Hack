#include "createmove.h"
#include "cs2/classes/entity_scanner.h"
#include "cs2/cheat/cheat.h"
#include "cs2/settings.h"

// Instialise the Original Create Move function address
CreateMoveFn oCreateMove = nullptr;

// The hook function for create move
double __fastcall hkCreateMove(CCSGOInput* pThis, unsigned int slot, CUserCmd* pCmd)
{
	double returnValue = oCreateMove(pThis, slot, pCmd);

	// If the pointer is empty
	if (!pThis || !pCmd)
		return returnValue;

	EntityScanner scanner;

	// If the AimbotEnabled = True
	if (Settings::AimbotEnabled)
	{
		QAngle_t aimAngle = Cheat::Aimbot(scanner);

		// Invalid Angle
		if (aimAngle != QAngle_t(360.0f, 0.0f, 0.0f)) {
			pCmd->SetSubTickAngle(aimAngle);
		}
	}

	return returnValue;
}