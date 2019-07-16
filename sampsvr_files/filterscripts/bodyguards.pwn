/* ---------------------------------
	 FCNPC Plugin bodyguards FS

- File: bodyguards.pwn
- Author: ziggi
---------------------------------*/

#define FILTERSCRIPT

#include <a_samp>

#if !defined _FCNPC_included
	#tryinclude <FCNPC>
#endif

#if !defined _FCNPC_included
	#tryinclude "FCNPC"
#endif

#if !defined _FCNPC_included
	#tryinclude "../FCNPC"
#endif

#if !defined _FCNPC_included
	#error Add FCNPC.inc to your scripts directory
#endif

/*
	Defines
*/

#define MAX_BODYGUARDS 6
#define DEBUG

#define MAX_BODYPART_NAME 10
#define BG_INVALID_SLOT_ID -1

/*
	Vars
*/

static
	gPlayerNpc[MAX_PLAYERS][MAX_BODYGUARDS],
	gNpcSlot[MAX_PLAYERS],
	gNpcPlayer[MAX_PLAYERS],
	gFollowTimer[MAX_PLAYERS],
	gFollowTarget[MAX_PLAYERS],
	PlayerText3D:gNpcText3D[MAX_PLAYERS];

/*
	Publics
*/

public OnFilterScriptInit()
{
	for (new playerid; playerid < MAX_PLAYERS; playerid++) {
		for (new slot; slot < MAX_BODYGUARDS; slot++) {
			gPlayerNpc[playerid][slot] = INVALID_PLAYER_ID;
		}

		gNpcSlot[playerid] = BG_INVALID_SLOT_ID;
		gNpcPlayer[playerid] = INVALID_PLAYER_ID;
		gFollowTimer[playerid] = 0;
		gFollowTarget[playerid] = INVALID_PLAYER_ID;
	}

	printf("\n-------------------------------");
	printf("    FCNPC Bodyguard System");
	printf("    Author: ziggi");
	printf("-------------------------------\n");
	return 1;
}

public OnFilterScriptExit()
{
	for (new playerid; playerid < MAX_PLAYERS; playerid++) {
		if (BG_IsValid(playerid)) {
			BG_DeleteByID(playerid);
		}
	}
}

public OnPlayerDisconnect(playerid, reason)
{
	if (!BG_IsValid(playerid)) {
		for (new slot; slot < MAX_BODYGUARDS; slot++) {
			if (BG_IsValidSlot(playerid, slot)) {
				BG_Delete(playerid, slot);
			}
		}
	}
	return 1;
}

public FCNPC_OnReachDestination(npcid)
{
	BG_SetFollowTimer(npcid);
}

public FCNPC_OnTakeDamage(npcid, issuerid, Float:amount, weaponid, bodypart)
{
	if (!IsPlayerConnected(issuerid) || !BG_IsValid(npcid)) {
		return 1;
	}

#if defined DEBUG
	new
		string[44 + 3 + MAX_PLAYER_NAME + 3 + 2 + MAX_BODYPART_NAME + 16],
		player_name[MAX_PLAYER_NAME + 1],
		bodypart_name[MAX_BODYPART_NAME],
		weapon_name[16];

	GetPlayerName(issuerid, player_name, sizeof(player_name));
	GetBodypartName(bodypart, bodypart_name);
	GetWeaponName(weaponid, weapon_name, sizeof(weapon_name));

	format(string, sizeof(string),
	       "NPC(%d) was damaged by %s(%d) with %s(%d) in the %s (hp: %f)",
	       npcid, player_name, issuerid, weapon_name, weaponid, bodypart_name, amount);
	SendClientMessageToAll(0xFF0000FF, string);
#endif
	return 1;
}

public FCNPC_OnVehicleEntryComplete(npcid, vehicleid, seatid)
{
#if defined DEBUG
	new string[27 + 3 + 1];
	format(string, sizeof(string), "NPC %d finished vehicle entry", npcid);
	SendClientMessageToAll(0xFF0000FF, string);
#endif
	return 1;
}

public FCNPC_OnVehicleExitComplete(npcid, vehicleid)
{
#if defined DEBUG
	new string[26 + 3 + 1];
	format(string, sizeof(string), "NPC %d finished vehicle exit", npcid);
	SendClientMessageToAll(0xFF0000FF, string);
#endif
	return 1;
}

