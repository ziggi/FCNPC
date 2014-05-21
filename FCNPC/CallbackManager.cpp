/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: CallbackManager.cpp
	- Author(s): OrMisicL

  =========================================*/


#include "Main.h"

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

void CCallbackManager::OnCreate(int iGameId)
{
	for(std::list<AMX *>::iterator i = m_listAMX.begin(); i != m_listAMX.end(); i++)
	{
		// Get the function index
		int iIndex;
		if(!amx_FindPublic((*i), "FCNPC_OnCreate", &iIndex))
		{
			// Push the NPC ID
			amx_Push((*i), iGameId);
			// Execute the callback
			amx_Exec((*i), NULL, iIndex);
		}
	}
}

void CCallbackManager::OnSpawn(int iGameId)
{
	for(std::list<AMX *>::iterator i = m_listAMX.begin(); i != m_listAMX.end(); i++)
	{
		// Get the function index
		int iIndex;
		if(!amx_FindPublic((*i), "FCNPC_OnSpawn", &iIndex))
		{
			// Push the NPC ID
			amx_Push((*i), iGameId);
			// Execute the callback
			amx_Exec((*i), NULL, iIndex);
		}
	}
}

void CCallbackManager::OnRespawn(int iGameId)
{
	for(std::list<AMX *>::iterator i = m_listAMX.begin(); i != m_listAMX.end(); i++)
	{
		// Get the function index
		int iIndex;
		if(!amx_FindPublic((*i), "FCNPC_OnRespawn", &iIndex))
		{
			// Push the NPC ID
			amx_Push((*i), iGameId);
			// Execute the callback
			amx_Exec((*i), NULL, iIndex);
		}
	}
}

void CCallbackManager::OnDeath(int iGameId, int iKillerId, int iWeapon)
{
	for(std::list<AMX *>::iterator i = m_listAMX.begin(); i != m_listAMX.end(); i++)
	{
		// Get the function index
		int iIndex;
		if(!amx_FindPublic((*i), "FCNPC_OnDeath", &iIndex))
		{
			// Push the parameters
			amx_Push((*i), iWeapon);
			amx_Push((*i), iKillerId);
			amx_Push((*i), iGameId);
			// Execute the callback
			amx_Exec((*i), NULL, iIndex);
		}
	}
}

void CCallbackManager::OnReachDestination(int iGameId)
{
	for(std::list<AMX *>::iterator i = m_listAMX.begin(); i != m_listAMX.end(); i++)
	{
		// Get the function index
		int iIndex;
		if(!amx_FindPublic((*i), "FCNPC_OnReachDestination", &iIndex))
		{
			// Push the parameters
			amx_Push((*i), iGameId);
			// Execute the callback
			amx_Exec((*i), NULL, iIndex);
		}
	}
}

void CCallbackManager::OnVehicleEntryComplete(int iGameId, int iVehicleId, int iSeat)
{
	for(std::list<AMX *>::iterator i = m_listAMX.begin(); i != m_listAMX.end(); i++)
	{
		// Get the function index
		int iIndex;
		if(!amx_FindPublic((*i), "FCNPC_OnVehicleEntryComplete", &iIndex))
		{
			// Push the parameters
			amx_Push((*i), iSeat);
			amx_Push((*i), iVehicleId);
			amx_Push((*i), iGameId);
			// Execute the callback
			amx_Exec((*i), NULL, iIndex);
		}
	}
}

void CCallbackManager::OnVehicleExitComplete(int iGameId)
{
	for(std::list<AMX *>::iterator i = m_listAMX.begin(); i != m_listAMX.end(); i++)
	{
		// Get the function index
		int iIndex;
		if(!amx_FindPublic((*i), "FCNPC_OnVehicleExitComplete", &iIndex))
		{
			// Push the parameters
			amx_Push((*i), iGameId);
			// Execute the callback
			amx_Exec((*i), NULL, iIndex);
		}
	}
}

int CCallbackManager::OnTakeDamage(int iGameId, int iDamagerId, int iWeapon, int iBodyPart, float fHealthLoss)
{
	cell cReturn = 0;
	for(std::list<AMX *>::iterator i = m_listAMX.begin(); i != m_listAMX.end(); i++)
	{
		// Get the function index
		int iIndex;
		if(!amx_FindPublic((*i), "FCNPC_OnTakeDamage", &iIndex))
		{
			// Push the parameters
			amx_Push((*i), amx_ftoc(fHealthLoss));
			amx_Push((*i), iBodyPart);
			amx_Push((*i), iWeapon);
			amx_Push((*i), iDamagerId);
			amx_Push((*i), iGameId);
			// Execute the callback
			if(!cReturn)
				amx_Exec((*i), &cReturn, iIndex);
			else
				amx_Exec((*i), NULL, iIndex);
		}
	}
	return cReturn;
}

void CCallbackManager::OnFinishPlayback(int iGameId)
{
	for(std::list<AMX *>::iterator i = m_listAMX.begin(); i != m_listAMX.end(); i++)
	{
		// Get the function index
		int iIndex;
		if(!amx_FindPublic((*i), "FCNPC_OnFinishPlayback", &iIndex))
		{
			// Push the parameters
			amx_Push((*i), iGameId);
			// Execute the callback
			amx_Exec((*i), NULL, iIndex);
		}
	}
}

int CCallbackManager::OnChangeNode(int iGameId, int iNodeId)
{
	cell cReturn = 0;
	for(std::list<AMX *>::iterator i = m_listAMX.begin(); i != m_listAMX.end(); i++)
	{
		// Get the function index
		int iIndex;
		if(!amx_FindPublic((*i), "FCNPC_OnChangeNode", &iIndex))
		{
			// Push the parameters
			amx_Push((*i), iNodeId);
			amx_Push((*i), iGameId);
			// Execute the callback
			if(!cReturn)
				amx_Exec((*i), &cReturn, iIndex);
			else
				amx_Exec((*i), NULL, iIndex);
		}
	}
	return cReturn;
}

int CCallbackManager::OnFinishNodePoint(int iGameId, int iNodePoint)
{
	cell cReturn = 0;
	for(std::list<AMX *>::iterator i = m_listAMX.begin(); i != m_listAMX.end(); i++)
	{
		// Get the function index
		int iIndex;
		if(!amx_FindPublic((*i), "FCNPC_OnFinishNodePoint", &iIndex))
		{
			// Push the parameters
			amx_Push((*i), iNodePoint);
			amx_Push((*i), iGameId);
			// Execute the callback
			if(!cReturn)
				amx_Exec((*i), &cReturn, iIndex);
			else
				amx_Exec((*i), NULL, iIndex);
		}
	}
	return cReturn;
}

void CCallbackManager::OnFinishNode(int iGameId)
{
	for(std::list<AMX *>::iterator i = m_listAMX.begin(); i != m_listAMX.end(); i++)
	{
		// Get the function index
		int iIndex;
		if(!amx_FindPublic((*i), "FCNPC_OnFinishNode", &iIndex))
		{
			// Push the parameters
			amx_Push((*i), iGameId);
			// Execute the callback
			amx_Exec((*i), NULL, iIndex);
		}
	}
}