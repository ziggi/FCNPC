/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: Server.h
	- Author(s): OrMisicL

  =========================================*/

#ifndef SERVER_H
#define SERVER_H

#include "PlayerManager.h"
#include "NodeManager.h"
#include <list>

class CServer
{
	public:
		CServer();
		~CServer();

		BYTE Initialize();

		CPlayerManager     *GetPlayerManager() { return m_pPlayerManager; };
		CNodeManager       *GetNodeManager() { return m_pNodeManager; };

		bool    DoesNameExist(char *szName);

		void    SetUpdateRate(DWORD dwRate) { m_dwUpdateRate = dwRate; };
		DWORD   GetUpdateRate() { return m_dwUpdateRate; };

		CZMap   *GetZMap() { return m_pZMap; };

		bool    IsVehicleSeatOccupied(int iPlayerId, WORD wVehicleId, BYTE byteSeatId);

	private:
		CPlayerManager      *m_pPlayerManager;
		CNodeManager        *m_pNodeManager;
		CThread             *m_pDamageThread;
		CZMap               *m_pZMap;
		DWORD               m_dwUpdateRate;
};

#endif
