/* =========================================

		FCNPC - Fully Controllable NPC
			----------------------

	- File: Address.cpp
	- Author(s): OrMisicL

  =========================================*/


#include "Main.hpp"

#if defined(WIN32)
DWORD CAddress::FUNC_Logprintf_037_R2 = 0x0048C8D0;
DWORD CAddress::FUNC_Logprintf_03DL_R1 = 0x00491fa0;
#elif defined(LINUX)
DWORD CAddress::FUNC_Logprintf_037_R2 = 0x080A91D0;
DWORD CAddress::FUNC_Logprintf_03DL_R1 = 0x080b1ca0;
#endif

// Functions

DWORD CAddress::FUNC_CPlayerPool__DeletePlayer = 0;
DWORD CAddress::FUNC_CPlayer__Kill = 0;
DWORD CAddress::FUNC_CPlayer__EnterVehicle = 0;
DWORD CAddress::FUNC_CPlayer__ExitVehicle = 0;
DWORD CAddress::FUNC_CPlayer__SpawnForWorld = 0;
DWORD CAddress::FUNC_GetVehicleModelInfo = 0;
DWORD CAddress::FUNC_CConsole__GetIntVariable = 0;
DWORD CAddress::FUNC_ClientJoin_RPC = 0;

// Variables
DWORD CAddress::VAR_ServerAuthentication = 0;
DWORD CAddress::VAR_NetVersion = 0;
#ifdef SAMP_03DL
DWORD CAddress::VAR_ArtInfo = 0;
#endif

// Offsets
DWORD CAddress::OFFSET_RemoteSystemManager = 0;
DWORD CAddress::OFFSET_RemoteSystemSize = 0;
DWORD CAddress::OFFSET_RemoteSystem__ConnectMode = 0;
DWORD CAddress::OFFSET_RemoteSystem__Unknown = 0;

// Callbacks
DWORD CAddress::FUNC_CGameMode__OnPlayerGiveDamage = 0;
DWORD CAddress::FUNC_CGameMode__OnPlayerTakeDamage = 0;
DWORD CAddress::FUNC_CGameMode__OnPlayerWeaponShot = 0;
DWORD CAddress::FUNC_CGameMode__OnPlayerStreamIn = 0;
DWORD CAddress::FUNC_CGameMode__OnPlayerStreamOut = 0;
DWORD CAddress::FUNC_CGameMode__OnGameModeExit = 0;

