#include <Windows.h>
#include <cstdint>
#include <core/math/math.h>
#include <cs2/output/offsets.hpp>
#include <cs2/output/client_dll.hpp>

#define MAX_ENTITY_SCAN 64
#define List_Entry_Offset 0x10
#define Controler_Offset 0x78

class EntityScanner {
	uintptr_t clientDllBase;
	uintptr_t entityListPrt;
	uintptr_t listEntry;

public:
	EntityScanner() {
		clientDllBase = reinterpret_cast<uintptr_t>(GetModuleHandleA("client.dll"));
		entityListPrt = clientDllBase + cs2_dumper::offsets::client_dll::dwEntityList;
		listEntry = entityListPrt + List_Entry_Offset;
	}

	uintptr_t getEntity(int index) {
		if (index < 0 || index >= MAX_ENTITY_SCAN) {
			return 0; // Invalid index
		}
		auto entityController = *reinterpret_cast<uintptr_t*>(listEntry + index * Controler_Offset);
		
		if (!entityController) {
			return 0; // Invalid controller
		}

		auto entityPawnHandle = *reinterpret_cast<uintptr_t*>(entityController + cs2_dumper::schemas::client_dll::CCSPlayerController::m_hPlayerPawn);

		if (!entityPawnHandle) {
			return 0; // Invalid pawn handle
		}
		
		auto listEntry2 = *reinterpret_cast<uintptr_t*>(entityListPrt + 0x8 * ((entityPawnHandle & 0x7FFF) >> 9) + List_Entry_Offset);
		
		if (!listEntry2) {
			return 0; // Invalid list entry2
		}

		auto entity = *reinterpret_cast<uintptr_t*>(listEntry2 + 0x78 * (entityPawnHandle & 0x1FF));

		return entity;
	}

	int8_t getEntityTeam(uintptr_t entity) {
		if (!entity) {
			return -1; // Invalid entity
		}
		return *reinterpret_cast<int8_t*>(entity + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum);
	}

	int8_t getEntityLifeState(uintptr_t entity) {
		if (!entity) {
			return -1; // Invalid entity
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