public OnPlayerCommandText(playerid, cmdtext[])
{
	new
		cmd[20],
		subcmd[20],
		param[20],
		idx;

	cmd = strcharsplit(cmdtext, idx);

	if (!strcmp(cmd, "/bg", true)) {
		subcmd = strcharsplit(cmdtext, idx);
		if (strlen(subcmd) == 0) {
			SendClientMessage(playerid, -1, "{8470FF}Use: {FFFFFF}/bg [option]");
			SendClientMessage(playerid, -1, "{8470FF}Options: {FFFFFF}add, delete, follow, unfollow, enter, exit");
			SendClientMessage(playerid, -1, "{8470FF}Options: {FFFFFF}shoot, aim, reset");
			return 1;
		}

		if (!strcmp(subcmd, "add", true)) {
			new npcid = BG_Add(playerid);
			if (npcid < 0) {
				if (npcid == -1) {
					SendClientMessage(playerid, -1, "{CC0000}Error: {FFFFFF}Maximum bodyguards allowed per player is reached.");
				} else if (npcid == -2) {
					SendClientMessage(playerid, -1, "{CC0000}Error: {FFFFFF}No slots available.");
				} else {
					SendClientMessage(playerid, -1, "{CC0000}Error: {FFFFFF}Unknown error.");
				}
				return 1;
			}

			new
				Float:player_x,
				Float:player_y,
				Float:player_z,
				Float:player_angle;

			GetPlayerPos(playerid, player_x, player_y, player_z);
			GetPlayerFacingAngle(playerid, player_angle);

			new
				Float:pos_x,
				Float:pos_y;

			GetCoordsInFront(player_x, player_y, player_angle, 2.0, pos_x, pos_y);

			FCNPC_Spawn(npcid, random(4) + 163, pos_x, pos_y, player_z);
			FCNPC_SetAngleToPlayer(npcid, playerid);
			FCNPC_SetVirtualWorld(npcid, GetPlayerVirtualWorld(playerid));
			FCNPC_SetInterior(npcid, GetPlayerInterior(playerid));
			FCNPC_SetWeapon(npcid, random(3) + 22);
			FCNPC_UseInfiniteAmmo(npcid, true);
			FCNPC_SetArmour(npcid, 80);
			FCNPC_SetFightingStyle(npcid, random(4) + 3);

			new label_text[2 + 3 + 1];
			format(label_text, sizeof(label_text), "[%d]", BG_GetSlotByID(npcid));
			gNpcText3D[npcid] = CreatePlayer3DTextLabel(playerid, label_text, 0xFFFFFFFF, 0.0, 0.0, 0.0, 5.0, npcid);

		#if defined DEBUG
			new
				string[38 + MAX_PLAYER_NAME + 3 + 1],
				playername[MAX_PLAYER_NAME + 1];

			GetPlayerName(playerid, playername, sizeof(playername));
			format(string, sizeof(string), "Player %s (ID: %d) has created a bodyguard", playername, playerid);
			SendClientMessageToAll(0xFF0000FF, string);
		#endif
			return 1;
		}

		if (!strcmp(subcmd, "delete", true)) {
			param = strcharsplit(cmdtext, idx);
			if (strlen(param) == 0) {
				SendClientMessage(playerid, -1, "{8470FF}Use: {FFFFFF}/bg delete [slot]");
				return 1;
			}

			new slot = strval(param);
			if (!BG_IsValidSlot(playerid, slot)) {
				SendClientMessage(playerid, -1, "{CC0000}Error: {FFFFFF}Invalid bodyguard.");
				return 1;
			}

			BG_Delete(playerid, slot);
			return 1;
		}

		if (!strcmp(subcmd, "follow", true)) {
			param = strcharsplit(cmdtext, idx);
			if (strlen(param) == 0) {
				SendClientMessage(playerid, -1, "{8470FF}Use: {FFFFFF}/bg follow [slot]");
				return 1;
			}

			new slot = strval(param);
			if (!BG_IsValidSlot(playerid, slot)) {
				SendClientMessage(playerid, -1, "{CC0000}Error: {FFFFFF}Invalid bodyguard.");
				return 1;
			}

			BG_FollowPlayer(playerid, slot, playerid);
			return 1;
		}

		if (!strcmp(subcmd, "unfollow", true)) {
			param = strcharsplit(cmdtext, idx);
			if (strlen(param) == 0) {
				SendClientMessage(playerid, -1, "{8470FF}Use: {FFFFFF}/bg unfollow [slot]");
				return 1;
			}

			new slot = strval(param);
			if (!BG_IsValidSlot(playerid, slot)) {
				SendClientMessage(playerid, -1, "{CC0000}Error: {FFFFFF}Invalid bodyguard.");
				return 1;
			}

			BG_UnFollowPlayer(playerid, slot);
			return 1;
		}

		if (!strcmp(subcmd, "aim", true)) {
			param = strcharsplit(cmdtext, idx);
			if (strlen(param) == 0) {
				SendClientMessage(playerid, -1, "{8470FF}Use: {FFFFFF}/bg aim [slot] [targetid]");
				return 1;
			}

			new slot = strval(param);
			if (!BG_IsValidSlot(playerid, slot)) {
				SendClientMessage(playerid, -1, "{CC0000}Error: {FFFFFF}Invalid bodyguard.");
				return 1;
			}

			param = strcharsplit(cmdtext, idx);
			if (strlen(param) == 0) {
				SendClientMessage(playerid, -1, "{8470FF}Use: {FFFFFF}/bg aim [slot] [targetid]");
				return 1;
			}

			new targetid = strval(param);
			if (!IsPlayerConnected(targetid)) {
				SendClientMessage(playerid, -1, "{CC0000}Error: {FFFFFF}Invalid target.");
				return 1;
			}

			BG_Aim(playerid, slot, targetid);
			return 1;
		}

		if (!strcmp(subcmd, "shoot", true)) {
			param = strcharsplit(cmdtext, idx);
			if (strlen(param) == 0) {
				SendClientMessage(playerid, -1, "{8470FF}Use: {FFFFFF}/bg shoot [slot] [targetid]");
				return 1;
			}

			new slot = strval(param);
			if (!BG_IsValidSlot(playerid, slot)) {
				SendClientMessage(playerid, -1, "{CC0000}Error: {FFFFFF}Invalid bodyguard.");
				return 1;
			}

			param = strcharsplit(cmdtext, idx);
			if (strlen(param) == 0) {
				SendClientMessage(playerid, -1, "{8470FF}Use: {FFFFFF}/bg shoot [slot] [targetid]");
				return 1;
			}

			new targetid = strval(param);
			if (!IsPlayerConnected(targetid)) {
				SendClientMessage(playerid, -1, "{CC0000}Error: {FFFFFF}Invalid target.");
				return 1;
			}

			BG_Shoot(playerid, slot, targetid);
			return 1;
		}

		if (!strcmp(subcmd, "reset", true)) {
			param = strcharsplit(cmdtext, idx);
			if (strlen(param) == 0) {
				SendClientMessage(playerid, -1, "{8470FF}Use: {FFFFFF}/bg reset [slot]");
				return 1;
			}

			new slot = strval(param);
			if (!BG_IsValidSlot(playerid, slot)) {
				SendClientMessage(playerid, -1, "{CC0000}Error: {FFFFFF}Invalid bodyguard.");
				return 1;
			}

			BG_AimReset(playerid, slot);
			return 1;
		}

		if (!strcmp(subcmd, "enter", true)) {
			param = strcharsplit(cmdtext, idx);
			if (strlen(param) == 0) {
				SendClientMessage(playerid, -1, "{8470FF}Use: {FFFFFF}/bg enter [slot] [vehicleid] [seat]");
				return 1;
			}

			new slot = strval(param);
			if (!BG_IsValidSlot(playerid, slot)) {
				SendClientMessage(playerid, -1, "{CC0000}Error: {FFFFFF}Invalid bodyguard.");
				return 1;
			}

			param = strcharsplit(cmdtext, idx);
			if (strlen(param) == 0) {
				SendClientMessage(playerid, -1, "{8470FF}Use: {FFFFFF}/bg enter [slot] [vehicleid] [seat]");
				return 1;
			}

			new vehicleid = strval(param);
			if (!GetVehicleModel(vehicleid)) {
				SendClientMessage(playerid, -1, "{CC0000}Error: {FFFFFF}Invalid target.");
				return 1;
			}

			param = strcharsplit(cmdtext, idx);
			new seat = strval(param);

			BG_UnFollowPlayer(playerid, slot);
			BG_EnterVehicle(playerid, slot, vehicleid, seat);
			return 1;
		}

		if (!strcmp(subcmd, "exit", true)) {
			param = strcharsplit(cmdtext, idx);
			if (strlen(param) == 0) {
				SendClientMessage(playerid, -1, "{8470FF}Use: {FFFFFF}/bg exit [slot]");
				return 1;
			}

			new slot = strval(param);
			if (!BG_IsValidSlot(playerid, slot)) {
				SendClientMessage(playerid, -1, "{CC0000}Error: {FFFFFF}Invalid bodyguard.");
				return 1;
			}

			BG_ExitVehicle(playerid, slot);
			return 1;
		}

		if (!strcmp(subcmd, "melee", true)) {
			param = strcharsplit(cmdtext, idx);
			if (strlen(param) == 0) {
				SendClientMessage(playerid, -1, "{8470FF}Use: {FFFFFF}/bg melee [slot]");
				return 1;
			}

			new slot = strval(param);
			if (!BG_IsValidSlot(playerid, slot)) {
				SendClientMessage(playerid, -1, "{CC0000}Error: {FFFFFF}Invalid bodyguard.");
				return 1;
			}

			BG_Melee(playerid, slot);
			return 1;
		}

		if (!strcmp(subcmd, "stopmelee", true)) {
			param = strcharsplit(cmdtext, idx);
			if (strlen(param) == 0) {
				SendClientMessage(playerid, -1, "{8470FF}Use: {FFFFFF}/bg stopmelee [slot]");
				return 1;
			}

			new slot = strval(param);
			if (!BG_IsValidSlot(playerid, slot)) {
				SendClientMessage(playerid, -1, "{CC0000}Error: {FFFFFF}Invalid bodyguard.");
				return 1;
			}

			BG_MeleeStop(playerid, slot);
			return 1;
		}
	}
	return 0;
}

