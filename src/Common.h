/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: Common.h
	- Author(s): OrMisicL

  =========================================*/

#ifndef COMMON_H
#define COMMON_H

#include <math.h>

// logprintf function defintion
typedef void (* logprintf_t)(char *szFormat, ...);
extern logprintf_t          logprintf;

// General definitions
#define MAX_NODES           64
#define MAX_FILTERSCRIPTS   16
#define INVALID_ENTITY_ID   (0xFFFF)
#define SAFE_DELETE(ptr)    if(ptr) { delete ptr; ptr = NULL; }
#define SAFE_RELEASE(ptr)   if(ptr) { ptr->Release(); ptr = NULL; }
#define PAD(a, b)           char a[b]
#ifndef CHECK_PARAMS
	#define CHECK_PARAMS(m,n)                                                                                           \
		do                                                                                                              \
		{                                                                                                               \
			if (params[0] != (m * 4))                                                                                   \
			{                                                                                                           \
				logprintf("[FCNPC] Error: Incorrect parameter count for \"" n "\", %d != %d\n", m, params[0] / 4); \
				return 0;                                                                                               \
			}                                                                                                           \
		}                                                                                                               \
		while (0)
#endif
// Plugin exports
#define PLUGIN_DATA_NETGAME        225
#define PLUGIN_DATA_RAKSERVER      226
#define PLUGIN_DATA_LOADFSCRIPT    227
#define PLUGIN_DATA_CONSOLE        228
#define PLUGIN_DATA_UNLOADFSCRIPT  229

// Update state
#define UPDATE_STATE_NONE       0
#define UPDATE_STATE_ONFOOT     1
#define UPDATE_STATE_DRIVER     2
#define UPDATE_STATE_PASSENGER  3
// Mouvement type
#define MOVE_TYPE_AUTO      -1
#define MOVE_TYPE_WALK      0
#define MOVE_TYPE_RUN       1
#define MOVE_TYPE_SPRINT    2
#define MOVE_TYPE_DRIVE     3
// Moving velocity
#define MOVE_SPEED_WALK     0.1552086f
#define MOVE_SPEED_RUN      0.56444f
#define MOVE_SPEED_SPRINT   0.926784f
// Playback types
#define PLAYBACK_TYPE_NONE              0
#define PLAYBACK_TYPE_DRIVER            1
#define PLAYBACK_TYPE_ONFOOT            2
// Vehicle mode info
#define VEHICLE_MODEL_INFO_SIZE         1
#define VEHICLE_MODEL_INFO_FRONTSEAT    2
#define VEHICLE_MODEL_INFO_REARSEAT     3
// Node types
#define NODE_TYPE_NONE                  (-1)
#define NODE_TYPE_PED                   0
#define NODE_TYPE_VEHICLE               1
#define NODE_TYPE_BOAT                  2
// Keys
#define KEY_NONE (0)
#define KEY_AIM  (128)

