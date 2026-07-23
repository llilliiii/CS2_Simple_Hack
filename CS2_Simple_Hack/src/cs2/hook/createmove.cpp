#include "createmove.h"

CreateMoveFn oCreateMove = nullptr;

double __fastcall hkCreateMove(CCSGOInput* pThis, unsigned int slot, CUserCmd* pCmd)
{
	double returnValue = oCreateMove(pThis, slot, pCmd);
	QAngle_t viewAngles(180.0f, 0.0f, 0.0f);

	if (!pThis || !pCmd)
		return returnValue;

	// Example: Check if the jump button is pressed
	//if (pCmd->nButtons.nValue & IN_JUMP)
	//{
	//	pCmd->nButtons.nValue &= ~IN_JUMP; // Remove the jump button from the command
	//}

	pCmd->SetSubTickAngle(viewAngles);

	return returnValue;
}
