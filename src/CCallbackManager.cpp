/* =========================================

		FCNPC - Fully Controllable NPC
			----------------------

	- File: CallbackManager.cpp
	- Author(s): OrMisicL

  =========================================*/


#include "Main.hpp"

std::vector<AMX *> CCallbackManager::m_vAmx;
std::map<AMX *, std::vector<int>> CCallbackManager::m_mapCallbacks;
std::queue<AMX *> CCallbackManager::m_vAmxLoadQueue;
std::array<char *, CCallbackManager::Callbacks::CallbacksCount> CCallbackManager::m_aCallbackNames = {
	"FCNPC_OnCreate",
	"FCNPC_OnDestroy",
	"FCNPC_OnSpawn",
	"FCNPC_OnRespawn",
	"FCNPC_OnDeath",
	"FCNPC_OnReachDestination",
	"FCNPC_OnVehicleEntryComplete",
	"FCNPC_OnVehicleExitComplete",
	"FCNPC_OnTakeDamage",
	"FCNPC_OnGiveDamage",
	"FCNPC_OnWeaponShot",
	"FCNPC_OnWeaponStateChange",
	"FCNPC_OnVehicleTakeDamage",
	"FCNPC_OnFinishPlayback",
	"FCNPC_OnChangeNode",
	"FCNPC_OnFinishNodePoint",
	"FCNPC_OnFinishNode",
	"FCNPC_OnStreamIn",
	"FCNPC_OnStreamOut",
	"FCNPC_OnUpdate",
	"FCNPC_OnFinishMovePath",
	"FCNPC_OnFinishMovePathPoint",
	"FCNPC_OnChangeHeightPos",
};

void CCallbackManager::Init()
{
	while (!m_vAmxLoadQueue.empty()) {
		int iIndex;
		AMX *pAmx = m_vAmxLoadQueue.front();

		for (int i = 0; i < Callbacks::CallbacksCount; i++) {
			if (amx_FindPublic(pAmx, m_aCallbackNames[i], &iIndex) != AMX_ERR_NONE || iIndex < 0) {
				iIndex = 0;
			}
			m_mapCallbacks.insert(std::make_pair(pAmx, iIndex));
		}

		m_vAmxLoadQueue.pop();
	}
}

void CCallbackManager::RegisterAMX(AMX *pAmx)
{
	m_vAmx.push_back(pAmx);
	m_vAmxLoadQueue.push(pAmx);
}

void CCallbackManager::UnregisterAMX(AMX *pAmx)
{
	m_vAmx.erase(std::remove(m_vAmx.begin(), m_vAmx.end(), pAmx), m_vAmx.end());
	m_mapCallbacks.erase(m_mapCallbacks.find(pAmx), m_mapCallbacks.end());
}

void CCallbackManager::OnCreate(WORD wPlayerId)
{
	int iIndex = 0;

	for (auto &amx : m_vAmx) {
		if (m_mapCallbacks[amx].size() > 0) {
			iIndex = m_mapCallbacks[amx].at(FCNPC_OnCreate);
		}
		if (iIndex != 0 || !amx_FindPublic(amx, "FCNPC_OnCreate", &iIndex)) {
			amx_Push(amx, wPlayerId);

			amx_Exec(amx, NULL, iIndex);
		}
	}
}

void CCallbackManager::OnDestroy(WORD wPlayerId)
{
	int iIndex = 0;

	for (auto &amx : m_vAmx) {
		if (m_mapCallbacks[amx].size() > 0) {
			iIndex = m_mapCallbacks[amx].at(FCNPC_OnDestroy);
		}
		if (iIndex != 0 || !amx_FindPublic(amx, "FCNPC_OnDestroy", &iIndex)) {
			amx_Push(amx, wPlayerId);

			amx_Exec(amx, NULL, iIndex);
		}
	}
}

void CCallbackManager::OnSpawn(WORD wPlayerId)
{
	int iIndex = 0;

	for (auto &amx : m_vAmx) {
		if (m_mapCallbacks[amx].size() > 0) {
			iIndex = m_mapCallbacks[amx].at(FCNPC_OnSpawn);
		}
		if (iIndex != 0 || !amx_FindPublic(amx, "FCNPC_OnSpawn", &iIndex)) {
			amx_Push(amx, wPlayerId);

			amx_Exec(amx, NULL, iIndex);
		}
	}
}

