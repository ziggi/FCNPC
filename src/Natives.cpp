/* =========================================

		FCNPC - Fully Controllable NPC
			----------------------

	- File: Natives.cpp
	- Author(s): OrMisicL

  =========================================*/

#include "Main.hpp"

extern CServer     *pServer;
extern CNetGame    *pNetGame;

cell AMX_NATIVE_CALL CNatives::FCNPC_Create(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_Create");

	// Get the params
	char *szName;
	amx_StrParam(amx, params[1], szName);

	return pServer->GetPlayerManager()->AddPlayer(szName);
}

cell AMX_NATIVE_CALL CNatives::FCNPC_Destroy(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_Destroy");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Destoy the player
	return pServer->GetPlayerManager()->DeletePlayer(wNpcId);
}

cell AMX_NATIVE_CALL CNatives::FCNPC_Spawn(AMX *amx, cell *params)
{
	CHECK_PARAMS(5, "FCNPC_Spawn");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	int iSkin = static_cast<int>(params[2]);
	CVector vecPoint(amx_ctof(params[3]), amx_ctof(params[4]), amx_ctof(params[5]));

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Set the spawn position
	pPlayerData->SetSpawnPosition(vecPoint);
	// Spawn the player
	return pPlayerData->Spawn(iSkin);
}

cell AMX_NATIVE_CALL CNatives::FCNPC_Respawn(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_Respawn");
	// Get the NPC id
	WORD wNpcId = static_cast<WORD>(params[1]);
	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Respawn the player
	return pPlayerData->Respawn();
}

cell AMX_NATIVE_CALL CNatives::FCNPC_IsSpawned(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_IsSpawned");
	// Get the NPC id
	WORD wNpcId = static_cast<WORD>(params[1]);
	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Respawn the player
	return pPlayerData->IsSpawned();
}

cell AMX_NATIVE_CALL CNatives::FCNPC_Kill(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_Kill");
	// Get the NPC id
	WORD wNpcId = static_cast<WORD>(params[1]);
	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Kill the player
	return pPlayerData->Kill(INVALID_PLAYER_ID, 255);
}

cell AMX_NATIVE_CALL CNatives::FCNPC_IsDead(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_IsDead");
	// Get the NPC id
	WORD wNpcId = static_cast<WORD>(params[1]);
	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Return the player dead state
	return pPlayerData->GetState() == PLAYER_STATE_WASTED;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_IsValid(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_IsValid");
	// Get the NPC id
	WORD wNpcId = static_cast<WORD>(params[1]);
	// Return the player is valid
	return pServer->GetPlayerManager()->IsNpcConnected(wNpcId);
}

cell AMX_NATIVE_CALL CNatives::FCNPC_IsStreamedIn(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_IsStreamedIn");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	WORD wForPlayerId = static_cast<WORD>(params[2]);

	// Make sure the npc is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsPlayerConnected(wForPlayerId) || wForPlayerId == wNpcId) {
		return 0;
	}

	// Return the player is streamed
	return pPlayerData->IsStreamedIn(wForPlayerId);
}

cell AMX_NATIVE_CALL CNatives::FCNPC_IsStreamedForAnyone(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_IsStreamedForAnyone");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the npc is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	for (WORD i = 0; i <= pNetGame->pPlayerPool->dwPlayerPoolSize; i++) {
		// Ignore non connected players and the same player
		if (!pServer->GetPlayerManager()->IsPlayerConnected(i) || wNpcId == i) {
			continue;
		}

		if (pPlayerData->IsStreamedIn(i)) {
			return 1;
		}
	}

	// Return the player is streamed
	return 0;
}

