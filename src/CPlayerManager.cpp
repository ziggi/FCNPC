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
	for (auto &pPlayer : m_pNpcArray) {
		pPlayer = NULL;
	}
}

CPlayerManager::~CPlayerManager()
{
	for (auto &pPlayer : m_pNpcArray) {
		SAFE_DELETE(pPlayer);
	}
}

WORD CPlayerManager::AddPlayer(char *szName)
{
	// Make sure the name is valid
	if (!pServer->IsValidNickName(szName)) {
		logprintf("[FCNPC] Error: NPC '%s' not created. Name '%s' is invalid.", szName, szName);
		return INVALID_PLAYER_ID;
	}

	// Make sure the name is not exists
	if (pServer->DoesNameExist(szName)) {
		logprintf("[FCNPC] Error: NPC '%s' not created. Another player already connected with that name.", szName, szName);
		return INVALID_PLAYER_ID;
	}

	// Create the player in SAMP server
	WORD wPlayerId = CFunctions::NewPlayer(szName);
	if (wPlayerId == INVALID_PLAYER_ID) {
		logprintf("[FCNPC] Error: NPC '%s' not created. The maxplayers limit in server.cfg has been reached.", szName);
		return INVALID_PLAYER_ID;
	}

	// Create the player instance
	m_pNpcArray[wPlayerId] = new CPlayerData(wPlayerId, szName);
	if (!m_pNpcArray[wPlayerId]) {
		CFunctions::DeletePlayer(wPlayerId);
		logprintf("[FCNPC] Error: NPC '%s' not created. The NPC instance could not be created.", szName);
		return INVALID_PLAYER_ID;
	}

	// Try to setup the player
	if (!SetupPlayer(wPlayerId)) {
		SAFE_DELETE(m_pNpcArray[wPlayerId]);
		CFunctions::DeletePlayer(wPlayerId);
		logprintf("[FCNPC] Error: NPC '%s' not created. Name '%s' is invalid or the maxnpc limit in server.cfg has been reached.", szName, szName);
		return INVALID_PLAYER_ID;
	}

	m_vNpcID.push_back(wPlayerId);
	// Call the created callback
	CCallbackManager::OnCreate(wPlayerId);
	return wPlayerId;
}

bool CPlayerManager::DeletePlayer(WORD wPlayerId)
{
	// If he's not connected then dont go any further
	if (!m_pNpcArray[wPlayerId]) {
		return false;
	}

	// Destroy the player
	m_vNpcID.erase(std::remove(m_vNpcID.begin(), m_vNpcID.end(), wPlayerId), m_vNpcID.end());
	m_pNpcArray[wPlayerId]->Destroy();
	SAFE_DELETE(m_pNpcArray[wPlayerId]);
	// Call the created callback
	CCallbackManager::OnDestroy(wPlayerId);
	return true;
}

void CPlayerManager::Process()
{
	// Process all the players
	for (auto &id : m_vNpcID) {
		m_pNpcArray[id]->Process();
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
		return pNetGame->pPlayerPool->bIsPlayerConnectedEx[wPlayerId] != 0;
	}
}

CPlayerData *CPlayerManager::GetAt(WORD wPlayerId)
{
	if (!IsNpcConnected(wPlayerId)) {
		return NULL;
	}
	return m_pNpcArray[wPlayerId];
}

bool CPlayerManager::SetupPlayer(WORD wPlayerId)
{
	// Setup the NPC
	return m_pNpcArray[wPlayerId]->Setup();
}

bool CPlayerManager::IsNPC(WORD wPlayerId)
{
	return !!m_pNpcArray[wPlayerId];
}
