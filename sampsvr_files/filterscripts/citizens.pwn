/* ---------------------------------
	 FCNPC Plugin citizens FS

- File: citizens.pwn
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

#define COLOR_GREY 0xAFAFAFAA
#define COLOR_RED  0xB35959AA

#define LOS_SANTOS

forward Float:frandom(Float:max);

new
	gMaxCitizens,
	gCitizenID[MAX_PLAYERS] = {INVALID_PLAYER_ID, ...},
	gCars[] = {
		401, 402, 404, 405, 409, 410, 411, 412, 415, 419, 420, 421, 426, 429,
		434, 436, 438, 439, 442, 445, 451, 458, 466, 467, 474, 475, 477, 479,
		480, 491, 492, 496, 506, 507, 516, 517, 518, 526, 527, 529, 533, 534,
		535, 536, 540, 541, 542, 545, 546, 547, 549, 550, 551, 555, 558, 559,
		560, 561, 562, 565, 566, 567, 575, 576, 580, 585, 587, 589, 596, 597,
		598, 600, 602, 603
	};

public OnFilterScriptInit()
{
	printf("");
	printf("-------------------------------------------------");
	printf(" FCNPC - Fully Customizable NPC citizens FS");
	printf("");
	printf("- Author: OrMisicL");
	printf("- Contributor: ziggi");
	printf("-------------------------------------------------");
	printf("");
	//
	gMaxCitizens = GetConsoleVarAsInt("maxnpc");
	// Set the update rate
	FCNPC_SetUpdateRate(80);

	// Open all nodes
	for (new i = 0; i < FCNPC_MAX_NODES; i++) {
		if (!FCNPC_IsNodeOpen(i) && !FCNPC_OpenNode(i)) {
			printf("Error: Failed to open node %d from (scriptfiles/FCNPC/nodes/NODES%d.DAT)", i, i);
			return 0;
		}
	}
	// Create all the citizens
	new
		name[MAX_PLAYER_NAME + 1],
		npcid,
		nodeid,
		point,
		Float:x, Float:y, Float:z,
		vehicleid,
		vehnodes, pednodes, navinodes;

	for (new i = 0; i < gMaxCitizens; i++) {
		// Generate the citizen name
		format(name, sizeof(name), "citizen_%d", i + 1);

		// Create and spawn the NPC
		npcid = FCNPC_Create(name);
		if (npcid == INVALID_PLAYER_ID) {
			printf("Warning: Failed to create NPC ID %d (%s)", i, name);
			continue;
		}
		gCitizenID[i] = npcid;
		FCNPC_Spawn(npcid, random(299), 0, 0, 0);

		// Generate a random nodeid
		do {
			nodeid = random(FCNPC_MAX_NODES);
		} while (!IsValidZone(nodeid));

		// Get the nodeid info
		FCNPC_GetNodeInfo(nodeid, vehnodes, pednodes, navinodes);

		// Randomize type of path
		if (random(2) == 1 && pednodes != 0) {
			point = random(pednodes - 1) + vehnodes + 1;

			FCNPC_SetNodePoint(nodeid, point);

			FCNPC_PlayNode(npcid, nodeid, FCNPC_MOVE_TYPE_WALK);
		} else {
			point = random(vehnodes - 1);

			FCNPC_SetNodePoint(nodeid, point);

			FCNPC_GetNodePointPosition(nodeid, x, y, z);
			vehicleid = CreateVehicle(GetRandomCarModel(), x, y, z, 0, -1, -1, 1);
			FCNPC_PutInVehicle(npcid, vehicleid, 0);

			FCNPC_PlayNode(npcid, nodeid, .speed = 1.0 + frandom(1.0));
		}
	}
	return 1;
}

public OnFilterScriptExit()
{
	// Delete all NPCs
	new vehicleid;
	for (new i = 0; i < MAX_PLAYERS; i++) {
		if (gCitizenID[i] != INVALID_PLAYER_ID) {
			vehicleid = FCNPC_GetVehicleID(gCitizenID[i]);
			if (vehicleid != 0) {
				DestroyVehicle(vehicleid);
			}
			FCNPC_Destroy(gCitizenID[i]);
			gCitizenID[i] = INVALID_PLAYER_ID;
		}
 	}
	// Close all nodes
	for (new i = 0; i < FCNPC_MAX_NODES; i++) {
		if (FCNPC_IsNodeOpen(i)) {
			FCNPC_CloseNode(i);
		}
	}
	return 1;
}

public OnPlayerCommandText(playerid, cmdtext[])
{
	// Test command
	if (strcmp(cmdtext, "/citizen", true, 9) == 0) {
		new
			param[4],
			npcid;

		strmid(param, cmdtext, 10, strlen(cmdtext));
		if (strlen(param) == 0) {
			SendClientMessage(playerid, 0xB35959AA, "Usage: /citizen <ID>");
			return 1;
		}

		npcid = strval(param);

		if (!FCNPC_IsValid(npcid)) {
			SendClientMessage(playerid, 0xB35959AA, "Invalid citizen ID");
			return 1;
		}

		new Float:x, Float:y, Float:z;
		FCNPC_GetPosition(npcid, x, y, z);
		SetPlayerPos(playerid, x + 4, y + 4, z);
		return 1;
	}
	if (strcmp(cmdtext, "/pause", true, 6) == 0) {
		for (new i = 0; i < MAX_PLAYERS; i++) {
			if (gCitizenID[i] != INVALID_PLAYER_ID) {
				FCNPC_PausePlayingNode(gCitizenID[i]);
			}
		}
		return 1;
	}
	if (strcmp(cmdtext, "/resume", true, 7) == 0) {
		for (new i = 0; i < MAX_PLAYERS; i++) {
			if (gCitizenID[i] != INVALID_PLAYER_ID) {
				FCNPC_ResumePlayingNode(gCitizenID[i]);
			}
		}
		return 1;
	}
	return 0;
}

public FCNPC_OnChangeNode(npcid, newnodeid, oldnodeid)
{
	// Validate the node zone
 	return IsValidZone(newnodeid) ? 1 : 0;
}

stock GetRandomCarModel()
{
	return gCars[ random( sizeof(gCars) ) ];
}

stock IsValidZone(zoneid)
{
#if defined SAN_ANDREAS
	return zoneid > 0 && zoneid < FCNPC_MAX_NODES;
#elseif defined LOS_SANTOS
	return (zoneid >= 5 && zoneid <= 7) || (zoneid >= 12 && zoneid <= 15) || (zoneid >= 20 && zoneid <= 23);
#elseif defined SAN_FIERRO
	return zoneid == 17 || (zoneid >= 24 && zoneid <= 26) || (zoneid >= 32 && zoneid <= 24)  || (zoneid >= 40 && zoneid <= 41);
#elseif defined LAS_VENTURAS
	return (zoneid >= 38 && zoneid <= 39) || (zoneid >= 45 && zoneid <= 47) || (zoneid >= 53 && zoneid <= 55) || (zoneid >= 61 && zoneid <= 63);
#else
	return false;
#endif
}

stock Float:frandom(Float:max)
{
	return floatdiv(float(random(0)), floatdiv(float(cellmax), max));
}
