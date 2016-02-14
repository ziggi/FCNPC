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

#define MAX_BODYGUARDS 6

forward UpdateGuards();

enum EBodyguard
{
	id,
	follow,
	aimat,
	shootat,
};

new Bodyguard[MAX_PLAYERS][MAX_BODYGUARDS][EBodyguard];
new Bodyguards[MAX_PLAYERS];
new LastPlayerId = INVALID_PLAYER_ID;

public OnFilterScriptInit()
{
	printf("");
	printf("-------------------------------------------------");
	printf("     FCNPC - Fully Controllable NPC Filterscript");
	printf("");
	printf("- Author: OrMisicL");
	printf("-------------------------------------------------");
	printf("");
	
	// Set the update rate
	FCNPC_SetUpdateRate(30);
	// Create the guards update timer
 	SetTimer("UpdateGuards", 100, -1);
}

public OnPlayerConnect(playerid)
{
	Bodyguards[playerid] = 0;
	for(new i = 0; i < MAX_BODYGUARDS; i++)
	{
		Bodyguard[playerid][i][id] = INVALID_PLAYER_ID;
		// Reset bodyguard stats
 		Bodyguard[playerid][i][follow] = 0;
 		Bodyguard[playerid][i][aimat] = INVALID_PLAYER_ID;
		Bodyguard[playerid][i][shootat] = INVALID_PLAYER_ID;
	}
	return 1;
}

public OnPlayerDisconnect(playerid)
{
	for(new i = 0; i < Bodyguards[playerid]; i++)
	{
	  	// Delete the bodyguard NPC
	  	FCNPC_Destroy(Bodyguard[playerid][i][id]);
	  	// Free the slot
		Bodyguard[playerid][i][id] = INVALID_PLAYER_ID;
		// Reset bodyguard stats
 		Bodyguard[playerid][i][follow] = 0;
 		Bodyguard[playerid][i][aimat] = INVALID_PLAYER_ID;
		Bodyguard[playerid][i][shootat] = INVALID_PLAYER_ID;
	}
	Bodyguards[playerid] = 0;
	return 1;
}

public UpdateGuards()
{
	// Loop through all the players
	for(new i = 0; i < MAX_PLAYERS; i++)
	{
		// Validate the player
		if(!IsPlayerConnected(i) || !Bodyguards[i])
		    continue;
		    
		// Loop through all the player guards
		for(new j = 0; j < Bodyguards[i]; j++)
		{
			// Process the guard follow
			if(Bodyguard[i][j][follow] == 1)
			{
				// Get the owner position
     			new Float:x, Float:y, Float:z, Float:nx, Float:ny, Float:nz;
		 		GetPlayerPos(i, x, y, z);
		 		// Get the npc position
				FCNPC_GetPosition(Bodyguard[i][j][id], nx, ny, nz);
				// Check if we need to make him follow
				if(!IsPlayerInRangeOfPoint(i, 2.0, nx, ny, nz))
				{
					// Set the moving velocity if he is a driver
					if(FCNPC_GetVehicleID(Bodyguard[i][j][id]) != INVALID_VEHICLE_ID)
					{
				    	FCNPC_GoTo(Bodyguard[i][j][id], x, y, z, MOVE_TYPE_DRIVE, 0.8, true);
				    	if(IsPlayerInAnyVehicle(i))
				    	{
                            new Float:vx, Float:vy, Float:vz;
							GetVehicleVelocity(GetPlayerVehicleID(i), vx, vy, vz);
							//FCNPC_SetVelocity(Bodyguard[i][j][id], vx * 0.1, vy, vz);
				    	}
    				}
    				else
    				    FCNPC_GoTo(Bodyguard[i][j][id], x, y, z, MOVE_TYPE_RUN, 0.0, true);
	    		}
    			else
		   			FCNPC_Stop(Bodyguard[i][j][id]);
			}
			// Process guard aiming
			else if(Bodyguard[i][j][aimat] != INVALID_PLAYER_ID)
			{
   				// Get the target position
   				new Float:x, Float:y, Float:z;
				GetPlayerPos(Bodyguard[i][j][aimat], x, y, z);
				// Aim at him
 				FCNPC_AimAt(Bodyguard[i][j][id], x, y, z, false);
			}
			// Process guard shooting
			else if(Bodyguard[i][j][shootat] != INVALID_PLAYER_ID)
			{
				new Float:hp;
				GetPlayerHealth(Bodyguard[i][j][shootat], hp);
			    // Make sure the target is not already dead
			    if(hp <= 0.0)
			        FCNPC_StopAim(Bodyguard[i][j][id]);
				else
				{
    				// Get the target position
					new Float:x, Float:y, Float:z;
 					GetPlayerPos(Bodyguard[i][j][shootat], x, y, z);
 					// Shoot him
 					FCNPC_AimAt(Bodyguard[i][j][id], x, y, z, true);
 				}
			}
		}
	}
	return 1;
}

