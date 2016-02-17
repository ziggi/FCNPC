/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: SAMPFunctions.cpp
	- Author(s): OrMisicL

  =========================================*/


#include "Main.h"

extern logprintf_t logprintf;

// Functions
ClientJoin_RPC_t                CFunctions::pfn__ClientJoin_RPC = NULL;
CPlayerPool__DeletePlayer_t     CFunctions::pfn__CPlayerPool__DeletePlayer = NULL;
CPlayer__SpawnForWorld_t        CFunctions::pfn__CPlayer__SpawnForWorld = NULL;
CPlayer__Kill_t                 CFunctions::pfn__CPlayer__Kill = NULL;
CPlayer__EnterVehicle_t         CFunctions::pfn__CPlayer__EnterVehicle = NULL;
CPlayer__ExitVehicle_t          CFunctions::pfn__CPlayer__ExitVehicle = NULL;
CConsole__GetIntVariable_t      CFunctions::pfn__CConsole__GetIntVariable = NULL;
GetVehicleModelInfo_t           CFunctions::pfn__GetVehicleModelInfo = NULL;
RakNet__Send_t                  CFunctions::pfn__RakNet__Send = NULL;
RakNet__RPC_t                   CFunctions::pfn__RakNet__RPC = NULL;
RakNet__Receive_t               CFunctions::pfn__RakNet__Receive = NULL;
GetNetGame_t                    CFunctions::pfn__GetNetGame = NULL;
GetConsole_t                    CFunctions::pfn__GetConsole = NULL;
GetRakServer_t                  CFunctions::pfn__GetRakServer = NULL;

void CFunctions::Initialize()
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

void CFunctions::PreInitialize()
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

int CFunctions::GetFreePlayerSlot()
{
	// Loop through all the players
	for (int i = (GetMaxPlayers() - 1); i != 0; i--)
	{
		// Is he not connected ?
		if (!pNetGame->pPlayerPool->bIsPlayerConnectedEx[i])
			return i;
	}
	return INVALID_PLAYER_ID;
}

int CFunctions::NewPlayer(char *szName)
{
	// Get a free player slot
	int iPlayerId;
	if ((iPlayerId = GetFreePlayerSlot()) == INVALID_PLAYER_ID)
		return INVALID_PLAYER_ID;

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

void CFunctions::DeletePlayer(int iPlayerId)
{
	pfn__CPlayerPool__DeletePlayer(pNetGame->pPlayerPool, iPlayerId, 0);
}

void CFunctions::SpawnPlayer(int iPlayerId)
{
	pfn__CPlayer__SpawnForWorld(pNetGame->pPlayerPool->pPlayer[iPlayerId]);	
}

void CFunctions::KillPlayer(int iPlayerId, int iKillerId, int iWeapon)
{
	pfn__CPlayer__Kill(pNetGame->pPlayerPool->pPlayer[iPlayerId], iKillerId, iWeapon);	
}

void CFunctions::PlayerEnterVehicle(int iPlayerId, int iVehicleId, int iSeatId)
{
	pfn__CPlayer__EnterVehicle(pNetGame->pPlayerPool->pPlayer[iPlayerId], iVehicleId, iSeatId);	
}

void CFunctions::PlayerExitVehicle(int iPlayerId, int iVehicleId)
{
	pfn__CPlayer__ExitVehicle(pNetGame->pPlayerPool->pPlayer[iPlayerId], iVehicleId);	
}

CVector *CFunctions::GetVehicleModelInfoEx(int iModelId, int iInfoType)
{
	return pfn__GetVehicleModelInfo(iModelId, iInfoType);
}

int CFunctions::GetMaxPlayers()
{
	return pfn__CConsole__GetIntVariable(pConsole, "maxplayers");
}

int CFunctions::GetMaxNPC()
{
	return pfn__CConsole__GetIntVariable(pConsole, "maxnpc");
}

void CFunctions::PlayerShoot(int iPlayerId, WORD iHitId, BYTE iHitType, BYTE iWeaponId, CVector vecPoint)
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
	bsSend.Write((BYTE)ID_BULLET_SYNC);
	bsSend.Write((WORD)iPlayerId);
	bsSend.Write((char *)&bulletSyncData, sizeof(CBulletSyncData));

	// Send it
	CFunctions::GlobalPacket(&bsSend);
}

void CFunctions::GlobalRPC(int* szUniqueID, RakNet::BitStream* bsParams, int iExcludePlayerId, char PacketStream)
{
	PacketReliability reliable = RELIABLE_ORDERED;

	if (PacketStream == 3)
		reliable = RELIABLE;

	if (iExcludePlayerId == INVALID_PLAYER_ID)
		pRakServer->RPC(szUniqueID, bsParams, HIGH_PRIORITY, reliable, PacketStream, UNASSIGNED_PLAYER_ID, true, false);
	else
		pRakServer->RPC(szUniqueID, bsParams, HIGH_PRIORITY, reliable, PacketStream, pRakServer->GetPlayerIDFromIndex(iExcludePlayerId), true, false);

}

void CFunctions::AddedPlayersRPC(int* szUniqueID, RakNet::BitStream* bsParams, int iPlayerId, char PacketStream)
{
	CPlayer *pPlayer;

	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		if (pNetGame->pPlayerPool->bIsPlayerConnectedEx[i] && i != iPlayerId)
		{
			pPlayer = pNetGame->pPlayerPool->pPlayer[i];

			if (pPlayer && pPlayer->byteStreamedIn[iPlayerId])
				pRakServer->RPC(szUniqueID, bsParams, HIGH_PRIORITY, RELIABLE_ORDERED, PacketStream, pRakServer->GetPlayerIDFromIndex(i), false, false);
		}
	}
}

void CFunctions::AddedVehicleRPC(int* szUniqueID, RakNet::BitStream* bsParams, int iVehicleId, int iExcludePlayerId, char PacketStream)
{
	CPlayer *pPlayer;

	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		if (pNetGame->pPlayerPool->bIsPlayerConnectedEx[i] && i != iExcludePlayerId)
		{
			pPlayer = pNetGame->pPlayerPool->pPlayer[i];

			if (pPlayer && pPlayer->byteVehicleStreamedIn[iVehicleId])
				pRakServer->RPC(szUniqueID, bsParams, HIGH_PRIORITY, RELIABLE_ORDERED, PacketStream, pRakServer->GetPlayerIDFromIndex(i), false, false);
		}
	}
}

void CFunctions::PlayerRPC(int* szUniqueID, RakNet::BitStream* bsParams, int iPlayerId, char PacketStream)
{
	PacketReliability reliable = RELIABLE_ORDERED;
	
	if (PacketStream == 2)
		reliable = RELIABLE;

	pRakServer->RPC(szUniqueID, bsParams, HIGH_PRIORITY, reliable, PacketStream, pRakServer->GetPlayerIDFromIndex(iPlayerId), false, false);
}

void CFunctions::GlobalPacket(RakNet::BitStream* bsParams)
{
	pRakServer->Send(bsParams, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 1, UNASSIGNED_PLAYER_ID, true);
}

void CFunctions::PlayerPacket(RakNet::BitStream* bsParams, int iPlayerId)
{
	pRakServer->Send(bsParams, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 1, pRakServer->GetPlayerIDFromIndex(iPlayerId), false);
}