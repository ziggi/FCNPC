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
	bulletSyncDataTarget.byteWeaponID = byteWeaponId;
	bulletSyncDataTarget.vecHitOrigin = vecOrigin;
	bulletSyncDataTarget.vecHitTarget = vecPoint;
	bulletSyncDataTarget.vecCenterOfHit = bulletSyncDataTarget.vecHitTarget;
	bulletSyncDataTarget.wHitID = wHitId;
	bulletSyncDataTarget.byteHitType = byteHitType;
	if (!bIsHit) {
		bulletSyncDataTarget.wHitID = 0xFFFF; // Using 0xFFFF instead of INVALID_PLAYER_ID, because the hitId is not necessarily a player
		bulletSyncDataTarget.byteHitType = BULLET_HIT_TYPE_NONE;
	}

	// Create the inBetween SendBullet structure
	CBulletSyncData bulletSyncDataInBetween;
	bulletSyncDataInBetween.byteWeaponID = bulletSyncDataTarget.byteWeaponID;
	bulletSyncDataInBetween.vecHitOrigin = bulletSyncDataTarget.vecHitOrigin;
	bulletSyncDataInBetween.vecHitTarget = CVector();
	bulletSyncDataInBetween.vecCenterOfHit = bulletSyncDataInBetween.vecHitTarget;
	bulletSyncDataInBetween.wHitID = 0xFFFF;
	bulletSyncDataInBetween.byteHitType = BULLET_HIT_TYPE_NONE;

	// Check if something is in between the origin and the target
	GetClosestEntityInBetween(wPlayerId, bulletSyncDataTarget.wHitID, bulletSyncDataTarget.vecHitOrigin, bulletSyncDataTarget.vecHitTarget, bulletSyncDataInBetween);

	// If something is in between the origin and the target, make that something the target
	if (bulletSyncDataInBetween.wHitID != 0xFFFF) {
		bulletSyncDataTarget = bulletSyncDataInBetween;
	}

	// Get center of hit
	switch (bulletSyncDataTarget.byteHitType) {
	case BULLET_HIT_TYPE_NONE:
		if (bulletSyncDataTarget.wHitID >= 0 && bulletSyncDataTarget.wHitID < MAX_ACTORS) { //Actors don't have a hit type
			CActor *pActor = pNetGame->pActorPool->pActor[bulletSyncDataTarget.wHitID];
			if (pActor) {
				bulletSyncDataTarget.vecCenterOfHit -= pActor->vecPosition;
			}
		}
		break;
	case BULLET_HIT_TYPE_PLAYER:
		if (bulletSyncDataTarget.wHitID >= 0 && bulletSyncDataTarget.wHitID < MAX_PLAYERS) {
			CPlayer *pPlayer = pNetGame->pPlayerPool->pPlayer[bulletSyncDataTarget.wHitID];
			if (pPlayer) {
				bulletSyncDataTarget.vecCenterOfHit -= pPlayer->vecPosition;
			}
		}
		break;
	case BULLET_HIT_TYPE_VEHICLE:
		if (bulletSyncDataTarget.wHitID >= 1 && bulletSyncDataTarget.wHitID < MAX_VEHICLES) { //Vehicle IDs start at 1
			CVehicle *pVehicle = pNetGame->pVehiclePool->pVehicle[bulletSyncDataTarget.wHitID];
			if (pVehicle) {
				bulletSyncDataTarget.vecCenterOfHit -= pVehicle->vecPosition;
			}
		}
		break;
	case BULLET_HIT_TYPE_OBJECT:
		if (bulletSyncDataTarget.wHitID >= 0 && bulletSyncDataTarget.wHitID < MAX_OBJECTS) {
			CObject *pObject = pNetGame->pObjectPool->pObjects[bulletSyncDataTarget.wHitID];
			if (pObject) {
				bulletSyncDataTarget.vecCenterOfHit -= pObject->matWorld.pos;
			}
		}
		break;
	case BULLET_HIT_TYPE_PLAYER_OBJECT:
		if (bulletSyncDataTarget.wHitID >= 0 && bulletSyncDataTarget.wHitID < MAX_OBJECTS) {
			CObject *pPlayerObject = pNetGame->pObjectPool->pPlayerObjects[wPlayerId][bulletSyncDataTarget.wHitID];
			if (pPlayerObject) {
				bulletSyncDataTarget.vecCenterOfHit -= pPlayerObject->matWorld.pos;
			}
		}
		break;
	}

	// Update bullet sync data
	pPlayerData->SetBulletSync(&bulletSyncDataTarget);

	// Call FCNPC_OnWeaponShot
	int send = CCallbackManager::OnWeaponShot(wPlayerId, bulletSyncDataTarget.byteWeaponID, bulletSyncDataTarget.byteHitType, bulletSyncDataTarget.wHitID, bulletSyncDataTarget.vecCenterOfHit);
	if (send != 0) {
		// If it is an NPC
		if (bIsHit && bulletSyncDataTarget.byteHitType == BULLET_HIT_TYPE_PLAYER && pServer->GetPlayerManager()->IsNpcConnected(bulletSyncDataTarget.wHitID)) {
			CPlayerData *pHitPlayerData = pServer->GetPlayerManager()->GetAt(bulletSyncDataTarget.wHitID);

			if (pHitPlayerData && !pHitPlayerData->IsInvulnerable() && pHitPlayerData->GetState() != PLAYER_STATE_WASTED) {
				SWeaponInfo sWeaponInfo = CWeaponInfo::GetDefaultInfo(bulletSyncDataTarget.byteWeaponID);

				pHitPlayerData->ProcessDamage(wPlayerId, sWeaponInfo.fDamage, bulletSyncDataTarget.byteWeaponID, BODY_PART_TORSO);
				CCallbackManager::OnGiveDamage(wPlayerId, bulletSyncDataTarget.wHitID, bulletSyncDataTarget.byteWeaponID, BODY_PART_TORSO, sWeaponInfo.fDamage);
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

void CFunctions::GetClosestEntityInBetween(WORD wPlayerId, WORD wTargetId, const CVector &vecHitOrigin, const CVector &vecHitTarget, CBulletSyncData &bulletSyncDataInBetween)
{
	WORD wClosestEntity = 0xFFFF;
	float fClosestEntityDistance = 0.0;

	// Create the playerInBetween SendBullet structure
	CBulletSyncData bulletSyncDataPlayerInBetween;
	bulletSyncDataPlayerInBetween.byteWeaponID = bulletSyncDataInBetween.byteWeaponID;
	bulletSyncDataPlayerInBetween.vecHitOrigin = bulletSyncDataInBetween.vecHitOrigin;
	bulletSyncDataPlayerInBetween.vecHitTarget = CVector();
	bulletSyncDataPlayerInBetween.vecCenterOfHit = bulletSyncDataPlayerInBetween.vecHitTarget;
	bulletSyncDataPlayerInBetween.wHitID = 0xFFFF;
	bulletSyncDataPlayerInBetween.byteHitType = BULLET_HIT_TYPE_NONE;

	// Check if a player is in between the origin and the target
	GetClosestPlayerInBetween(wPlayerId, wTargetId, vecHitOrigin, vecHitTarget, bulletSyncDataPlayerInBetween);
	if (bulletSyncDataPlayerInBetween.wHitID != INVALID_PLAYER_ID && (wClosestEntity == 0xFFFF || fClosestPlayerDistance < fClosestEntityDistance)) {
		bulletSyncDataInBetween = bulletSyncDataPlayerInBetween;
		fClosestEntityDistance = fClosestPlayerDistance;
		wClosestEntity = bulletSyncDataPlayerInBetween.wHitID;
	}

	// Create the npcInBetween SendBullet structure
	CBulletSyncData bulletSyncDataNPCInBetween;
	bulletSyncDataNPCInBetween.byteWeaponID = bulletSyncDataInBetween.byteWeaponID;
	bulletSyncDataNPCInBetween.vecHitOrigin = bulletSyncDataInBetween.vecHitOrigin;
	bulletSyncDataNPCInBetween.vecHitTarget = CVector();
	bulletSyncDataNPCInBetween.vecCenterOfHit = bulletSyncDataNPCInBetween.vecHitTarget;
	bulletSyncDataNPCInBetween.wHitID = 0xFFFF;
	bulletSyncDataNPCInBetween.byteHitType = BULLET_HIT_TYPE_NONE;

	// Check if an NPC is in between the origin and the target
	GetClosestNPCInBetween(wPlayerId, wTargetId, vecHitOrigin, vecHitTarget, bulletSyncDataNPCInBetween);
	if (bulletSyncDataNPCInBetween.wHitID != INVALID_PLAYER_ID && (wClosestEntity == 0xFFFF || fClosestNPCDistance < fClosestEntityDistance)) {
		bulletSyncDataInBetween = bulletSyncDataNPCInBetween;
		fClosestEntityDistance = fClosestNPCDistance;
		wClosestEntity = bulletSyncDataNPCInBetween.wHitID;
	}

	// Create the actorInBetween SendBullet structure
	CBulletSyncData bulletSyncDataActorInBetween;
	bulletSyncDataActorInBetween.byteWeaponID = bulletSyncDataInBetween.byteWeaponID;
	bulletSyncDataActorInBetween.vecHitOrigin = bulletSyncDataInBetween.vecHitOrigin;
	bulletSyncDataActorInBetween.vecHitTarget = CVector();
	bulletSyncDataActorInBetween.vecCenterOfHit = bulletSyncDataActorInBetween.vecHitTarget;
	bulletSyncDataActorInBetween.wHitID = 0xFFFF;
	bulletSyncDataActorInBetween.byteHitType = BULLET_HIT_TYPE_NONE;

	// Check if an actor is in between the origin and the target
	GetClosestActorInBetween(wPlayerId, wTargetId, vecHitOrigin, vecHitTarget, bulletSyncDataActorInBetween);
	if (bulletSyncDataActorInBetween.wHitID != INVALID_ACTOR_ID && (wClosestEntity == 0xFFFF || fClosestActorDistance < fClosestEntityDistance)) {
		bulletSyncDataInBetween = bulletSyncDataActorInBetween;
		fClosestEntityDistance = fClosestActorDistance;
		wClosestEntity = bulletSyncDataActorInBetween.wHitID;
	}

	// Create the vehicleInBetween SendBullet structure
	CBulletSyncData bulletSyncDataVehicleInBetween;
	bulletSyncDataVehicleInBetween.byteWeaponID = bulletSyncDataInBetween.byteWeaponID;
	bulletSyncDataVehicleInBetween.vecHitOrigin = bulletSyncDataInBetween.vecHitOrigin;
	bulletSyncDataVehicleInBetween.vecHitTarget = CVector();
	bulletSyncDataVehicleInBetween.vecCenterOfHit = bulletSyncDataVehicleInBetween.vecHitTarget;
	bulletSyncDataVehicleInBetween.wHitID = 0xFFFF;
	bulletSyncDataVehicleInBetween.byteHitType = BULLET_HIT_TYPE_NONE;

	// Check if a vehicle is in between the origin and the target
	GetClosestVehicleInBetween(wPlayerId, wTargetId, vecHitOrigin, vecHitTarget, bulletSyncDataVehicleInBetween);
	if (bulletSyncDataVehicleInBetween.wHitID != INVALID_VEHICLE_ID && (wClosestEntity == 0xFFFF || fClosestVehicleDistance < fClosestEntityDistance)) {
		bulletSyncDataInBetween = bulletSyncDataVehicleInBetween;
		fClosestEntityDistance = fClosestVehicleDistance;
		wClosestEntity = bulletSyncDataVehicleInBetween.wHitID;
	}

	// Create the objectInBetween SendBullet structure
	CBulletSyncData bulletSyncDataObjectInBetween;
	bulletSyncDataObjectInBetween.byteWeaponID = bulletSyncDataInBetween.byteWeaponID;
	bulletSyncDataObjectInBetween.vecHitOrigin = bulletSyncDataInBetween.vecHitOrigin;
	bulletSyncDataObjectInBetween.vecHitTarget = CVector();
	bulletSyncDataObjectInBetween.vecCenterOfHit = bulletSyncDataObjectInBetween.vecHitTarget;
	bulletSyncDataObjectInBetween.wHitID = 0xFFFF;
	bulletSyncDataObjectInBetween.byteHitType = BULLET_HIT_TYPE_NONE;

	// Check if an object is in between the origin and the target
	GetClosestObjectInBetween(wPlayerId, wTargetId, vecHitOrigin, vecHitTarget, bulletSyncDataObjectInBetween);
	if (bulletSyncDataObjectInBetween.wHitID != INVALID_OBJECT_ID && (wClosestEntity == 0xFFFF || fClosestObjectDistance < fClosestEntityDistance)) {
		bulletSyncDataInBetween = bulletSyncDataObjectInBetween;
		fClosestEntityDistance = fClosestObjectDistance;
		wClosestEntity = bulletSyncDataObjectInBetween.wHitID;
	}

	// Create the playerObjectInBetween SendBullet structure
	CBulletSyncData bulletSyncDataPlayerObjectInBetween;
	bulletSyncDataPlayerObjectInBetween.byteWeaponID = bulletSyncDataInBetween.byteWeaponID;
	bulletSyncDataPlayerObjectInBetween.vecHitOrigin = bulletSyncDataInBetween.vecHitOrigin;
	bulletSyncDataPlayerObjectInBetween.vecHitTarget = CVector();
	bulletSyncDataPlayerObjectInBetween.vecCenterOfHit = bulletSyncDataPlayerObjectInBetween.vecHitTarget;
	bulletSyncDataPlayerObjectInBetween.wHitID = 0xFFFF;
	bulletSyncDataPlayerObjectInBetween.byteHitType = BULLET_HIT_TYPE_NONE;

	// Check if a player object is in between the origin and the target
	GetClosestPlayerObjectInBetween(wPlayerId, wTargetId, vecHitOrigin, vecHitTarget, bulletSyncDataPlayerObjectInBetween);
	if (bulletSyncDataPlayerObjectInBetween.wHitID != INVALID_OBJECT_ID && (wClosestEntity == 0xFFFF || fClosestPlayerObjectDistance < fClosestEntityDistance)) {
		bulletSyncDataInBetween = bulletSyncDataPlayerObjectInBetween;
		fClosestEntityDistance = fClosestPlayerObjectDistance;
		wClosestEntity = bulletSyncDataPlayerObjectInBetween.wHitID;
	}
}

void CFunctions::GetClosestPlayerInBetween(WORD wPlayerId, WORD wTargetId, const CVector &vecHitOrigin, const CVector &vecHitTarget, CBulletSyncData &bulletSyncDataInBetween)
{
	WORD wClosestPlayer = INVALID_PLAYER_ID;
	float fClosestPlayerDistance = 0.0;

	// Loop through all the players
	for (WORD i = 0; i <= pNetGame->pPlayerPool->dwPlayerPoolSize; i++) {

		// Validate the player
		CPlayer *pPlayer = pNetGame->pPlayerPool->pPlayer[i];
		if (wPlayerId == i || wTargetId == i || !pPlayer || !pServer->GetPlayerManager()->IsPlayerConnected(i) || pServer->GetPlayerManager()->IsNPC(i)) {
			continue;
		}

		// Is the player on the ray
		if (CMath::GetDistanceFromRayToPoint(vecHitOrigin, vecHitTarget, pPlayer->vecPosition) > MAX_HIT_RADIUS) {
			continue;
		}

		// Is the player in the damage range
		float fPlayerDistance = CMath::GetDistanceBetween3DPoints(vecHitOrigin, pPlayer->vecPosition);
		if (fPlayerDistance > MAX_DAMAGE_DISTANCE) {
			continue;
		}

		// Is the player closer
		if (wClosestPlayer == INVALID_PLAYER_ID || fPlayerDistance < fClosestPlayerDistance) {
			bulletSyncDataInBetween.vecHitTarget = CMath::GetNearestPointToRay(vecHitOrigin, vecHitTarget, pPlayer->vecPosition);
			bulletSyncDataInBetween.vecCenterOfHit = bulletSyncDataInBetween.vecHitTarget;
			bulletSyncDataInBetween.wHitID = i;
			bulletSyncDataInBetween.byteHitType = BULLET_HIT_TYPE_PLAYER;
			fClosestPlayerDistance = fPlayerDistance;
			wClosestPlayer = i;
		}
	}
}

void CFunctions::GetClosestNPCInBetween(WORD wPlayerId, WORD wTargetId, const CVector &vecHitOrigin, const CVector &vecHitTarget, CBulletSyncData &bulletSyncDataInBetween)
{
	WORD wClosestNPC = INVALID_PLAYER_ID;
	float fClosestNPCDistance = 0.0;

	// Loop through all the NPCs
	for (WORD i = 0; i <= pNetGame->pPlayerPool->dwPlayerPoolSize; i++) {

		// Validate the NPC
		CPlayer *pNPC = pNetGame->pPlayerPool->pPlayer[i];
		if (wPlayerId == i || wTargetId == i || !pNPC || !pServer->GetPlayerManager()->IsNpcConnected(i)) {
			continue;
		}

		// Is the NPC on the ray
		if (CMath::GetDistanceFromRayToPoint(vecHitOrigin, vecHitTarget, pNPC->vecPosition) > MAX_HIT_RADIUS) {
			continue;
		}

		// Is the NPC in the damage range
		float fNPCDistance = CMath::GetDistanceBetween3DPoints(vecHitOrigin, pNPC->vecPosition);
		if (fNPCDistance > MAX_DAMAGE_DISTANCE) {
			continue;
		}

		// Is the NPC closer
		if (wClosestNPC == INVALID_PLAYER_ID || fNPCDistance < fClosestNPCDistance) {
			bulletSyncDataInBetween.vecHitTarget = CMath::GetNearestPointToRay(vecHitOrigin, vecHitTarget, pNPC->vecPosition);
			bulletSyncDataInBetween.vecCenterOfHit = bulletSyncDataInBetween.vecHitTarget;
			bulletSyncDataInBetween.wHitID = i;
			bulletSyncDataInBetween.byteHitType = BULLET_HIT_TYPE_PLAYER;
			fClosestNPCDistance = fNPCDistance;
			wClosestNPC = i;
		}
	}
}

void CFunctions::GetClosestActorInBetween(WORD wPlayerId, WORD wTargetId, const CVector &vecHitOrigin, const CVector &vecHitTarget, CBulletSyncData &bulletSyncDataInBetween)
{
	WORD wClosestActor = INVALID_ACTOR_ID;
	float fClosestActorDistance = 0.0;

	// Loop through all the actors
	for (WORD i = 0; i <= pNetGame->pActorPool->dwActorPoolSize; i++) {

		// Validate the actor
		CActor *pActor = pNetGame->pActorPool->pActor[i];
		if (!pActor || !pNetGame->pActorPool->bValidActor[i]) {
			continue;
		}

		// Is the actor on the ray
		if (CMath::GetDistanceFromRayToPoint(vecHitOrigin, vecHitTarget, pActor->vecPosition) > MAX_HIT_RADIUS) {
			continue;
		}

		// Is the actor in the damage range
		float fActorDistance = CMath::GetDistanceBetween3DPoints(vecHitOrigin, pActor->vecPosition);
		if (fActorDistance > MAX_DAMAGE_DISTANCE) {
			continue;
		}

		// Is the actor closer
		if (wClosestActor == INVALID_ACTOR_ID || fActorDistance < fClosestActorDistance) {
			bulletSyncDataInBetween.vecHitTarget = CMath::GetNearestPointToRay(vecHitOrigin, vecHitTarget, pActor->vecPosition);
			bulletSyncDataInBetween.vecCenterOfHit = bulletSyncDataInBetween.vecHitTarget;
			bulletSyncDataInBetween.wHitID = i;
			bulletSyncDataInBetween.byteHitType = BULLET_HIT_TYPE_NONE; //Actors don't have a hit type
			fClosestActorDistance = fActorDistance;
			wClosestActor = i;
		}
	}
}

void CFunctions::GetClosestVehicleInBetween(WORD wPlayerId, WORD wTargetId, const CVector &vecHitOrigin, const CVector &vecHitTarget, CBulletSyncData &bulletSyncDataInBetween)
{
	WORD wClosestVehicle = INVALID_VEHICLE_ID;
	float fClosestVehicleDistance = 0.0;

	// Loop through all the vehicles
	for (WORD i = 1; i <= pNetGame->pVehiclePool->dwVehiclePoolSize; i++) { //Vehicle IDs start at 1

		// Validate the vehicle
		CVehicle *pVehicle = pNetGame->pVehiclePool->pVehicle[i];
		if (!pVehicle) {
			continue;
		}

		// Is the vehicle on the ray
		if (CMath::GetDistanceFromRayToPoint(vecHitOrigin, vecHitTarget, pVehicle->vecPosition) > MAX_HIT_RADIUS) {
			continue;
		}

		// Is the vehicle in the damage range
		float fVehicleDistance = CMath::GetDistanceBetween3DPoints(vecHitOrigin, pVehicle->vecPosition);
		if (fVehicleDistance > MAX_DAMAGE_DISTANCE) {
			continue;
		}

		// Is the vehicle closer
		if (wClosestVehicle == INVALID_VEHICLE_ID || fVehicleDistance < fClosestVehicleDistance) {
			bulletSyncDataInBetween.vecHitTarget = CMath::GetNearestPointToRay(vecHitOrigin, vecHitTarget, pVehicle->vecPosition);
			bulletSyncDataInBetween.vecCenterOfHit = bulletSyncDataInBetween.vecHitTarget;
			bulletSyncDataInBetween.wHitID = i;
			bulletSyncDataInBetween.byteHitType = BULLET_HIT_TYPE_VEHICLE;
			fClosestVehicleDistance = fVehicleDistance;
			wClosestVehicle = i;
		}
	}
}

void CFunctions::GetClosestObjectInBetween(WORD wPlayerId, WORD wTargetId, const CVector &vecHitOrigin, const CVector &vecHitTarget, CBulletSyncData &bulletSyncDataInBetween)
{
	WORD wClosestObject = INVALID_OBJECT_ID;
	float fClosestObjectDistance = 0.0;

	// Loop through all the objects
	for (WORD i = 0; i < MAX_OBJECTS; i++) {

		// Validate the object
		CObject *pObject = pNetGame->pObjectPool->pObjects[i];
		if (!pObject) {
			continue;
		}

		// Is the object on the ray
		if (CMath::GetDistanceFromRayToPoint(vecHitOrigin, vecHitTarget, pObject->matWorld.pos) > MAX_HIT_RADIUS) {
			continue;
		}

		// Is the object in the damage range
		float fObjectDistance = CMath::GetDistanceBetween3DPoints(vecHitOrigin, pObject->matWorld.pos);
		if (fObjectDistance > MAX_DAMAGE_DISTANCE) {
			continue;
		}

		// Is the object closer
		if (wClosestObject == INVALID_OBJECT_ID || fObjectDistance < fClosestObjectDistance) {
			bulletSyncDataInBetween.vecHitTarget = CMath::GetNearestPointToRay(vecHitOrigin, vecHitTarget, pObject->matWorld.pos);
			bulletSyncDataInBetween.vecCenterOfHit = bulletSyncDataInBetween.vecHitTarget;
			bulletSyncDataInBetween.wHitID = i;
			bulletSyncDataInBetween.byteHitType = BULLET_HIT_TYPE_OBJECT;
			fClosestObjectDistance = fObjectDistance;
			wClosestObject = i;
		}
	}
}

void CFunctions::GetClosestPlayerObjectInBetween(WORD wPlayerId, WORD wTargetId, const CVector &vecHitOrigin, const CVector &vecHitTarget, CBulletSyncData &bulletSyncDataInBetween)
{
	WORD wClosestPlayerObject = INVALID_OBJECT_ID;
	float fClosestPlayerObjectDistance = 0.0;

	// Loop through all the player objects
	for (WORD i = 0; i < MAX_OBJECTS; i++) {

		// Validate the player object
		CObject *pPlayerObject = pNetGame->pObjectPool->pPlayerObjects[wPlayerId][i];
		if (!pPlayerObject) {
			continue;
		}

		// Is the player object on the ray
		if (CMath::GetDistanceFromRayToPoint(vecHitOrigin, vecHitTarget, pPlayerObject->matWorld.pos) > MAX_HIT_RADIUS) {
			continue;
		}

		// Is the player object in the damage range
		float fPlayerObjectDistance = CMath::GetDistanceBetween3DPoints(vecHitOrigin, pPlayerObject->matWorld.pos);
		if (fPlayerObjectDistance > MAX_DAMAGE_DISTANCE) {
			continue;
		}

		// Is the player object closer
		if (wClosestPlayerObject == INVALID_OBJECT_ID || fPlayerObjectDistance < fClosestPlayerObjectDistance) {
			bulletSyncDataInBetween.vecHitTarget = CMath::GetNearestPointToRay(vecHitOrigin, vecHitTarget, pPlayerObject->matWorld.pos);
			bulletSyncDataInBetween.vecCenterOfHit = bulletSyncDataInBetween.vecHitTarget;
			bulletSyncDataInBetween.wHitID = i;
			bulletSyncDataInBetween.byteHitType = BULLET_HIT_TYPE_PLAYER_OBJECT;
			fClosestPlayerObjectDistance = fPlayerObjectDistance;
			wClosestPlayerObject = i;
		}
	}
}