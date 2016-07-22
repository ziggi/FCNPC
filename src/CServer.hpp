/* =========================================

		FCNPC - Fully Controllable NPC
			----------------------

	- File: Server.hpp
	- Author(s): OrMisicL

  =========================================*/

#ifndef SERVER_H
#define SERVER_H

struct CNetGame;
class RakServer;
struct Packet;
struct CVehicleSpawn;
class CGangZonePool;

#include "Address.hpp"
#include "PlayerManager.hpp"
#include "NodeManager.hpp"
#include <list>

class CMapAndreas;

class CServer
{
public:
	CServer(eSAMPVersion version);
	~CServer();

	BYTE Initialize(AMX *pAMX);
	void Process();

	void SetTickRate(int rate) { m_iTickRate = rate; }
	int GetTickRate(void) { return m_iTickRate; }

	CPlayerManager *GetPlayerManager()
	{
		return m_pPlayerDataManager;
	};
	CNodeManager *GetNodeManager()
	{
		return m_pNodeManager;
	};

	bool DoesNameExist(char *szName);

	bool SetUpdateRate(DWORD dwRate)
	{
		if (dwRate < 0) {
			return false;
		}
		m_dwUpdateRate = dwRate;
		return true;
	};
	DWORD GetUpdateRate()
	{
		return m_dwUpdateRate;
	};

	void SetMapAndreas(CMapAndreas *pMapAndreas);
	CMapAndreas *GetMapAndreas();
	bool IsMapAndreasInited();

	bool IsVehicleSeatOccupied(int iPlayerId, WORD wVehicleId, BYTE byteSeatId);
	WORD GetVehicleSeatPlayerId(WORD wVehicleId, BYTE byteSeatId);
	float GetVehicleAngle(CVehicle *pVehicle);
	CVector GetVehiclePos(CVehicle *pVehicle);
	CVector GetVehicleSeatPos(CVehicle *pVehicle, int iSeatId);

	eSAMPVersion GetVersion()
	{
		return m_Version;
	}

private:
	eSAMPVersion   m_Version;
	CPlayerManager *m_pPlayerDataManager;
	CNodeManager   *m_pNodeManager;
	CThread        *m_pDamageThread;
	CMapAndreas    *m_pMapAndreas;
	DWORD          m_dwUpdateRate;

	int m_iTicks;
	int m_iTickRate;
};

#endif
