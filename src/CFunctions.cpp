/* =========================================

		FCNPC - Fully Controllable NPC
			----------------------

	- File: SAMPFunctions.cpp
	- Author(s): OrMisicL

  =========================================*/


#include "Main.hpp"

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
	pfn__GetNetGame = reinterpret_cast<GetNetGame_t>(ppPluginData[PLUGIN_DATA_NETGAME]);
	pNetGame = reinterpret_cast<CNetGame*>(pfn__GetNetGame());

	pfn__GetConsole = reinterpret_cast<GetConsole_t>(ppPluginData[PLUGIN_DATA_CONSOLE]);
	pConsole = reinterpret_cast<void*>(pfn__GetConsole());

	pfn__GetRakServer = reinterpret_cast<GetRakServer_t>(ppPluginData[PLUGIN_DATA_RAKSERVER]);
	pRakServer = reinterpret_cast<RakServer*>(pfn__GetRakServer());

	int *pRakServer_VTBL = reinterpret_cast<int*>(*reinterpret_cast<void**>(pRakServer));

	CUtils::UnProtect(pRakServer_VTBL[RAKNET_SEND_OFFSET], 4);
	CUtils::UnProtect(pRakServer_VTBL[RAKNET_RPC_OFFSET], 4);
	CUtils::UnProtect(pRakServer_VTBL[RAKNET_RECEIVE_OFFSET], 4);

	pfn__RakNet__Send = reinterpret_cast<RakNet__Send_t>(pRakServer_VTBL[RAKNET_SEND_OFFSET]);
	pfn__RakNet__RPC = reinterpret_cast<RakNet__RPC_t>(pRakServer_VTBL[RAKNET_RPC_OFFSET]);
	pfn__RakNet__Receive = reinterpret_cast<RakNet__Receive_t>(pRakServer_VTBL[RAKNET_RECEIVE_OFFSET]);
}

int CFunctions::GetNetGame()
{
	return pfn__GetNetGame();
}

int CFunctions::GetConsole()
{
	return pfn__GetConsole();
}

int CFunctions::GetRakServer()
{
	return pfn__GetRakServer();
}

