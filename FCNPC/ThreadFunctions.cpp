/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: ThreadFunctions.cpp
	- Author(s): OrMisicL

  =========================================*/

#include "Main.h"

extern CServer		*pServer;
extern bool			bServerInit;

// Keep that until a further notice, for now, no threads needed
void CThreadFunctions::DamageThread(void *pThread)
{
/*	// Get the thread pointer
	CThread *pThisThread = (CThread *)pThread;
	// Wait for the server to be initialized
	while(!bServerInit)
		CUtils::FCNPCSleep(5);

	// Get the playerpool pointer
	CSAMPServer *pSAMPServer = (CSAMPServer *)CAddress::VAR_ServerPtr;
	CSAMPPlayerPool *pPlayerPool = pSAMPServer->pPlayerPool;
	// Are we still alive ?
	while(bServerInit)
	{		
		// Loop through all the players
		for(int i = 0; i < MAX_PLAYERS; i++)
		{
			// Ignore non connected players
			if(!pPlayerPool->bIsPlayerConnected[i] || 
				(pServer->GetPlayerManager()->IsNPC(i) && !pServer->GetPlayerManager()->GetAt(i)->IsSpawned()))
				continue;

			// Loop through all the npcs
			for(int j = (MAX_PLAYERS - 1); j != 0; j--)
			{
				// Ignore non connected NPCs
				if(!pServer->GetPlayerManager()->IsPlayerConnected(j) || !pServer->GetPlayerManager()->GetAt(j)->IsSpawned())
					continue;

				// Process damage for players
				//pServer->GetPlayerManager()->GetAt(j)->ProcessDamage(i);
			}
		}
		CUtils::FCNPCSleep(5);
	}
	// Stop our thread
	pThisThread->Stop();*/
}
