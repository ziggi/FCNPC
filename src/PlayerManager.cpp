/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: PlayerManager.cpp
	- Author(s): OrMisicL

  =========================================*/


#include "Main.h"

extern CServer			*pServer;
bool					bCreated = false;

CPlayerManager::CPlayerManager()
{
	// Reset values
	m_players = 0;
	// Reset player values
	for(WORD i = 0; i < MAX_PLAYERS; i++)
	{
		m_bConnected[i] = false;
		m_pPlayerData[i] = NULL;
	}
}

CPlayerManager::~CPlayerManager()
{
	// Reset values
	m_players = 0;
	// Reset player values
	for(WORD i = 0; i < MAX_PLAYERS; i++)
	{
		if(m_bConnected[i])
			SAFE_DELETE(m_pPlayerData[i]);
	}
}

WORD CPlayerManager::AddPlayer(char *szName)
{
	// Make sure the name is valid
	if(strlen(szName) > MAX_PLAYER_NAME || pServer->DoesNameExist(szName))
		return INVALID_ENTITY_ID;

	// Create the player in SAMP server
	int iGameId = CSAMPFunctions::NewPlayer(szName);
	if(iGameId == INVALID_ENTITY_ID)
		return INVALID_ENTITY_ID;

	// Create the player instance
	m_pPlayerData[iGameId] = new CPlayerData(iGameId, szName);
	if(!m_pPlayerData[iGameId])
	{
		// Delete the player from samp's playerpool
		CSAMPFunctions::DeletePlayer(iGameId);
		return INVALID_ENTITY_ID;
	}
	// Try to setup the player
	if(!SetupPlayer(iGameId))
	{
		// Delete the player instance
		SAFE_DELETE(m_pPlayerData[iGameId]);
		// Delete the player from samp's playerpool
		CSAMPFunctions::DeletePlayer(iGameId);
		return INVALID_ENTITY_ID;
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
	if(!m_bConnected[playerId])
		return false;

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
	for(WORD i = (CSAMPFunctions::GetMaxPlayers() - 1); i != 0; i--)
	{
		if(m_bConnected[i])
			m_pPlayerData[i]->Process();
	}
}

bool CPlayerManager::SetupPlayer(WORD playerId)
{
	// Setup the NPC
	return GetAt(playerId)->Setup();
}

bool CPlayerManager::IsNPC(int iPlayerId)
{
	// Loop through all the players
	for(WORD i = (CSAMPFunctions::GetMaxPlayers() - 1); i != 0; i--)
	{
		if(m_bConnected[i] && i == iPlayerId)
			return true;
	}
	return false;
}
