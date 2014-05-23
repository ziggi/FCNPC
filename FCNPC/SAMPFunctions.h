/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: SAMPFunctions.h
	- Author(s): OrMisicL

  =========================================*/


#ifndef SAMPFUNCTIONS_H
#define SAMPFUNCTIONS_H

// Functions definitions
typedef void ( *CreateNPC_RPC_t)(CSAMPRPCParams *pRPCParams);
typedef int  ( *GetNetGame_t)();
typedef int  ( *GetConsole_t)();
typedef int  ( *GetRakServer_t)();
#ifdef _WIN32
typedef void (__thiscall *CPlayerPool__DeletePlayer_t)(void *pPlayerPool, int iPlayerId, int iReason);
typedef void (__thiscall *CPlayer__SpawnForWorld_t)(void *pPlayer);
typedef void (__thiscall *CPlayer__Kill_t)(void *pPlayer, int iKillerId, int iWeapon);
typedef void (__thiscall *CPlayer__EnterVehicle_t)(void *pPlayer, int iVehicleId, int iSeatId);
typedef void (__thiscall *CPlayer__ExitVehicle_t)(void *pPlayer, int iVehicleId);
typedef int  (__thiscall *CConfig__GetValueAsInteger_t)(void *pConfig, char *szKey);
#else
typedef int  ( *CPlayerPool__DeletePlayer_t)(void *pPlayerPool, WORD wPlayerId, BYTE byteReason);
typedef void ( *CPlayer__SpawnForWorld_t)(void *pPlayer);
typedef void ( *CPlayer__Kill_t)(void *pPlayer, int iKillerId, int iWeapon);
typedef void ( *CPlayer__EnterVehicle_t)(void *pPlayer, int iVehicleId, int iSeatId);
typedef void ( *CPlayer__ExitVehicle_t)(void *pPlayer, int iVehicleId);
typedef int  ( *CConfig__GetValueAsInteger_t)(void *pConfig, char *szKey);
#endif
typedef CVector3 *( *GetVehicleModelInfo_t)(int iModelId, int iInfoType);

class CSAMPFunctions
{
	public:
		static void		Initialize();
		static void		PreInitialize();

		static int		GetFreePlayerSlot();
		static int		NewPlayer(char *szName);
		static void		DeletePlayer(int iPlayerId);
		static void		SpawnPlayer(int iPlayerId);
		static void		KillPlayer(int iPlayerId, int iKillerId, int iWeapon);
		static void		PlayerEnterVehicle(int iPlayerId, int iVehicleId, int iSeatId);
		static void		PlayerExitVehicle(int iPlayerId, int iVehicleId);
		static CVector3	*GetVehicleModelInfo(int iModelId, int iInfoType);
		static int		GetMaxPlayers();
		static int		GetMaxNPC();
		static int		GetNetGame() { return pfn__GetNetGame(); }
		static int		GetConsole() { return pfn__GetConsole(); }
		static int		GetRakServer() { return pfn__GetRakServer(); }

		// Functions
		static CreateNPC_RPC_t					pfn__CreateNPC_RPC;
		static CPlayerPool__DeletePlayer_t		pfn__CPlayerPool__DeletePlayer;
		static CPlayer__SpawnForWorld_t			pfn__CPlayer__SpawnForWorld;
		static CPlayer__Kill_t					pfn__CPlayer__Kill;
		static CPlayer__EnterVehicle_t			pfn__CPlayer__EnterVehicle;
		static CPlayer__ExitVehicle_t			pfn__CPlayer__ExitVehicle;
		static CConfig__GetValueAsInteger_t		pfn__CConfig__GetValueAsInteger;
		static GetVehicleModelInfo_t			pfn__GetVehicleModelInfo;
		static GetNetGame_t						pfn__GetNetGame;
		static GetConsole_t						pfn__GetConsole;
		static GetRakServer_t					pfn__GetRakServer;

};

#endif