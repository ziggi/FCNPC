# FCNPC - Fully Controllable NPC
[![GitHub version](https://badge.fury.io/gh/Open-GTO%2FFCNPC.svg)](https://badge.fury.io/gh/Open-GTO%2FFCNPC) [![Build Status](https://travis-ci.org/Open-GTO/FCNPC.svg?branch=master)](https://travis-ci.org/Open-GTO/FCNPC)

This is a fork of [original repository](https://github.com/OrMisicL/FCNPC) by [OrMisicL](https://github.com/OrMisicL/).

If you found a bug or get crash, create issue in [issues section](https://github.com/Open-GTO/FCNPC/issues) with your **crashlog** and your **Pawn script**.

Discussion in [forum thread](http://forum.sa-mp.com/showthread.php?t=428066). Or in [Russian forum thread](http://forum.sa-mp.com/showthread.php?t=602965).

# Usage notes
If you want to see that your scripts will work correctly, you need to declare this callback functions at least in one of your scripts:
- OnPlayerGiveDamage
- OnPlayerWeaponShot
- OnPlayerStreamIn
- OnPlayerStreamOut

# Building (Windows)
You can use Visual Studio for build. Just use CMake for generate VS project:
```bash
cd FCNPC
mkdir build
cd build
cmake ../ -G "Visual Studio 12"
```

# Building (Linux)
```bash
cd FCNPC
mkdir build
cd build
cmake ../
make
```

# Definitions
```Pawn
#define MOVE_TYPE_AUTO      (-1)
#define MOVE_TYPE_WALK      (0)
#define MOVE_TYPE_RUN       (1)
#define MOVE_TYPE_SPRINT    (2)
#define MOVE_TYPE_DRIVE     (3)

#define MOVE_SPEED_AUTO     (-1.0)
#define MOVE_SPEED_WALK     (0.1552086)
#define MOVE_SPEED_RUN      (0.56444)
#define MOVE_SPEED_SPRINT   (0.926784)

#define MAX_NODES           (64)

#define NODE_TYPE_NONE      (-1)
#define NODE_TYPE_PED       (0)
#define NODE_TYPE_VEHICLE   (1)
#define NODE_TYPE_BOAT      (2)
```

# Callbacks
```Pawn
forward FCNPC_OnCreate(npcid);
forward FCNPC_OnSpawn(npcid);
forward FCNPC_OnRespawn(npcid);
forward FCNPC_OnDeath(npcid, killerid, weaponid);

forward FCNPC_OnVehicleEntryComplete(npcid, vehicleid, seat);
forward FCNPC_OnVehicleExitComplete(npcid);

forward FCNPC_OnReachDestination(npcid);
forward FCNPC_OnFinishPlayback(npcid);

forward FCNPC_OnTakeDamage(npcid, damagerid, weaponid, bodypart, Float:health_loss);
forward FCNPC_OnVehicleTakeDamage(npcid, damagerid, vehicleid, weaponid, Float:x, Float:y, Float:z);

forward FCNPC_OnFinishNodePoint(npcid, point);
forward FCNPC_OnChangeNode(npcid, nodeid);
forward FCNPC_OnFinishNode(npcid);

forward FCNPC_OnStreamIn(npcid, forplayerid);
forward FCNPC_OnStreamOut(npcid, forplayerid);
```

# Natives
```Pawn
native FCNPC_SetUpdateRate(rate);
native FCNPC_InitMapAndreas(address);

native FCNPC_Create(name[]);
native FCNPC_Destroy(npcid);
native FCNPC_Spawn(npcid, skinid, Float:x, Float:y, Float:z);
native FCNPC_Respawn(npcid);
native FCNPC_IsSpawned(npcid);
native FCNPC_Kill(npcid);
native FCNPC_IsDead(npcid);
native FCNPC_IsValid(npcid);
native FCNPC_IsStreamedIn(npcid, forplayerid);
native FCNPC_IsStreamedForAnyone(npcid);

native FCNPC_SetPosition(npcid, Float:x, Float:y, Float:z);
native FCNPC_GetPosition(npcid, &Float:x, &Float:y, &Float:z);
native FCNPC_SetAngle(npcid, Float:angle);
native FCNPC_SetAngleToPos(npcid, Float:x, Float:y);
native FCNPC_SetAngleToPlayer(npcid, playerid);
native Float:FCNPC_GetAngle(npcid);
native FCNPC_SetQuaternion(npcid, Float:w, Float:x, Float:y, Float:z);
native FCNPC_GetQuaternion(npcid, &Float:w, &Float:x, &Float:y, &Float:z);
native FCNPC_SetVelocity(npcid, Float:x, Float:y, Float:z);
native FCNPC_GetVelocity(npcid, &Float:x, &Float:y, &Float:z);
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
native FCNPC_SetWeaponState(npcid, weaponstate);
native FCNPC_GetWeaponState(npcid);

native FCNPC_SetWeaponReloadTime(npcid, weaponid, time);
native FCNPC_GetWeaponReloadTime(npcid, weaponid);
native FCNPC_SetWeaponShootTime(npcid, weaponid, time);
native FCNPC_GetWeaponShootTime(npcid, weaponid);
native FCNPC_SetWeaponClipSize(npcid, weaponid, size);
native FCNPC_GetWeaponClipSize(npcid, weaponid);
native FCNPC_SetWeaponInfo(npcid, weaponid, reload_time = -1, shoot_time = -1, clip_size = -1);
native FCNPC_GetWeaponInfo(npcid, weaponid, &reload_time = -1, &shoot_time = -1, &clip_size = -1);
native FCNPC_SetWeaponDefaultInfo(weaponid, reload_time = -1, shoot_time = -1, clip_size = -1);
native FCNPC_GetWeaponDefaultInfo(weaponid, &reload_time = -1, &shoot_time = -1, &clip_size = -1);

native FCNPC_SetKeys(npcid, ud_analog, lr_analog, keys);
native FCNPC_GetKeys(npcid, &ud_analog, &lr_analog, &keys);

native FCNPC_SetSpecialAction(npcid, actionid);
native FCNPC_GetSpecialAction(npcid);

native FCNPC_SetAnimation(npcid, animationid, Float:fDelta = 4.1, loop = 0, lockx = 1, locky = 1, freeze = 0, time = 1);
native FCNPC_SetAnimationByName(npcid, name[], Float:fDelta = 4.1, loop = 0, lockx = 1, locky = 1, freeze = 0, time = 1);
native FCNPC_ResetAnimation(npcid);
native FCNPC_GetAnimation(npcid, &animationid = 0, &Float:fDelta = 4.1, &loop = 0, &lockx = 1, &locky = 1, &freeze = 0, &time = 1);
native FCNPC_ApplyAnimation(npcid, animlib[], animname[], Float:fDelta = 4.1, loop = 0, lockx = 1, locky = 1, freeze = 0, time = 1);
native FCNPC_ClearAnimations(npcid);

native FCNPC_SetFightingStyle(npcid, style);
native FCNPC_GetFightingStyle(npcid);

native FCNPC_ToggleReloading(npcid, bool:toggle);
native FCNPC_ToggleInfiniteAmmo(npcid, bool:toggle);

native FCNPC_GoTo(npcid, Float:x, Float:y, Float:z, type = MOVE_TYPE_AUTO, Float:speed = MOVE_SPEED_AUTO, bool:UseMapAndreas = false, Float:radius = 0.0, bool:setangle = true);
native FCNPC_GoToPlayer(npcid, playerid, type = MOVE_TYPE_AUTO, Float:speed = MOVE_SPEED_AUTO, bool:UseMapAndreas = false, Float:radius = 0.0, bool:setangle = true);
native FCNPC_Stop(npcid);
native FCNPC_IsMoving(npcid);
native FCNPC_IsMovingAtPlayer(npcid, playerid);

native FCNPC_AimAt(npcid, Float:x, Float:y, Float:z, bool:shoot = false, shoot_delay = -1, bool:setangle = true);
native FCNPC_AimAtPlayer(npcid, playerid, bool:shoot = false, shoot_delay = -1, bool:setangle = true);
native FCNPC_StopAim(npcid);
native FCNPC_MeleeAttack(npcid, delay = -1, bool:fightstyle = false);
native FCNPC_StopAttack(npcid);
native FCNPC_IsAttacking(npcid);
native FCNPC_IsAiming(npcid);
native FCNPC_IsAimingAtPlayer(npcid, playerid);
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
native FCNPC_SetVehicleHydraThrusters(npcid, direction);
native FCNPC_GetVehicleHydraThrusters(npcid);
native FCNPC_SetVehicleGearState(npcid, gear_state);
native FCNPC_GetVehicleGearState(npcid);

native FCNPC_SetSurfingOffsets(npcid, Float:x, Float:y, Float:z);
native FCNPC_GetSurfingOffsets(npcid, &Float:x, &Float:y, &Float:z);
native FCNPC_SetSurfingVehicle(npcid, vehicleid);
native FCNPC_GetSurfingVehicle(npcid);
native FCNPC_SetSurfingObject(npcid, objectid);
native FCNPC_GetSurfingObject(npcid);
native FCNPC_SetSurfingPlayerObject(npcid, objectid);
native FCNPC_GetSurfingPlayerObject(npcid);
native FCNPC_StopSurfing(npcid);

native FCNPC_StartPlayingPlayback(npcid, file[]);
native FCNPC_StopPlayingPlayback(npcid);
native FCNPC_PausePlayingPlayback(npcid);
native FCNPC_ResumePlayingPlayback(npcid);

native FCNPC_OpenNode(nodeid);
native FCNPC_CloseNode(nodeid);
native FCNPC_IsNodeOpen(nodeid);
native FCNPC_GetNodeType(nodeid);
native FCNPC_SetNodePoint(nodeid, point);
native FCNPC_GetNodePointPosition(nodeid, &Float:x, &Float:y, &Float:z);
native FCNPC_GetNodePointCount(nodeid);
native FCNPC_GetNodeInfo(nodeid, &vehnodes, &pednodes, &navinode);
native FCNPC_PlayNode(npcid, nodeid, type);
native FCNPC_StopPlayingNode(npcid);
```
