/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: SAMPFunctions.cpp
	- Author(s): OrMisicL

  =========================================*/


#include "Main.h"

extern CServer			*pServer;
extern CSAMPRPCParams	*pCreateNPCParams;
extern void				**ppPluginData;
extern logprintf_t			logprintf;

// Functions
CreateNPC_RPC_t					CSAMPFunctions::pfn__CreateNPC_RPC = NULL;
CPlayerPool__DeletePlayer_t		CSAMPFunctions::pfn__CPlayerPool__DeletePlayer = NULL;
CPlayer__SpawnForWorld_t		CSAMPFunctions::pfn__CPlayer__SpawnForWorld = NULL;
CPlayer__Kill_t					CSAMPFunctions::pfn__CPlayer__Kill = NULL;
CPlayer__EnterVehicle_t			CSAMPFunctions::pfn__CPlayer__EnterVehicle = NULL;
CPlayer__ExitVehicle_t			CSAMPFunctions::pfn__CPlayer__ExitVehicle = NULL;
CConfig__GetValueAsInteger_t	CSAMPFunctions::pfn__CConfig__GetValueAsInteger = NULL;
GetVehicleModelInfo_t			CSAMPFunctions::pfn__GetVehicleModelInfo = NULL;
GetNetGame_t					CSAMPFunctions::pfn__GetNetGame = NULL;
GetConsole_t					CSAMPFunctions::pfn__GetConsole = NULL;
GetRakServer_t					CSAMPFunctions::pfn__GetRakServer = NULL;

void CSAMPFunctions::Initialize()
{
	// Initialize function pointers
	pfn__CreateNPC_RPC = (CreateNPC_RPC_t)(CAddress::FUNC_CreateNPC_RPC);
	pfn__CPlayerPool__DeletePlayer = (CPlayerPool__DeletePlayer_t)(CAddress::FUNC_CPlayerPool__DeletePlayer);
	
	pfn__CPlayer__SpawnForWorld = (CPlayer__SpawnForWorld_t)(CAddress::FUNC_CPlayer__SpawnForWorld);
	pfn__CPlayer__Kill = (CPlayer__Kill_t)(CAddress::FUNC_CPlayer__Kill);
	pfn__CPlayer__EnterVehicle = (CPlayer__EnterVehicle_t)(CAddress::FUNC_CPlayer__EnterVehicle);
	pfn__CPlayer__ExitVehicle = (CPlayer__ExitVehicle_t)(CAddress::FUNC_CPlayer__ExitVehicle);

	pfn__CConfig__GetValueAsInteger = (CConfig__GetValueAsInteger_t)(CAddress::FUNC_CConfig__GetValueAsInteger);

	pfn__GetVehicleModelInfo = (GetVehicleModelInfo_t)(CAddress::FUNC_GetVehicleModelInfo);
}

void CSAMPFunctions::PreInitialize()
{
	pfn__GetNetGame = (GetNetGame_t)(ppPluginData[PLUGIN_DATA_NETGAME]);
	pfn__GetConsole = (GetConsole_t)(ppPluginData[PLUGIN_DATA_CONFIG]);
	pfn__GetRakServer = (GetRakServer_t)(ppPluginData[PLUGIN_DATA_RAKPEER]);
}

int CSAMPFunctions::GetFreePlayerSlot()
{
	// Get the playerpool interface
	CSAMPServer *pSAMPServer = (CSAMPServer *)CAddress::VAR_ServerPtr;
	CSAMPPlayerPool *pPlayerPool = pSAMPServer->pPlayerPool;
	// Loop through all the players
	for(int i = (GetMaxPlayers() - 1); i != 0; i--)
	{
		// Is he not connected ?
		if(!pPlayerPool->bIsPlayerConnected[i])
			return i;
	}
	return INVALID_ENTITY_ID;
}

