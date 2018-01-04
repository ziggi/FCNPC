/* ---------------------------------
	 FCNPC Plugin PAWN include

- File: FCNPC.inc
- Author: OrMisicL
- Contributor: ziggi
---------------------------------*/

#if defined _FCNPC_included
	#endinput
#endif
#define _FCNPC_included
#pragma library FCNPC

// Version
#define FCNPC_INCLUDE_VERSION 180

public FCNPC_IncludeVersion = FCNPC_INCLUDE_VERSION;
#pragma unused FCNPC_IncludeVersion

// Definitions
#define FCNPC_MOVE_TYPE_AUTO      (-1)
#define FCNPC_MOVE_TYPE_WALK      (0)
#define FCNPC_MOVE_TYPE_RUN       (1)
#define FCNPC_MOVE_TYPE_SPRINT    (2)
#define FCNPC_MOVE_TYPE_DRIVE     (3)

#define FCNPC_MOVE_SPEED_AUTO     (-1.0)
#define FCNPC_MOVE_SPEED_WALK     (0.1552086)
#define FCNPC_MOVE_SPEED_RUN      (0.56444)
#define FCNPC_MOVE_SPEED_SPRINT   (0.926784)

#define FCNPC_MAX_NODES           (64)

#define FCNPC_INVALID_MOVEPATH_ID (-1)
#define FCNPC_INVALID_RECORD_ID   (-1)

// Compatibility with older versions
#if !defined MOVE_TYPE_AUTO
	#define MOVE_TYPE_AUTO FCNPC_MOVE_TYPE_AUTO
#endif

#if !defined MOVE_TYPE_WALK
	#define MOVE_TYPE_WALK FCNPC_MOVE_TYPE_WALK
#endif

#if !defined MOVE_TYPE_RUN
	#define MOVE_TYPE_RUN FCNPC_MOVE_TYPE_RUN
#endif

#if !defined MOVE_TYPE_SPRINT
	#define MOVE_TYPE_SPRINT FCNPC_MOVE_TYPE_SPRINT
#endif

#if !defined MOVE_TYPE_DRIVE
	#define MOVE_TYPE_DRIVE FCNPC_MOVE_TYPE_DRIVE
#endif


#if !defined MOVE_SPEED_AUTO
	#define MOVE_SPEED_AUTO FCNPC_MOVE_SPEED_AUTO
#endif

#if !defined MOVE_SPEED_WALK
	#define MOVE_SPEED_WALK FCNPC_MOVE_SPEED_WALK
#endif

#if !defined MOVE_SPEED_RUN
	#define MOVE_SPEED_RUN FCNPC_MOVE_SPEED_RUN
#endif

#if !defined MOVE_SPEED_SPRINT
	#define MOVE_SPEED_SPRINT FCNPC_MOVE_SPEED_SPRINT
#endif


#if !defined MAX_NODES
	#define MAX_NODES FCNPC_MAX_NODES
#endif


#if !defined INVALID_MOVEPATH_ID
	#define INVALID_MOVEPATH_ID FCNPC_INVALID_MOVEPATH_ID
#endif

#if !defined INVALID_RECORD_ID
	#define INVALID_RECORD_ID FCNPC_INVALID_RECORD_ID
#endif

// Callbacks
forward FCNPC_OnCreate(npcid);
forward FCNPC_OnDestroy(npcid);
forward FCNPC_OnSpawn(npcid);
forward FCNPC_OnRespawn(npcid);
forward FCNPC_OnDeath(npcid, killerid, weaponid);

forward FCNPC_OnVehicleEntryComplete(npcid, vehicleid, seat);
forward FCNPC_OnVehicleExitComplete(npcid);

forward FCNPC_OnReachDestination(npcid);
forward FCNPC_OnFinishPlayback(npcid);

forward FCNPC_OnTakeDamage(npcid, damagerid, weaponid, bodypart, Float:health_loss);
forward FCNPC_OnGiveDamage(npcid, damagedid, weaponid, bodypart, Float:health_loss);
forward FCNPC_OnVehicleTakeDamage(npcid, damagerid, vehicleid, weaponid, Float:x, Float:y, Float:z);
forward FCNPC_OnWeaponShot(npcid, weaponid, hittype, hitid, Float:x, Float:y, Float:z);
forward FCNPC_OnWeaponStateChange(npcid, weapon_state);

