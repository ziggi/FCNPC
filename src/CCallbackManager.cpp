/* =========================================

		FCNPC - Fully Controllable NPC
			----------------------

	- File: CallbackManager.cpp
	- Author(s): OrMisicL

  =========================================*/


#include "Main.hpp"

std::vector<AMX *> CCallbackManager::m_vAMX;

void CCallbackManager::RegisterAMX(AMX *pAMX)
{
	m_vAMX.push_back(pAMX);
}

void CCallbackManager::UnregisterAMX(AMX *pAMX)
{
	m_vAMX.erase(std::remove(m_vAMX.begin(), m_vAMX.end(), pAMX), m_vAMX.end());
}

void CCallbackManager::OnCreate(WORD wPlayerId)
{
	for (auto &amx : m_vAMX) {
		// Get the function index
		int iIndex;
		if (!amx_FindPublic(amx, "FCNPC_OnCreate", &iIndex)) {
			// Push the NPC ID
			amx_Push(amx, wPlayerId);
			// Execute the callback
			amx_Exec(amx, NULL, iIndex);
		}
	}
}

void CCallbackManager::OnDestroy(WORD wPlayerId)
{
	for (auto &amx : m_vAMX) {
		// Get the function index
		int iIndex;
		if (!amx_FindPublic(amx, "FCNPC_OnDestroy", &iIndex)) {
			amx_Push(amx, wPlayerId);
			amx_Exec(amx, NULL, iIndex);
		}
	}
}

void CCallbackManager::OnSpawn(WORD wPlayerId)
{
	for (auto &amx : m_vAMX) {
		// Get the function index
		int iIndex;
		if (!amx_FindPublic(amx, "FCNPC_OnSpawn", &iIndex)) {
			// Push the NPC ID
			amx_Push(amx, wPlayerId);
			// Execute the callback
			amx_Exec(amx, NULL, iIndex);
		}
	}
}

void CCallbackManager::OnRespawn(WORD wPlayerId)
{
	for (auto &amx : m_vAMX) {
		// Get the function index
		int iIndex;
		if (!amx_FindPublic(amx, "FCNPC_OnRespawn", &iIndex)) {
			// Push the NPC ID
			amx_Push(amx, wPlayerId);
			// Execute the callback
			amx_Exec(amx, NULL, iIndex);
		}
	}
}

void CCallbackManager::OnDeath(WORD wPlayerId, WORD wKillerId, BYTE byteWeaponId)
{
	for (auto &amx : m_vAMX) {
		// Get the function index
		int iIndex;
		if (!amx_FindPublic(amx, "FCNPC_OnDeath", &iIndex)) {
			// Push the parameters
			amx_Push(amx, byteWeaponId);
			amx_Push(amx, wKillerId);
			amx_Push(amx, wPlayerId);
			// Execute the callback
			amx_Exec(amx, NULL, iIndex);
		}
	}
}

void CCallbackManager::OnReachDestination(WORD wPlayerId)
{
	for (auto &amx : m_vAMX) {
		// Get the function index
		int iIndex;
		if (!amx_FindPublic(amx, "FCNPC_OnReachDestination", &iIndex)) {
			// Push the parameters
			amx_Push(amx, wPlayerId);
			// Execute the callback
			amx_Exec(amx, NULL, iIndex);
		}
	}
}

void CCallbackManager::OnVehicleEntryComplete(WORD wPlayerId, WORD wVehicleId, int iSeat)
{
	for (auto &amx : m_vAMX) {
		// Get the function index
		int iIndex;
		if (!amx_FindPublic(amx, "FCNPC_OnVehicleEntryComplete", &iIndex)) {
			// Push the parameters
			amx_Push(amx, iSeat);
			amx_Push(amx, wVehicleId);
			amx_Push(amx, wPlayerId);
			// Execute the callback
			amx_Exec(amx, NULL, iIndex);
		}
	}
}

void CCallbackManager::OnVehicleExitComplete(WORD wPlayerId)
{
	for (auto &amx : m_vAMX) {
		// Get the function index
		int iIndex;
		if (!amx_FindPublic(amx, "FCNPC_OnVehicleExitComplete", &iIndex)) {
			// Push the parameters
			amx_Push(amx, wPlayerId);
			// Execute the callback
			amx_Exec(amx, NULL, iIndex);
		}
	}
}

