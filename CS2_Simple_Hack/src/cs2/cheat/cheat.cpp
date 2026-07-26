#include "cheat.h"
#include "core/math/math.h"
#include "cs2/settings.h"
#include <format>

// Calculate the aiming angle
QAngle_t CalculateAngle(const Vec3& source, const Vec3& destination) {
	Vec3 delta = destination - source;
	float hypotenuse = std::sqrt(delta.x * delta.x + delta.y * delta.y);
	QAngle_t angles;
	angles.pitch = std::atan2(-delta.z, hypotenuse) * (180.0f / 3.14159265358979323846f);
	angles.yaw = std::atan2(delta.y, delta.x) * (180.0f / 3.14159265358979323846f);
	angles.roll = 0.0f; // Roll is typically not used in aiming
	return angles;
}

// Return the pointer of the closeset enemy
uintptr_t getClosestEnemy(uintptr_t localPlayer, EntityScanner& scan) {
	uintptr_t closestEntity = 0;
	float closestAngle = FLT_MAX;
	int8_t localTeam = scan.getEntityTeam(localPlayer);
	QAngle_t localViewAngles = scan.getLocalViewAngles();
	uintptr_t entity;
	int8_t entityTeam;
	int8_t entityLifeState;
	bool spotted;
	Vec3 localPos;
	Vec3 targetPos;
	QAngle_t aimAngles;
	float angle_diff;

	for (int i = 0; i < MAX_ENTITY_SCAN; ++i) {
		entity = scan.getEntity(i);

		// Check if the entity is valid and not the local player
		if (!entity || entity == localPlayer)
			continue;

		// Check if the entity is alive
		entityLifeState = scan.getEntityLifeState(entity);
		if (entityLifeState != 0)
			continue;

		// Check if the entity is on the same team or not spotted
		entityTeam = scan.getEntityTeam(entity);
		spotted = scan.getEntitySpottedState(entity);
		
		if (Settings::Hiding == true) {
			if (!spotted) {
				continue;
			}
		}
		else {
			if (entityTeam == localTeam) {
				continue;
			}
		}

		// Calculate the angle diff between the local player view angle and the angle of the entity
		localPos = scan.getEntityPosition(localPlayer);
		targetPos = scan.getEntityPosition(entity);
		aimAngles = CalculateAngle(localPos, targetPos);
		aimAngles.Normalize();
		angle_diff = ((localViewAngles - aimAngles).Normalized()).Length2DSquared();

		// Update the closest entity if this one is closer
		if (angle_diff < closestAngle) {
			closestAngle = angle_diff;
			closestEntity = entity;
		}
	}

	return closestEntity;
}

// Aimbot
QAngle_t Cheat::Aimbot(EntityScanner& scan) {
	// Get the local player entity
	uintptr_t localPlayer = scan.getLocalPlayer();

	// Invalid local player
	if (!localPlayer)
		return QAngle_t(360.0f, 0.0f, 0.0f);

	uintptr_t closestEnemy = getClosestEnemy(localPlayer, scan);

	// No closest enemy
	if (closestEnemy == 0)
		return QAngle_t(360.0f, 0.0f, 0.0f);

	// Calculate the head position
	Vec3 localPlayerHeadPos = scan.getEntityPosition(localPlayer) + scan.getEntityViewOffset(localPlayer);
	Vec3 closestEnemyPos = scan.getEntityPosition(closestEnemy) + scan.getEntityViewOffset(closestEnemy);

	// Adjust for closest enemy velocity
	Vec3 closestEnemyVelocity = scan.getEntityVelocity(closestEnemy);
	closestEnemyPos = closestEnemyPos + Vec3(closestEnemyVelocity.x * 0.01, closestEnemyVelocity.y * 0.01, closestEnemyVelocity.z * 0.01);

	// Calculate the aim angle
	QAngle_t aimAngles = CalculateAngle(localPlayerHeadPos, closestEnemyPos);
	aimAngles.Normalize();

	return aimAngles;
}