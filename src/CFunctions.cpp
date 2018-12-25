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

void CFunctions::PlayerShoot(WORD wPlayerId, WORD wHitId, BYTE byteHitType, BYTE byteWeaponId, const CVector &vecPoint, const CVector &vecOffsetFrom, bool bIsHit, BYTE byteBetweenCheckFlags)
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

	// Create the initial SendBullet structure
	CBulletSyncData bulletSyncDataTarget;
	bulletSyncDataTarget.byteWeaponID = byteWeaponId;
	bulletSyncDataTarget.vecHitOrigin = vecOrigin;
	bulletSyncDataTarget.vecHitTarget = vecPoint;
	bulletSyncDataTarget.vecCenterOfHit = bulletSyncDataTarget.vecHitTarget;
	bulletSyncDataTarget.wHitID = wHitId;
	bulletSyncDataTarget.byteHitType = byteHitType;

	// If the shooter misses his shot
	if (!bIsHit) {
		// logprintf("TARGET MISSED");
		bulletSyncDataTarget.wHitID = 0xFFFF; // Using 0xFFFF instead of INVALID_PLAYER_ID, because the hitId is not necessarily a player
		bulletSyncDataTarget.byteHitType = BULLET_HIT_TYPE_NONE;
	}

	// If the target is not in range
	float fTargetDistance = CMath::GetDistanceBetween3DPoints(bulletSyncDataTarget.vecHitOrigin, bulletSyncDataTarget.vecHitTarget);
	if (fTargetDistance > CWeaponInfo::GetDefaultInfo(bulletSyncDataTarget.byteWeaponID).fRange) {
		// logprintf("TARGET OUT OF RANGE");
		bulletSyncDataTarget.wHitID = 0xFFFF;
		bulletSyncDataTarget.byteHitType = BULLET_HIT_TYPE_NONE;
	}

	// If something is in between the origin and the target (we currently don't handle checking beyond the target, even when missing with leftover range)
	BYTE byteClosestEntityType = FCNPC_ENTITY_CHECK_NONE;
	WORD wPlayerObjectOwnerId = INVALID_PLAYER_ID;
	CVector vecHitMap = bulletSyncDataTarget.vecHitTarget;
	float fRange = CWeaponInfo::GetDefaultInfo(bulletSyncDataTarget.byteWeaponID).fRange;

	// Pass original hit ID to correctly handle missed or out of range shots!
	WORD wClosestEntity = GetClosestEntityInBetween(bulletSyncDataTarget.vecHitOrigin, bulletSyncDataTarget.vecHitTarget, min(fRange, fTargetDistance), byteBetweenCheckFlags, wPlayerId, wHitId, byteClosestEntityType, wPlayerObjectOwnerId, vecHitMap);
	if (wClosestEntity != 0xFFFF) {
		// logprintf("SOMETHING IN BETWEEN SHOOTER AND TARGET");
		bulletSyncDataTarget.wHitID = wClosestEntity;
		switch (byteClosestEntityType) {
			case FCNPC_ENTITY_CHECK_PLAYER:
			case FCNPC_ENTITY_CHECK_NPC:
				bulletSyncDataTarget.byteHitType = BULLET_HIT_TYPE_PLAYER;
				break;
			case FCNPC_ENTITY_CHECK_ACTOR:
				bulletSyncDataTarget.byteHitType = BULLET_HIT_TYPE_NONE;
				break;
			case FCNPC_ENTITY_CHECK_VEHICLE:
				bulletSyncDataTarget.byteHitType = BULLET_HIT_TYPE_VEHICLE;
				break;
			case FCNPC_ENTITY_CHECK_OBJECT:
				bulletSyncDataTarget.byteHitType = BULLET_HIT_TYPE_OBJECT;
				break;
			case FCNPC_ENTITY_CHECK_POBJECT_ORIG:
			case FCNPC_ENTITY_CHECK_POBJECT_TARG:
				bulletSyncDataTarget.byteHitType = BULLET_HIT_TYPE_PLAYER_OBJECT;
				break;
			case FCNPC_ENTITY_CHECK_MAP:
				bulletSyncDataTarget.byteHitType = BULLET_HIT_TYPE_NONE;
				break;
			default:
				bulletSyncDataTarget.byteHitType = BULLET_HIT_TYPE_NONE;
				break;
		}
	}

	// Get center of hit
	switch (bulletSyncDataTarget.byteHitType) {
		case BULLET_HIT_TYPE_NONE:
			if (bulletSyncDataTarget.wHitID >= 0 && bulletSyncDataTarget.wHitID < MAX_ACTORS) { // Actors don't have a hit type, this is conform with the SA-MP callback OnPlayerWeaponShot
				CActor *pActor = pNetGame->pActorPool->pActor[bulletSyncDataTarget.wHitID];
				if (pActor) {
					// logprintf("HIT ACTOR");
					bulletSyncDataTarget.vecHitTarget = CMath::GetNearestPointToRay(bulletSyncDataTarget.vecHitOrigin, bulletSyncDataTarget.vecHitTarget, pActor->vecPosition);
					bulletSyncDataTarget.vecCenterOfHit = bulletSyncDataTarget.vecHitTarget; // When actor is hit use the actor collision position, this is conform with the SA-MP callback OnPlayerWeaponShot
				}
			} else if (bulletSyncDataTarget.wHitID == -1) { // Hit map
				// logprintf("HIT MAP");
				bulletSyncDataTarget.vecCenterOfHit = vecHitMap; // When map is hit use the object collision position, this is conform with the SA-MP callback OnPlayerWeaponShot
			} else { // Hit nothing
				// logprintf("HIT NOTHING");
				bulletSyncDataTarget.vecCenterOfHit = CVector(); // When nothing is hit use 0.0, this is conform with the SA-MP callback OnPlayerWeaponShot
			}
			break;
		case BULLET_HIT_TYPE_PLAYER:
			if (bulletSyncDataTarget.wHitID >= 0 && bulletSyncDataTarget.wHitID < MAX_PLAYERS) {
				CPlayer *pPlayer = pNetGame->pPlayerPool->pPlayer[bulletSyncDataTarget.wHitID];
				if (pPlayer) {
					if (!pServer->GetPlayerManager()->IsNPC(bulletSyncDataTarget.wHitID)) { // Separate code for players and NPCs, in case we need to handle them differently in the future
						// logprintf("HIT PLAYER");
						bulletSyncDataTarget.vecHitTarget = CMath::GetNearestPointToRay(bulletSyncDataTarget.vecHitOrigin, bulletSyncDataTarget.vecHitTarget, pPlayer->vecPosition);
						bulletSyncDataTarget.vecCenterOfHit = bulletSyncDataTarget.vecHitTarget - pPlayer->vecPosition;
					} else {
						// logprintf("HIT NPC");
						bulletSyncDataTarget.vecHitTarget = CMath::GetNearestPointToRay(bulletSyncDataTarget.vecHitOrigin, bulletSyncDataTarget.vecHitTarget, pPlayer->vecPosition);
						bulletSyncDataTarget.vecCenterOfHit = bulletSyncDataTarget.vecHitTarget - pPlayer->vecPosition;
					}
				}
			}
			break;
		case BULLET_HIT_TYPE_VEHICLE:
			if (bulletSyncDataTarget.wHitID >= 1 && bulletSyncDataTarget.wHitID < MAX_VEHICLES) { // Vehicle IDs start at 1
				CVehicle *pVehicle = pNetGame->pVehiclePool->pVehicle[bulletSyncDataTarget.wHitID];
				if (pVehicle) {
					// logprintf("HIT VEHICLE");
					bulletSyncDataTarget.vecHitTarget = CMath::GetNearestPointToRay(bulletSyncDataTarget.vecHitOrigin, bulletSyncDataTarget.vecHitTarget, pVehicle->vecPosition);
					bulletSyncDataTarget.vecCenterOfHit = bulletSyncDataTarget.vecHitTarget - pVehicle->vecPosition;
				}
			}
			break;
		case BULLET_HIT_TYPE_OBJECT:
			if (bulletSyncDataTarget.wHitID >= 1 && bulletSyncDataTarget.wHitID < MAX_OBJECTS) { // Object IDs start at 1
				CObject *pObject = pNetGame->pObjectPool->pObjects[bulletSyncDataTarget.wHitID];
				if (pObject) {
					// logprintf("HIT OBJECT");
					bulletSyncDataTarget.vecHitTarget = CMath::GetNearestPointToRay(bulletSyncDataTarget.vecHitOrigin, bulletSyncDataTarget.vecHitTarget, pObject->matWorld.pos);
					bulletSyncDataTarget.vecCenterOfHit = bulletSyncDataTarget.vecHitTarget - pObject->matWorld.pos;
				}
			}
			break;
		case BULLET_HIT_TYPE_PLAYER_OBJECT:
			if (bulletSyncDataTarget.wHitID >= 1 && bulletSyncDataTarget.wHitID < MAX_OBJECTS) { // Player object IDs start at 1
				if (wPlayerObjectOwnerId == wPlayerId) { // Handles player objects of the shooter
					CObject *pPlayerObject = pNetGame->pObjectPool->pPlayerObjects[wPlayerId][bulletSyncDataTarget.wHitID];
					if (pPlayerObject) {
						// logprintf("HIT PLAYER OBJECT SHOOTER");
						bulletSyncDataTarget.vecHitTarget = CMath::GetNearestPointToRay(bulletSyncDataTarget.vecHitOrigin, bulletSyncDataTarget.vecHitTarget, pPlayerObject->matWorld.pos);
						bulletSyncDataTarget.vecCenterOfHit = bulletSyncDataTarget.vecHitTarget - pPlayerObject->matWorld.pos;
					}
				} else  if (wPlayerObjectOwnerId == wHitId) { // Handles player objects of the target
					if (pServer->GetPlayerManager()->IsPlayerConnected(wHitId)) {
						CObject *pPlayerObject = pNetGame->pObjectPool->pPlayerObjects[wHitId][bulletSyncDataTarget.wHitID]; // Use original hit ID to correctly handle target player objects!
						if (pPlayerObject) {
							// logprintf("HIT PLAYER OBJECT TARGET");
							bulletSyncDataTarget.vecHitTarget = CMath::GetNearestPointToRay(bulletSyncDataTarget.vecHitOrigin, bulletSyncDataTarget.vecHitTarget, pPlayerObject->matWorld.pos);
							bulletSyncDataTarget.vecCenterOfHit = bulletSyncDataTarget.vecHitTarget - pPlayerObject->matWorld.pos;
						}
					}
				} else if (pServer->GetPlayerManager()->IsPlayerConnected(wPlayerObjectOwnerId)) {
					if (!pServer->GetPlayerManager()->IsNPC(wPlayerObjectOwnerId)) { // Handles player objects of the closestPlayer
						CObject *pPlayerObject = pNetGame->pObjectPool->pPlayerObjects[wPlayerObjectOwnerId][bulletSyncDataTarget.wHitID];
						if (pPlayerObject) {
							// logprintf("HIT PLAYER OBJECT CLOSESTPLAYER");
							bulletSyncDataTarget.vecHitTarget = CMath::GetNearestPointToRay(bulletSyncDataTarget.vecHitOrigin, bulletSyncDataTarget.vecHitTarget, pPlayerObject->matWorld.pos);
							bulletSyncDataTarget.vecCenterOfHit = bulletSyncDataTarget.vecHitTarget - pPlayerObject->matWorld.pos;
						}
					} else { // Handles player objects of the closestNPC
						CObject *pPlayerObject = pNetGame->pObjectPool->pPlayerObjects[wPlayerObjectOwnerId][bulletSyncDataTarget.wHitID];
						if (pPlayerObject) {
							// logprintf("HIT PLAYER OBJECT CLOSESTNPC");
							bulletSyncDataTarget.vecHitTarget = CMath::GetNearestPointToRay(bulletSyncDataTarget.vecHitOrigin, bulletSyncDataTarget.vecHitTarget, pPlayerObject->matWorld.pos);
							bulletSyncDataTarget.vecCenterOfHit = bulletSyncDataTarget.vecHitTarget - pPlayerObject->matWorld.pos;
						}
					}
				}
			}
			break;
	}

	// Update bullet sync data
	pPlayerData->SetBulletSync(&bulletSyncDataTarget);

	// Call FCNPC_OnWeaponShot
	int send = CCallbackManager::OnWeaponShot(wPlayerId, bulletSyncDataTarget.byteWeaponID, bulletSyncDataTarget.byteHitType, bulletSyncDataTarget.wHitID, bulletSyncDataTarget.vecCenterOfHit);
	// logprintf("TYPE %d, ID %d, %.2f, %.2f, %.2f", bulletSyncDataTarget.byteHitType, bulletSyncDataTarget.wHitID, bulletSyncDataTarget.vecCenterOfHit.fX, bulletSyncDataTarget.vecCenterOfHit.fY, bulletSyncDataTarget.vecCenterOfHit.fZ);

	// Deal damage to the target NPC if the target is an NPC
	if (send != 0) {
		if (bulletSyncDataTarget.byteHitType == BULLET_HIT_TYPE_PLAYER && pServer->GetPlayerManager()->IsNpcConnected(bulletSyncDataTarget.wHitID)) {
			CPlayerData *pHitPlayerData = pServer->GetPlayerManager()->GetAt(bulletSyncDataTarget.wHitID);

			if (pHitPlayerData && !pHitPlayerData->IsInvulnerable() && pHitPlayerData->GetState() != PLAYER_STATE_WASTED) {
				SWeaponInfo sWeaponInfo = CWeaponInfo::GetDefaultInfo(bulletSyncDataTarget.byteWeaponID);

				pHitPlayerData->ProcessDamage(wPlayerId, sWeaponInfo.fDamage, bulletSyncDataTarget.byteWeaponID, BODY_PART_TORSO);
				CCallbackManager::OnGiveDamage(wPlayerId, bulletSyncDataTarget.wHitID, bulletSyncDataTarget.byteWeaponID, BODY_PART_TORSO, sWeaponInfo.fDamage);
				// logprintf("DAMAGE NPC");
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

WORD CFunctions::GetClosestEntityInBetween(const CVector &vecHitOrigin, const CVector &vecHitTarget, float fRange, BYTE byteBetweenCheckFlags, WORD wPlayerId, WORD wTargetId, BYTE &byteEntityType, WORD &wPlayerObjectOwnerId, CVector &vecHitMap)
{
	WORD wClosestEntity = 0xFFFF;
	float fClosestEntityDistance = 0.0;
	byteEntityType = FCNPC_ENTITY_CHECK_NONE;

	// Check if a player is in between the origin and the target
	WORD wClosestPlayer = INVALID_PLAYER_ID;
	if (byteBetweenCheckFlags & FCNPC_ENTITY_CHECK_PLAYER) {
		float fClosestPlayerDistance = 0.0;
		wClosestPlayer = GetClosestPlayerInBetween(vecHitOrigin, vecHitTarget, fRange, fClosestPlayerDistance, wPlayerId, wTargetId);
		if (wClosestPlayer != INVALID_PLAYER_ID && (wClosestEntity == 0xFFFF || fClosestPlayerDistance < fClosestEntityDistance)) {
			byteEntityType = FCNPC_ENTITY_CHECK_PLAYER;
			fClosestEntityDistance = fClosestPlayerDistance;
			wClosestEntity = wClosestPlayer;
		}
	}

	// Check if an NPC is in between the origin and the target
	WORD wClosestNPC = INVALID_PLAYER_ID;
	if (byteBetweenCheckFlags & FCNPC_ENTITY_CHECK_NPC) {
		float fClosestNPCDistance = 0.0;
		wClosestNPC = GetClosestNPCInBetween(vecHitOrigin, vecHitTarget, fRange, fClosestNPCDistance, wPlayerId, wTargetId);
		if (wClosestNPC != INVALID_PLAYER_ID && (wClosestEntity == 0xFFFF || fClosestNPCDistance < fClosestEntityDistance)) {
			byteEntityType = FCNPC_ENTITY_CHECK_NPC;
			fClosestEntityDistance = fClosestNPCDistance;
			wClosestEntity = wClosestNPC;
		}
	}

	// Check if an actor is in between the origin and the target
	if (byteBetweenCheckFlags & FCNPC_ENTITY_CHECK_ACTOR) {
		float fClosestActorDistance = 0.0;
		WORD wClosestActor = GetClosestActorInBetween(vecHitOrigin, vecHitTarget, fRange, fClosestActorDistance);
		if (wClosestActor != INVALID_ACTOR_ID && (wClosestEntity == 0xFFFF || fClosestActorDistance < fClosestEntityDistance)) {
			byteEntityType = FCNPC_ENTITY_CHECK_ACTOR;
			fClosestEntityDistance = fClosestActorDistance;
			wClosestEntity = wClosestActor;
		}
	}

	// Check if a vehicle is in between the origin and the target
	if (byteBetweenCheckFlags & FCNPC_ENTITY_CHECK_VEHICLE) {
		float fClosestVehicleDistance = 0.0;
		WORD wClosestVehicle = GetClosestVehicleInBetween(vecHitOrigin, vecHitTarget, fRange, fClosestVehicleDistance);
		if (wClosestVehicle != INVALID_VEHICLE_ID && (wClosestEntity == 0xFFFF || fClosestVehicleDistance < fClosestEntityDistance)) {
			byteEntityType = FCNPC_ENTITY_CHECK_VEHICLE;
			fClosestEntityDistance = fClosestVehicleDistance;
			wClosestEntity = wClosestVehicle;
		}
	}

	// Check if an object is in between the origin and the target
	if (byteBetweenCheckFlags & FCNPC_ENTITY_CHECK_OBJECT) {
		float fClosestObjectDistance = 0.0;
		WORD wClosestObject = GetClosestObjectInBetween(vecHitOrigin, vecHitTarget, fRange, fClosestObjectDistance);
		if (wClosestObject != INVALID_OBJECT_ID && (wClosestEntity == 0xFFFF || fClosestObjectDistance < fClosestEntityDistance)) {
			byteEntityType = FCNPC_ENTITY_CHECK_OBJECT;
			fClosestEntityDistance = fClosestObjectDistance;
			wClosestEntity = wClosestObject;
		}
	}

	// Check if a player object of the shooter is in between the origin and the target
	if (byteBetweenCheckFlags & FCNPC_ENTITY_CHECK_POBJECT_ORIG) {
		float fClosestPlayerObjectDistance = 0.0;
		WORD wClosestPlayerObject = GetClosestPlayerObjectInBetween(vecHitOrigin, vecHitTarget, fRange, fClosestPlayerObjectDistance, wPlayerId);
		if (wClosestPlayerObject != INVALID_OBJECT_ID && (wClosestEntity == 0xFFFF || fClosestPlayerObjectDistance < fClosestEntityDistance)) {
			byteEntityType = FCNPC_ENTITY_CHECK_POBJECT_ORIG;
			wPlayerObjectOwnerId = wPlayerId;
			fClosestEntityDistance = fClosestPlayerObjectDistance;
			wClosestEntity = wClosestPlayerObject;
		}
	}

	if (byteBetweenCheckFlags & FCNPC_ENTITY_CHECK_POBJECT_TARG) { // One flag for 3 checks
		// Check if a player object of the target is in between the origin and the target
		if (wTargetId != INVALID_PLAYER_ID) {
			float fClosestPlayerObjectDistance = 0.0;
			WORD wClosestPlayerObject = GetClosestPlayerObjectInBetween(vecHitOrigin, vecHitTarget, fRange, fClosestPlayerObjectDistance, wTargetId);
			if (wClosestPlayerObject != INVALID_OBJECT_ID && (wClosestEntity == 0xFFFF || fClosestPlayerObjectDistance < fClosestEntityDistance)) {
				byteEntityType = FCNPC_ENTITY_CHECK_POBJECT_TARG;
				wPlayerObjectOwnerId = wTargetId;
				fClosestEntityDistance = fClosestPlayerObjectDistance;
				wClosestEntity = wClosestPlayerObject;
			}
		}

		// Check if a player object of the closestPlayer is in between the origin and the target when the closestPlayer is currently the closest entity
		if (wClosestPlayer != INVALID_PLAYER_ID && wClosestEntity == wClosestPlayer) {
			float fClosestPlayerObjectDistance = 0.0;
			WORD wClosestPlayerObject = GetClosestPlayerObjectInBetween(vecHitOrigin, vecHitTarget, fRange, fClosestPlayerObjectDistance, wClosestPlayer);
			if (wClosestPlayerObject != INVALID_OBJECT_ID && (wClosestEntity == 0xFFFF || fClosestPlayerObjectDistance < fClosestEntityDistance)) {
				byteEntityType = FCNPC_ENTITY_CHECK_POBJECT_TARG;
				wPlayerObjectOwnerId = wClosestPlayer;
				fClosestEntityDistance = fClosestPlayerObjectDistance;
				wClosestEntity = wClosestPlayerObject;
			}
		}

		// Check if a player object of the closestNPC is in between the origin and the target when the closestNPC is currently the closest entity
		if (wClosestNPC != INVALID_PLAYER_ID && wClosestEntity == wClosestNPC) {
			float fClosestPlayerObjectDistance = 0.0;
			WORD wClosestPlayerObject = GetClosestPlayerObjectInBetween(vecHitOrigin, vecHitTarget, fRange, fClosestPlayerObjectDistance, wClosestNPC);
			if (wClosestPlayerObject != INVALID_OBJECT_ID && (wClosestEntity == 0xFFFF || fClosestPlayerObjectDistance < fClosestEntityDistance)) {
				byteEntityType = FCNPC_ENTITY_CHECK_POBJECT_TARG;
				wPlayerObjectOwnerId = wClosestNPC;
				fClosestEntityDistance = fClosestPlayerObjectDistance;
				wClosestEntity = wClosestPlayerObject;
			}
		}
	}

	// Check if a map point is in between the origin and the target
	if (byteBetweenCheckFlags & FCNPC_ENTITY_CHECK_MAP) {
		float fClosestMapPointDistance = 0.0;
		WORD wClosestMapPoint = GetClosestMapPointInBetween(vecHitOrigin, vecHitTarget, fRange, fClosestMapPointDistance, vecHitMap);
		if (wClosestMapPoint != 0 && (wClosestEntity == 0xFFFF || fClosestMapPointDistance < fClosestEntityDistance)) {
			byteEntityType = FCNPC_ENTITY_CHECK_MAP;
			fClosestEntityDistance = fClosestMapPointDistance;
			wClosestEntity = wClosestMapPoint;
		}
	}

	return wClosestEntity;
}

WORD CFunctions::GetClosestPlayerInBetween(const CVector &vecHitOrigin, const CVector &vecHitTarget, float fRange, float &fDistance, WORD wPlayerId, WORD wTargetId)
{
	WORD wClosestPlayer = INVALID_PLAYER_ID;

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
		if (fPlayerDistance > fRange) {
			continue;
		}

		// Is the player closer than another player
		if (wClosestPlayer == INVALID_PLAYER_ID || fPlayerDistance < fDistance) {
			fDistance = fPlayerDistance;
			wClosestPlayer = i;
		}
	}

	return wClosestPlayer;
}

WORD CFunctions::GetClosestNPCInBetween(const CVector &vecHitOrigin, const CVector &vecHitTarget, float fRange, float &fDistance, WORD wPlayerId, WORD wTargetId)
{
	WORD wClosestNPC = INVALID_PLAYER_ID;

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
		if (fNPCDistance > fRange) {
			continue;
		}

		// Is the NPC closer than another NPC
		if (wClosestNPC == INVALID_PLAYER_ID || fNPCDistance < fDistance) {
			fDistance = fNPCDistance;
			wClosestNPC = i;
		}
	}

	return wClosestNPC;
}

WORD CFunctions::GetClosestActorInBetween(const CVector &vecHitOrigin, const CVector &vecHitTarget, float fRange, float &fDistance)
{
	WORD wClosestActor = INVALID_ACTOR_ID;

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
		if (fActorDistance > fRange) {
			continue;
		}

		// Is the actor closer than another actor
		if (wClosestActor == INVALID_ACTOR_ID || fActorDistance < fDistance) {
			fDistance = fActorDistance;
			wClosestActor = i;
		}
	}

	return wClosestActor;
}