int CCallbackManager::OnTakeDamage(WORD wPlayerId, WORD wDamagerId, BYTE byteWeaponId, int iBodyPart, float fHealthLoss)
{
	cell cReturn = 1;
	for (auto &amx : m_vAMX) {
		// Get the function index
		int iIndex;
		if (!amx_FindPublic(amx, "FCNPC_OnTakeDamage", &iIndex)) {
			// Push the parameters
			amx_Push(amx, amx_ftoc(fHealthLoss));
			amx_Push(amx, iBodyPart);
			amx_Push(amx, byteWeaponId);
			amx_Push(amx, wDamagerId);
			amx_Push(amx, wPlayerId);
			// Execute the callback
			amx_Exec(amx, &cReturn, iIndex);
			if (!cReturn) {
				return cReturn;
			}
		}
	}
	return cReturn;
}

void CCallbackManager::OnGiveDamage(WORD wPlayerId, WORD wDamagedId, BYTE byteWeaponId, int iBodyPart, float fHealthLoss)
{
	for (auto &amx : m_vAMX) {
		// Get the function index
		int iIndex;
		if (!amx_FindPublic(amx, "FCNPC_OnGiveDamage", &iIndex)) {
			// Push the parameters
			amx_Push(amx, amx_ftoc(fHealthLoss));
			amx_Push(amx, iBodyPart);
			amx_Push(amx, byteWeaponId);
			amx_Push(amx, wDamagedId);
			amx_Push(amx, wPlayerId);
			// Execute the callback
			amx_Exec(amx, NULL, iIndex);
		}
	}
}

int CCallbackManager::OnWeaponShot(WORD wPlayerId, WORD wHitId, BYTE byteHitType, BYTE byteWeaponId, CVector vecPoint)
{
	cell cReturn = 1;
	for (auto &amx : m_vAMX) {
		// Get the function index
		int iIndex;
		if (!amx_FindPublic(amx, "FCNPC_OnWeaponShot", &iIndex)) {
			// Push the parameters
			amx_Push(amx, amx_ftoc(vecPoint.fZ));
			amx_Push(amx, amx_ftoc(vecPoint.fY));
			amx_Push(amx, amx_ftoc(vecPoint.fX));
			amx_Push(amx, byteWeaponId);
			amx_Push(amx, byteHitType);
			amx_Push(amx, wHitId);
			amx_Push(amx, wPlayerId);
			// Execute the callback
			amx_Exec(amx, &cReturn, iIndex);
			if (!cReturn) {
				return cReturn;
			}
		}
	}
	return cReturn;
}

void CCallbackManager::OnWeaponStateChange(WORD wPlayerId, int iWeaponState)
{
	int iIndex;
	for (auto &amx : m_vAMX) {
		// Get the function index
		if (!amx_FindPublic(amx, "FCNPC_OnWeaponStateChange", &iIndex)) {
			// Push the parameters
			amx_Push(amx, iWeaponState);
			amx_Push(amx, wPlayerId);
			// Execute the callback
			amx_Exec(amx, NULL, iIndex);
		}
	}
}

int CCallbackManager::OnVehicleTakeDamage(WORD wPlayerId, WORD wDamagerId, WORD wVehicleId, BYTE byteWeaponId, CVector vecHit)
{
	cell cReturn = 1;
	for (auto &amx : m_vAMX) {
		// Get the function index
		int iIndex;
		if (!amx_FindPublic(amx, "FCNPC_OnVehicleTakeDamage", &iIndex)) {
			// Push the parameters
			amx_Push(amx, amx_ftoc(vecHit.fZ));
			amx_Push(amx, amx_ftoc(vecHit.fY));
			amx_Push(amx, amx_ftoc(vecHit.fX));
			amx_Push(amx, byteWeaponId);
			amx_Push(amx, wVehicleId);
			amx_Push(amx, wDamagerId);
			amx_Push(amx, wPlayerId);
			// Execute the callback
			amx_Exec(amx, &cReturn, iIndex);
			if (!cReturn) {
				return cReturn;
			}
		}
	}
	return cReturn;
}

void CCallbackManager::OnFinishPlayback(WORD wPlayerId)
{
	for (auto &amx : m_vAMX) {
		// Get the function index
		int iIndex;
		if (!amx_FindPublic(amx, "FCNPC_OnFinishPlayback", &iIndex)) {
			// Push the parameters
			amx_Push(amx, wPlayerId);
			// Execute the callback
			amx_Exec(amx, NULL, iIndex);
		}
	}
}

int CCallbackManager::OnChangeNode(WORD wPlayerId, WORD wNodeId)
{
	cell cReturn = 1;
	for (auto &amx : m_vAMX) {
		// Get the function index
		int iIndex;
		if (!amx_FindPublic(amx, "FCNPC_OnChangeNode", &iIndex)) {
			// Push the parameters
			amx_Push(amx, wNodeId);
			amx_Push(amx, wPlayerId);
			// Execute the callback
			amx_Exec(amx, &cReturn, iIndex);
			if (!cReturn) {
				return cReturn;
			}
		}
	}
	return cReturn;
}