forward FCNPC_OnFinishNodePoint(npcid, point);
forward FCNPC_OnChangeNode(npcid, nodeid);
forward FCNPC_OnFinishNode(npcid);

forward FCNPC_OnStreamIn(npcid, forplayerid);
forward FCNPC_OnStreamOut(npcid, forplayerid);

forward FCNPC_OnUpdate(npcid);

forward FCNPC_OnFinishMovePath(npcid, pathid);
forward FCNPC_OnFinishMovePathPoint(npcid, pathid, pointid);

forward FCNPC_OnChangeHeightPos(npcid, Float:new_z, Float:old_z); // disabled by default, see FCNPC_SetMinHeightPosCall

// Natives
native FCNPC_GetPluginVersion(version[], const size = sizeof(version));
native FCNPC_SetUpdateRate(rate);
native FCNPC_GetUpdateRate();
native FCNPC_SetTickRate(rate);
native FCNPC_GetTickRate();
native FCNPC_InitMapAndreas(address);
native FCNPC_ToggleCrashLogCreation(bool:toggle);
native FCNPC_GetCrashLogCreation();

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
native FCNPC_GetValidArray(npcs[], const size = sizeof(npcs));

native FCNPC_SetPosition(npcid, Float:x, Float:y, Float:z);
native FCNPC_GivePosition(npcid, Float:x, Float:y, Float:z);
native FCNPC_GetPosition(npcid, &Float:x, &Float:y, &Float:z);
native FCNPC_SetAngle(npcid, Float:angle);
native Float:FCNPC_GiveAngle(npcid, Float:angle);
native FCNPC_SetAngleToPos(npcid, Float:x, Float:y);
native FCNPC_SetAngleToPlayer(npcid, playerid);
native Float:FCNPC_GetAngle(npcid);
native FCNPC_SetQuaternion(npcid, Float:w, Float:x, Float:y, Float:z);
native FCNPC_GiveQuaternion(npcid, Float:w, Float:x, Float:y, Float:z);
native FCNPC_GetQuaternion(npcid, &Float:w, &Float:x, &Float:y, &Float:z);
native FCNPC_SetVelocity(npcid, Float:x, Float:y, Float:z, bool:update_pos = false);
native FCNPC_GiveVelocity(npcid, Float:x, Float:y, Float:z, bool:update_pos = false);
native FCNPC_GetVelocity(npcid, &Float:x, &Float:y, &Float:z);
native FCNPC_SetSpeed(npcid, Float:speed);
native Float:FCNPC_GetSpeed(npcid);
native FCNPC_SetInterior(npcid, interiorid);
native FCNPC_GetInterior(npcid);
native FCNPC_SetVirtualWorld(npcid, worldid);
native FCNPC_GetVirtualWorld(npcid);

native FCNPC_SetHealth(npcid, Float:health);
native Float:FCNPC_GiveHealth(npcid, Float:health);
native Float:FCNPC_GetHealth(npcid);
native FCNPC_SetArmour(npcid, Float:armour);
native Float:FCNPC_GiveArmour(npcid, Float:armour);
native Float:FCNPC_GetArmour(npcid);

native FCNPC_SetInvulnerable(npcid, bool:invulnerable = true);
native FCNPC_IsInvulnerable(npcid);

native FCNPC_SetSkin(npcid, skinid);
native FCNPC_GetSkin(npcid);

native FCNPC_SetWeapon(npcid, weaponid);
native FCNPC_GetWeapon(npcid);
native FCNPC_SetAmmo(npcid, ammo);
native FCNPC_GiveAmmo(npcid, ammo);
native FCNPC_GetAmmo(npcid);
native FCNPC_SetAmmoInClip(npcid, ammo);
native FCNPC_GiveAmmoInClip(npcid, ammo);
native FCNPC_GetAmmoInClip(npcid);
native FCNPC_SetWeaponSkillLevel(npcid, skill, level);
native FCNPC_GiveWeaponSkillLevel(npcid, skill, level);
native FCNPC_GetWeaponSkillLevel(npcid, skill);
native FCNPC_SetWeaponState(npcid, weaponstate);
native FCNPC_GetWeaponState(npcid);

