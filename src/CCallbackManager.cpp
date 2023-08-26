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
	(char *)"FCNPC_OnInit",
	(char *)"FCNPC_OnCreate",
	(char *)"FCNPC_OnDestroy",
	(char *)"FCNPC_OnSpawn",
	(char *)"FCNPC_OnRespawn",
	(char *)"FCNPC_OnDeath",
	(char *)"FCNPC_OnReachDestination",
	(char *)"FCNPC_OnVehicleEntryComplete",
	(char *)"FCNPC_OnVehicleExitComplete",
	(char *)"FCNPC_OnTakeDamage",
	(char *)"FCNPC_OnGiveDamage",
	(char *)"FCNPC_OnWeaponShot",
	(char *)"FCNPC_OnWeaponStateChange",
	(char *)"FCNPC_OnVehicleTakeDamage",
	(char *)"FCNPC_OnFinishPlayback",
	(char *)"FCNPC_OnChangeNode",
	(char *)"FCNPC_OnFinishNodePoint",
	(char *)"FCNPC_OnFinishNode",
	(char *)"FCNPC_OnStreamIn",
	(char *)"FCNPC_OnStreamOut",
	(char *)"FCNPC_OnUpdate",
	(char *)"FCNPC_OnFinishMovePath",
	(char *)"FCNPC_OnFinishMovePathPoint",
	(char *)"FCNPC_OnChangeHeightPos",
};

