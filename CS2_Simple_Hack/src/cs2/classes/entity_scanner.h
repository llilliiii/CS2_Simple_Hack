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
#define Controler_Offset 0x70

class EntityScanner {
	uintptr_t clientDllBase;
	uintptr_t entityList;
	uintptr_t listEntry1;

public:
	// Initialise Entity Scanner
	EntityScanner() {
		clientDllBase = reinterpret_cast<uintptr_t>(GetModuleHandleA("client.dll"));
		entityList = *reinterpret_cast<uintptr_t*>(clientDllBase + cs2_dumper::offsets::client_dll::dwEntityList);
		listEntry1 = *reinterpret_cast<uintptr_t*>(entityList + List_Entry_Offset);
	}

	// Get the Entity pointer
	uintptr_t getEntity(int index) {
		if (index < 0 || index >= MAX_ENTITY_SCAN) {
			return 0; // Invalid index
		}

		// First find the controller
		auto entityController = *reinterpret_cast<uintptr_t*>(listEntry1 + index * Controler_Offset);
		if (!entityController) {
			return 0; // Invalid controller
		}

		// Second find the pawn handler in controller
		auto entityPawnHandle = *reinterpret_cast<uintptr_t*>(entityController + cs2_dumper::schemas::client_dll::CCSPlayerController::m_hPlayerPawn);
		if (!entityPawnHandle) {
			return 0; // Invalid pawn handle
		}

		// Find the second list entry
		auto listEntry2 = *reinterpret_cast<uintptr_t*>(entityList + 0x8 * ((entityPawnHandle & 0x7FFF) >> 9) + List_Entry_Offset);
		if (!listEntry2) {
			return 0; // Invalid list entry2
		}

		// Find the entry
		auto entity = *reinterpret_cast<uintptr_t*>(listEntry2 + Controler_Offset * (entityPawnHandle & 0x1FF));
		if (!entity) {
			return 0; // Invalid entity
		}

		return entity;
	}

	// Get the address of the local player
	uintptr_t getLocalPlayer() {
		return *(uintptr_t*)(clientDllBase + cs2_dumper::offsets::client_dll::dwLocalPlayerPawn);
	}

	// Get team number of the entity
	int8_t getEntityTeam(uintptr_t entity) {
		if (!entity) {
			return 0; // Invalid entity
		}
		return *reinterpret_cast<int8_t*>(entity + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum);
	}

	// Get life state of the entity
	int8_t getEntityLifeState(uintptr_t entity) {
		if (!entity) {
			return 0; // Invalid entity
		}
		return *reinterpret_cast<int8_t*>(entity + cs2_dumper::schemas::client_dll::C_BaseEntity::m_lifeState);
	}
	
	// Check whether or not the enetity is been spotted
	bool getEntitySpottedState(uintptr_t entity) {
		if (!entity) {
			return false; // Invalid entity
		}
		return *reinterpret_cast<bool*>(entity + cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_entitySpottedState + cs2_dumper::schemas::client_dll::EntitySpottedState_t::m_bSpotted);
	}

	// Get the velocity of the entity
	Vec3 getEntityVelocity(uintptr_t entity) {
		if (!entity) {
			return Vec3(); // Invalid entity
		}
		static_assert(sizeof(Vec3) == sizeof(float) * 3);
		return *reinterpret_cast<Vec3*>(entity + cs2_dumper::schemas::client_dll::C_BaseEntity::m_vecAbsVelocity);
	}

	// Get the position of the entity
	static Vec3 getEntityPosition(uintptr_t entity) {
		if (!entity) {
			return Vec3(); // Invalid entity
		}
		static_assert(sizeof(Vec3) == sizeof(float) * 3);
		return *reinterpret_cast<Vec3*>(entity + cs2_dumper::schemas::client_dll::C_BasePlayerPawn::m_vOldOrigin);
	}
	
	// Get the view offset of the entity
	static Vec3 getEntityViewOffset(uintptr_t entity) {
		if (!entity) {
			return Vec3(); // Invalid entity
		}
		static_assert(sizeof(Vec3) == sizeof(float) * 3);
		return *reinterpret_cast<Vec3*>(entity + cs2_dumper::schemas::client_dll::C_BaseModelEntity::m_vecViewOffset);
	}

	// Get the view angle of the local player
	QAngle_t getLocalViewAngles() {
		static_assert(sizeof(QAngle_t) == sizeof(float) * 3);
		return *reinterpret_cast<QAngle_t*>(clientDllBase + cs2_dumper::offsets::client_dll::dwViewAngles);
	}
};