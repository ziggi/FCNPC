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
class ColAndreasWorld;

#include "CAddress.hpp"
#include "CPlayerManager.hpp"
#include "CNodeManager.hpp"
#include "CRecordManager.hpp"
#include "CMovePath.hpp"

class CServer
{
public:
	CServer(eSAMPVersion version);
	~CServer();

	BYTE Initialize(AMX *pAMX);
	void Process();

	CPlayerManager *GetPlayerManager();
	CNodeManager *GetNodeManager();
	CRecordManager *GetRecordManager();
	CMovePath *GetMovePath();
	CMapAndreas *GetMapAndreas();
	ColAndreasWorld *GetColAndreas();

	bool IsValidNickName(char *szName);
	bool DoesNameExist(char *szName);

	bool SetTickRate(int iRate);
	int GetTickRate();
	bool SetUpdateRate(DWORD dwRate);
	DWORD GetUpdateRate();

	void ToggleCrashLogCreation(bool enabled);
	bool GetCrashLogCreation();

	void ToggleMoveMode(int iMoveMode, bool bIsEnabled);
	bool IsMoveModeEnabled(int iMoveMode);
	void ToggleMovePathfinding(int iMovePathfinding, bool bIsEnabled);
	bool IsMovePathfindingEnabled(int iMovePathfinding);

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
	CRecordManager *m_pRecordManager;
	CMapAndreas *m_pMapAndreas;
	ColAndreasWorld *m_pColAndreas;
	CMovePath *m_pMovePath;
	DWORD m_dwUpdateRate;
	bool m_bCrashLogCreation;
	bool m_bMoveModeEnabled[MOVE_MODE_SIZE];
	bool m_bMovePathfindingEnabled[MOVE_PATHFINDING_SIZE];

	int m_iTicks;
	int m_iTickRate;
};

#endif
