/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: Address.h
	- Author(s): OrMisicL

  =========================================*/


#ifndef ADDRESS_H
#define ADDRESS_H

enum eSAMPVersion
{
	SAMP_VERSION_UNKNOWN,
	SAMP_VERSION_03Z_R2,
#ifndef _WIN32
	SAMP_VERSION_03Z_R2_1000P,
#endif
};

class CAddress
{
	public:
		static void	Initialize(eSAMPVersion sampVersion);
		
		// Callbacks
		static DWORD			CALLBACK_CFilterScriptPool__OnPlayerGiveDamage;

		// Functions
		static DWORD			FUNC_CreateNPC_RPC;
		static DWORD			FUNC_CPlayerPool__DeletePlayer;

		static DWORD			FUNC_CPlayer__SpawnForWorld;
		static DWORD			FUNC_CPlayer__Kill;
		static DWORD			FUNC_CPlayer__EnterVehicle;
		static DWORD			FUNC_CPlayer__ExitVehicle;

		static DWORD			FUNC_CConfig__GetValueAsInteger;

		static DWORD			FUNC_GetVehicleModelInfo;

		// Variables
		static DWORD			VAR_ServerPtr;
		static DWORD			VAR_ConfigPtr;
		static DWORD			VAR_RakPeerPtr;
		static DWORD			VAR_ServerAuthentication;
		
		// Arrays


};

#endif