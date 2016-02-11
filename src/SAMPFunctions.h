/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: SAMPFunctions.h
	- Author(s): OrMisicL

  =========================================*/


#ifndef SAMPFUNCTIONS_H
#define SAMPFUNCTIONS_H

class CSAMPPlayerId
{
public:
	CSAMPPlayerId(CSAMPSystemAddress systemAddress)
	{
		uiSystemAddress = systemAddress.uiSystemAddress;
		usPort = systemAddress.usPort;
	};
	
	unsigned int		uiSystemAddress;	// 0x0000 - 0x0004
	unsigned short		usPort;				// 0x0004 - 0x0006
};

/// This represents a user message from another system.
struct Packet
{
	/// Server only - this is the index into the player array that this playerId maps to
	unsigned short playerIndex; // 0 - 2

	/// The system that send this packet.
	CSAMPPlayerId playerId; // 2  - 8

	/// The length of the data in bytes
	/// \deprecated You should use bitSize.
	unsigned int length; // 8 - 12

	/// The length of the data in bits
	unsigned int bitSize; // 12 - 16

	/// The data from the sender
	unsigned char* data; // 16 - 20

	/// @internal
	/// Indicates whether to delete the data, or to simply delete the packet.
	bool deleteData;
};

// Functions definitions
typedef void ( *CreateNPC_RPC_t)(CSAMPRPCParams *pRPCParams);
typedef int  ( *GetNetGame_t)();
typedef int  ( *GetConsole_t)();
typedef int  ( *GetRakServer_t)();

typedef void (THISCALL *CPlayerPool__DeletePlayer_t)(void *pPlayerPool, int iPlayerId, int iReason);
typedef void (THISCALL *CPlayer__SpawnForWorld_t)(void *pPlayer);
typedef void (THISCALL *CPlayer__Kill_t)(void *pPlayer, int iKillerId, int iWeapon);
typedef void (THISCALL *CPlayer__EnterVehicle_t)(void *pPlayer, int iVehicleId, int iSeatId);
typedef void (THISCALL *CPlayer__ExitVehicle_t)(void *pPlayer, int iVehicleId);
typedef int  (THISCALL *CConfig__GetValueAsInteger_t)(void *pConfig, char *szKey);
typedef void (THISCALL *RakServer__Send_t)(void *pRakServer, RakNet::BitStream* pBitStream, int iPriority, int iReliability, unsigned ucOrderingChannel, CSAMPPlayerId playerId, bool bBroadcast);
typedef bool (THISCALL *RakServer__RPC_t)(void* pRakServer, int* iUniqueID, RakNet::BitStream* pBitStream, int iPriority, int iReliability, unsigned ucOrderingChannel, CSAMPPlayerId playerId, bool bBroadcast, bool bShiftTimestamp);
typedef Packet* (THISCALL *RakServer__Receive_t)(void* pRakServer);

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
		static void		PlayerShoot(int iPlayerId, WORD iHitId, BYTE iHitType, BYTE iWeaponId, CVector3 vecPoint);
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
		static RakServer__Send_t				pfn__RakServer__Send;
		static RakServer__RPC_t				    pfn__RakServer__RPC;
		static RakServer__Receive_t				pfn__RakServer__Receive;
		static GetNetGame_t						pfn__GetNetGame;
		static GetConsole_t						pfn__GetConsole;
		static GetRakServer_t					pfn__GetRakServer;

};

#endif
