/* =========================================

		FCNPC - Fully Controllable NPC
			----------------------

	- File: CallbackManager.cpp
	- Author(s): OrMisicL

  =========================================*/


#include "Main.hpp"

std::list<AMX *> CCallbackManager::m_listAMX;

void CCallbackManager::RegisterAMX(AMX *pAMX)
{
	// Add the amx to the pointers list
	m_listAMX.push_back(pAMX);
}

void CCallbackManager::UnregisterAMX(AMX *pAMX)
{
	// Remove the amx from the pointers list
	m_listAMX.remove(pAMX);
}

void CCallbackManager::OnCreate(WORD wPlayerId)
{
	for (std::list<AMX *>::iterator i = m_listAMX.begin(); i != m_listAMX.end(); i++) {
		// Get the function index
		int iIndex;
		if (!amx_FindPublic((*i), "FCNPC_OnCreate", &iIndex)) {
			// Push the NPC ID
			amx_Push((*i), wPlayerId);
			// Execute the callback
			amx_Exec((*i), NULL, iIndex);
		}
	}
}

void CCallbackManager::OnSpawn(WORD wPlayerId)
{
	for (std::list<AMX *>::iterator i = m_listAMX.begin(); i != m_listAMX.end(); i++) {
		// Get the function index
		int iIndex;
		if (!amx_FindPublic((*i), "FCNPC_OnSpawn", &iIndex)) {
			// Push the NPC ID
			amx_Push((*i), wPlayerId);
			// Execute the callback
			amx_Exec((*i), NULL, iIndex);
		}
	}
}

void CCallbackManager::OnRespawn(WORD wPlayerId)
{
	for (std::list<AMX *>::iterator i = m_listAMX.begin(); i != m_listAMX.end(); i++) {
		// Get the function index
		int iIndex;
		if (!amx_FindPublic((*i), "FCNPC_OnRespawn", &iIndex)) {
			// Push the NPC ID
			amx_Push((*i), wPlayerId);
			// Execute the callback
			amx_Exec((*i), NULL, iIndex);
		}
	}
}

void CCallbackManager::OnDeath(WORD wPlayerId, WORD wKillerId, BYTE byteWeaponId)
{
	for (std::list<AMX *>::iterator i = m_listAMX.begin(); i != m_listAMX.end(); i++) {
		// Get the function index
		int iIndex;
		if (!amx_FindPublic((*i), "FCNPC_OnDeath", &iIndex)) {
			// Push the parameters
			amx_Push((*i), byteWeaponId);
			amx_Push((*i), wKillerId);
			amx_Push((*i), wPlayerId);
			// Execute the callback
			amx_Exec((*i), NULL, iIndex);
		}
	}
}

void CCallbackManager::OnReachDestination(WORD wPlayerId)
{
	for (std::list<AMX *>::iterator i = m_listAMX.begin(); i != m_listAMX.end(); i++) {
		// Get the function index
		int iIndex;
		if (!amx_FindPublic((*i), "FCNPC_OnReachDestination", &iIndex)) {
			// Push the parameters
			amx_Push((*i), wPlayerId);
			// Execute the callback
			amx_Exec((*i), NULL, iIndex);
		}
	}
}

void CCallbackManager::OnVehicleEntryComplete(WORD wPlayerId, WORD wVehicleId, int iSeat)
{
	for (std::list<AMX *>::iterator i = m_listAMX.begin(); i != m_listAMX.end(); i++) {
		// Get the function index
		int iIndex;
		if (!amx_FindPublic((*i), "FCNPC_OnVehicleEntryComplete", &iIndex)) {
			// Push the parameters
			amx_Push((*i), iSeat);
			amx_Push((*i), wVehicleId);
			amx_Push((*i), wPlayerId);
			// Execute the callback
			amx_Exec((*i), NULL, iIndex);
		}
	}
}

void CCallbackManager::OnVehicleExitComplete(WORD wPlayerId)
{
	for (std::list<AMX *>::iterator i = m_listAMX.begin(); i != m_listAMX.end(); i++) {
		// Get the function index
		int iIndex;
		if (!amx_FindPublic((*i), "FCNPC_OnVehicleExitComplete", &iIndex)) {
			// Push the parameters
			amx_Push((*i), wPlayerId);
			// Execute the callback
			amx_Exec((*i), NULL, iIndex);
		}
	}
}

int CCallbackManager::OnTakeDamage(WORD wPlayerId, WORD wDamagerId, BYTE byteWeaponId, int iBodyPart, float fHealthLoss)
{
	cell cReturn = 1;
	for (std::list<AMX *>::iterator i = m_listAMX.begin(); i != m_listAMX.end(); i++) {
		// Get the function index
		int iIndex;
		if (!amx_FindPublic((*i), "FCNPC_OnTakeDamage", &iIndex)) {
			// Push the parameters
			amx_Push((*i), amx_ftoc(fHealthLoss));
			amx_Push((*i), iBodyPart);
			amx_Push((*i), byteWeaponId);
			amx_Push((*i), wDamagerId);
			amx_Push((*i), wPlayerId);
			// Execute the callback
			amx_Exec((*i), &cReturn, iIndex);
			if (!cReturn) {
				return cReturn;
			}
		}
	}
	return cReturn;
}

