/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: Address.h
	- Author(s): OrMisicL

  =========================================*/


#ifndef ADDRESS_H
#define ADDRESS_H

class CAddress
{
	public:
		static void	Initialize();

		// Functions
		static DWORD			FUNC_CreateNPC_RPC;
		static DWORD			FUNC_CPlayerPool__DeletePlayer;

		static DWORD			FUNC_CPlayer__SpawnForWorld;
		static DWORD			FUNC_CPlayer__Kill;
		static DWORD			FUNC_CPlayer__EnterVehicle;
		static DWORD			FUNC_CPlayer__ExitVehicle;

		static DWORD			FUNC_CConfig__GetValueAsInteger;

		static DWORD			FUNC_GetVehicleModelInfo;

		static DWORD			FUNC_RakServer__Send;

		// Variables
		static DWORD			VAR_ServerPtr;
		static DWORD			VAR_ConfigPtr;
		static DWORD			VAR_RakPeerPtr;
		static DWORD			VAR_ServerAuthentication;

		// Offsets
		static DWORD			OFFSET_NetVersion;
		static DWORD			OFFSET_RemoteSystemManager;
		static DWORD			OFFSET_RemoteSystemSize;
		static DWORD			OFFSET_RemoteSystem__ConnectMode;
		static DWORD			OFFSET_RemoteSystem__Unknown;
		static BYTE				OFFSET_SendBullet_RPC;
		
		// Arrays


};

#endif