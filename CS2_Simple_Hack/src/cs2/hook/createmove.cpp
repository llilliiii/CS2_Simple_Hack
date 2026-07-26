#include "createmove.h"
#include "cs2/classes/entity_scanner.h"
#include "cs2/cheat/cheat.h"
#include "cs2/settings.h"
#include <format>

CreateMoveFn oCreateMove = nullptr;

static void TestCall(const QAngle_t&) {
}

double __fastcall hkCreateMove(CCSGOInput* pThis, unsigned int slot, CUserCmd* pCmd)
{
	double returnValue = oCreateMove(pThis, slot, pCmd);

	if (!pThis || !pCmd)
		return returnValue;

	EntityScanner scanner;

	if (Settings::AimbotEnabled)
	{
		QAngle_t aimAngle = Cheat::Aimbot(scanner);
		if (aimAngle != QAngle_t(360.0f, 0.0f, 0.0f)) {
			pCmd->SetSubTickAngle(aimAngle);
		}
	}
	//msg = std::format("closest Enemy = {}\n", closestEnemy);
	//OutputDebugStringA(msg.c_str());

	// Example: Check if the jump button is pressed
	//if (pCmd->nButtons.nValue & IN_JUMP)
	//{
	//	pCmd->nButtons.nValue &= ~IN_JUMP; // Remove the jump button from the command
	//}

	return returnValue;
}