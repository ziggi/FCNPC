/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: SAMPFunctions.cpp
	- Author(s): OrMisicL

  =========================================*/


#include "Main.h"

extern logprintf_t logprintf;

// Functions
ClientJoin_RPC_t                CSAMPFunctions::pfn__ClientJoin_RPC = NULL;
CPlayerPool__DeletePlayer_t     CSAMPFunctions::pfn__CPlayerPool__DeletePlayer = NULL;
CPlayer__SpawnForWorld_t        CSAMPFunctions::pfn__CPlayer__SpawnForWorld = NULL;
CPlayer__Kill_t                 CSAMPFunctions::pfn__CPlayer__Kill = NULL;
CPlayer__EnterVehicle_t         CSAMPFunctions::pfn__CPlayer__EnterVehicle = NULL;
CPlayer__ExitVehicle_t          CSAMPFunctions::pfn__CPlayer__ExitVehicle = NULL;
CConsole__GetIntVariable_t      CSAMPFunctions::pfn__CConsole__GetIntVariable = NULL;
GetVehicleModelInfo_t           CSAMPFunctions::pfn__GetVehicleModelInfo = NULL;
RakNet__Send_t                  CSAMPFunctions::pfn__RakNet__Send = NULL;
RakNet__RPC_t                   CSAMPFunctions::pfn__RakNet__RPC = NULL;
RakNet__Receive_t               CSAMPFunctions::pfn__RakNet__Receive = NULL;
GetNetGame_t                    CSAMPFunctions::pfn__GetNetGame = NULL;
GetConsole_t                    CSAMPFunctions::pfn__GetConsole = NULL;
GetRakServer_t                  CSAMPFunctions::pfn__GetRakServer = NULL;

void CSAMPFunctions::Initialize()
{
	// Initialize function pointers
	pfn__ClientJoin_RPC = (ClientJoin_RPC_t)(CAddress::FUNC_ClientJoin_RPC);
	pfn__CPlayerPool__DeletePlayer = (CPlayerPool__DeletePlayer_t)(CAddress::FUNC_CPlayerPool__DeletePlayer);
	
	pfn__CPlayer__SpawnForWorld = (CPlayer__SpawnForWorld_t)(CAddress::FUNC_CPlayer__SpawnForWorld);
	pfn__CPlayer__Kill = (CPlayer__Kill_t)(CAddress::FUNC_CPlayer__Kill);
	pfn__CPlayer__EnterVehicle = (CPlayer__EnterVehicle_t)(CAddress::FUNC_CPlayer__EnterVehicle);
	pfn__CPlayer__ExitVehicle = (CPlayer__ExitVehicle_t)(CAddress::FUNC_CPlayer__ExitVehicle);

	pfn__CConsole__GetIntVariable = (CConsole__GetIntVariable_t)(CAddress::FUNC_CConsole__GetIntVariable);

	pfn__GetVehicleModelInfo = (GetVehicleModelInfo_t)(CAddress::FUNC_GetVehicleModelInfo);
}

void CSAMPFunctions::PreInitialize()
{
	pfn__GetNetGame = (GetNetGame_t)(ppPluginData[PLUGIN_DATA_NETGAME]);
	pNetGame = (CNetGame*)pfn__GetNetGame();

	pfn__GetConsole = (GetConsole_t)(ppPluginData[PLUGIN_DATA_CONSOLE]);
	pConsole = (void*)pfn__GetConsole();

	pfn__GetRakServer = (GetRakServer_t)(ppPluginData[PLUGIN_DATA_RAKSERVER]);
	pRakServer = (RakServer*)pfn__GetRakServer();

	int *pRakServer_VTBL = ((int*)(*(void**)pRakServer));

	CUtils::UnProtect(pRakServer_VTBL[RAKNET_SEND_OFFSET], 4);
	CUtils::UnProtect(pRakServer_VTBL[RAKNET_RPC_OFFSET], 4);
	CUtils::UnProtect(pRakServer_VTBL[RAKNET_RECEIVE_OFFSET], 4);

	pfn__RakNet__Send = (RakNet__Send_t)(pRakServer_VTBL[RAKNET_SEND_OFFSET]);
	pfn__RakNet__RPC = (RakNet__RPC_t)(pRakServer_VTBL[RAKNET_RPC_OFFSET]);
	pfn__RakNet__Receive = (RakNet__Receive_t)(pRakServer_VTBL[RAKNET_RECEIVE_OFFSET]);
}

int CSAMPFunctions::GetFreePlayerSlot()
{
	// Loop through all the players
	for(int i = (GetMaxPlayers() - 1); i != 0; i--)
	{
		// Is he not connected ?
		if(!pNetGame->pPlayerPool->bIsPlayerConnectedEx[i])
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
	PlayerID systemAddress = { 0x0100007F, 9000 + iPlayerId };
	int iVersion = *(int *)(CAddress::VAR_ServerAuthentication) ^ CAddress::VAR_NetVersion;
	BYTE byteMod = 1;
	BYTE byteNameLen = (BYTE)strlen(szName);

	RakNet::BitStream bsSend;
	bsSend.Write(CAddress::VAR_NetVersion);
	bsSend.Write(byteMod);
	bsSend.Write(byteNameLen);
	bsSend.Write(szName, byteNameLen);
	bsSend.Write(iVersion);
	
	RPCParameters pNPCParams;
	pNPCParams.input = bsSend.GetData();
	pNPCParams.numberOfBitsOfData = bsSend.GetNumberOfBitsUsed();
	pNPCParams.sender = systemAddress;

	((CSAMPRakPeer *)pRakServer)->SetConnectedPlayer(systemAddress, iPlayerId);

	pfn__ClientJoin_RPC(&pNPCParams);

	((CSAMPRakPeer *)pRakServer)->SetDisonnectedPlayer(iPlayerId);

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

CVector *CSAMPFunctions::GetVehicleModelInfoEx(int iModelId, int iInfoType)
{
	return pfn__GetVehicleModelInfo(iModelId, iInfoType);
}

int CSAMPFunctions::GetMaxPlayers()
{
	return pfn__CConsole__GetIntVariable(pConsole, "maxplayers");
}

int CSAMPFunctions::GetMaxNPC()
{
	return pfn__CConsole__GetIntVariable(pConsole, "maxnpc");
}

void CSAMPFunctions::PlayerShoot(int iPlayerId, WORD iHitId, BYTE iHitType, BYTE iWeaponId, CVector vecPoint)
{
	// Validate the player
	if (!pServer->GetPlayerManager()->IsPlayerConnectedEx(iPlayerId))
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
			if (!pNetGame->pPlayerPool->bIsPlayerConnectedEx[i] || iPlayerId == i)
				continue;

			CPlayer *pPlayer = pNetGame->pPlayerPool->pPlayer[i];

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
	bsSend.Write((WORD)iPlayerId);
	bsSend.Write((char *)&bulletSyncData, sizeof(CBulletSyncData));

	// Send it
	pRakServer->Send(&bsSend, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_PLAYER_ID, true);
}

