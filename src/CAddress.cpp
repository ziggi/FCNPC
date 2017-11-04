/* =========================================

		FCNPC - Fully Controllable NPC
			----------------------

	- File: Address.cpp
	- Author(s): OrMisicL

  =========================================*/


#include "Main.hpp"

#if defined(WIN32)
	DWORD CAddress::FUNC_Logprintf_037 = 0x0048A0B0;
	DWORD CAddress::FUNC_Logprintf_037_R2 = 0x0048C8D0;
#elif defined(LINUX)
	DWORD CAddress::FUNC_Logprintf_037 = 0x080A9000;
	DWORD CAddress::FUNC_Logprintf_037_R2 = 0x080A91D0;
#endif

// Functions
DWORD CAddress::FUNC_ClientJoin_RPC = 0;
DWORD CAddress::FUNC_CPlayerPool__DeletePlayer = 0;

DWORD CAddress::FUNC_CPlayer__SpawnForWorld = 0;
DWORD CAddress::FUNC_CPlayer__Kill = 0;
DWORD CAddress::FUNC_CPlayer__EnterVehicle = 0;
DWORD CAddress::FUNC_CPlayer__ExitVehicle = 0;

DWORD CAddress::FUNC_CConsole__GetIntVariable = 0;

DWORD CAddress::FUNC_GetVehicleModelInfo = 0;

// Variables
DWORD CAddress::VAR_ServerAuthentication = 0;
DWORD CAddress::VAR_NetVersion = 0;

// Offsets
DWORD CAddress::OFFSET_RemoteSystemManager = 0;
DWORD CAddress::OFFSET_RemoteSystemSize = 0;
DWORD CAddress::OFFSET_RemoteSystem__ConnectMode = 0;
DWORD CAddress::OFFSET_RemoteSystem__Unknown = 0;