WORD CFunctions::GetClosestVehicleInBetween(const CVector &vecHitOrigin, const CVector &vecHitTarget, float fRange, float &fDistance)
{
	WORD wClosestVehicle = INVALID_VEHICLE_ID;

	// Loop through all the vehicles
	for (WORD i = 1; i <= pNetGame->pVehiclePool->dwVehiclePoolSize; i++) { // Vehicle IDs start at 1
		// Validate the vehicle
		CVehicle *pVehicle = pNetGame->pVehiclePool->pVehicle[i];
		if (!pVehicle) {
			continue;
		}

		// Is the vehicle on the ray
		if (CMath::GetDistanceFromRayToPoint(vecHitOrigin, vecHitTarget, pVehicle->vecPosition) > MAX_HIT_RADIUS_VEHICLE) { //Don't use MAX_HIT_RADIUS
			continue;
		}

		// Is the vehicle in the damage range
		float fVehicleDistance = CMath::GetDistanceBetween3DPoints(vecHitOrigin, pVehicle->vecPosition);
		if (fVehicleDistance > fRange) {
			continue;
		}

		// Is the vehicle closer than another vehicle
		if (wClosestVehicle == INVALID_VEHICLE_ID || fVehicleDistance < fDistance) {
			fDistance = fVehicleDistance;
			wClosestVehicle = i;
		}
	}

	return wClosestVehicle;
}

