/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: Natives.cpp
	- Author(s): OrMisicL

  =========================================*/

#include "Main.h"

extern CServer					*pServer;

cell AMX_NATIVE_CALL CNatives::FCNPC_Create(AMX *amx, cell *params)
{
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
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnected(iNPCId))
		return 0;

	// Destoy the player
	return pServer->GetPlayerManager()->DeletePlayer(iNPCId);
}

cell AMX_NATIVE_CALL CNatives::FCNPC_Spawn(AMX *amx, cell *params)
{
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Get the NPC skin id
	int iSkin = (int)params[2];
	// Get the position coordinates
	float fX = amx_ctof(params[3]);
	float fY = amx_ctof(params[4]);
	float fZ = amx_ctof(params[5]);
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnected(iNPCId))
		return 0;

	// Set the spawn position
	pServer->GetPlayerManager()->GetAt(iNPCId)->SetSpawnPosition(CVector3(fX, fY, fZ));
	// Spawn the player
	return pServer->GetPlayerManager()->GetAt(iNPCId)->Spawn(iSkin) ? 1 : 0;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_Respawn(AMX *amx, cell *params)
{
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnected(iNPCId))
		return 0;

	// Respawn the player
	return pServer->GetPlayerManager()->GetAt(iNPCId)->Respawn() ? 1 : 0;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_IsSpawned(AMX *amx, cell *params)
{
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnected(iNPCId))
		return 0;

	// Respawn the player
	return pServer->GetPlayerManager()->GetAt(iNPCId)->IsSpawned() ? 1 : 0;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_Kill(AMX *amx, cell *params)
{
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnected(iNPCId))
		return 0;

	// Kill the player
	pServer->GetPlayerManager()->GetAt(iNPCId)->Kill(INVALID_ENTITY_ID, 0);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_IsDead(AMX *amx, cell *params)
{
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnected(iNPCId))
		return 0;

	// Return the player dead state
	return pServer->GetPlayerManager()->GetAt(iNPCId)->GetState() == PLAYER_STATE_DEAD ? 1 : 0;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_SetPosition(AMX *amx, cell *params)
{
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Get the position coordinates
	float fX = amx_ctof(params[2]);
	float fY = amx_ctof(params[3]);
	float fZ = amx_ctof(params[4]);
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnected(iNPCId))
		return 0;

	// Set the player position
	pServer->GetPlayerManager()->GetAt(iNPCId)->SetPosition(CVector3(fX, fY, fZ));
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetPosition(AMX *amx, cell *params)
{
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnected(iNPCId))
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
	CVector3 vecPosition;
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
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Get the angle
	float fAngle = amx_ctof(params[2]);
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnected(iNPCId))
		return 0;

	// Set the player angle
	pServer->GetPlayerManager()->GetAt(iNPCId)->SetAngle(fAngle);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetAngle(AMX *amx, cell *params)
{
	// Get the NPC id
	int iNPCId = (int)params[1];
	float Angle = 0.0f;
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnected(iNPCId))
		return amx_ftoc(Angle);

	// Get the player angle
	Angle = pServer->GetPlayerManager()->GetAt(iNPCId)->GetAngle();
	return amx_ftoc(Angle);
}

cell AMX_NATIVE_CALL CNatives::FCNPC_SetInterior(AMX *amx, cell *params)
{
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Get the interior
	int iInterior = (int)params[2];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnected(iNPCId))
		return 0;

	// Set the player angle
	pServer->GetPlayerManager()->GetAt(iNPCId)->SetInterior(iInterior);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetInterior(AMX *amx, cell *params)
{
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnected(iNPCId))
		return 0;

	// Get the player interior
	return pServer->GetPlayerManager()->GetAt(iNPCId)->GetInterior();
}