void CAddress::Initialize(eSAMPVersion sampVersion)
{
#if defined(WIN32)
	switch (sampVersion) {
		case SAMP_VERSION_037:
			FUNC_CPlayerPool__DeletePlayer = 0x466740;
			FUNC_CPlayer__Kill = 0x4831f0;
			FUNC_CPlayer__EnterVehicle = 0x483840;
			FUNC_CPlayer__ExitVehicle = 0x483960;
			FUNC_CPlayer__SpawnForWorld = 0x485900;
			FUNC_GetVehicleModelInfo = 0x486e10;
			FUNC_CConsole__GetIntVariable = 0x488db0;
			FUNC_ClientJoin_RPC = 0x48f040;

			VAR_ServerAuthentication = 0x4f6cf0;
			VAR_NetVersion = 0xfd9;

			OFFSET_RemoteSystemManager = 0x33c;
			OFFSET_RemoteSystemSize = 0xcb8;
			OFFSET_RemoteSystem__ConnectMode = 0xcb0;
			OFFSET_RemoteSystem__Unknown = 0xcb5;
			break;

		case SAMP_VERSION_037_R2:
			FUNC_CPlayerPool__DeletePlayer = 0x466570;
			FUNC_CPlayer__Kill = 0x484620;
			FUNC_CPlayer__EnterVehicle = 0x484c70;
			FUNC_CPlayer__ExitVehicle = 0x484f50;
			FUNC_CPlayer__SpawnForWorld = 0x486d30;
			FUNC_GetVehicleModelInfo = 0x488240;
			FUNC_CConsole__GetIntVariable = 0x48b5b0;
			FUNC_ClientJoin_RPC = 0x4918f0;

			VAR_ServerAuthentication = 0x4f5fe8;
			VAR_NetVersion = 0xfd9;

			OFFSET_RemoteSystemManager = 0x33c;
			OFFSET_RemoteSystemSize = 0xcb8;
			OFFSET_RemoteSystem__ConnectMode = 0xcb0;
			OFFSET_RemoteSystem__Unknown = 0xcb5;
			break;

		case CRMP_VERSION_037_R2:
			FUNC_CPlayerPool__DeletePlayer = 0x466570;
			FUNC_CPlayer__Kill = 0x484620;
			FUNC_CPlayer__EnterVehicle = 0x484c70;
			FUNC_CPlayer__ExitVehicle = 0x484f50;
			FUNC_CPlayer__SpawnForWorld = 0x486d30;
			FUNC_GetVehicleModelInfo = 0x488240;
			FUNC_CConsole__GetIntVariable = 0x48b5b0;
			FUNC_ClientJoin_RPC = 0x4918f0;

			VAR_ServerAuthentication = 0x4f5fe8;
			VAR_NetVersion = 0x9999;

			OFFSET_RemoteSystemManager = 0x33c;
			OFFSET_RemoteSystemSize = 0xcb8;
			OFFSET_RemoteSystem__ConnectMode = 0xcb0;
			OFFSET_RemoteSystem__Unknown = 0xcb5;
			break;

		case SAMP_VERSION_UNKNOWN:
			// Functions
			FUNC_CPlayerPool__DeletePlayer = CUtils::FindPattern("\x8B\x44\x24\x10\x64\x89\x25\x00\x00\x00\x00\x81\xEC\x20\x01\x00\x00\x66\x3D\xE8\x03\x53", "xxxxxxxxxxxxxxxxxxxxxx") - 0xE;
			FUNC_CPlayer__Kill = CUtils::FindPattern("\xB6\x9C\x24\x38\x01\x00\x00\x66\x8B\xAC\x24\x3C", "xxxxxxxxxxxx") - 0x3F;
			FUNC_CPlayer__EnterVehicle = CUtils::FindPattern("\xEC\x1C\x01\x00\x00\x53\x55\x56\x8B\xF1\x57\x8D\x4C\x24\x18", "xxxxxxxxxxxxxxx") - 0x14;
			FUNC_CPlayer__ExitVehicle = CUtils::FindPattern("\xEC\x18\x01\x00\x00\x56\x8B\xF1\x57\x8B\xBC\x24\x30\x01\x00\x00\x8D\x4C\x24", "xxxxxxxxxxxxxxxxxxx") - 0x14;
			FUNC_CPlayer__SpawnForWorld = CUtils::FindPattern("\x83\x3B\x00\x74\x08\x57\x8B\xCE", "xxxxxxxx") - 0x33;
			FUNC_GetVehicleModelInfo = CUtils::FindPattern("\x8B\x44\x24\x04\x3D\x90\x01\x00\x00\x0F\x8C\x83\x00\x00\x00\x3D\x63\x02\x00\x00\x7F\x7C", "xxxxxxxxxxxxxxxxxxxxxx");
			FUNC_CConsole__GetIntVariable = CUtils::FindPattern("\x85\xC0\x74\x0D\x83\x38\x01\x75\x08\x8B\x48\x08\x8B\x01", "xxxxxxxxxxxxxx") - 0x0A;
			FUNC_ClientJoin_RPC = CUtils::FindPattern("\x83\xC1\x08\x8B\x11\x66\x8B\x49\x04\x89\x54\x24\x14\x99\x83\xE2\x07\x03\xC2\xC1\xF8\x03\x53\x40\x50\x66\x89", "xxxxxxxxxxxxxxxxxxxxxxxxxxx") - 0x3E;

			// Variables
			VAR_ServerAuthentication = *(DWORD *)(CUtils::FindPattern("\x8B\x4C\x24\x28\xA1\xE8\x5F\x4F\x00\x81\xF1\xD9\x0F\x00\x00\x3B\xC1", "xxxxx????xx????xx") + 5);
			VAR_NetVersion = *(DWORD *)(CUtils::FindPattern("\x8B\x4C\x24\x28\xA1\xE8\x5F\x4F\x00\x81\xF1\xD9\x0F\x00\x00\x3B\xC1", "xxxxx????xx????xx") + 11);

			// Offsets
			OFFSET_RemoteSystemManager = *(DWORD *)(CUtils::FindPattern("\x8B\x96\x3C\x03\x00\x00\x8A\x0C\x17\x84\xC9\x8D\x04\x17", "xx????xx?xxxx?") + 2);
			OFFSET_RemoteSystemSize = *(DWORD *)(CUtils::FindPattern("\x0F\xB7\x56\x08\x43\x81\xC7\xB8\x0C\x00\x00\x3B\xDA", "xxxxxxx????xx") + 7);
			OFFSET_RemoteSystem__ConnectMode = *(DWORD *)(CUtils::FindPattern("\x83\xB8\xB0\x0C\x00\x00\x08\x75\x1A\x40\x8B\xD0\x8B\x44\x24\x04", "xx????xxxxxxxxxx") + 2);
			OFFSET_RemoteSystem__Unknown = *(DWORD *)(CUtils::FindPattern("\x80\xB8\xB5\x0C\x00\x00\x02\x0F\x85\x4C\x01\x00\x00\x8B\x7C\x24\x3C\x8B\xCF", "xx????xx??????xx?xx") + 2);
			break;
	}
#elif defined(LINUX)
	switch (sampVersion) {
		case SAMP_VERSION_037:
			FUNC_CPlayerPool__DeletePlayer = 0x80d0600;
			FUNC_CPlayer__Kill = 0x80cadb0;
			FUNC_CPlayer__EnterVehicle = 0x80cbd50;
			FUNC_CPlayer__ExitVehicle = 0x80cbed0;
			FUNC_CPlayer__SpawnForWorld = 0x80ccb50;
			FUNC_GetVehicleModelInfo = 0x80d58a0;
			FUNC_CConsole__GetIntVariable = 0x80a0040;
			FUNC_ClientJoin_RPC = 0x80afcf0;

			VAR_ServerAuthentication = 0x81a06e8;
			VAR_NetVersion = 0xfd9;

			OFFSET_RemoteSystemManager = 0x334;
			OFFSET_RemoteSystemSize = 0xc69;
			OFFSET_RemoteSystem__ConnectMode = 0xc62;
			OFFSET_RemoteSystem__Unknown = 0xc67;
			break;

		case SAMP_VERSION_037_R2:
			FUNC_CPlayerPool__DeletePlayer = 0x80d0a90;
			FUNC_CPlayer__Kill = 0x80cb220;
			FUNC_CPlayer__EnterVehicle = 0x80cc1c0;
			FUNC_CPlayer__ExitVehicle = 0x80cc340;
			FUNC_CPlayer__SpawnForWorld = 0x80ccfc0;
			// FUNC_GetVehicleModelInfo = 0x80d5d30; // R2-2: 0x80d5e00
			FUNC_GetVehicleModelInfo = CUtils::FindPattern("\x55\x89\xE5\x57\x8B\x45\x08\x8B\x4D\x0C\x2D\x90\x01\x00\x00\x3D\xD3\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxx");
			FUNC_CConsole__GetIntVariable = 0x80a0070;
			FUNC_ClientJoin_RPC = 0x80b0030;

			// VAR_ServerAuthentication = 0x81aa8a8; // R2-2: 0x81ab8ec
			VAR_ServerAuthentication = *(DWORD *)(CUtils::FindPattern("\x8B\x85\x7C\xFC\xFF\xFF\x35", "xxxxxxx") + 13);
			VAR_NetVersion = 0xfd9;

			OFFSET_RemoteSystemManager = 0x334;
			OFFSET_RemoteSystemSize = 0xc69;
			OFFSET_RemoteSystem__ConnectMode = 0xc62;
			OFFSET_RemoteSystem__Unknown = 0xc67;
			break;

		case CRMP_VERSION_037_R2:
			FUNC_CPlayerPool__DeletePlayer = 0x80d0a90;
			FUNC_CPlayer__Kill = 0x80cb220;
			FUNC_CPlayer__EnterVehicle = 0x80cc1c0;
			FUNC_CPlayer__ExitVehicle = 0x80cc340;
			FUNC_CPlayer__SpawnForWorld = 0x80ccfc0;
			FUNC_GetVehicleModelInfo = 0x80d5d30;
			FUNC_CConsole__GetIntVariable = 0x80a0070;
			FUNC_ClientJoin_RPC = 0x80b0030;

			VAR_ServerAuthentication = 0x81aa8a8;
			VAR_NetVersion = 0x9999;

			OFFSET_RemoteSystemManager = 0x334;
			OFFSET_RemoteSystemSize = 0xc69;
			OFFSET_RemoteSystem__ConnectMode = 0xc62;
			OFFSET_RemoteSystem__Unknown = 0xc67;
			break;

		case SAMP_VERSION_UNKNOWN:
			// Functions
			FUNC_CPlayerPool__DeletePlayer = CUtils::FindPattern("\x55\x89\xE5\x81\xEC\x68\x01\x00\x00\x89\x5D\xF4\x0F\xB7\x5D\x0C\x0F\xB6\x45\x10", "xxxxxxxxxxxxxxxxxxx");
			FUNC_ClientJoin_RPC = CUtils::FindPattern("\x55\x89\xE5\x81\xEC\xC8\x03\x00\x00\x89\x5D\xF4\x8B\x4D\x08\x89\x75\xF8\x89\x7D\xFC", "xxxxxxxxxxxxxxxxxxxx");
			FUNC_CPlayer__SpawnForWorld = CUtils::FindPattern("\x55\x89\xE5\x56\x53\x83\xEC\x10\x8B\x75\x08\xA1\x14\x07\x1A\x08", "xxxxxxxxxxxx????");
			FUNC_CPlayer__Kill = CUtils::FindPattern("\x55\x89\xE5\x81\xEC\x68\x01\x00\x00\x0F\xB6\x45\x0C\x89\x7D\xFC\x8B\x7D\x08\x8D\x95\xC8\xFE\xFF\xFF", "xxxxxxxxxxxxxxxxxxxxxxxxx");
			FUNC_CPlayer__EnterVehicle = CUtils::FindPattern("\x55\x89\xE5\x81\xEC\x68\x01\x00\x00\x89\x5D\xF4\x8B\x45\x08\x0F\xB6\x5D\x10\x89\x75\xF8\x89\x7D\xFC", "xxxxxxxxxxxxxxxxxxxxxxxxx");
			FUNC_CPlayer__ExitVehicle = CUtils::FindPattern("\x55\x89\xE5\x81\xEC\x68\x01\x00\x00\x89\x5D\xF4\x89\x75\xF8\x8D\xB5\xC8\xFE\xFF\xFF\x0F\xB7\x5D\x0C\x89\x7D\xFC\x8B\x7D\x08\x89\x34\x24", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
			FUNC_CConsole__GetIntVariable = CUtils::FindPattern("\x55\x89\xE5\x83\xEC\x08\x8B\x45\x0C\x89\x44\x24\x04\x8B\x45\x08\x89\x04\x24\xE8\x68\xFA\xFF\xFF\x85\xC0\x74\x05\x83\x38\x01", "xxxxxxxxxxxxxxxxxxxx????xxx?xxx") + 0x30;
			FUNC_GetVehicleModelInfo = CUtils::FindPattern("\x55\x89\xE5\x57\x8B\x45\x08\x8B\x4D\x0C\x2D\x90\x01\x00\x00\x3D\xD3\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxx");

			// Variables
			VAR_ServerAuthentication = *(DWORD *)(CUtils::FindPattern("\x8B\x85\x7C\xFC\xFF\xFF\x35", "xxxxxxx") + 13);
			VAR_NetVersion = *(DWORD *)(CUtils::FindPattern("\x8B\x85\x7C\xFC\xFF\xFF\x35", "xxxxxxx") + 7);

			// Offsets
			OFFSET_RemoteSystemManager = *(DWORD *)(CUtils::FindPattern("\x8B\x87\x34\x03\x00\x00\x80\x3C\x03\x00\x74\xE6\x8D\x55\x0C\x8D\x44\x18\x01\x89\x54\x24\x04", "xx????xxxxx?xxxxxxxxxxx") + 2);
			OFFSET_RemoteSystemSize = *(DWORD *)(CUtils::FindPattern("\x46\x0F\xB7\xC2\x81\xC3\x69\x0C\x00\x00\x39\xF0", "xxxxx????xxx") + 6);
			OFFSET_RemoteSystem__ConnectMode = *(DWORD *)(CUtils::FindPattern("\x83\xBC\x18\x62\x0C\x00\x00\x08\x75\xD1\x01\xD8\x8B\x50\x01\x89\x11", "xxx????xx?xxxxxxx") + 3);
			OFFSET_RemoteSystem__Unknown = *(DWORD *)(CUtils::FindPattern("\x80\xB8\x67\x0C\x00\x00\x02\x74\x1D\x8B\x45\xD8\x89\x04\x24", "xxxxxx?x?xxxxxx") + 2);
			break;
	}
#endif
/*
	logprintf("\nFunctions:");
	logprintf("  FUNC_CPlayerPool__DeletePlayer: 0x%x", CAddress::FUNC_CPlayerPool__DeletePlayer);
	logprintf("  FUNC_CPlayer__Kill: 0x%x", CAddress::FUNC_CPlayer__Kill);
	logprintf("  FUNC_CPlayer__EnterVehicle: 0x%x", CAddress::FUNC_CPlayer__EnterVehicle);
	logprintf("  FUNC_CPlayer__ExitVehicle: 0x%x", CAddress::FUNC_CPlayer__ExitVehicle);
	logprintf("  FUNC_CPlayer__SpawnForWorld: 0x%x", CAddress::FUNC_CPlayer__SpawnForWorld);
	logprintf("  FUNC_GetVehicleModelInfo: 0x%x", CAddress::FUNC_GetVehicleModelInfo);
	logprintf("  FUNC_CConsole__GetIntVariable: 0x%x", CAddress::FUNC_CConsole__GetIntVariable);
	logprintf("  FUNC_ClientJoin_RPC: 0x%x", CAddress::FUNC_ClientJoin_RPC);

	logprintf("\nPointers:");
	logprintf("  VAR_ServerAuthentication: 0x%x", CAddress::VAR_ServerAuthentication);
	logprintf("  VAR_NetVersion: 0x%x", CAddress::VAR_NetVersion);

	logprintf("\nOffsets:");
	logprintf("  OFFSET_RemoteSystemManager: 0x%x", CAddress::OFFSET_RemoteSystemManager);
	logprintf("  OFFSET_RemoteSystemSize: 0x%x", CAddress::OFFSET_RemoteSystemSize);
	logprintf("  OFFSET_RemoteSystem__ConnectMode: 0x%x", CAddress::OFFSET_RemoteSystem__ConnectMode);
	logprintf("  OFFSET_RemoteSystem__Unknown: 0x%x", CAddress::OFFSET_RemoteSystem__Unknown);
*/
}

DWORD CAddress::FindNetVersion()
{
	DWORD dwAddress;
#if defined(WIN32)
	dwAddress = CUtils::FindPattern("\x8B\x4C\x24\x28\xA1\xE8\x5F\x4F\x00\x81\xF1\xD9\x0F\x00\x00\x3B\xC1", "xxxxx????xx????xx");
	if (!dwAddress) {
		return 0;
	}
	return *(DWORD *)(dwAddress + 11);
#elif defined(LINUX)
	dwAddress = CUtils::FindPattern("\x8B\x85\x7C\xFC\xFF\xFF\x35", "xxxxxxx");
	if (!dwAddress) {
		return 0;
	}
	return *(DWORD *)(dwAddress + 7);
#endif
}
