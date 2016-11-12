/* ---------------------------------
	 FCNPC Plugin Sample FS

- File: FCNPC.pwn
- Author: OrMisicL
---------------------------------*/

#define FILTERSCRIPT

#include <a_samp>
#include <FCNPC>

#define COLOR_GREY 0xAFAFAFAA
#define COLOR_RED  0xB35959AA

#define MAX_NPCS    25

forward UpdateGuards();

enum ENPC
{
	stage
};

new NPC[MAX_NPCS][ENPC];

new Float:Spawns[][4] =
{
	{1697.2258, -1706.5187, 13.5419, 91.8542}
};

new Float:Mouvement[][3] =
{
	{1697.7418, -1600.1525, 13.5469},
	{1797.9518, -1619.9912, 13.5469},
	{1814.1798, -1645.9603, 13.5469},
	{1815.2373, -1724.6650, 13.5469},
	{1696.7618, -1725.9985, 13.5469}
};

public OnFilterScriptInit()
{
	printf("");
	printf("-------------------------------------------------");
	printf("     FCNPC - Fully Controllable NPC Filterscript");
	printf("");
	printf("- Author: OrMisicL");
	printf("-------------------------------------------------");
	printf("");
	// Add some vehicles
	AddStaticVehicle(541, 1683.8768, -1670.6482, 13.0868, 0.3921, 1, 0);
	AddStaticVehicle(541, 1683.8768, -1665.6482, 13.0868, 0.3921, 2, 0);
	AddStaticVehicle(541, 1683.8768, -1660.6482, 13.0868, 0.3921, 3, 0);
	AddStaticVehicle(541, 1683.8768, -1655.6482, 13.0868, 0.3921, 4, 0);
	// Set the NPC update rate to 30ms each
	FCNPC_SetUpdateRate(80);
	new npcid;
	// Create NPCs
	for(new i = 0; i < MAX_NPCS; i++)
	{
		new name[24];
		format(name, 24, "NPC_%d", i + 1);
		npcid = FCNPC_Create(name);
		NPC[i][stage] = 0;
		// Generate a random skin and position for the NPC
		FCNPC_Spawn(npcid, random(299), Spawns[0][0], Spawns[0][1] + npcid, Spawns[0][2]);
		FCNPC_SetAngle(npcid, Spawns[0][3]);
		FCNPC_SetInterior(npcid, 0);
		// Give him some weapons and ammo
		FCNPC_SetWeapon(npcid, random(11) + 22);
		FCNPC_SetAmmo(npcid, 500);
		// Move the NPC
		FCNPC_GoTo(npcid, Mouvement[0][0], Mouvement[0][1], Mouvement[0][2], MOVE_TYPE_RUN, 0, true);
	}
//	FCNPC_Destroy(npcid);
	return 1;
}

public FCNPC_OnDeath(npcid, killerid, weaponid)
{
	new name[24];
	GetPlayerName(killerid, name, 24);
	new msg[128];
 	format(msg, 128, "NPC %d was killed by %d (%s) with weapon %d", npcid, killerid, name, weaponid);
 	SendClientMessageToAll(0xFF00FF00, msg);
 	return 1;
}

public FCNPC_OnCreate(npcid)
{
	return 1;
}

public FCNPC_OnReachDestination(npcid)
{
	// Increase the moving stage
	NPC[npcid][stage]++;
	if(NPC[npcid][stage] > 4)
        NPC[npcid][stage] = 0;

	new mstage = NPC[npcid][stage];
	FCNPC_GoTo(npcid, Mouvement[mstage][0], Mouvement[mstage][1], Mouvement[mstage][2], MOVE_TYPE_RUN, 0, true);
	return 1;
}

new npcs = 0;

public OnRconCommand(cmd[])
{
	if(!strcmp(cmd, "npc", true))
	{
		new name[24];
		format(name, 24, "NPC_%d", npcs + 1);
 		FCNPC_Create(name);
 		npcs++;
 	}
 	return 1;
}