// native FCNPC_GetValidArray(npcs[], const size = sizeof(npcs));
cell AMX_NATIVE_CALL CNatives::FCNPC_GetValidArray(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_GetValidArray");

	// get the params
	int iSize = static_cast<int>(params[2]);

	// fill the array with ids
	cell *pAddress = NULL;
	amx_GetAddr(amx, params[1], &pAddress);

	int iCount = 0;
	for (auto &id : pServer->GetPlayerManager()->m_vNpcID) {
		*(pAddress + iCount) = id;

		iCount++;
		if (iCount >= iSize) {
			break;
		}
	}

	// Return the count of players
	return iCount;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_SetPosition(AMX *amx, cell *params)
{
	CHECK_PARAMS(4, "FCNPC_SetPosition");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	CVector vecPoint(amx_ctof(params[2]), amx_ctof(params[3]), amx_ctof(params[4]));

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Set the player position
	pPlayerData->SetPosition(vecPoint);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GivePosition(AMX *amx, cell *params)
{
	CHECK_PARAMS(4, "FCNPC_GivePosition");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	CVector vecPoint(amx_ctof(params[2]), amx_ctof(params[3]), amx_ctof(params[4]));

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Set the player position
	CVector vecOldPoint;
	pPlayerData->GetPosition(&vecOldPoint);
	pPlayerData->SetPosition(vecOldPoint + vecPoint);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetPosition(AMX *amx, cell *params)
{
	CHECK_PARAMS(4, "FCNPC_GetPosition");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		// Get the argument pointers and set its value
		cell *pAddress = NULL;
		float fPos = 0.0f;
		amx_GetAddr(amx, params[2], &pAddress);
		*pAddress = amx_ftoc(fPos);

		amx_GetAddr(amx, params[3], &pAddress);
		*pAddress = amx_ftoc(fPos);

		amx_GetAddr(amx, params[4], &pAddress);
		*pAddress = amx_ftoc(fPos);
		return 0;
	}

	// Get the player position
	CVector vecPosition;
	pPlayerData->GetPosition(&vecPosition);

	// Get the argument pointers and set its value
	cell *pAddress = NULL;
	amx_GetAddr(amx, params[2], &pAddress);
	*pAddress = amx_ftoc(vecPosition.fX);

	amx_GetAddr(amx, params[3], &pAddress);
	*pAddress = amx_ftoc(vecPosition.fY);

	amx_GetAddr(amx, params[4], &pAddress);
	*pAddress = amx_ftoc(vecPosition.fZ);

	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_SetAngle(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_SetAngle");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	float fAngle = amx_ctof(params[2]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Set the player angle
	pPlayerData->SetAngle(fAngle);
	return 1;
}

// native FCNPC_GiveAngle(npcid, Float:angle);
cell AMX_NATIVE_CALL CNatives::FCNPC_GiveAngle(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_GiveAngle");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	float fAngle = amx_ctof(params[2]);
	float fNewAngle = 0.0f;

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return amx_ftoc(fNewAngle);
	}

	// Set the player angle
	fNewAngle = pPlayerData->GetAngle() + fAngle;
	pPlayerData->SetAngle(fNewAngle);
	return amx_ftoc(fNewAngle);
}

// native FCNPC_SetAngleToPos(npcid, Float:X, Float:Y);
cell AMX_NATIVE_CALL CNatives::FCNPC_SetAngleToPos(AMX *amx, cell *params)
{
	CHECK_PARAMS(3, "FCNPC_SetAngleToPos");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	CVector vecPoint(amx_ctof(params[2]), amx_ctof(params[3]), 0.0f);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Set the player angle
	CVector vecPos;
	pPlayerData->GetPosition(&vecPos);

	CVector vecFront = vecPoint - vecPos;
	pPlayerData->SetAngle(CMath::GetAngle(vecFront.fX, vecFront.fY));
	return 1;
}

// native FCNPC_SetAngleToPlayer(npcid, playerid);
cell AMX_NATIVE_CALL CNatives::FCNPC_SetAngleToPlayer(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_SetAngleToPlayer");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	WORD wPlayerId = static_cast<WORD>(params[2]);

	// Make sure the npc is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsPlayerConnected(wPlayerId) || wPlayerId == wNpcId) {
		return 0;
	}

	// Set the player angle
	CVector vecPlayerPos = pNetGame->pPlayerPool->pPlayer[wPlayerId]->vecPosition;

	CVector vecPos;
	pPlayerData->GetPosition(&vecPos);

	CVector vecFront = vecPlayerPos - vecPos;
	pPlayerData->SetAngle(CMath::GetAngle(vecFront.fX, vecFront.fY));
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetAngle(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetAngle");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	float fAngle = 0.0f;

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return amx_ftoc(fAngle);
	}

	// Get the player angle
	fAngle = pPlayerData->GetAngle();
	return amx_ftoc(fAngle);
}

cell AMX_NATIVE_CALL CNatives::FCNPC_SetInterior(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_SetInterior");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	int iInterior = static_cast<int>(params[2]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Set the player interior
	pPlayerData->SetInterior(iInterior);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetInterior(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetInterior");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Get the player interior
	return pPlayerData->GetInterior();
}

cell AMX_NATIVE_CALL CNatives::FCNPC_SetVirtualWorld(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_SetVirtualWorld");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	int iVirtualWorld = static_cast<int>(params[2]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Set the player virtual world
	pPlayerData->SetVirtualWorld(iVirtualWorld);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetVirtualWorld(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetVirtualWorld");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Get the player virtual world
	return pPlayerData->GetVirtualWorld();
}

cell AMX_NATIVE_CALL CNatives::FCNPC_SetQuaternion(AMX *amx, cell *params)
{
	CHECK_PARAMS(5, "FCNPC_SetQuaternion");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	float *fQuaternion = new float [4];
	fQuaternion[0] = amx_ctof(params[2]);
	fQuaternion[1] = amx_ctof(params[3]);
	fQuaternion[2] = amx_ctof(params[4]);
	fQuaternion[3] = amx_ctof(params[5]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Set the player quaternion
	pPlayerData->SetQuaternion(fQuaternion);
	SAFE_DELETE(fQuaternion);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GiveQuaternion(AMX *amx, cell *params)
{
	CHECK_PARAMS(5, "FCNPC_GiveQuaternion");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	float *fQuaternion = new float[4];
	fQuaternion[0] = amx_ctof(params[2]);
	fQuaternion[1] = amx_ctof(params[3]);
	fQuaternion[2] = amx_ctof(params[4]);
	fQuaternion[3] = amx_ctof(params[5]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Set the player quaternion
	float *fOldQuaternion = new float[4];
	pPlayerData->GetQuaternion(fOldQuaternion);

	float *fNewQuaternion = new float[4];
	fNewQuaternion[0] = fOldQuaternion[0] + fQuaternion[0];
	fNewQuaternion[1] = fOldQuaternion[1] + fQuaternion[1];
	fNewQuaternion[2] = fOldQuaternion[2] + fQuaternion[2];
	fNewQuaternion[3] = fOldQuaternion[3] + fQuaternion[3];

	pPlayerData->SetQuaternion(fNewQuaternion);
	SAFE_DELETE(fQuaternion);
	SAFE_DELETE(fOldQuaternion);
	SAFE_DELETE(fNewQuaternion);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetQuaternion(AMX *amx, cell *params)
{
	CHECK_PARAMS(5, "FCNPC_GetQuaternion");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		// Get the argument pointers and set its value
		cell *pAddress = NULL;
		float fPos = 0.0f;
		amx_GetAddr(amx, params[2], &pAddress);
		*pAddress = amx_ftoc(fPos);

		amx_GetAddr(amx, params[3], &pAddress);
		*pAddress = amx_ftoc(fPos);

		amx_GetAddr(amx, params[4], &pAddress);
		*pAddress = amx_ftoc(fPos);

		amx_GetAddr(amx, params[5], &pAddress);
		*pAddress = amx_ftoc(fPos);
		return 0;
	}

	// Get the player quaternion
	float *fQuaternion = new float [4];
	pPlayerData->GetQuaternion(fQuaternion);

	// Get the argument pointers and set its value
	cell *pAddress = NULL;
	amx_GetAddr(amx, params[2], &pAddress);
	*pAddress = amx_ftoc(fQuaternion[0]);

	amx_GetAddr(amx, params[3], &pAddress);
	*pAddress = amx_ftoc(fQuaternion[1]);

	amx_GetAddr(amx, params[4], &pAddress);
	*pAddress = amx_ftoc(fQuaternion[2]);

	amx_GetAddr(amx, params[5], &pAddress);
	*pAddress = amx_ftoc(fQuaternion[3]);

	SAFE_DELETE(fQuaternion);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_SetVelocity(AMX *amx, cell *params)
{
	CHECK_PARAMS(5, "FCNPC_SetVelocity");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	CVector vecVelocity(amx_ctof(params[2]), amx_ctof(params[3]), amx_ctof(params[4]));
	bool bUpdatePos = static_cast<int>(params[5]) != 0;

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Set the player velocity
	pPlayerData->SetVelocity(vecVelocity, bUpdatePos);
	return 1;
}

// native FCNPC_GiveVelocity(npcid, Float:x, Float:y, Float:z);
cell AMX_NATIVE_CALL CNatives::FCNPC_GiveVelocity(AMX *amx, cell *params)
{
	CHECK_PARAMS(5, "FCNPC_GiveVelocity");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	CVector vecVelocity(amx_ctof(params[2]), amx_ctof(params[3]), amx_ctof(params[4]));
	bool bUpdatePos = static_cast<int>(params[5]) != 0;

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Set the player velocity
	CVector vecOldVelocity;
	pPlayerData->GetVelocity(&vecOldVelocity);
	pPlayerData->SetVelocity(vecOldVelocity + vecVelocity, bUpdatePos);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetVelocity(AMX *amx, cell *params)
{
	CHECK_PARAMS(4, "FCNPC_GetVelocity");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		// Get the argument pointers and set its value
		cell *pAddress = NULL;
		float fPos = 0.0f;
		amx_GetAddr(amx, params[2], &pAddress);
		*pAddress = amx_ftoc(fPos);

		amx_GetAddr(amx, params[3], &pAddress);
		*pAddress = amx_ftoc(fPos);

		amx_GetAddr(amx, params[4], &pAddress);
		*pAddress = amx_ftoc(fPos);
		return 0;
	}

	// Get the player velocity
	CVector vecVelocity;
	pPlayerData->GetVelocity(&vecVelocity);

	// Get the argument pointers and set its value
	cell *pAddress = NULL;
	amx_GetAddr(amx, params[2], &pAddress);
	*pAddress = amx_ftoc(vecVelocity.fX);

	amx_GetAddr(amx, params[3], &pAddress);
	*pAddress = amx_ftoc(vecVelocity.fY);

	amx_GetAddr(amx, params[4], &pAddress);
	*pAddress = amx_ftoc(vecVelocity.fZ);
	return 1;
}

// native FCNPC_SetSpeed(npcid, Float:speed);
cell AMX_NATIVE_CALL CNatives::FCNPC_SetSpeed(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_SetSpeed");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	float fSpeed = amx_ctof(params[2]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Set the player velocity
	pPlayerData->SetSpeed(fSpeed);
	return 1;
}

// native Float:FCNPC_GetSpeed(npcid);
cell AMX_NATIVE_CALL CNatives::FCNPC_GetSpeed(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetSpeed");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	float fSpeed = 0.0f;

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return amx_ftoc(fSpeed);
	}

	// Get the player health
	fSpeed = pPlayerData->GetSpeed();
	return amx_ftoc(fSpeed);
}

cell AMX_NATIVE_CALL CNatives::FCNPC_SetHealth(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_SetHealth");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	float fHealth = amx_ctof(params[2]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Set the player health
	pPlayerData->SetHealth(fHealth);
	return 1;
}

// native Float:FCNPC_GiveHealth(npcid, Float:health);
cell AMX_NATIVE_CALL CNatives::FCNPC_GiveHealth(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_GiveHealth");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	float fHealth = amx_ctof(params[2]);
	float fNewHealth = 0.0f;

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return amx_ftoc(fNewHealth);
	}

	// Set the player health
	fNewHealth = pPlayerData->GetHealth() + fHealth;
	pPlayerData->SetHealth(fNewHealth);
	return amx_ftoc(fNewHealth);
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetHealth(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetHealth");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	float fHealth = 0.0f;

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return amx_ftoc(fHealth);
	}

	// Get the player health
	fHealth = pPlayerData->GetHealth();
	return amx_ftoc(fHealth);
}

cell AMX_NATIVE_CALL CNatives::FCNPC_SetArmour(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_SetArmour");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	float fArmour = amx_ctof(params[2]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Set the player armour
	pPlayerData->SetArmour(fArmour);
	return 1;
}

// native FCNPC_GiveArmour(npcid, Float:armour);
cell AMX_NATIVE_CALL CNatives::FCNPC_GiveArmour(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_GiveArmour");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	float fArmour = amx_ctof(params[2]);
	float fNewArmour = 0.0f;

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return amx_ftoc(fNewArmour);
	}

	// Set the player armour
	fNewArmour = pPlayerData->GetArmour() + fArmour;
	pPlayerData->SetArmour(fNewArmour);
	return amx_ftoc(fNewArmour);
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetArmour(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetArmour");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	float fArmour = 0.0f;

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return amx_ftoc(fArmour);
	}

	// Get the player armour
	fArmour = pPlayerData->GetArmour();
	return amx_ftoc(fArmour);
}

// native FCNPC_SetInvulnerable(npcid, bool:invulnerable = true);
cell AMX_NATIVE_CALL CNatives::FCNPC_SetInvulnerable(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_SetInvulnerable");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	bool bInvulnerable = static_cast<int>(params[2]) != 0;

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Set the player invulnerable
	pPlayerData->SetInvulnerable(bInvulnerable);
	return 1;
}

// native FCNPC_IsInvulnerable(npcid);
cell AMX_NATIVE_CALL CNatives::FCNPC_IsInvulnerable(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_IsInvulnerable");

	// Get the NPC id
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Get the player invulnerable
	return pPlayerData->IsInvulnerable();
}

cell AMX_NATIVE_CALL CNatives::FCNPC_SetSkin(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_SetSkin");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	int iSkin = static_cast<int>(params[2]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Set the player skin
	pPlayerData->SetSkin(iSkin);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetSkin(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetSkin");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Get the player skin
	return pPlayerData->GetSkin();
}

cell AMX_NATIVE_CALL CNatives::FCNPC_SetKeys(AMX *amx, cell *params)
{
	CHECK_PARAMS(4, "FCNPC_SetKeys");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	WORD wUDAnalog = static_cast<WORD>(params[2]);
	WORD wLRAnalog = static_cast<WORD>(params[3]);
	DWORD dwKeys = static_cast<DWORD>(params[4]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Set the player keys
	pPlayerData->SetKeys(wUDAnalog, wLRAnalog, dwKeys);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetKeys(AMX *amx, cell *params)
{
	CHECK_PARAMS(4, "FCNPC_GetKeys");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		// Get the argument pointers and set its value
		cell *pAddress = NULL;
		amx_GetAddr(amx, params[2], &pAddress);
		*pAddress = 0;

		amx_GetAddr(amx, params[3], &pAddress);
		*pAddress = 0;

		amx_GetAddr(amx, params[4], &pAddress);
		*pAddress = 0;
		return 0;
	}

	// Get the player keys
	WORD wUDAnalog = 0, wLRAnalog = 0;
	DWORD dwKeys = 0;
	pPlayerData->GetKeys(&wUDAnalog, &wLRAnalog, &dwKeys);

	// Get the argument pointers and set its value
	cell *pAddress = NULL;
	amx_GetAddr(amx, params[2], &pAddress);
	*pAddress = wUDAnalog;

	amx_GetAddr(amx, params[3], &pAddress);
	*pAddress = wLRAnalog;

	amx_GetAddr(amx, params[4], &pAddress);
	*pAddress = dwKeys;
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_SetSpecialAction(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_SetSpecialAction");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	BYTE byteActionId = static_cast<BYTE>(params[2]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Set the player special action
	pPlayerData->SetSpecialAction(byteActionId);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetSpecialAction(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetSpecialAction");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Get the player special action
	return pPlayerData->GetSpecialAction();
}

// native FCNPC_SetAnimation(npcid, animationid, Float:fDelta = 4.1, loop = 0, lockx = 1, locky = 1, freeze = 0, time = 1);
cell AMX_NATIVE_CALL CNatives::FCNPC_SetAnimation(AMX *amx, cell *params)
{
	CHECK_PARAMS(8, "FCNPC_SetAnimation");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);
	WORD wAnimationId = static_cast<WORD>(params[2]);
	float fDelta = amx_ctof(params[3]);
	bool bLoop = static_cast<int>(params[4]) != 0;
	bool bLockX = static_cast<int>(params[5]) != 0;
	bool bLockY = static_cast<int>(params[6]) != 0;
	bool bFreeze = static_cast<int>(params[7]) != 0;
	int iTime = static_cast<int>(params[8]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Set the player animation
	pPlayerData->SetAnimation(wAnimationId, fDelta, bLoop, bLockX, bLockY, bFreeze, iTime);
	return 1;
}

// native FCNPC_SetAnimationByName(npcid, name[], Float:fDelta = 4.1, loop = 0, lockx = 1, locky = 1, freeze = 0, time = 1);
cell AMX_NATIVE_CALL CNatives::FCNPC_SetAnimationByName(AMX *amx, cell *params)
{
	CHECK_PARAMS(8, "FCNPC_SetAnimationByName");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);
	char *szName;
	amx_StrParam(amx, params[2], szName);
	float fDelta = amx_ctof(params[3]);
	bool bLoop = static_cast<int>(params[4]) != 0;
	bool bLockX = static_cast<int>(params[5]) != 0;
	bool bLockY = static_cast<int>(params[6]) != 0;
	bool bFreeze = static_cast<int>(params[7]) != 0;
	int iTime = static_cast<int>(params[8]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Set the player animation
	pPlayerData->SetAnimationByName(szName, fDelta, bLoop, bLockX, bLockY, bFreeze, iTime);
	return 1;
}

// native FCNPC_ResetAnimation(npcid);
cell AMX_NATIVE_CALL CNatives::FCNPC_ResetAnimation(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_ResetAnimation");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Set the player animation
	pPlayerData->ResetAnimation();
	return 1;
}

// native FCNPC_ApplyAnimation(npcid, animlib[], animname[], Float:fDelta = 4.1, loop = 0, lockx = 1, locky = 1, freeze = 0, time = 1);
cell AMX_NATIVE_CALL CNatives::FCNPC_ApplyAnimation(AMX *amx, cell *params)
{
	CHECK_PARAMS(9, "FCNPC_ApplyAnimation");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);
	char *szAnimLib, *szAnimName;
	amx_StrParam(amx, params[2], szAnimLib);
	amx_StrParam(amx, params[3], szAnimName);
	float fDelta = amx_ctof(params[4]);
	bool bLoop = static_cast<int>(params[5]) != 0;
	bool bLockX = static_cast<int>(params[6]) != 0;
	bool bLockY = static_cast<int>(params[7]) != 0;
	bool bFreeze = static_cast<int>(params[8]) != 0;
	int iTime = static_cast<int>(params[9]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Set the player animation
	pPlayerData->ApplyAnimation(szAnimLib, szAnimName, fDelta, bLoop, bLockX, bLockY, bFreeze, iTime);
	return 1;
}

// native FCNPC_ClearAnimations(npcid);
cell AMX_NATIVE_CALL CNatives::FCNPC_ClearAnimations(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_ClearAnimations");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Set the player animation
	pPlayerData->ClearAnimations();
	return 1;
}

// native FCNPC_GetAnimation(npcid, &animationid = 0, &Float:fDelta = 4.1, &loop = 0, &lockx = 1, &locky = 1, &freeze = 0, &time = 1);
cell AMX_NATIVE_CALL CNatives::FCNPC_GetAnimation(AMX *amx, cell *params)
{
	CHECK_PARAMS(8, "FCNPC_GetAnimation");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Get parms
	WORD wAnimationId;
	float fDelta;
	bool bLoop;
	bool bLockX;
	bool bLockY;
	bool bFreeze;
	int iTime;

	// Get the player animation
	pPlayerData->GetAnimation(&wAnimationId, &fDelta, &bLoop, &bLockX, &bLockY, &bFreeze, &iTime);

	cell *pAddress = NULL;
	amx_GetAddr(amx, params[2], &pAddress);
	*pAddress = wAnimationId;

	amx_GetAddr(amx, params[3], &pAddress);
	*pAddress = amx_ftoc(fDelta);

	amx_GetAddr(amx, params[4], &pAddress);
	*pAddress = bLoop;

	amx_GetAddr(amx, params[5], &pAddress);
	*pAddress = bLockX;

	amx_GetAddr(amx, params[6], &pAddress);
	*pAddress = bLockY;

	amx_GetAddr(amx, params[7], &pAddress);
	*pAddress = bFreeze;

	amx_GetAddr(amx, params[8], &pAddress);
	*pAddress = iTime;
	return 1;
}

// native FCNPC_SetFightingStyle(npcid, style);
cell AMX_NATIVE_CALL CNatives::FCNPC_SetFightingStyle(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_SetFightingStyle");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);
	BYTE byteStyle = static_cast<BYTE>(params[2]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	switch (byteStyle) {
		case FIGHT_STYLE_NORMAL:
		case FIGHT_STYLE_BOXING:
		case FIGHT_STYLE_KUNGFU:
		case FIGHT_STYLE_KNEEHEAD:
		case FIGHT_STYLE_GRABKICK:
		case FIGHT_STYLE_ELBOW:
			pPlayerData->SetFightingStyle(byteStyle);
			return 1;
	}

	return 0;
}

// native FCNPC_GetFightingStyle(npcid);
cell AMX_NATIVE_CALL CNatives::FCNPC_GetFightingStyle(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetFightingStyle");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Get the player special action
	return pPlayerData->GetFightingStyle();
}

// native FCNPC_GoTo(npcid, Float:x, Float : y, Float : z, type = FCNPC_MOVE_TYPE_AUTO, Float : speed = FCNPC_MOVE_SPEED_AUTO, mode = FCNPC_MOVE_MODE_AUTO, Float : radius = 0.0, bool : setangle = true, Float : dist_offset = 0.0, stopdelay = 250);
cell AMX_NATIVE_CALL CNatives::FCNPC_GoTo(AMX *amx, cell *params)
{
	CHECK_PARAMS(11, "FCNPC_GoTo");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	CVector vecPoint(amx_ctof(params[2]), amx_ctof(params[3]), amx_ctof(params[4]));
	int iType = static_cast<int>(params[5]);
	float fSpeed = amx_ctof(params[6]);
	int iMode = static_cast<int>(params[7]);
	float fRadius = amx_ctof(params[8]);
	bool bSetAngle = static_cast<int>(params[9]) != 0;
	float fDistOffset = amx_ctof(params[10]);
	DWORD dwStopDelay = static_cast<DWORD>(params[11]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Move the player
	return pPlayerData->GoTo(vecPoint, iType, iMode, fRadius, bSetAngle, fSpeed, fDistOffset, dwStopDelay);
}

// native FCNPC_GoToPlayer(npcid, playereid, type, Float:speed, bool:UseZMap = false, Float:radius = 0.0, bool:setangle = true, Float:dist_offset = 0.0, Float:dist_check = 1.5, stopdelay = 250);
cell AMX_NATIVE_CALL CNatives::FCNPC_GoToPlayer(AMX *amx, cell *params)
{
	CHECK_PARAMS(10, "FCNPC_GoToPlayer");

	// get params
	WORD wNpcId = static_cast<WORD>(params[1]);
	WORD wPlayerId = static_cast<WORD>(params[2]);
	int iType = static_cast<int>(params[3]);
	float fSpeed = amx_ctof(params[4]);
	bool bZMap = static_cast<int>(params[5]) != 0;
	float fRadius = amx_ctof(params[6]);
	bool bSetAngle = static_cast<int>(params[7]) != 0;
	float fDistOffset = amx_ctof(params[8]);
	float fDistCheck = amx_ctof(params[9]);
	DWORD dwStopDelay = static_cast<DWORD>(params[10]);

	// validation
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	if (!pServer->GetPlayerManager()->IsPlayerConnected(wPlayerId) || wPlayerId == wNpcId) {
		return 0;
	}

	// move the player
	return pPlayerData->GoToPlayer(wPlayerId, iType, bZMap, fRadius, bSetAngle, fSpeed, fDistOffset, fDistCheck, dwStopDelay);
}

cell AMX_NATIVE_CALL CNatives::FCNPC_Stop(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_Stop");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Stop the player
	pPlayerData->StopMoving();
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_IsMoving(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_IsMoving");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Get the player moving state
	return pPlayerData->IsMoving();
}

// native FCNPC_IsMovingAtPlayer(npcid, playerid);
cell AMX_NATIVE_CALL CNatives::FCNPC_IsMovingAtPlayer(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_IsMovingAtPlayer");

	// get params
	WORD wNpcId = static_cast<WORD>(params[1]);
	WORD wPlayerId = static_cast<WORD>(params[2]);

	// validation
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	if (!pServer->GetPlayerManager()->IsPlayerConnected(wPlayerId) || wPlayerId == wNpcId) {
		return 0;
	}

	// Get the player moving state
	return pPlayerData->IsMovingAtPlayer(wPlayerId);
}

// native FCNPC_GetDestinationPoint(npcid, &Float:x, &Float:y, &Float:z);
cell AMX_NATIVE_CALL CNatives::FCNPC_GetDestinationPoint(AMX *amx, cell *params)
{
	CHECK_PARAMS(4, "FCNPC_GetDestinationPoint");

	// get params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		// Get the argument pointers and set its value
		cell *pAddress = NULL;
		float fPos = 0.0f;
		amx_GetAddr(amx, params[2], &pAddress);
		*pAddress = amx_ftoc(fPos);

		amx_GetAddr(amx, params[3], &pAddress);
		*pAddress = amx_ftoc(fPos);

		amx_GetAddr(amx, params[4], &pAddress);
		*pAddress = amx_ftoc(fPos);
		return 0;
	}

	// Get the player destination
	CVector vecDestination;
	pPlayerData->GetDestination(&vecDestination);

	// Get the argument pointers and set its value
	cell *pAddress = NULL;
	amx_GetAddr(amx, params[2], &pAddress);
	*pAddress = amx_ftoc(vecDestination.fX);

	amx_GetAddr(amx, params[3], &pAddress);
	*pAddress = amx_ftoc(vecDestination.fY);

	amx_GetAddr(amx, params[4], &pAddress);
	*pAddress = amx_ftoc(vecDestination.fZ);

	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_SetWeapon(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_SetWeapon");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	BYTE byteWeapon = static_cast<BYTE>(params[2]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Set the player weapon
	pPlayerData->SetWeapon(byteWeapon);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetWeapon(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetWeapon");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Get the player weapon
	return pPlayerData->GetWeapon();
}

cell AMX_NATIVE_CALL CNatives::FCNPC_SetAmmo(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_SetAmmo");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	WORD wAmmo = static_cast<WORD>(params[2]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Set the player ammo
	pPlayerData->SetAmmo(wAmmo);
	return 1;
}

// native FCNPC_GiveAmmo(npcid, ammo);
cell AMX_NATIVE_CALL CNatives::FCNPC_GiveAmmo(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_GiveAmmo");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	WORD wAmmo = static_cast<WORD>(params[2]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Set the player ammo
	WORD wNewAmmo = pPlayerData->GetAmmo() + wAmmo;
	pPlayerData->SetAmmo(wNewAmmo);
	return wNewAmmo;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetAmmo(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetAmmo");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Get the player ammo
	return pPlayerData->GetAmmo();
}

// native FCNPC_SetAmmoInClip(npcid, ammo);
cell AMX_NATIVE_CALL CNatives::FCNPC_SetAmmoInClip(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_SetAmmoInClip");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	WORD wAmmo = static_cast<WORD>(params[2]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Set the player ammo
	pPlayerData->SetAmmoInClip(wAmmo);
	return 1;
}

// native FCNPC_GiveAmmoInClip(npcid, ammo);
cell AMX_NATIVE_CALL CNatives::FCNPC_GiveAmmoInClip(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_GiveAmmoInClip");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	WORD wAmmo = static_cast<WORD>(params[2]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Set the player ammo
	WORD wNewAmmo = pPlayerData->GetAmmoInClip() + wAmmo;
	pPlayerData->SetAmmoInClip(wNewAmmo);
	return wNewAmmo;
}

// native FCNPC_GetAmmoInClip(npcid);
cell AMX_NATIVE_CALL CNatives::FCNPC_GetAmmoInClip(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetAmmoInClip");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Get the player ammo
	return pPlayerData->GetAmmoInClip();
}

cell AMX_NATIVE_CALL CNatives::FCNPC_SetWeaponSkillLevel(AMX *amx, cell *params)
{
	CHECK_PARAMS(3, "FCNPC_SetWeaponSkillLevel");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	DWORD dwSkill = static_cast<DWORD>(params[2]);
	WORD wLevel = static_cast<WORD>(params[3]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	if (dwSkill < 0 || dwSkill > 10) {
		return 0;
	}

	// Set the player weapon skill level
	pPlayerData->SetWeaponSkill(dwSkill, wLevel);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GiveWeaponSkillLevel(AMX *amx, cell *params)
{
	CHECK_PARAMS(3, "FCNPC_GiveWeaponSkillLevel");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	DWORD dwSkill = static_cast<DWORD>(params[2]);
	WORD wLevel = static_cast<WORD>(params[3]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	if (dwSkill < 0 || dwSkill > 10) {
		return 0;
	}

	// Set the player weapon skill level
	WORD wNewLevel = pPlayerData->GetWeaponSkill(dwSkill) + wLevel;
	pPlayerData->SetWeaponSkill(dwSkill, wNewLevel);
	return wNewLevel;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetWeaponSkillLevel(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_GetWeaponSkillLevel");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);
	DWORD dwSkill = static_cast<DWORD>(params[2]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Get the player weapon skill level
	return pPlayerData->GetWeaponSkill(dwSkill);
}

// native FCNPC_SetWeaponState(npcid, weaponstate);
cell AMX_NATIVE_CALL CNatives::FCNPC_SetWeaponState(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_SetWeaponState");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);
	int iState = static_cast<int>(params[2]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	switch (iState) {
		case WEAPONSTATE_NO_BULLETS:
		case WEAPONSTATE_LAST_BULLET:
		case WEAPONSTATE_MORE_BULLETS:
		case WEAPONSTATE_RELOADING:
			pPlayerData->SetWeaponState(iState);
			return 1;
	}

	return 0;
}

// native FCNPC_GetWeaponState(npcid);
cell AMX_NATIVE_CALL CNatives::FCNPC_GetWeaponState(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetWeaponState");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Get the player weapon state
	return pPlayerData->GetWeaponState();
}

// native FCNPC_SetWeaponReloadTime(npcid, weaponid, time);
cell AMX_NATIVE_CALL CNatives::FCNPC_SetWeaponReloadTime(AMX *amx, cell *params)
{
	CHECK_PARAMS(3, "FCNPC_SetWeaponReloadTime");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);
	BYTE byteWeaponId = static_cast<BYTE>(params[2]);
	int iReloadTime = static_cast<int>(params[3]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Set the player weapon reload time
	return pPlayerData->SetWeaponReloadTime(byteWeaponId, iReloadTime);
}

// native FCNPC_GetWeaponReloadTime(npcid, weaponid);
cell AMX_NATIVE_CALL CNatives::FCNPC_GetWeaponReloadTime(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_GetWeaponReloadTime");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);
	BYTE byteWeaponId = static_cast<BYTE>(params[2]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Get the player weapon reload time
	return pPlayerData->GetWeaponReloadTime(byteWeaponId);
}

// native FCNPC_GetWeaponActualReloadTime(npcid, weaponid);
cell AMX_NATIVE_CALL CNatives::FCNPC_GetWeaponActualReloadTime(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_GetWeaponActualReloadTime");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);
	BYTE byteWeaponId = static_cast<BYTE>(params[2]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Get the player weapon reload time
	return pPlayerData->GetWeaponActualReloadTime(byteWeaponId);
}

// native FCNPC_SetWeaponShootTime(npcid, weaponid, time);
cell AMX_NATIVE_CALL CNatives::FCNPC_SetWeaponShootTime(AMX *amx, cell *params)
{
	CHECK_PARAMS(3, "FCNPC_SetWeaponShootTime");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);
	BYTE byteWeaponId = static_cast<BYTE>(params[2]);
	int iShootTime = static_cast<int>(params[3]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Set the player weapon shoot time
	return pPlayerData->SetWeaponShootTime(byteWeaponId, iShootTime);
}

// native FCNPC_GetWeaponShootTime(npcid, weaponid);
cell AMX_NATIVE_CALL CNatives::FCNPC_GetWeaponShootTime(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_GetWeaponShootTime");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);
	BYTE byteWeaponId = static_cast<BYTE>(params[2]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Get the player weapon shoot time
	return pPlayerData->GetWeaponShootTime(byteWeaponId);
}

// native FCNPC_SetWeaponClipSize(npcid, weaponid, size);
cell AMX_NATIVE_CALL CNatives::FCNPC_SetWeaponClipSize(AMX *amx, cell *params)
{
	CHECK_PARAMS(3, "FCNPC_SetWeaponClipSize");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);
	BYTE byteWeaponId = static_cast<BYTE>(params[2]);
	int iClipSize = static_cast<int>(params[3]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Set the player weapon clip size
	return pPlayerData->SetWeaponClipSize(byteWeaponId, iClipSize);
}

// native FCNPC_GetWeaponClipSize(npcid, weaponid);
cell AMX_NATIVE_CALL CNatives::FCNPC_GetWeaponClipSize(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_GetWeaponClipSize");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);
	BYTE byteWeaponId = static_cast<BYTE>(params[2]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Get the player weapon clip size
	return pPlayerData->GetWeaponClipSize(byteWeaponId);
}

// native FCNPC_GetWeaponActualClipSize(npcid, weaponid);
cell AMX_NATIVE_CALL CNatives::FCNPC_GetWeaponActualClipSize(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_GetWeaponActualClipSize");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);
	BYTE byteWeaponId = static_cast<BYTE>(params[2]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Get the player weapon clip size
	return pPlayerData->GetWeaponActualClipSize(byteWeaponId);
}

// native FCNPC_SetWeaponAccuracy(npcid, weaponid, Float:accuracy);
cell AMX_NATIVE_CALL CNatives::FCNPC_SetWeaponAccuracy(AMX *amx, cell *params)
{
	CHECK_PARAMS(3, "FCNPC_SetWeaponAccuracy");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);
	BYTE byteWeaponId = static_cast<BYTE>(params[2]);
	float fAccuracy = amx_ctof(params[3]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Set the player weapon accuracy
	return pPlayerData->SetWeaponAccuracy(byteWeaponId, fAccuracy);
}

// native Float:FCNPC_GetWeaponAccuracy(npcid, weaponid);
cell AMX_NATIVE_CALL CNatives::FCNPC_GetWeaponAccuracy(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_GetWeaponAccuracy");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);
	BYTE byteWeaponId = static_cast<BYTE>(params[2]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Get the player weapon accuracy
	float fAccuracy = pPlayerData->GetWeaponAccuracy(byteWeaponId);
	return amx_ftoc(fAccuracy);
}

// native FCNPC_SetWeaponInfo(npcid, weaponid, reload_time = -1, shoot_time = -1, clip_size = -1, Float:accuracy = 1.0);
cell AMX_NATIVE_CALL CNatives::FCNPC_SetWeaponInfo(AMX *amx, cell *params)
{
	CHECK_PARAMS(6, "FCNPC_SetWeaponInfo");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);
	BYTE byteWeaponId = static_cast<BYTE>(params[2]);
	int iReloadTime = static_cast<int>(params[3]);
	int iShootTime = static_cast<int>(params[4]);
	int iClipSize = static_cast<int>(params[5]);
	float fAccuracy = amx_ctof(params[6]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Set the player weapon info
	SWeaponInfo sWeaponInfo = pPlayerData->GetWeaponInfo(byteWeaponId);

	if (iClipSize != -1) {
		sWeaponInfo.iClipSize = iClipSize;
	}

	if (iReloadTime != -1) {
		sWeaponInfo.iReloadTime = iReloadTime;
	}

	if (iShootTime != -1) {
		sWeaponInfo.iShootTime = iShootTime;
	}

	if (fAccuracy != 1.0f) {
		sWeaponInfo.fAccuracy = fAccuracy;
	}

	return pPlayerData->SetWeaponInfo(byteWeaponId, sWeaponInfo);
}

// native FCNPC_GetWeaponInfo(npcid, weaponid, &reload_time = -1, &shoot_time = -1, &clip_size = -1, &Float:accuracy = 1.0);
cell AMX_NATIVE_CALL CNatives::FCNPC_GetWeaponInfo(AMX *amx, cell *params)
{
	CHECK_PARAMS(6, "FCNPC_GetWeaponInfo");

	// get params
	WORD wNpcId = static_cast<WORD>(params[1]);
	BYTE byteWeaponId = static_cast<BYTE>(params[2]);

	// valid npc
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// valid weapon
	if (!CWeaponInfo::IsValid(byteWeaponId)) {
		return 0;
	}

	// get weapon info
	SWeaponInfo sWeaponInfo = pPlayerData->GetWeaponInfo(byteWeaponId);

	// write data to amx
	cell *pAddress = NULL;
	amx_GetAddr(amx, params[3], &pAddress);
	*pAddress = sWeaponInfo.iReloadTime;

	amx_GetAddr(amx, params[4], &pAddress);
	*pAddress = sWeaponInfo.iShootTime;

	amx_GetAddr(amx, params[5], &pAddress);
	*pAddress = sWeaponInfo.iClipSize;

	amx_GetAddr(amx, params[6], &pAddress);
	*pAddress = amx_ftoc(sWeaponInfo.fAccuracy);

	return 1;
}

// native FCNPC_SetWeaponDefaultInfo(weaponid, reload_time = -1, shoot_time = -1, clip_size = -1, Float:accuracy = 1.0);
cell AMX_NATIVE_CALL CNatives::FCNPC_SetWeaponDefaultInfo(AMX *amx, cell *params)
{
	CHECK_PARAMS(5, "FCNPC_SetWeaponDefaultInfo");

	// Get params
	BYTE byteWeaponId = static_cast<BYTE>(params[1]);
	int iReloadTime = static_cast<int>(params[2]);
	int iShootTime = static_cast<int>(params[3]);
	int iClipSize = static_cast<int>(params[4]);
	float fAccuracy = amx_ctof(params[5]);

	// Set default weapon info
	SWeaponInfo sWeaponInfo = CWeaponInfo::GetDefaultInfo(byteWeaponId);

	if (iClipSize != -1) {
		sWeaponInfo.iClipSize = iClipSize;
	}

	if (iReloadTime != -1) {
		sWeaponInfo.iReloadTime = iReloadTime;
	}

	if (iShootTime != -1) {
		sWeaponInfo.iShootTime = iShootTime;
	}

	if (fAccuracy != 1.0f) {
		sWeaponInfo.fAccuracy = fAccuracy;
	}

	return CWeaponInfo::SetDefaultInfo(byteWeaponId, sWeaponInfo);
}

// native FCNPC_GetWeaponDefaultInfo(weaponid, &reload_time = -1, &shoot_time = -1, &clip_size = -1, &Float:accuracy = 1.0);
cell AMX_NATIVE_CALL CNatives::FCNPC_GetWeaponDefaultInfo(AMX *amx, cell *params)
{
	CHECK_PARAMS(5, "FCNPC_GetWeaponDefaultInfo");

	// get params
	BYTE byteWeaponId = static_cast<BYTE>(params[1]);

	// valid weapon
	if (!CWeaponInfo::IsValid(byteWeaponId)) {
		return 0;
	}

	// get weapon info
	SWeaponInfo sWeaponInfo = CWeaponInfo::GetDefaultInfo(byteWeaponId);

	// write data to amx
	cell *pAddress = NULL;

	amx_GetAddr(amx, params[2], &pAddress);
	*pAddress = sWeaponInfo.iReloadTime;

	amx_GetAddr(amx, params[3], &pAddress);
	*pAddress = sWeaponInfo.iShootTime;

	amx_GetAddr(amx, params[4], &pAddress);
	*pAddress = sWeaponInfo.iClipSize;

	amx_GetAddr(amx, params[5], &pAddress);
	*pAddress = amx_ftoc(sWeaponInfo.fAccuracy);

	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_AimAt(AMX *amx, cell *params)
{
	CHECK_PARAMS(10, "FCNPC_AimAt");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	CVector vecPoint(amx_ctof(params[2]), amx_ctof(params[3]), amx_ctof(params[4]));
	bool bShoot = static_cast<int>(params[5]) != 0;
	int iShootDelay = static_cast<int>(params[6]);
	bool bSetAngle = static_cast<int>(params[7]) != 0;
	CVector vecOffsetFrom(amx_ctof(params[8]), amx_ctof(params[9]), amx_ctof(params[10]));

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	if (pPlayerData->IsMoving() && pPlayerData->GetMovingType() == MOVE_TYPE_SPRINT) {
		return 0;
	}

	int iWeaponType = pPlayerData->GetWeaponType(pPlayerData->GetWeapon());
	switch (iWeaponType) {
		case WEAPON_TYPE_MELEE:
		case WEAPON_TYPE_SHOOT:
		case WEAPON_TYPE_ROCKET:
		case WEAPON_TYPE_SPRAY:
		case WEAPON_TYPE_THROW:
			pPlayerData->StopAim();
			pPlayerData->AimAt(vecPoint, bShoot, iShootDelay, bSetAngle, vecOffsetFrom);
			return 1;
	}

	return 0;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_AimAtPlayer(AMX *amx, cell *params)
{
	CHECK_PARAMS(11, "FCNPC_AimAtPlayer");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	WORD wPlayerId = static_cast<WORD>(params[2]);
	bool bShoot = static_cast<int>(params[3]) != 0;
	int iShootDelay = static_cast<int>(params[4]);
	bool bSetAngle = static_cast<int>(params[5]) != 0;
	CVector vecOffsetTo(amx_ctof(params[6]), amx_ctof(params[7]), amx_ctof(params[8]));
	CVector vecOffsetFrom(amx_ctof(params[9]), amx_ctof(params[10]), amx_ctof(params[11]));

	// Make sure the npc is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsPlayerConnected(wPlayerId) || wPlayerId == wNpcId) {
		return 0;
	}

	// Set the player aiming
	int iWeaponType = pPlayerData->GetWeaponType(pPlayerData->GetWeapon());
	switch (iWeaponType) {
		case WEAPON_TYPE_MELEE:
		case WEAPON_TYPE_SHOOT:
		case WEAPON_TYPE_ROCKET:
		case WEAPON_TYPE_SPRAY:
		case WEAPON_TYPE_THROW:
			pPlayerData->StopAim();
			pPlayerData->AimAtPlayer(wPlayerId, bShoot, iShootDelay, bSetAngle, vecOffsetTo, vecOffsetFrom);
			return 1;
	}

	return 0;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_StopAim(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_StopAim");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Stop the player aim
	pPlayerData->StopAim();
	return 1;
}

// native FCNPC_MeleeAttack(npcid, delay = -1, bool:fightstyle = true);
cell AMX_NATIVE_CALL CNatives::FCNPC_MeleeAttack(AMX *amx, cell *params)
{
	CHECK_PARAMS(3, "FCNPC_MeleeAttack");

	// Get parms
	WORD wNpcId = static_cast<WORD>(params[1]);
	int iTime = static_cast<int>(params[2]);
	bool bUseFightstyle = static_cast<int>(params[3]) != 0;

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Set the player melee attack
	return pPlayerData->MeleeAttack(iTime, bUseFightstyle);
}

cell AMX_NATIVE_CALL CNatives::FCNPC_StopAttack(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_StopAttack");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Stop the player attack
	pPlayerData->StopAttack();
	return 1;
}

// native FCNPC_IsAttacking(npcid);
cell AMX_NATIVE_CALL CNatives::FCNPC_IsAttacking(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_IsAttacking");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Get the player attacking state
	return pPlayerData->IsAttacking();
}

cell AMX_NATIVE_CALL CNatives::FCNPC_IsAiming(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_IsAiming");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Get the player aiming state
	return pPlayerData->IsAiming();
}

// native FCNPC_IsAimingAtPlayer(npcid, playerid);
cell AMX_NATIVE_CALL CNatives::FCNPC_IsAimingAtPlayer(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_IsAimingAtPlayer");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	WORD wPlayerId = static_cast<WORD>(params[2]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	if (!pServer->GetPlayerManager()->IsPlayerConnected(wPlayerId) || wPlayerId == wNpcId) {
		return 0;
	}

	// Get the player aiming state
	return pPlayerData->IsAimingAtPlayer(wPlayerId);
}

// native FCNPC_GetAimingPlayer(npcid);
cell AMX_NATIVE_CALL CNatives::FCNPC_GetAimingPlayer(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetAimingPlayer");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return INVALID_PLAYER_ID;
	}

	// Get the player aim target
	return pPlayerData->GetAimingPlayer();
}

cell AMX_NATIVE_CALL CNatives::FCNPC_IsShooting(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_IsShooting");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Get the player shooting state
	return pPlayerData->IsShooting();
}

cell AMX_NATIVE_CALL CNatives::FCNPC_IsReloading(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_IsReloading");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Get the player shooting state
	return pPlayerData->IsReloading();
}

cell AMX_NATIVE_CALL CNatives::FCNPC_EnterVehicle(AMX *amx, cell *params)
{
	CHECK_PARAMS(4, "FCNPC_EnterVehicle");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	WORD wVehicleId = static_cast<WORD>(params[2]);
	BYTE byteSeatId = static_cast<BYTE>(params[3]);
	int iType = static_cast<int>(params[4]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Validate the vehicle
	if (wVehicleId < 1 || wVehicleId > MAX_VEHICLES) {
		return 0;
	}

	// Make the player enter the vehicle
	return pPlayerData->EnterVehicle(wVehicleId, byteSeatId, iType);
}

cell AMX_NATIVE_CALL CNatives::FCNPC_ExitVehicle(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_ExitVehicle");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Make the player exit the vehicle
	return pPlayerData->ExitVehicle();
}

cell AMX_NATIVE_CALL CNatives::FCNPC_PutInVehicle(AMX *amx, cell *params)
{
	CHECK_PARAMS(3, "FCNPC_PutInVehicle");

	// Get the parameters
	WORD wNpcId = static_cast<WORD>(params[1]);
	WORD wVehicleId = static_cast<WORD>(params[2]);
	BYTE byteSeatId = static_cast<BYTE>(params[3]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Validate the vehicle
	if (wVehicleId < 1 || wVehicleId > MAX_VEHICLES) {
		return 0;
	}

	// Make the player enter the vehicle
	return pPlayerData->PutInVehicle(wVehicleId, byteSeatId);
}

cell AMX_NATIVE_CALL CNatives::FCNPC_RemoveFromVehicle(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_RemoveFromVehicle");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Validate the player state
	int iState = pPlayerData->GetState();
	if (iState != PLAYER_STATE_DRIVER && iState != PLAYER_STATE_PASSENGER) {
		return 0;
	}

	// Make the player exit the vehicle
	return pPlayerData->RemoveFromVehicle();
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetVehicleID(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetVehicleID");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return INVALID_VEHICLE_ID;
	}

	// Make the player exit the vehicle
	return pPlayerData->GetVehicleId();
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetVehicleSeat(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetVehicleSeat");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Make the player exit the vehicle
	if (pPlayerData->GetVehicleId() == INVALID_VEHICLE_ID) {
		return 128;
	}

	return pPlayerData->GetSeatId();
}

// native FCNPC_SetVehicleSiren(npcid, bool:status);
cell AMX_NATIVE_CALL CNatives::FCNPC_SetVehicleSiren(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_SetVehicleSiren");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);
	bool bSiren = static_cast<int>(params[2]) != 0;

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Make sure the player is in vehicle
	if (pPlayerData->GetVehicleId() == INVALID_VEHICLE_ID) {
		return 0;
	}

	// Change siren state
	pPlayerData->SetVehicleSiren(bSiren);
	return 1;
}

// native FCNPC_IsVehicleSiren(npcid);
cell AMX_NATIVE_CALL CNatives::FCNPC_IsVehicleSiren(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_IsVehicleSiren");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Make sure the player is in vehicle
	if (pPlayerData->GetVehicleId() == INVALID_VEHICLE_ID) {
		return 0;
	}

	// Return siren state
	return pPlayerData->IsVehicleSiren();
}

// native FCNPC_SetVehicleHealth(npcid, Float:health);
cell AMX_NATIVE_CALL CNatives::FCNPC_SetVehicleHealth(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_SetVehicleHealth");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);
	float fHealth = amx_ctof(params[2]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Make sure the player is in vehicle
	if (pPlayerData->GetVehicleId() == INVALID_VEHICLE_ID) {
		return 0;
	}

	// Change vehicle health
	pPlayerData->SetVehicleHealth(fHealth);
	return 1;
}

// native Float:FCNPC_GetVehicleHealth(npcid);
cell AMX_NATIVE_CALL CNatives::FCNPC_GetVehicleHealth(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetVehicleHealth");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);
	float fHealth = 0.0f;

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return amx_ftoc(fHealth);
	}

	// Make sure the player is in vehicle
	if (pPlayerData->GetVehicleId() == INVALID_VEHICLE_ID) {
		return amx_ftoc(fHealth);
	}

	// Get the vehicle health
	fHealth = pPlayerData->GetVehicleHealth();
	return amx_ftoc(fHealth);
}

// native FCNPC_SetVehicleHydraThrusters(npcid, direction);
cell AMX_NATIVE_CALL CNatives::FCNPC_SetVehicleHydraThrusters(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_SetVehicleHydraThrusters");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);
	WORD wDirection = static_cast<WORD>(params[2]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Change vehicle hydra thrusters
	pPlayerData->SetVehicleHydraThrusters(wDirection);
	return 1;
}

// native FCNPC_GetVehicleHydraThrusters(npcid);
cell AMX_NATIVE_CALL CNatives::FCNPC_GetVehicleHydraThrusters(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetVehicleHydraThrusters");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Get the vehicle hydra thrusters
	return pPlayerData->GetVehicleHydraThrusters();
}

// native FCNPC_SetVehicleGearState(npcid, gear_state);
cell AMX_NATIVE_CALL CNatives::FCNPC_SetVehicleGearState(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_SetVehicleGearState");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);
	BYTE byteState = static_cast<BYTE>(params[2]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Change vehicle hydra thrusters
	pPlayerData->SetVehicleGearState(byteState);
	return 1;
}

// native FCNPC_GetVehicleGearState(npcid);
cell AMX_NATIVE_CALL CNatives::FCNPC_GetVehicleGearState(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetVehicleGearState");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Get the vehicle hydra thrusters
	return pPlayerData->GetVehicleGearState();
}

// native FCNPC_SetSurfingOffsets(npcid, Float:fX, Float:fY, Float:fZ);
cell AMX_NATIVE_CALL CNatives::FCNPC_SetSurfingOffsets(AMX *amx, cell *params)
{
	CHECK_PARAMS(4, "FCNPC_SetSurfingOffsets");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);
	CVector vecPoint(amx_ctof(params[2]), amx_ctof(params[3]), amx_ctof(params[4]));

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Change vehicle health
	pPlayerData->SetSurfingOffsets(vecPoint);
	return 1;
}

// native FCNPC_GiveSurfingOffsets(npcid, Float:fX, Float:fY, Float:fZ);
cell AMX_NATIVE_CALL CNatives::FCNPC_GiveSurfingOffsets(AMX *amx, cell *params)
{
	CHECK_PARAMS(4, "FCNPC_GiveSurfingOffsets");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);
	CVector vecPoint(amx_ctof(params[2]), amx_ctof(params[3]), amx_ctof(params[4]));

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Change vehicle health
	CVector vecOldPoint;
	pPlayerData->GetSurfingOffsets(&vecOldPoint);
	pPlayerData->SetSurfingOffsets(vecOldPoint + vecPoint);
	return 1;
}

// native FCNPC_GetSurfingOffsets(npcid, &Float:fX, &Float:fY, &Float:fZ);
cell AMX_NATIVE_CALL CNatives::FCNPC_GetSurfingOffsets(AMX *amx, cell *params)
{
	CHECK_PARAMS(4, "FCNPC_GetSurfingOffsets");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	CVector vecOffsets;
	pPlayerData->GetSurfingOffsets(&vecOffsets);

	cell *pAddress = NULL;
	amx_GetAddr(amx, params[2], &pAddress);
	*pAddress = amx_ftoc(vecOffsets.fX);

	amx_GetAddr(amx, params[3], &pAddress);
	*pAddress = amx_ftoc(vecOffsets.fY);

	amx_GetAddr(amx, params[4], &pAddress);
	*pAddress = amx_ftoc(vecOffsets.fZ);

	return 1;
}

// native FCNPC_SetSurfingVehicle(npcid, vehicleid);
cell AMX_NATIVE_CALL CNatives::FCNPC_SetSurfingVehicle(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_SetSurfingVehicle");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);
	WORD wVehicleId = static_cast<WORD>(params[2]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Validate the vehicle
	if (wVehicleId < 1 || wVehicleId > MAX_VEHICLES) {
		return 0;
	}

	// Set the surfing vehicle
	pPlayerData->SetSurfingVehicle(wVehicleId);
	return 1;
}

// native FCNPC_GetSurfingVehicle(npcid);
cell AMX_NATIVE_CALL CNatives::FCNPC_GetSurfingVehicle(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetSurfingVehicle");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return INVALID_VEHICLE_ID;
	}

	// Get the surfing vehicle
	return pPlayerData->GetSurfingVehicle();
}

// native FCNPC_SetSurfingObject(npcid, objectid);
cell AMX_NATIVE_CALL CNatives::FCNPC_SetSurfingObject(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_SetSurfingObject");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);
	WORD wObjectId = static_cast<WORD>(params[2]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Validate the object
	if (wObjectId < 1 || wObjectId >= MAX_OBJECTS) {
		return 0;
	}

	// Set the surfing object
	pPlayerData->SetSurfingObject(wObjectId);
	return 1;
}

// native FCNPC_GetSurfingObject(npcid);
cell AMX_NATIVE_CALL CNatives::FCNPC_GetSurfingObject(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetSurfingObject");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return INVALID_OBJECT_ID;
	}

	// Get the surfing object
	return pPlayerData->GetSurfingObject();
}

// native FCNPC_SetSurfingPlayerObject(npcid, objectid);
cell AMX_NATIVE_CALL CNatives::FCNPC_SetSurfingPlayerObject(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_SetSurfingPlayerObject");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);
	WORD wObjectId = static_cast<WORD>(params[2]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Validate the object
	if (wObjectId < 1 || wObjectId >= MAX_OBJECTS) {
		return 0;
	}

	// Set the surfing object
	pPlayerData->SetSurfingPlayerObject(wObjectId);
	return 1;
}

// native FCNPC_GetSurfingPlayerObject(npcid);
cell AMX_NATIVE_CALL CNatives::FCNPC_GetSurfingPlayerObject(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetSurfingPlayerObject");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return INVALID_OBJECT_ID;
	}

	// Get the surfing object
	return pPlayerData->GetSurfingPlayerObject();
}

// native FCNPC_StopSurfing(npcid);
cell AMX_NATIVE_CALL CNatives::FCNPC_StopSurfing(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_StopSurfing");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Get the surfing object
	pPlayerData->StopSurfing();
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_ToggleReloading(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_ToggleReloading");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	bool bReload = static_cast<int>(params[2]) != 0;

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Toggle it
	pPlayerData->ToggleReloading(bReload);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_ToggleInfiniteAmmo(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_ToggleInfiniteAmmo");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	bool bToggle = static_cast<int>(params[2]) != 0;

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Toggle it
	pPlayerData->ToggleInfiniteAmmo(bToggle);
	return 1;
}

// native FCNPC_StartPlayingPlayback(npcid, file[] = "", recordid = FCNPC_INVALID_RECORD_ID, bool:auto_unload = false, Float : delta_x = 0.0, Float : delta_y = 0.0, Float : delta_z = 0.0, Float : delta_qw = 0.0, Float : delta_qx = 0.0, Float : delta_qy = 0.0, Float : delta_qz = 0.0);
cell AMX_NATIVE_CALL CNatives::FCNPC_StartPlayingPlayback(AMX *amx, cell *params)
{
	CHECK_PARAMS(11, "FCNPC_StartPlayingPlayback");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	char *szFile;
	amx_StrParam(amx, params[2], szFile);
	int iRecordId = static_cast<int>(params[3]);
	bool bAutoUnload = static_cast<int>(params[4]) != 0;
	CVector vecPoint(amx_ctof(params[5]), amx_ctof(params[6]), amx_ctof(params[7]));
	float *fQuaternion = new float[4];
	fQuaternion[0] = amx_ctof(params[8]);
	fQuaternion[1] = amx_ctof(params[9]);
	fQuaternion[2] = amx_ctof(params[10]);
	fQuaternion[3] = amx_ctof(params[11]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Make sure the playback is valid
	if (szFile && strlen(szFile) == 0 && !pServer->GetRecordManager()->IsValid(iRecordId)) {
		return 0;
	}

	// Start Playing the player playback
	bool bSuccess = pPlayerData->StartPlayingPlayback(szFile, iRecordId, bAutoUnload, vecPoint, fQuaternion);
	SAFE_DELETE(fQuaternion);
	return bSuccess;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_StopPlayingPlayback(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_StopPlayingPlayback");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Stop Playing the player playback
	pPlayerData->StopPlayingPlayback();
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_PausePlayingPlayback(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_PausePlayingPlayback");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Pause Playing the player playback
	pPlayerData->PausePlayingPlayback();
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_ResumePlayingPlayback(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_ResumePlayingPlayback");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Resume Playing the player playback
	pPlayerData->ResumePlayingPlayback();
	return 1;
}

// native FCNPC_LoadPlayingPlayback(file[]);
cell AMX_NATIVE_CALL CNatives::FCNPC_LoadPlayingPlayback(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_LoadPlayingPlayback");

	// Get the params
	char *szFile;
	amx_StrParam(amx, params[1], szFile);

	// Make sure the filename is valid
	if (strlen(szFile) == 0) {
		return 0;
	}

	// Start Playing the player playback
	return pServer->GetRecordManager()->Load(szFile);
}

// native FCNPC_UnloadPlayingPlayback(playbackid);
cell AMX_NATIVE_CALL CNatives::FCNPC_UnloadPlayingPlayback(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_UnloadPlayingPlayback");

	// Get the params
	int iRecordId = static_cast<int>(params[1]);

	// Make sure the playback is valid
	if (!pServer->GetRecordManager()->IsValid(iRecordId)) {
		return 0;
	}

	// Start Playing the player playback
	return !!pServer->GetRecordManager()->Unload(iRecordId);
}

// native FCNPC_SetPlayingPlaybackPath(npcid, path[]);
cell AMX_NATIVE_CALL CNatives::FCNPC_SetPlayingPlaybackPath(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_SetPlayingPlaybackPath");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	char *szFile;
	amx_StrParam(amx, params[2], szFile);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Make sure the path is valid
	if (strlen(szFile) == 0) {
		return 0;
	}

	// Set playing path the player playback
	pPlayerData->SetPlayingPlaybackPath(szFile);
	return 1;
}

// native FCNPC_GetPlayingPlaybackPath(npcid, path[], const size = sizeof(path));
cell AMX_NATIVE_CALL CNatives::FCNPC_GetPlayingPlaybackPath(AMX *amx, cell *params)
{
	CHECK_PARAMS(3, "FCNPC_GetPlayingPlaybackPath");

	cell *pAddress = NULL;

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	amx_GetAddr(amx, params[2], &pAddress);
	size_t size = static_cast<size_t>(params[3]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Get playing path the player playback
	char *szPath = new char[size];
	pPlayerData->GetPlayingPlaybackPath(szPath, size);
	amx_SetString(pAddress, szPath, 0, 0, size);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetPluginVersion(AMX *amx, cell *params)
{
	cell *pAddress = NULL;

	// Get the params
	amx_GetAddr(amx, params[1], &pAddress);
	size_t size = static_cast<size_t>(params[2]);

	// Write version to the string
	amx_SetString(pAddress, PLUGIN_VERSION, 0, 0, size);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_SetUpdateRate(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_SetUpdateRate");

	// Get the params
	int iRate = static_cast<int>(params[1]);

	// Set the update rate
	return pServer->SetUpdateRate(iRate);
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetUpdateRate(AMX *amx, cell *params)
{
	return pServer->GetUpdateRate();
}

cell AMX_NATIVE_CALL CNatives::FCNPC_SetTickRate(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_SetUpdateRate");

	// Get the params
	int iRate = static_cast<int>(params[1]);

	// Set the tick rate
	return pServer->SetTickRate(iRate);
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetTickRate(AMX *amx, cell *params)
{
	return pServer->GetTickRate();
}

cell AMX_NATIVE_CALL CNatives::FCNPC_OpenNode(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_OpenNode");

	// Get the params
	int iNodeId = static_cast<int>(params[1]);

	// Open the node
	return pServer->GetNodeManager()->OpenNode(iNodeId);
}

cell AMX_NATIVE_CALL CNatives::FCNPC_CloseNode(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_CloseNode");

	// Get the params
	int iNodeId = static_cast<int>(params[1]);

	// Close the node
	pServer->GetNodeManager()->CloseNode(iNodeId);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_IsNodeOpen(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_IsNodeOpen");

	// Get the params
	int iNodeId = static_cast<int>(params[1]);

	// Open the node
	return pServer->GetNodeManager()->IsNodeOpen(iNodeId);
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetNodeType(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetNodeType");

	// Get the params
	int iNodeId = static_cast<int>(params[1]);

	// Make sure the node is valid
	if (!pServer->GetNodeManager()->IsNodeOpen(iNodeId)) {
		return NODE_TYPE_NONE;
	}

	// Get the node type
	return pServer->GetNodeManager()->GetAt(iNodeId)->GetNodeType();
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetNodePointCount(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetNodePointCount");

	// Get the params
	int iNodeId = static_cast<int>(params[1]);

	// Make sure the node is valid
	if (!pServer->GetNodeManager()->IsNodeOpen(iNodeId)) {
		return 0;
	}

	// Get the node point count
	return pServer->GetNodeManager()->GetAt(iNodeId)->GetNodesNumber();
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetNodePointPosition(AMX *amx, cell *params)
{
	CHECK_PARAMS(4, "FCNPC_GetNodePointPosition");

	// Get the params
	int iNodeId = static_cast<int>(params[1]);

	// Make sure the player is valid
	if (!pServer->GetNodeManager()->IsNodeOpen(iNodeId)) {
		// Get the argument pointers and set its value
		cell *pAddress = NULL;
		float fPos = 0.0f;
		amx_GetAddr(amx, params[2], &pAddress);
		*pAddress = amx_ftoc(fPos);

		amx_GetAddr(amx, params[3], &pAddress);
		*pAddress = amx_ftoc(fPos);

		amx_GetAddr(amx, params[4], &pAddress);
		*pAddress = amx_ftoc(fPos);
		return 0;
	}

	// Get the player position
	CVector vecPosition;
	pServer->GetNodeManager()->GetAt(iNodeId)->GetPosition(&vecPosition);

	// Get the argument pointers and set its value
	cell *pAddress = NULL;
	amx_GetAddr(amx, params[2], &pAddress);
	*pAddress = amx_ftoc(vecPosition.fX);

	amx_GetAddr(amx, params[3], &pAddress);
	*pAddress = amx_ftoc(vecPosition.fY);

	amx_GetAddr(amx, params[4], &pAddress);
	*pAddress = amx_ftoc(vecPosition.fZ);

	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_SetNodePoint(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_SetNodePoint");

	// Get the params
	int iNodeId = static_cast<int>(params[1]);
	WORD wPoint = static_cast<WORD>(params[2]);

	// Make sure the node is valid
	if (!pServer->GetNodeManager()->IsNodeOpen(iNodeId)) {
		return NODE_TYPE_NONE;
	}

	// Set the node type
	pServer->GetNodeManager()->GetAt(iNodeId)->SetPoint(wPoint);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetNodeInfo(AMX *amx, cell *params)
{
	CHECK_PARAMS(4, "FCNPC_GetNodeInfo");

	// Get the params
	int iNodeId = static_cast<int>(params[1]);

	// Make sure the player is valid
	if (!pServer->GetNodeManager()->IsNodeOpen(iNodeId)) {
		// Get the argument pointers and set its value
		cell *pAddress = NULL;
		amx_GetAddr(amx, params[2], &pAddress);
		*pAddress = 0;

		amx_GetAddr(amx, params[3], &pAddress);
		*pAddress = 0;

		amx_GetAddr(amx, params[4], &pAddress);
		*pAddress = 0;
		return 0;
	}

	// Get the node header informations
	DWORD dwVehNodes, dwPedNodes, dwNaviNodes;
	pServer->GetNodeManager()->GetAt(iNodeId)->GetHeaderInfo(&dwVehNodes, &dwPedNodes, &dwNaviNodes);

	// Get the argument pointers and set its value
	cell *pAddress = NULL;
	amx_GetAddr(amx, params[2], &pAddress);
	*pAddress = dwVehNodes;

	amx_GetAddr(amx, params[3], &pAddress);
	*pAddress = dwPedNodes;

	amx_GetAddr(amx, params[4], &pAddress);
	*pAddress = dwNaviNodes;

	return 1;
}

// native FCNPC_PlayNode(npcid, nodeid, move_type = MOVE_TYPE_AUTO, Float:speed = MOVE_SPEED_AUTO, bool:UseMapAndreas = false, Float:radius = 0.0, bool:setangle = true);
cell AMX_NATIVE_CALL CNatives::FCNPC_PlayNode(AMX *amx, cell *params)
{
	CHECK_PARAMS(7, "FCNPC_PlayNode");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	int iNodeId = static_cast<int>(params[2]);
	int iMoveType = static_cast<int>(params[3]);
	float fSpeed = amx_ctof(params[4]);
	bool bZMap = static_cast<int>(params[5]) != 0;
	float fRadius = amx_ctof(params[6]);
	bool bSetAngle = static_cast<int>(params[7]) != 0;

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Make sure the node is valid
	if (!pServer->GetNodeManager()->IsNodeOpen(iNodeId)) {
		return 0;
	}

	// Play the node
	return pPlayerData->PlayNode(iNodeId, iMoveType, bZMap, fRadius, bSetAngle, fSpeed);
}

cell AMX_NATIVE_CALL CNatives::FCNPC_StopPlayingNode(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_StopPlayingNode");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Stop playing the node
	pPlayerData->StopPlayingNode();
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_PausePlayingNode(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_PausePlayingNode");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Stop playing the node
	pPlayerData->PausePlayingNode();
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_ResumePlayingNode(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_ResumelayingNode");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Stop playing the node
	pPlayerData->ResumePlayingNode();
	return 1;
}

// native FCNPC_IsPlayingNode(npcid);
cell AMX_NATIVE_CALL CNatives::FCNPC_IsPlayingNode(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_IsPlayingNode");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// return node playing status
	return pPlayerData->IsPlayingNode();
}

// native FCNPC_IsPlayingNodePaused(npcid);
cell AMX_NATIVE_CALL CNatives::FCNPC_IsPlayingNodePaused(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_IsPlayingNodePaused");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// return node pause status
	return pPlayerData->IsPlayingNodePaused();
}

cell AMX_NATIVE_CALL CNatives::FCNPC_InitMapAndreas(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_InitMapAndreas");

	// Get the params
	CMapAndreas *pAddress = (CMapAndreas*)params[1];

	// Set the address
	if (pAddress != NULL) {
		pServer->SetMapAndreas(new CMapAndreas(pAddress));
		return 1;
	}
	return 0;
}

// native FCNPC_TriggerWeaponShot(npcid, weaponid, hittype, hitid, Float:x, Float:y, Float:z, bool:ishit = true, Float:offset_from_x = 0.0, Float:offset_from_y = 0.0, Float:offset_from_z = 0.0);
cell AMX_NATIVE_CALL CNatives::FCNPC_TriggerWeaponShot(AMX *amx, cell *params)
{
	CHECK_PARAMS(11, "FCNPC_TriggerWeaponShot");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	BYTE byteWeaponId = static_cast<BYTE>(params[2]);
	BYTE byteHitType = static_cast<BYTE>(params[3]);
	WORD wHitId = static_cast<WORD>(params[4]);
	CVector vecPoint(amx_ctof(params[5]), amx_ctof(params[6]), amx_ctof(params[7]));
	bool bIsHit = static_cast<int>(params[8]) != 0;
	CVector vecOffsetFrom(amx_ctof(params[9]), amx_ctof(params[10]), amx_ctof(params[11]));

	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	CFunctions::PlayerShoot(wNpcId, wHitId, byteHitType, byteWeaponId, vecPoint, vecOffsetFrom, bIsHit);
	return 1;
}

// native FCNPC_CreateMovePath();
cell AMX_NATIVE_CALL CNatives::FCNPC_CreateMovePath(AMX *amx, cell *params)
{
	CHECK_PARAMS(0, "FCNPC_CreateMovePath");

	return pServer->GetMovePath()->Create();
}

// native FCNPC_DestroyMovePath(pathid);
cell AMX_NATIVE_CALL CNatives::FCNPC_DestroyMovePath(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_DestroyMovePath");

	// get params
	int iPathId = static_cast<int>(params[1]);

	return pServer->GetMovePath()->Destroy(iPathId);
}

// native FCNPC_IsValidMovePath(pathid);
cell AMX_NATIVE_CALL CNatives::FCNPC_IsValidMovePath(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_IsValidMovePath");

	// get params
	int iPathId = static_cast<int>(params[1]);

	return pServer->GetMovePath()->IsPathValid(iPathId);
}

// native FCNPC_AddPointToPath(pathid, Float:x, Float:y, Float:z);
cell AMX_NATIVE_CALL CNatives::FCNPC_AddPointToPath(AMX *amx, cell *params)
{
	CHECK_PARAMS(4, "FCNPC_AddPointToPath");

	// get params
	int iPathId = static_cast<int>(params[1]);
	CVector vecPoint(amx_ctof(params[2]), amx_ctof(params[3]), amx_ctof(params[4]));

	return pServer->GetMovePath()->AddPoint(iPathId, vecPoint);
}

// native FCNPC_AddPointsToPath(pathid, Float:points[][3], const size = sizeof(points));
cell AMX_NATIVE_CALL CNatives::FCNPC_AddPointsToPath(AMX *amx, cell *params)
{
	CHECK_PARAMS(3, "FCNPC_AddPointsToPath");

	// get params
	int iPathId = static_cast<int>(params[1]);
	int iSize = static_cast<int>(params[3]);

	// validation
	if (!pServer->GetMovePath()->IsPathValid(iPathId)) {
		return 0;
	}

	// get and add points
	CVector vecPoint;
	cell *pAddress = NULL;
	amx_GetAddr(amx, params[2], &pAddress);
	pAddress += iSize;

	for (int i = 0; i < iSize; i++) {
		vecPoint.fX = amx_ctof(*(pAddress++));
		vecPoint.fY = amx_ctof(*(pAddress++));
		vecPoint.fZ = amx_ctof(*(pAddress++));

		pServer->GetMovePath()->AddPoint(iPathId, vecPoint);
	}
	return 1;
}

// native FCNPC_AddPointsToPath2(pathid, Float:points_x[], Float:points_y[], Float:points_z[], const size = sizeof(points_x));
cell AMX_NATIVE_CALL CNatives::FCNPC_AddPointsToPath2(AMX *amx, cell *params)
{
	CHECK_PARAMS(5, "FCNPC_AddPointsToPath2");

	// get params
	int iPathId = static_cast<int>(params[1]);
	int iSize = static_cast<int>(params[5]);

	// validation
	if (!pServer->GetMovePath()->IsPathValid(iPathId)) {
		return 0;
	}

	// get and add points
	CVector vecPoint;
	cell *pAddressX = NULL;
	cell *pAddressY = NULL;
	cell *pAddressZ = NULL;

	amx_GetAddr(amx, params[2], &pAddressX);
	amx_GetAddr(amx, params[3], &pAddressY);
	amx_GetAddr(amx, params[4], &pAddressZ);

	for (int i = 0; i < iSize; i++) {
		vecPoint.fX = amx_ctof(*(pAddressX + i));
		vecPoint.fY = amx_ctof(*(pAddressY + i));
		vecPoint.fZ = amx_ctof(*(pAddressZ + i));

		pServer->GetMovePath()->AddPoint(iPathId, vecPoint);
	}
	return 1;
}

// native FCNPC_RemovePointFromPath(pathid, pointid);
cell AMX_NATIVE_CALL CNatives::FCNPC_RemovePointFromPath(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_RemovePointFromPath");

	// get params
	int iPathId = static_cast<int>(params[1]);
	int iPointId = static_cast<int>(params[2]);

	return pServer->GetMovePath()->RemovePoint(iPathId, iPointId);
}

// native FCNPC_IsValidMovePoint(pathid, pointid);
cell AMX_NATIVE_CALL CNatives::FCNPC_IsValidMovePoint(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_IsValidMovePoint");

	// get params
	int iPathId = static_cast<int>(params[1]);
	int iPointId = static_cast<int>(params[2]);

	return pServer->GetMovePath()->IsPointValid(iPathId, iPointId);
}

// native FCNPC_GetMovePoint(pathid, pointid, &Float:x, &Float:y, &Float:z);
cell AMX_NATIVE_CALL CNatives::FCNPC_GetMovePoint(AMX *amx, cell *params)
{
	CHECK_PARAMS(5, "FCNPC_GetMovePoint");

	// get params
	int iPathId = static_cast<int>(params[1]);
	int iPointId = static_cast<int>(params[2]);

	// valid
	CVector *vecPoint = pServer->GetMovePath()->GetPoint(iPathId, iPointId);
	if (!vecPoint) {
		return 0;
	}

	// return point data
	cell *pAddress = NULL;
	amx_GetAddr(amx, params[3], &pAddress);
	*pAddress = amx_ftoc(vecPoint->fX);

	amx_GetAddr(amx, params[4], &pAddress);
	*pAddress = amx_ftoc(vecPoint->fY);

	amx_GetAddr(amx, params[5], &pAddress);
	*pAddress = amx_ftoc(vecPoint->fZ);
	return 1;
}

// native FCNPC_GetNumberMovePoint(pathid);
cell AMX_NATIVE_CALL CNatives::FCNPC_GetNumberMovePoint(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetNumberMovePoint");

	// get params
	int iPathId = static_cast<int>(params[1]);

	// validation
	if (!pServer->GetMovePath()->IsPathValid(iPathId)) {
		return -1;
	}

	return pServer->GetMovePath()->GetPoints(iPathId)->size();
}

// native FCNPC_GoByMovePath(npcid, pathid, pointid = 0, type = FCNPC_MOVE_TYPE_AUTO, Float:speed = FCNPC_MOVE_SPEED_AUTO, bool : UseMapAndreas = false, Float : radius = 0.0, bool : setangle = true, Float : dist_offset = 0.0);
cell AMX_NATIVE_CALL CNatives::FCNPC_GoByMovePath(AMX *amx, cell *params)
{
	CHECK_PARAMS(9, "FCNPC_GoByMovePath");

	// get params
	WORD wNpcId = static_cast<WORD>(params[1]);
	int iPathId = static_cast<int>(params[2]);
	int iPointId = static_cast<int>(params[3]);
	int iType = static_cast<int>(params[4]);
	float fSpeed = amx_ctof(params[5]);
	bool bZMap = static_cast<int>(params[6]) != 0;
	float fRadius = amx_ctof(params[7]);
	bool bSetAngle = static_cast<int>(params[8]) != 0;
	float fDistOffset = amx_ctof(params[9]);

	// validation
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// move the player
	return pPlayerData->GoByMovePath(iPathId, iPointId, iType, bZMap, fRadius, bSetAngle, fSpeed, fDistOffset);
}

// native FCNPC_SetMoveMode(npcid, bool:enabled);
cell AMX_NATIVE_CALL CNatives::FCNPC_SetMoveMode(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_SetMoveMode");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	bool bIsEnabled = static_cast<int>(params[2]) != 0;

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// Change move mode
	pPlayerData->SetMoveMode(bIsEnabled);
	return 1;
}

// native FCNPC_GetMoveMode(npcid);
cell AMX_NATIVE_CALL CNatives::FCNPC_GetMoveMode(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetMoveMode");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// return the state
	return pPlayerData->GetMoveMode();
}

// native FCNPC_SetMinHeightPosCall(npcid, Float:height);
cell AMX_NATIVE_CALL CNatives::FCNPC_SetMinHeightPosCall(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_SetMinHeightPosCall");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);
	float fHeight = amx_ctof(params[2]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// update minimum height
	pPlayerData->SetMinHeightPosCall(fHeight);
	return 1;
}

// native FCNPC_GetMinHeightPosCall(npcid);
cell AMX_NATIVE_CALL CNatives::FCNPC_GetMinHeightPosCall(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetMinHeightPosCall");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);
	if (!pPlayerData) {
		return 0;
	}

	// get height pos
	float fHeight = pPlayerData->GetMinHeightPosCall();
	return amx_ftoc(fHeight);
}

// native FCNPC_ToggleCrashLogCreation(bool:toggle);
cell AMX_NATIVE_CALL CNatives::FCNPC_ToggleCrashLogCreation(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_ToggleCrashLogCreation");

	// get the params
	bool bIsEnabled = static_cast<int>(params[1]) != 0;

	// change the state
	pServer->ToggleCrashLogCreation(bIsEnabled);
	return 1;
}

// native FCNPC_GetCrashLogCreation();
cell AMX_NATIVE_CALL CNatives::FCNPC_GetCrashLogCreation(AMX *amx, cell *params)
{
	CHECK_PARAMS(0, "FCNPC_GetCrashLogCreation");

	// get the state
	return static_cast<int>(pServer->GetCrashLogCreation());
}
