/*
 *  Version: MPL 1.1
 *
 *  The contents of this file are subject to the Mozilla Public License Version
 *  1.1 (the "License"); you may not use this file except in compliance with
 *  the License. You may obtain a copy of the License at
 *  http://www.mozilla.org/MPL/
 *
 *  Software distributed under the License is distributed on an "AS IS" basis,
 *  WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 *  for the specific language governing rights and limitations under the
 *  License.
 *
 *  The Original Code is the YSI 2.0 SA:MP plugin.
 *
 *  The Initial Developer of the Original Code is Alex "Y_Less" Cole.
 *  Portions created by the Initial Developer are Copyright (C) 2008
 *  the Initial Developer. All Rights Reserved.
 *
 *  Contributor(s):
 *
 *  Peter Beverloo
 *  Marcus Bauer
 *  MaVe;
 *  Sammy91
 *  Incognito
 *
 *  Special Thanks to:
 *
 *  SA:MP Team past, present and future
 */

#ifndef YSF_RPCS_H
#define YSF_RPCS_H

extern int RPC_Gravity;
extern int RPC_Weather;
extern int RPC_CreatePickup;
extern int RPC_DestroyPickup;
extern int RPC_SetPlayerTeam;
extern int RPC_CreateObject;
extern int RPC_DestroyObject;
extern int RPC_AttachObject;
extern int RPC_Widescreen;
extern int RPC_ShowGangZone;
extern int RPC_HideGangZone;
extern int RPC_FlashGangZone;
extern int RPC_StopFlashGangZone;
extern int RPC_RemovePlayerAttachedObject;
extern int RPC_WorldPlayerAdd;
extern int RPC_WorldPlayerRemove;
extern int RPC_WorldPlayerDeath;
extern int RPC_ChatBubble;
extern int RPC_SetPlayerSkin;
extern int RPC_SetPlayerName;
extern int RPC_SetPlayerHealth;
extern int RPC_SetPlayerArmour;
extern int RPC_SetFightingStyle;
extern int RPC_SetPlayerSkillLevel;
extern int RPC_ApplyAnimation;
extern int RPC_ClearAnimations;
extern int RPC_ClientMessage;
extern int RPC_ScrDisplayGameText;
extern int RPC_Chat;
extern int RPC_NewPlayer;

extern int RPC_UpdateScoresPingsIPs;
extern int RPC_PickedUpPickup;
extern int RPC_Spawn;
extern int RPC_Death;
extern int RPC_DeathBroadcast;

void InitRPCs();

enum PacketEnumeration : unsigned char {
	ID_VEHICLE_SYNC = 200,
	ID_AIM_SYNC = 203,
	ID_BULLET_SYNC = 206,
	ID_PLAYER_SYNC = 207,
	ID_MARKERS_SYNC,
	ID_UNOCCUPIED_SYNC = 209,
	ID_TRAILER_SYNC = 210,
	ID_PASSENGER_SYNC = 211,
	ID_SPECTATOR_SYNC = 212,
	ID_RCON_COMMAND = 201,
	ID_RCON_RESPONCE,
	ID_WEAPONS_UPDATE = 204,
	ID_STATS_UPDATE = 205,
};

#endif
