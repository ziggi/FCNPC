/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: SAMPFunctions.cpp
	- Author(s): OrMisicL

  =========================================*/


#include "Main.h"

extern CServer          *pServer;
extern CSAMPRPCParams   *pCreateNPCParams;
extern void             **ppPluginData;
extern logprintf_t      logprintf;
extern CSAMPServer      *pNetGame;

// Functions
CreateNPC_RPC_t                 CSAMPFunctions::pfn__CreateNPC_RPC = NULL;
CPlayerPool__DeletePlayer_t     CSAMPFunctions::pfn__CPlayerPool__DeletePlayer = NULL;
CPlayer__SpawnForWorld_t        CSAMPFunctions::pfn__CPlayer__SpawnForWorld = NULL;
CPlayer__Kill_t                 CSAMPFunctions::pfn__CPlayer__Kill = NULL;
CPlayer__EnterVehicle_t         CSAMPFunctions::pfn__CPlayer__EnterVehicle = NULL;
CPlayer__ExitVehicle_t          CSAMPFunctions::pfn__CPlayer__ExitVehicle = NULL;
CConfig__GetValueAsInteger_t    CSAMPFunctions::pfn__CConfig__GetValueAsInteger = NULL;
GetVehicleModelInfo_t           CSAMPFunctions::pfn__GetVehicleModelInfo = NULL;
RakServer__Send_t               CSAMPFunctions::pfn__RakServer__Send = NULL;
RakServer__RPC_t                CSAMPFunctions::pfn__RakServer__RPC = NULL;
RakServer__Receive_t            CSAMPFunctions::pfn__RakServer__Receive = NULL;
GetNetGame_t                    CSAMPFunctions::pfn__GetNetGame = NULL;
GetConsole_t                    CSAMPFunctions::pfn__GetConsole = NULL;
GetRakServer_t                  CSAMPFunctions::pfn__GetRakServer = NULL;

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
	pNetGame = (CSAMPServer*)pfn__GetNetGame();

	pfn__GetConsole = (GetConsole_t)(ppPluginData[PLUGIN_DATA_CONSOLE]);

	pfn__GetRakServer = (GetRakServer_t)(ppPluginData[PLUGIN_DATA_RAKSERVER]);

	int *pRakServer_VTBL = ((int*)(*(void**)pfn__GetRakServer()));

	CUtils::UnProtect(pRakServer_VTBL[RAKNET_SEND_OFFSET], 4);
	CUtils::UnProtect(pRakServer_VTBL[RAKNET_RPC_OFFSET], 4);
	CUtils::UnProtect(pRakServer_VTBL[RAKNET_RECEIVE_OFFSET], 4);

	pfn__RakServer__Send = (RakServer__Send_t)(pRakServer_VTBL[RAKNET_SEND_OFFSET]);
	pfn__RakServer__RPC = (RakServer__RPC_t)(pRakServer_VTBL[RAKNET_RPC_OFFSET]);
	pfn__RakServer__Receive = (RakServer__Receive_t)(pRakServer_VTBL[RAKNET_RECEIVE_OFFSET]);
}

int CSAMPFunctions::GetFreePlayerSlot()
{
	// Loop through all the players
	for(int i = (GetMaxPlayers() - 1); i != 0; i--)
	{
		// Is he not connected ?
		if(!pNetGame->pPlayerPool->bIsPlayerConnected[i])
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
	int iAuthentication = *(int *)(CAddress::VAR_ServerAuthentication) ^ CAddress::OFFSET_NetVersion;
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
	pfn__CPlayerPool__DeletePlayer(pNetGame->pPlayerPool, iPlayerId, 0);
}

void CSAMPFunctions::SpawnPlayer(int iPlayerId)
{
	pfn__CPlayer__SpawnForWorld(pNetGame->pPlayerPool->pPlayer[iPlayerId]);	
}

void CSAMPFunctions::KillPlayer(int iPlayerId, int iKillerId, int iWeapon)
{
	pfn__CPlayer__Kill(pNetGame->pPlayerPool->pPlayer[iPlayerId], iKillerId, iWeapon);	
}

void CSAMPFunctions::PlayerEnterVehicle(int iPlayerId, int iVehicleId, int iSeatId)
{
	pfn__CPlayer__EnterVehicle(pNetGame->pPlayerPool->pPlayer[iPlayerId], iVehicleId, iSeatId);	
}

void CSAMPFunctions::PlayerExitVehicle(int iPlayerId, int iVehicleId)
{
	pfn__CPlayer__ExitVehicle(pNetGame->pPlayerPool->pPlayer[iPlayerId], iVehicleId);	
}

CVector *CSAMPFunctions::GetVehicleModelInfo(int iModelId, int iInfoType)
{
	return pfn__GetVehicleModelInfo(iModelId, iInfoType);
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

void CSAMPFunctions::PlayerShoot(int iPlayerId, WORD iHitId, BYTE iHitType, BYTE iWeaponId, CVector vecPoint)
{
	// Validate the player
	if (!pServer->GetPlayerManager()->IsPlayerConnected(iPlayerId))
		return;

	// Get the player position
	CVector vecPosition;
	pServer->GetPlayerManager()->GetAt(iPlayerId)->GetPosition(&vecPosition);

	// Create the SendBullet structure
	CBulletSyncData bulletSyncData;
	bulletSyncData.wHitID = iHitId;
	bulletSyncData.byteHitType = iHitType;
	bulletSyncData.byteWeaponID = iWeaponId;
	bulletSyncData.vecCenterOfHit = CVector(0.1f, 0.1f, 0.1f);
	bulletSyncData.vecHitOrigin = vecPosition;
	bulletSyncData.vecHitTarget = vecPoint;

	// find player in vecPoint
	if (bulletSyncData.byteHitType == BULLET_HIT_TYPE_NONE)
	{
		for (int i = 0; i < MAX_PLAYERS; i++)
		{
			if (!pNetGame->pPlayerPool->bIsPlayerConnected[i] || iPlayerId == i)
				continue;

			CSAMPPlayer *pPlayer = pNetGame->pPlayerPool->pPlayer[i];

			if (CMath::GetDistanceFromRayToPoint(vecPoint, vecPosition, pPlayer->vecPosition) < 1.0f &&
				CMath::GetDistanceBetween3DPoints(vecPosition, pPlayer->vecPosition) < MAX_DAMAGE_DISTANCE)
			{
				bulletSyncData.byteHitType = BULLET_HIT_TYPE_PLAYER;
				bulletSyncData.wHitID = i;
				break;
			}
		}
	}

	// Write it to BitStream
	RakNet::BitStream bsSend;
	bsSend.Write((BYTE)CAddress::OFFSET_SendBullet_RPC);
	bsSend.Write((unsigned short)iPlayerId);
	bsSend.Write((char *)&bulletSyncData, sizeof(CBulletSyncData));

	// Send it
	CSAMPSystemAddress systemAddress = CSAMPSystemAddress();
	CSAMPRakPeer *pSAMPRakPeer = (CSAMPRakPeer *)CAddress::VAR_RakPeerPtr;
	pfn__RakServer__Send((void *)pSAMPRakPeer, &bsSend, 1, 9, 0, CSAMPPlayerId(systemAddress), 1);
}

