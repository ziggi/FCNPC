/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: Natives.cpp
	- Author(s): OrMisicL

  =========================================*/

#include "Main.h"

extern CServer        *pServer;
extern CNetGame    *pNetGame;

cell AMX_NATIVE_CALL CNatives::FCNPC_Create(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_Create");
	// Get the name length
	int iLength = 0;
	cell *pAddress = NULL;
	amx_GetAddr(amx, params[1], &pAddress);
	amx_StrLen(pAddress, &iLength);
	// Make sure the length is valid
	if(!iLength)
		return INVALID_ENTITY_ID;

	// Make room for the string end
	iLength++;
	// Allocate the name string
	char *szName = new char [iLength];
	// Get the name
	amx_GetString(szName, pAddress, 0, iLength);
	// Create the player in the player manager
	return pServer->GetPlayerManager()->AddPlayer(szName);
}

cell AMX_NATIVE_CALL CNatives::FCNPC_Destroy(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_Destroy");
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	// Destoy the player
	return pServer->GetPlayerManager()->DeletePlayer(iNPCId);
}

cell AMX_NATIVE_CALL CNatives::FCNPC_Spawn(AMX *amx, cell *params)
{
	CHECK_PARAMS(5, "FCNPC_Spawn");
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Get the NPC skin id
	int iSkin = (int)params[2];
	// Get the position coordinates
	float fX = amx_ctof(params[3]);
	float fY = amx_ctof(params[4]);
	float fZ = amx_ctof(params[5]);
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	// Set the spawn position
	pServer->GetPlayerManager()->GetAt(iNPCId)->SetSpawnPosition(CVector(fX, fY, fZ));
	// Spawn the player
	return pServer->GetPlayerManager()->GetAt(iNPCId)->Spawn(iSkin) ? 1 : 0;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_Respawn(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_Respawn");
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	// Respawn the player
	return pServer->GetPlayerManager()->GetAt(iNPCId)->Respawn() ? 1 : 0;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_IsSpawned(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_IsSpawned");
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	// Respawn the player
	return pServer->GetPlayerManager()->GetAt(iNPCId)->IsSpawned() ? 1 : 0;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_Kill(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_Kill");
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	// Kill the player
	pServer->GetPlayerManager()->GetAt(iNPCId)->Kill(INVALID_ENTITY_ID, 0);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_IsDead(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_IsDead");
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	// Return the player dead state
	return pServer->GetPlayerManager()->GetAt(iNPCId)->GetState() == PLAYER_STATE_WASTED ? 1 : 0;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_SetPosition(AMX *amx, cell *params)
{
	CHECK_PARAMS(4, "FCNPC_SetPosition");
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Get the position coordinates
	float fX = amx_ctof(params[2]);
	float fY = amx_ctof(params[3]);
	float fZ = amx_ctof(params[4]);
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	// Set the player position
	pServer->GetPlayerManager()->GetAt(iNPCId)->SetPosition(CVector(fX, fY, fZ));
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetPosition(AMX *amx, cell *params)
{
	CHECK_PARAMS(4, "FCNPC_GetPosition");
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
	{
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
	pServer->GetPlayerManager()->GetAt(iNPCId)->GetPosition(&vecPosition);
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
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Get the angle
	float fAngle = amx_ctof(params[2]);
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	// Set the player angle
	pServer->GetPlayerManager()->GetAt(iNPCId)->SetAngle(fAngle);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetAngle(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetAngle");
	// Get the NPC id
	int iNPCId = (int)params[1];
	float Angle = 0.0f;
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return amx_ftoc(Angle);

	// Get the player angle
	Angle = pServer->GetPlayerManager()->GetAt(iNPCId)->GetAngle();
	return amx_ftoc(Angle);
}

cell AMX_NATIVE_CALL CNatives::FCNPC_SetInterior(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_SetInterior");
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Get the interior
	int iInterior = (int)params[2];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	// Set the player interior
	pServer->GetPlayerManager()->GetAt(iNPCId)->SetInterior(iInterior);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetInterior(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetInterior");
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	// Get the player interior
	return pServer->GetPlayerManager()->GetAt(iNPCId)->GetInterior();
}

cell AMX_NATIVE_CALL CNatives::FCNPC_SetVirtualWorld(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_SetVirtualWorld");
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Get the virtual world
	int iVirtualWorld = (int)params[2];
	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	// Set the player virtual world
	pNetGame->pPlayerPool->dwVirtualWorld[iNPCId] = iVirtualWorld;
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetVirtualWorld(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetVirtualWorld");
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	// Get the player virtual world
	return pNetGame->pPlayerPool->dwVirtualWorld[iNPCId];
}

cell AMX_NATIVE_CALL CNatives::FCNPC_SetQuaternion(AMX *amx, cell *params)
{
	CHECK_PARAMS(5, "FCNPC_SetQuaternion");
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Get the quaternion
	float *fQuaternion = new float [4];
	fQuaternion[0] = amx_ctof(params[2]);
	fQuaternion[1] = amx_ctof(params[3]);
	fQuaternion[2] = amx_ctof(params[4]);
	fQuaternion[3] = amx_ctof(params[5]);
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	// Set the player quaternion
	pServer->GetPlayerManager()->GetAt(iNPCId)->SetQuaternion(fQuaternion);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetQuaternion(AMX *amx, cell *params)
{
	CHECK_PARAMS(5, "FCNPC_GetQuaternion");
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
	{
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
	pServer->GetPlayerManager()->GetAt(iNPCId)->GetQuaternion(fQuaternion);
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
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_SetVelocity(AMX *amx, cell *params)
{
	CHECK_PARAMS(4, "FCNPC_SetVelocity");
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Get the velocity vector
	float fX = amx_ctof(params[2]);
	float fY = amx_ctof(params[3]);
	float fZ = amx_ctof(params[4]);
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	// Set the player velocity
	pServer->GetPlayerManager()->GetAt(iNPCId)->SetVelocity(CVector(fX, fY, fZ));
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetVelocity(AMX *amx, cell *params)
{
	CHECK_PARAMS(4, "FCNPC_GetVelocity");
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
	{
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
	pServer->GetPlayerManager()->GetAt(iNPCId)->GetVelocity(&vecVelocity);
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
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Get the health
	float fHealth = amx_ctof(params[2]);
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	// Set the player health
	pServer->GetPlayerManager()->GetAt(iNPCId)->SetHealth(fHealth);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetHealth(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetHealth");
	// Get the NPC id
	int iNPCId = (int)params[1];
	float fHealth = 0.0f;
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return amx_ftoc(fHealth);

	// Get the player health
	fHealth = pServer->GetPlayerManager()->GetAt(iNPCId)->GetHealth();
	return amx_ftoc(fHealth);
}

cell AMX_NATIVE_CALL CNatives::FCNPC_SetArmour(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_SetArmour");
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Get the armour
	float fArmour = amx_ctof(params[2]);
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	// Set the player armour
	pServer->GetPlayerManager()->GetAt(iNPCId)->SetArmour(fArmour);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetArmour(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetArmour");
	// Get the NPC id
	int iNPCId = (int)params[1];
	float fArmour = 0.0f;
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return amx_ftoc(fArmour);

	// Get the player armour
	fArmour = pServer->GetPlayerManager()->GetAt(iNPCId)->GetArmour();
	return amx_ftoc(fArmour);
}

cell AMX_NATIVE_CALL CNatives::FCNPC_SetSkin(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_SetSkin");
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Get the skin
	int iSkin = (int)params[2];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	// Set the player skin
	pServer->GetPlayerManager()->GetAt(iNPCId)->SetSkin(iSkin);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetSkin(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetSkin");
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	// Get the player skin
	return pServer->GetPlayerManager()->GetAt(iNPCId)->GetSkin();
}

cell AMX_NATIVE_CALL CNatives::FCNPC_SetKeys(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_SetKeys");
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Get the keys
	DWORD dwKeys = (DWORD)params[2];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	// Get the player keys
	WORD wUDAnalog = 0, wLRAnalog = 0;
	DWORD _dwKeys = 0;
	pServer->GetPlayerManager()->GetAt(iNPCId)->GetKeys(&wUDAnalog, &wLRAnalog, &_dwKeys);
	// Set the player keys
	pServer->GetPlayerManager()->GetAt(iNPCId)->SetKeys(wUDAnalog, wLRAnalog, dwKeys);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetKeys(AMX *amx, cell *params)
{
	CHECK_PARAMS(4, "FCNPC_GetKeys");
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
	{
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
	pServer->GetPlayerManager()->GetAt(iNPCId)->GetKeys(&wUDAnalog, &wLRAnalog, &dwKeys);
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
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Get the action id
	int iActionId = (int)params[2];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	// Set the player special action
	pServer->GetPlayerManager()->GetAt(iNPCId)->SetSpecialAction(iActionId);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetSpecialAction(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetSpecialAction");
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	// Get the player special action
	return pServer->GetPlayerManager()->GetAt(iNPCId)->GetSpecialAction();
}

// native FCNPC_SetAnimation(npcid, animationid, Float:fDelta = 4.1, loop = 0, lockx = 1, locky = 1, freeze = 0, time = 1);
cell AMX_NATIVE_CALL CNatives::FCNPC_SetAnimation(AMX *amx, cell *params)
{
	CHECK_PARAMS(8, "FCNPC_SetAnimation");

	// Get parms
	int iNPCId = (int)params[1];
	int iAnimationId = (int)params[2];
	float fDelta = amx_ctof(params[3]);
	bool bLoop = (bool)params[4];
	bool bLockX = (bool)params[5];
	bool bLockY = (bool)params[6];
	bool bFreeze = (bool)params[7];
	int iTime = (int)params[8];

	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	// Set the player animation
	pServer->GetPlayerManager()->GetAt(iNPCId)->
		SetAnimation(iAnimationId, fDelta, bLoop, bLockX, bLockY, bFreeze, iTime);
	return 1;
}

// native FCNPC_GetAnimation(npcid, &animationid = 0, &Float:fDelta = 4.1, &loop = 0, &lockx = 1, &locky = 1, &freeze = 0, &time = 1);
cell AMX_NATIVE_CALL CNatives::FCNPC_GetAnimation(AMX *amx, cell *params)
{
	CHECK_PARAMS(8, "FCNPC_GetAnimation");
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	// Get parms
	int iAnimationId;
	float fDelta;
	bool bLoop;
	bool bLockX;
	bool bLockY;
	bool bFreeze;
	int iTime;

	// Get the player animation
	pServer->GetPlayerManager()->GetAt(iNPCId)->
		GetAnimation(&iAnimationId, &fDelta, &bLoop, &bLockX, &bLockY, &bFreeze, &iTime);

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

cell AMX_NATIVE_CALL CNatives::FCNPC_GoTo(AMX *amx, cell *params)
{
	CHECK_PARAMS(8, "FCNPC_GoTo");
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Get the velocity vector
	float fX = amx_ctof(params[2]);
	float fY = amx_ctof(params[3]);
	float fZ = amx_ctof(params[4]);
	// Get the mouvement type
	int iType = (int)params[5];
	// Get the mouvement driving speed
	float fSpeed = amx_ctof(params[6]);
	// Get the ZMap status
	int iZMap = (int)params[7];
	// Get the radius
	float fRadius = amx_ctof(params[8]);
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	// Set the player velocity
	pServer->GetPlayerManager()->GetAt(iNPCId)->SetVelocity(CVector(fSpeed, fSpeed, fSpeed));
	// Move the player
	return pServer->GetPlayerManager()->GetAt(iNPCId)->GoTo(CVector(fX, fY, fZ), iType, !iZMap ? false : true, fRadius);
}

// native FCNPC_GoToPlayer(npcid, playereid, type, Float:speed, bool:UseZMap = false, Float:radius = 0.0);
cell AMX_NATIVE_CALL CNatives::FCNPC_GoToPlayer(AMX *amx, cell *params)
{
	CHECK_PARAMS(6, "FCNPC_GoToPlayer");

	// get params
	int iNPCId = (int)params[1];
	int iPlayerId = (int)params[2];
	int iType = (int)params[3];
	float fSpeed = amx_ctof(params[4]);
	int iZMap = (int)params[5];
	float fRadius = amx_ctof(params[6]);

	// validation
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	if (iPlayerId < 0 || iPlayerId > MAX_PLAYERS)
		return 0;

	if (!pNetGame->pPlayerPool->bIsPlayerConnectedEx[iPlayerId] || iPlayerId == iNPCId)
		return 0;

	// set the player velocity
	pServer->GetPlayerManager()->GetAt(iNPCId)->SetVelocity(CVector(fSpeed, fSpeed, fSpeed));

	// move the player
	CVector vecPos = pNetGame->pPlayerPool->pPlayer[iPlayerId]->vecPosition;
	return pServer->GetPlayerManager()->GetAt(iNPCId)->GoTo(vecPos, iType, !iZMap ? false : true, fRadius);
}

cell AMX_NATIVE_CALL CNatives::FCNPC_Stop(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_Stop");
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	// Stop the player
	pServer->GetPlayerManager()->GetAt(iNPCId)->StopMoving();
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_IsMoving(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_IsMoving");
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	// Get the player moving state
	return pServer->GetPlayerManager()->GetAt(iNPCId)->IsMoving() ? 1 : 0;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_SetWeapon(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_SetWeapon");
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Get the weapon id
	int iWeapon = (int)params[2];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	// Set the player weapon
	pServer->GetPlayerManager()->GetAt(iNPCId)->SetWeapon(iWeapon);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetWeapon(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetWeapon");
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	// Get the player weapon
	return pServer->GetPlayerManager()->GetAt(iNPCId)->GetWeapon();
}

cell AMX_NATIVE_CALL CNatives::FCNPC_SetAmmo(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_SetAmmo");
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Get the ammo
	int iAmmo = (int)params[2];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	// Set the player ammo
	pServer->GetPlayerManager()->GetAt(iNPCId)->SetAmmo(iAmmo);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetAmmo(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetAmmo");
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	// Get the player ammo
	return pServer->GetPlayerManager()->GetAt(iNPCId)->GetAmmo();
}

cell AMX_NATIVE_CALL CNatives::FCNPC_SetWeaponSkillLevel(AMX *amx, cell *params)
{
	CHECK_PARAMS(3, "FCNPC_SetWeaponSkillLevel");
	// Get params
	int iNPCId = (int)params[1];
	int iSkill = (int)params[2];
	int iLevel = (int)params[3];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	if (iSkill < 0 || iSkill > 10)
		return 0;

	// Set the player ammo
	pServer->GetPlayerManager()->GetAt(iNPCId)->SetWeaponSkill(iSkill, iLevel);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetWeaponSkillLevel(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_GetWeaponSkillLevel");
	// Get params
	int iNPCId = (int)params[1];
	int iSkill = (int)params[2];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	// Get the player ammo
	return pServer->GetPlayerManager()->GetAt(iNPCId)->GetWeaponSkill(iSkill);
}

cell AMX_NATIVE_CALL CNatives::FCNPC_AimAt(AMX *amx, cell *params)
{
	CHECK_PARAMS(5, "FCNPC_AimAt");
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Get the aim vector
	float fX = amx_ctof(params[2]);
	float fY = amx_ctof(params[3]);
	float fZ = amx_ctof(params[4]);
	// Get the shooting flag
	int iShoot = (int)params[5];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	/*// Thanks to abagail
	CVector vecPosition;
	pServer->GetPlayerManager()->GetAt(iNPCId)->GetPosition(&vecPosition);
	// The coordinates are too far away to shoot at.
	if (amx_ftoc(vecPosition.fX) > fX + 100.0 || amx_ftoc(vecPosition.fX) < fX + 100.0 || amx_ftoc(vecPosition.fY) > fY + 100.0 || amx_ftoc(vecPosition.fY) < fY + 100.0)
		return 0;*/

	// Set the player aiming
	pServer->GetPlayerManager()->GetAt(iNPCId)->AimAt(CVector(fX, fY, fZ), !iShoot ? false : true);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_AimAtPlayer(AMX *amx, cell *params)
{
	CHECK_PARAMS(3, "FCNPC_AimAtPlayer");
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Get the aim playerid
	int iPlayerId = (int)params[2];
	// Get the shooting flag
	int iShoot = (int)params[3];
	// Make sure the npc is valid
	if (!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;
	// Make sure the player is valid
	if (iPlayerId < 0 || iPlayerId > MAX_PLAYERS)
		return 0;

	if (!pNetGame->pPlayerPool->bIsPlayerConnectedEx[iPlayerId] || iPlayerId == iNPCId)
		return 0;

	// Set the player aiming
	pServer->GetPlayerManager()->GetAt(iNPCId)->AimAtPlayer(iPlayerId, !iShoot ? false : true);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_MeleeAttack(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_MeleeAttack");
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Get the attack time
	int iTime = (int)params[2];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	// Set the player melee attack
	pServer->GetPlayerManager()->GetAt(iNPCId)->MeleeAttack(iTime);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_StopAttack(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_StopAttack");
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	// Stop the player attack
	pServer->GetPlayerManager()->GetAt(iNPCId)->StopAttack();
	return 1;
}


cell AMX_NATIVE_CALL CNatives::FCNPC_StopAim(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_StopAim");
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	// Stop the player aim
	pServer->GetPlayerManager()->GetAt(iNPCId)->StopAim();
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_IsAiming(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_IsAiming");
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	// Get the player aiming state
	return pServer->GetPlayerManager()->GetAt(iNPCId)->IsAiming() ? 1 : 0;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_IsShooting(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_IsShooting");
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	// Get the player shooting state
	return pServer->GetPlayerManager()->GetAt(iNPCId)->IsShooting() ? 1 : 0;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_IsReloading(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_IsReloading");
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	// Get the player shooting state
	return pServer->GetPlayerManager()->GetAt(iNPCId)->IsReloading() ? 1 : 0;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_EnterVehicle(AMX *amx, cell *params)
{
	CHECK_PARAMS(4, "FCNPC_EnterVehicle");
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Get the vehicle id
	int iVehicleId = (int)params[2];
	// Get the seat id
	int iSeatId = (int)params[3];
	// Get the move type
	int iType = (int)params[4];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	// Make the player enter the vehicle
	return pServer->GetPlayerManager()->GetAt(iNPCId)->EnterVehicle(iVehicleId, iSeatId, iType) ? 1 : 0;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_ExitVehicle(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_ExitVehicle");
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	// Make the player exit the vehicle
	return pServer->GetPlayerManager()->GetAt(iNPCId)->ExitVehicle() ? 1 : 0;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_PutInVehicle(AMX *amx, cell *params)
{
	CHECK_PARAMS(3, "FCNPC_PutInVehicle");
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Get the vehicle id
	int iVehicleId = (int)params[2];
	// Get the seat id
	int iSeatId = (int)params[3];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	// Make the player enter the vehicle
	return pServer->GetPlayerManager()->GetAt(iNPCId)->PutInVehicle(iVehicleId, iSeatId) ? 1 : 0;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_RemoveFromVehicle(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_RemoveFromVehicle");
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	// Make the player exit the vehicle
	return pServer->GetPlayerManager()->GetAt(iNPCId)->RemoveFromVehicle() ? 1 : 0;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetVehicleID(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetVehicleID");
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return INVALID_ENTITY_ID;

	// Make the player exit the vehicle
	return pServer->GetPlayerManager()->GetAt(iNPCId)->GetVehicleId();
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetVehicleSeat(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetVehicleSeat");
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	// Make the player exit the vehicle
	if (pServer->GetPlayerManager()->GetAt(iNPCId)->GetVehicleId() == INVALID_ENTITY_ID)
		return 128;

	return pServer->GetPlayerManager()->GetAt(iNPCId)->GetSeatId();
}

// native FCNPC_SetVehicleSiren(npcid, bool:status);
cell AMX_NATIVE_CALL CNatives::FCNPC_SetVehicleSiren(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_SetVehicleSiren");

	// Get params
	int iNPCId = (int)params[1];
	int iSiren = (int)params[2];

	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	// Make sure the player is in vehicle
	if (pServer->GetPlayerManager()->GetAt(iNPCId)->GetVehicleId() == INVALID_ENTITY_ID)
		return 0;

	// Change siren state
	pServer->GetPlayerManager()->GetAt(iNPCId)->SetVehicleSiren(iSiren);
	return 1;
}

// native FCNPC_GetVehicleSiren(npcid);
cell AMX_NATIVE_CALL CNatives::FCNPC_GetVehicleSiren(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetVehicleSiren");

	// Get params
	int iNPCId = (int)params[1];

	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	// Make sure the player is in vehicle
	if (pServer->GetPlayerManager()->GetAt(iNPCId)->GetVehicleId() == INVALID_ENTITY_ID)
		return 0;

	// Return siren state
	return pServer->GetPlayerManager()->GetAt(iNPCId)->GetVehicleSiren() ? 1 : 0;
}

// native FCNPC_SetVehicleHealth(npcid, Float:health);
cell AMX_NATIVE_CALL CNatives::FCNPC_SetVehicleHealth(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_SetVehicleHealth");

	// Get params
	int iNPCId = (int)params[1];
	float fHealth = amx_ctof(params[2]);

	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	// Make sure the player is in vehicle
	if (pServer->GetPlayerManager()->GetAt(iNPCId)->GetVehicleId() == INVALID_ENTITY_ID)
		return 0;

	// Change vehicle health
	pServer->GetPlayerManager()->GetAt(iNPCId)->SetVehicleHealth(fHealth);
	return 1;
}

// native Float:FCNPC_GetVehicleHealth(npcid);
cell AMX_NATIVE_CALL CNatives::FCNPC_GetVehicleHealth(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetVehicleHealth");

	// Get params
	int iNPCId = (int)params[1];
	float fHealth = 0.0f;

	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return amx_ftoc(fHealth);

	// Make sure the player is in vehicle
	if (pServer->GetPlayerManager()->GetAt(iNPCId)->GetVehicleId() == INVALID_ENTITY_ID)
		return amx_ftoc(fHealth);

	// Get the vehicle health
	fHealth = pServer->GetPlayerManager()->GetAt(iNPCId)->GetVehicleHealth();
	return amx_ftoc(fHealth);
}

cell AMX_NATIVE_CALL CNatives::FCNPC_ToggleReloading(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_ToggleReloading");
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Get the reloading state
	int iReload = (int)params[2];
	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	// Toggle it
	pServer->GetPlayerManager()->GetAt(iNPCId)->ToggleReloading(iReload ? true : false);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_ToggleInfiniteAmmo(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_ToggleInfiniteAmmo");
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Get the reloading state
	int iToggle = (int)params[2];
	// Make sure the player is valid
	if (!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	// Toggle it
	pServer->GetPlayerManager()->GetAt(iNPCId)->ToggleInfiniteAmmo(iToggle ? true : false);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_StartPlayingPlayback(AMX *amx, cell *params)
{
	CHECK_PARAMS(2, "FCNPC_StartPlayingPlayback");
	// Get the npc id
	int iNPCId = (int)params[1];
	// Get the filename length
	int iLength = 0;
	cell *pAddress = NULL;
	amx_GetAddr(amx, params[2], &pAddress);
	amx_StrLen(pAddress, &iLength);
	// Make sure the length is valid
	if(!iLength)
		return 0;
	
	// Make room for the string end
	iLength++;
	// Allocate the filename string
	char *szFile = new char [iLength];
	// Get the name
	amx_GetString(szFile, pAddress, 0, iLength);
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	// Start Playing the player playback
	return pServer->GetPlayerManager()->GetAt(iNPCId)->StartPlayingPlayback(szFile) ? 1 : 0;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_StopPlayingPlayback(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_StopPlayingPlayback");
	// Get the npc id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	// Stop Playing the player playback
	pServer->GetPlayerManager()->GetAt(iNPCId)->StopPlayingPlayback();
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_PausePlayingPlayback(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_PausePlayingPlayback");
	// Get the npc id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	// Pause Playing the player playback
	pServer->GetPlayerManager()->GetAt(iNPCId)->PausePlayingPlayback();
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_ResumePlayingPlayback(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_ResumePlayingPlayback");
	// Get the npc id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	// Resume Playing the player playback
	pServer->GetPlayerManager()->GetAt(iNPCId)->ResumePlayingPlayback();
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_SetUpdateRate(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_SetUpdateRate");
	// Get the update rate
	int iRate = (int)params[1];
	// Set the update rate
	pServer->SetUpdateRate(iRate);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_OpenNode(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_OpenNode");
	// Get the node id
	int iNodeId = (int)params[1];
	// Open the node
	return pServer->GetNodeManager()->OpenNode(iNodeId) ? 1 : 0;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_CloseNode(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_CloseNode");
	// Get the node id
	int iNodeId = (int)params[1];
	// Close the node
	pServer->GetNodeManager()->CloseNode(iNodeId);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_IsNodeOpen(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_IsNodeOpen");
	// Get the node id
	int iNodeId = (int)params[1];
	// Open the node
	return pServer->GetNodeManager()->IsNodeOpen(iNodeId) ? 1 : 0;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetNodeType(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetNodeType");
	// Get the node id
	int iNodeId = (int)params[1];
	// Make sure the node is valid
	if(!pServer->GetNodeManager()->IsNodeOpen(iNodeId))
		return NODE_TYPE_NONE;

	// Get the node type
	return pServer->GetNodeManager()->GetAt(iNodeId)->GetNodeType();
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetNodePointCount(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_GetNodePointCount");
	// Get the node id
	int iNodeId = (int)params[1];
	// Make sure the node is valid
	if(!pServer->GetNodeManager()->IsNodeOpen(iNodeId))
		return 0;

	// Get the node point count
	return pServer->GetNodeManager()->GetAt(iNodeId)->GetNodesNumber();
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetNodePointPosition(AMX *amx, cell *params)
{
	CHECK_PARAMS(4, "FCNPC_GetNodePointPosition");
	// Get the node id
	int iNodeId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetNodeManager()->IsNodeOpen(iNodeId))
	{
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
	// Get the node id
	int iNodeId = (int)params[1];
	// Get the point
	int iPoint = (int)params[2];
	// Make sure the node is valid
	if(!pServer->GetNodeManager()->IsNodeOpen(iNodeId))
		return NODE_TYPE_NONE;

	// Set the node type
	pServer->GetNodeManager()->GetAt(iNodeId)->SetPoint(iPoint);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetNodeInfo(AMX *amx, cell *params)
{
	CHECK_PARAMS(4, "FCNPC_GetNodeInfo");
	// Get the node id
	int iNodeId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetNodeManager()->IsNodeOpen(iNodeId))
	{
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
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Get the node id
	int iNodeId = (int)params[2];
	// Get the node type
	int iType = (int)params[3];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	// Make sure the node is valid
	if(!pServer->GetNodeManager()->IsNodeOpen(iNodeId))
		return 0;

	// Play the node
	return pServer->GetPlayerManager()->GetAt(iNPCId)->PlayNode(iNodeId, iType) ? 1 : 0;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_StopPlayingNode(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_StopPlayingNode");
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnectedEx(iNPCId))
		return 0;

	// Stop playing the node
	pServer->GetPlayerManager()->GetAt(iNPCId)->StopPlayingNode();
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetZGround(AMX *amx, cell *params)
{
	CHECK_PARAMS(3, "FCNPC_GetZGround");
	// Get the X coord
	float fX = amx_ctof(params[1]);
	// Get the Y coord
	float fY = amx_ctof(params[2]);
	// Get the Z ground
	float fZ = pServer->GetZMap()->GetGroundForCoord(CVector(fX, fY, 0.0f));
	// Get the argument pointer and set its value
	cell *pAddress = NULL;
	amx_GetAddr(amx, params[3], &pAddress);
	*pAddress = amx_ftoc(fZ);

	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_InitZMap(AMX *amx, cell *params)
{
	CHECK_PARAMS(1, "FCNPC_InitZMap");
	// Get the filename length
	int iLength = 0;
	cell *pAddress = NULL;
	amx_GetAddr(amx, params[1], &pAddress);
	amx_StrLen(pAddress, &iLength);
	// Make sure the length is valid
	if(!iLength)
		return 0;

	// Make room for the string end
	iLength++;
	// Allocate the filename string
	char *szFilename = new char [iLength];
	// Get the filename
	amx_GetString(szFilename, pAddress, 0, iLength);
	// Set the file path
	pServer->GetZMap()->SetPath(szFilename);
	// Try to initialize the ZMap
	return (cell)pServer->GetZMap()->Initialize();
}
