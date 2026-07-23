#include <Windows.h>
#include <cstdint>
#include <core/math/math.h>
#include <cs2/output/offsets.hpp>
#include <cs2/output/client_dll.hpp>

#define MAX_ENTITY_SCAN 64
#define List_Entry_Offset 0x10
#define Controler_Offset 0x78

class EntityScanner {
	uint64_t clientDllBase;
	uint64_t entityListPrt;
	uint64_t listEntry;

public:
	EntityScanner() {
		clientDllBase = reinterpret_cast<uint64_t>(GetModuleHandleA("client.dll"));
		entityListPrt = clientDllBase + cs2_dumper::offsets::client_dll::dwEntityList;
		listEntry = entityListPrt + List_Entry_Offset;
	}

	uint64_t getEntity(int index) {
		if (index < 0 || index >= MAX_ENTITY_SCAN) {
			return 0; // Invalid index
		}
		auto entityController = *reinterpret_cast<uint64_t*>(listEntry + index * Controler_Offset);
		
		if (!entityController) {
			return 0; // Invalid controller
		}

		auto entityPawnHandle = *reinterpret_cast<uint64_t*>(entityController + cs2_dumper::schemas::client_dll::CCSPlayerController::m_hPlayerPawn);

		if (!entityPawnHandle) {
			return 0; // Invalid pawn handle
		}
		
		auto listEntry2 = *reinterpret_cast<uint64_t*>(entityListPrt + 0x8 * ((entityPawnHandle & 0x7FFF) >> 9) + 0x10);
	
		auto entity = *reinterpret_cast<uint64_t*>(listEntry2 + 0x78 * (entityPawnHandle & 0x1FF));

		return entity;
	}

	int8_t getEntityTeam(uint64_t entity) {
		if (!entity) {
			return -1; // Invalid entity
		}
		return *reinterpret_cast<int8_t*>(entity + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum);
	}

	int8_t isEntityAlive(uint64_t entity) {
		if (!entity) {
			return -1; // Invalid entity
		}
		return *reinterpret_cast<int8_t*>(entity + cs2_dumper::schemas::client_dll::C_BaseEntity::m_lifeState);
	}


	static Vec3 getEntityPosition(uint64_t entity) {
		if (!entity) {
			return Vec3(); // Invalid entity
		}
		static_assert(sizeof(Vec3) == sizeof(float) * 3);
		return *reinterpret_cast<Vec3*>(entity + cs2_dumper::schemas::client_dll::C_BasePlayerPawn::m_vOldOrigin);
	}
	


	uint64_t getClosestEntity() {
		uint64_t closestEntity = 0;
		float closestDistance = FLT_MAX;
		for (int i = 0; i < MAX_ENTITY_SCAN; ++i) {
			uint64_t entity = getEntity(i);
			if (entity == 0) {
				continue; // Skip invalid entities
			}
			// Assuming you have a way to get the distance to the entity
			float distance = getDistanceToEntity(entity);
			if (distance < closestDistance) {
				closestDistance = distance;
				closestEntity = entity;
			}
		}
		return closestEntity;
	}
};