// from sampgdk
#define MAX_PLAYER_NAME (24)
#define MAX_CLIENT_MESSAGE (144)
#define MAX_PLAYERS (1000)
#define MAX_VEHICLES (2000)
#define MAX_ACTORS (1000)
#define INVALID_PLAYER_ID (0xFFFF)
#define INVALID_VEHICLE_ID (0xFFFF)
#define INVALID_ACTOR_ID (0xFFFF)
#define NO_TEAM (255)
#define MAX_OBJECTS (1000)
#define INVALID_OBJECT_ID (0xFFFF)
#define MAX_GANG_ZONES (1024)
#define MAX_TEXT_DRAWS (2048)
#define MAX_PLAYER_TEXT_DRAWS (256)
#define MAX_MENUS (128)
#define MAX_3DTEXT_GLOBAL (1024)
#define MAX_3DTEXT_PLAYER (1024)
#define MAX_PICKUPS (4096)
#define INVALID_MENU (0xFF)
#define INVALID_TEXT_DRAW (0xFFFF)
#define INVALID_GANG_ZONE (-1)
#define INVALID_3DTEXT_ID (0xFFFF)
#define SERVER_VARTYPE_NONE (0)
#define SERVER_VARTYPE_INT (1)
#define SERVER_VARTYPE_STRING (2)
#define SERVER_VARTYPE_FLOAT (3)
#define TEXT_DRAW_FONT_SPRITE_DRAW (4)
#define TEXT_DRAW_FONT_MODEL_PREVIEW (5)
#define DIALOG_STYLE_MSGBOX (0)
#define DIALOG_STYLE_INPUT (1)
#define DIALOG_STYLE_LIST (2)
#define DIALOG_STYLE_PASSWORD (3)
#define DIALOG_STYLE_TABLIST (4)
#define DIALOG_STYLE_TABLIST_HEADERS (5)
#define PLAYER_STATE_NONE (0)
#define PLAYER_STATE_ONFOOT (1)
#define PLAYER_STATE_DRIVER (2)
#define PLAYER_STATE_PASSENGER (3)
#define PLAYER_STATE_EXIT_VEHICLE (4)
#define PLAYER_STATE_ENTER_VEHICLE_DRIVER (5)
#define PLAYER_STATE_ENTER_VEHICLE_PASSENGER (6)
#define PLAYER_STATE_WASTED (7)
#define PLAYER_STATE_SPAWNED (8)
#define PLAYER_STATE_SPECTATING (9)
#define PLAYER_MARKERS_MODE_OFF (0)
#define PLAYER_MARKERS_MODE_GLOBAL (1)
#define PLAYER_MARKERS_MODE_STREAMED (2)
#define WEAPON_BRASSKNUCKLE (1)
#define WEAPON_GOLFCLUB (2)
#define WEAPON_NITESTICK (3)
#define WEAPON_KNIFE (4)
#define WEAPON_BAT (5)
#define WEAPON_SHOVEL (6)
#define WEAPON_POOLSTICK (7)
#define WEAPON_KATANA (8)
#define WEAPON_CHAINSAW (9)
#define WEAPON_DILDO (10)
#define WEAPON_DILDO2 (11)
#define WEAPON_VIBRATOR (12)
#define WEAPON_VIBRATOR2 (13)
#define WEAPON_FLOWER (14)
#define WEAPON_CANE (15)
#define WEAPON_GRENADE (16)
#define WEAPON_TEARGAS (17)
#define WEAPON_MOLTOV (18)
#define WEAPON_COLT45 (22)
#define WEAPON_SILENCED (23)
#define WEAPON_DEAGLE (24)
#define WEAPON_SHOTGUN (25)
#define WEAPON_SAWEDOFF (26)
#define WEAPON_SHOTGSPA (27)
#define WEAPON_UZI (28)
#define WEAPON_MP5 (29)
#define WEAPON_AK47 (30)
#define WEAPON_M4 (31)
#define WEAPON_TEC9 (32)
#define WEAPON_RIFLE (33)
#define WEAPON_SNIPER (34)
#define WEAPON_ROCKETLAUNCHER (35)
#define WEAPON_HEATSEEKER (36)
#define WEAPON_FLAMETHROWER (37)
#define WEAPON_MINIGUN (38)
#define WEAPON_SATCHEL (39)
#define WEAPON_BOMB (40)
#define WEAPON_SPRAYCAN (41)
#define WEAPON_FIREEXTINGUISHER (42)
#define WEAPON_CAMERA (43)
#define WEAPON_NIGHTVISION (44)
#define WEAPON_INFRARED (45)
#define WEAPON_PARACHUTE (46)
#define WEAPON_VEHICLE (49)
#define WEAPON_DROWN (53)
#define WEAPON_COLLISION (54)
#define KEY_ACTION (1)
#define KEY_CROUCH (2)
#define KEY_FIRE (4)
#define KEY_SPRINT (8)
#define KEY_SECONDARY_ATTACK (16)
#define KEY_JUMP (32)
#define KEY_LOOK_RIGHT (64)
#define KEY_HANDBRAKE (128)
#define KEY_LOOK_LEFT (256)
#define KEY_SUBMISSION (512)
#define KEY_LOOK_BEHIND (512)
#define KEY_WALK (1024)
#define KEY_ANALOG_UP (2048)
#define KEY_ANALOG_DOWN (4096)
#define KEY_ANALOG_LEFT (8192)
#define KEY_ANALOG_RIGHT (16384)
#define KEY_YES (65536)
#define KEY_NO (131072)
#define KEY_CTRL_BACK (262144)
#define KEY_UP (-128)
#define KEY_DOWN (128)
#define KEY_LEFT (-128)
#define KEY_RIGHT (128)
#define BODY_PART_TORSO (3)
#define BODY_PART_GROIN (4)
#define BODY_PART_LEFT_ARM (5)
#define BODY_PART_RIGHT_ARM (6)
#define BODY_PART_LEFT_LEG (7)
#define BODY_PART_RIGHT_LEG (8)
#define BODY_PART_HEAD (9)
#define CLICK_SOURCE_SCOREBOARD (0)
#define EDIT_RESPONSE_CANCEL (0)
#define EDIT_RESPONSE_FINAL (1)
#define EDIT_RESPONSE_UPDATE (2)
#define SELECT_OBJECT_GLOBAL_OBJECT (1)
#define SELECT_OBJECT_PLAYER_OBJECT (2)
#define BULLET_HIT_TYPE_NONE (0)
#define BULLET_HIT_TYPE_PLAYER (1)
#define BULLET_HIT_TYPE_VEHICLE (2)
#define BULLET_HIT_TYPE_OBJECT (3)
#define BULLET_HIT_TYPE_PLAYER_OBJECT (4)

// Other
#define MAX_DAMAGE_DISTANCE             200.0f
#define MAX_DISTANCE_TO_ENTER_VEHICLE   30.0f
#define DEFAULT_UPDATE_RATE             50

// Hook functions
#ifdef _WIN32
	#define HOOK void _declspec(naked)
#else
	#define HOOK void __attribute__((naked))
#endif

#endif
