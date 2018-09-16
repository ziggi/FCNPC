/* =========================================

		FCNPC - Fully Controllable NPC
			----------------------

	- File: SAMPFunctions.cpp
	- Author(s): OrMisicL

  =========================================*/


#include "Main.hpp"

// Functions
CPlayerPool__DeletePlayer_t     CFunctions::pfn__CPlayerPool__DeletePlayer = NULL;
CPlayer__SpawnForWorld_t        CFunctions::pfn__CPlayer__SpawnForWorld = NULL;
CPlayer__Kill_t                 CFunctions::pfn__CPlayer__Kill = NULL;
CPlayer__EnterVehicle_t         CFunctions::pfn__CPlayer__EnterVehicle = NULL;
CPlayer__ExitVehicle_t          CFunctions::pfn__CPlayer__ExitVehicle = NULL;
GetVehicleModelInfo_t           CFunctions::pfn__GetVehicleModelInfo = NULL;
CConsole__GetIntVariable_t      CFunctions::pfn__CConsole__GetIntVariable = NULL;
ClientJoin_RPC_t                CFunctions::pfn__ClientJoin_RPC = NULL;

RakNet__Send_t                  CFunctions::pfn__RakNet__Send = NULL;
RakNet__RPC_t                   CFunctions::pfn__RakNet__RPC = NULL;
RakNet__Receive_t               CFunctions::pfn__RakNet__Receive = NULL;
RakNet__GetPlayerIDFromIndex_t  CFunctions::pfn__RakNet__GetPlayerIDFromIndex = NULL;
RakNet__GetIndexFromPlayerID_t  CFunctions::pfn__RakNet__GetIndexFromPlayerID = NULL;

void CFunctions::Initialize()
{
#ifdef SAMP_03DL
	pArtInfo = *reinterpret_cast<CArtInfo**>(CAddress::VAR_ArtInfo);
#endif

	// Initialize function pointers
	pfn__CPlayerPool__DeletePlayer = (CPlayerPool__DeletePlayer_t)(CAddress::FUNC_CPlayerPool__DeletePlayer);
	pfn__CPlayer__SpawnForWorld = (CPlayer__SpawnForWorld_t)(CAddress::FUNC_CPlayer__SpawnForWorld);
	pfn__CPlayer__Kill = (CPlayer__Kill_t)(CAddress::FUNC_CPlayer__Kill);
	pfn__CPlayer__EnterVehicle = (CPlayer__EnterVehicle_t)(CAddress::FUNC_CPlayer__EnterVehicle);
	pfn__CPlayer__ExitVehicle = (CPlayer__ExitVehicle_t)(CAddress::FUNC_CPlayer__ExitVehicle);
	pfn__GetVehicleModelInfo = (GetVehicleModelInfo_t)(CAddress::FUNC_GetVehicleModelInfo);
	pfn__CConsole__GetIntVariable = (CConsole__GetIntVariable_t)(CAddress::FUNC_CConsole__GetIntVariable);
	pfn__ClientJoin_RPC = (ClientJoin_RPC_t)(CAddress::FUNC_ClientJoin_RPC);
}