WORD CFunctions::GetFreePlayerSlot()
{
	// Loop through all the players
	for (WORD i = GetMaxPlayers() - 1; i >= 0; i--) {
		// Is he not connected ?
		if (!pNetGame->pPlayerPool->bIsPlayerConnectedEx[i]) {
			return i;
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

	//reinterpret_cast<CSAMPRakPeer*>(pRakServer)->SetDisonnectedPlayer(wPlayerId);

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

CVector *CFunctions::GetVehicleModelInfoEx(DWORD dwModelID, int iInfoType)
{
	return pfn__GetVehicleModelInfo(dwModelID, iInfoType);
}

WORD CFunctions::GetMaxPlayers()
{
	return static_cast<WORD>(pfn__CConsole__GetIntVariable(pConsole, "maxplayers"));
}

WORD CFunctions::GetMaxNPC()
{
	return static_cast<WORD>(pfn__CConsole__GetIntVariable(pConsole, "maxnpc"));
}

void CFunctions::PlayerShoot(WORD wPlayerId, WORD wHitId, BYTE byteHitType, BYTE byteWeaponId, CVector vecPoint, CVector vecOffsetFrom, bool bIsHit)
{
	// Validate the player
	if (!pServer->GetPlayerManager()->IsNpcConnected(wPlayerId)) {
		return;
	}

	// Get the player
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wPlayerId);

	// Get the origin hit vector position
	CVector vecOrigin;
	pPlayerData->GetPosition(&vecOrigin);
	vecOrigin += vecOffsetFrom;

	// Create the SendBullet structure
	CBulletSyncData bulletSyncData;
	if (bIsHit) {
		bulletSyncData.wHitID = wHitId;
		bulletSyncData.byteHitType = byteHitType;
	} else {
		bulletSyncData.wHitID = INVALID_PLAYER_ID;
		bulletSyncData.byteHitType = BULLET_HIT_TYPE_NONE;
	}
	bulletSyncData.byteWeaponID = byteWeaponId;
	bulletSyncData.vecCenterOfHit = CVector();
	bulletSyncData.vecHitOrigin = vecOrigin;
	bulletSyncData.vecHitTarget = vecPoint;

	// find player in vecPoint
	if (bIsHit && bulletSyncData.byteHitType == BULLET_HIT_TYPE_NONE) {
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
				bulletSyncData.byteHitType = BULLET_HIT_TYPE_PLAYER;
				bulletSyncData.wHitID = i;
				bulletSyncData.vecHitTarget = CMath::GetNearestPointToRay(vecOrigin, vecPoint, pPlayer->vecPosition);
				break;
			}
		}
	}

	// get center of hit
	switch (bulletSyncData.byteHitType) {
		case BULLET_HIT_TYPE_NONE:
			bulletSyncData.vecCenterOfHit = bulletSyncData.vecHitTarget;
			break;
		case BULLET_HIT_TYPE_PLAYER:
			if (bulletSyncData.wHitID >= 0 && bulletSyncData.wHitID < MAX_PLAYERS) {
				CPlayer *pHitPlayerData = pNetGame->pPlayerPool->pPlayer[bulletSyncData.wHitID];
				if (pHitPlayerData) {
					bulletSyncData.vecCenterOfHit = bulletSyncData.vecHitTarget - pHitPlayerData->vecPosition;
				}
			}
			break;
		case BULLET_HIT_TYPE_VEHICLE:
			if (bulletSyncData.wHitID >= 1 && bulletSyncData.wHitID < MAX_VEHICLES) {
				CVehicle *pVehicle = pNetGame->pVehiclePool->pVehicle[bulletSyncData.wHitID];
				if (pVehicle) {
					bulletSyncData.vecCenterOfHit = bulletSyncData.vecHitTarget - pVehicle->vecPosition;
				}
			}
			break;
		case BULLET_HIT_TYPE_OBJECT:
			if (bulletSyncData.wHitID >= 1 && bulletSyncData.wHitID < MAX_OBJECTS) {
				CObject *pObject = pNetGame->pObjectPool->pObjects[bulletSyncData.wHitID];
				if (pObject) {
					bulletSyncData.vecCenterOfHit = bulletSyncData.vecHitTarget - pObject->matWorld.pos;
				}
			}
			break;
		case BULLET_HIT_TYPE_PLAYER_OBJECT:
			if (bulletSyncData.wHitID >= 1 && bulletSyncData.wHitID < MAX_OBJECTS) {
				CObject *pObject = pNetGame->pObjectPool->pPlayerObjects[wPlayerId][bulletSyncData.wHitID];
				if (pObject) {
					bulletSyncData.vecCenterOfHit = bulletSyncData.vecHitTarget - pObject->matWorld.pos;
				}
			}
			break;
	}

	// update bullet sync data
	pPlayerData->SetBulletSync(&bulletSyncData);

	// call FCNPC_OnWeaponShot
	int send = CCallbackManager::OnWeaponShot(wPlayerId, bulletSyncData.wHitID, bulletSyncData.byteHitType, bulletSyncData.byteWeaponID, bulletSyncData.vecCenterOfHit);
	if (send != 0) {
		// if it is a NPC
		if (bIsHit && bulletSyncData.byteHitType == BULLET_HIT_TYPE_PLAYER && pServer->GetPlayerManager()->IsNpcConnected(bulletSyncData.wHitID)) {
			CPlayerData *pHitPlayerData = pServer->GetPlayerManager()->GetAt(bulletSyncData.wHitID);

			if (pHitPlayerData && !pHitPlayerData->IsInvulnerable() && pHitPlayerData->GetState() != PLAYER_STATE_WASTED) {
				SWeaponInfo sWeaponInfo = CWeaponInfo::GetDefaultInfo(byteWeaponId);

				pHitPlayerData->ProcessDamage(wPlayerId, sWeaponInfo.fDamage, byteWeaponId, BODY_PART_TORSO);
				CCallbackManager::OnGiveDamage(wPlayerId, bulletSyncData.wHitID, byteWeaponId, BODY_PART_TORSO, sWeaponInfo.fDamage);
			}
		}

		// Write it to BitStream
		RakNet::BitStream bsSend;
		bsSend.Write(static_cast<BYTE>(ID_BULLET_SYNC));
		bsSend.Write(wPlayerId);
		bsSend.Write(reinterpret_cast<char*>(&bulletSyncData), sizeof(CBulletSyncData));

		// Send it
		CFunctions::GlobalPacket(&bsSend);
	}
}