WORD CFunctions::GetClosestObjectInBetween(const CVector &vecHitOrigin, const CVector &vecHitTarget, float fRange, float &fDistance)
{
	WORD wClosestObject = INVALID_OBJECT_ID;

	// Loop through all the objects
	for (WORD i = 1; i < MAX_OBJECTS; i++) { // Object IDs start at 1
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
		if (fObjectDistance > fRange) {
			continue;
		}

		// Is the object closer than another object
		if (wClosestObject == INVALID_OBJECT_ID || fObjectDistance < fDistance) {
			fDistance = fObjectDistance;
			wClosestObject = i;
		}
	}

	return wClosestObject;
}

WORD CFunctions::GetClosestPlayerObjectInBetween(const CVector &vecHitOrigin, const CVector &vecHitTarget, float fRange, float &fDistance, WORD wOwnerId)
{
	WORD wClosestPlayerObject = INVALID_OBJECT_ID;

	// Validate the owner
	if (!pServer->GetPlayerManager()->IsPlayerConnected(wOwnerId)) {
		return wClosestPlayerObject;
	}

	// Loop through all the player objects of the owner
	for (WORD i = 1; i < MAX_OBJECTS; i++) { // Player object IDs start at 1
		// Validate the player object
		CObject *pPlayerObject = pNetGame->pObjectPool->pPlayerObjects[wOwnerId][i];
		if (!pPlayerObject) {
			continue;
		}

		// Is the player object on the ray
		if (CMath::GetDistanceFromRayToPoint(vecHitOrigin, vecHitTarget, pPlayerObject->matWorld.pos) > MAX_HIT_RADIUS) {
			continue;
		}

		// Is the player object in the damage range
		float fPlayerObjectDistance = CMath::GetDistanceBetween3DPoints(vecHitOrigin, pPlayerObject->matWorld.pos);
		if (fPlayerObjectDistance > fRange) {
			continue;
		}

		// Is the player object closer than another player object
		if (wClosestPlayerObject == INVALID_OBJECT_ID || fPlayerObjectDistance < fDistance) {
			fDistance = fPlayerObjectDistance;
			wClosestPlayerObject = i;
		}
	}

	return wClosestPlayerObject;
}