void CCallbackManager::OnRespawn(WORD wPlayerId)
{
	int iIndex = 0;

	for (auto &amx : m_vAmx) {
		if (m_mapCallbacks[amx].size() > 0) {
			iIndex = m_mapCallbacks[amx].at(FCNPC_OnRespawn);
		}
		if (iIndex != 0 || !amx_FindPublic(amx, "FCNPC_OnRespawn", &iIndex)) {
			amx_Push(amx, wPlayerId);

			amx_Exec(amx, NULL, iIndex);
		}
	}
}

void CCallbackManager::OnDeath(WORD wPlayerId, WORD wKillerId, BYTE byteWeaponId)
{
	int iIndex = 0;

	for (auto &amx : m_vAmx) {
		if (m_mapCallbacks[amx].size() > 0) {
			iIndex = m_mapCallbacks[amx].at(FCNPC_OnDeath);
		}
		if (iIndex != 0 || !amx_FindPublic(amx, "FCNPC_OnDeath", &iIndex)) {
			amx_Push(amx, byteWeaponId);
			amx_Push(amx, wKillerId);
			amx_Push(amx, wPlayerId);

			amx_Exec(amx, NULL, iIndex);
		}
	}
}

void CCallbackManager::OnReachDestination(WORD wPlayerId)
{
	int iIndex = 0;

	for (auto &amx : m_vAmx) {
		if (m_mapCallbacks[amx].size() > 0) {
			iIndex = m_mapCallbacks[amx].at(FCNPC_OnReachDestination);
		}
		if (iIndex != 0 || !amx_FindPublic(amx, "FCNPC_OnReachDestination", &iIndex)) {
			amx_Push(amx, wPlayerId);

			amx_Exec(amx, NULL, iIndex);
		}
	}
}

void CCallbackManager::OnVehicleEntryComplete(WORD wPlayerId, WORD wVehicleId, int iSeat)
{
	int iIndex = 0;

	for (auto &amx : m_vAmx) {
		if (m_mapCallbacks[amx].size() > 0) {
			iIndex = m_mapCallbacks[amx].at(FCNPC_OnVehicleEntryComplete);
		}
		if (iIndex != 0 || !amx_FindPublic(amx, "FCNPC_OnVehicleEntryComplete", &iIndex)) {
			amx_Push(amx, iSeat);
			amx_Push(amx, wVehicleId);
			amx_Push(amx, wPlayerId);

			amx_Exec(amx, NULL, iIndex);
		}
	}
}

void CCallbackManager::OnVehicleExitComplete(WORD wPlayerId)
{
	int iIndex = 0;

	for (auto &amx : m_vAmx) {
		if (m_mapCallbacks[amx].size() > 0) {
			iIndex = m_mapCallbacks[amx].at(FCNPC_OnVehicleExitComplete);
		}
		if (iIndex != 0 || !amx_FindPublic(amx, "FCNPC_OnVehicleExitComplete", &iIndex)) {
			amx_Push(amx, wPlayerId);

			amx_Exec(amx, NULL, iIndex);
		}
	}
}