native FCNPC_SetWeaponReloadTime(npcid, weaponid, time);
native FCNPC_GetWeaponReloadTime(npcid, weaponid);
native FCNPC_GetWeaponActualReloadTime(npcid, weaponid);
native FCNPC_SetWeaponShootTime(npcid, weaponid, time);
native FCNPC_GetWeaponShootTime(npcid, weaponid);
native FCNPC_SetWeaponClipSize(npcid, weaponid, size);
native FCNPC_GetWeaponClipSize(npcid, weaponid);
native FCNPC_GetWeaponActualClipSize(npcid, weaponid);
native FCNPC_SetWeaponAccuracy(npcid, weaponid, Float:accuracy);
native Float:FCNPC_GetWeaponAccuracy(npcid, weaponid);
native FCNPC_SetWeaponInfo(npcid, weaponid, reload_time = -1, shoot_time = -1, clip_size = -1, Float:accuracy = 1.0);
native FCNPC_GetWeaponInfo(npcid, weaponid, &reload_time = -1, &shoot_time = -1, &clip_size = -1, &Float:accuracy = 1.0);
native FCNPC_SetWeaponDefaultInfo(weaponid, reload_time = -1, shoot_time = -1, clip_size = -1, Float:accuracy = 1.0);
native FCNPC_GetWeaponDefaultInfo(weaponid, &reload_time = -1, &shoot_time = -1, &clip_size = -1, &Float:accuracy = 1.0);

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

native FCNPC_GoTo(npcid, Float:x, Float:y, Float:z, type = FCNPC_MOVE_TYPE_AUTO, Float:speed = FCNPC_MOVE_SPEED_AUTO, bool:UseMapAndreas = false, Float:radius = 0.0, bool:setangle = true, Float:dist_offset = 0.0, stopdelay = 250);
native FCNPC_GoToPlayer(npcid, playerid, type = FCNPC_MOVE_TYPE_AUTO, Float:speed = FCNPC_MOVE_SPEED_AUTO, bool:UseMapAndreas = false, Float:radius = 0.0, bool:setangle = true, Float:dist_offset = 0.0, Float:dist_check = 1.5, stopdelay = 250);
native FCNPC_Stop(npcid);
native FCNPC_IsMoving(npcid);
native FCNPC_IsMovingAtPlayer(npcid, playerid);
native FCNPC_GetDestinationPoint(npcid, &Float:x, &Float:y, &Float:z);

native FCNPC_AimAt(npcid, Float:x, Float:y, Float:z, bool:shoot = false, shoot_delay = -1, bool:setangle = true, Float:offset_from_x = 0.0, Float:offset_from_y = 0.0, Float:offset_from_z = 0.0);
native FCNPC_AimAtPlayer(npcid, playerid, bool:shoot = false, shoot_delay = -1, bool:setangle = true, Float:offset_x = 0.0, Float:offset_y = 0.0, Float:offset_z = 0.0, Float:offset_from_x = 0.0, Float:offset_from_y = 0.0, Float:offset_from_z = 0.0);
native FCNPC_StopAim(npcid);
native FCNPC_MeleeAttack(npcid, delay = -1, bool:fightstyle = false);
native FCNPC_StopAttack(npcid);
native FCNPC_IsAttacking(npcid);
native FCNPC_IsAiming(npcid);
native FCNPC_IsAimingAtPlayer(npcid, playerid);
native FCNPC_GetAimingPlayer(npcid);
native FCNPC_IsShooting(npcid);
native FCNPC_IsReloading(npcid);
native FCNPC_TriggerWeaponShot(npcid, weaponid, hittype, hitid, Float:x, Float:y, Float:z, bool:ishit = true, Float:offset_from_x = 0.0, Float:offset_from_y = 0.0, Float:offset_from_z = 0.0);