void CCallbackManager::Init()
{
	while (!m_vAmxLoadQueue.empty()) {
		int iIndex;
		AMX *pAmx = m_vAmxLoadQueue.front();
		std::vector<int> vIndexes;

		for (int i = 0; i < Callbacks::CallbacksCount; i++) {
			if (amx_FindPublic(pAmx, m_aCallbackNames[i], &iIndex) != AMX_ERR_NONE || iIndex < 0) {
				iIndex = 0;
			}
			vIndexes.push_back(iIndex);
		}

		if (vIndexes.size() > 0) {
			m_mapCallbacks[pAmx] = vIndexes;

			int iIndex = vIndexes.at(FCNPC_OnInit);
			if (iIndex != 0) {
				amx_Exec(pAmx, NULL, iIndex);
			}
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

	for (const auto &c : m_mapCallbacks) {
		if (c.second.size() > 0) {
			iIndex = c.second.at(FCNPC_OnCreate);
		}
		if (iIndex != 0 || !amx_FindPublic(c.first, "FCNPC_OnCreate", &iIndex)) {
			amx_Push(c.first, wPlayerId);

			amx_Exec(c.first, NULL, iIndex);
		}
	}
}

void CCallbackManager::OnDestroy(WORD wPlayerId)
{
	int iIndex = 0;

	for (const auto &c : m_mapCallbacks) {
		if (c.second.size() > 0) {
			iIndex = c.second.at(FCNPC_OnDestroy);
		}
		if (iIndex != 0 || !amx_FindPublic(c.first, "FCNPC_OnDestroy", &iIndex)) {
			amx_Push(c.first, wPlayerId);

			amx_Exec(c.first, NULL, iIndex);
		}
	}
}

void CCallbackManager::OnSpawn(WORD wPlayerId)
{
	int iIndex = 0;

	for (const auto &c : m_mapCallbacks) {
		if (c.second.size() > 0) {
			iIndex = c.second.at(FCNPC_OnSpawn);
		}
		if (iIndex != 0 || !amx_FindPublic(c.first, "FCNPC_OnSpawn", &iIndex)) {
			amx_Push(c.first, wPlayerId);

			amx_Exec(c.first, NULL, iIndex);
		}
	}
}

void CCallbackManager::OnRespawn(WORD wPlayerId)
{
	int iIndex = 0;

	for (const auto &c : m_mapCallbacks) {
		if (c.second.size() > 0) {
			iIndex = c.second.at(FCNPC_OnRespawn);
		}
		if (iIndex != 0 || !amx_FindPublic(c.first, "FCNPC_OnRespawn", &iIndex)) {
			amx_Push(c.first, wPlayerId);

			amx_Exec(c.first, NULL, iIndex);
		}
	}
}

void CCallbackManager::OnDeath(WORD wPlayerId, WORD wKillerId, BYTE byteWeaponId)
{
	int iIndex = 0;

	for (const auto &c : m_mapCallbacks) {
		if (c.second.size() > 0) {
			iIndex = c.second.at(FCNPC_OnDeath);
		}
		if (iIndex != 0 || !amx_FindPublic(c.first, "FCNPC_OnDeath", &iIndex)) {
			amx_Push(c.first, byteWeaponId);
			amx_Push(c.first, wKillerId);
			amx_Push(c.first, wPlayerId);

			amx_Exec(c.first, NULL, iIndex);
		}
	}
}

void CCallbackManager::OnReachDestination(WORD wPlayerId)
{
	int iIndex = 0;

	for (const auto &c : m_mapCallbacks) {
		if (c.second.size() > 0) {
			iIndex = c.second.at(FCNPC_OnReachDestination);
		}
		if (iIndex != 0 || !amx_FindPublic(c.first, "FCNPC_OnReachDestination", &iIndex)) {
			amx_Push(c.first, wPlayerId);

			amx_Exec(c.first, NULL, iIndex);
		}
	}
}

void CCallbackManager::OnVehicleEntryComplete(WORD wPlayerId, WORD wVehicleId, int iSeat)
{
	int iIndex = 0;

	for (const auto &c : m_mapCallbacks) {
		if (c.second.size() > 0) {
			iIndex = c.second.at(FCNPC_OnVehicleEntryComplete);
		}
		if (iIndex != 0 || !amx_FindPublic(c.first, "FCNPC_OnVehicleEntryComplete", &iIndex)) {
			amx_Push(c.first, iSeat);
			amx_Push(c.first, wVehicleId);
			amx_Push(c.first, wPlayerId);

			amx_Exec(c.first, NULL, iIndex);
		}
	}
}

void CCallbackManager::OnVehicleExitComplete(WORD wPlayerId, WORD wVehicleId)
{
	int iIndex = 0;

	for (const auto &c : m_mapCallbacks) {
		if (c.second.size() > 0) {
			iIndex = c.second.at(FCNPC_OnVehicleExitComplete);
		}
		if (iIndex != 0 || !amx_FindPublic(c.first, "FCNPC_OnVehicleExitComplete", &iIndex)) {
			amx_Push(c.first, wVehicleId);
			amx_Push(c.first, wPlayerId);

			amx_Exec(c.first, NULL, iIndex);
		}
	}
}

int CCallbackManager::OnTakeDamage(WORD wPlayerId, WORD wDamagerId, BYTE byteWeaponId, int iBodyPart, float fHealthLoss)
{
	cell cReturn = 1;
	int iIndex = 0;

	for (const auto &c : m_mapCallbacks) {
		if (c.second.size() > 0) {
			iIndex = c.second.at(FCNPC_OnTakeDamage);
		}
		if (iIndex != 0 || !amx_FindPublic(c.first, "FCNPC_OnTakeDamage", &iIndex)) {
			amx_Push(c.first, iBodyPart);
			amx_Push(c.first, byteWeaponId);
			amx_Push(c.first, amx_ftoc(fHealthLoss));
			amx_Push(c.first, wDamagerId);
			amx_Push(c.first, wPlayerId);

			amx_Exec(c.first, &cReturn, iIndex);
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

	for (const auto &c : m_mapCallbacks) {
		if (c.second.size() > 0) {
			iIndex = c.second.at(FCNPC_OnGiveDamage);
		}
		if (iIndex != 0 || !amx_FindPublic(c.first, "FCNPC_OnGiveDamage", &iIndex)) {
			amx_Push(c.first, iBodyPart);
			amx_Push(c.first, byteWeaponId);
			amx_Push(c.first, amx_ftoc(fHealthLoss));
			amx_Push(c.first, wDamagedId);
			amx_Push(c.first, wPlayerId);

			amx_Exec(c.first, NULL, iIndex);
		}
	}
}

int CCallbackManager::OnWeaponShot(WORD wPlayerId, BYTE byteWeaponId, BYTE byteHitType, WORD wHitId, CVector vecPoint)
{
	cell cReturn = 1;
	int iIndex = 0;

	for (const auto &c : m_mapCallbacks) {
		if (c.second.size() > 0) {
			iIndex = c.second.at(FCNPC_OnWeaponShot);
		}
		if (iIndex != 0 || !amx_FindPublic(c.first, "FCNPC_OnWeaponShot", &iIndex)) {
			amx_Push(c.first, amx_ftoc(vecPoint.fZ));
			amx_Push(c.first, amx_ftoc(vecPoint.fY));
			amx_Push(c.first, amx_ftoc(vecPoint.fX));
			amx_Push(c.first, wHitId);
			amx_Push(c.first, byteHitType);
			amx_Push(c.first, byteWeaponId);
			amx_Push(c.first, wPlayerId);

			amx_Exec(c.first, &cReturn, iIndex);
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

	for (const auto &c : m_mapCallbacks) {
		if (c.second.size() > 0) {
			iIndex = c.second.at(FCNPC_OnWeaponStateChange);
		}
		if (iIndex != 0 || !amx_FindPublic(c.first, "FCNPC_OnWeaponStateChange", &iIndex)) {
			amx_Push(c.first, iWeaponState);
			amx_Push(c.first, wPlayerId);

			amx_Exec(c.first, NULL, iIndex);
		}
	}
}

int CCallbackManager::OnVehicleTakeDamage(WORD wPlayerId, WORD wDamagerId, WORD wVehicleId, BYTE byteWeaponId, CVector vecHit, float fHealthLoss)
{
	cell cReturn = 1;
	int iIndex = 0;

	for (const auto &c : m_mapCallbacks) {
		if (c.second.size() > 0) {
			iIndex = c.second.at(FCNPC_OnVehicleTakeDamage);
		}
		if (iIndex != 0 || !amx_FindPublic(c.first, "FCNPC_OnVehicleTakeDamage", &iIndex)) {
			amx_Push(c.first, amx_ftoc(vecHit.fZ));
			amx_Push(c.first, amx_ftoc(vecHit.fY));
			amx_Push(c.first, amx_ftoc(vecHit.fX));
			amx_Push(c.first, byteWeaponId);
			amx_Push(c.first, amx_ftoc(fHealthLoss));
			amx_Push(c.first, wVehicleId);
			amx_Push(c.first, wDamagerId);
			amx_Push(c.first, wPlayerId);

			amx_Exec(c.first, &cReturn, iIndex);
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

	for (const auto &c : m_mapCallbacks) {
		if (c.second.size() > 0) {
			iIndex = c.second.at(FCNPC_OnFinishPlayback);
		}
		if (iIndex != 0 || !amx_FindPublic(c.first, "FCNPC_OnFinishPlayback", &iIndex)) {
			amx_Push(c.first, wPlayerId);

			amx_Exec(c.first, NULL, iIndex);
		}
	}
}

int CCallbackManager::OnChangeNode(WORD wPlayerId, WORD wNodeId, WORD wOldNodeId)
{
	cell cReturn = 1;
	int iIndex = 0;

	for (const auto &c : m_mapCallbacks) {
		if (c.second.size() > 0) {
			iIndex = c.second.at(FCNPC_OnChangeNode);
		}
		if (iIndex != 0 || !amx_FindPublic(c.first, "FCNPC_OnChangeNode", &iIndex)) {
			amx_Push(c.first, wOldNodeId);
			amx_Push(c.first, wNodeId);
			amx_Push(c.first, wPlayerId);

			amx_Exec(c.first, &cReturn, iIndex);
			if (!cReturn) {
				return cReturn;
			}
		}
	}

	return cReturn;
}

int CCallbackManager::OnFinishNodePoint(WORD wPlayerId, WORD wNodeId, WORD wNodePoint)
{
	cell cReturn = 1;
	int iIndex = 0;

	for (const auto &c : m_mapCallbacks) {
		if (c.second.size() > 0) {
			iIndex = c.second.at(FCNPC_OnFinishNodePoint);
		}
		if (iIndex != 0 || !amx_FindPublic(c.first, "FCNPC_OnFinishNodePoint", &iIndex)) {
			amx_Push(c.first, wNodePoint);
			amx_Push(c.first, wNodeId);
			amx_Push(c.first, wPlayerId);

			amx_Exec(c.first, &cReturn, iIndex);
			if (!cReturn) {
				return cReturn;
			}
		}
	}

	return cReturn;
}

void CCallbackManager::OnFinishNode(WORD wPlayerId, WORD wNodeId)
{
	int iIndex = 0;

	for (const auto &c : m_mapCallbacks) {
		if (c.second.size() > 0) {
			iIndex = c.second.at(FCNPC_OnFinishNode);
		}
		if (iIndex != 0 || !amx_FindPublic(c.first, "FCNPC_OnFinishNode", &iIndex)) {
			amx_Push(c.first, wNodeId);
			amx_Push(c.first, wPlayerId);

			amx_Exec(c.first, NULL, iIndex);
		}
	}
}

void CCallbackManager::OnStreamIn(WORD wPlayerId, WORD wForPlayerId)
{
	int iIndex = 0;

	for (const auto &c : m_mapCallbacks) {
		if (c.second.size() > 0) {
			iIndex = c.second.at(FCNPC_OnStreamIn);
		}
		if (iIndex != 0 || !amx_FindPublic(c.first, "FCNPC_OnStreamIn", &iIndex)) {
			amx_Push(c.first, wForPlayerId);
			amx_Push(c.first, wPlayerId);

			amx_Exec(c.first, NULL, iIndex);
		}
	}
}

void CCallbackManager::OnStreamOut(WORD wPlayerId, WORD wForPlayerId)
{
	int iIndex = 0;

	for (const auto &c : m_mapCallbacks) {
		if (c.second.size() > 0) {
			iIndex = c.second.at(FCNPC_OnStreamOut);
		}
		if (iIndex != 0 || !amx_FindPublic(c.first, "FCNPC_OnStreamOut", &iIndex)) {
			amx_Push(c.first, wForPlayerId);
			amx_Push(c.first, wPlayerId);

			amx_Exec(c.first, NULL, iIndex);
		}
	}
}

int CCallbackManager::OnUpdate(WORD wPlayerId)
{
	cell cReturn = 1;
	int iIndex = 0;

	for (const auto &c : m_mapCallbacks) {
		if (c.second.size() > 0) {
			iIndex = c.second.at(FCNPC_OnUpdate);
		}
		if (iIndex != 0 || !amx_FindPublic(c.first, "FCNPC_OnUpdate", &iIndex)) {
			amx_Push(c.first, wPlayerId);

			if (cReturn) {
				amx_Exec(c.first, &cReturn, iIndex);
			} else {
				amx_Exec(c.first, NULL, iIndex);
			}
		}
	}

	return cReturn;
}

void CCallbackManager::OnFinishMovePath(WORD wPlayerId, int iMovePath)
{
	int iIndex = 0;

	for (const auto &c : m_mapCallbacks) {
		if (c.second.size() > 0) {
			iIndex = c.second.at(FCNPC_OnFinishMovePath);
		}
		if (iIndex != 0 || !amx_FindPublic(c.first, "FCNPC_OnFinishMovePath", &iIndex)) {
			amx_Push(c.first, iMovePath);
			amx_Push(c.first, wPlayerId);

			amx_Exec(c.first, NULL, iIndex);
		}
	}
}

int CCallbackManager::OnFinishMovePathPoint(WORD wPlayerId, int iMovePath, int iMovePoint)
{
	cell cReturn = 1;
	int iIndex = 0;

	for (const auto &c : m_mapCallbacks) {
		if (c.second.size() > 0) {
			iIndex = c.second.at(FCNPC_OnFinishMovePathPoint);
		}
		if (iIndex != 0 || !amx_FindPublic(c.first, "FCNPC_OnFinishMovePathPoint", &iIndex)) {
			amx_Push(c.first, iMovePoint);
			amx_Push(c.first, iMovePath);
			amx_Push(c.first, wPlayerId);

			amx_Exec(c.first, &cReturn, iIndex);
			if (!cReturn) {
				return cReturn;
			}
		}
	}

	return cReturn;
}

int CCallbackManager::OnChangeHeightPos(WORD wPlayerId, float fNewZ, float fOldZ)
{
	cell cReturn = 1;
	int iIndex = 0;

	for (const auto &c : m_mapCallbacks) {
		if (c.second.size() > 0) {
			iIndex = c.second.at(FCNPC_OnChangeHeightPos);
		}
		if (iIndex != 0 || !amx_FindPublic(c.first, "FCNPC_OnChangeHeightPos", &iIndex)) {
			amx_Push(c.first, amx_ftoc(fOldZ));
			amx_Push(c.first, amx_ftoc(fNewZ));
			amx_Push(c.first, wPlayerId);

			amx_Exec(c.first, &cReturn, iIndex);
			if (!cReturn) {
				return cReturn;
			}
		}
	}

	return cReturn;
}
