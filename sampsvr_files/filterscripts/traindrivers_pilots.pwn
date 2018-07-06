/* ---------------------------------
	 FCNPC Plugin traindrivers_pilots FS

- File: traindrivers_pilots.pwn
- Author: Freaksken
- Description: converts the 3 traindriver NPCs and 3 pilot NPCs from the gl_npcs script to FCNPCs

- Files: train_ls_to_sf.rec, train_sf_to_lv.rec, train_lv_to_ls.rec, at400_ls_to_lv.rec, at400_sf_to_ls.rec, at400_lv_to_sf.rec
- Author: Kalcor
- Description: the recording files needed for playback
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

#define COLOR_STANDARD_NPC						0xffffffff
#define MAX_STANDARD_NPC_TRAINDRIVERS			3
#define MAX_STANDARD_NPC_PILOTS					3
new TrainDriverVehicles[MAX_STANDARD_NPC_TRAINDRIVERS] = {INVALID_VEHICLE_ID, ...};
new PilotVehicles[MAX_STANDARD_NPC_PILOTS] = {INVALID_VEHICLE_ID, ...};
new TrainDriverNPCs[MAX_STANDARD_NPC_TRAINDRIVERS] = {INVALID_PLAYER_ID, ...};
new PilotNPCs[MAX_STANDARD_NPC_PILOTS] = {INVALID_PLAYER_ID, ...};
new TrainDriverPlaybackCycle[MAX_STANDARD_NPC_TRAINDRIVERS] = {0, ...};
new PilotPlaybackCycle[MAX_STANDARD_NPC_PILOTS] = {0, ...};

#if defined FILTERSCRIPT
public OnFilterScriptInit()
{
	TrainDriverVehicles[0] = AddStaticVehicleEx(537, 1462.0745, 2630.8787, 10.8203, 270.0, -1, -1, 0); // freight
	TrainDriverVehicles[1] = AddStaticVehicleEx(537, -1942.7950, 168.4164, 27.0006, 270.0, -1, -1, 0); // freight
	TrainDriverVehicles[2] = AddStaticVehicleEx(537, 1700.7551, -1953.6531, 14.8756, 270.0, -1, -1, 0); // freight
	TrainDriverNPCs[0] = FCNPC_Create("TrainDriverLS");
	TrainDriverNPCs[1] = FCNPC_Create("TrainDriverSF");
	TrainDriverNPCs[2] = FCNPC_Create("TrainDriverLV");
	for(new traindriver = 0; traindriver < MAX_STANDARD_NPC_TRAINDRIVERS; traindriver++) {
		if(TrainDriverNPCs[traindriver] != INVALID_PLAYER_ID && TrainDriverVehicles[traindriver] != INVALID_VEHICLE_ID) {
			FCNPC_Spawn(TrainDriverNPCs[traindriver], 255, 0.0, 0.0, 0.0);
			//SetPlayerTeam(TrainDriverNPCs[traindriver], 69);
			FCNPC_SetInvulnerable(TrainDriverNPCs[traindriver], true);
			SetPlayerColor(TrainDriverNPCs[traindriver], COLOR_STANDARD_NPC);
			FCNPC_PutInVehicle(TrainDriverNPCs[traindriver], TrainDriverVehicles[traindriver], 0);
			TrainDriverPlaybackCycle[traindriver] = traindriver;
			TrainDriverNextPlayback(traindriver);
		}
	}
	PilotVehicles[0] = AddStaticVehicleEx(577, 0.0, 0.0, 0.0, 315.1902, -1, -1, 0); // at400
	PilotVehicles[1] = AddStaticVehicleEx(577, 0.0, 0.0, 0.0, 315.1902, -1, -1, 0); // at400
	PilotVehicles[2] = AddStaticVehicleEx(577, 0.0, 0.0, 0.0, 315.1902, -1, -1, 0); // at400
	PilotNPCs[0] = FCNPC_Create("PilotLS");
	PilotNPCs[1] = FCNPC_Create("PilotSF");
	PilotNPCs[2] = FCNPC_Create("PilotLV");
	for(new pilot = 0; pilot < MAX_STANDARD_NPC_PILOTS; pilot++) {
		if(PilotNPCs[pilot] != INVALID_PLAYER_ID && PilotVehicles[pilot] != INVALID_VEHICLE_ID) {
			FCNPC_Spawn(PilotNPCs[pilot], 61, 0.0, 0.0, 0.0);
			//SetPlayerTeam(PilotNPCs[pilot], 69);
			FCNPC_SetInvulnerable(PilotNPCs[pilot], true);
			SetPlayerColor(PilotNPCs[pilot], COLOR_STANDARD_NPC);
			FCNPC_PutInVehicle(PilotNPCs[pilot], PilotVehicles[pilot], 0);
			PilotPlaybackCycle[pilot] = pilot;
			PilotNextPlayback(pilot);
		}
	}
	return 1;
}

public OnFilterScriptExit()
{
	for(new traindriver = 0; traindriver < MAX_STANDARD_NPC_TRAINDRIVERS; traindriver++) {
		FCNPC_StopPlayingPlayback(TrainDriverVehicles[traindriver]);
		DestroyVehicle(TrainDriverVehicles[traindriver]);
		TrainDriverVehicles[traindriver] = INVALID_VEHICLE_ID;
		FCNPC_Destroy(TrainDriverNPCs[traindriver]);
		TrainDriverNPCs[traindriver] = INVALID_PLAYER_ID;
		TrainDriverPlaybackCycle[traindriver] = 0;
	}
	for(new pilot = 0; pilot < MAX_STANDARD_NPC_PILOTS; pilot++) {
		FCNPC_StopPlayingPlayback(PilotNPCs[pilot]);
		DestroyVehicle(PilotVehicles[pilot]);
		PilotVehicles[pilot] = INVALID_VEHICLE_ID;
		FCNPC_Destroy(PilotNPCs[pilot]);
		PilotNPCs[pilot] = INVALID_PLAYER_ID;
		PilotPlaybackCycle[pilot] = 0;
	}
	return 1;
}
#endif

public OnPlayerConnect(playerid)
{
	//IMPORTANT:
	//This restricts NPCs connecting from an IP address outside this server.
	//If you need to connect NPCs externally you will need to modify the code in this callback.
	/*if(IsPlayerNPC(playerid)) {
		new ip_addr_npc[64 + 1];
		new ip_addr_server[64 + 1];
		GetServerVarAsString("bind", ip_addr_server, sizeof(ip_addr_server));
		GetPlayerIp(playerid, ip_addr_npc, sizeof(ip_addr_npc));

		if(!strlen(ip_addr_server)) {
			ip_addr_server = "127.0.0.1";
		}

		if(strcmp(ip_addr_npc, ip_addr_server, true) && strcmp(npcIP, "255.255.255.255", true)) { //255.255.255.255 happens with FCNPC on gmx
			printf("NPC: Got a remote NPC connecting from %s and I'm kicking it.", ip_addr_npc);
			Kick(playerid);
			return 0;
		}
		printf("NPC: Connection from %s is allowed.", ip_addr_npc);
	}*/
	return 1;
}