void CFunctions::PreInitialize()
{
	int(*pfn_GetNetGame)(void) = reinterpret_cast<int(*)(void)>(ppPluginData[PLUGIN_DATA_NETGAME]);
	pNetGame = reinterpret_cast<CNetGame*>(pfn_GetNetGame());

	int(*pfn__GetConsole)(void) = reinterpret_cast<int(*)(void)>(ppPluginData[PLUGIN_DATA_CONSOLE]);
	pConsole = reinterpret_cast<void*>(pfn__GetConsole());

	int(*pfn__GetRakServer)(void) = reinterpret_cast<int(*)(void)>(ppPluginData[PLUGIN_DATA_RAKSERVER]);
	pRakServer = reinterpret_cast<void*>(pfn__GetRakServer());

	int *pRakServer_VTBL = reinterpret_cast<int*>(*reinterpret_cast<void**>(pRakServer));

	CUtils::UnProtect(pRakServer_VTBL[RAKNET_SEND_OFFSET], 4);
	CUtils::UnProtect(pRakServer_VTBL[RAKNET_RPC_OFFSET], 4);
	CUtils::UnProtect(pRakServer_VTBL[RAKNET_RECEIVE_OFFSET], 4);
	CUtils::UnProtect(pRakServer_VTBL[RAKNET_GET_PLAYERID_FROM_INDEX_OFFSET], 4);
	CUtils::UnProtect(pRakServer_VTBL[RAKNET_GET_INDEX_FROM_PLAYERID_OFFSET], 4);

	pfn__RakNet__Send = reinterpret_cast<RakNet__Send_t>(pRakServer_VTBL[RAKNET_SEND_OFFSET]);
	pfn__RakNet__RPC = reinterpret_cast<RakNet__RPC_t>(pRakServer_VTBL[RAKNET_RPC_OFFSET]);
	pfn__RakNet__Receive = reinterpret_cast<RakNet__Receive_t>(pRakServer_VTBL[RAKNET_RECEIVE_OFFSET]);
	pfn__RakNet__GetPlayerIDFromIndex = reinterpret_cast<RakNet__GetPlayerIDFromIndex_t>(pRakServer_VTBL[RAKNET_GET_PLAYERID_FROM_INDEX_OFFSET]);
	pfn__RakNet__GetIndexFromPlayerID = reinterpret_cast<RakNet__GetIndexFromPlayerID_t>(pRakServer_VTBL[RAKNET_GET_INDEX_FROM_PLAYERID_OFFSET]);
}

WORD CFunctions::GetFreePlayerSlot()
{
	// Loop through all the players
	int iMaxPlayers = static_cast<int>(GetMaxPlayers());
	for (int i = iMaxPlayers - 1; i != -1; i--) {
		// Is he not connected ?
		if (!pNetGame->pPlayerPool->bIsPlayerConnected[i]) {
			return static_cast<WORD>(i);
		}
	}
	return INVALID_PLAYER_ID;
}

WORD CFunctions::NewPlayer(char *szName)
{
	// Get a free player slot
	WORD wPlayerId = GetFreePlayerSlot();
	if (wPlayerId == INVALID_PLAYER_ID) {
		return INVALID_PLAYER_ID;
	}

	// Get the SAMP authentication
	WORD wPort = 9000 + wPlayerId;
	PlayerID systemAddress = { 0x0100007F, wPort };
	int iVersion = *reinterpret_cast<int *>(CAddress::VAR_ServerAuthentication) ^ CAddress::VAR_NetVersion;
	BYTE byteMod = 1;
	BYTE byteNameLen = static_cast<BYTE>(strlen(szName));

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

	reinterpret_cast<CSAMPRakPeer*>(pRakServer)->SetConnectedPlayer(systemAddress, wPlayerId);

	pfn__ClientJoin_RPC(&pNPCParams);

	reinterpret_cast<CSAMPRakPeer*>(pRakServer)->SetDisonnectedPlayer(wPlayerId);

	// Return the player id
	return wPlayerId;
}

void CFunctions::DeletePlayer(WORD wPlayerId)
{
	pfn__CPlayerPool__DeletePlayer(pNetGame->pPlayerPool, wPlayerId, 0);
}

void CFunctions::SpawnPlayer(CPlayer *pPlayer)
{
	pfn__CPlayer__SpawnForWorld(pPlayer);
}

void CFunctions::KillPlayer(CPlayer *pPlayer, BYTE byteReason, WORD wKillerId)
{
	pfn__CPlayer__Kill(pPlayer, byteReason, wKillerId);
}

void CFunctions::PlayerEnterVehicle(CPlayer *pPlayer, WORD wVehicleId, BYTE byteSeatId)
{
	pfn__CPlayer__EnterVehicle(pPlayer, wVehicleId, byteSeatId);
}