cell AMX_NATIVE_CALL CNatives::FCNPC_SetQuaternion(AMX *amx, cell *params)
{
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Get the quaternion vector
	float fX = amx_ctof(params[2]);
	float fY = amx_ctof(params[3]);
	float fZ = amx_ctof(params[4]);
	// Get the quaternion angle
	float fA = amx_ctof(params[5]);
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnected(iNPCId))
		return 0;

	// Set the player quaternion
	pServer->GetPlayerManager()->GetAt(iNPCId)->SetQuaternion(CVector3(fX, fY, fZ), fA);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetQuaternion(AMX *amx, cell *params)
{
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnected(iNPCId))
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
	CVector3 vecVelocity;
	float fAngle;
	pServer->GetPlayerManager()->GetAt(iNPCId)->GetQuaternion(&vecVelocity, &fAngle);
	// Get the argument pointers and set its value
	cell *pAddress = NULL;
	amx_GetAddr(amx, params[2], &pAddress);
	*pAddress = amx_ftoc(vecVelocity.fX);

	amx_GetAddr(amx, params[3], &pAddress);
	*pAddress = amx_ftoc(vecVelocity.fY);

	amx_GetAddr(amx, params[4], &pAddress);
	*pAddress = amx_ftoc(vecVelocity.fZ);

	amx_GetAddr(amx, params[5], &pAddress);
	*pAddress = amx_ftoc(fAngle);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_SetVelocity(AMX *amx, cell *params)
{
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Get the velocity vector
	float fX = amx_ctof(params[2]);
	float fY = amx_ctof(params[3]);
	float fZ = amx_ctof(params[4]);
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnected(iNPCId))
		return 0;

	// Set the player velocity
	pServer->GetPlayerManager()->GetAt(iNPCId)->SetVelocity(CVector3(fX, fY, fZ));
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetVelocity(AMX *amx, cell *params)
{
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnected(iNPCId))
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
	CVector3 vecVelocity;
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
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Get the health
	float fHealth = amx_ctof(params[2]);
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnected(iNPCId))
		return 0;

	// Set the player health
	pServer->GetPlayerManager()->GetAt(iNPCId)->SetHealth(fHealth);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetHealth(AMX *amx, cell *params)
{
	// Get the NPC id
	int iNPCId = (int)params[1];
	float fHealth = 0.0f;
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnected(iNPCId))
		return amx_ftoc(fHealth);;

	// Get the player health
	fHealth = pServer->GetPlayerManager()->GetAt(iNPCId)->GetHealth();
	return amx_ftoc(fHealth);
}

cell AMX_NATIVE_CALL CNatives::FCNPC_SetArmour(AMX *amx, cell *params)
{
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Get the armour
	float fArmour = amx_ctof(params[2]);
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnected(iNPCId))
		return 0;

	// Set the player armour
	pServer->GetPlayerManager()->GetAt(iNPCId)->SetArmour(fArmour);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetArmour(AMX *amx, cell *params)
{
	// Get the NPC id
	int iNPCId = (int)params[1];
	float fArmour = 0.0f;
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnected(iNPCId))
		return amx_ftoc(fArmour);;

	// Get the player armour
	fArmour = pServer->GetPlayerManager()->GetAt(iNPCId)->GetArmour();
	return amx_ftoc(fArmour);
}

cell AMX_NATIVE_CALL CNatives::FCNPC_SetSkin(AMX *amx, cell *params)
{
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Get the skin
	int iSkin = (int)params[2];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnected(iNPCId))
		return 0;

	// Set the player skin
	pServer->GetPlayerManager()->GetAt(iNPCId)->SetSkin(iSkin);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetSkin(AMX *amx, cell *params)
{
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnected(iNPCId))
		return 0;

	// Get the player skin
	return pServer->GetPlayerManager()->GetAt(iNPCId)->GetSkin();
}

cell AMX_NATIVE_CALL CNatives::FCNPC_SetKeys(AMX *amx, cell *params)
{
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Get the keys
	DWORD dwKeys = (DWORD)params[2];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnected(iNPCId))
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
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnected(iNPCId))
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
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Get the action id
	int iActionId = (int)params[2];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnected(iNPCId))
		return 0;

	// Set the player special action
	pServer->GetPlayerManager()->GetAt(iNPCId)->SetSpecialAction(iActionId);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetSpecialAction(AMX *amx, cell *params)
{
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnected(iNPCId))
		return 0;

	// Get the player special action
	return pServer->GetPlayerManager()->GetAt(iNPCId)->GetSpecialAction();
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GoTo(AMX *amx, cell *params)
{
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
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnected(iNPCId))
		return 0;

	// Set the player velocity
	pServer->GetPlayerManager()->GetAt(iNPCId)->SetVelocity(CVector3(fSpeed, fSpeed, fSpeed));
	// Move the player
	pServer->GetPlayerManager()->GetAt(iNPCId)->GoTo(CVector3(fX, fY, fZ), iType, !iZMap ? false : true);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_Stop(AMX *amx, cell *params)
{
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnected(iNPCId))
		return 0;

	// Stop the player
	pServer->GetPlayerManager()->GetAt(iNPCId)->StopMoving();
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_IsMoving(AMX *amx, cell *params)
{
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnected(iNPCId))
		return 0;

	// Get the player moving state
	return pServer->GetPlayerManager()->GetAt(iNPCId)->IsMoving() ? 1 : 0;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_SetWeapon(AMX *amx, cell *params)
{
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Get the weapon id
	int iWeapon = (int)params[2];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnected(iNPCId))
		return 0;

	// Set the player weapon
	pServer->GetPlayerManager()->GetAt(iNPCId)->SetWeapon(iWeapon);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetWeapon(AMX *amx, cell *params)
{
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnected(iNPCId))
		return 0;

	// Get the player weapon
	return pServer->GetPlayerManager()->GetAt(iNPCId)->GetWeapon();
}