public FCNPC_OnFinishPlayback(npcid)
{
	for(new traindriver = 0; traindriver < MAX_STANDARD_NPC_TRAINDRIVERS; traindriver++) {
		if(npcid == TrainDriverNPCs[traindriver]) {
			TrainDriverNextPlayback(traindriver);
			break;
		}
	}
	for(new pilot = 0; pilot < MAX_STANDARD_NPC_PILOTS; pilot++) {
		if(npcid == PilotNPCs[pilot]) {
			PilotNextPlayback(pilot);
			break;
		}
	}
	return 1;
}

public FCNPC_OnVehicleTakeDamage(npcid, issuerid, vehicleid, Float:amount, weaponid, Float:fX, Float:fY, Float:fZ)
{
	for(new traindriver = 0; traindriver < MAX_STANDARD_NPC_TRAINDRIVERS; traindriver++) {
		if(npcid == TrainDriverNPCs[traindriver]) {
			return 0;
		}
	}
	for(new pilot = 0; pilot < MAX_STANDARD_NPC_PILOTS; pilot++) {
		if(npcid == PilotNPCs[pilot]) {
			return 0;
		}
	}
	return 1;
}

TrainDriverNextPlayback(index) {
	switch(TrainDriverPlaybackCycle[index]) {
		case 0: {
			FCNPC_StartPlayingPlayback(TrainDriverNPCs[index], "train_ls_to_sf");
		}
		case 1: {
			FCNPC_StartPlayingPlayback(TrainDriverNPCs[index], "train_sf_to_lv");
		}
		case 2: {
			FCNPC_StartPlayingPlayback(TrainDriverNPCs[index], "train_lv_to_ls");
		}
	}
	TrainDriverPlaybackCycle[index]++;
	if(TrainDriverPlaybackCycle[index] == 3) {
		TrainDriverPlaybackCycle[index] = 0;
	}
}

PilotNextPlayback(index) {
	switch(PilotPlaybackCycle[index]) {
		case 0: {
			FCNPC_StartPlayingPlayback(PilotNPCs[index], "at400_ls_to_lv");
		}
		case 1: {
			FCNPC_StartPlayingPlayback(PilotNPCs[index], "at400_lv_to_sf");
		}
		case 2: {
			FCNPC_StartPlayingPlayback(PilotNPCs[index], "at400_sf_to_ls");
		}
	}
	PilotPlaybackCycle[index]++;
	if(PilotPlaybackCycle[index] == 3) {
		PilotPlaybackCycle[index] = 0;
	}
}
