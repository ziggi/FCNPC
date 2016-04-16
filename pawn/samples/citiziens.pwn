/* ---------------------------------
	 FCNPC Plugin Sample FS

- File: FCNPC.pwn
- Author: OrMisicL
- Contributor: ziggi
---------------------------------*/

#define FILTERSCRIPT

#include <a_samp>
#include <FCNPC>

#define COLOR_GREY 0xAFAFAFAA
#define COLOR_RED  0xB35959AA

#define LOS_SANTOS

new
	gMaxCitiziens,
	gCitizienID[MAX_PLAYERS] = {INVALID_PLAYER_ID, ...},
	gBoats[] = {
		430, 446, 452, 453, 454, 472, 473, 484, 493, 595
	},
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
	printf(" FCNPC - Fully Customizable NPC citiziens FS");
	printf("");
	printf("- Author: OrMisicL");
	printf("-------------------------------------------------");
	printf("");
	//
	gMaxCitiziens = GetConsoleVarAsInt("maxnpc");
	// Set the update rate
	FCNPC_SetUpdateRate(80);
	// Open all nodes
	for (new i = 0; i < MAX_NODES; i++)
	{
		if (!FCNPC_IsNodeOpen(i) && !FCNPC_OpenNode(i))
		{
			printf("Error: Failed to open node %d from (scriptfiles/FCNPC/nodes/NODES%d.DAT)", i, i);
			return 0;
		}
	}
	// Create all the citiziens
	for (new i = 0; i < gMaxCitiziens; i++)
	{
		// Generate the citizien name
		new name[MAX_PLAYER_NAME + 1];
		format(name, sizeof(name), "citizien_%d", i + 1);
		// Create and spawn the NPC
		new npcid = FCNPC_Create(name);
		if (npcid == INVALID_PLAYER_ID)
		{
			printf("Warning: Failed to create NPC ID %d (%s)", i, name);
			continue;
		}
		gCitizienID[i] = npcid;
		FCNPC_Spawn(npcid, random(299), 0, 0, 0);
		// Generate a random node id
		new node = random(MAX_NODES);
		while (!IsValidZone(node))
		{
			node = random(MAX_NODES);
		}

		// Get the node info
		new vehnodes, pednodes, navinodes;
		FCNPC_GetNodeInfo(node, vehnodes, pednodes, navinodes);
		// Randomize type of path
		new point;
		if (random(2) == 1 && pednodes != 0)
		{
			// Generate a random node point
			point = random(pednodes - 1) + vehnodes + 1;
		}
		else
		{
			point = random(vehnodes - 1);
		}

		// Set the node to the random point
		FCNPC_SetNodePoint(node, point);
		// Create vehicle if its a vehicle node
		if (point < vehnodes)
		{
			// Get the point position
			new Float:x, Float:y, Float:z;
			FCNPC_GetNodePointPosition(node, x, y, z);
			// Create a random vehicle
			new model = FCNPC_GetNodeType(node) == NODE_TYPE_VEHICLE ? GetRandomCarModel() : GetRandomBoatModel();
			new vehicleid = CreateVehicle(model, x, y, z, 0, -1, -1, 1);
			// Put the NPC into the vehicle
			FCNPC_PutInVehicle(npcid, vehicleid, 0);
			// Set his moving velocity
			FCNPC_SetVelocity(npcid, 0.6, 0.6, 0.0);
  		}
		// Play the node
		FCNPC_PlayNode(npcid, node, point < vehnodes ? NODE_TYPE_VEHICLE : NODE_TYPE_PED);
	}
	return 1;
}

public OnFilterScriptExit()
{
	// Delete all NPCs
	for (new i = 0; i < MAX_PLAYERS; i++)
	{
		if (gCitizienID[i] != INVALID_PLAYER_ID)
		{
			FCNPC_Destroy(gCitizienID[i]);
			gCitizienID[i] = INVALID_PLAYER_ID;
		}
 	}
	// Close all nodes
	for (new i = 0; i < MAX_NODES; i++)
	{
		if (FCNPC_IsNodeOpen(i))
		{
			FCNPC_CloseNode(i);
		}
	}
	return 1;
}

public OnPlayerCommandText(playerid, cmdtext[])
{
	// Test command
	if (strcmp(cmdtext, "/citizien", true, 9) == 0)
	{
		new
			param[4],
			npcid;

		strmid(param, cmdtext, 10, strlen(cmdtext));
		npcid = strval(param);

		if (!IsPlayerConnected(npcid))
		{
			SendClientMessage(playerid, 0xB35959AA, "Invalid citizien ID");
			return 1;
		}

		new Float:x, Float:y, Float:z;
		FCNPC_GetPosition(npcid, x, y, z);
		SetPlayerPos(playerid, x + 4, y + 4, z);
	}
	return 1;
}

public FCNPC_OnChangeNode(npcid, nodeid)
{
	// Validate the node zone
 	return IsValidZone(nodeid) ? 1 : 0;
}

stock GetRandomBoatModel()
{
	return gBoats[ random( sizeof(gBoats) ) ];
}

stock GetRandomCarModel()
{
	return gCars[ random( sizeof(gCars) ) ];
}

stock IsValidZone(zoneid)
{
#if defined SAN_ANDREAS
	return zoneid > 0 && zoneid < MAX_NODES;
#elseif defined LOS_SANTOS
	return (zoneid >= 5 && zoneid <= 7) || (zoneid >= 12 && zoneid <= 15) || (zoneid >= 20 && zoneid <= 23);
#elseif defined SAN_FIERRO
	return zoneid == 17 || (zoneid >= 24 && zoneid <= 26) || (zoneid >= 32 && zoneid <= 24)  || (zoneid >= 40 && zoneid <= 41);
#elseif defined LAS_VENTURAS
	return (zoneid >= 37 && zoneid <= 39) || (zoneid >= 45 && zoneid <= 47) || (zoneid >= 53 && zoneid <= 55) || (zoneid >= 61 && zoneid <= 63);
#else
	return false;
#endif
}
