#include "cheat.h"
#include "core/math/math.h"

QAngle_t CalculateAngle(const Vec3& source, const Vec3& destination) {
	Vec3 delta = destination - source;
	float hypotenuse = std::sqrt(delta.x * delta.x + delta.y * delta.y);
	QAngle_t angles;
	angles.pitch = std::atan2(-delta.z, hypotenuse) * (180.0f / 3.14159265358979323846f);
	angles.yaw = std::atan2(delta.y, delta.x) * (180.0f / 3.14159265358979323846f);
	angles.roll = 0.0f; // Roll is typically not used in aiming
	return angles;
}

QAngle_t Cheat::Aimbot(EntityScanner& scan) {
	// Get the local player entity
	uintptr_t localPlayer = scan.getLocalPlayer();
	if (!localPlayer)
		return QAngle_t(360.0f, 0.0f, 0.0f);

	// Get the local player's team
	int8_t localTeam = scan.getEntityTeam(localPlayer);
	uintptr_t closestEnemy = scan.getClosestEnemy(localPlayer, localTeam, scan);

	if (closestEnemy == 0)
		return QAngle_t(360.0f, 0.0f, 0.0f);

	Vec3 localPlayerHeadPos = scan.getEntityPosition(localPlayer) + scan.getEntityViewOffset(localPlayer);
	Vec3 closestEnemyPos = scan.getEntityPosition(closestEnemy) + scan.getEntityViewOffset(closestEnemy);

	//LogPosition(localPlayerHeadPos);
	//LogPosition(closestEnemyPos);

	QAngle_t aimAngles = CalculateAngle(localPlayerHeadPos, closestEnemyPos);
	aimAngles.Normalize();

	//OutputDebugStringA("Returned from Aimbot\n");
	return aimAngles;
}