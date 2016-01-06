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

#define LOS_SANTOS
#define MAX_CITIZIENS   450

public OnFilterScriptInit()
{
	printf("");
	printf("-------------------------------------------------");
	printf("     FCNPC - Fully Customizable NPC citiziens FS");
	printf("");
	printf("- Author: OrMisicL");
	printf("-------------------------------------------------");
	printf("");
	// Set the update rate
	FCNPC_SetUpdateRate(80);
	// Open all nodes
	for(new i = 0; i < MAX_NODES; i++)
	{
	    if(!FCNPC_IsNodeOpen(i) && !FCNPC_OpenNode(i))
 		{
 		    printf("Error: Failed to open node %d from (scriptfiles/FCNPC/Nodes/NODES%d.DAT)", i, i);
 		    return 0;
   		}
	}
	// Create all the citiziens
	for(new i = 0; i < MAX_CITIZIENS; i++)
	{
		// Generate the citizien name
		new name[24];
  		format(name, 24, "citizien_%d", i + 1);
  		// Create and spawn the NPC
		new npcid = FCNPC_Create(name);
		if(npcid == INVALID_PLAYER_ID)
		{
		    printf("Warning: Failed to create NPC ID %d (%s)", i, name);
		    continue;
  		}
		FCNPC_Spawn(npcid, random(299), 0, 0, 0);
		// Generate a random node id
		new node = random(MAX_NODES);
		while(!IsValidZone(node))
		    node = random(MAX_NODES);
		    
		// Get the node info
		new vehnodes, pednodes, navinodes;
		FCNPC_GetNodeInfo(node, vehnodes, pednodes, navinodes);
		// Randomize type of path
		new point;
		if(random(2) == 1 && pednodes != 0)
			// Generate a random node point
			point = random(pednodes - 1) + vehnodes + 1;
	    else
	        point = random(vehnodes - 1);

		// Set the node to the random point
		FCNPC_SetNodePoint(node, point);
		// Create vehicle if its a vehicle node
		if(point < vehnodes)
		{
			// Get the point position
			new Float:x, Float:y, Float:z;
			FCNPC_GetNodePointPosition(node, x, y, z);
			// Create a random vehicle
		    new vehid = CreateVehicle(FCNPC_GetNodeType(node) == NODE_TYPE_VEHICLE ? GetRandomCarModel() : GetRandomBoatModel(), x, y, z, 0, random(127) + 128, random(127) + 128, 1);
		    // Put the NPC into the vehicle
		    FCNPC_PutInVehicle(npcid, vehid, 0);
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
	for(new i = 0; i < MAX_CITIZIENS; i++)
	{
	    if(IsPlayerConnected(i) && IsPlayerNPC(i))
	        FCNPC_Destroy(i);
 	}
	// Close all nodes
	for(new i = 0; i < MAX_NODES; i++)
	{
	    if(FCNPC_IsNodeOpen(i))
			FCNPC_CloseNode(i);
	}
	return 1;
}

public FCNPC_OnChangeNode(playerid, nodeid)
{
	// Validate the node zone
 	return IsValidZone(nodeid) ? 1 : 0;
}

stock GetRandomBoatModel()
{
	// Generate a random model
	new model = random(10);
	// Get the random boat model
	switch(model)
	{
		case 0:return 430;
		case 1:return 446;
		case 2:return 452;
		case 3:return 453;
		case 4:return 454;
		case 5:return 472;
		case 6:return 473;
		case 7:return 484;
		case 8:return 493;
		case 9:return 595;
	}
	return 430;
}

stock GetRandomCarModel()
{
	// Generate a random model
	new model = random(165) + 400;
	// Validate the model
	while(!IsVehicleCar(model))
	    model = random(211) + 400;

	return model;
}
	
stock IsVehicleCar(model)
{
	switch(model)
	{
		case 401:return 1;
		case 402:return 1;
		case 404:return 1;
		case 405:return 1;
		case 409:return 1;
		case 410:return 1;
		case 411:return 1;
		case 412:return 1;
		case 415:return 1;
		case 419:return 1;
		case 420:return 1;
		case 421:return 1;
		case 426:return 1;
		case 429:return 1;
		case 434:return 1;
		case 436:return 1;
		case 438:return 1;
		case 439:return 1;
		case 442:return 1;
		case 445:return 1;
		case 451:return 1;
		case 458:return 1;
		case 466:return 1;
		case 467:return 1;
		case 474:return 1;
		case 475:return 1;
		case 477:return 1;
		case 479:return 1;
		case 480:return 1;
		case 491:return 1;
		case 492:return 1;
		case 496:return 1;
		case 506:return 1;
		case 507:return 1;
		case 516:return 1;
		case 517:return 1;
		case 518:return 1;
		case 526:return 1;
		case 527:return 1;
		case 529:return 1;
		case 533:return 1;
		case 534:return 1;
		case 535:return 1;
		case 536:return 1;
		case 540:return 1;
		case 541:return 1;
		case 542:return 1;
		case 545:return 1;
		case 546:return 1;
		case 547:return 1;
		case 549:return 1;
		case 550:return 1;
		case 551:return 1;
		case 555:return 1;
		case 558:return 1;
		case 559:return 1;
		case 560:return 1;
		case 561:return 1;
		case 562:return 1;
		case 565:return 1;
		case 566:return 1;
		case 567:return 1;
		case 575:return 1;
		case 576:return 1;
		case 580:return 1;
		case 585:return 1;
		case 587:return 1;
		case 589:return 1;
		case 596:return 1;
		case 597:return 1;
		case 598:return 1;
		case 600:return 1;
		case 602:return 1;
		case 603:return 1;
	}
	return 0;
}

stock IsValidZone(zoneid)
{
#if defined SAN_ANDREAS
	if(zoneid > 0 && zoneid < MAX_NODES)
	    return true;
	else
	    return false;
	    
#elseif defined LOS_SANTOS
    if((zoneid >= 5 && zoneid <= 7) || (zoneid >= 12 && zoneid <= 15) || (zoneid >= 20 && zoneid <= 23))
 		return true;
    else
        return false;
        
#elseif defined SAN_FIERRO
    if(zoneid == 17 || (zoneid >= 24 && zoneid <= 26) || (zoneid >= 32 && zoneid <= 24)  || (zoneid >= 40 && zoneid <= 41))
 		return true;
    else
        return false;
        
#elseif defined LAS_VENTURAS
    if((zoneid >= 37 && zoneid <= 39) || (zoneid >= 45 && zoneid <= 47) || (zoneid >= 53 && zoneid <= 55) || (zoneid >= 61 && zoneid <= 63))
 		return true;
    else
        return false;
        
#else
	return false;
#endif
}

strtok(const string[], &index)
{
	new length = strlen(string);
	while ((index < length) && (string[index] <= ' '))
	{
		index++;
	}

	new offset = index;
	new result[20];
	while ((index < length) && (string[index] > ' ') && ((index - offset) < (sizeof(result) - 1)))
	{
		result[index - offset] = string[index];
		index++;
	}
	result[index - offset] = EOS;
	return result;
}

public OnPlayerCommandText(playerid, cmdtext[])
{
	new cmd[128];
	new idx;
	new tmp[64];
	cmd = strtok(cmdtext, idx);
	// Test command
	if(!strcmp(cmd, "/citizien", true))
	{
        tmp = strtok(cmdtext, idx);
        if(!IsPlayerConnected(strval(tmp)))
        {
            SendClientMessage(playerid, 0xB35959AA, "Invalid citizien ID");
            return 1;
        }
		new Float:x, Float:y, Float:z;
		FCNPC_GetPosition(strval(tmp), x, y, z);
		SetPlayerPos(playerid, x + 4, y + 4, z);
 	}
 	return 1;
}