native FCNPC_EnterVehicle(npcid, vehicleid, seatid, type = FCNPC_MOVE_TYPE_WALK);
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
native FCNPC_GiveSurfingOffsets(npcid, Float:x, Float:y, Float:z);
native FCNPC_GetSurfingOffsets(npcid, &Float:x, &Float:y, &Float:z);
native FCNPC_SetSurfingVehicle(npcid, vehicleid);
native FCNPC_GetSurfingVehicle(npcid);
native FCNPC_SetSurfingObject(npcid, objectid);
native FCNPC_GetSurfingObject(npcid);
native FCNPC_SetSurfingPlayerObject(npcid, objectid);
native FCNPC_GetSurfingPlayerObject(npcid);
native FCNPC_StopSurfing(npcid);

native FCNPC_StartPlayingPlayback(npcid, file[] = "", recordid = FCNPC_INVALID_RECORD_ID, bool:auto_unload = false);
native FCNPC_StopPlayingPlayback(npcid);
native FCNPC_PausePlayingPlayback(npcid);
native FCNPC_ResumePlayingPlayback(npcid);
native FCNPC_LoadPlayingPlayback(file[]);
native FCNPC_UnloadPlayingPlayback(recordid);
native FCNPC_SetPlayingPlaybackPath(npcid, path[]);
native FCNPC_GetPlayingPlaybackPath(npcid, path[], const size = sizeof(path));

native FCNPC_OpenNode(nodeid);
native FCNPC_CloseNode(nodeid);
native FCNPC_IsNodeOpen(nodeid);
native FCNPC_GetNodeType(nodeid);
native FCNPC_SetNodePoint(nodeid, point);
native FCNPC_GetNodePointPosition(nodeid, &Float:x, &Float:y, &Float:z);
native FCNPC_GetNodePointCount(nodeid);
native FCNPC_GetNodeInfo(nodeid, &vehnodes, &pednodes, &navinode);
native FCNPC_PlayNode(npcid, nodeid, move_type = FCNPC_MOVE_TYPE_AUTO, Float:speed = FCNPC_MOVE_SPEED_AUTO, bool:UseMapAndreas = false, Float:radius = 0.0, bool:setangle = true);
native FCNPC_StopPlayingNode(npcid);
native FCNPC_PausePlayingNode(npcid);
native FCNPC_ResumePlayingNode(npcid);
native FCNPC_IsPlayingNode(npcid);
native FCNPC_IsPlayingNodePaused(npcid);

native FCNPC_CreateMovePath();
native FCNPC_DestroyMovePath(pathid);
native FCNPC_IsValidMovePath(pathid);
native FCNPC_AddPointToPath(pathid, Float:x, Float:y, Float:z);
native FCNPC_AddPointsToPath(pathid, Float:points[][3], const size = sizeof(points));
native FCNPC_AddPointsToPath2(pathid, Float:points_x[], Float:points_y[], Float:points_z[], const size = sizeof(points_x));
native FCNPC_RemovePointFromPath(pathid, pointid);
native FCNPC_IsValidMovePoint(pathid, pointid);
native FCNPC_GetMovePoint(pathid, pointid, &Float:x, &Float:y, &Float:z);
native FCNPC_GetNumberMovePoint(pathid);
native FCNPC_GoByMovePath(npcid, pathid, type = FCNPC_MOVE_TYPE_AUTO, Float:speed = FCNPC_MOVE_SPEED_AUTO, bool:UseMapAndreas = false, Float:radius = 0.0, bool:setangle = true, Float:dist_offset = 0.0);

native FCNPC_ToggleMapAndreasUsage(npcid, bool:enabled);
native FCNPC_IsMapAndreasUsed(npcid);
native FCNPC_SetMinHeightPosCall(npcid, Float:height);
native Float:FCNPC_GetMinHeightPosCall(npcid);