cell AMX_NATIVE_CALL CNatives::FCNPC_SetAmmo(AMX *amx, cell *params)
{
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Get the ammo
	int iAmmo = (int)params[2];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnected(iNPCId))
		return 0;

	// Set the player ammo
	pServer->GetPlayerManager()->GetAt(iNPCId)->SetAmmo(iAmmo);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetAmmo(AMX *amx, cell *params)
{
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnected(iNPCId))
		return 0;

	// Get the player ammo
	return pServer->GetPlayerManager()->GetAt(iNPCId)->GetAmmo();
}

cell AMX_NATIVE_CALL CNatives::FCNPC_AimAt(AMX *amx, cell *params)
{
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Get the aim vector
	float fX = amx_ctof(params[2]);
	float fY = amx_ctof(params[3]);
	float fZ = amx_ctof(params[4]);
	// Get the shooting flag
	int iShoot = (int)params[5];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnected(iNPCId))
		return 0;

	// Set the player aiming
	pServer->GetPlayerManager()->GetAt(iNPCId)->AimAt(CVector3(fX, fY, fZ), !iShoot ? false : true);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_MeleeAttack(AMX *amx, cell *params)
{
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Get the attack time
	int iTime = (int)params[2];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnected(iNPCId))
		return 0;

	// Set the player melee attack
	pServer->GetPlayerManager()->GetAt(iNPCId)->MeleeAttack(iTime);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_StopAttack(AMX *amx, cell *params)
{
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Get the attack time
	int iTime = (int)params[2];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnected(iNPCId))
		return 0;

	// Stop the player attack
	pServer->GetPlayerManager()->GetAt(iNPCId)->StopAttack();
	return 1;
}


cell AMX_NATIVE_CALL CNatives::FCNPC_StopAim(AMX *amx, cell *params)
{
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnected(iNPCId))
		return 0;

	// Stop the player aim
	pServer->GetPlayerManager()->GetAt(iNPCId)->StopAim();
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_IsAiming(AMX *amx, cell *params)
{
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnected(iNPCId))
		return 0;

	// Get the player aiming state
	return pServer->GetPlayerManager()->GetAt(iNPCId)->IsAiming() ? 1 : 0;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_IsShooting(AMX *amx, cell *params)
{
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnected(iNPCId))
		return 0;

	// Get the player shooting state
	return pServer->GetPlayerManager()->GetAt(iNPCId)->IsShooting() ? 1 : 0;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_IsReloading(AMX *amx, cell *params)
{
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnected(iNPCId))
		return 0;

	// Get the player shooting state
	return pServer->GetPlayerManager()->GetAt(iNPCId)->IsReloading() ? 1 : 0;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_EnterVehicle(AMX *amx, cell *params)
{
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Get the vehicle id
	int iVehicleId = (int)params[2];
	// Get the seat id
	int iSeatId = (int)params[3];
	// Get the move type
	int iType = (int)params[4];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnected(iNPCId))
		return 0;

	// Make the player enter the vehicle
	return pServer->GetPlayerManager()->GetAt(iNPCId)->EnterVehicle(iVehicleId, iSeatId, iType) ? 1 : 0;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_ExitVehicle(AMX *amx, cell *params)
{
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Get the vehicle id
	int iVehicleId = (int)params[2];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnected(iNPCId))
		return 0;

	// Make the player exit the vehicle
	return pServer->GetPlayerManager()->GetAt(iNPCId)->ExitVehicle() ? 1 : 0;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_PutInVehicle(AMX *amx, cell *params)
{
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Get the vehicle id
	int iVehicleId = (int)params[2];
	// Get the seat id
	int iSeatId = (int)params[3];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnected(iNPCId))
		return 0;

	// Make the player enter the vehicle
	return pServer->GetPlayerManager()->GetAt(iNPCId)->PutInVehicle(iVehicleId, iSeatId) ? 1 : 0;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_RemoveFromVehicle(AMX *amx, cell *params)
{
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnected(iNPCId))
		return 0;

	// Make the player exit the vehicle
	return pServer->GetPlayerManager()->GetAt(iNPCId)->RemoveFromVehicle() ? 1 : 0;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetVehicleID(AMX *amx, cell *params)
{
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnected(iNPCId))
		return INVALID_ENTITY_ID;

	// Make the player exit the vehicle
	return pServer->GetPlayerManager()->GetAt(iNPCId)->GetVehicleId();
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetVehicleSeat(AMX *amx, cell *params)
{
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnected(iNPCId))
		return 0;

	// Make the player exit the vehicle
	return pServer->GetPlayerManager()->GetAt(iNPCId)->GetVehicleId() == INVALID_ENTITY_ID ? 128 : pServer->GetPlayerManager()->GetAt(iNPCId)->GetSeatId();
}

