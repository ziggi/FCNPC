/* ---------------------------------
	 FCNPC Plugin PAWN include
		
- File: FCNPC.inc
- Author: OrMisicL
---------------------------------*/

#if defined _FCNPC_included
  #endinput
#endif
#define _FCNPC_included
#pragma library FCNPC

// Definitions
#define MOVE_TYPE_AUTO      -1
#define MOVE_TYPE_WALK      0
#define MOVE_TYPE_RUN       1
#define MOVE_TYPE_SPRINT    2
#define MOVE_TYPE_DRIVE     3

#define MAX_NODES           64

#define NODE_TYPE_NONE      (-1)
#define NODE_TYPE_PED       0
#define NODE_TYPE_VEHICLE   1
#define NODE_TYPE_BOAT      2

// Callbacks
forward FCNPC_OnCreate(npcid);
forward FCNPC_OnSpawn(npcid);
forward FCNPC_OnRespawn(npcid);
forward FCNPC_OnDeath(npcid, killerid, weaponid);

forward FCNPC_OnVehicleEntryComplete(npcid, vehicleid, seat);
forward FCNPC_OnVehicleExitComplete(npcid);

forward FCNPC_OnReachDestination(npcid);
forward FCNPC_OnFinishPlayback(npcid);

forward FCNPC_OnTakeDamage(npcid, damagerid, weaponid, bodypart, Float:health_loss);

forward FCNPC_OnFinishNodePoint(npcid, point);
forward FCNPC_OnChangeNode(npcid, nodeid);
forward FCNPC_OnFinishNode(npcid);

forward FCNPC_OnStreamIn(npcid, forplayerid);
forward FCNPC_OnStreamOut(npcid, forplayerid);

// Natives
native FCNPC_SetUpdateRate(rate);
native FCNPC_InitMapAndreas(address);

native FCNPC_Create(name[]);
native FCNPC_Destroy(npcid);
native FCNPC_Spawn(npcid, skinid, Float:X, Float:Y, Float:Z);
native FCNPC_Respawn(npcid);
native FCNPC_IsSpawned(npcid);
native FCNPC_Kill(npcid);
native FCNPC_IsDead(npcid);
native FCNPC_IsValid(npcid);
native FCNPC_IsStreamedIn(npcid, forplayerid);

native FCNPC_SetPosition(npcid, Float:X, Float:Y, Float:Z);
native FCNPC_GetPosition(npcid, &Float:X, &Float:Y, &Float:Z);
native FCNPC_SetAngle(npcid, Float:Angle);
native Float:FCNPC_GetAngle(npcid);
native FCNPC_SetQuaternion(npcid, Float:W, Float:X, Float:Y, Float:Z);
native FCNPC_GetQuaternion(npcid, &Float:W, &Float:X, &Float:Y, &Float:Z);
native FCNPC_SetVelocity(npcid, Float:X, Float:Y, Float:Z);
native FCNPC_GetVelocity(npcid, &Float:X, &Float:Y, &Float:Z);
native FCNPC_SetInterior(npcid, interiorid);
native FCNPC_GetInterior(npcid);
native FCNPC_SetVirtualWorld(npcid, worldid);
native FCNPC_GetVirtualWorld(npcid);

native FCNPC_SetHealth(npcid, Float:health);
native Float:FCNPC_GetHealth(npcid);
native FCNPC_SetArmour(npcid, Float:armour);
native Float:FCNPC_GetArmour(npcid);

native FCNPC_SetInvulnerable(npcid, bool:invulnerable = true);
native FCNPC_IsInvulnerable(npcid);

native FCNPC_SetSkin(npcid, skinid);
native FCNPC_GetSkin(npcid);

native FCNPC_SetWeapon(npcid, weaponid);
native FCNPC_GetWeapon(npcid);
native FCNPC_SetAmmo(npcid, ammo);
native FCNPC_GetAmmo(npcid);
native FCNPC_SetWeaponSkillLevel(npcid, skill, level);
native FCNPC_GetWeaponSkillLevel(npcid, skill);

native FCNPC_SetKeys(npcid, Keys);
native FCNPC_GetKeys(npcid, &UDAnalog, &LRAnalog, &Keys);