public FCNPC_OnCreate(npcid)
{
	new msg[64], name[24];
	GetPlayerName(LastPlayerId, name, 24);
 	format(msg, 64, "Player %s (ID: %d) has created a bodyguard", name, LastPlayerId);
 	SendClientMessageToAll(0xFF0000FF, msg);
	return 1;
}

stock GetNPCBodypart(bodypart, str[], length)
{
	
	switch(bodypart)
	{
		case 3:
	        strmid(str, "torso", 0, 255, length);

 	    case 4:
	        strmid(str, "groin", 0, 255, length);
	        
 	    case 5:
	        strmid(str, "left arm", 0, 255, length);
	        
 	    case 6:
	        strmid(str, "right arm", 0, 255, length);
	        
 	    case 7:
	        strmid(str, "left leg", 0, 255, length);
	        
  	    case 8:
	        strmid(str, "right leg", 0, 255, length);
	        
 	    case 9:
	        strmid(str, "head", 0, 255, length);
	}
}

public FCNPC_OnTakeDamage(npcid, damagerid, weaponid, bodypart, Float:health_loss)
{
	if(!IsPlayerConnected(damagerid) || !IsPlayerConnected(npcid))
	    return 0;
	    
 	new name[MAX_PLAYER_NAME], bp[16];
    GetPlayerName(damagerid, name, MAX_PLAYER_NAME);
    GetNPCBodypart(bodypart, bp, 16);
	new msg[128];
 	format(msg, 128, "NPC(%d) was damaged by %s(%d) with %d in the %s (hp: %f)", npcid, name, damagerid, weaponid, bp, health_loss);
 	SendClientMessageToAll(0xFF0000FF, msg);
 	return 0;
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

public FCNPC_OnVehicleEntryComplete(npcid, vehicleid, seat)
{
	new msg[64];
	format(msg, 64, "NPC %d finished vehicle entry", npcid);
	SendClientMessageToAll(0xFF0000FF, msg);
	return 1;
}

public FCNPC_OnVehicleExitComplete(npcid)
{
	new msg[64];
	format(msg, 64, "NPC %d finished vehicle exit", npcid);
	SendClientMessageToAll(0xFF0000FF, msg);
	return 1;
}

public OnPlayerCommandText(playerid, cmdtext[])
{
	new cmd[128];
	new idx;
	new tmp[64];
	cmd = strtok(cmdtext, idx);
	// Bodyguard command
	if(!strcmp(cmd, "/bg", true))
	{
	    // Get the options
	    tmp = strtok(cmdtext, idx);
		// Check params
		if(!strlen(tmp))
		{
		    SendClientMessage(playerid, COLOR_GREY, "Use: /bodyguard [option]");
		    SendClientMessage(playerid, COLOR_GREY, "add, delete, follow, unfollow, shoot, aim");
		    SendClientMessage(playerid, COLOR_GREY, "reset, enter, exit");
		    return 1;
	 	}
	 	// Add bodyguard
	 	if(!strcmp(tmp, "add", true))
	 	{
			new guards = Bodyguards[playerid];
			// Check the bodyguards count for player
			if(guards == MAX_BODYGUARDS)
	  		{
	    		SendClientMessage(playerid, COLOR_RED, "Maximum bodyguards allowed per player is reached");
		    	return 1;
	  		}
	  		// Format a bodyguard name
	  		new name[24];
	  		format(name, 24, "guard_%d_%d", playerid, guards + 1);
 		 	// Increase the player bodyguards
		 	Bodyguards[playerid]++;
		 	// Save the last player ID
		 	LastPlayerId = playerid;
	  		// Create a new bodyguard NPC
	  		new npc = FCNPC_Create(name);
	  		if(npc == INVALID_PLAYER_ID)
	  		{
	     		SendClientMessage(playerid, COLOR_RED, "Failed to create the bodyguard (no slots available ?)");
				Bodyguards[playerid]--;
				return 1;
	  		}
		 	// Set the bodyguard slot
		 	Bodyguard[playerid][guards][id] = npc;
		 	// Reset bodyguard stats
		 	Bodyguard[playerid][guards][follow] = 0;
		 	Bodyguard[playerid][guards][aimat] = INVALID_PLAYER_ID;
   			Bodyguard[playerid][guards][shootat] = INVALID_PLAYER_ID;
		 	// Get the current player pos
			new Float:x, Float:y, Float:z;
			GetPlayerPos(playerid, x, y, z);
			// Get interior
			new interior = GetPlayerInterior(playerid);
			// Generate a random skin for the bodyguard and spawn him
			FCNPC_Spawn(npc, (random(4) + 163), x + 2, y + 2, z);
			FCNPC_SetPosition(npc, x + 2, y + 2, z);
			FCNPC_SetInterior(npc, interior);
			// Give him some weapons and ammo
			FCNPC_SetWeapon(npc, (random(3) + 22));
			FCNPC_SetAmmo(npc, 400);
			FCNPC_SetArmour(npc, 80);
			return 1;
	 	}
	  	if(!strcmp(tmp, "delete", true))
	 	{
			new guard = Bodyguards[playerid];
			// Check the bodyguards count for player
			if(guard == 0)
	  		{
	    		SendClientMessage(playerid, COLOR_RED, "No bodyguards to delete");
		    	return 1;
	  		}
	  		// Delete the bodyguard NPC
	  		FCNPC_Destroy(Bodyguard[playerid][guard - 1][id]);
		 	// Free the bodyguard slot
		 	Bodyguard[playerid][guard - 1][id] = INVALID_PLAYER_ID;
		 	// Decrease the player bodyguards
		 	Bodyguards[playerid]--;
		 	return 1;
	 	}
	  	if(!strcmp(tmp, "follow", true))
	 	{
			new guards = Bodyguards[playerid];
			// Check the bodyguards count for player
			if(guards == 0)
	  		{
	    		SendClientMessage(playerid, COLOR_RED, "No bodyguards to follow");
		    	return 1;
	  		}
  		 	// Get the guard id
	    	tmp = strtok(cmdtext, idx);
			// Check params
			if(!strlen(tmp))
			{
			    SendClientMessage(playerid, COLOR_GREY, "Use: /bodyguard follow [id]");
			    return 1;
		 	}
	    	if(strval(tmp) < 0 || strval(tmp) > guards)
	    	{
	    		SendClientMessage(playerid, COLOR_RED, "Invalid bodyguard");
		    	return 1;
	    	}
			// Set the bodyguard follow flag
			Bodyguard[playerid][strval(tmp) - 1][follow] = 1;
		 	return 1;
	 	}
  		if(!strcmp(tmp, "unfollow", true))
	 	{
			new guards = Bodyguards[playerid];
			// Check the bodyguards count for player
			if(guards == 0)
	  		{
	    		SendClientMessage(playerid, COLOR_RED, "No bodyguards to unfollow");
		    	return 1;
	  		}
  		 	// Get the guard id
	    	tmp = strtok(cmdtext, idx);
			// Check params
			if(!strlen(tmp))
			{
			    SendClientMessage(playerid, COLOR_GREY, "Use: /bodyguard unfollow [id]");
			    return 1;
		 	}
	    	if(strval(tmp) < 0 || strval(tmp) > guards)
	    	{
	    		SendClientMessage(playerid, COLOR_RED, "Invalid bodyguard");
		    	return 1;
	    	}
			// Reset the bodyguard follow flag
			Bodyguard[playerid][strval(tmp) - 1][follow] = 0;
		 	return 1;
	 	}
  		if(!strcmp(tmp, "aim", true))
	 	{
			new guards = Bodyguards[playerid];
			// Check the bodyguards count for player
			if(guards == 0)
	  		{
	    		SendClientMessage(playerid, COLOR_RED, "No bodyguards to aim");
		    	return 1;
	  		}
  		 	// Get the guard id
	    	tmp = strtok(cmdtext, idx);
			// Check params
			if(!strlen(tmp))
			{
			    SendClientMessage(playerid, COLOR_GREY, "Use: /bodyguard aim [id] [playerid]");
			    return 1;
		 	}
	    	if(strval(tmp) < 0 || strval(tmp) > guards)
	    	{
	    		SendClientMessage(playerid, COLOR_RED, "Invalid bodyguard");
		    	return 1;
	    	}
	    	new bg = strval(tmp) - 1;
			// Get the guard id
	    	tmp = strtok(cmdtext, idx);
			// Check params
			if(!strlen(tmp))
			{
			    SendClientMessage(playerid, COLOR_GREY, "Use: /bodyguard aim [id] [playerid]");
			    return 1;
		 	}
			// Set the bodyguard aiming target
			Bodyguard[playerid][bg][aimat] = strval(tmp);
			Bodyguard[playerid][bg][shootat] = INVALID_PLAYER_ID;
		 	return 1;
	 	}
  		if(!strcmp(tmp, "shoot", true))
	 	{
			new guards = Bodyguards[playerid];
			// Check the bodyguards count for player
			if(guards == 0)
	  		{
	    		SendClientMessage(playerid, COLOR_RED, "No bodyguards to shoot");
		    	return 1;
	  		}
  		 	// Get the guard id
	    	tmp = strtok(cmdtext, idx);
			// Check params
			if(!strlen(tmp))
			{
			    SendClientMessage(playerid, COLOR_GREY, "Use: /bodyguard shoot [id] [playerid]");
			    return 1;
		 	}
	    	if(strval(tmp) < 1 || strval(tmp) > guards)
	    	{
	    		SendClientMessage(playerid, COLOR_RED, "Invalid bodyguard");
		    	return 1;
	    	}
	    	new bg = strval(tmp) - 1;
			// Get the guard id
	    	tmp = strtok(cmdtext, idx);
			// Check params
			if(!strlen(tmp))
			{
			    SendClientMessage(playerid, COLOR_GREY, "Use: /bodyguard shoot [id] [playerid]");
			    return 1;
		 	}
			// Set the bodyguard shooting target
			Bodyguard[playerid][bg][shootat] = strval(tmp);
			Bodyguard[playerid][bg][aimat] = INVALID_PLAYER_ID;
		 	return 1;
	 	}
  		if(!strcmp(tmp, "reset", true))
	 	{
			new guards = Bodyguards[playerid];
			// Check the bodyguards count for player
			if(guards == 0)
	  		{
	    		SendClientMessage(playerid, COLOR_RED, "No bodyguards to reset");
		    	return 1;
	  		}
  		 	// Get the guard id
	    	tmp = strtok(cmdtext, idx);
			// Check params
			if(!strlen(tmp))
			{
			    SendClientMessage(playerid, COLOR_GREY, "Use: /bodyguard reset [id]");
			    return 1;
		 	}
	    	if(strval(tmp) < 0 || strval(tmp) > guards)
	    	{
	    		SendClientMessage(playerid, COLOR_RED, "Invalid bodyguard");
		    	return 1;
	    	}
	    	new bg = strval(tmp) - 1;
			// Reset the bodyguard shooting and aiming target
			Bodyguard[playerid][bg][shootat] = INVALID_PLAYER_ID;
			Bodyguard[playerid][bg][aimat] = INVALID_PLAYER_ID;
		 	return 1;
	 	}
  		if(!strcmp(tmp, "enter", true))
	 	{
			new guards = Bodyguards[playerid];
			// Check the bodyguards count for player
			if(guards == 0)
	  		{
	    		SendClientMessage(playerid, COLOR_RED, "No bodyguards to enter vehicle");
		    	return 1;
	  		}
  		 	// Get the guard id
	    	tmp = strtok(cmdtext, idx);
			// Check params
			if(!strlen(tmp))
			{
			    SendClientMessage(playerid, COLOR_GREY, "Use: /bodyguard enter [id] [vehicleid] [seat]");
			    return 1;
		 	}
	    	if(strval(tmp) < 0 || strval(tmp) > guards)
	    	{
	    		SendClientMessage(playerid, COLOR_RED, "Invalid bodyguard");
		    	return 1;
	    	}
    		new bg = strval(tmp) - 1;
    		tmp = strtok(cmdtext, idx);
			if(!strlen(tmp))
			{
			    SendClientMessage(playerid, COLOR_GREY, "Use: /bodyguard enter [id] [vehicleid] [seat]");
			    return 1;
		 	}
		 	new vehid = strval(tmp);
    		tmp = strtok(cmdtext, idx);
			if(!strlen(tmp))
			{
			    SendClientMessage(playerid, COLOR_GREY, "Use: /bodyguard enter [id] [vehicleid] [seat]");
			    return 1;
		 	}
			// Make the bodyguard enter the vehicle
			FCNPC_EnterVehicle(Bodyguard[playerid][bg][id], vehid, strval(tmp), MOVE_TYPE_RUN);
		 	return 1;
	 	}
  		if(!strcmp(tmp, "exit", true))
	 	{
			new guards = Bodyguards[playerid];
			// Check the bodyguards count for player
			if(guards == 0)
	  		{
	    		SendClientMessage(playerid, COLOR_RED, "No bodyguards to exit vehicle");
		    	return 1;
	  		}
  		 	// Get the guard id
	    	tmp = strtok(cmdtext, idx);
			// Check params
			if(!strlen(tmp))
			{
			    SendClientMessage(playerid, COLOR_GREY, "Use: /bodyguard exit [id]");
			    return 1;
		 	}
	    	if(strval(tmp) < 0 || strval(tmp) > guards)
	    	{
	    		SendClientMessage(playerid, COLOR_RED, "Invalid bodyguard");
		    	return 1;
	    	}
			// Make the bodyguard exit the vehicle
			FCNPC_ExitVehicle(Bodyguard[playerid][strval(tmp) - 1][id]);
		 	return 1;
	 	}
  		if(!strcmp(tmp, "melee", true))
	 	{
			new guards = Bodyguards[playerid];
			// Check the bodyguards count for player
			if(guards == 0)
	  		{
	    		SendClientMessage(playerid, COLOR_RED, "No bodyguards to melee attack");
		    	return 1;
	  		}
  		 	// Get the guard id
	    	tmp = strtok(cmdtext, idx);
			// Check params
			if(!strlen(tmp))
			{
			    SendClientMessage(playerid, COLOR_GREY, "Use: /bodyguard melee [id]");
			    return 1;
		 	}
	    	if(strval(tmp) < 0 || strval(tmp) > guards)
	    	{
	    		SendClientMessage(playerid, COLOR_RED, "Invalid bodyguard");
		    	return 1;
	    	}
	    	// Reset the bodyguard weapon
	    	FCNPC_SetWeapon(Bodyguard[playerid][strval(tmp) - 1][id], 0);
			// Make the bodyguard melee attack
			FCNPC_MeleeAttack(Bodyguard[playerid][strval(tmp) - 1][id], 35);
		 	return 1;
	 	}
  		if(!strcmp(tmp, "stopmelee", true))
	 	{
			new guards = Bodyguards[playerid];
			// Check the bodyguards count for player
			if(guards == 0)
	  		{
	    		SendClientMessage(playerid, COLOR_RED, "No bodyguards to stop melee attack");
		    	return 1;
	  		}
  		 	// Get the guard id
	    	tmp = strtok(cmdtext, idx);
			// Check params
			if(!strlen(tmp))
			{
			    SendClientMessage(playerid, COLOR_GREY, "Use: /bodyguard stopmelee [id]");
			    return 1;
		 	}
	    	if(strval(tmp) < 0 || strval(tmp) > guards)
	    	{
	    		SendClientMessage(playerid, COLOR_RED, "Invalid bodyguard");
		    	return 1;
	    	}
	    	// Set back the bodyguard weapon
	    	FCNPC_SetWeapon(Bodyguard[playerid][strval(tmp) - 1][id], 22);
			// Make the bodyguard stop the melee attack
			FCNPC_StopAttack(Bodyguard[playerid][strval(tmp) - 1][id]);
		 	return 1;
	 	}
 	}
 	return 1;
}