void CAddress::Initialize(eSAMPVersion sampVersion)
{
#if defined(WIN32)
	switch (sampVersion) {
		case SAMP_VERSION_037_R2:
			FUNC_CPlayerPool__DeletePlayer = 0x466570;
			FUNC_CPlayer__Kill = 0x484620;
			FUNC_CPlayer__EnterVehicle = 0x484c70;
			FUNC_CPlayer__ExitVehicle = 0x484d90;
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

			FUNC_CGameMode__OnPlayerGiveDamage = 0x46ed70;
			FUNC_CGameMode__OnPlayerTakeDamage = 0x46ecf0;
			FUNC_CGameMode__OnPlayerWeaponShot = 0x46f360;
			FUNC_CGameMode__OnPlayerStreamIn = 0x46e8e0;
			FUNC_CGameMode__OnPlayerStreamOut = 0x46e950;
			FUNC_CGameMode__OnGameModeExit = 0x46adc0;
			break;

		case CRMP_VERSION_037_R2:
			FUNC_CPlayerPool__DeletePlayer = 0x466570;
			FUNC_CPlayer__Kill = 0x484620;
			FUNC_CPlayer__EnterVehicle = 0x484c70;
			FUNC_CPlayer__ExitVehicle = 0x484d90;
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

			FUNC_CGameMode__OnPlayerGiveDamage = 0x46ed70;
			FUNC_CGameMode__OnPlayerTakeDamage = 0x46ecf0;
			FUNC_CGameMode__OnPlayerWeaponShot = 0x46f360;
			FUNC_CGameMode__OnPlayerStreamIn = 0x46e8e0;
			FUNC_CGameMode__OnPlayerStreamOut = 0x46e950;
			FUNC_CGameMode__OnGameModeExit = 0x46adc0;
			break;

		case SAMP_VERSION_03DL_R1:
			FUNC_CPlayerPool__DeletePlayer = 0x466550;
			FUNC_CPlayer__Kill = 0x485020;
			FUNC_CPlayer__EnterVehicle = 0x485670;
			FUNC_CPlayer__ExitVehicle = 0x485790;
			FUNC_CPlayer__SpawnForWorld = 0x487730;
			FUNC_GetVehicleModelInfo = 0x4892a0;
			FUNC_CConsole__GetIntVariable = 0x490c80;
			FUNC_ClientJoin_RPC = 0x497ef0;

			VAR_ServerAuthentication = 0x4fc470;
			VAR_NetVersion = 0xfde;
#ifdef SAMP_03DL
			VAR_ArtInfo = 0x4fc444;
#endif

			OFFSET_RemoteSystemManager = 0x33c;
			OFFSET_RemoteSystemSize = 0xcb8;
			OFFSET_RemoteSystem__ConnectMode = 0xcb0;
			OFFSET_RemoteSystem__Unknown = 0xcb5;

			// callbacks
			FUNC_CGameMode__OnPlayerGiveDamage = 0x46f050;
			FUNC_CGameMode__OnPlayerTakeDamage = 0x46efd0;
			FUNC_CGameMode__OnPlayerWeaponShot = 0x46f640;
			FUNC_CGameMode__OnPlayerStreamIn = 0x46ebc0;
			FUNC_CGameMode__OnPlayerStreamOut = 0x46ec30;
			FUNC_CGameMode__OnGameModeExit = 0x46da90;
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
#ifdef SAMP_03DL
			VAR_ArtInfo = *(DWORD *)(CUtils::FindPattern("\xA3\x44\xC4\x4F\x00\xE8\x80\x4A\x00\x00\x68\xD8\x3E\x00\x00\xE8\xC7\xE6\x00\x00", "xxxxxxxxxxxxxxxxxxxx") + 1);
#endif

			// Offsets
			OFFSET_RemoteSystemManager = *(DWORD *)(CUtils::FindPattern("\x8B\x96\x3C\x03\x00\x00\x8A\x0C\x17\x84\xC9\x8D\x04\x17", "xx????xx?xxxx?") + 2);
			OFFSET_RemoteSystemSize = *(DWORD *)(CUtils::FindPattern("\x0F\xB7\x56\x08\x43\x81\xC7\xB8\x0C\x00\x00\x3B\xDA", "xxxxxxx????xx") + 7);
			OFFSET_RemoteSystem__ConnectMode = *(DWORD *)(CUtils::FindPattern("\x83\xB8\xB0\x0C\x00\x00\x08\x75\x1A\x40\x8B\xD0\x8B\x44\x24\x04", "xx????xxxxxxxxxx") + 2);
			OFFSET_RemoteSystem__Unknown = *(DWORD *)(CUtils::FindPattern("\x80\xB8\xB5\x0C\x00\x00\x02\x0F\x85\x4C\x01\x00\x00\x8B\x7C\x24\x3C\x8B\xCF", "xx????xx??????xx?xx") + 2);

			// callbacks
			FUNC_CGameMode__OnPlayerGiveDamage = CUtils::FindPattern("\x83\xEC\x08\x56\x8D\x44\x24\x08\x50\x8B\xF1\x68\x9C\xBE\x4B\x00", "xxxxxxxxxxxxxxxx");
			FUNC_CGameMode__OnPlayerTakeDamage = CUtils::FindPattern("\x83\xEC\x08\x56\x8D\x44\x24\x08\x50\x8B\xF1\x68\x88\x5E\x4B\x00", "xxxxxxxxxxxxx?xx");
			FUNC_CGameMode__OnPlayerWeaponShot = CUtils::FindPattern("\x51\x8B\x44\x24\x18\x53\x8B\x58\x04\x55\x8B\x68\x08\x56\x57\x8B\x38\x8D\x44\x24\x28\x50\x8B\xF1\x68\xA8\x5F\x4B\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxx?xx");
			FUNC_CGameMode__OnPlayerStreamIn = CUtils::FindPattern("\x83\xEC\x08\x56\x8B\xF1\x8A\x46\x68\x84\xC0\x75\x09\x33\xC0\x5E\x83\xC4\x08\xC2\x08\x00\x8D\x44\x24\x08\x50\x68\xEC\x5D\x4B\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxx?xx");
			FUNC_CGameMode__OnPlayerStreamOut = CUtils::FindPattern("\x83\xEC\x08\x56\x8B\xF1\x8A\x46\x68\x84\xC0\x75\x09\x33\xC0\x5E\x83\xC4\x08\xC2\x08\x00\x8D\x44\x24\x08\x50\x68\x00\x5E\x4B\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxx?xx");
			FUNC_CGameMode__OnGameModeExit = CUtils::FindPattern("\x51\x53\x56\x8D\x44\x24\x08\x50\x8B\xF1\x68\x78\xBB\x4B\x00", "xxxxxxxxxxxx?xx");
			break;
	}
#elif defined(LINUX)
	switch (sampVersion) {
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

			// callbacks
			FUNC_CGameMode__OnPlayerGiveDamage = 0x80A69C0;
			FUNC_CGameMode__OnPlayerTakeDamage = 0x80A6910;
			FUNC_CGameMode__OnPlayerWeaponShot = 0x80A72A0;
			FUNC_CGameMode__OnPlayerStreamIn = 0x80A6450;
			FUNC_CGameMode__OnPlayerStreamOut = 0x80A64D0;
			FUNC_CGameMode__OnGameModeExit = 0x80A4DB0;
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

			// callbacks
			FUNC_CGameMode__OnPlayerGiveDamage = 0x80A69C0;
			FUNC_CGameMode__OnPlayerTakeDamage = 0x80A6910;
			FUNC_CGameMode__OnPlayerWeaponShot = 0x80A72A0;
			FUNC_CGameMode__OnPlayerStreamIn = 0x80A6450;
			FUNC_CGameMode__OnPlayerStreamOut = 0x80A64D0;
			FUNC_CGameMode__OnGameModeExit = 0x80A4DB0;
			break;

		case SAMP_VERSION_03DL_R1:
			FUNC_CPlayerPool__DeletePlayer = 0x80dc0b0;
			FUNC_CPlayer__Kill = 0x80d5ac0;
			FUNC_CPlayer__EnterVehicle = 0x80d6a70;
			FUNC_CPlayer__ExitVehicle = 0x80d6bf0;
			FUNC_CPlayer__SpawnForWorld = 0x80d7870;
			FUNC_GetVehicleModelInfo = 0x80e14f0;
			FUNC_CConsole__GetIntVariable = 0x80a87d0;
			FUNC_ClientJoin_RPC = 0x80ba350;

			VAR_ServerAuthentication = 0x81b7d2c;
			VAR_NetVersion = 0xfde;
#ifdef SAMP_03DL
			VAR_ArtInfo = 0x81d7934;
#endif

			OFFSET_RemoteSystemManager = 0x334;
			OFFSET_RemoteSystemSize = 0xc69;
			OFFSET_RemoteSystem__ConnectMode = 0xc62;
			OFFSET_RemoteSystem__Unknown = 0xc67;

			// callbacks
			FUNC_CGameMode__OnPlayerGiveDamage = 0x80AF380;
			FUNC_CGameMode__OnPlayerTakeDamage = 0x80AF2D0;
			FUNC_CGameMode__OnPlayerWeaponShot = 0x80AFC60;
			FUNC_CGameMode__OnPlayerStreamIn = 0x80AEE10;
			FUNC_CGameMode__OnPlayerStreamOut = 0x80AEE90;
			FUNC_CGameMode__OnGameModeExit = 0x80AD770;
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
#ifdef SAMP_03DL
			VAR_ArtInfo = *(DWORD *)(CUtils::FindPattern("\x89\x3D\x34\x79\x1D\x08\xB8\xCD\x5D\x16\x08\x89\x44\x24\x04\x89\x3C\x24", "xxxxxxxxxxxxxxxxxx") + 1);
#endif

			// Offsets
			OFFSET_RemoteSystemManager = *(DWORD *)(CUtils::FindPattern("\x8B\x87\x34\x03\x00\x00\x80\x3C\x03\x00\x74\xE6\x8D\x55\x0C\x8D\x44\x18\x01\x89\x54\x24\x04", "xx????xxxxx?xxxxxxxxxxx") + 2);
			OFFSET_RemoteSystemSize = *(DWORD *)(CUtils::FindPattern("\x46\x0F\xB7\xC2\x81\xC3\x69\x0C\x00\x00\x39\xF0", "xxxxx????xxx") + 6);
			OFFSET_RemoteSystem__ConnectMode = *(DWORD *)(CUtils::FindPattern("\x83\xBC\x18\x62\x0C\x00\x00\x08\x75\xD1\x01\xD8\x8B\x50\x01\x89\x11", "xxx????xx?xxxxxxx") + 3);
			OFFSET_RemoteSystem__Unknown = *(DWORD *)(CUtils::FindPattern("\x80\xB8\x67\x0C\x00\x00\x02\x74\x1D\x8B\x45\xD8\x89\x04\x24", "xxxxxx?x?xxxxxx") + 2);

			// callbacks
			// TODO not working
			FUNC_CGameMode__OnPlayerGiveDamage = CUtils::FindPattern("\x55\x89\xE5\x8D\x45\xF4\x56\x53\x83\xEC\x20\x8B\x5D\x08\xC7\x45\xF0\x00\x00\x00\x00\x8B\x75\x14\x89\x44\x24\x08\xB8\x5F\x98\x15", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
			FUNC_CGameMode__OnPlayerTakeDamage = CUtils::FindPattern("\x55\x89\xE5\x8D\x45\xF4\x56\x53\x83\xEC\x20\x8B\x5D\x08\xC7\x45\xF0\x00\x00\x00\x00\x8B\x75\x14\x89\x44\x24\x08\xB8\x4C\x98\x15", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
			FUNC_CGameMode__OnPlayerWeaponShot = CUtils::FindPattern("\x55\x89\xE5\x83\xEC\x28\x89\x5D\xF4\x8B\x45\x1C\x8B\x5D\x08\x89\x75\xF8\x89\x7D\xFC\xC7\x45\xEC\x01\x00\x00\x00\x8B\x10\x8B\x78", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
			FUNC_CGameMode__OnPlayerStreamIn = CUtils::FindPattern("\x55\x31\xC0\x89\xE5\x53\x83\xEC\x14\x8B\x5D\x08\x80\x7B\x68\x00\x75\x06\x83\xC4\x14\x5B\x5D\xC3\xC7\x45\xF4\x01\x00\x00\x00\x8D\x45\xF8\x89", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
			FUNC_CGameMode__OnPlayerStreamOut = CUtils::FindPattern("\x55\x31\xC0\x89\xE5\x53\x83\xEC\x14\x8B\x5D\x08\x80\x7B\x68\x00\x75\x06\x83\xC4\x14\x5B\x5D\xC3\xC7\x45\xF4\x01\x00\x00\x00\x8D\x45\xF8\xBA", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
			FUNC_CGameMode__OnGameModeExit = CUtils::FindPattern("\x55\xB8\x62\x95\x15\x08\x89\xE5\x56\x8D\x75\xF4\x53\x83\xEC\x10\x8B\x5D\x08\x89\x74\x24\x08\x89\x44\x24\x04\x89\x1C\x24\xE8\xDD", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
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
#ifdef SAMP_03DL
	logprintf("  VAR_ArtInfo: 0x%x", CAddress::VAR_ArtInfo);
#endif

	logprintf("\nOffsets:");
	logprintf("  OFFSET_RemoteSystemManager: 0x%x", CAddress::OFFSET_RemoteSystemManager);
	logprintf("  OFFSET_RemoteSystemSize: 0x%x", CAddress::OFFSET_RemoteSystemSize);
	logprintf("  OFFSET_RemoteSystem__ConnectMode: 0x%x", CAddress::OFFSET_RemoteSystem__ConnectMode);
	logprintf("  OFFSET_RemoteSystem__Unknown: 0x%x", CAddress::OFFSET_RemoteSystem__Unknown);

	logprintf("\nCallbacks:");
	logprintf("  FUNC_CGameMode__OnPlayerGiveDamage: 0x%x", CAddress::FUNC_CGameMode__OnPlayerGiveDamage);
	logprintf("  FUNC_CGameMode__OnPlayerTakeDamage: 0x%x", CAddress::FUNC_CGameMode__OnPlayerTakeDamage);
	logprintf("  FUNC_CGameMode__OnPlayerWeaponShot: 0x%x", CAddress::FUNC_CGameMode__OnPlayerWeaponShot);
	logprintf("  FUNC_CGameMode__OnPlayerStreamIn: 0x%x", CAddress::FUNC_CGameMode__OnPlayerStreamIn);
	logprintf("  FUNC_CGameMode__OnPlayerStreamOut: 0x%x", CAddress::FUNC_CGameMode__OnPlayerStreamOut);
	logprintf("  FUNC_CGameMode__OnGameModeExit: 0x%x", CAddress::FUNC_CGameMode__OnGameModeExit);
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
