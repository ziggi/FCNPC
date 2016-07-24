/* =========================================

		FCNPC - Fully Controllable NPC
			----------------------

	- File: ThreadFunctions.cpp
	- Author(s): OrMisicL

  =========================================*/

#include "Main.hpp"

extern CServer     *pServer;
extern bool        bServerInit;

// Keep that until a further notice, for now, no threads needed
void CThreadFunctions::DamageThread(void *pThread)
{
	/*	// Get the thread pointer
		CThread *pThisThread = (CThread *)pThread;
		// Wait for the server to be initialized
		while(!bServerInit)
			CUtils::FCNPCSleep(5);

		// Get the playerpool pointer
		CPlayerPool *pPlayerPool = pNetGame->pPlayerPool;
		// Are we still alive ?
		while(bServerInit)
		{
			// Loop through all the players
			for (int i = 0; i < pNetGame->pPlayerPool->dwPlayerPoolSize; i++)
			{
				// Ignore non connected players
				if (!pServer->GetPlayerManager()->IsPlayerConnected(i) ||
					(pServer->GetPlayerManager()->IsNPC(i) && !pServer->GetPlayerManager()->GetAt(i)->IsSpawned()))
					continue;

				// Loop through all the npcs
				for (int j = pNetGame->pPlayerPool->dwPlayerPoolSize; j != -1; j--)
				{
					// Ignore non connected NPCs
					if (!pServer->GetPlayerManager()->IsNpcConnected(j) || !pServer->GetPlayerManager()->GetAt(j)->IsSpawned())
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