void CFunctions::GlobalRPC(int* szUniqueID, RakNet::BitStream* bsParams, WORD wExcludePlayerId, char PacketStream)
{
	PacketReliability reliable = RELIABLE_ORDERED;

	if (PacketStream == 3) {
		reliable = RELIABLE;
	}

	if (wExcludePlayerId == INVALID_PLAYER_ID) {
		pRakServer->RPC(szUniqueID, bsParams, HIGH_PRIORITY, reliable, PacketStream, UNASSIGNED_PLAYER_ID, true, false);
	} else {
		pRakServer->RPC(szUniqueID, bsParams, HIGH_PRIORITY, reliable, PacketStream, pRakServer->GetPlayerIDFromIndex(wExcludePlayerId), true, false);
	}

}

void CFunctions::AddedPlayersRPC(int* szUniqueID, RakNet::BitStream* bsParams, WORD wPlayerId, char PacketStream)
{
	CPlayer *pPlayer;

	for (WORD i = 0; i <= pNetGame->pPlayerPool->dwPlayerPoolSize; i++) {
		if (pNetGame->pPlayerPool->bIsPlayerConnectedEx[i] && i != wPlayerId) {
			pPlayer = pNetGame->pPlayerPool->pPlayer[i];

			if (pPlayer && pPlayer->byteStreamedIn[wPlayerId]) {
				pRakServer->RPC(szUniqueID, bsParams, HIGH_PRIORITY, RELIABLE_ORDERED, PacketStream, pRakServer->GetPlayerIDFromIndex(i), false, false);
			}
		}
	}
}

void CFunctions::AddedVehicleRPC(int* szUniqueID, RakNet::BitStream* bsParams, WORD wVehicleId, WORD wExcludePlayerId, char PacketStream)
{
	CPlayer *pPlayer;

	for (WORD i = 0; i <= pNetGame->pPlayerPool->dwPlayerPoolSize; i++) {
		if (pNetGame->pPlayerPool->bIsPlayerConnectedEx[i] && i != wExcludePlayerId) {
			pPlayer = pNetGame->pPlayerPool->pPlayer[i];

			if (pPlayer && pPlayer->byteVehicleStreamedIn[wVehicleId]) {
				pRakServer->RPC(szUniqueID, bsParams, HIGH_PRIORITY, RELIABLE_ORDERED, PacketStream, pRakServer->GetPlayerIDFromIndex(i), false, false);
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

	pRakServer->RPC(szUniqueID, bsParams, HIGH_PRIORITY, reliable, PacketStream, pRakServer->GetPlayerIDFromIndex(wPlayerId), false, false);
}

void CFunctions::GlobalPacket(RakNet::BitStream* bsParams)
{
	pRakServer->Send(bsParams, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 1, UNASSIGNED_PLAYER_ID, true);
}

void CFunctions::PlayerPacket(RakNet::BitStream* bsParams, WORD wPlayerId)
{
	pRakServer->Send(bsParams, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 1, pRakServer->GetPlayerIDFromIndex(wPlayerId), false);
}