// ALS for some callbacks
public OnPlayerGiveDamage(playerid, damagedid, Float:amount, weaponid, bodypart)
{
	#if defined FCNPC_OnPlayerGiveDamage
		return FCNPC_OnPlayerGiveDamage(playerid, damagedid, amount, weaponid, bodypart);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerGiveDamage
	#undef OnPlayerGiveDamage
#else
	#define _ALS_OnPlayerGiveDamage
#endif

#define OnPlayerGiveDamage FCNPC_OnPlayerGiveDamage
#if defined FCNPC_OnPlayerGiveDamage
	forward FCNPC_OnPlayerGiveDamage(playerid, damagedid, Float:amount, weaponid, bodypart);
#endif


public OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart)
{
	#if defined FCNPC_OnPlayerTakeDamage
		return FCNPC_OnPlayerTakeDamage(playerid, issuerid, amount, weaponid, bodypart);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerTakeDamage
	#undef OnPlayerTakeDamage
#else
	#define _ALS_OnPlayerTakeDamage
#endif

#define OnPlayerTakeDamage FCNPC_OnPlayerTakeDamage
#if defined FCNPC_OnPlayerTakeDamage
	forward FCNPC_OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart);
#endif


public OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
	#if defined FCNPC_OnPlayerWeaponShot
		return FCNPC_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, fX, fY, fZ);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerWeaponShot
	#undef OnPlayerWeaponShot
#else
	#define _ALS_OnPlayerWeaponShot
#endif

#define OnPlayerWeaponShot FCNPC_OnPlayerWeaponShot
#if defined FCNPC_OnPlayerWeaponShot
	forward FCNPC_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ);
#endif


