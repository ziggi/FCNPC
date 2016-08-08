/* =========================================

		FCNPC - Fully Controllable NPC
			----------------------

	- File: PlayerManager.cpp
	- Author(s): OrMisicL

  =========================================*/


#include "Main.hpp"

extern CServer			*pServer;
bool					bCreated = false;

CPlayerManager::CPlayerManager()
{
}

CPlayerManager::~CPlayerManager()
{
	m_NpcMap.clear();
}

WORD CPlayerManager::AddPlayer(char *szName)
{
	// Make sure the name is valid
	if (strlen(szName) > MAX_PLAYER_NAME || pServer->DoesNameExist(szName)) {
		logprintf("[FCNPC] Error: name '%s' is invalid.", szName);
		return INVALID_PLAYER_ID;
	}

	// Create the player in SAMP server
	WORD wPlayerId = CFunctions::NewPlayer(szName);
	if (wPlayerId == INVALID_PLAYER_ID) {
		logprintf("[FCNPC] Error: player '%s' is not created.", szName);
		return INVALID_PLAYER_ID;
	}

	// Create the player instance
	m_NpcMap.insert(std::make_pair(wPlayerId, new CPlayerData(wPlayerId, szName)));
	if (!m_NpcMap[wPlayerId]) {
		CFunctions::DeletePlayer(wPlayerId);
		m_NpcMap.erase(wPlayerId);
		logprintf("[FCNPC] Error: player instance for '%s' is not created.", szName);
		return INVALID_PLAYER_ID;
	}

	// Try to setup the player
	if (!SetupPlayer(wPlayerId)) {
		SAFE_DELETE(m_NpcMap[wPlayerId]);
		m_NpcMap.erase(wPlayerId);
		CFunctions::DeletePlayer(wPlayerId);
		logprintf("[FCNPC] Error: player '%s' is not setup.", szName);
		return INVALID_PLAYER_ID;
	}

	// Call the created callback
	CCallbackManager::OnCreate(wPlayerId);
	return wPlayerId;
}

bool CPlayerManager::DeletePlayer(WORD wPlayerId)
{
	// If he's not connected then dont go any further
	if (m_NpcMap.find(wPlayerId) == m_NpcMap.end()) {
		return false;
	}

	// Destroy the player
	m_NpcMap[wPlayerId]->Destroy();
	SAFE_DELETE(m_NpcMap[wPlayerId]);
	m_NpcMap.erase(wPlayerId);
	return true;
}

void CPlayerManager::Process()
{
	// Process all the players
	for (auto &kv : m_NpcMap) {
		kv.second->Process();
	}
}

bool CPlayerManager::IsNpcConnected(WORD wPlayerId)
{
	return IsPlayerConnected(wPlayerId) && IsNPC(wPlayerId);
}

bool CPlayerManager::IsPlayerConnected(WORD wPlayerId)
{
	if (wPlayerId >= MAX_PLAYERS || wPlayerId < 0) {
		return false;
	} else {
		return pNetGame->pPlayerPool->bIsPlayerConnectedEx[wPlayerId];
	}
}

CPlayerData *CPlayerManager::GetAt(WORD wPlayerId)
{
	if (!IsNpcConnected(wPlayerId)) {
		return NULL;
	}
	return m_NpcMap[wPlayerId];
}

bool CPlayerManager::SetupPlayer(WORD wPlayerId)
{
	// Setup the NPC
	return m_NpcMap[wPlayerId]->Setup();
}

bool CPlayerManager::IsNPC(WORD wPlayerId)
{
	return !!(m_NpcMap.find(wPlayerId) != m_NpcMap.end());
}