int CSAMPFunctions::NewPlayer(char *szName)
{
	// Get a free player slot
	int iPlayerId;
	if((iPlayerId = GetFreePlayerSlot()) == INVALID_ENTITY_ID)
		return INVALID_ENTITY_ID;

	// Get the SAMP authentication
	int iAuthentication = *(int *)(CAddress::VAR_ServerAuthentication) ^ SAMP_NETVERSION;
	// Set the RPC parameters writing pointer
	pCreateNPCParams->SetWritePointer(5);
	// Write to the RPC params
	pCreateNPCParams->Write<BYTE>(strlen(szName));
	pCreateNPCParams->WriteString(szName, strlen(szName));
	pCreateNPCParams->Write<int>(iAuthentication);
	// Get the RakPeer pointer
	CSAMPRakPeer *pRakPeer = (CSAMPRakPeer *)(CAddress::VAR_RakPeerPtr);
	// Create a fake player system address
	CSAMPSystemAddress systemAddress;
	systemAddress.uiSystemAddress = 0x0100007F; // Localhost
	systemAddress.usPort = rand() % 9999; // Random port
	systemAddress.usPlayerId = iPlayerId;
	// Set the RPC params system address
	pCreateNPCParams->SetSystemAddress(systemAddress);
	// Set a fake connected player in the RakPeer instance
	pRakPeer->SetConnectedPlayer(systemAddress);
	// Create the NPC
	pfn__CreateNPC_RPC(pCreateNPCParams);
	// Remove the fake player from the RakPeer instance
	pRakPeer->SetDisonnectedPlayer(iPlayerId);
	// Return the player id
	return iPlayerId;
}

void CSAMPFunctions::DeletePlayer(int iPlayerId)
{
	// Call the function
	CSAMPServer *pSAMPServer = (CSAMPServer *)CAddress::VAR_ServerPtr;
	pfn__CPlayerPool__DeletePlayer(pSAMPServer->pPlayerPool, iPlayerId, 0);
}

void CSAMPFunctions::SpawnPlayer(int iPlayerId)
{
	// Get the player pointer
	CSAMPServer *pSAMPServer = (CSAMPServer *)CAddress::VAR_ServerPtr;
	// Call the function
	pfn__CPlayer__SpawnForWorld(pSAMPServer->pPlayerPool->pPlayer[iPlayerId]);	
}

void CSAMPFunctions::KillPlayer(int iPlayerId, int iKillerId, int iWeapon)
{
	// Get the player pointer
	CSAMPServer *pSAMPServer = (CSAMPServer *)CAddress::VAR_ServerPtr;
	// Call the function
	pfn__CPlayer__Kill(pSAMPServer->pPlayerPool->pPlayer[iPlayerId], iKillerId, iWeapon);	
}

void CSAMPFunctions::PlayerEnterVehicle(int iPlayerId, int iVehicleId, int iSeatId)
{
	// Get the player pointer
	CSAMPServer *pSAMPServer = (CSAMPServer *)CAddress::VAR_ServerPtr;
	// Call the function
	pfn__CPlayer__EnterVehicle(pSAMPServer->pPlayerPool->pPlayer[iPlayerId], iVehicleId, iSeatId);	
}

void CSAMPFunctions::PlayerExitVehicle(int iPlayerId, int iVehicleId)
{
	// Get the player pointer
	CSAMPServer *pSAMPServer = (CSAMPServer *)CAddress::VAR_ServerPtr;
	// Call the function
	pfn__CPlayer__ExitVehicle(pSAMPServer->pPlayerPool->pPlayer[iPlayerId], iVehicleId);	
}

CVector3 *CSAMPFunctions::GetVehicleModelInfo(int iModelId, int iInfoType)
{
	CVector3 *vecInfo = pfn__GetVehicleModelInfo(iModelId, iInfoType);
	return vecInfo;
}

int CSAMPFunctions::GetMaxPlayers()
{
	// Call the function
	void *pConfig = (void *)CAddress::VAR_ConfigPtr;
	return pfn__CConfig__GetValueAsInteger(pConfig, "maxplayers");
}

int CSAMPFunctions::GetMaxNPC()
{
	// Call the function
	void *pConfig = (void *)CAddress::VAR_ConfigPtr;
	return pfn__CConfig__GetValueAsInteger(pConfig, "maxnpc");
}