cell AMX_NATIVE_CALL CNatives::FCNPC_StartRecordingPlayback(AMX *amx, cell *params)
{
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
	if(!pServer->GetPlayerManager()->IsPlayerConnected(iNPCId))
		return 0;

	// Start recording the player playback
	return pServer->GetPlayerManager()->GetAt(iNPCId)->StartRecordingPlayback(szFile) ? 1 : 0;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_StopRecordingPlayback(AMX *amx, cell *params)
{
	// Get the npc id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnected(iNPCId))
		return 0;

	// Stop recording the player playback
	pServer->GetPlayerManager()->GetAt(iNPCId)->StopRecordingPlayback();
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_PauseRecordingPlayback(AMX *amx, cell *params)
{
	// Get the npc id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnected(iNPCId))
		return 0;

	// Pause recording the player playback
	pServer->GetPlayerManager()->GetAt(iNPCId)->PauseRecordingPlayback();
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_ResumeRecordingPlayback(AMX *amx, cell *params)
{
	// Get the npc id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnected(iNPCId))
		return 0;

	// Resume recording the player playback
	pServer->GetPlayerManager()->GetAt(iNPCId)->ResumeRecordingPlayback();
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_SetUpdateRate(AMX *amx, cell *params)
{
	// Get the update rate
	int iRate = (int)params[1];
	// Set the update rate
	pServer->SetUpdateRate(iRate);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_OpenNode(AMX *amx, cell *params)
{
	// Get the node id
	int iNodeId = (int)params[1];
	// Open the node
	return pServer->GetNodeManager()->OpenNode(iNodeId) ? 1 : 0;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_CloseNode(AMX *amx, cell *params)
{
	// Get the node id
	int iNodeId = (int)params[1];
	// Close the node
	pServer->GetNodeManager()->CloseNode(iNodeId);
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_IsNodeOpen(AMX *amx, cell *params)
{
	// Get the node id
	int iNodeId = (int)params[1];
	// Open the node
	return pServer->GetNodeManager()->IsNodeOpen(iNodeId) ? 1 : 0;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetNodeType(AMX *amx, cell *params)
{
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
	CVector3 vecPosition;
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
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Get the node id
	int iNodeId = (int)params[2];
	// Get the node type
	int iType = (int)params[3];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnected(iNPCId))
		return 0;

	// Make sure the node is valid
	if(!pServer->GetNodeManager()->IsNodeOpen(iNodeId))
		return 0;

	// Play the node
	return pServer->GetPlayerManager()->GetAt(iNPCId)->PlayNode(iNodeId, iType) ? 1 : 0;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_StopPlayingNode(AMX *amx, cell *params)
{
	// Get the NPC id
	int iNPCId = (int)params[1];
	// Make sure the player is valid
	if(!pServer->GetPlayerManager()->IsPlayerConnected(iNPCId))
		return 0;

	// Stop playing the node
	pServer->GetPlayerManager()->GetAt(iNPCId)->StopPlayingNode();
	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_GetZGround(AMX *amx, cell *params)
{
	// Get the X coord
	float fX = amx_ctof(params[1]);
	// Get the Y coord
	float fY = amx_ctof(params[2]);
	// Get the Z ground
	float fZ = pServer->GetZMap()->GetGroundForCoord(CVector3(fX, fY, 0.0f));
	// Get the argument pointer and set its value
	cell *pAddress = NULL;
	amx_GetAddr(amx, params[3], &pAddress);
	*pAddress = amx_ftoc(fZ);

	return 1;
}

cell AMX_NATIVE_CALL CNatives::FCNPC_InitZMap(AMX *amx, cell *params)
{
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

cell AMX_NATIVE_CALL CNatives::FCNPC_ProcessDamage(AMX *amx, cell *params)
{
	// Get the parameters
	int iPlayerId = (int)params[1];
	int iDamagerId = (int)params[2];
	float fHealthLoss = amx_ctof(params[3]);
	int iWeapon = (int)params[4];
	int iBodypart = (int)params[5];
	// Validate the player id
	if(!pServer->GetPlayerManager()->IsPlayerConnected(iDamagerId))
		return 0;

	// Validate the damager id
	CSAMPServer *pSAMPServer = (CSAMPServer *)CAddress::VAR_ServerPtr;
	if(!pSAMPServer->pPlayerPool->bIsPlayerConnected[iPlayerId])
		return 0;

	// Process damage
	pServer->GetPlayerManager()->GetAt(iDamagerId)->ProcessDamage(iPlayerId, fHealthLoss, iWeapon, iBodypart);
	return 1;
}