int CCallbackManager::OnFinishNodePoint(WORD wPlayerId, WORD wNodePoint)
{
	cell cReturn = 1;
	for (auto &amx : m_vAMX) {
		// Get the function index
		int iIndex;
		if (!amx_FindPublic(amx, "FCNPC_OnFinishNodePoint", &iIndex)) {
			// Push the parameters
			amx_Push(amx, wNodePoint);
			amx_Push(amx, wPlayerId);
			// Execute the callback
			amx_Exec(amx, &cReturn, iIndex);
			if (!cReturn) {
				return cReturn;
			}
		}
	}
	return cReturn;
}

void CCallbackManager::OnFinishNode(WORD wPlayerId)
{
	for (auto &amx : m_vAMX) {
		// Get the function index
		int iIndex;
		if (!amx_FindPublic(amx, "FCNPC_OnFinishNode", &iIndex)) {
			// Push the parameters
			amx_Push(amx, wPlayerId);
			// Execute the callback
			amx_Exec(amx, NULL, iIndex);
		}
	}
}

void CCallbackManager::OnStreamIn(WORD wPlayerId, WORD wForPlayerId)
{
	for (auto &amx : m_vAMX) {
		// Get the function index
		int iIndex;
		if (!amx_FindPublic(amx, "FCNPC_OnStreamIn", &iIndex)) {
			// Push the parameters
			amx_Push(amx, wForPlayerId);
			amx_Push(amx, wPlayerId);
			// Execute the callback
			amx_Exec(amx, NULL, iIndex);
		}
	}
}

void CCallbackManager::OnStreamOut(WORD wPlayerId, WORD wForPlayerId)
{
	for (auto &amx : m_vAMX) {
		// Get the function index
		int iIndex;
		if (!amx_FindPublic(amx, "FCNPC_OnStreamOut", &iIndex)) {
			// Push the parameters
			amx_Push(amx, wForPlayerId);
			amx_Push(amx, wPlayerId);
			// Execute the callback
			amx_Exec(amx, NULL, iIndex);
		}
	}
}

int CCallbackManager::OnUpdate(WORD wPlayerId)
{
	cell cReturn = 1;
	for (auto &amx : m_vAMX) {
		// Get the function index
		int iIndex;
		if (!amx_FindPublic(amx, "FCNPC_OnUpdate", &iIndex)) {
			// Push the parameters
			amx_Push(amx, wPlayerId);
			// Execute the callback
			if (cReturn) {
				amx_Exec(amx, &cReturn, iIndex);
			} else {
				amx_Exec(amx, NULL, iIndex);
			}
		}
	}
	return cReturn;
}

void CCallbackManager::OnFinishMovePath(WORD wPlayerId, int iMovePath)
{
	for (auto &amx : m_vAMX) {
		// Get the function index
		int iIndex;
		if (!amx_FindPublic(amx, "FCNPC_OnFinishMovePath", &iIndex)) {
			// Push the parameters
			amx_Push(amx, iMovePath);
			amx_Push(amx, wPlayerId);
			// Execute the callback
			amx_Exec(amx, NULL, iIndex);
		}
	}
}

void CCallbackManager::OnFinishMovePathPoint(WORD wPlayerId, int iMovePath, int iMovePoint)
{
	for (auto &amx : m_vAMX) {
		// Get the function index
		int iIndex;
		if (!amx_FindPublic(amx, "FCNPC_OnFinishMovePathPoint", &iIndex)) {
			// Push the parameters
			amx_Push(amx, iMovePoint);
			amx_Push(amx, iMovePath);
			amx_Push(amx, wPlayerId);
			// Execute the callback
			amx_Exec(amx, NULL, iIndex);
		}
	}
}

int CCallbackManager::OnChangeHeightPos(WORD wPlayerId, float fNewZ, float fOldZ)
{
	cell cReturn = 1;
	int iIndex;
	for (auto &amx : m_vAMX) {
		if (!amx_FindPublic(amx, "FCNPC_OnChangeHeightPos", &iIndex)) {
			amx_Push(amx, amx_ftoc(fOldZ));
			amx_Push(amx, amx_ftoc(fNewZ));
			amx_Push(amx, wPlayerId);

			amx_Exec(amx, &cReturn, iIndex);
			if (!cReturn) {
				return cReturn;
			}
		}
	}
	return cReturn;
}
