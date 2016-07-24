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

	// Make sure the length is valid
	int iLength = strlen(szName);
	if (iLength == 0 || iLength > MAX_PLAYER_NAME) {
		return INVALID_PLAYER_ID;
	}

	return pServer->GetPlayerManager()->AddPlayer(szName);
}

cell AMX_NATIVE_CALL CNatives::FCNPC_Destroy(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_Destroy");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
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
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Set the spawn position
	pServer->GetPlayerManager()->GetAt(wNpcId)->SetSpawnPosition(vecPoint);
	// Spawn the player
	return pServer->GetPlayerManager()->GetAt(wNpcId)->Spawn(iSkin);
}

cell AMX_NATIVE_CALL CNatives::FCNPC_Respawn(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_Respawn");
	// Get the NPC id
	WORD wNpcId = static_cast<WORD>(params[1]);
	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Respawn the player
	return pServer->GetPlayerManager()->GetAt(wNpcId)->Respawn();
}

cell AMX_NATIVE_CALL CNatives::FCNPC_IsSpawned(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_IsSpawned");
	// Get the NPC id
	WORD wNpcId = static_cast<WORD>(params[1]);
	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Respawn the player
	return pServer->GetPlayerManager()->GetAt(wNpcId)->IsSpawned();
}

cell AMX_NATIVE_CALL CNatives::FCNPC_Kill(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_Kill");
	// Get the NPC id
	WORD wNpcId = static_cast<WORD>(params[1]);
	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Kill the player
	pServer->GetPlayerManager()->GetAt(wNpcId)->Kill(INVALID_PLAYER_ID, 255);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_IsDead(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_IsDead");
	// Get the NPC id
	WORD wNpcId = static_cast<WORD>(params[1]);
	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Return the player dead state
	return pServer->GetPlayerManager()->GetAt(wNpcId)->GetState() == PLAYER_STATE_WASTED;
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
	int iForPlayerId = static_cast<int>(params[2]);

	// Make sure the npc is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsPlayerConnected(iForPlayerId) || iForPlayerId == wNpcId) {
		return 0;
	}

	// Return the player is streamed
	return pServer->GetPlayerManager()->GetAt(wNpcId)->IsStreamedIn(iForPlayerId);
}

cell AMX_NATIVE_CALL CNatives::FCNPC_IsStreamedForAnyone(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_IsStreamedForAnyone");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the npc is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	for (int i = 0; i < pNetGame->pPlayerPool->dwPlayerPoolSize; i++) {
		// Ignore non connected players and the same player
		if (!pServer->GetPlayerManager()->IsPlayerConnected(i)) {
			continue;
		}

		if (pServer->GetPlayerManager()->GetAt(wNpcId)->IsStreamedIn(i)) {
			return 1;
		}
	}

	// Return the player is streamed
	return 0;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_SetPosition(AMX *amx, cell *params)
{
	CHECK_PARAMS(4, "FCNPC_SetPosition");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	CVector vecPoint(amx_ctof(params[2]), amx_ctof(params[3]), amx_ctof(params[4]));

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Set the player position
	pServer->GetPlayerManager()->GetAt(wNpcId)->SetPosition(vecPoint);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetPosition(AMX *amx, cell *params)
{
	CHECK_PARAMS(4, "FCNPC_GetPosition");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
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
	pServer->GetPlayerManager()->GetAt(wNpcId)->GetPosition(&vecPosition);

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
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Set the player angle
	pServer->GetPlayerManager()->GetAt(wNpcId)->SetAngle(fAngle);
	return 1;
}

// native FCNPC_SetAngleToPos(npcid, Float:X, Float:Y);
cell AMX_NATIVE_CALL CNatives::FCNPC_SetAngleToPos(AMX *amx, cell *params)
{
	CHECK_PARAMS(3, "FCNPC_SetAngleToPos");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	CVector vecPoint(amx_ctof(params[2]), amx_ctof(params[3]), 0.0f);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Set the player angle
	CVector vecPos;
	pServer->GetPlayerManager()->GetAt(wNpcId)->GetPosition(&vecPos);

	CVector vecFront = vecPoint - vecPos;
	pServer->GetPlayerManager()->GetAt(wNpcId)->SetAngle(CMath::RadiansToDegree(atan2(vecFront.fY, vecFront.fX)));
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
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsPlayerConnected(wPlayerId) || wPlayerId == wNpcId) {
		return 0;
	}

	// Set the player angle
	CVector vecPlayerPos = pNetGame->pPlayerPool->pPlayer[wPlayerId]->vecPosition;

	CVector vecPos;
	pServer->GetPlayerManager()->GetAt(wNpcId)->GetPosition(&vecPos);

	CVector vecFront = vecPlayerPos - vecPos;
	pServer->GetPlayerManager()->GetAt(wNpcId)->SetAngle(CMath::RadiansToDegree(atan2(vecFront.fY, vecFront.fX)));
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetAngle(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetAngle");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	float fAngle = 0.0f;

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return amx_ftoc(fAngle);
	}

	// Get the player angle
	fAngle = pServer->GetPlayerManager()->GetAt(wNpcId)->GetAngle();
	return amx_ftoc(fAngle);
}