void CFunctions::PlayerExitVehicle(CPlayer *pPlayer, WORD wVehicleId)
{
	pfn__CPlayer__ExitVehicle(pPlayer, wVehicleId);
}

CVector *CFunctions::GetVehicleModelInfoEx(int iModelID, int iInfoType)
{
	return pfn__GetVehicleModelInfo(iModelID, iInfoType);
}

WORD CFunctions::GetMaxPlayers()
{
	return static_cast<WORD>(pfn__CConsole__GetIntVariable(pConsole, "maxplayers"));
}

WORD CFunctions::GetMaxNPC()
{
	return static_cast<WORD>(pfn__CConsole__GetIntVariable(pConsole, "maxnpc"));
}

#ifdef SAMP_03DL
int CFunctions::GetSkinBaseID(DWORD dwSkinId)
{
	if (pArtInfo->artList.dwCapacity == 0) {
		return -1;
	}

	for (uint32_t i = 0; i < pArtInfo->artList.dwCapacity; i++) {
		if (pArtInfo->artList.pModelList[i]->dwNewId == dwSkinId) {
			return pArtInfo->artList.pModelList[i]->dwBaseId;
		}
	}

	return -1;
}
#endif

void CFunctions::GlobalRPC(int* szUniqueID, RakNet::BitStream* bsParams, WORD wExcludePlayerId, char PacketStream)
{
	PacketReliability reliable = RELIABLE_ORDERED;

	if (PacketStream == 3) {
		reliable = RELIABLE;
	}

	if (wExcludePlayerId == INVALID_PLAYER_ID) {
		pfn__RakNet__RPC(pRakServer, szUniqueID, bsParams, HIGH_PRIORITY, reliable, PacketStream, UNASSIGNED_PLAYER_ID, true, false);
	} else {
		pfn__RakNet__RPC(pRakServer, szUniqueID, bsParams, HIGH_PRIORITY, reliable, PacketStream, GetPlayerIDFromIndex(wExcludePlayerId), true, false);
	}

}

void CFunctions::AddedPlayersRPC(int* szUniqueID, RakNet::BitStream* bsParams, WORD wPlayerId, char PacketStream)
{
	CPlayer *pPlayer;

	for (WORD i = 0; i <= pNetGame->pPlayerPool->dwPlayerPoolSize; i++) {
		if (pNetGame->pPlayerPool->bIsPlayerConnected[i] && i != wPlayerId) {
			pPlayer = pNetGame->pPlayerPool->pPlayer[i];

			if (pPlayer && pPlayer->byteStreamedIn[wPlayerId]) {
				pfn__RakNet__RPC(pRakServer, szUniqueID, bsParams, HIGH_PRIORITY, RELIABLE_ORDERED, PacketStream, GetPlayerIDFromIndex(i), false, false);
			}
		}
	}
}

void CFunctions::AddedVehicleRPC(int* szUniqueID, RakNet::BitStream* bsParams, WORD wVehicleId, WORD wExcludePlayerId, char PacketStream)
{
	CPlayer *pPlayer;

	for (WORD i = 0; i <= pNetGame->pPlayerPool->dwPlayerPoolSize; i++) {
		if (pNetGame->pPlayerPool->bIsPlayerConnected[i] && i != wExcludePlayerId) {
			pPlayer = pNetGame->pPlayerPool->pPlayer[i];

			if (pPlayer && pPlayer->byteVehicleStreamedIn[wVehicleId]) {
				pfn__RakNet__RPC(pRakServer, szUniqueID, bsParams, HIGH_PRIORITY, RELIABLE_ORDERED, PacketStream, GetPlayerIDFromIndex(i), false, false);
			}
		}
	}
}

void CFunctions::PlayerRPC(int* szUniqueID, RakNet::BitStream* bsParams, WORD wPlayerId, char PacketStream)
{
	PacketReliability reliable = RELIABLE_ORDERED;

	if (PacketStream == 2) {
		reliable = RELIABLE;
	}

	pfn__RakNet__RPC(pRakServer, szUniqueID, bsParams, HIGH_PRIORITY, reliable, PacketStream, GetPlayerIDFromIndex(wPlayerId), false, false);
}

