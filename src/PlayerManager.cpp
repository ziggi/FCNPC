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
	// Reset values
	m_players = 0;
	// Reset player values
	for (WORD i = 0; i < MAX_PLAYERS; i++) {
		m_bConnected[i] = false;
		m_pPlayerData[i] = NULL;
	}
}

CPlayerManager::~CPlayerManager()
{
	// Reset values
	m_players = 0;
	// Reset player values
	for (WORD i = 0; i < MAX_PLAYERS; i++) {
		if (m_bConnected[i]) {
			SAFE_DELETE(m_pPlayerData[i]);
		}
	}
}

WORD CPlayerManager::AddPlayer(char *szName)
{
	// Make sure the name is valid
	if (strlen(szName) > MAX_PLAYER_NAME || pServer->DoesNameExist(szName)) {
		return INVALID_PLAYER_ID;
	}

	// Create the player in SAMP server
	int iGameId = CFunctions::NewPlayer(szName);
	if (iGameId == INVALID_PLAYER_ID) {
		return INVALID_PLAYER_ID;
	}

	// Create the player instance
	m_pPlayerData[iGameId] = new CPlayerData(iGameId, szName);
	if (!m_pPlayerData[iGameId]) {
		// Delete the player from samp's playerpool
		CFunctions::DeletePlayer(iGameId);
		return INVALID_PLAYER_ID;
	}
	// Try to setup the player
	if (!SetupPlayer(iGameId)) {
		// Delete the player instance
		SAFE_DELETE(m_pPlayerData[iGameId]);
		// Delete the player from samp's playerpool
		CFunctions::DeletePlayer(iGameId);
		return INVALID_PLAYER_ID;
	}
	// Mark connected
	m_bConnected[iGameId] = true;
	// Increase players count
	m_players++;
	// Call the created callback
	CCallbackManager::OnCreate(iGameId);
	// Return the NPC ID
	return iGameId;
}

bool CPlayerManager::DeletePlayer(WORD playerId)
{
	// If he's not connected then dont go any further
	if (!m_bConnected[playerId]) {
		return false;
	}

	// Destroy the player
	m_pPlayerData[playerId]->Destroy();
	// Delete the player instance
	SAFE_DELETE(m_pPlayerData[playerId]);
	// Mark not connected
	m_bConnected[playerId] = false;
	// Decrease players count
	m_players--;
	return true;
}

void CPlayerManager::Process()
{
	// Process all the players
	for (WORD i = (CFunctions::GetMaxPlayers() - 1); i != 0; i--) {
		if (m_bConnected[i]) {
			m_pPlayerData[i]->Process();
		}
	}
}

bool CPlayerManager::IsPlayerConnectedEx(WORD playerId)
{
	if (playerId >= MAX_PLAYERS || playerId < 0) {
		return false;
	} else {
		return m_bConnected[playerId];
	}
}

CPlayerData *CPlayerManager::GetAt(WORD playerId)
{
	return m_pPlayerData[playerId];
}

bool CPlayerManager::SetupPlayer(WORD playerId)
{
	// Setup the NPC
	return GetAt(playerId)->Setup();
}

bool CPlayerManager::IsNPC(int iPlayerId)
{
	return m_bConnected[iPlayerId];
}
