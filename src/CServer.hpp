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

#include "CAddress.hpp"
#include "CPlayerManager.hpp"
#include "CNodeManager.hpp"
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

	CPlayerManager *GetPlayerManager();
	CNodeManager *GetNodeManager();

	bool DoesNameExist(char *szName);

	bool SetUpdateRate(DWORD dwRate);
	DWORD GetUpdateRate();

	void SetMapAndreas(CMapAndreas *pMapAndreas);
	CMapAndreas *GetMapAndreas();
	bool IsMapAndreasInited();

	bool IsVehicleSeatOccupied(WORD wPlayerId, WORD wVehicleId, BYTE byteSeatId);
	WORD GetVehicleSeatPlayerId(WORD wVehicleId, BYTE byteSeatId);
	float GetVehicleAngle(CVehicle *pVehicle);
	CVector GetVehiclePos(CVehicle *pVehicle);
	CVector GetVehicleSeatPos(CVehicle *pVehicle, BYTE byteSeatId);

	eSAMPVersion GetVersion();

private:
	eSAMPVersion m_Version;
	CPlayerManager *m_pPlayerDataManager;
	CNodeManager *m_pNodeManager;
	CMapAndreas *m_pMapAndreas;
	DWORD m_dwUpdateRate;

	int m_iTicks;
	int m_iTickRate;
};

#endif