/*
	Vehicle functions
*/

stock BG_EnterVehicle(playerid, slot, vehicleid, seat)
{
	new npcid = gPlayerNpc[playerid][slot];
	FCNPC_EnterVehicle(npcid, vehicleid, seat, FCNPC_MOVE_TYPE_RUN);
}

stock BG_ExitVehicle(playerid, slot)
{
	new npcid = gPlayerNpc[playerid][slot];
	FCNPC_ExitVehicle(npcid);
}

/*
	Aim functions
*/

stock BG_Aim(playerid, slot, targetid)
{
	new npcid = gPlayerNpc[playerid][slot];
	FCNPC_AimAtPlayer(npcid, targetid);
}

stock BG_Shoot(playerid, slot, targetid)
{
	new npcid = gPlayerNpc[playerid][slot];
	FCNPC_AimAtPlayer(npcid, targetid, .shoot = true);
}

stock BG_AimReset(playerid, slot)
{
	new npcid = gPlayerNpc[playerid][slot];
	FCNPC_StopAim(npcid);
}

/*
	Melee functions
*/

stock BG_Melee(playerid, slot)
{
	BG_MeleeStop(playerid, slot);

	new npcid = gPlayerNpc[playerid][slot];
	SetPVarInt(npcid, "bg_last_weapon", FCNPC_GetWeapon(npcid));
	FCNPC_SetWeapon(npcid, 0);

	FCNPC_MeleeAttack(npcid, .fighting_style = true);
}

