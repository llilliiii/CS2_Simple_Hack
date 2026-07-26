#pragma once
#include <Windows.h>
#include <cstdint>
#include <stdexcept>
#include <core/math/math.h>
#include <cs2/output/offsets.hpp>
#include <cs2/output/client_dll.hpp>
#include <format>

#define MAX_ENTITY_SCAN 64
#define List_Entry_Offset 0x10
#define Controler_Offset 0x78

class EntityScanner {
	uintptr_t clientDllBase;
	uintptr_t entityList;
	uintptr_t listEntry1;

public:
	EntityScanner() {
		clientDllBase = reinterpret_cast<uintptr_t>(GetModuleHandleA("client.dll"));
		entityList = *reinterpret_cast<uintptr_t*>(clientDllBase + cs2_dumper::offsets::client_dll::dwEntityList);
		listEntry1 = *reinterpret_cast<uintptr_t*>(entityList + 0x10);
		//std::string msg = std::format("Client DLL Base = {}\n", clientDllBase);
		//OutputDebugStringA(msg.c_str());
		//msg = std::format("entityListPrt = {}\n", entityListPrt);
		//OutputDebugStringA(msg.c_str());
		//msg = std::format("entityList = {}\n", entityList);
		//OutputDebugStringA(msg.c_str());
		//msg = std::format("entry = {}\n", listEntry);
		//OutputDebugStringA(msg.c_str());
	}

	uintptr_t getEntity(int index) {
		if (index < 0 || index >= MAX_ENTITY_SCAN) {
			return 0; // Invalid index
		}

		auto entityController = *reinterpret_cast<uintptr_t*>(listEntry1 + index * 0x70);
		if (!entityController) {
			return 0; // Invalid controller
		}

		auto entityPawnHandle = *reinterpret_cast<uintptr_t*>(entityController + cs2_dumper::schemas::client_dll::CCSPlayerController::m_hPlayerPawn);
		//auto entityPawnHandle = *(uint64_t*)(entityController + cs2_dumper::schemas::client_dll::CCSPlayerController::m_hPlayerPawn);
		if (!entityPawnHandle) {
			return 0; // Invalid pawn handle
		}

		auto listEntry2 = *reinterpret_cast<uintptr_t*>(entityList + 0x8 * ((entityPawnHandle & 0x7FFF) >> 9) + 0x10);
		
		if (!listEntry2) {
			return 0; // Invalid list entry2
		}

		auto entity = *reinterpret_cast<uintptr_t*>(listEntry2 + 0x70 * (entityPawnHandle & 0x1FF));

		if (!entity) {
			return 0; // Invalid entity
		}

		return entity;
	}

	uintptr_t getLocalPlayer() {
		return *(uintptr_t*)(clientDllBase + cs2_dumper::offsets::client_dll::dwLocalPlayerPawn);
	}

	int8_t getEntityTeam(uintptr_t entity) {
		if (!entity) {
			return 0; // Invalid entity
		}
		return *reinterpret_cast<int8_t*>(entity + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum);
	}

	int8_t getEntityLifeState(uintptr_t entity) {
		if (!entity) {
			return 0; // Invalid entity
		}
		return *reinterpret_cast<int8_t*>(entity + cs2_dumper::schemas::client_dll::C_BaseEntity::m_lifeState);
	}

	static Vec3 getEntityPosition(uintptr_t entity) {
		if (!entity) {
			return Vec3(); // Invalid entity
		}
		static_assert(sizeof(Vec3) == sizeof(float) * 3);
		return *reinterpret_cast<Vec3*>(entity + cs2_dumper::schemas::client_dll::C_BasePlayerPawn::m_vOldOrigin);
	}
	
	static Vec3 getEntityViewOffset(uintptr_t entity) {
		if (!entity) {
			return Vec3(); // Invalid entity
		}
		static_assert(sizeof(Vec3) == sizeof(float) * 3);
		return *reinterpret_cast<Vec3*>(entity + cs2_dumper::schemas::client_dll::C_BaseModelEntity::m_vecViewOffset);
	}

	QAngle_t getLocalViewAngles() {
		static_assert(sizeof(QAngle_t) == sizeof(float) * 3);
		return *reinterpret_cast<QAngle_t*>(clientDllBase + cs2_dumper::offsets::client_dll::dwViewAngles);
	}

	uintptr_t getClosestEnemy(uintptr_t localPlayer, int8_t localTeam, EntityScanner& scan) {
		uintptr_t closestEntity = 0;
		float closestDistance = FLT_MAX;
		int8_t entityTeam;
		int8_t entityLifeState;
		uintptr_t entity;
		Vec3 localPos;
		Vec3 targetPos;

		for (int i = 0; i < MAX_ENTITY_SCAN; ++i) {
			entity = scan.getEntity(i);

			// Check if the entity is valid and not the local player
			if (!entity || entity == localPlayer)
				continue;

			// Check if the entity is on the same team
			entityTeam = scan.getEntityTeam(entity);
			//std::string msg = std::format("Team = {}\n",entityTeam);
			//OutputDebugStringA(msg.c_str());
			if (entityTeam == localTeam)
				continue;
			
			// Check if the entity is alive
			entityLifeState = scan.getEntityLifeState(entity);
			if (entityLifeState)
				continue;

			// Calculate the distance between the local player and the entity
			localPos = getEntityPosition(localPlayer);
			targetPos = getEntityPosition(entity);
			float distance = (localPos - targetPos).Length();

			// Update the closest entity if this one is closer
			if (distance < closestDistance) {
				closestDistance = distance;
				closestEntity = entity;
			}
		}

		return closestEntity;
	}

	static QAngle_t getPunchAngle(uintptr_t entity) {
		if (!entity) {
			return QAngle_t(); // Invalid entity
		}
		static_assert(sizeof(QAngle_t) == sizeof(float) * 3);
		QAngle_t predictablePunch = *reinterpret_cast<QAngle_t*>(entity + cs2_dumper::schemas::client_dll::CCSPlayer_AimPunchServices::m_predictableBaseAngle);
		QAngle_t unpredictablePunch = *reinterpret_cast<QAngle_t*>(entity + cs2_dumper::schemas::client_dll::CCSPlayer_AimPunchServices::m_unpredictableBaseAngle);
		return predictablePunch + unpredictablePunch;
	}
};
