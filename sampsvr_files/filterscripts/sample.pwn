/* ---------------------------------
	 FCNPC Plugin sample FS

- File: sample.pwn
- Author: OrMisicL
- Contributor: ziggi
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

#define START_NPCS 25

forward CreateNPC();
forward DestroyNPC(npcid);

enum E_NPC {
	bool:E_VALID,
	E_MOVE_STAGE,
};

new gNpc[MAX_PLAYERS][E_NPC];

new Float:gSpawns[][3] = {
	{1697.2258, -1706.5187, 13.5419}
};

new Float:gMovements[][3] = {
	{1697.7418, -1600.1525, 13.5469},
	{1797.9518, -1619.9912, 13.5469},
	{1814.1798, -1645.9603, 13.5469},
	{1815.2373, -1724.6650, 13.5469},
	{1696.7618, -1725.9985, 13.5469}
};

new gNpcCount;

public OnFilterScriptInit()
{
	// Create NPCs
	for (new i = 0; i < START_NPCS; i++) {
		SetTimer("CreateNPC", 3000 * i + random(10000), 0);
	}

	return 1;
}

public CreateNPC()
{
	new name[MAX_PLAYER_NAME + 1];
	format(name, sizeof(name), "NPC_%d", gNpcCount);

	new npcid = FCNPC_Create(name);

	gNpc[npcid][E_MOVE_STAGE] = 0;
	gNpc[npcid][E_VALID] = true;

	FCNPC_Spawn(npcid, random(312), gSpawns[0][0], gSpawns[0][1], gSpawns[0][2]);

	gNpcCount++;
}

stock MoveNPC(npcid)
{
	new mstage = gNpc[npcid][E_MOVE_STAGE];
	FCNPC_GoTo(npcid, gMovements[mstage][0], gMovements[mstage][1], gMovements[mstage][2], FCNPC_MOVE_TYPE_RUN);
}

public FCNPC_OnDeath(npcid, killerid, reason)
{
	if (!gNpc[npcid][E_VALID]) {
		return 1;
	}

	new name[MAX_PLAYER_NAME + 1];
	GetPlayerName(killerid, name, sizeof(name));

	new msg[144];
 	format(msg, sizeof(msg), "NPC %d was killed by %d (%s) with weapon %d", npcid, killerid, name, reason);
 	SendClientMessageToAll(0xFF00FF00, msg);

 	SetTimerEx("DestroyNPC", 2000, 0, "i", npcid);
 	return 1;
}

public DestroyNPC(npcid)
{
	FCNPC_Destroy(npcid);
	gNpc[npcid][E_VALID] = false;
}

public FCNPC_OnSpawn(npcid)
{
	if (!gNpc[npcid][E_VALID]) {
		return 1;
	}

	// Give him some weapons and ammo
	FCNPC_SetWeapon(npcid, random(11) + 22);
	FCNPC_SetAmmo(npcid, 500);

	// Move the NPC
	MoveNPC(npcid);
	return 1;
}

public FCNPC_OnReachDestination(npcid)
{
	if (!gNpc[npcid][E_VALID]) {
		return 1;
	}

	// Increase the moving stage
	gNpc[npcid][E_MOVE_STAGE]++;
	if (gNpc[npcid][E_MOVE_STAGE] >= sizeof(gMovements)) {
		gNpc[npcid][E_MOVE_STAGE] = 0;
	}

	MoveNPC(npcid);
	return 1;
}

public OnRconCommand(cmd[])
{
	if (!strcmp(cmd, "npc", true)) {
		CreateNPC();
	}

 	return 1;
}
