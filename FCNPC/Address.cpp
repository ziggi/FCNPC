/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: Address.cpp
	- Author(s): OrMisicL

  =========================================*/


#include "Main.h"

// Callbacks
DWORD CAddress::CALLBACK_CFilterScriptPool__OnPlayerGiveDamage = NULL;

// Functions
DWORD CAddress::FUNC_CreateNPC_RPC = NULL;
DWORD CAddress::FUNC_CPlayerPool__DeletePlayer = NULL;

DWORD CAddress::FUNC_CPlayer__SpawnForWorld = NULL;
DWORD CAddress::FUNC_CPlayer__Kill = NULL;
DWORD CAddress::FUNC_CPlayer__EnterVehicle = NULL;
DWORD CAddress::FUNC_CPlayer__ExitVehicle = NULL;

DWORD CAddress::FUNC_CConfig__GetValueAsInteger = NULL;

DWORD CAddress::FUNC_GetVehicleModelInfo = NULL;

// Variables
DWORD CAddress::VAR_ServerPtr = NULL;
DWORD CAddress::VAR_ConfigPtr = NULL;
DWORD CAddress::VAR_RakPeerPtr = NULL;
DWORD CAddress::VAR_ServerAuthentication = NULL;

void CAddress::Initialize(eSAMPVersion sampVersion)
{
	switch(sampVersion)
	{
		case SAMP_VERSION_03Z_R2:
		{
#ifdef _WIN32
			// Credits to therainycat
			CALLBACK_CFilterScriptPool__OnPlayerGiveDamage  =   0x48E4BF;

			FUNC_CPlayerPool__DeletePlayer					=	0x4660F0;  
			FUNC_CPlayer__Kill								=	0x480CE0;
			FUNC_CPlayer__EnterVehicle						=	0x481270; 
			FUNC_CPlayer__ExitVehicle						=	0x481390; 
			FUNC_CPlayer__SpawnForWorld						=	0x4832F0; 
			FUNC_GetVehicleModelInfo						=	0x484800; 
			FUNC_CConfig__GetValueAsInteger					=	0x486020; 
			FUNC_CreateNPC_RPC								=	0x48C150; 
 		
			VAR_ServerAuthentication						=	0x4F2380;
#else
			// Credits to Lorenc_
			CALLBACK_CFilterScriptPool__OnPlayerGiveDamage  = 0x80B1343; 

			FUNC_CPlayerPool__DeletePlayer                  = 0x80CE070; 
			FUNC_CreateNPC_RPC                              = 0x80AE340; 
			FUNC_CPlayer__SpawnForWorld                     = 0x80CA960; 
			FUNC_CPlayer__Kill                              = 0x80C8E20; 
			FUNC_CPlayer__EnterVehicle                      = 0x80C9B60; 
			FUNC_CPlayer__ExitVehicle                       = 0x80C9CE0; 
			FUNC_CConfig__GetValueAsInteger                 = 0x809EEE0; 
			FUNC_GetVehicleModelInfo                        = 0x80D3220; 

			VAR_ServerAuthentication                        = 0x819C108; 
#endif
			break;
		}
#ifndef _WIN32
		case SAMP_VERSION_03Z_R2_1000P:
		{
			CALLBACK_CFilterScriptPool__OnPlayerGiveDamage  = 0x80B1343; 

			FUNC_CPlayerPool__DeletePlayer                  = 0x80CE070; 
			FUNC_CreateNPC_RPC                              = 0x80AE340; 
			FUNC_CPlayer__SpawnForWorld                     = 0x80CA960; 
			FUNC_CPlayer__Kill                              = 0x80C8E20; 
			FUNC_CPlayer__EnterVehicle                      = 0x80C9B60; 
			FUNC_CPlayer__ExitVehicle                       = 0x80C9CE0; 
			FUNC_CConfig__GetValueAsInteger                 = 0x809EEE0; 
			FUNC_GetVehicleModelInfo                        = 0x80D3220; 

			VAR_ServerAuthentication                        = 0x819B5A8; 
			break;
		}
#endif
	}
	VAR_ServerPtr									=	CSAMPFunctions::GetNetGame(); 
	VAR_ConfigPtr									=	CSAMPFunctions::GetConsole();
	VAR_RakPeerPtr                                  =   CSAMPFunctions::GetRakServer();
}