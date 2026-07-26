#include "createmove.h"
#include "cs2/classes/entity_scanner.h"
#include "cs2/cheat/cheat.h"
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

	QAngle_t aimAngles = QAngle_t(90.0f, 0.0f, 0.0f);

	if (pCmd) {
		volatile auto address =
			reinterpret_cast<std::uintptr_t>(pCmd);
	}

	TestCall(aimAngles);

	//pCmd->SetSubTickAngle(QAngle_t(90.0f, 0.0f, 0.0f));

	uintptr_t entity = 0;
	uintptr_t entity_lift_state = 0;
	std::string msg;
	for (int i = 0; i < 64; ++i) {
		entity = scanner.getEntity(i);
		if (!entity)
			continue;
		//entity_lift_state = scanner.getEntityLifeState(entity);
		//msg = std::format("Life State = {}\n", entity_lift_state);
		//OutputDebugStringA(msg.c_str());
	}
	uintptr_t localPlayer = scanner.getLocalPlayer();
	QAngle_t localViewAngles = scanner.getLocalViewAngles();
	msg = std::format("My ViewAngle: pitch = {}, yaw = {}, roll = {}\n", localViewAngles.pitch, localViewAngles.yaw, localViewAngles.roll);
	OutputDebugStringA(msg.c_str());


	//int8_t closestTeam = scanner.getEntityTeam(closestEnemy);
	//int8_t closestLifeState = scanner.getEntityLifeState(closestEnemy);
	//Vec3 closestPosition = scanner.getEntityPosition(closestEnemy);
	//Vec3 closestViewOffset = scanner.getEntityViewOffset(closestEnemy)

	//msg = std::format("closest Enemy = {}\n", closestEnemy);
	//OutputDebugStringA(msg.c_str());
	//msg = std::format("Local Life State = {}\n", closestEnemy);
	//OutputDebugStringA(msg.c_str());
	//msg = std::format("Local View Offset = {} {} {}\n", localViewOffset.x, localViewOffset.y, localViewOffset.z);
	//OutputDebugStringA(msg.c_str());
	//std::string msg = std::format("Pitch = {} Yaw = {}\n", aimAngles.pitch, aimAngles.yaw);
	//OutputDebugStringA(msg.c_str());
	//LogAngle(aimAngles);
	//if (aimAngles != QAngle_t(360.0f, 0.0f, 0.0f)) {
	//	pCmd->SetSubTickAngle(aimAngles);
	//}
	// Example: Check if the jump button is pressed
	//if (pCmd->nButtons.nValue & IN_JUMP)
	//{
	//	pCmd->nButtons.nValue &= ~IN_JUMP; // Remove the jump button from the command
	//}

	return returnValue;
}