/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: Address.cpp
	- Author(s): OrMisicL

  =========================================*/


#include "Main.h"

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

// Offsets
DWORD CAddress::OFFSET_NetVersion = NULL;
DWORD CAddress::OFFSET_RemoteSystemManager = NULL;
DWORD CAddress::OFFSET_RemoteSystemSize = NULL;
DWORD CAddress::OFFSET_RemoteSystem__ConnectMode = NULL;
DWORD CAddress::OFFSET_RemoteSystem__Unknown = NULL;

void CAddress::Initialize()
{
#ifdef _WIN32
	// Functions
	FUNC_CPlayerPool__DeletePlayer = CUtils::FindPattern("\x8B\x44\x24\x10\x64\x89\x25\x00\x00\x00\x00\x81\xEC\x20\x01\x00\x00\x66\x3D\xE8\x03\x53", "xxxxxxxxxxxxxxxxxxxxxx") - 0xE;
	FUNC_CPlayer__Kill = CUtils::FindPattern("\xB6\x9C\x24\x38\x01\x00\x00\x66\x8B\xAC\x24\x3C", "xxxxxxxxxxxx") - 0x3F;
	FUNC_CPlayer__EnterVehicle = CUtils::FindPattern("\x8A\x9C\x24\x40\x01\x00\x00\x0F\xB7\xAC\x24\x3C\x01\x00\x00\x0F\xB7\x96\x79\x25\x00\x00", "xxxxxxxxxxxxxxxxxxxxx") - 0x2A;
	FUNC_CPlayer__ExitVehicle = CUtils::FindPattern("\x0F\xB7\x96\x79\x25\x00\x00\xA1\x0C\x4C\x4F\x00\x8B\x48\x04\x0F\xB7\xFB\x57\x52\xC7\x84\x24\x34", "xxxxxxxx????xxxxxxxxxxxx") - 0x30;
	FUNC_CPlayer__SpawnForWorld = CUtils::FindPattern("\x83\x3B\x00\x74\x08\x57\x8B\xCE", "xxxxxxxx") - 0x33;
	FUNC_GetVehicleModelInfo = CUtils::FindPattern("\x8B\x44\x24\x04\x3D\x90\x01\x00\x00\x0F\x8C\x83\x00\x00\x00\x3D\x63\x02\x00\x00\x7F\x7C", "xxxxxxxxxxxxxxxxxxxxxx");
	FUNC_CConfig__GetValueAsInteger = CUtils::FindPattern("\x85\xC0\x74\x0D\x83\x38\x01\x75\x08\x8B\x48\x08\x8B\x01", "xxxxxxxxxxxxxx") - 0x0A;
	FUNC_CreateNPC_RPC = CUtils::FindPattern("\x8B\x39\x85\xFF\x0F\x84\x4E\x04\x00\x00\x85\xC0\x0F\x84\x46\x04\x00\x00\x99\x83\xE2\x07\x03\xC2\x83\xC1\x08", "xxxxxxxxxxxxxxxxxxxxxxxxxxx") - 0x2A;
 	
	// Variables
	VAR_ServerAuthentication = *(DWORD *)(CUtils::FindPattern("\x8B\x4C\x24\x28\xA1\xF0\x6C\x4F\x00\x81\xF1\xD9\x0F\x00\x00\x3B\xC1", "xxxxx????xx????xx") + 5);
	VAR_ServerPtr = CSAMPFunctions::GetNetGame();
	VAR_ConfigPtr = CSAMPFunctions::GetConsole();
	VAR_RakPeerPtr = CSAMPFunctions::GetRakServer();

	// Offsets
	OFFSET_NetVersion = *(DWORD *)(CUtils::FindPattern("\x8B\x4C\x24\x28\xA1\xF0\x6C\x4F\x00\x81\xF1\xD9\x0F\x00\x00\x3B\xC1", "xxxxx????xx????xx") + 11);
	OFFSET_RemoteSystemManager = *(DWORD *)(CUtils::FindPattern("\x8B\x96\x3C\x03\x00\x00\x8A\x0C\x3A\x84\xC9\x8D\x04\x3A", "xx????xxxxxxxx") + 2); 
	OFFSET_RemoteSystemSize = *(DWORD *)(CUtils::FindPattern("\x0F\xB7\x56\x08\x43\x81\xC7\xB8\x0C\x00\x00\x3B\xDA", "xxxxxxx????xx") + 7);
	OFFSET_RemoteSystem__ConnectMode = *(DWORD *)(CUtils::FindPattern("\x83\xB8\xB0\x0C\x00\x00\x08\x75\x1A\x40\x8B\xD0\x8B\x44\x24\x04", "xx????xxxxxxxxxx") + 2);
	OFFSET_RemoteSystem__Unknown = *(DWORD *)(CUtils::FindPattern("\x80\xB8\xB5\x0C\x00\x00\x02\x0F\x85\xEE\x01\x00\x00\x8B\x7C\x24\x2C\x8B\xCF", "xx????xx??????xxxxx") + 2);
#else
	FUNC_CPlayerPool__DeletePlayer                  = 0x80CE070; 
	FUNC_CreateNPC_RPC                              = 0x80AE340; 
	FUNC_CPlayer__SpawnForWorld                     = 0x80CA960; 
	FUNC_CPlayer__Kill                              = 0x80C8E20; 
	FUNC_CPlayer__EnterVehicle                      = 0x80C9B60; 
	FUNC_CPlayer__ExitVehicle                       = 0x80C9CE0; 
	FUNC_CConfig__GetValueAsInteger                 = 0x809EEE0; 
	FUNC_GetVehicleModelInfo                        = 0x80D3220; 

	VAR_ServerAuthentication                        = 0x819C108; 
	VAR_ServerPtr = CSAMPFunctions::GetNetGame();
	VAR_ConfigPtr = CSAMPFunctions::GetConsole();
	VAR_RakPeerPtr = CSAMPFunctions::GetRakServer();
#endif
}