void CFunctions::GlobalPacket(RakNet::BitStream* bsParams)
{
	pfn__RakNet__Send(pRakServer, bsParams, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 1, UNASSIGNED_PLAYER_ID, true);
}

void CFunctions::PlayerPacket(RakNet::BitStream* bsParams, WORD wPlayerId)
{
	pfn__RakNet__Send(pRakServer, bsParams, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 1, GetPlayerIDFromIndex(wPlayerId), false);
}

PlayerID CFunctions::GetPlayerIDFromIndex(int index)
{
	return pfn__RakNet__GetPlayerIDFromIndex(pRakServer, index);
}

int CFunctions::GetIndexFromPlayerID(PlayerID playerId)
{
	return pfn__RakNet__GetIndexFromPlayerID(pRakServer, playerId);
}

void CFunctions::PlayerShoot(WORD wPlayerId, WORD wHitId, BYTE byteHitType, BYTE byteWeaponId, const CVector &vecPoint, const CVector &vecOffsetFrom, bool bIsHit)
{
	// Validate the player
	CPlayerManager *pPlayerManager = pServer->GetPlayerManager();
	if (!pPlayerManager->IsNpcConnected(wPlayerId)) {
		return;
	}

	// Get the player
	CPlayerData *pPlayerData = pPlayerManager->GetAt(wPlayerId);

	// Get the origin hit vector position
	CVector vecOrigin;
	pPlayerData->GetPosition(&vecOrigin);
	vecOrigin += vecOffsetFrom;

	// Create the target SendBullet structure
	CBulletSyncData bulletSyncDataTarget;
	if (bIsHit) {
		bulletSyncDataTarget.wHitID = wHitId;
		bulletSyncDataTarget.byteHitType = byteHitType;
	}
	else {
		bulletSyncDataTarget.wHitID = INVALID_PLAYER_ID;
		bulletSyncDataTarget.byteHitType = BULLET_HIT_TYPE_NONE;
	}
	bulletSyncDataTarget.byteWeaponID = byteWeaponId;
	bulletSyncDataTarget.vecCenterOfHit = CVector();
	bulletSyncDataTarget.vecHitOrigin = vecOrigin;
	bulletSyncDataTarget.vecHitTarget = vecPoint;

	// Find player in vecPoint
	if (bIsHit && bulletSyncDataTarget.byteHitType == BULLET_HIT_TYPE_NONE) {
		for (WORD i = 0; i <= pNetGame->pPlayerPool->dwPlayerPoolSize; i++) {
			if (!pServer->GetPlayerManager()->IsPlayerConnected(i) || wPlayerId == i) {
				continue;
			}

			CPlayer *pPlayer = pNetGame->pPlayerPool->pPlayer[i];
			if (!pPlayer) {
				continue;
			}

			bool bIsPlayerOnRay = CMath::GetDistanceFromRayToPoint(vecOrigin, vecPoint, pPlayer->vecPosition) < MAX_HIT_RADIUS;
			bool bIsPlayerInDamageRange = bIsPlayerOnRay && CMath::GetDistanceBetween3DPoints(vecOrigin, pPlayer->vecPosition) < MAX_DAMAGE_DISTANCE;

			if (bIsPlayerOnRay && bIsPlayerInDamageRange) {
				bulletSyncDataTarget.byteHitType = BULLET_HIT_TYPE_PLAYER;
				bulletSyncDataTarget.wHitID = i;
				bulletSyncDataTarget.vecHitTarget = CMath::GetNearestPointToRay(vecOrigin, vecPoint, pPlayer->vecPosition);
				break;
			}
		}
	}

	// Get center of hit
	switch (bulletSyncDataTarget.byteHitType) {
	case BULLET_HIT_TYPE_NONE:
		bulletSyncDataTarget.vecCenterOfHit = bulletSyncDataTarget.vecHitTarget;
		break;
	case BULLET_HIT_TYPE_PLAYER:
		if (bulletSyncDataTarget.wHitID < MAX_PLAYERS) {
			CPlayer *pHitPlayerData = pNetGame->pPlayerPool->pPlayer[bulletSyncDataTarget.wHitID];
			if (pHitPlayerData) {
				bulletSyncDataTarget.vecCenterOfHit = bulletSyncDataTarget.vecHitTarget - pHitPlayerData->vecPosition;
			}
		}
		break;
	case BULLET_HIT_TYPE_VEHICLE:
		if (bulletSyncDataTarget.wHitID >= 1 && bulletSyncDataTarget.wHitID < MAX_VEHICLES) {
			CVehicle *pVehicle = pNetGame->pVehiclePool->pVehicle[bulletSyncDataTarget.wHitID];
			if (pVehicle) {
				bulletSyncDataTarget.vecCenterOfHit = bulletSyncDataTarget.vecHitTarget - pVehicle->vecPosition;
			}
		}
		break;
	case BULLET_HIT_TYPE_OBJECT:
		if (bulletSyncDataTarget.wHitID >= 1 && bulletSyncDataTarget.wHitID < MAX_OBJECTS) {
			CObject *pObject = pNetGame->pObjectPool->pObjects[bulletSyncDataTarget.wHitID];
			if (pObject) {
				bulletSyncDataTarget.vecCenterOfHit = bulletSyncDataTarget.vecHitTarget - pObject->matWorld.pos;
			}
		}
		break;
	case BULLET_HIT_TYPE_PLAYER_OBJECT:
		if (bulletSyncDataTarget.wHitID >= 1 && bulletSyncDataTarget.wHitID < MAX_OBJECTS) {
			CObject *pObject = pNetGame->pObjectPool->pPlayerObjects[wPlayerId][bulletSyncDataTarget.wHitID];
			if (pObject) {
				bulletSyncDataTarget.vecCenterOfHit = bulletSyncDataTarget.vecHitTarget - pObject->matWorld.pos;
			}
		}
		break;
	}

	// Update bullet sync data
	pPlayerData->SetBulletSync(&bulletSyncDataTarget);

	// Call FCNPC_OnWeaponShot
	int send = CCallbackManager::OnWeaponShot(wPlayerId, bulletSyncDataTarget.byteWeaponID, bulletSyncDataTarget.byteHitType, bulletSyncDataTarget.wHitID, bulletSyncDataTarget.vecCenterOfHit);
	if (send != 0) {
		// If it is a NPC
		if (bIsHit && bulletSyncDataTarget.byteHitType == BULLET_HIT_TYPE_PLAYER && pServer->GetPlayerManager()->IsNpcConnected(bulletSyncDataTarget.wHitID)) {
			CPlayerData *pHitPlayerData = pServer->GetPlayerManager()->GetAt(bulletSyncDataTarget.wHitID);

			if (pHitPlayerData && !pHitPlayerData->IsInvulnerable() && pHitPlayerData->GetState() != PLAYER_STATE_WASTED) {
				SWeaponInfo sWeaponInfo = CWeaponInfo::GetDefaultInfo(byteWeaponId);

				pHitPlayerData->ProcessDamage(wPlayerId, sWeaponInfo.fDamage, byteWeaponId, BODY_PART_TORSO);
				CCallbackManager::OnGiveDamage(wPlayerId, bulletSyncDataTarget.wHitID, byteWeaponId, BODY_PART_TORSO, sWeaponInfo.fDamage);
			}
		}

		// Write it to BitStream
		RakNet::BitStream bsSend;
		bsSend.Write(static_cast<BYTE>(ID_BULLET_SYNC));
		bsSend.Write(wPlayerId);
		bsSend.Write(reinterpret_cast<char*>(&bulletSyncDataTarget), sizeof(CBulletSyncData));

		// Send it
		CFunctions::GlobalPacket(&bsSend);
	}
}