stock BG_MeleeStop(playerid, slot)
{
	new npcid = gPlayerNpc[playerid][slot];
	FCNPC_SetWeapon(npcid, GetPVarInt(npcid, "bg_last_weapon"));
	FCNPC_StopAttack(npcid);
}

/*
	Follow functions
*/

stock BG_SetFollowTimer(npcid)
{
	if (gFollowTarget[npcid] != INVALID_PLAYER_ID) {
		gFollowTimer[npcid] = SetTimerEx("BG_SetFollowPosition", 300, true, "ii", npcid, gFollowTarget[npcid]);
	}
}

stock BG_FollowPlayer(playerid, slot, targetid)
{
	BG_UnFollowPlayer(playerid, slot);

	new npcid = gPlayerNpc[playerid][slot];
	gFollowTarget[npcid] = targetid;

	new isgo = BG_SetFollowPosition(npcid, targetid);
	if (!isgo) {
		BG_SetFollowTimer(npcid);
	}
}

stock BG_UnFollowPlayer(playerid, slot)
{
	new npcid = gPlayerNpc[playerid][slot];

	if (gFollowTimer[npcid] != 0) {
		KillTimer(gFollowTimer[npcid]);
		gFollowTimer[npcid] = 0;
	}

	if (gFollowTarget[npcid] == INVALID_PLAYER_ID) {
		return 0;
	}

	gFollowTarget[npcid] = INVALID_PLAYER_ID;
	return 1;
}