cell AMX_NATIVE_CALL CNatives::FCNPC_SetInterior(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_SetInterior");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	int iInterior = static_cast<int>(params[2]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Set the player interior
	pServer->GetPlayerManager()->GetAt(wNpcId)->SetInterior(iInterior);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetInterior(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetInterior");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Get the player interior
	return pServer->GetPlayerManager()->GetAt(wNpcId)->GetInterior();
}

cell AMX_NATIVE_CALL CNatives::FCNPC_SetVirtualWorld(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_SetVirtualWorld");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	int iVirtualWorld = static_cast<int>(params[2]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Set the player virtual world
	pServer->GetPlayerManager()->GetAt(wNpcId)->SetVirtualWorld(iVirtualWorld);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetVirtualWorld(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetVirtualWorld");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Get the player virtual world
	return pServer->GetPlayerManager()->GetAt(wNpcId)->GetVirtualWorld();
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
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Set the player quaternion
	pServer->GetPlayerManager()->GetAt(wNpcId)->SetQuaternion(fQuaternion);
	SAFE_DELETE(fQuaternion);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetQuaternion(AMX *amx, cell *params)
{
	CHECK_PARAMS(5, "FCNPC_GetQuaternion");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
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
	pServer->GetPlayerManager()->GetAt(wNpcId)->GetQuaternion(fQuaternion);

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
	CHECK_PARAMS(4, "FCNPC_SetVelocity");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	CVector vecVelocity(amx_ctof(params[2]), amx_ctof(params[3]), amx_ctof(params[4]));

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Set the player velocity
	pServer->GetPlayerManager()->GetAt(wNpcId)->SetVelocity(vecVelocity);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetVelocity(AMX *amx, cell *params)
{
	CHECK_PARAMS(4, "FCNPC_GetVelocity");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
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
	pServer->GetPlayerManager()->GetAt(wNpcId)->GetVelocity(&vecVelocity);

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

cell AMX_NATIVE_CALL CNatives::FCNPC_SetHealth(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_SetHealth");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	float fHealth = amx_ctof(params[2]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Set the player health
	pServer->GetPlayerManager()->GetAt(wNpcId)->SetHealth(fHealth);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetHealth(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetHealth");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	float fHealth = 0.0f;

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return amx_ftoc(fHealth);
	}

	// Get the player health
	fHealth = pServer->GetPlayerManager()->GetAt(wNpcId)->GetHealth();
	return amx_ftoc(fHealth);
}

cell AMX_NATIVE_CALL CNatives::FCNPC_SetArmour(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_SetArmour");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	float fArmour = amx_ctof(params[2]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Set the player armour
	pServer->GetPlayerManager()->GetAt(wNpcId)->SetArmour(fArmour);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetArmour(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetArmour");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	float fArmour = 0.0f;

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return amx_ftoc(fArmour);
	}

	// Get the player armour
	fArmour = pServer->GetPlayerManager()->GetAt(wNpcId)->GetArmour();
	return amx_ftoc(fArmour);
}

// native FCNPC_SetInvulnerable(npcid, bool:invulnerable = true);
cell AMX_NATIVE_CALL CNatives::FCNPC_SetInvulnerable(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_SetInvulnerable");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	bool bInvulnerable = static_cast<bool>(params[2]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Set the player invulnerable
	pServer->GetPlayerManager()->GetAt(wNpcId)->SetInvulnerable(bInvulnerable);
	return 1;
}

// native FCNPC_IsInvulnerable(npcid);
cell AMX_NATIVE_CALL CNatives::FCNPC_IsInvulnerable(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_IsInvulnerable");

	// Get the NPC id
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Get the player invulnerable
	return pServer->GetPlayerManager()->GetAt(wNpcId)->IsInvulnerable();
}

cell AMX_NATIVE_CALL CNatives::FCNPC_SetSkin(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_SetSkin");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	int iSkin = static_cast<int>(params[2]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Set the player skin
	pServer->GetPlayerManager()->GetAt(wNpcId)->SetSkin(iSkin);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetSkin(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetSkin");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Get the player skin
	return pServer->GetPlayerManager()->GetAt(wNpcId)->GetSkin();
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
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Set the player keys
	pServer->GetPlayerManager()->GetAt(wNpcId)->SetKeys(wUDAnalog, wLRAnalog, dwKeys);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetKeys(AMX *amx, cell *params)
{
	CHECK_PARAMS(4, "FCNPC_GetKeys");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		// Get the argument pointers and set its value
		cell *pAddress = NULL;
		float fPos = 0.0f;
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
	pServer->GetPlayerManager()->GetAt(wNpcId)->GetKeys(&wUDAnalog, &wLRAnalog, &dwKeys);

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
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Set the player special action
	pServer->GetPlayerManager()->GetAt(wNpcId)->SetSpecialAction(byteActionId);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetSpecialAction(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetSpecialAction");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Get the player special action
	return pServer->GetPlayerManager()->GetAt(wNpcId)->GetSpecialAction();
}

// native FCNPC_SetAnimation(npcid, animationid, Float:fDelta = 4.1, loop = 0, lockx = 1, locky = 1, freeze = 0, time = 1);
cell AMX_NATIVE_CALL CNatives::FCNPC_SetAnimation(AMX *amx, cell *params)
{
	CHECK_PARAMS(8, "FCNPC_SetAnimation");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);
	int iAnimationId = static_cast<int>(params[2]);
	float fDelta = amx_ctof(params[3]);
	bool bLoop = static_cast<bool>(params[4]);
	bool bLockX = static_cast<bool>(params[5]);
	bool bLockY = static_cast<bool>(params[6]);
	bool bFreeze = static_cast<bool>(params[7]);
	int iTime = static_cast<int>(params[8]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Set the player animation
	pServer->GetPlayerManager()->GetAt(wNpcId)->SetAnimation(iAnimationId, fDelta, bLoop, bLockX, bLockY, bFreeze, iTime);
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
	float fDelta = amx_ctof(params[4]);
	bool bLoop = static_cast<bool>(params[5]);
	bool bLockX = static_cast<bool>(params[6]);
	bool bLockY = static_cast<bool>(params[7]);
	bool bFreeze = static_cast<bool>(params[8]);
	int iTime = static_cast<int>(params[9]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Set the player animation
	pServer->GetPlayerManager()->GetAt(wNpcId)->SetAnimationByName(szName, fDelta, bLoop, bLockX, bLockY, bFreeze, iTime);
	return 1;
}

// native FCNPC_ResetAnimation(npcid);
cell AMX_NATIVE_CALL CNatives::FCNPC_ResetAnimation(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_ResetAnimation");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Set the player animation
	pServer->GetPlayerManager()->GetAt(wNpcId)->ResetAnimation();
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
	bool bLoop = static_cast<bool>(params[5]);
	bool bLockX = static_cast<bool>(params[6]);
	bool bLockY = static_cast<bool>(params[7]);
	bool bFreeze = static_cast<bool>(params[8]);
	int iTime = static_cast<int>(params[9]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Set the player animation
	pServer->GetPlayerManager()->GetAt(wNpcId)->ApplyAnimation(szAnimLib, szAnimName, fDelta, bLoop, bLockX, bLockY, bFreeze, iTime);
	return 1;
}

// native FCNPC_ClearAnimations(npcid);
cell AMX_NATIVE_CALL CNatives::FCNPC_ClearAnimations(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_ClearAnimations");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Set the player animation
	pServer->GetPlayerManager()->GetAt(wNpcId)->ClearAnimations();
	return 1;
}

// native FCNPC_GetAnimation(npcid, &animationid = 0, &Float:fDelta = 4.1, &loop = 0, &lockx = 1, &locky = 1, &freeze = 0, &time = 1);
cell AMX_NATIVE_CALL CNatives::FCNPC_GetAnimation(AMX *amx, cell *params)
{
	CHECK_PARAMS(8, "FCNPC_GetAnimation");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Get parms
	int iAnimationId;
	float fDelta;
	bool bLoop;
	bool bLockX;
	bool bLockY;
	bool bFreeze;
	int iTime;

	// Get the player animation
	pServer->GetPlayerManager()->GetAt(wNpcId)->GetAnimation(&iAnimationId, &fDelta, &bLoop, &bLockX, &bLockY, &bFreeze, &iTime);

	cell *pAddress = NULL;
	amx_GetAddr(amx, params[2], &pAddress);
	*pAddress = iAnimationId;

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
	int iStyle = static_cast<int>(params[2]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	switch (iStyle) {
		case FIGHT_STYLE_NORMAL:
		case FIGHT_STYLE_BOXING:
		case FIGHT_STYLE_KUNGFU:
		case FIGHT_STYLE_KNEEHEAD:
		case FIGHT_STYLE_GRABKICK:
		case FIGHT_STYLE_ELBOW:
			pServer->GetPlayerManager()->GetAt(wNpcId)->SetFightingStyle(iStyle);
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
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Get the player special action
	return pServer->GetPlayerManager()->GetAt(wNpcId)->GetFightingStyle();
}

// native FCNPC_GoTo(npcid, Float:X, Float:Y, Float:Z, type = MOVE_TYPE_AUTO, Float:speed = -1.0, bool:UseZMap = false, Float:radius = 0.0, bool:setangle = true);
cell AMX_NATIVE_CALL CNatives::FCNPC_GoTo(AMX *amx, cell *params)
{
	CHECK_PARAMS(9, "FCNPC_GoTo");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	CVector vecPoint(amx_ctof(params[2]), amx_ctof(params[3]), amx_ctof(params[4]));
	int iType = static_cast<int>(params[5]);
	float fSpeed = amx_ctof(params[6]);
	bool bZMap = static_cast<bool>(params[7]);
	float fRadius = amx_ctof(params[8]);
	bool bSetAngle = static_cast<bool>(params[9]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Move the player
	return pServer->GetPlayerManager()->GetAt(wNpcId)->GoTo(vecPoint, iType, bZMap, fRadius, bSetAngle, fSpeed);
}

// native FCNPC_GoToPlayer(npcid, playereid, type, Float:speed, bool:UseZMap = false, Float:radius = 0.0, bool:setangle = true);
cell AMX_NATIVE_CALL CNatives::FCNPC_GoToPlayer(AMX *amx, cell *params)
{
	CHECK_PARAMS(7, "FCNPC_GoToPlayer");

	// get params
	WORD wNpcId = static_cast<WORD>(params[1]);
	WORD wPlayerId = static_cast<WORD>(params[2]);
	int iType = static_cast<int>(params[3]);
	float fSpeed = amx_ctof(params[4]);
	int bZMap = static_cast<bool>(params[5]);
	float fRadius = amx_ctof(params[6]);
	bool bSetAngle = static_cast<bool>(params[7]);

	// validation
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	if (!pServer->GetPlayerManager()->IsPlayerConnected(wPlayerId) || wPlayerId == wNpcId) {
		return 0;
	}

	// move the player
	return pServer->GetPlayerManager()->GetAt(wNpcId)->GoToPlayer(wPlayerId, iType, bZMap, fRadius, bSetAngle, fSpeed);
}

cell AMX_NATIVE_CALL CNatives::FCNPC_Stop(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_Stop");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Stop the player
	pServer->GetPlayerManager()->GetAt(wNpcId)->StopMoving();
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_IsMoving(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_IsMoving");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Get the player moving state
	return pServer->GetPlayerManager()->GetAt(wNpcId)->IsMoving();
}

// native FCNPC_IsMovingAtPlayer(npcid, playerid);
cell AMX_NATIVE_CALL CNatives::FCNPC_IsMovingAtPlayer(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_IsMovingAtPlayer");

	// get params
	WORD wNpcId = static_cast<WORD>(params[1]);
	WORD wPlayerId = static_cast<WORD>(params[2]);

	// validation
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	if (!pServer->GetPlayerManager()->IsPlayerConnected(wPlayerId) || wPlayerId == wNpcId) {
		return 0;
	}

	// Get the player moving state
	return pServer->GetPlayerManager()->GetAt(wNpcId)->IsMovingAtPlayer(wPlayerId);
}

cell AMX_NATIVE_CALL CNatives::FCNPC_SetWeapon(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_SetWeapon");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	BYTE byteWeapon = static_cast<BYTE>(params[2]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Set the player weapon
	pServer->GetPlayerManager()->GetAt(wNpcId)->SetWeapon(byteWeapon);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetWeapon(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetWeapon");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Get the player weapon
	return pServer->GetPlayerManager()->GetAt(wNpcId)->GetWeapon();
}

cell AMX_NATIVE_CALL CNatives::FCNPC_SetAmmo(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_SetAmmo");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	WORD wAmmo = static_cast<WORD>(params[2]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Set the player ammo
	pServer->GetPlayerManager()->GetAt(wNpcId)->SetAmmo(wAmmo);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetAmmo(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetAmmo");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Get the player ammo
	return pServer->GetPlayerManager()->GetAt(wNpcId)->GetAmmo();
}

cell AMX_NATIVE_CALL CNatives::FCNPC_SetWeaponSkillLevel(AMX *amx, cell *params)
{
	CHECK_PARAMS(3, "FCNPC_SetWeaponSkillLevel");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	int iSkill = static_cast<int>(params[2]);
	int iLevel = static_cast<int>(params[3]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	if (iSkill < 0 || iSkill > 10) {
		return 0;
	}

	// Set the player weapon skill level
	pServer->GetPlayerManager()->GetAt(wNpcId)->SetWeaponSkill(iSkill, iLevel);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetWeaponSkillLevel(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_GetWeaponSkillLevel");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);
	int iSkill = static_cast<int>(params[2]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Get the player weapon skill level
	return pServer->GetPlayerManager()->GetAt(wNpcId)->GetWeaponSkill(iSkill);
}

// native FCNPC_SetWeaponState(npcid, weaponstate);
cell AMX_NATIVE_CALL CNatives::FCNPC_SetWeaponState(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_SetWeaponState");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);
	int iState = static_cast<int>(params[2]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	switch (iState) {
		case WEAPONSTATE_NO_BULLETS:
		case WEAPONSTATE_LAST_BULLET:
		case WEAPONSTATE_MORE_BULLETS:
		case WEAPONSTATE_RELOADING:
			pServer->GetPlayerManager()->GetAt(wNpcId)->SetWeaponState(iState);
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
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Get the player weapon state
	return pServer->GetPlayerManager()->GetAt(wNpcId)->GetWeaponState();
}

// native FCNPC_SetWeaponReloadTime(npcid, weaponid, time);
cell AMX_NATIVE_CALL CNatives::FCNPC_SetWeaponReloadTime(AMX *amx, cell *params)
{
	CHECK_PARAMS(3, "FCNPC_SetWeaponReloadTime");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);
	int iWeaponId = static_cast<int>(params[2]);
	int iReloadTime = static_cast<int>(params[3]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Set the player weapon reload time
	return pServer->GetPlayerManager()->GetAt(wNpcId)->SetWeaponReloadTime(iWeaponId, iReloadTime);
}

// native FCNPC_GetWeaponReloadTime(npcid, weaponid);
cell AMX_NATIVE_CALL CNatives::FCNPC_GetWeaponReloadTime(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_GetWeaponReloadTime");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);
	int iWeaponId = static_cast<int>(params[2]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Get the player weapon reload time
	return pServer->GetPlayerManager()->GetAt(wNpcId)->GetWeaponReloadTime(iWeaponId);
}

// native FCNPC_SetWeaponShootTime(npcid, weaponid, time);
cell AMX_NATIVE_CALL CNatives::FCNPC_SetWeaponShootTime(AMX *amx, cell *params)
{
	CHECK_PARAMS(3, "FCNPC_SetWeaponShootTime");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);
	int iWeaponId = static_cast<int>(params[2]);
	int iShootTime = static_cast<int>(params[3]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Set the player weapon shoot time
	return pServer->GetPlayerManager()->GetAt(wNpcId)->SetWeaponShootTime(iWeaponId, iShootTime);
}

// native FCNPC_GetWeaponShootTime(npcid, weaponid);
cell AMX_NATIVE_CALL CNatives::FCNPC_GetWeaponShootTime(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_GetWeaponShootTime");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);
	int iWeaponId = static_cast<int>(params[2]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Get the player weapon shoot time
	return pServer->GetPlayerManager()->GetAt(wNpcId)->GetWeaponShootTime(iWeaponId);
}

// native FCNPC_SetWeaponClipSize(npcid, weaponid, size);
cell AMX_NATIVE_CALL CNatives::FCNPC_SetWeaponClipSize(AMX *amx, cell *params)
{
	CHECK_PARAMS(3, "FCNPC_SetWeaponClipSize");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);
	int iWeaponId = static_cast<int>(params[2]);
	int iClipSize = static_cast<int>(params[3]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Set the player weapon clip size
	return pServer->GetPlayerManager()->GetAt(wNpcId)->SetWeaponClipSize(iWeaponId, iClipSize);
}

// native FCNPC_GetWeaponClipSize(npcid, weaponid);
cell AMX_NATIVE_CALL CNatives::FCNPC_GetWeaponClipSize(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_GetWeaponClipSize");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);
	int iWeaponId = static_cast<int>(params[2]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Get the player weapon clip size
	return pServer->GetPlayerManager()->GetAt(wNpcId)->GetWeaponClipSize(iWeaponId);
}

// native FCNPC_SetWeaponInfo(npcid, weaponid, reload_time = -1, shoot_time = -1, clip_size = -1);
cell AMX_NATIVE_CALL CNatives::FCNPC_SetWeaponInfo(AMX *amx, cell *params)
{
	CHECK_PARAMS(6, "FCNPC_SetWeaponInfo");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);
	int iWeaponId = static_cast<int>(params[2]);
	int iReloadTime = static_cast<int>(params[3]);
	int iShootTime = static_cast<int>(params[4]);
	int iClipSize = static_cast<int>(params[5]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Set the player weapon info
	SWeaponInfo sWeaponInfo = pServer->GetPlayerManager()->GetAt(wNpcId)->GetWeaponInfo(iWeaponId);

	if (iClipSize != -1) {
		sWeaponInfo.iClipSize = iClipSize;
	}

	if (iReloadTime != -1) {
		sWeaponInfo.iReloadTime = iReloadTime;
	}

	if (iShootTime != -1) {
		sWeaponInfo.iShootTime = iShootTime;
	}

	return pServer->GetPlayerManager()->GetAt(wNpcId)->SetWeaponInfo(iWeaponId, sWeaponInfo);
}

// native FCNPC_GetWeaponInfo(npcid, weaponid, &reload_time = -1, &shoot_time = -1, &clip_size = -1);
cell AMX_NATIVE_CALL CNatives::FCNPC_GetWeaponInfo(AMX *amx, cell *params)
{
	CHECK_PARAMS(6, "FCNPC_GetWeaponInfo");

	// get params
	WORD wNpcId = static_cast<WORD>(params[1]);
	int iWeaponId = static_cast<int>(params[2]);

	// valid weapon
	if (!CWeaponInfo::IsValid(iWeaponId)) {
		return 0;
	}

	// get weapon info
	SWeaponInfo sWeaponInfo = pServer->GetPlayerManager()->GetAt(wNpcId)->GetWeaponInfo(iWeaponId);

	// write data to amx
	cell *pAddress = NULL;
	amx_GetAddr(amx, params[2], &pAddress);
	*pAddress = sWeaponInfo.iReloadTime;

	amx_GetAddr(amx, params[3], &pAddress);
	*pAddress = sWeaponInfo.iShootTime;

	amx_GetAddr(amx, params[4], &pAddress);
	*pAddress = sWeaponInfo.iClipSize;

	return 1;
}

// native FCNPC_SetWeaponDefaultInfo(weaponid, reload_time = -1, shoot_time = -1, clip_size = -1);
cell AMX_NATIVE_CALL CNatives::FCNPC_SetWeaponDefaultInfo(AMX *amx, cell *params)
{
	CHECK_PARAMS(5, "FCNPC_SetWeaponDefaultInfo");

	// Get params
	int iWeaponId = static_cast<int>(params[1]);
	int iReloadTime = static_cast<int>(params[2]);
	int iShootTime = static_cast<int>(params[3]);
	int iClipSize = static_cast<int>(params[4]);

	// Set default weapon info
	SWeaponInfo sWeaponInfo = CWeaponInfo::GetDefaultInfo(iWeaponId);

	if (iClipSize != -1) {
		sWeaponInfo.iClipSize = iClipSize;
	}

	if (iReloadTime != -1) {
		sWeaponInfo.iReloadTime = iReloadTime;
	}

	if (iShootTime != -1) {
		sWeaponInfo.iShootTime = iShootTime;
	}

	return CWeaponInfo::SetDefaultInfo(iWeaponId, sWeaponInfo);
}

// native FCNPC_GetWeaponDefaultInfo(weaponid, &reload_time = -1, &shoot_time = -1, &clip_size = -1);
cell AMX_NATIVE_CALL CNatives::FCNPC_GetWeaponDefaultInfo(AMX *amx, cell *params)
{
	CHECK_PARAMS(5, "FCNPC_GetWeaponDefaultInfo");

	// get params
	int iWeaponId = static_cast<int>(params[1]);

	// valid weapon
	if (!CWeaponInfo::IsValid(iWeaponId)) {
		return 0;
	}

	// get weapon info
	SWeaponInfo sWeaponInfo = CWeaponInfo::GetDefaultInfo(iWeaponId);

	// write data to amx
	cell *pAddress = NULL;

	amx_GetAddr(amx, params[2], &pAddress);
	*pAddress = sWeaponInfo.iReloadTime;

	amx_GetAddr(amx, params[3], &pAddress);
	*pAddress = sWeaponInfo.iShootTime;

	amx_GetAddr(amx, params[4], &pAddress);
	*pAddress = sWeaponInfo.iClipSize;

	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_AimAt(AMX *amx, cell *params)
{
	CHECK_PARAMS(7, "FCNPC_AimAt");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	CVector vecPoint(amx_ctof(params[2]), amx_ctof(params[3]), amx_ctof(params[4]));
	bool bShoot = static_cast<bool>(params[5]);
	int iShootDelay = static_cast<int>(params[6]);
	bool bSetAngle = static_cast<bool>(params[7]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);

	int iWeaponType = pPlayerData->GetWeaponType(pPlayerData->GetWeapon());
	switch (iWeaponType) {
		case WEAPON_TYPE_MELEE:
		case WEAPON_TYPE_SHOOT:
		case WEAPON_TYPE_ROCKET:
		case WEAPON_TYPE_SPRAY:
		case WEAPON_TYPE_THROW:
			pPlayerData->StopAim();
			pPlayerData->AimAt(vecPoint, bShoot, iShootDelay, bSetAngle);
			return 1;
	}

	return 0;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_AimAtPlayer(AMX *amx, cell *params)
{
	CHECK_PARAMS(5, "FCNPC_AimAtPlayer");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	WORD wPlayerId = static_cast<WORD>(params[2]);
	bool bShoot = static_cast<bool>(params[3]);
	int iShootDelay = static_cast<int>(params[4]);
	bool bSetAngle = static_cast<bool>(params[5]);

	// Make sure the npc is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsPlayerConnected(wPlayerId) || wPlayerId == wNpcId) {
		return 0;
	}

	// Set the player aiming
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);

	int iWeaponType = pPlayerData->GetWeaponType(pPlayerData->GetWeapon());
	switch (iWeaponType) {
		case WEAPON_TYPE_MELEE:
		case WEAPON_TYPE_SHOOT:
		case WEAPON_TYPE_ROCKET:
		case WEAPON_TYPE_SPRAY:
		case WEAPON_TYPE_THROW:
			pPlayerData->StopAim();
			pPlayerData->AimAtPlayer(wPlayerId, bShoot, iShootDelay, bSetAngle);
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
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Stop the player aim
	pServer->GetPlayerManager()->GetAt(wNpcId)->StopAim();
	return 1;
}

// native FCNPC_MeleeAttack(npcid, delay = -1, bool:fightstyle = true);
cell AMX_NATIVE_CALL CNatives::FCNPC_MeleeAttack(AMX *amx, cell *params)
{
	CHECK_PARAMS(3, "FCNPC_MeleeAttack");

	// Get parms
	WORD wNpcId = static_cast<WORD>(params[1]);
	int iTime = static_cast<int>(params[2]);
	bool bUseFightstyle = static_cast<bool>(params[3]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Set the player melee attack
	return pServer->GetPlayerManager()->GetAt(wNpcId)->MeleeAttack(iTime, bUseFightstyle);
}

cell AMX_NATIVE_CALL CNatives::FCNPC_StopAttack(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_StopAttack");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Stop the player attack
	pServer->GetPlayerManager()->GetAt(wNpcId)->StopAttack();
	return 1;
}

// native FCNPC_IsAttacking(npcid);
cell AMX_NATIVE_CALL CNatives::FCNPC_IsAttacking(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_IsAttacking");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Get the player attacking state
	return pServer->GetPlayerManager()->GetAt(wNpcId)->IsAttacking();
}

cell AMX_NATIVE_CALL CNatives::FCNPC_IsAiming(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_IsAiming");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Get the player aiming state
	return pServer->GetPlayerManager()->GetAt(wNpcId)->IsAiming();
}

// native FCNPC_IsAimingAtPlayer(npcid, playerid);
cell AMX_NATIVE_CALL CNatives::FCNPC_IsAimingAtPlayer(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_IsAimingAtPlayer");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	WORD wPlayerId = static_cast<WORD>(params[2]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	if (!pServer->GetPlayerManager()->IsPlayerConnected(wPlayerId) || wPlayerId == wNpcId) {
		return 0;
	}

	// Get the player aiming state
	return pServer->GetPlayerManager()->GetAt(wNpcId)->IsAimingAtPlayer(wPlayerId);
}

cell AMX_NATIVE_CALL CNatives::FCNPC_IsShooting(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_IsShooting");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Get the player shooting state
	return pServer->GetPlayerManager()->GetAt(wNpcId)->IsShooting();
}

cell AMX_NATIVE_CALL CNatives::FCNPC_IsReloading(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_IsReloading");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Get the player shooting state
	return pServer->GetPlayerManager()->GetAt(wNpcId)->IsReloading();
}

cell AMX_NATIVE_CALL CNatives::FCNPC_EnterVehicle(AMX *amx, cell *params)
{
	CHECK_PARAMS(4, "FCNPC_EnterVehicle");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	int iVehicleId = static_cast<int>(params[2]);
	int iSeatId = static_cast<int>(params[3]);
	int iType = static_cast<int>(params[4]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Make the player enter the vehicle
	return pServer->GetPlayerManager()->GetAt(wNpcId)->EnterVehicle(iVehicleId, iSeatId, iType);
}

cell AMX_NATIVE_CALL CNatives::FCNPC_ExitVehicle(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_ExitVehicle");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Make the player exit the vehicle
	return pServer->GetPlayerManager()->GetAt(wNpcId)->ExitVehicle();
}

cell AMX_NATIVE_CALL CNatives::FCNPC_PutInVehicle(AMX *amx, cell *params)
{
	CHECK_PARAMS(3, "FCNPC_PutInVehicle");

	// Get the parameters
	WORD wNpcId = static_cast<WORD>(params[1]);
	int iVehicleId = static_cast<int>(params[2]);
	int iSeatId = static_cast<int>(params[3]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Make the player enter the vehicle
	return pServer->GetPlayerManager()->GetAt(wNpcId)->PutInVehicle(iVehicleId, iSeatId);
}

cell AMX_NATIVE_CALL CNatives::FCNPC_RemoveFromVehicle(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_RemoveFromVehicle");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Get the player data pointer
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);

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
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return INVALID_VEHICLE_ID;
	}

	// Make the player exit the vehicle
	return pServer->GetPlayerManager()->GetAt(wNpcId)->GetVehicleId();
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetVehicleSeat(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetVehicleSeat");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Get the player data pointer
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);

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
	bool bSiren = static_cast<bool>(params[2]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Get the player data pointer
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);

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
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Get the player data pointer
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);

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
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Get the player data pointer
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wNpcId);

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
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return amx_ftoc(fHealth);
	}

	// Make sure the player is in vehicle
	if (pServer->GetPlayerManager()->GetAt(wNpcId)->GetVehicleId() == INVALID_VEHICLE_ID) {
		return amx_ftoc(fHealth);
	}

	// Get the vehicle health
	fHealth = pServer->GetPlayerManager()->GetAt(wNpcId)->GetVehicleHealth();
	return amx_ftoc(fHealth);
}


// native FCNPC_SetSurfingOffsets(npcid, Float:fX, Float:fY, Float:fZ);
cell AMX_NATIVE_CALL CNatives::FCNPC_SetSurfingOffsets(AMX *amx, cell *params)
{
	CHECK_PARAMS(4, "FCNPC_SetSurfingOffsets");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);
	CVector vecPoint(amx_ctof(params[2]), amx_ctof(params[3]), amx_ctof(params[4]));

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Change vehicle health
	pServer->GetPlayerManager()->GetAt(wNpcId)->SetSurfingOffsets(vecPoint);
	return 1;
}

// native FCNPC_GetSurfingOffsets(npcid, &Float:fX, &Float:fY, &Float:fZ);
cell AMX_NATIVE_CALL CNatives::FCNPC_GetSurfingOffsets(AMX *amx, cell *params)
{
	CHECK_PARAMS(4, "FCNPC_GetSurfingOffsets");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	CVector vecOffsets;
	pServer->GetPlayerManager()->GetAt(wNpcId)->GetSurfingOffsets(&vecOffsets);

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
	int iVehicleId = static_cast<int>(params[2]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Valid vehicle
	if (iVehicleId < 1 || iVehicleId > MAX_VEHICLES) {
		return 0;
	}

	// Set the surfing vehicle
	pServer->GetPlayerManager()->GetAt(wNpcId)->SetSurfingVehicle(iVehicleId);
	return 1;
}

// native FCNPC_GetSurfingVehicle(npcid);
cell AMX_NATIVE_CALL CNatives::FCNPC_GetSurfingVehicle(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetSurfingVehicle");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return INVALID_VEHICLE_ID;
	}

	// Get the surfing vehicle
	return pServer->GetPlayerManager()->GetAt(wNpcId)->GetSurfingVehicle();
}

// native FCNPC_SetSurfingObject(npcid, objectid);
cell AMX_NATIVE_CALL CNatives::FCNPC_SetSurfingObject(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_SetSurfingObject");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);
	int iObjectId = static_cast<int>(params[2]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Validate the object
	if (iObjectId < 1 || iObjectId > MAX_OBJECTS) {
		return 0;
	}

	// Set the surfing object
	pServer->GetPlayerManager()->GetAt(wNpcId)->SetSurfingObject(iObjectId);
	return 1;
}

// native FCNPC_GetSurfingObject(npcid);
cell AMX_NATIVE_CALL CNatives::FCNPC_GetSurfingObject(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetSurfingObject");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return INVALID_OBJECT_ID;
	}

	// Get the surfing object
	return pServer->GetPlayerManager()->GetAt(wNpcId)->GetSurfingObject();
}

// native FCNPC_SetSurfingPlayerObject(npcid, objectid);
cell AMX_NATIVE_CALL CNatives::FCNPC_SetSurfingPlayerObject(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_SetSurfingPlayerObject");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);
	int iObjectId = static_cast<int>(params[2]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Validate the object
	if (iObjectId < 1 || iObjectId > MAX_OBJECTS) {
		return 0;
	}

	// Set the surfing object
	pServer->GetPlayerManager()->GetAt(wNpcId)->SetSurfingPlayerObject(iObjectId);
	return 1;
}

// native FCNPC_GetSurfingPlayerObject(npcid);
cell AMX_NATIVE_CALL CNatives::FCNPC_GetSurfingPlayerObject(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetSurfingPlayerObject");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return INVALID_OBJECT_ID;
	}

	// Get the surfing object
	return pServer->GetPlayerManager()->GetAt(wNpcId)->GetSurfingPlayerObject();
}

// native FCNPC_StopSurfing(npcid);
cell AMX_NATIVE_CALL CNatives::FCNPC_StopSurfing(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_StopSurfing");

	// Get params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Get the surfing object
	pServer->GetPlayerManager()->GetAt(wNpcId)->StopSurfing();
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_ToggleReloading(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_ToggleReloading");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	bool bReload = static_cast<bool>(params[2]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Toggle it
	pServer->GetPlayerManager()->GetAt(wNpcId)->ToggleReloading(bReload);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_ToggleInfiniteAmmo(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_ToggleInfiniteAmmo");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	bool bToggle = static_cast<bool>(params[2]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Toggle it
	pServer->GetPlayerManager()->GetAt(wNpcId)->ToggleInfiniteAmmo(bToggle);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_StartPlayingPlayback(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_StartPlayingPlayback");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	char *szFile;
	amx_StrParam(amx, params[2], szFile);

	// Make sure the length is valid
	if (strlen(szFile) == 0) {
		return 0;
	}

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Start Playing the player playback
	return pServer->GetPlayerManager()->GetAt(wNpcId)->StartPlayingPlayback(szFile);
}

cell AMX_NATIVE_CALL CNatives::FCNPC_StopPlayingPlayback(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_StopPlayingPlayback");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Stop Playing the player playback
	pServer->GetPlayerManager()->GetAt(wNpcId)->StopPlayingPlayback();
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_PausePlayingPlayback(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_PausePlayingPlayback");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Pause Playing the player playback
	pServer->GetPlayerManager()->GetAt(wNpcId)->PausePlayingPlayback();
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_ResumePlayingPlayback(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_ResumePlayingPlayback");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Resume Playing the player playback
	pServer->GetPlayerManager()->GetAt(wNpcId)->ResumePlayingPlayback();
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
	int iPoint = static_cast<int>(params[2]);

	// Make sure the node is valid
	if (!pServer->GetNodeManager()->IsNodeOpen(iNodeId)) {
		return NODE_TYPE_NONE;
	}

	// Set the node type
	pServer->GetNodeManager()->GetAt(iNodeId)->SetPoint(iPoint);
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
	unsigned long ulVehNodes, ulPedNodes, ulNaviNodes;
	pServer->GetNodeManager()->GetAt(iNodeId)->GetHeaderInfo(&ulVehNodes, &ulPedNodes, &ulNaviNodes);

	// Get the argument pointers and set its value
	cell *pAddress = NULL;
	amx_GetAddr(amx, params[2], &pAddress);
	*pAddress = ulVehNodes;

	amx_GetAddr(amx, params[3], &pAddress);
	*pAddress = ulPedNodes;

	amx_GetAddr(amx, params[4], &pAddress);
	*pAddress = ulNaviNodes;

	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_PlayNode(AMX *amx, cell *params)
{
	CHECK_PARAMS(3, "FCNPC_PlayNode");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);
	int iNodeId = static_cast<int>(params[2]);
	int iType = static_cast<int>(params[3]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Make sure the node is valid
	if (!pServer->GetNodeManager()->IsNodeOpen(iNodeId)) {
		return 0;
	}

	// Play the node
	return pServer->GetPlayerManager()->GetAt(wNpcId)->PlayNode(iNodeId, iType);
}

cell AMX_NATIVE_CALL CNatives::FCNPC_StopPlayingNode(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_StopPlayingNode");

	// Get the params
	WORD wNpcId = static_cast<WORD>(params[1]);

	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsNpcConnected(wNpcId)) {
		return 0;
	}

	// Stop playing the node
	pServer->GetPlayerManager()->GetAt(wNpcId)->StopPlayingNode();
	return 1;
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
