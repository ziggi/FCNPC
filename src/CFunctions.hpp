/* =========================================

		FCNPC - Fully Controllable NPC
			----------------------

	- File: SAMPFunctions.hpp
	- Author(s): OrMisicL

  =========================================*/


#ifndef SAMPFUNCTIONS_H
#define SAMPFUNCTIONS_H

struct Packet;
struct PlayerId;

// Functions definitions
typedef void (*ClientJoin_RPC_t)(RPCParameters *parameters);
typedef int  (*GetNetGame_t)();
typedef int  (*GetConsole_t)();
typedef int  (*GetRakServer_t)();

typedef void (THISCALL *CPlayerPool__DeletePlayer_t)(void *pPlayerPool, int iPlayerId, int iReason);
typedef void (THISCALL *CPlayer__SpawnForWorld_t)(void *pPlayer);
typedef void (THISCALL *CPlayer__Kill_t)(void *pPlayer, int iKillerId, int iWeapon);
typedef void (THISCALL *CPlayer__EnterVehicle_t)(void *pPlayer, int iVehicleId, int iSeatId);
typedef void (THISCALL *CPlayer__ExitVehicle_t)(void *pPlayer, int iVehicleId);
typedef int  (THISCALL *CConsole__GetIntVariable_t)(void *pConfig, char *szKey);
typedef bool (THISCALL *RakNet__Send_t)(void* ppRakServer, RakNet::BitStream* parameters, PacketPriority priority, PacketReliability reliability, unsigned orderingChannel, PlayerID playerId, bool broadcast);
typedef bool (THISCALL *RakNet__RPC_t)(void* ppRakServer, int* uniqueID, RakNet::BitStream* parameters, PacketPriority priority, PacketReliability reliability, unsigned orderingChannel, PlayerID playerId, bool broadcast, bool shiftTimestamp);
typedef Packet* (THISCALL *RakNet__Receive_t)(void* ppRakServer);

typedef CVector *( *GetVehicleModelInfo_t)(int iModelId, int iInfoType);

class CFunctions
{
public:
	static void		Initialize();
	static void		PreInitialize();

	static int		GetFreePlayerSlot();
	static int		NewPlayer(char *szName);
	static void		DeletePlayer(int iPlayerId);
	static void		SpawnPlayer(CPlayer *pPlayer);
	static void		KillPlayer(CPlayer *pPlayer, int iKillerId, int iWeapon);
	static void		PlayerEnterVehicle(CPlayer *pPlayer, int iVehicleId, int iSeatId);
	static void		PlayerExitVehicle(CPlayer *pPlayer, int iVehicleId);
	static CVector	*GetVehicleModelInfoEx(int iModelId, int iInfoType);
	static int		GetMaxPlayers();
	static int		GetMaxNPC();
	static void		PlayerShoot(int iPlayerId, WORD iHitId, BYTE iHitType, BYTE iWeaponId, CVector vecPoint);
	static int		GetNetGame()
	{
		return pfn__GetNetGame();
	}
	static int		GetConsole()
	{
		return pfn__GetConsole();
	}
	static int		GetRakServer()
	{
		return pfn__GetRakServer();
	}

	static void		GlobalRPC(int* szUniqueID, RakNet::BitStream* bsParams, int iExcludePlayerId = INVALID_PLAYER_ID, char PacketStream = 2);
	static void		AddedPlayersRPC(int* szUniqueID, RakNet::BitStream* bsParams, int iPlayerId, char PacketStream = 2);
	static void		AddedVehicleRPC(int* szUniqueID, RakNet::BitStream* bsParams, int iVehicleId, int iExcludePlayerId = INVALID_PLAYER_ID, char PacketStream = 2);
	static void		PlayerRPC(int* szUniqueID, RakNet::BitStream* bsParams, int iPlayerId, char PacketStream = 2);

	static void		GlobalPacket(RakNet::BitStream* bsParams);
	static void		PlayerPacket(RakNet::BitStream* bsParams, int iPlayerId);

	// Functions
	static ClientJoin_RPC_t					pfn__ClientJoin_RPC;
	static CPlayerPool__DeletePlayer_t		pfn__CPlayerPool__DeletePlayer;
	static CPlayer__SpawnForWorld_t			pfn__CPlayer__SpawnForWorld;
	static CPlayer__Kill_t					pfn__CPlayer__Kill;
	static CPlayer__EnterVehicle_t			pfn__CPlayer__EnterVehicle;
	static CPlayer__ExitVehicle_t			pfn__CPlayer__ExitVehicle;
	static CConsole__GetIntVariable_t		pfn__CConsole__GetIntVariable;
	static GetVehicleModelInfo_t			pfn__GetVehicleModelInfo;
	static RakNet__Send_t					pfn__RakNet__Send;
	static RakNet__RPC_t					pfn__RakNet__RPC;
	static RakNet__Receive_t				pfn__RakNet__Receive;
	static GetNetGame_t						pfn__GetNetGame;
	static GetConsole_t						pfn__GetConsole;
	static GetRakServer_t					pfn__GetRakServer;

};

#endif