native FCNPC_SetSpecialAction(npcid, actionid);
native FCNPC_GetSpecialAction(npcid);

native FCNPC_SetAnimation(npcid, animationid, Float:fDelta = 4.1, loop = 0, lockx = 1, locky = 1, freeze = 0, time = 1);
native FCNPC_GetAnimation(npcid, &animationid = 0, &Float:fDelta = 4.1, &loop = 0, &lockx = 1, &locky = 1, &freeze = 0, &time = 1);

native FCNPC_SetFightingStyle(npcid, style);
native FCNPC_GetFightingStyle(npcid);

native FCNPC_ToggleReloading(npcid, bool:toggle);
native FCNPC_ToggleInfiniteAmmo(npcid, bool:toggle);

native FCNPC_GoTo(npcid, Float:X, Float:Y, Float:Z, type = MOVE_TYPE_AUTO, Float:speed = 0.5, bool:UseMapAndreas = false, Float:radius = 0.0, bool:getangle = true);
native FCNPC_GoToPlayer(npcid, playerid, type = MOVE_TYPE_AUTO, Float:speed = 0.5, bool:UseMapAndreas = false, Float:radius = 0.0, bool:getangle = true);
native FCNPC_Stop(npcid);
native FCNPC_IsMoving(npcid);

native FCNPC_AimAt(npcid, Float:X, Float:Y, Float:Z, bool:shoot = false);
native FCNPC_AimAtPlayer(npcid, playerid, bool:shoot = false);
native FCNPC_StopAim(npcid);
native FCNPC_MeleeAttack(npcid, delay = 100, bool:fightstyle = true);
native FCNPC_StopAttack(npcid);
native FCNPC_IsAiming(npcid);
native FCNPC_IsShooting(npcid);
native FCNPC_IsReloading(npcid);

native FCNPC_EnterVehicle(npcid, vehicleid, seatid, type = MOVE_TYPE_WALK);
native FCNPC_ExitVehicle(npcid);

native FCNPC_PutInVehicle(npcid, vehicleid, seatid);
native FCNPC_RemoveFromVehicle(npcid);
native FCNPC_GetVehicleID(npcid);
native FCNPC_GetVehicleSeat(npcid);
native FCNPC_SetVehicleSiren(npcid, bool:status);
native FCNPC_IsVehicleSiren(npcid);
native FCNPC_SetVehicleHealth(npcid, Float:health);
native Float:FCNPC_GetVehicleHealth(npcid);

// not working
//native FCNPC_SetPassengerDriveBy(npcid, bool:status);
//native FCNPC_IsPassengerDriveBy(npcid);

native FCNPC_SetSurfingOffsets(npcid, Float:fX, Float:fY, Float:fZ);
native FCNPC_GetSurfingOffsets(npcid, &Float:fX, &Float:fY, &Float:fZ);
native FCNPC_SetSurfingVehicle(npcid, vehicleid);
native FCNPC_GetSurfingVehicle(npcid);
native FCNPC_SetSurfingObject(npcid, objectid);
native FCNPC_GetSurfingObject(npcid);
native FCNPC_SetSurfingPlayerObject(npcid, objectid);
native FCNPC_GetSurfingPlayerObject(npcid);

native FCNPC_StartPlayingPlayback(npcid, file[]);
native FCNPC_StopPlayingPlayback(npcid);
native FCNPC_PausePlayingPlayback(npcid);
native FCNPC_ResumePlayingPlayback(npcid);

native FCNPC_OpenNode(nodeid);
native FCNPC_CloseNode(nodeid);
native FCNPC_IsNodeOpen(nodeid);
native FCNPC_GetNodeType(nodeid);
native FCNPC_SetNodePoint(nodeid, point);
native FCNPC_GetNodePointPosition(nodeid, &Float:X, &Float:Y, &Float:Z);
native FCNPC_GetNodePointCount(nodeid);
native FCNPC_GetNodeInfo(nodeid, &vehnodes, &pednodes, &navinode);
native FCNPC_PlayNode(npcid, nodeid, type);
native FCNPC_StopPlayingNode(npcid);