forward BG_SetFollowPosition(npcid, playerid);
public BG_SetFollowPosition(npcid, playerid)
{
	new
		Float:pos_x,
		Float:pos_y,
		Float:pos_z,
		Float:min_distance,
		Float:range,
		Float:speed;

	GetPlayerPos(playerid, pos_x, pos_y, pos_z);

	if (FCNPC_GetVehicleID(npcid) != INVALID_VEHICLE_ID) {
		min_distance = -3.0;
		range = -min_distance + 1.0;
		speed = 0.8;
	} else {
		min_distance = -1.0;
		range = -min_distance + 0.5;
		speed = FCNPC_MOVE_SPEED_AUTO;
	}

	if (!IsPlayerInRangeOfPoint(npcid, range, pos_x, pos_y, pos_z)) {
		FCNPC_GoToPlayer(npcid, playerid, .speed = speed, .min_distance = min_distance);
		KillTimer(gFollowTimer[npcid]);
		gFollowTimer[npcid] = 0;
		return 1;
	}
	return 0;
}

/*
	Add/Delete functions
*/

stock BG_Add(playerid)
{
	new slot = BG_GetFreeSlot(playerid);
	if (slot == BG_INVALID_SLOT_ID) {
		return -1;
	}

	new name[MAX_PLAYER_NAME + 1];
	format(name, sizeof(name), "Guard_%d_%d", playerid, slot);
	new npcid = FCNPC_Create(name);
	if (npcid == INVALID_PLAYER_ID) {
		return -2;
	}

	gPlayerNpc[playerid][slot] = npcid;
	gNpcPlayer[npcid] = playerid;
	gNpcSlot[npcid] = slot;
	return npcid;
}

stock BG_Delete(playerid, slot)
{
	new npcid = gPlayerNpc[playerid][slot];

	FCNPC_Destroy(npcid);
	BG_UnFollowPlayer(playerid, slot);
	DeletePlayer3DTextLabel(playerid, gNpcText3D[npcid]);

	gPlayerNpc[playerid][slot] = INVALID_PLAYER_ID;
	gNpcPlayer[npcid] = INVALID_PLAYER_ID;
	gNpcSlot[npcid] = BG_INVALID_SLOT_ID;
}

stock BG_DeleteByID(npcid)
{
	new playerid = gNpcPlayer[npcid];
	new slot = gNpcSlot[npcid];

	BG_Delete(playerid, slot);
}

/*
	Slot functions
*/

stock BG_IsValidSlot(playerid, slot)
{
	if (slot < 0 || slot > sizeof(gPlayerNpc[])) {
		return false;
	}
	return gPlayerNpc[playerid][slot] != INVALID_PLAYER_ID;
}

stock BG_IsValid(npcid)
{
	return FCNPC_IsValid(npcid) && gNpcSlot[npcid] != BG_INVALID_SLOT_ID;
}

stock BG_GetFreeSlot(playerid)
{
	for (new slot; slot < sizeof(gPlayerNpc[]); slot++) {
		if (gPlayerNpc[playerid][slot] == INVALID_PLAYER_ID) {
			return slot;
		}
	}
	return BG_INVALID_SLOT_ID;
}

stock BG_GetSlotByID(npcid)
{
	return gNpcSlot[npcid];
}

/*
	Other functions
*/

stock strcharsplit(const string[], &index, seperator = ' ')
{
	new
		result[20],
		i;

	if (index != 0 && string[index] != '\0') {
		index++;
	}

	while (string[index] && string[index] != seperator && string[index] != '\r' && string[index] != '\n') {
		result[i++] = string[index++];
	}
	return result;
}

stock GetCoordsInFront(Float:x, Float:y, Float:a, Float:distance, &Float:res_x, &Float:res_y)
{
	res_x = x + (distance * floatsin(-a, degrees));
	res_y = y + (distance * floatcos(-a, degrees));
}

stock GetBodypartName(bodypart, name[], const size = sizeof(name))
{
	static const
		names[][] = {
			!"torso",
			!"groin",
			!"left arm",
			!"right arm",
			!"left leg",
			!"right leg",
			!"head"
		};

	if (9 < bodypart < 3) {
		return 0;
	}

	return strunpack(name, names[bodypart - 3], size);
}
