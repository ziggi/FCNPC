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
	for(EntityId i = 0; i < MAX_PLAYERS; i++)
	{
		m_bConnected[i] = false;
		m_pPlayer[i] = NULL;
	}
}

CPlayerManager::~CPlayerManager()
{
	// Reset values
	m_players = 0;
	// Reset player values
	for(EntityId i = 0; i < MAX_PLAYERS; i++)
	{
		if(m_bConnected[i])
			SAFE_DELETE(m_pPlayer[i]);
	}
}

EntityId CPlayerManager::AddPlayer(char *szName)
{
	// Make sure the name is valid
	if(strlen(szName) > MAX_NAME_LENGTH || pServer->DoesNameExist(szName))
		return INVALID_ENTITY_ID;

	// Create the player in SAMP server
	int iGameId = CSAMPFunctions::NewPlayer(szName);
	if(iGameId == INVALID_ENTITY_ID)
		return INVALID_ENTITY_ID;

	// Create the player instance
	m_pPlayer[iGameId] = new CPlayer(iGameId, szName);
	if(!m_pPlayer[iGameId])
	{
		// Delete the player from samp's playerpool
		CSAMPFunctions::DeletePlayer(iGameId);
		return INVALID_ENTITY_ID;
	}
	// Try to setup the player
	if(!SetupPlayer(iGameId))
	{
		// Delete the player instance
		SAFE_DELETE(m_pPlayer[iGameId]);
		// Delete the player from samp's playerpool
		CSAMPFunctions::DeletePlayer(iGameId);
		return INVALID_ENTITY_ID;
	}
	// Mark connected
	m_bConnected[iGameId] = true;
	// Increase players count
	m_players++;
	// Gall the created callback
	CCallbackManager::OnCreate(iGameId);
	// Return the NPC ID
	return iGameId;
}

bool CPlayerManager::DeletePlayer(EntityId playerId)
{
	// If he's not connected then dont go any further
	if(!m_bConnected[playerId])
		return false;

	// Destroy the player
	m_pPlayer[playerId]->Destroy();
	// Delete the player instance
	SAFE_DELETE(m_pPlayer[playerId]);
	// Mark not connected
	m_bConnected[playerId] = false;
	// Decrease players count
	m_players--;
	return true;
}

void CPlayerManager::Process()
{
	// Process all the players
	for(EntityId i = (CSAMPFunctions::GetMaxPlayers() - 1); i != 0; i--)
	{
		if(m_bConnected[i])
			m_pPlayer[i]->Process();
	}
}

bool CPlayerManager::SetupPlayer(EntityId playerId)
{
	// Setup the NPC
	if(!GetAt(playerId)->Setup())
		return false;

	return true;
}

bool CPlayerManager::IsNPC(int iPlayerId)
{
	// Loop through all the players
	for(EntityId i = (CSAMPFunctions::GetMaxPlayers() - 1); i != 0; i--)
	{
		if(m_bConnected[i] && i == iPlayerId)
			return true;
	}
	return false;
}