void CCallbackManager::OnGiveDamage(WORD wPlayerId, WORD wIssuerId, BYTE byteWeaponId, int iBodyPart, float fHealthLoss)
{
	for (std::list<AMX *>::iterator i = m_listAMX.begin(); i != m_listAMX.end(); i++) {
		// Get the function index
		int iIndex;
		if (!amx_FindPublic((*i), "FCNPC_OnGiveDamage", &iIndex)) {
			// Push the parameters
			amx_Push((*i), amx_ftoc(fHealthLoss));
			amx_Push((*i), iBodyPart);
			amx_Push((*i), byteWeaponId);
			amx_Push((*i), wIssuerId);
			amx_Push((*i), wPlayerId);
			// Execute the callback
			amx_Exec((*i), NULL, iIndex);
		}
	}
}

int CCallbackManager::OnVehicleTakeDamage(WORD wPlayerId, WORD wDamagerId, WORD wVehicleId, BYTE byteWeaponId, CVector vecHit)
{
	cell cReturn = 1;
	for (std::list<AMX *>::iterator i = m_listAMX.begin(); i != m_listAMX.end(); i++) {
		// Get the function index
		int iIndex;
		if (!amx_FindPublic((*i), "FCNPC_OnVehicleTakeDamage", &iIndex)) {
			// Push the parameters
			amx_Push((*i), amx_ftoc(vecHit.fZ));
			amx_Push((*i), amx_ftoc(vecHit.fY));
			amx_Push((*i), amx_ftoc(vecHit.fX));
			amx_Push((*i), byteWeaponId);
			amx_Push((*i), wVehicleId);
			amx_Push((*i), wDamagerId);
			amx_Push((*i), wPlayerId);
			// Execute the callback
			amx_Exec((*i), &cReturn, iIndex);
			if (!cReturn) {
				return cReturn;
			}
		}
	}
	return cReturn;
}

void CCallbackManager::OnFinishPlayback(WORD wPlayerId)
{
	for (std::list<AMX *>::iterator i = m_listAMX.begin(); i != m_listAMX.end(); i++) {
		// Get the function index
		int iIndex;
		if (!amx_FindPublic((*i), "FCNPC_OnFinishPlayback", &iIndex)) {
			// Push the parameters
			amx_Push((*i), wPlayerId);
			// Execute the callback
			amx_Exec((*i), NULL, iIndex);
		}
	}
}

int CCallbackManager::OnChangeNode(WORD wPlayerId, WORD wNodeId)
{
	cell cReturn = 1;
	for (std::list<AMX *>::iterator i = m_listAMX.begin(); i != m_listAMX.end(); i++) {
		// Get the function index
		int iIndex;
		if (!amx_FindPublic((*i), "FCNPC_OnChangeNode", &iIndex)) {
			// Push the parameters
			amx_Push((*i), wNodeId);
			amx_Push((*i), wPlayerId);
			// Execute the callback
			amx_Exec((*i), &cReturn, iIndex);
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
	for (std::list<AMX *>::iterator i = m_listAMX.begin(); i != m_listAMX.end(); i++) {
		// Get the function index
		int iIndex;
		if (!amx_FindPublic((*i), "FCNPC_OnFinishNodePoint", &iIndex)) {
			// Push the parameters
			amx_Push((*i), wNodePoint);
			amx_Push((*i), wPlayerId);
			// Execute the callback
			amx_Exec((*i), &cReturn, iIndex);
			if (!cReturn) {
				return cReturn;
			}
		}
	}
	return cReturn;
}

void CCallbackManager::OnFinishNode(WORD wPlayerId)
{
	for (std::list<AMX *>::iterator i = m_listAMX.begin(); i != m_listAMX.end(); i++) {
		// Get the function index
		int iIndex;
		if (!amx_FindPublic((*i), "FCNPC_OnFinishNode", &iIndex)) {
			// Push the parameters
			amx_Push((*i), wPlayerId);
			// Execute the callback
			amx_Exec((*i), NULL, iIndex);
		}
	}
}

void CCallbackManager::OnStreamIn(WORD wPlayerId, WORD wForPlayerId)
{
	for (std::list<AMX *>::iterator i = m_listAMX.begin(); i != m_listAMX.end(); i++) {
		// Get the function index
		int iIndex;
		if (!amx_FindPublic((*i), "FCNPC_OnStreamIn", &iIndex)) {
			// Push the parameters
			amx_Push((*i), wForPlayerId);
			amx_Push((*i), wPlayerId);
			// Execute the callback
			amx_Exec((*i), NULL, iIndex);
		}
	}
}

void CCallbackManager::OnStreamOut(WORD wPlayerId, WORD wForPlayerId)
{
	for (std::list<AMX *>::iterator i = m_listAMX.begin(); i != m_listAMX.end(); i++) {
		// Get the function index
		int iIndex;
		if (!amx_FindPublic((*i), "FCNPC_OnStreamOut", &iIndex)) {
			// Push the parameters
			amx_Push((*i), wForPlayerId);
			amx_Push((*i), wPlayerId);
			// Execute the callback
			amx_Exec((*i), NULL, iIndex);
		}
	}
}

void CCallbackManager::OnUpdate(WORD wPlayerId)
{
	for (std::list<AMX *>::iterator i = m_listAMX.begin(); i != m_listAMX.end(); i++) {
		// Get the function index
		int iIndex;
		if (!amx_FindPublic((*i), "FCNPC_OnUpdate", &iIndex)) {
			// Push the parameters
			amx_Push((*i), wPlayerId);
			// Execute the callback
			amx_Exec((*i), NULL, iIndex);
		}
	}
}
