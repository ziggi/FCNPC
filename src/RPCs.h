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

extern int RPC_ShowPlayerForPlayer;
extern int RPC_SetPlayerSkillLevel;
extern int RPC_ApplyAnimation;
extern int RPC_ClearAnimations;
extern int RPC_SetPlayerFightingStyle;
extern int RPC_ServerJoin;
extern int RPC_ServerQuit;
extern int RPC_SetPlayerSkin;
extern int RPC_HidePlayerForPlayer;
extern int RPC_WorldPlayerDeath;

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