WORD CFunctions::GetClosestMapPointInBetween(const CVector &vecHitOrigin, const CVector &vecHitTarget, float fRange, float &fDistance, CVector &vecHitMap)
{
	//TODO
	//1) GetClosestMapPointInBetween:
	//- implement when ColAndreas is enabled, otherwise return nothing (0).
	//- currently the code handles map points when the hit type is BULLET_HIT_TYPE_NONE and the hit ID is -1.
	//- this function should specificly check for map points only, not for global objects or custom objects!
	//- keep in mind that bullets can penetrate water and still deal damage.
	//- store the collision point in vecHitMap

	//2) GetClosestObjectInBetween:
	//- improve when ColAndreas is enabled, otherwise fall back on existing code.
	//- this function should specificly check for global objects only, not for custom objects or map points!
	//- keep in mind that bullets can penetrate water and still deal damage.
	
	//3) GetClosestPlayerObjectInBetween:
	//- improve when ColAndreas is enabled, otherwise fall back on existing code.
	//- this function should specificly check for custom objects only, not for global objects or map points!
	//- keep in mind that bullets can penetrate water and still deal damage.

	//4) FCNPC_SHOOT_MODE_X:
	//- add FCNPC_AimAt, FCNPC_AimAtPlayer, FCNPC_TriggerWeaponShot extra parameter with same effect as FCNPC_MOVE_MODE_X, called FCNPC_SHOOT_MODE_X

	return 0;
}


WORD CFunctions::RayCastLine(const CVector &vecStart, const CVector &vecEnd, CVector *vecResult)
{
	btVector3 Start = btVector3(btScalar(vecStart.fX + 0.00001), btScalar(vecStart.fY + 0.00001), btScalar(vecStart.fZ + 0.00001));
	btVector3 End = btVector3(btScalar(vecEnd.fX), btScalar(vecEnd.fY), btScalar(vecEnd.fZ));
	btVector3 Result;
	WORD wModel = 0;

	if (pServer->GetColAndreas()->performRayTest(Start, End, Result, wModel)) {
		vecResult->fX = Result.getX();
		vecResult->fY = Result.getY();
		vecResult->fZ = Result.getZ();

		return wModel;
	}

	return 0;
}