int CCallbackManager::OnTakeDamage(WORD wPlayerId, WORD wDamagerId, BYTE byteWeaponId, int iBodyPart, float fHealthLoss)
{
	cell cReturn = 1;
	int iIndex = 0;

	for (auto &amx : m_vAmx) {
		if (m_mapCallbacks[amx].size() > 0) {
			iIndex = m_mapCallbacks[amx].at(FCNPC_OnTakeDamage);
		}
		if (iIndex != 0 || !amx_FindPublic(amx, "FCNPC_OnTakeDamage", &iIndex)) {
			amx_Push(amx, amx_ftoc(fHealthLoss));
			amx_Push(amx, iBodyPart);
			amx_Push(amx, byteWeaponId);
			amx_Push(amx, wDamagerId);
			amx_Push(amx, wPlayerId);

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
	int iIndex = 0;

	for (auto &amx : m_vAmx) {
		if (m_mapCallbacks[amx].size() > 0) {
			iIndex = m_mapCallbacks[amx].at(FCNPC_OnGiveDamage);
		}
		if (iIndex != 0 || !amx_FindPublic(amx, "FCNPC_OnGiveDamage", &iIndex)) {
			amx_Push(amx, amx_ftoc(fHealthLoss));
			amx_Push(amx, iBodyPart);
			amx_Push(amx, byteWeaponId);
			amx_Push(amx, wDamagedId);
			amx_Push(amx, wPlayerId);

			amx_Exec(amx, NULL, iIndex);
		}
	}
}

int CCallbackManager::OnWeaponShot(WORD wPlayerId, WORD wHitId, BYTE byteHitType, BYTE byteWeaponId, CVector vecPoint)
{
	cell cReturn = 1;
	int iIndex = 0;

	for (auto &amx : m_vAmx) {
		if (m_mapCallbacks[amx].size() > 0) {
			iIndex = m_mapCallbacks[amx].at(FCNPC_OnWeaponShot);
		}
		if (iIndex != 0 || !amx_FindPublic(amx, "FCNPC_OnWeaponShot", &iIndex)) {
			amx_Push(amx, amx_ftoc(vecPoint.fZ));
			amx_Push(amx, amx_ftoc(vecPoint.fY));
			amx_Push(amx, amx_ftoc(vecPoint.fX));
			amx_Push(amx, byteWeaponId);
			amx_Push(amx, byteHitType);
			amx_Push(amx, wHitId);
			amx_Push(amx, wPlayerId);

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
	int iIndex = 0;

	for (auto &amx : m_vAmx) {
		if (m_mapCallbacks[amx].size() > 0) {
			iIndex = m_mapCallbacks[amx].at(FCNPC_OnWeaponStateChange);
		}
		if (iIndex != 0 || !amx_FindPublic(amx, "FCNPC_OnWeaponStateChange", &iIndex)) {
			amx_Push(amx, iWeaponState);
			amx_Push(amx, wPlayerId);

			amx_Exec(amx, NULL, iIndex);
		}
	}
}

int CCallbackManager::OnVehicleTakeDamage(WORD wPlayerId, WORD wDamagerId, WORD wVehicleId, BYTE byteWeaponId, CVector vecHit)
{
	cell cReturn = 1;
	int iIndex = 0;

	for (auto &amx : m_vAmx) {
		if (m_mapCallbacks[amx].size() > 0) {
			iIndex = m_mapCallbacks[amx].at(FCNPC_OnVehicleTakeDamage);
		}
		if (iIndex != 0 || !amx_FindPublic(amx, "FCNPC_OnVehicleTakeDamage", &iIndex)) {
			amx_Push(amx, amx_ftoc(vecHit.fZ));
			amx_Push(amx, amx_ftoc(vecHit.fY));
			amx_Push(amx, amx_ftoc(vecHit.fX));
			amx_Push(amx, byteWeaponId);
			amx_Push(amx, wVehicleId);
			amx_Push(amx, wDamagerId);
			amx_Push(amx, wPlayerId);

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
	int iIndex = 0;

	for (auto &amx : m_vAmx) {
		if (m_mapCallbacks[amx].size() > 0) {
			iIndex = m_mapCallbacks[amx].at(FCNPC_OnFinishPlayback);
		}
		if (iIndex != 0 || !amx_FindPublic(amx, "FCNPC_OnFinishPlayback", &iIndex)) {
			amx_Push(amx, wPlayerId);

			amx_Exec(amx, NULL, iIndex);
		}
	}
}

int CCallbackManager::OnChangeNode(WORD wPlayerId, WORD wNodeId)
{
	cell cReturn = 1;
	int iIndex = 0;

	for (auto &amx : m_vAmx) {
		if (m_mapCallbacks[amx].size() > 0) {
			iIndex = m_mapCallbacks[amx].at(FCNPC_OnChangeNode);
		}
		if (iIndex != 0 || !amx_FindPublic(amx, "FCNPC_OnChangeNode", &iIndex)) {
			amx_Push(amx, wNodeId);
			amx_Push(amx, wPlayerId);

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
	int iIndex = 0;

	for (auto &amx : m_vAmx) {
		if (m_mapCallbacks[amx].size() > 0) {
			iIndex = m_mapCallbacks[amx].at(FCNPC_OnFinishNodePoint);
		}
		if (iIndex != 0 || !amx_FindPublic(amx, "FCNPC_OnFinishNodePoint", &iIndex)) {
			amx_Push(amx, wNodePoint);
			amx_Push(amx, wPlayerId);

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
	int iIndex = 0;

	for (auto &amx : m_vAmx) {
		if (m_mapCallbacks[amx].size() > 0) {
			iIndex = m_mapCallbacks[amx].at(FCNPC_OnFinishNode);
		}
		if (iIndex != 0 || !amx_FindPublic(amx, "FCNPC_OnFinishNode", &iIndex)) {
			amx_Push(amx, wPlayerId);

			amx_Exec(amx, NULL, iIndex);
		}
	}
}

void CCallbackManager::OnStreamIn(WORD wPlayerId, WORD wForPlayerId)
{
	int iIndex = 0;

	for (auto &amx : m_vAmx) {
		if (m_mapCallbacks[amx].size() > 0) {
			iIndex = m_mapCallbacks[amx].at(FCNPC_OnStreamIn);
		}
		if (iIndex != 0 || !amx_FindPublic(amx, "FCNPC_OnStreamIn", &iIndex)) {
			amx_Push(amx, wForPlayerId);
			amx_Push(amx, wPlayerId);

			amx_Exec(amx, NULL, iIndex);
		}
	}
}

void CCallbackManager::OnStreamOut(WORD wPlayerId, WORD wForPlayerId)
{
	int iIndex = 0;

	for (auto &amx : m_vAmx) {
		if (m_mapCallbacks[amx].size() > 0) {
			iIndex = m_mapCallbacks[amx].at(FCNPC_OnStreamOut);
		}
		if (iIndex != 0 || !amx_FindPublic(amx, "FCNPC_OnStreamOut", &iIndex)) {
			amx_Push(amx, wForPlayerId);
			amx_Push(amx, wPlayerId);

			amx_Exec(amx, NULL, iIndex);
		}
	}
}

int CCallbackManager::OnUpdate(WORD wPlayerId)
{
	cell cReturn = 1;
	int iIndex = 0;

	for (auto &amx : m_vAmx) {
		if (m_mapCallbacks[amx].size() > 0) {
			iIndex = m_mapCallbacks[amx].at(FCNPC_OnUpdate);
		}
		if (iIndex != 0 || !amx_FindPublic(amx, "FCNPC_OnUpdate", &iIndex)) {
			amx_Push(amx, wPlayerId);

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
	int iIndex = 0;

	for (auto &amx : m_vAmx) {
		if (m_mapCallbacks[amx].size() > 0) {
			iIndex = m_mapCallbacks[amx].at(FCNPC_OnFinishMovePath);
		}
		if (iIndex != 0 || !amx_FindPublic(amx, "FCNPC_OnFinishMovePath", &iIndex)) {
			amx_Push(amx, iMovePath);
			amx_Push(amx, wPlayerId);

			amx_Exec(amx, NULL, iIndex);
		}
	}
}

void CCallbackManager::OnFinishMovePathPoint(WORD wPlayerId, int iMovePath, int iMovePoint)
{
	int iIndex = 0;

	for (auto &amx : m_vAmx) {
		if (m_mapCallbacks[amx].size() > 0) {
			iIndex = m_mapCallbacks[amx].at(FCNPC_OnFinishMovePathPoint);
		}
		if (iIndex != 0 || !amx_FindPublic(amx, "FCNPC_OnFinishMovePathPoint", &iIndex)) {
			amx_Push(amx, iMovePoint);
			amx_Push(amx, iMovePath);
			amx_Push(amx, wPlayerId);

			amx_Exec(amx, NULL, iIndex);
		}
	}
}

int CCallbackManager::OnChangeHeightPos(WORD wPlayerId, float fNewZ, float fOldZ)
{
	cell cReturn = 1;
	int iIndex = 0;

	for (auto &amx : m_vAmx) {
		if (m_mapCallbacks[amx].size() > 0) {
			iIndex = m_mapCallbacks[amx].at(FCNPC_OnChangeHeightPos);
		}
		if (iIndex != 0 || !amx_FindPublic(amx, "FCNPC_OnChangeHeightPos", &iIndex)) {
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
