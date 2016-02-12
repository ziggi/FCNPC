/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: Server.h
	- Author(s): OrMisicL

  =========================================*/

#ifndef SERVER_H
#define SERVER_H

struct CNetGame;
class RakServer;
struct Packet;
struct CVehicleSpawn;
class CGangZonePool;

#include "Address.h"
#include "PlayerManager.h"
#include "NodeManager.h"
#include "ZMap.h"
#include <list>

class CServer
{
	public:
		CServer(eSAMPVersion version);
		~CServer();

		BYTE Initialize();

		CPlayerManager     *GetPlayerManager() { return m_pPlayerDataManager; };
		CNodeManager       *GetNodeManager() { return m_pNodeManager; };

		bool    DoesNameExist(char *szName);

		void    SetUpdateRate(DWORD dwRate) { m_dwUpdateRate = dwRate; };
		DWORD   GetUpdateRate() { return m_dwUpdateRate; };

		CZMap   *GetZMap() { return m_pZMap; };

		bool    IsVehicleSeatOccupied(int iPlayerId, WORD wVehicleId, BYTE byteSeatId);

		eSAMPVersion GetVersion() { return m_Version; }

	private:
		eSAMPVersion        m_Version;
		CPlayerManager      *m_pPlayerDataManager;
		CNodeManager        *m_pNodeManager;
		CThread             *m_pDamageThread;
		CZMap               *m_pZMap;
		DWORD               m_dwUpdateRate;
};

#endif
