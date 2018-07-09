/* ---------------------------------
	 FCNPC Plugin missions FS

- File: missions.pwn
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

// General definitions
#define MAX_MISSIONS            10
#define MAX_MISSION_PLAYERS     3
#define MAX_MISSION_VEHICLES    12
#define MAX_MISSION_NPCS        16
#define MAX_MISSION_CUTSCENES   3
#define INVALID_MISSION         (-1)
// NOTE: Each mission will take 3 players and 16 NPCs (19 player slots), so adjust the values above to
// suit your server max players (19 slots per mission / 10 missions: 19 * 10 = 190 slots used ny this script)
// Same goes for vehicles (11 vehicles per mission = 110 vehicle slots used)

forward StartMission(slotid, starterid);
forward EndMission(mission, reason[], bool:success);
forward JoinMission(playerid, mission, slot);
forward LeaveMission(playerid);
forward BeginMission(mission);
forward PlayCutscene(mission);
forward EndCutscene(mission);

// Timers
forward BeginMissionTimer(mission);
forward ProcessMissionTimer(mission);
forward LockCameraTimer(mission, playerid, type);

enum EMission
{
	bool:started,
	players[MAX_MISSION_PLAYERS],
	vehicles[MAX_MISSION_VEHICLES],
	npcs[MAX_MISSION_NPCS],
	npc_target[MAX_MISSION_NPCS],
	player_dead[MAX_MISSION_PLAYERS],
	level,
	bool:cutscene,
	bool:cutscene_process,
	cutscene_stage,
	cutscene_stages[MAX_MISSION_CUTSCENES]
};

new Mission[MAX_MISSIONS][EMission];

new MissionsStarted;

new PlayerMission[MAX_PLAYERS];
new PlayerCreatedMission[MAX_PLAYERS];

// NPCs informations
new Float:MissionSpawns[][4] =
{
	{1463.8781, -1025.8901, 23.8281, 177.3026},
	{1460.9760, -1010.1360, 26.8438, 87.2448},
	{2366.9006, -1653.1716, 13.5469, 89.7455},
	{2367.0542, -1656.4420, 13.3828, 87.8655},
	{2372.5830, -1667.2329, 13.5469, 5.4815},
	{2386.1616, -1654.5337, 13.3828, 101.0490},
	{2390.4465, -1664.4171, 13.5469, 78.4888},
	{2409.3940, -1669.2445, 13.5534, 16.3482},
	{2465.8904, -1669.7217, 13.4757, 83.0888},
	{2466.5623, -1651.0841, 13.4711, 143.5627},
	{2486.7644, -1649.5516, 13.4841, 151.3961},
	{2473.6099, -1680.3300, 13.3624, 42.7424},
	{2494.7471, -1687.3690, 13.5147, 49.8751},
	{2494.2883, -1693.6166, 23.6947, 20.7349},
	{2494.2883, -1693.6166, 23.6947, 20.7349},
	{2482.8660, -1644.3629, 22.8124, 179.9096}
};

// Skin, Weapon, WeaponSkill
new MissionNPCInfo[][3] =
{
	{149, 0, 0},
	{174, 0, 0},
	{174, 22, WEAPONSKILL_PISTOL},
	{174, 28, WEAPONSKILL_MICRO_UZI},
	{174, 24, WEAPONSKILL_DESERT_EAGLE},
	{174, 32, WEAPONSKILL_MICRO_UZI},
	{174, 22, WEAPONSKILL_PISTOL},
	{174, 31, WEAPONSKILL_M4},
	{174, 30, WEAPONSKILL_AK47},
	{174, 30, WEAPONSKILL_AK47},
	{174, 31, WEAPONSKILL_M4},
	{174, 29, WEAPONSKILL_MP5},
	{174, 31, WEAPONSKILL_M4},
	{174, 30, WEAPONSKILL_AK47},
	{174, 34, WEAPONSKILL_SNIPERRIFLE},
	{174, 34, WEAPONSKILL_SNIPERRIFLE}
};

// Players informations
new Float:MissionPlayerAttackPos[][4] =
{
	{2293.7493, -1670.5682, 14.7908, 283.9182},
	{2293.5847, -1673.4008, 14.5787, 281.0981},
	{2293.8721, -1676.2352, 14.3766, 281.0981}
};

new Float:MissionPlayerSpawns[][4] =
{
	{1515.6289, -1058.1167, 25.0625, 40.3945},
	{1512.8232, -1058.4934, 25.0625, 87.2448},
	{1509.7125, -1058.9824, 25.0625, 40.3945}
};

new Float:MissionPlayerEndPos[][4] =
{
	{2494.6973, -1674.1779, 13.3359, 296.0828},
	{2494.1831, -1671.6985, 13.3359, 292.6360},
	{2494.6003, -1668.9293, 13.3359, 273.2092}
};

// Model, Weapon
new MissionPlayerInfo[][2] =
{
	{124, 29},
	{126, 30},
	{122, 31}
};

// Vehicles informations
new Float:MissionVehicles[][4] =
{
	{1463.5126, -1039.6022, 23.3640, 89.8247},
	{1508.3071, -1041.3304, 23.5849, 83.8528},
	{2365.5054, -1655.3934, 13.3611, 180.7228},
	{2368.7273, -1666.5074, 13.4219, 55.3572},
	{2382.8577, -1653.9227, 13.2437, 222.8567},
	{2464.4875, -1654.3873, 13.1641, 174.4650},
	{2481.8472, -1684.1266, 13.1947, 264.2903},
	{2468.4878, -1671.1930, 13.1756, 4.5945},
	{2492.7986, -1651.0204, 13.2758, 85.8759},
	{2431.0161, -1656.5094, 13.1642, 119.9113},
	{2441.7739, -1666.8046, 13.2742, 303.8073},
	{2474.2739, -1678.2626, 13.2145, 104.4158}
};

// Model, color1, color2
new MissionVehicleInfo[][3] =
{
	{411, 1, 0},
	{492, 0, 0},
	{492, 2, 0},
	{566, 6, 0},
	{566, 2, 0},
	{492, 8, 0},
	{566, 0, 0},
	{492, 1, 0},
	{492, 11, 0},
	{492, 7, 0},
	{566, 0, 0},
	{566, 4, 0}
};

// Cutscenes informations
new MissionCutscenes[][160] =
{
	{""},
	{"He is coming out right now, should we assasinate him ?"},
	{"Yes, we will have to shoot him before he enters the vehicle."},
	{"No, i think we should follow him to his gang base"},
	{"He will have full protection there, it will become impossible ~n~for us to kill him"},
	{"What if we kill them all !"},
	{"Thats suicide !"},
	{"No, we are highly trained assasins, we can kill the entire gang"},
	{"What do you guys think ?"},
	{"I started to like the idea, even that it sounds stupid"},
	{"Hmm ... well, should we do it then ?"},
	{"Lets go for it"},
	{""}
};

new Float:MissionCutsceneStart[][4] =
{
	{1515.6289, -1058.1167, 25.0625, 40.3945},
	{1516.2162, -1056.0927, 25.0625, 114.0285},
	{1513.0555, -1055.5886, 25.0625, 241.5564},
	{1513.0546, -1058.7903, 25.0625, 313.9370}
};

// Time, Player
new MissionCutsceneInfo[][2] =
{
	{5000, 0},
	{5000, 0},
	{6000, 1},
	{5000, 2},
	{7000, 1},
	{4000, 2},
	{3000, 1},
	{5000, 2},
	{3000, 0},
	{5000, 1},
	{3000, 2},
	{2000, 0},
	{2000, 0}
};

new Text:TextdrawBox, Text:TextdrawBox2, Text:TextdrawBoxText;
new Text:TextdrawText;
new Text:TextdrawFollow;

public OnFilterScriptInit()
{
	printf("");
	printf("-------------------------------------------------");
	printf("     FCNPC - Fully Controllable NPC");
	printf("");
	printf("- Author: OrMisicL");
	printf("- File: Missions script");
	printf("-------------------------------------------------");
	printf("");
	// Reset missions
	for (new i = 0; i < MAX_PLAYERS; i++)
	{
		PlayerMission[i] = INVALID_MISSION;
		PlayerCreatedMission[i] = INVALID_MISSION;
	}
	for (new i = 0; i < 4; i++)
	{
		Mission[i][started] = false;
		for (new j = 0; j < MAX_MISSION_PLAYERS; j++)
		{
			Mission[i][players][j] = INVALID_PLAYER_ID;
			Mission[i][player_dead][j] = false;
		}
		for (new j = 0; j < MAX_MISSION_NPCS; j++)
		{
			Mission[i][npcs][j] = INVALID_PLAYER_ID;
			Mission[i][npc_target][j] = INVALID_PLAYER_ID;
		}
		Mission[i][level] = 0;
		Mission[i][cutscene] = false;
		Mission[i][cutscene_process] = false;
		Mission[i][cutscene_stage] = 0;
	}
	MissionsStarted = 0;
	// Create texdraws
	TextdrawBox = TextDrawCreate(2.000000, 1.500000, "usebox");
	TextDrawLetterSize(TextdrawBox, 0.000000, 13.040371);
	TextDrawTextSize(TextdrawBox, 637.199951, 0.000000);
	TextDrawAlignment(TextdrawBox, 1);
	TextDrawColor(TextdrawBox, 0);
	TextDrawUseBox(TextdrawBox, true);
	TextDrawBoxColor(TextdrawBox, 255);
	TextDrawSetShadow(TextdrawBox, 0);
	TextDrawSetOutline(TextdrawBox, 0);
	TextDrawFont(TextdrawBox, 0);

	TextdrawBox2 = TextDrawCreate(2.000000, 342.726654, "usebox");
	TextDrawLetterSize(TextdrawBox2, 0.000000, 11.464075);
	TextDrawTextSize(TextdrawBox2, 637.199951, 0.000000);
	TextDrawAlignment(TextdrawBox2, 1);
	TextDrawColor(TextdrawBox2, 0);
	TextDrawUseBox(TextdrawBox2, true);
	TextDrawBoxColor(TextdrawBox2, 255);
	TextDrawSetShadow(TextdrawBox2, 0);
	TextDrawSetOutline(TextdrawBox2, 0);
	TextDrawBackgroundColor(TextdrawBox2, 16777215);
	TextDrawFont(TextdrawBox2, 0);

	TextdrawBoxText = TextDrawCreate(304.800048, 29.866661, "~b~Mission 1:~n~~w~Kill the boss");
	TextDrawLetterSize(TextdrawBoxText, 0.647601, 1.749333);
	TextDrawAlignment(TextdrawBoxText, 2);
	TextDrawColor(TextdrawBoxText, -1);
	TextDrawSetShadow(TextdrawBoxText, 0);
	TextDrawSetOutline(TextdrawBoxText, 1);
	TextDrawBackgroundColor(TextdrawBoxText, 51);
	TextDrawFont(TextdrawBoxText, 1);
	TextDrawSetProportional(TextdrawBoxText, 1);

	TextdrawText = TextDrawCreate(30.000000, 372.400085, "");
	TextDrawLetterSize(TextdrawText, 0.449999, 1.600000);
	TextDrawAlignment(TextdrawText, 1);
	TextDrawColor(TextdrawText, -1);
	TextDrawSetShadow(TextdrawText, 0);
	TextDrawSetOutline(TextdrawText, 1);
	TextDrawBackgroundColor(TextdrawText, 51);
	TextDrawFont(TextdrawText, 1);
	TextDrawSetProportional(TextdrawText, 1);

	TextdrawFollow = TextDrawCreate(323.200042, 414.400054, "");
	TextDrawLetterSize(TextdrawFollow, 0.449999, 1.600000);
	TextDrawAlignment(TextdrawFollow, 2);
	TextDrawColor(TextdrawFollow, -1);
	TextDrawSetShadow(TextdrawFollow, 0);
	TextDrawSetOutline(TextdrawFollow, 1);
	TextDrawBackgroundColor(TextdrawFollow, 51);
	TextDrawFont(TextdrawFollow, 1);
	TextDrawSetProportional(TextdrawFollow, 1);
	return 1;
}

public OnFilterScriptExit()
{
	for (new i = 0; i < MAX_PLAYERS; i++)
	{
		LeaveMission(i);
	}
}

public OnPlayerConnect(playerid)
{
	PlayerMission[playerid] = INVALID_MISSION;
	PlayerCreatedMission[playerid] = INVALID_MISSION;
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
	if (!strcmp(cmd, "/suic", true))
	{
		SetPlayerHealth(playerid, 0.0);
		return 1;
	}
	if (!strcmp(cmd, "/mission", true))
	{
		// Get the options
		tmp = strtok(cmdtext, idx);
		// Check params
		if (!strlen(tmp))
		{
			SendClientMessage(playerid, COLOR_GREY, "Use: /mission [option]");
			SendClientMessage(playerid, COLOR_GREY, "start, join, leave, invite");
			return 1;
		}
		// Start mission
		if (!strcmp(tmp, "start", true))
		{
			if (PlayerMission[playerid] != INVALID_MISSION || PlayerCreatedMission[playerid] != INVALID_MISSION)
			{
				SendClientMessage(playerid, COLOR_RED, "You are already in a mission !");
				return 1;
			}
			// Verify mission slots
			if (MissionsStarted == MAX_MISSIONS)
			{
				SendClientMessage(playerid, COLOR_RED, "No mission slots available !");
				return 1;
			}
			// Get a free mission slot
			new slot = FindFreeMissionSlot();
			if (slot == INVALID_MISSION)
			{
				SendClientMessage(playerid, COLOR_RED, "No mission slots available !");
				return 1;
			}
			// Start a new mission
			if (!StartMission(slot, playerid))
			{
				SendClientMessage(playerid, COLOR_RED, "Failed to start the mission !");
				return 1;
			}
			// Join the mission
			JoinMission(playerid, slot, 0);
			// Send confirmation message
			new msg[64];
			format(msg, sizeof(msg), "You have successfully created a mission (ID: %d)", slot);
			SendClientMessage(playerid, COLOR_GREY, msg);
			SendClientMessage(playerid, COLOR_GREY, "Use </mission invite> to invite players to it");
			return 1;
		}

		// Join mission
		if (!strcmp(tmp, "join", true))
		{
			if (PlayerMission[playerid] != INVALID_MISSION || PlayerCreatedMission[playerid] != INVALID_MISSION)
			{
				SendClientMessage(playerid, COLOR_RED, "You are already in a mission !");
				return 1;
			}
			// Verify mission slots
			if (MissionsStarted == 0)
			{
				SendClientMessage(playerid, COLOR_RED, "No missions available !");
				return 1;
			}
			// Get the mission id
			tmp = strtok(cmdtext, idx);
			if (!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_GREY, "Use: /mission join [id]");
				return 1;
			}
			// Get a free mission player slot
			new slot = FindFreeMissionPlayerSlot(strval(tmp));
			if (slot == INVALID_MISSION)
			{
				SendClientMessage(playerid, COLOR_RED, "No players slots available for that mission !");
				return 1;
			}
			// Join the mission
			JoinMission(playerid, strval(tmp), slot);
			return 1;
		}

		// Leave mission
		if (!strcmp(tmp, "leave", true))
		{
			if (PlayerMission[playerid] == INVALID_MISSION)
			{
				SendClientMessage(playerid, COLOR_RED, "You are not in a mission !");
				return 1;
			}
			// Leave the mission
			LeaveMission(playerid);
			SendClientMessage(playerid, COLOR_GREY, "You have left the mission");
			return 1;
		}

		// Invite to mission
		if (!strcmp(tmp, "invite", true))
		{
			if (PlayerMission[playerid] == INVALID_MISSION || PlayerCreatedMission[playerid] == INVALID_MISSION)
			{
				SendClientMessage(playerid, COLOR_RED, "You haven't created any mission !");
				return 1;
			}
			// Get the mission id
			tmp = strtok(cmdtext, idx);
			if (!strlen(tmp))
			{
				SendClientMessage(playerid, COLOR_GREY, "Use: /mission invite [id]");
				return 1;
			}
			// Validate the player
			if (!IsPlayerConnected(strval(tmp)))
			{
				SendClientMessage(playerid, COLOR_RED, "Invalid player !");
				return 1;
			}
			// Invite the player
			new msg[128], name[MAX_PLAYER_NAME + 1];
			GetPlayerName(playerid, name, sizeof(name));
			format(msg, sizeof(msg), "%s has invited you to join his mission", name);
			SendClientMessage(strval(tmp), COLOR_GREY, msg);
			format(msg, sizeof(msg), "Use </mission join %d> to join it.", PlayerMission[playerid]);
			SendClientMessage(strval(tmp), COLOR_GREY, msg);
			return 1;
		}
	}
	return 0;
}

public FCNPC_OnVehicleEntryComplete(npcid, vehicleid, seatid)
{
	// Get the NPC mission
	new mission = GetNPCMission(npcid);
	if (mission == INVALID_MISSION)
	{
		return 1;
	}

	if (Mission[mission][level] == 1)
	{
		// Stop the cutscene
		EndCutscene(mission);
		// Start the playback
		FCNPC_StartPlayingPlayback(Mission[mission][npcs][1], "mission_1");
	}
	return 1;
}

public FCNPC_OnVehicleExitComplete(npcid, vehicleid)
{
	// Get the NPC mission
	new mission = GetNPCMission(npcid);
	if (mission == INVALID_MISSION)
	{
		return 1;
	}

	if (Mission[mission][level] == 4)
	{
		PlayCutscene(mission);
	}
	return 1;
}

public FCNPC_OnFinishPlayback(npcid)
{
	// Get the NPC mission
	new mission = GetNPCMission(npcid);
	if (mission == INVALID_MISSION)
	{
		return 1;
	}

	if (Mission[mission][level] == 2 || Mission[mission][level] == 3 || Mission[mission][level] == 4 || Mission[mission][level] == 5)
		// Stop the cutscene
		EndCutscene(mission);

	return 1;
}

stock FindFreeMissionSlot()
{
	for (new i = 0; i < MAX_MISSIONS; i++)
	{
		if (!Mission[i][started])
		{
			return i;
		}
	}
	return INVALID_MISSION;
}

stock FindFreeMissionPlayerSlot(mission)
{
	for (new i = 0; i < MAX_MISSION_PLAYERS; i++)
	{
		if (Mission[mission][players][i] == INVALID_PLAYER_ID)
		{
			return i;
		}
	}
	return INVALID_MISSION;
}

stock GetMissionPlayers(mission)
{
	new p = 0;
	if (mission == INVALID_MISSION)
	{
		return p;
	}
	for (new i = 0; i < MAX_MISSION_PLAYERS; i++)
	{
		if (Mission[mission][players][i] != INVALID_PLAYER_ID)
		{
			p++;
		}
	}
	return p;
}

stock GetPlayerMissionSlot(playerid)
{
	new mission = PlayerMission[playerid];
	if (mission == INVALID_MISSION)
	{
		return INVALID_PLAYER_ID;
	}
	for (new i = 0; i < MAX_MISSION_PLAYERS; i++)
	{
		if (Mission[mission][players][i] == playerid)
		{
			return i;
		}
	}
	return INVALID_PLAYER_ID;
}

stock IsAnyPlayerInRange(mission, npc)
{
	new Float:x, Float:y, Float:z;
	FCNPC_GetPosition(npc, x, y, z);
	new bool:range = false;
	for (new i = 0; i < MAX_MISSION_PLAYERS; i++)
	{
		if (IsPlayerConnected(Mission[mission][players][i]) && IsPlayerInRangeOfPoint(Mission[mission][players][i], 50.0, x, y, z))
		{
			range = true;
			break;
		}
	}
	return range;
}

stock GetRandomMissionPlayerToShoot(mission, npc)
{
	if (!IsAnyPlayerInRange(mission, npc))
		return INVALID_PLAYER_ID;

	new Float:x, Float:y, Float:z;
	FCNPC_GetPosition(npc, x, y, z);
	// Choose a random id
	new id = random(MAX_MISSION_PLAYERS);
	// Check if he is in range
	while (!IsPlayerConnected(Mission[mission][players][id]) || !IsPlayerInRangeOfPoint(Mission[mission][players][id], 50.0, x, y, z))
	{
		id = random(MAX_MISSION_PLAYERS);
	}

	return id;
}

stock MarkDead(playerid, mission)
{
	for (new i = 0; i < MAX_MISSION_PLAYERS; i++)
	{
		if (IsPlayerConnected(Mission[mission][players][i]) && Mission[mission][players][i] == playerid)
		{
			Mission[mission][player_dead][i] = true;
			break;
		}
	}
}

stock GetRandomMissionPlayerSpectace(mission)
{
	new bool:alive = false;
	for (new i = 0; i < MAX_MISSION_PLAYERS; i++)
	{
		if (IsPlayerConnected(Mission[mission][players][i]) && !Mission[mission][player_dead][i])
		{
			printf("found that %d", Mission[mission][players][i]);
			alive = true;
			break;
		}
	}
	if (!alive)
	{
		return INVALID_PLAYER_ID;
	}

	new player = random(MAX_MISSION_PLAYERS);
	while (!IsPlayerConnected(Mission[mission][players][player]) || Mission[mission][player_dead][player])
	{
		player = random(MAX_MISSION_PLAYERS);
	}

	return Mission[mission][players][player];
}

stock GetNPCMission(npcid)
{
	// Validate the NPC
	if (!FCNPC_IsSpawned(npcid))
	{
		return INVALID_MISSION;
	}

	for (new i = 0; i < MAX_MISSIONS; i++)
	{
		for (new j = 0; j < MAX_MISSION_NPCS; j++)
		{
			if (Mission[i][npcs][j] == npcid)
			{
				return i;
			}
		}
	}
	return INVALID_MISSION;
}

stock GetNPCAlive(mission)
{
	new a = 0;
	for (new i = 0; i < MAX_MISSION_NPCS; i++)
	{
		if (!FCNPC_IsDead(Mission[mission][npcs][i]))
		{
			a++;
		}
	}
	return a;
}

new missiontimer[MAX_MISSIONS];

public BeginMissionTimer(mission)
{
	// Begin the mission
	BeginMission(mission);
	// Loop through all the players
	for (new i = 0; i < MAX_MISSION_PLAYERS; i++)
	{
		GameTextForPlayer(Mission[mission][players][i], "~b~Mission started.", 3000, 0);
	}

	// Start the actual mission timer
	missiontimer[mission] = SetTimerEx("ProcessMissionTimer", 100, true, "i", mission);
	return 1;
}

new LastCutsceneTick;
new LastBossTick;

public ProcessMissionTimer(mission)
{
	// Process cutscene
	if (Mission[mission][cutscene] && Mission[mission][cutscene_process])
	{
		// Process cutscene from its level
		if (Mission[mission][level] == 0)
		{
			// Get the stage
			new stage = Mission[mission][cutscene_stage];
			// End the cutscene if we finished all its stages
			if (stage >= Mission[mission][cutscene_stages][Mission[mission][level]])
			{
				// End the cutscene
				EndCutscene(mission);
			}
			else
			{
				// Do we need to change the text ?
				if ((GetTickCount() - LastCutsceneTick) >= MissionCutsceneInfo[stage][0])
				{
					Mission[mission][cutscene_stage]++;
					// Reset the player animation
					if (stage > 0)
					{
						// Get the playerid
						new lplayer = MissionCutsceneInfo[stage-1][1];
						new lplayerid = Mission[mission][players][lplayer];
						ClearAnimations(lplayerid);
					}
					// Get the playerid
					new player = MissionCutsceneInfo[stage][1];
					new playerid = Mission[mission][players][player];
					// Make him talk
					ApplyAnimation(playerid, "PED", "IDLE_CHAT", 4.1 , 1, 1, 1, 1, 0);
					// Format the sending text
					new msg[190], name[MAX_PLAYER_NAME + 1];
					GetPlayerName(playerid, name, sizeof(name));
					format(msg, sizeof(msg), "~g~%s: ~w~%s", name, MissionCutscenes[stage]);
					// Set textdraw for all the mission players
					TextDrawSetString(TextdrawText, msg);
					// Update stage
					LastCutsceneTick = GetTickCount();
				}
			}
		}
	}
	// Process NPCs
	if (Mission[mission][level] != 6)
	{
		for (new i = 2; i < MAX_MISSION_NPCS; i++)
		{
			// Validate the NPC
			if (Mission[mission][npcs][i] != INVALID_PLAYER_ID && FCNPC_IsSpawned(Mission[mission][npcs][i]) && !FCNPC_IsDead(Mission[mission][npcs][i]))
			{
				// Is he not shooting ?
				if (Mission[mission][npc_target][i] == INVALID_PLAYER_ID)
				{
					// Get a random mission player to shoot
					new target = GetRandomMissionPlayerToShoot(mission, Mission[mission][npcs][i]);
					if (target != INVALID_PLAYER_ID)
					{
						// Get target position
						new Float:x, Float:y, Float:z;
						GetPlayerPos(target, x, y, z);
						// Shoot him
						FCNPC_AimAt(Mission[mission][npcs][i], x, y, z, true);
						// Mark as shooting
						Mission[mission][npc_target][i] = target;
					}
				}
				else
				{
					// Get NPC position
					new Float:x, Float:y, Float:z;
					FCNPC_GetPosition(Mission[mission][npcs][i], x, y, z);
					// Validate the target
					if (IsPlayerInRangeOfPoint(Mission[mission][npc_target][i], 20.0, x, y, z))
					{
						// Shoot him
						GetPlayerPos(Mission[mission][npc_target][i], x, y, z);
						FCNPC_AimAt(Mission[mission][npcs][i], x, y, z, true);
					}
					else
					{
						FCNPC_StopAim(Mission[mission][npcs][i]);
						Mission[mission][npc_target][i] = INVALID_PLAYER_ID;
					}
				}
			}
		}
	}
	// Process following
	if (Mission[mission][level] == 2)
	{
		new Float:x, Float:y, Float:z;
		FCNPC_GetPosition(Mission[mission][npcs][0], x, y, z);
		// Process players
		for (new i = 0; i < MAX_MISSION_PLAYERS; i++)
		{
			if (!IsPlayerConnected(Mission[mission][players][i]))
			{
				continue;
			}

			new Float:distance = GetPlayerDistanceFromPoint(Mission[mission][players][i], x, y, z);
			if (distance > 150.0)
			{
				// End the mission
				EndMission(mission, "You lost the target", false);
				break;
			}
			else if (distance > 110.0)
			{
				TextDrawSetString(TextdrawFollow, "~r~Status:~n~~w~You're loosing the target");
			}
			else if (distance < 25.0)
			{
				// End the mission
				EndMission(mission, "You got too close", false);
				break;
			}
			else if (distance < 55.0)
			{
				TextDrawSetString(TextdrawFollow, "~r~Status:~n~~w~The target has started to feel your presence");
			}
			else
			{
				TextDrawSetString(TextdrawFollow, "~r~Status:~n~~w~Safe");
			}
		}
	}
	// Process boss fighting
	if (Mission[mission][level] == 6)
	{
		// Is he not shooting ?
		if (Mission[mission][npc_target][0] == INVALID_PLAYER_ID || Mission[mission][player_dead][Mission[mission][npc_target][0]] ||
			(GetTickCount() - LastBossTick) >= 4000)
		{
			// Get a random mission player to shoot
			new target = GetRandomMissionPlayerToShoot(mission, Mission[mission][npcs][0]);
			if (target != INVALID_PLAYER_ID)
			{
				// Get target position
				new Float:x, Float:y, Float:z;
				GetPlayerPos(target, x, y, z);
				// Shoot him
				FCNPC_AimAt(Mission[mission][npcs][0], x, y, z, true);
				// Mark as shooting
				Mission[mission][npc_target][0] = target;
				LastBossTick = GetTickCount();
			}
		}
		else
		{
			// Get NPC position
			new Float:x, Float:y, Float:z;
			FCNPC_GetPosition(Mission[mission][npcs][0], x, y, z);
			// Validate the target
			if (IsPlayerInRangeOfPoint(Mission[mission][npc_target][0], 20.0, x, y, z))
			{
				// Shoot him
				GetPlayerPos(Mission[mission][npc_target][0], x, y, z);
				FCNPC_AimAt(Mission[mission][npcs][0], x, y, z, true);
			}
			else
			{
				FCNPC_StopAim(Mission[mission][npcs][0]);
				Mission[mission][npc_target][0] = INVALID_PLAYER_ID;
			}
		}
	}
	return 1;
}

public StartMission(slotid, starterid)
{
	// Create mission NPCs
	for (new i = 0; i < MAX_MISSION_NPCS; i++)
	{
		// Generate a name
		new name[24];
		format(name, sizeof(name), "mission_%d_%d", slotid, i);
		// Create the NPC
		Mission[slotid][npcs][i] = FCNPC_Create(name);
		if (Mission[slotid][npcs][i] == INVALID_PLAYER_ID)
		{
			return false;
		}

		// Spawn the NPC and set his angle
		FCNPC_Spawn(Mission[slotid][npcs][i], MissionNPCInfo[i][0], MissionSpawns[i][0], MissionSpawns[i][1], MissionSpawns[i][2]);
		FCNPC_SetAngle(Mission[slotid][npcs][i], MissionSpawns[i][3]);
		// Give him a weapon
		FCNPC_SetWeapon(Mission[slotid][npcs][i], MissionNPCInfo[i][1]);
		FCNPC_SetAmmo(Mission[slotid][npcs][i], 10000);
		SetPlayerSkillLevel(Mission[slotid][npcs][i], MissionNPCInfo[i][2], 1);
		// Set him to the mission virtual world
		SetPlayerVirtualWorld(Mission[slotid][npcs][i], slotid + 1);
	}
	// Create mission vehicles
	for (new i = 0; i < MAX_MISSION_VEHICLES; i++)
	{
		Mission[slotid][vehicles][i] = CreateVehicle(MissionVehicleInfo[i][0], MissionVehicles[i][0], MissionVehicles[i][1],
			MissionVehicles[i][2], MissionVehicles[i][3], MissionVehicleInfo[i][1], MissionVehicleInfo[i][2], -1);

		// Set it to the mission virtual world
		SetVehicleVirtualWorld(Mission[slotid][vehicles][i], slotid + 1);
	}
	// Put player into vehicle
	FCNPC_PutInVehicle(Mission[slotid][npcs][1], Mission[slotid][vehicles][0], 0);
	FCNPC_SetAngle(Mission[slotid][npcs][1], MissionVehicles[0][3]);
	// Mark mission as started and initialize its values
	Mission[slotid][started] = true;
	Mission[slotid][players][0] = starterid;
	// Increase missions count
	MissionsStarted++;
	// Set player created mission
	PlayerCreatedMission[starterid] = slotid;
	return true;
}

public EndMission(mission, reason[], bool:success)
{
	if (mission == INVALID_MISSION)
	{
		return;
	}
	KillTimer(missiontimer[mission]);
	FreeCamera(mission);
	// Reset missions
	Mission[mission][started] = false;
	for (new j = 0; j < MAX_MISSION_PLAYERS; j++)
	{
		if (!IsPlayerConnected(Mission[mission][players][j]))
		{
			continue;
		}

		TextDrawHideForPlayer(Mission[mission][players][j], TextdrawBox);
		TextDrawHideForPlayer(Mission[mission][players][j], TextdrawBox2);
		TextDrawHideForPlayer(Mission[mission][players][j], TextdrawBoxText);
		TextDrawHideForPlayer(Mission[mission][players][j], TextdrawText);
		TextDrawHideForPlayer(Mission[mission][players][j], TextdrawFollow);
		TogglePlayerSpectating(Mission[mission][players][j], 0);
		TogglePlayerControllable(Mission[mission][players][j], 1);
		SetPlayerVirtualWorld(Mission[mission][players][j], 0);
		if (!success)
		{
			new reas[164];
			format(reas, sizeof(reas), "~r~Mission failed !~n~~w~%s", reason);
			GameTextForPlayer(Mission[mission][players][j], reas, 5000, 0);
		}
		else
		{
			GameTextForPlayer(Mission[mission][players][j], reason, 5000, 0);
		}

		PlayerMission[Mission[mission][players][j]] = INVALID_MISSION;
		PlayerCreatedMission[Mission[mission][players][j]] = INVALID_MISSION;
		Mission[mission][players][j] = INVALID_PLAYER_ID;
		Mission[mission][player_dead][j] = false;
	}
	for (new j = 0; j < MAX_MISSION_NPCS; j++)
	{
		FCNPC_Destroy(Mission[mission][npcs][j]);
		Mission[mission][npcs][j] = INVALID_PLAYER_ID;
		Mission[mission][npc_target][j] = INVALID_PLAYER_ID;
	}
	for (new j = 0; j < MAX_MISSION_VEHICLES; j++)
	{
		DestroyVehicle(Mission[mission][vehicles][j]);
	}
	Mission[mission][level] = 0;
	Mission[mission][cutscene] = false;
	Mission[mission][cutscene_process] = false;
	Mission[mission][cutscene_stage] = 0;
	// Decrease missions count
	MissionsStarted--;
}

public JoinMission(playerid, mission, slot)
{
	// Set the player mission
	PlayerMission[playerid] = mission;
	Mission[mission][players][slot] = playerid;
	// Set the player position and skin
	SetPlayerSkin(playerid, MissionPlayerInfo[slot][0]);
	SetPlayerPos(playerid, MissionPlayerSpawns[slot][0], MissionPlayerSpawns[slot][1], MissionPlayerSpawns[slot][2]);
	SetPlayerFacingAngle(playerid, MissionPlayerSpawns[slot][3]);
	// Set the player camera position at the mission start
	SetPlayerCameraPos(playerid, 1522.93, -1050.27, 26.27);
	SetPlayerCameraLookAt(playerid, 1518.88, -1053.20, 25.89);
	TogglePlayerControllable(playerid, 0);
	// Give him a weapon
	GivePlayerWeapon(playerid, MissionPlayerInfo[slot][1], 5000);
	// Set his virtual world
	SetPlayerVirtualWorld(playerid, mission + 1);
	// Display message
	if (GetMissionPlayers(mission) < MAX_MISSION_PLAYERS)
	{
		GameTextForPlayer(playerid, "~w~Joined mission ~n~~r~Waiting for players ...", 4000, 0);
	}
	else
	{
		GameTextForPlayer(playerid, "~w~Joined mission ~n~~b~Starting ...", 4000, 0);
		SetTimerEx("BeginMissionTimer", 5000, 0, "i", mission);
	}
	// Notify others about the player join
	new msg[64], name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof(name));
	format(msg, sizeof(msg), "~b~%s~n~~w~has joined the mission", name);
	for (new i = 0; i < MAX_MISSION_PLAYERS; i++)
	{
		if (!IsPlayerConnected(Mission[mission][players][i]) || i == slot)
		{
			continue;
		}

		GameTextForPlayer(Mission[mission][players][i], msg, 3000, 3);
	}
}

public LeaveMission(playerid)
{
	// Get the player slot in the mission
	new slot = GetPlayerMissionSlot(playerid);
	// Reset the player mission
	new mission = PlayerMission[playerid];
	if (mission != INVALID_MISSION)
	{
		Mission[mission][players][slot] = INVALID_PLAYER_ID;
		Mission[mission][player_dead][slot] = false;
	}
	PlayerMission[playerid] = INVALID_MISSION;
	PlayerCreatedMission[playerid] = INVALID_MISSION;
	// Reset player stats
	TogglePlayerSpectating(playerid, 0);
	SetCameraBehindPlayer(playerid);
	TogglePlayerControllable(playerid, 1);
	TextDrawHideForPlayer(playerid, TextdrawBox);
	TextDrawHideForPlayer(playerid, TextdrawText);
	TextDrawHideForPlayer(playerid, TextdrawBox2);
	TextDrawHideForPlayer(playerid, TextdrawBoxText);
	TextDrawHideForPlayer(playerid, TextdrawFollow);
	ClearAnimations(playerid);
	SetPlayerVirtualWorld(playerid, 0);
	// Check the players available
	if (GetMissionPlayers(mission) <= 1)
	{
		EndMission(mission, "not enough players", false);
	}
	else
	{
		// Notify others about the player leave
		new msg[64], name[MAX_PLAYER_NAME + 1];
		GetPlayerName(playerid, name, sizeof(name));
		format(msg, sizeof(msg), "~b~%s~n~~w~has left the mission", name);
		for (new i = 0; i < MAX_MISSION_PLAYERS; i++)
		{
			if (!IsPlayerConnected(Mission[PlayerMission[playerid]][players][i]) || Mission[PlayerMission[playerid]][players][i] == playerid)
			{
				continue;
			}

			GameTextForPlayer(Mission[PlayerMission[playerid]][players][i], msg, 3000, 3);
		}
	}
}

public BeginMission(mission)
{
	// Start the first cutscene
	PlayCutscene(mission);
}

public PlayCutscene(mission)
{
	// Play the mission cutsecene depending on the level
	if (Mission[mission][level] == 0)
	{
		for (new i = 0; i < MAX_MISSION_PLAYERS; i++)
		{
			// Set him to his starting position
			SetPlayerPos(Mission[mission][players][i], MissionCutsceneStart[i][0], MissionCutsceneStart[i][1], MissionCutsceneStart[i][2]);
			SetPlayerFacingAngle(Mission[mission][players][i], MissionCutsceneStart[i][3]);
			SetPlayerCameraPos(Mission[mission][players][i], 1522.28, -1052.47, 27.05);
			SetPlayerCameraLookAt(Mission[mission][players][i], 1517.86, -1054.71, 26.41);
			TogglePlayerControllable(Mission[mission][players][i], 0);
			// Show the textdraw box for the player
			TextDrawShowForPlayer(Mission[mission][players][i], TextdrawBox);
			TextDrawShowForPlayer(Mission[mission][players][i], TextdrawText);
			TextDrawShowForPlayer(Mission[mission][players][i], TextdrawBox2);
			TextDrawShowForPlayer(Mission[mission][players][i], TextdrawBoxText);
			Mission[mission][cutscene_stages][0] = 13;
			Mission[mission][cutscene_process] = true;
		}
	}
	else if (Mission[mission][level] == 1)
	{
		for (new i = 0; i < MAX_MISSION_PLAYERS; i++)
		{
			SetPlayerCameraPos(Mission[mission][players][i], 1479.23, -1024.99, 31.18);
			SetPlayerCameraLookAt(Mission[mission][players][i], 1475.30, -1027.52, 29.42);
		}
		// Make the first player enter the vehicle
		FCNPC_EnterVehicle(Mission[mission][npcs][0], Mission[mission][vehicles][0], 1, FCNPC_MOVE_TYPE_WALK);
		Mission[mission][cutscene_stages][1] = 0;
		Mission[mission][cutscene_process] = false;
	}
	else if (Mission[mission][level] == 3)
	{
		// Start the playback
		FCNPC_StartPlayingPlayback(Mission[mission][npcs][1], "mission_2");
		for (new i = 0; i < MAX_MISSION_PLAYERS; i++)
		{
			TextDrawHideForPlayer(Mission[mission][players][i], TextdrawFollow);
			RemovePlayerFromVehicle(Mission[mission][players][i]);
			TogglePlayerControllable(Mission[mission][players][i], 0);
			SetPlayerPos(Mission[mission][players][i], MissionPlayerAttackPos[i][0], MissionPlayerAttackPos[i][1], MissionPlayerAttackPos[i][2]);
			SetPlayerFacingAngle(Mission[mission][players][i], MissionPlayerAttackPos[i][3]);
			// Lock their vehicle
			SetVehicleParamsForPlayer(Mission[mission][vehicles][1], Mission[mission][players][i], 0, 1);
		}
		// Lock camera on player
		LockCameraOn(mission, Mission[mission][npcs][0], 0);
		Mission[mission][cutscene_process] = false;
	}
	else if (Mission[mission][level] == 4)
	{
		FCNPC_StartPlayingPlayback(Mission[mission][npcs][0], "mission_3");
	}
	else if (Mission[mission][level] == 5)
	{
		SetPlayerVirtualWorld(Mission[mission][npcs][0], mission + 1);
		FCNPC_SetArmour(Mission[mission][npcs][0], 100.0);
		FCNPC_SetWeapon(Mission[mission][npcs][0], 38);
		FCNPC_SetAmmo(Mission[mission][npcs][0], 20000);
		FCNPC_StartPlayingPlayback(Mission[mission][npcs][0], "mission_4");
		LockCameraOn(mission, Mission[mission][npcs][0], 1);
	}
	else if (Mission[mission][level] == 6)
	{
		for (new i = 0; i < MAX_MISSION_PLAYERS; i++)
		{
			TogglePlayerSpectating(Mission[mission][players][i], 0);
			SetPlayerPos(Mission[mission][players][i], MissionPlayerEndPos[i][0], MissionPlayerEndPos[i][1], MissionPlayerEndPos[i][2]);
			SetPlayerFacingAngle(Mission[mission][players][i], MissionPlayerEndPos[i][2]);
		}
		EndMission(mission, "~b~mission accomplished", true);
	}
	// Start the tick count
	LastCutsceneTick = GetTickCount();
	// Mark cutscene as started
	Mission[mission][cutscene_stage] = 1;
	Mission[mission][cutscene] = true;
}

public EndCutscene(mission)
{
	// Mark cutscene as not started
	Mission[mission][cutscene_stage] = 0;
	Mission[mission][cutscene] = false;
	Mission[mission][cutscene_process] = false;
	// Increase the mission level
	Mission[mission][level]++;
	// Start the next cutscene if we should
	if (Mission[mission][level] - 1 == 0)
	{
		for (new i = 0; i < MAX_MISSION_PLAYERS; i++)
		{
			ClearAnimations(Mission[mission][players][i], 1);
			// Hide the textdraws for the player
			TextDrawHideForPlayer(Mission[mission][players][i], TextdrawBox);
			TextDrawHideForPlayer(Mission[mission][players][i], TextdrawText);
			TextDrawHideForPlayer(Mission[mission][players][i], TextdrawBox2);
			TextDrawHideForPlayer(Mission[mission][players][i], TextdrawBoxText);
		}
		PlayCutscene(mission);
	}
	else if (Mission[mission][level] - 1 == 1)
	{
		for (new i = 0; i < MAX_MISSION_PLAYERS; i++)
		{
			SetCameraBehindPlayer(Mission[mission][players][i]);
			TogglePlayerControllable(Mission[mission][players][i], 1);
			GameTextForPlayer(Mission[mission][players][i], "~r~Follow him !", 2500, 4);
			TextDrawShowForPlayer(Mission[mission][players][i], TextdrawFollow);
		}
	}
	else if (Mission[mission][level] - 1 == 2)
	{
		PlayCutscene(mission);
	}
	else if (Mission[mission][level] - 1 == 3)
	{
		FCNPC_ExitVehicle(Mission[mission][npcs][0]);
	}
	else if (Mission[mission][level] - 1 == 4)
	{
		FreeCamera(mission);
		for (new i = 0; i < MAX_MISSION_PLAYERS; i++)
		{
			SetCameraBehindPlayer(Mission[mission][players][i]);
			TogglePlayerControllable(Mission[mission][players][i], 1);
			GameTextForPlayer(Mission[mission][players][i], "~r~Kill them all !", 3500, 4);
		}
		// Delete the driver NPC
		FCNPC_Destroy(Mission[mission][npcs][1]);
		SetPlayerVirtualWorld(Mission[mission][npcs][0], 255);
	}
	else if (Mission[mission][level] - 1 == 5)
	{
		LastBossTick = GetTickCount();
		FreeCamera(mission);
		for (new i = 0; i < MAX_MISSION_PLAYERS; i++)
		{
			if (Mission[mission][player_dead][i])
			{
				TogglePlayerSpectating(Mission[mission][players][i], 1);
				PlayerSpectatePlayer(Mission[mission][players][i], GetRandomMissionPlayerSpectace(PlayerMission[Mission[mission][players][i]]), SPECTATE_MODE_NORMAL);
			}
			SetCameraBehindPlayer(Mission[mission][players][i]);
			TogglePlayerControllable(Mission[mission][players][i], 1);
			GameTextForPlayer(Mission[mission][players][i], "~r~Kill him !", 3500, 4);
		}
	}
}

new locktimer[MAX_MISSIONS];

public LockCameraTimer(mission, playerid, type)
{
	new Float:x, Float:y, Float:z;
	GetPlayerPos(playerid, x, y, z);
	for (new i = 0; i < MAX_MISSION_PLAYERS; i++)
	{
		if (type == 0)
		{
			SetPlayerCameraPos(Mission[mission][players][i], x + 12.0, y - 12.0, z + 12.0);
		}
		else
		{
			SetPlayerCameraPos(Mission[mission][players][i], x - 4.0, y, z + 0.2);
		}

		SetPlayerCameraLookAt(Mission[mission][players][i], x, y, z);
	}
}

stock LockCameraOn(mission, playerid, type)
{
	// Start camera lock timer
	locktimer[mission] = SetTimerEx("LockCameraTimer", 50, true, "iii", mission, playerid, type);
}

stock FreeCamera(mission)
{
	// Kill camera lock timer
	KillTimer(locktimer[mission]);
}

public OnPlayerDeath(playerid, killerid, reason)
{
	// Is he a mission player
	if (PlayerMission[playerid] != INVALID_MISSION)
	{
		// Mark the player dead
		MarkDead(playerid, PlayerMission[playerid]);
		// Is he the last one ?
		new spec = GetRandomMissionPlayerSpectace(PlayerMission[playerid]);
		if (spec != INVALID_PLAYER_ID)
		{
			TogglePlayerSpectating(playerid, 1);
			PlayerSpectatePlayer(playerid, spec, SPECTATE_MODE_NORMAL);
			new msg[64], name[MAX_PLAYER_NAME + 1];
			GetPlayerName(playerid, name, sizeof(name));
			format(msg, sizeof(msg), "~b~%s~n~~w~has died", name);
			for (new i = 0; i < MAX_MISSION_PLAYERS; i++)
			{
				if (!IsPlayerConnected(Mission[PlayerMission[playerid]][players][i]) || Mission[PlayerMission[playerid]][players][i] == playerid)
				{
					continue;
				}

				GameTextForPlayer(Mission[PlayerMission[playerid]][players][i], msg, 3000, 3);
			}
		}
		else
		{
			EndMission(PlayerMission[playerid], "all players dead", false);
		}

		// Send a death message
		SendDeathMessage(killerid, playerid, reason);
	}
}

public FCNPC_OnDeath(npcid, killerid, reason)
{
	// Is he a mission NPC ?
	new mission = GetNPCMission(npcid);
	if (mission != INVALID_MISSION)
	{
		// Did they finished killing all the NPCS ?
		new alive = GetNPCAlive(mission);
		if (alive == 2 || alive == 1)
		{
			// Play the next cutscene
			PlayCutscene(mission);
		}

		// Send a death message
		SendDeathMessage(killerid, npcid, reason);

	}
	return 1;
}
