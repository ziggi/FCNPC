/* =========================================

		FCNPC - Fully Controllable NPC
			----------------------

	- File: Address.hpp
	- Author(s): OrMisicL

  =========================================*/


#ifndef ADDRESS_H
#define ADDRESS_H

#if defined(WIN32)
	#define RAKNET_SEND_OFFSET          7
	#define RAKNET_RPC_OFFSET           32
	#define RAKNET_RECEIVE_OFFSET       10
	#define RAKNET_GET_INDEX_FROM_PLAYERID_OFFSET 57
	#define RAKNET_GET_PLAYERID_FROM_INDEX_OFFSET 58
#elif defined(LINUX)
	#define RAKNET_SEND_OFFSET          9
	#define RAKNET_RPC_OFFSET           35
	#define RAKNET_RECEIVE_OFFSET       11
	#define RAKNET_GET_INDEX_FROM_PLAYERID_OFFSET 58
	#define RAKNET_GET_PLAYERID_FROM_INDEX_OFFSET 59
#endif

enum eSAMPVersion {
	SAMP_VERSION_UNKNOWN,
	SAMP_VERSION_037_R2,
	CRMP_VERSION_037_R2,
	SAMP_VERSION_03DL_R1,
};

class CAddress
{
public:
	static void	Initialize(eSAMPVersion sampVersion);
	static DWORD FindNetVersion();

	static DWORD           FUNC_Logprintf_037_R2;
	static DWORD           FUNC_Logprintf_03DL_R1;

	// Functions
	static DWORD           FUNC_CPlayerPool__DeletePlayer;
	static DWORD           FUNC_CPlayer__Kill;
	static DWORD           FUNC_CPlayer__EnterVehicle;
	static DWORD           FUNC_CPlayer__ExitVehicle;
	static DWORD           FUNC_CPlayer__SpawnForWorld;
	static DWORD           FUNC_GetVehicleModelInfo;
	static DWORD           FUNC_CConsole__GetIntVariable;
	static DWORD           FUNC_ClientJoin_RPC;

	// Variables
	static DWORD           VAR_ServerAuthentication;
	static DWORD           VAR_NetVersion;
#ifdef SAMP_03DL
	static DWORD           VAR_ArtInfo;
#endif

	// Offsets
	static DWORD           OFFSET_RemoteSystemManager;
	static DWORD           OFFSET_RemoteSystemSize;
	static DWORD           OFFSET_RemoteSystem__ConnectMode;
	static DWORD           OFFSET_RemoteSystem__Unknown;

	// Callbacks
	static DWORD           FUNC_CGameMode__OnPlayerGiveDamage;
	static DWORD           FUNC_CGameMode__OnPlayerTakeDamage;
	static DWORD           FUNC_CGameMode__OnPlayerWeaponShot;
	static DWORD           FUNC_CGameMode__OnPlayerStreamIn;
	static DWORD           FUNC_CGameMode__OnPlayerStreamOut;
	static DWORD           FUNC_CGameMode__OnGameModeExit;
};

#endif