public OnPlayerStreamIn(playerid, forplayerid)
{
	#if defined FCNPC_OnPlayerStreamIn
		return FCNPC_OnPlayerStreamIn(playerid, forplayerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerStreamIn
	#undef OnPlayerStreamIn
#else
	#define _ALS_OnPlayerStreamIn
#endif

#define OnPlayerStreamIn FCNPC_OnPlayerStreamIn
#if defined FCNPC_OnPlayerStreamIn
	forward FCNPC_OnPlayerStreamIn(playerid, forplayerid);
#endif


public OnPlayerStreamOut(playerid, forplayerid)
{
	#if defined FCNPC_OnPlayerStreamOut
		return FCNPC_OnPlayerStreamOut(playerid, forplayerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerStreamOut
	#undef OnPlayerStreamOut
#else
	#define _ALS_OnPlayerStreamOut
#endif

#define OnPlayerStreamOut FCNPC_OnPlayerStreamOut
#if defined FCNPC_OnPlayerStreamOut
	forward FCNPC_OnPlayerStreamOut(playerid, forplayerid);
#endif


// Update checker
#if !defined HTTP
	#tryinclude <a_http>
#endif

#if !defined HTTP
	#tryinclude "a_http"
#endif

#if defined HTTP && !defined FCNPC_DISABLE_VERSION_CHECK
	#if defined FILTERSCRIPT
		public OnFilterScriptInit()
		{
			FCNPC_SendVersionRequest();

			#if defined fcnpc0_OnFilterScriptInit
				return fcnpc0_OnFilterScriptInit();
			#else
				return 1;
			#endif
		}
		#if defined _ALS_OnFilterScriptInit
			#undef OnFilterScriptInit
		#else
			#define _ALS_OnFilterScriptInit
		#endif

		#define OnFilterScriptInit fcnpc0_OnFilterScriptInit
		#if defined fcnpc0_OnFilterScriptInit
			forward fcnpc0_OnFilterScriptInit();
		#endif
	#else
		public OnGameModeInit()
		{
			FCNPC_SendVersionRequest();

			#if defined fcnpc0_OnGameModeInit
				return fcnpc0_OnGameModeInit();
			#else
				return 1;
			#endif
		}
		#if defined _ALS_OnGameModeInit
			#undef OnGameModeInit
		#else
			#define _ALS_OnGameModeInit
		#endif

		#define OnGameModeInit fcnpc0_OnGameModeInit
		#if defined fcnpc0_OnGameModeInit
			forward fcnpc0_OnGameModeInit();
		#endif
	#endif

	static stock FCNPC_SendVersionRequest()
	{
		HTTP(0, HTTP_GET, "ziggi.org/github/lastrelease.php?owner=ziggi&repo=FCNPC&elem=tag_name", "", "FCNPC_OnRequestResponse");
	}

	forward FCNPC_OnRequestResponse(index, response_code, data[]);
	public FCNPC_OnRequestResponse(index, response_code, data[])
	{
		if (index == 0 && response_code == 200) {
			new length = strlen(data);
			if (length == 0 || length > 10) {
				return;
			}

			new version[16];
			strmid(version, data, 1, length);

			if (!('0' <= version[0] <= '9')) {
				return;
			}

			new plugin_version[16];
			FCNPC_GetPluginVersion(plugin_version);

			if (strcmp(version, plugin_version) != 0) {
				printf("\n** Please update your FCNPC plugin to %s from here: https://github.com/ziggi/FCNPC/releases.\n", version);
			}
		}
	}
#endif

#if defined foreach
	/*
		Iterator
	*/
	new
		Iterator:FCNPC<MAX_PLAYERS>;

	/*
		Init
	*/
	#if defined FILTERSCRIPT
		/*
			OnFilterScriptInit
		*/
		public OnFilterScriptInit()
		{
			Iter_Clear(FCNPC);

			for (new i = 0; i != MAX_PLAYERS; ++i) {
				if (FCNPC_IsValid(i)) {
					Iter_Add(FCNPC, i);
				}
			}

			#if defined fcnpc1_OnFilterScriptInit
				return fcnpc1_OnFilterScriptInit();
			#else
				return 1;
			#endif
		}
		#if defined _ALS_OnFilterScriptInit
			#undef OnFilterScriptInit
		#else
			#define _ALS_OnFilterScriptInit
		#endif

		#define OnFilterScriptInit fcnpc1_OnFilterScriptInit
		#if defined fcnpc1_OnFilterScriptInit
			forward fcnpc1_OnFilterScriptInit();
		#endif
	#else
		/*
			OnGameModeInit
		*/
		public OnGameModeInit()
		{
			Iter_Clear(FCNPC);

			for (new i = 0; i != MAX_PLAYERS; ++i) {
				if (FCNPC_IsValid(i)) {
					Iter_Add(FCNPC, i);
				}
			}

			#if defined fcnpc1_OnGameModeInit
				return fcnpc1_OnGameModeInit();
			#else
				return 1;
			#endif
		}
		#if defined _ALS_OnGameModeInit
			#undef OnGameModeInit
		#else
			#define _ALS_OnGameModeInit
		#endif

		#define OnGameModeInit fcnpc1_OnGameModeInit
		#if defined fcnpc1_OnGameModeInit
			forward fcnpc1_OnGameModeInit();
		#endif
	#endif

	/*
		FCNPC_OnCreate
	*/
	public FCNPC_OnCreate(npcid)
	{
		Iter_Add(FCNPC, npcid);

		#if defined fcnpc1_FCNPC_OnCreate
			return fcnpc1_FCNPC_OnCreate(npcid);
		#else
			return 1;
		#endif
	}
	#if defined _ALS_FCNPC_OnCreate
		#undef FCNPC_OnCreate
	#else
		#define _ALS_FCNPC_OnCreate
	#endif

	#define FCNPC_OnCreate fcnpc1_FCNPC_OnCreate
	#if defined fcnpc1_FCNPC_OnCreate
		forward fcnpc1_FCNPC_OnCreate(npcid);
	#endif

	/*
		FCNPC_OnDestroy
	*/
	public FCNPC_OnDestroy(npcid)
	{
		SetTimerEx("Iter_FCNPCOnDestroyInternal", 0, false, "i", npcid);

		#if defined fcnpc1_FCNPC_OnDestroy
			return fcnpc1_FCNPC_OnDestroy(npcid);
		#else
			return 1;
		#endif
	}
	#if defined _ALS_FCNPC_OnDestroy
		#undef FCNPC_OnDestroy
	#else
		#define _ALS_FCNPC_OnDestroy
	#endif

	#define FCNPC_OnDestroy fcnpc1_FCNPC_OnDestroy
	#if defined fcnpc1_FCNPC_OnDestroy
		forward fcnpc1_FCNPC_OnDestroy(npcid);
	#endif

	/*
		Iter_FCNPCOnDestroyInternal
	*/
	forward Iter_FCNPCOnDestroyInternal(npcid);
	public Iter_FCNPCOnDestroyInternal(npcid)
	{
		Iter_Remove(FCNPC, npcid);
	}
#endif
