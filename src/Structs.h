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

#ifndef YSF_STRUCTS_H
#define YSF_STRUCTS_H

#include "CServer.h"
#include "CVector.h"
#include "CGangZonePool.h"
#include "CPickupPool.h"
#include <map>

#include <raknet/BitStream.h>
//#include <sampgdk/sampgdk.h>

class CGangZonePool;

/* -------------------------------------------------------- */

// Defines
#define	MAX_ATTACHED_OBJECTS		10
#define	MAX_FILTER_SCRIPTS			16
#define AFK_ACCURACY				1500

#define MAX_PVARS					800
#define MAX_PVAR_NAME				40

#define MAX_MENU_TEXT_SIZE			32
#define MAX_ITEMS					12
#define MAX_COLUMNS					2

/* -------------------------------------------------------- */

// Server rules special
enum CON_VARTYPE { CON_VARTYPE_FLOAT, CON_VARTYPE_INT, CON_VARTYPE_BOOL, CON_VARTYPE_STRING };

#define CON_VARFLAG_DEBUG		1
#define CON_VARFLAG_READONLY	2
#define CON_VARFLAG_RULE		4	// Gets sent with a RULES query responce

typedef void(*VARCHANGEFUNC)();

/* -------------------------------------------------------- */
typedef struct _MATRIX4X4 
{
	CVector right;
	DWORD  flags;
	CVector up;
	float  pad_u;
	CVector at;
	float  pad_a;
	CVector pos;
	float  pad_p;
} MATRIX4X4, *PMATRIX4X4;

struct ConsoleVariable_s
{
	CON_VARTYPE		VarType;
	DWORD			VarFlags;
	void*			VarPtr;
	VARCHANGEFUNC	VarChangeFunc;
};

struct C3DText  // size 0x21
{
	char*			szText;                                     // + 0x00
    DWORD			dwColor;                         // + 0x04
	CVector			vecPos;
	float			fDrawDistance;                     // + 0x14
    bool			bLineOfSight;            // + 0x18
    int				iWorld;                  // + 0x19
    WORD			attachedToPlayerID;    // + 0x1D
	WORD			attachedToVehicleID;   // + 0x1F
};

/* -------------------------------------------------------- */
// CPlayer
/* -------------------------------------------------------- */

// Big thanks to OrMisicL
struct CAimSyncData
{
	BYTE			byteCameraMode;			// 0
	CVector			vecFront;				// 1 - 13
	CVector			vecPosition;			// 13 - 25
	float			fZAim;					// 25 - 29
	BYTE			byteWeaponState;		// 29
	BYTE			unk;					// 30 - 31
	WORD			wCameraObject;			// 31 - 33
	WORD			wCameraVehicle;			// 33 - 35
	WORD			wCameraPlayer;			// 35 - 37
	WORD			wCameraActor;			// 37 - 39
	// Size = 39
};

struct CVehicleSyncData
{
	WORD			wVehicleId;				// 0x001F - 0x0021
	WORD			wUDAnalog;				// 0x0021 - 0x0023
	WORD			wLRAnalog;				// 0x0023 - 0x0025
	WORD			wKeys;					// 0x0025 - 0x0027
	float			fQuaternion[4];			// 0x0027 - 0x0037x
	CVector			vecPosition;			// 0x0037 - 0x0043
	CVector			vecVelocity;			// 0x0043 - 0x004F
	float			fHealth;				// 0x004F - 0x0053
	BYTE			bytePlayerHealth;		// 0x0053 - 0x0054
	BYTE			bytePlayerArmour;		// 0x0054 - 0x0055
	BYTE			bytePlayerWeapon : 6;	// 0x0055 - 0x0056
	BYTE			unk_2 : 2;				// 0x0055 - 0x0056
	BYTE			byteSirenState;			// 0x0056 - 0x0057
	BYTE			byteGearState;			// 0x0057 -	0x0058
	WORD			wTrailerID;				// 0x0058 - 0x005A
    union									// 
    {
            WORD			wHydraReactorAngle[2];                       
            float           fTrainSpeed;
    };
	// Size = 63
};

struct CPassengerSyncData
{
	WORD			wVehicleId;				// 0x005E - 0x0060
	BYTE			byteSeatFlags : 7;
	BYTE			byteDriveBy : 1;
	BYTE			bytePlayerWeapon;		// 0x0061 - 0x0062
	BYTE			bytePlayerHealth;		// 0x0062 - 0x0063
	BYTE			bytePlayerArmour;		// 0x0063 - 0x0064
	WORD			wUDAnalog;				// 0x0064 - 0x0066
	WORD			wLRAnalog;				// 0x0066 - 0x0068
	WORD			wKeys;					// 0x0068 - 0x006A
	CVector			vecPosition;			// 0x006A - 0x0076
	// Size = 24
};

struct CSyncData
{
	WORD			wUDAnalog;				// 0x0076 - 0x0078
	WORD			wLRAnalog;				// 0x0078 - 0x007A
	WORD			wKeys;					// 0x007A - 0x007C
	CVector			vecPosition;			// 0x007C - 0x0088
	float			fQuaternion[4];			// 0x0088 - 0x008C
	BYTE			byteHealth;				// 0x0098 - 0x0099
	BYTE			byteArmour;				// 0x0099 - 0x009A
	BYTE			byteWeapon : 6;				// 0x009A - 0x009B
	BYTE			_unk_ : 2;
	BYTE			byteSpecialAction;		// 0x009B - 0x009C
	CVector			vecVelocity;			// 0x009C - 0x00A8
	CVector			vecSurfing;				// 0x00A8 - 0x00B4
	WORD			wSurfingInfo;			// 0x00B4 - 0x00B6
	union 
	{
		DWORD		dwAnimationData;		// 0x00B6 - 0x00BA
		struct 
		{
			WORD	wAnimIndex;
			WORD	wAnimFlags;
		};
	};
	// Size = 68
};

struct CUnoccupiedSyncData // size 0x43
{
	WORD			wVehicleID;				// + 0x0000
	BYTE			bytePassengerSlot;			// + 0x0002
	CVector			vecRool;				// + 0x0003
	CVector			vecDirection;			// + 0x000F
	CVector			vecPosition;			// + 0x001B
	CVector			vecVelocity;			// + 0x0027
	CVector			vecTurnVelocity;		// + 0x0033
	float			fHealth;					// + 0x003F
};

struct CSpectatingSyncData		// size 0x12
{
	WORD			wLeftRightKeysOnSpectating;				// + 0x0000
	WORD			wUpDownKeysOnSpectating;				// + 0x0002
	WORD			wKeysOnSpectating;						// + 0x0004
	CVector			vecPosition;							// + 0x0006
};

struct CTrailerSyncData // size 0x36 = 54
{
	WORD			wTrailerID;				// + 0x0000
	CVector			vecRoll;				// + 0x0002
	CVector			vecDirection;			// + 0x000E
	CVector			vecPosition;			// + 0x001A
	CVector			vecVelocity;			// + 0x0026
	DWORD			pad;
};

typedef struct CTextdraw
{
	union
	{
		BYTE byteFlags;			// 25
		struct
		{
			BYTE byteBox : 1;
			BYTE byteLeft : 1; 
			BYTE byteRight : 1;
			BYTE byteCenter : 1;
			BYTE byteProportional : 1;
			BYTE bytePadding : 3;
		};
	};
	float			fLetterWidth;			// 1
	float			fLetterHeight;		// 5
	DWORD			dwLetterColor;		// 9
	float			fLineWidth;			// 13
	float			fLineHeight;			// 17
	DWORD			dwBoxColor;			// 21
	BYTE			byteShadow; // 26
	BYTE			byteOutline; // 27
	DWORD			dwBackgroundColor; // 31
	BYTE			byteStyle; // 32
	BYTE			byteSelectable; // 32
	float			fX; // 33
	float			fY; // 37
	WORD			dwModelIndex; // 41 - 43
	CVector			vecRot;  // 43 - 55
	float			fZoom; // 55 - 59
	WORD			color1; // 59 - 61
	WORD			color2; // 61 - 63
} _CTextdraw;

struct CPlayerTextDraw
{
	BOOL			bSlotState[MAX_PLAYER_TEXT_DRAWS];
	CTextdraw	    *TextDraw[MAX_PLAYER_TEXT_DRAWS];
	char			*szFontText[MAX_PLAYER_TEXT_DRAWS];
	bool			bHasText[MAX_PLAYER_TEXT_DRAWS];
};

struct CPlayerText3DLabels // size 0x9802
{
	C3DText			TextLabels[ MAX_3DTEXT_PLAYER ];	// + 0x0000
	BOOL			isCreated[ MAX_3DTEXT_PLAYER ];	// + 0x8400
	BYTE			unknown9800[MAX_3DTEXT_PLAYER];				// + 0x9400
	WORD			wOwnerID;
};

struct CAttachedObject // sizeof = 52 - 0x34
{
    int				iModelID;
    int				iBoneiD;
    CVector         vecPos;
    CVector         vecRot;
    CVector         vecScale;
	DWORD			dwMaterialColor1;
	DWORD			dwMaterialColor2;
};

struct CPlayerSpawnInfo // size  46
{
	BYTE			byteTeam;				// 0 - 1
	int				iSkin;					// 1 - 5
	BYTE			unk;					// 5 - 6
	CVector			vecPos;				// 6 - 18
	float			fRotation;			// 18 - 22
	int				iSpawnWeapons[3];		// 22 - 34
	int				iSpawnWeaponsAmmo[3];	// 34 - 46
};

struct CBulletSyncData // sizeof = 40
{
	BYTE			byteHitType;
	WORD			wHitID;
	CVector			vecHitOrigin;
	CVector			vecHitTarget;
	CVector			vecCenterOfHit;
	BYTE			byteWeaponID;
}; 

struct CPVar
{
    char			szVarName[MAX_PVAR_NAME + 1];
    BOOL			bIsReadOnly;
    int				iVarType;
    int				iValue;
    float			fValue;
    char*			szValue;
};

struct CPlayerVar
{
    CPVar			Vars[MAX_PVARS];
	BOOL			bIsPVarActive[MAX_PVARS];
    int				iUpperIndex;
};

struct CPlayer
{
	CAimSyncData			aimSyncData;			// 0 - 39
	CVehicleSyncData		vehicleSyncData;		// 39 -
	CPassengerSyncData		passengerSyncData;		//
	CSyncData				syncData;				// 126 - 194
	CUnoccupiedSyncData		unoccupiedSyncData;		// 194 - 261
	CSpectatingSyncData		spectatingSyncData;		// 261 - 279
	CTrailerSyncData		trailerSyncData;		// 279 - 333
	DWORD					dwPlayerSyncUnused;		// 333 - 337
	DWORD					dwVehicleSyncUnused;	// 337 - 341
	BYTE					byteStreamedIn[MAX_PLAYERS];				// 341 - 1341
	BYTE					byteVehicleStreamedIn[MAX_VEHICLES];		// 1341 - 3341
	BYTE					byteSomethingUnused[1000];					// 3341 - 4341
	BYTE					byte3DTextLabelStreamedIn[1024];			// 4341  - 5365
	BYTE					bPickupStreamedIn[MAX_PICKUPS];				// 5365 - 9461
	BYTE					byteActorStreamedIn[MAX_PLAYERS];			// 9461 - 10461
	DWORD					dwStreamedInPlayers;						// 10461 - 10465
	DWORD					dwStreamedInVehicles;						// 10465 - 10469
	DWORD					dwStreamedInSomethingUnused;				// 10469 - 10473
	DWORD					dwStreamedIn3DTextLabels;					// 10479 - 10477
	DWORD					dwStreamedInPickups;						// 10477 - 10481
	DWORD					dwStreamedInActors;							// 10481 - 10485
	DWORD					bHasSetVehiclePos;	// 10485 - 10489
	DWORD					dwSetVehiclePosTick;// 10489 - 10493
	CVector					vecVehicleNewPos;	// 10493 - 10505
	BOOL					bCameraTarget;		// 10505
	DWORD					bHasSpawnInfo;		// 10509
	BOOL					bUpdateKeys;		// 10513
	CVector					vecPosition;		// 10517
	float					fHealth;			// 10529 - 10533
	float					fArmour;			// 10533 - 10537
	float					fQuaternion[4];		// 10537 - 10553
	float					fAngle;				// 10553 - 10557
	CVector					vecVelocity;		// 10557 - 10569
	WORD					wUDAnalog;			// 10569
	WORD					wLRAnalog;			// 10571
	DWORD					dwKeys;				// 10573 - 10577
	DWORD					dwOldKeys;			// 10577 - 10581
	BOOL					bEditObject;		// 10581 - 10585
	BOOL					bEditAttachedObject;// 10585 - 10589
	WORD					wDialogID;			// 10589 - 10591
	CPlayerTextDraw*		pTextdraw;			// 10591 - 10595
	CPlayerText3DLabels*	p3DText;			// 10595 - 10599
	WORD					wPlayerId;			// 10599 - 10601
	int						iUpdateState;		// 10601 - 10605
	//DWORD					dwLastSyncTick;		// 10605 - 10609
	CAttachedObject			attachedObject[MAX_ATTACHED_OBJECTS]; // 10605 - 11125
	BOOL					attachedObjectSlot[MAX_ATTACHED_OBJECTS]; // 11125 - 11165
	BOOL					bHasAimSync;		// 11165 - 11169
	BOOL					bHasTrailerSync;	// 11169 - 11173
	BOOL					bHasUnoccupiedSync;	// 11173 - 11177
	BYTE					byteState;			// 11177 - 11178
	CVector					vecCPPos;			// 11178 - 11190
	float					fCPSize;			// 11190 - 11194
	BOOL					bIsInCP;			// 11194 - 11198
	CVector					vecRaceCPPos;		// 11198 - 11210
	CVector					vecRaceCPNextPos;	// 11210 - 11222 
	BYTE					byteRaceCPType;		// 11222 - 11223 // TODO -> replace
	float					fRaceCPSize;		// 11223 - 11227
	BOOL					bIsInRaceCP;		// 11227 - 11231
	BOOL					bIsInModShop;		// 11231 - 11235
	WORD					wSkillLevel[11];	// 11235 - 11257
	int						iLastMarkerUpdate;	// 11257 - 11261
	CPlayerSpawnInfo		spawn;				// 11261 - 11307
	BOOL					bReadyToSpawn;		// 11307 - 11311
	BYTE					byteWantedLevel;	// 11311 - 11312
	BYTE					byteFightingStyle;  // 11312 - 11313
	BYTE					byteSeatId;			// 11313 - 11314
	WORD					wVehicleId;			// 11314 - 11316
	DWORD					dwNickNameColor;	// 11316 - 11320
	BOOL					bShowCheckpoint;	// 11320 - 11324
	BOOL					bShowRaceCheckpoint;// 11324 - 11328
	int						iInteriorId;		// 11328 - 11332
	WORD					wWeaponAmmo[12];	// 11332 - 11356
	PAD(pad10, 28);								// 11356 - 11384
	BYTE					byteWeaponId[12];	// 11384 - 11396
	BYTE					byteWeaponID_unknown;// 11396 - 11397
	BYTE					byteCurrentWeapon;	// 11397 - 11398
	WORD					wTargetId;			// 11398 - 11400
	WORD					wTargetActorId;		// 11400 - 11402
	DWORD					dwLastShotTick;		// 11402 - 11406
	BYTE					dwLastShotWeapon;	// 11406 - 11407
	CBulletSyncData			bulletSyncData;		// 11407 - 11447	
	BYTE					m_byteTime;			// 11447 - 11448
	float					m_fGameTime;		// 11448 - 11452
	BYTE					byteSpectateType;	// 11452 - 11453
	DWORD					wSpectateID;		// 11453 - 11457
	DWORD					dwLastStreaming;	// 11457 - 11461
	DWORD					dwNPCRecordingType;	// 11461 - 11465
	FILE					*pRecordingFile;	// 11465 - 11469
	DWORD					dwFirstNPCWritingTime; // 11469 - 11473 
	PAD(unused, 9);								// 11473 - 11482
	CPlayerVar*				pPlayerVars;		// 11482 - 11486
	// Size = 9963
};

struct CPlayerPool // sizeof = 99520
{
	DWORD			dwVirtualWorld[MAX_PLAYERS];			// 0 - 4000
	DWORD			dwPlayersCount;							// 4000 - 4004
	DWORD			dwlastMarkerUpdate;						// 4004 - 4008
	float			fUpdatePlayerGameTimers;				// 4008 - 4012
	DWORD			dwScore[MAX_PLAYERS];					// 4012 - 8012
	DWORD			dwMoney[MAX_PLAYERS];					// 8012 - 12012
	DWORD			dwDrunkLevel[MAX_PLAYERS];				// 12012 - 16012
	DWORD			dwLastScoreUpdate[MAX_PLAYERS];			// 16012 - 20012
	char			szSerial[MAX_PLAYERS][101];				// 20012 - 121012				
	char			szVersion[MAX_PLAYERS][29];				// 121012 - 150012
	BOOL			bIsPlayerConnectedEx[MAX_PLAYERS];		// 150012 - 154012
	CPlayer			*pPlayer[MAX_PLAYERS];					// 154012 - 158012
	char			szName[MAX_PLAYERS][25];				// 158012 - 183012
	BOOL			bIsAnAdmin[MAX_PLAYERS];				// 183012 - 187012
	BOOL			bIsNPC[MAX_PLAYERS];					// 187012 - 191012
	PAD(pad0, 8000);											// 191012 - 199012
	DWORD			dwConnectedPlayers;						// 199012 - 199016
	DWORD			dwPlayerPoolSize;						// 199016 - 199020
};

/* -------------------------------------------------------- */
// CVehicle
/* -------------------------------------------------------- */

struct CVehicleSpawn // size 36
{
	int				iModelID;
    CVector			vecPos;   
    float			fRot;
    int				iColor1;  
    int				iColor2;  
    int				iRespawnTime;
    int				iInterior;
};

struct CVehicleModInfo // sizeof = 26
{
	BYTE			byteModSlots[14];                // + 0x0000
    BYTE			bytePaintJob;                    // + 0x000E
    int				iColor1;                             // + 0x000F
    int				iColor2;                             // + 0x0010
};

struct CVehicleParams // sizeof = 16
{
	BYTE engine;
	BYTE lights;
	BYTE alarm;
	BYTE doors;
	BYTE bonnet;
	BYTE boot;
	BYTE objective; // 6
	BYTE siren; // 7
	BYTE door_driver; // 8
	BYTE door_passenger;
	BYTE door_backleft;
	BYTE door_backright; // 11
	BYTE window_driver; // 12
	BYTE window_passenger;
	BYTE window_backleft;
	BYTE window_backright; // 15 - 16
};

struct CVehicle
{
	CVector			vecPosition;		// 0 - 12
	MATRIX4X4		vehMatrix;			// 12 - 76
	CVector			vecVelocity;		// 76 - 88
	CVector			vecTurnSpeed;		// 88 - 100
	WORD			wVehicleID;			// 100 - 102
	WORD			wTrailerID;			// 102 - 104
	WORD			wCabID;				// 104 - 106
	WORD			wLastDriverID;		// 106 - 108
	WORD			vehPassengers[7];	// 108 - 122
	DWORD			vehActive;			// 122 - 126
	DWORD			vehWasted;			// 126 - 130	
	CVehicleSpawn	customSpawn;		// 130 - 166
	float			fHealth;			// 166 - 170
	DWORD			vehDoorStatus;		// 170 - 174
	DWORD			vehPanelStatus;		// 174 - 178
	BYTE			vehLightStatus;		// 178 - 179
	BYTE			vehTireStatus;		// 179 - 180
	bool			bDead;				// 180 - 181
	WORD			wKillerID;			// 181 - 183
	CVehicleModInfo vehModInfo;			// 183 - 206
	char			szNumberplate[32 + 1]; // 206 - 239
	CVehicleParams	vehParamEx;			// 239 - 255
    BYTE			bDeathNotification; // 255 - 256
    BYTE			bOccupied;			// 256 - 257
    DWORD			vehOccupiedTick;	// 257 - 261
    DWORD			vehRespawnTick;		// 261 -265
};

struct CVehiclePool
{
	BYTE			byteVehicleModelsUsed[212];			// 0 - 212
	int				iVirtualWorld[MAX_VEHICLES];		// 212 - 8212
	BOOL			bVehicleSlotState[MAX_VEHICLES];	// 8212 - 16212
	CVehicle		*pVehicle[MAX_VEHICLES];			// 16212 - 24212
	DWORD			dwVehiclePoolSize;					// 24212
};

/* -------------------------------------------------------- */
// CPickup
/* -------------------------------------------------------- */

struct tPickup // size 0x14
{
	int				iModel;
	int				iType;
	CVector			vecPos;
};

struct CPickupPool
{
	tPickup			Pickup[MAX_PICKUPS];			// + 0x0000
	BOOL			bActive[MAX_PICKUPS];			// + 0xA000
	int				iWorld[MAX_PICKUPS];		// + 0xC000
	int				iPickupCount;
};

/* -------------------------------------------------------- */
// CObject
/* -------------------------------------------------------- */

struct CObjectMaterial // sizeof = 212
{
	BYTE			byteUsed;				// 197 - 198
	BYTE			byteSlot;				// 198 - 199
	WORD			wModelID;				// 199 - 201
	DWORD			dwMaterialColor;		// 201 - 205
	char			szMaterialTXD[64 + 1];	// 205 - 270
	char			szMaterialTexture[64 + 1]; // 270 - 335
	BYTE			byteMaterialSize;		// 335 - 336
	char			szFont[64 + 1];			// 336 - 401
	BYTE			byteFontSize;			// 401 - 402
	BYTE			byteBold;				// 402 - 403
	DWORD			dwFontColor;			// 403 - 407
	DWORD			dwBackgroundColor;		// 407 - 411
	BYTE			byteAlignment;			// 411 - 412
};

struct CObject // sizeof = 3700
{
	WORD			wObjectID;			// 0 - 2
	int				iModel;				// 2 - 6
	BOOL			bActive;			// 6 - 10
	MATRIX4X4		matWorld;			// 10 - 74 - pos - Object position
	CVector			vecRot; 			// 74 - 86 - Object rotation
	MATRIX4X4		matTarget;			// 86 - 150	- 
	BYTE			bIsMoving;			// 150 - 151
	BYTE			bNoCameraCol;		// 151 - 152
	float			fMoveSpeed;			// 152 - 156
	DWORD			unk_4;				// 156 -160
	float			fDrawDistance;		// 160 - 164
	WORD			wAttachedVehicleID;	// 164 - 166
	WORD			wAttachedObjectID;	// 166 - 168
	CVector			vecAttachedOffset;	// 168 - 180
	CVector			vecAttachedRotation;// 180 - 192
	BYTE			byteSyncRot;		// 192 - 193
	DWORD			dwMaterialCount;	// 193 - 197
	CObjectMaterial	Material[16];		// 197 - 3637
	char			*szMaterialText[16];// 3637 - 3653
};

struct CObjectPool
{
	BOOL			bPlayerObjectSlotState[MAX_PLAYERS][MAX_OBJECTS];	// 0 
	BOOL			bPlayersObject[MAX_OBJECTS];							// 4.000.000
	CObject			*pPlayerObjects[MAX_PLAYERS][MAX_OBJECTS];		// 4.004.000
	BOOL			bObjectSlotState[MAX_OBJECTS];						// 8.004.000
	CObject			*pObjects[MAX_OBJECTS];							// 8.008.000
};

/* -------------------------------------------------------- */
// CMenu
/* -------------------------------------------------------- */

struct MenuInteraction
{
	BOOL			Menu;
	BOOL			Row[MAX_ITEMS];
	char			unknown[12];
};
	
struct CMenu	// size 0xB84
{
	BYTE			menuID;														// + 0x0000
	char			szTitle[ MAX_MENU_TEXT_SIZE ];								// + 0x0001
	char			szItems[ MAX_ITEMS ][ MAX_COLUMNS ][ MAX_MENU_TEXT_SIZE ];	// + 0x0021
	char			szHeaders[MAX_COLUMNS][MAX_MENU_TEXT_SIZE];					// + 0x0321
	BOOL			bIsInitiedForPlayer[MAX_PLAYERS];							// + 0x0361
	MenuInteraction interaction;												// + 0x0B31
	float			fPosX;														// + 0x0B71
	float			fPosY;														// + 0x0B75
	float			fColumn1Width;												// + 0x0B79
	float			fColumn2Width;												// + 0x0B7D
	BYTE			byteColumnsNumber;											// + 0x0B81
	BYTE			byteItemsCount[ MAX_COLUMNS ];								// + 0x0B82
};

struct CMenuPool
{
	CMenu*			pMenu[ MAX_MENUS ];			//	+ 0x0000
	BOOL			bIsCreated[ MAX_MENUS ];		//	+ 0x0200
	BOOL			bPlayerMenu[MAX_PLAYERS];	//	+ 0x0400
};

/* -------------------------------------------------------- */
// CTextDraw
/* -------------------------------------------------------- */
struct CTextDrawPool
{
	BOOL			bSlotState[MAX_TEXT_DRAWS];
	CTextdraw*		TextDraw[MAX_TEXT_DRAWS];
	char*			szFontText[MAX_TEXT_DRAWS];
	bool			bHasText[MAX_TEXT_DRAWS][MAX_PLAYERS];
};

/* -------------------------------------------------------- */
// C3DText
/* -------------------------------------------------------- */

class C3DTextPool
{
public:
	BOOL			bIsCreated[MAX_3DTEXT_GLOBAL]; // 0 - 4096 <- OK
	C3DText			TextLabels[MAX_3DTEXT_GLOBAL];
};

/* -------------------------------------------------------- */
// CGangZone
/* -------------------------------------------------------- */

struct CSAMPGangZonePool
{
	float			fGangZone[MAX_GANG_ZONES][4];
	BOOL			bSlotState[MAX_GANG_ZONES];
};

/* -------------------------------------------------------- */
// CActor
/* -------------------------------------------------------- */

struct CActorAnim // 140
{
	char			szAnimLib[64]; // 0 - 64
	char			szAnimName[64]; // 64 - 128
	float			fDelta;		// 128 - 132
	BYTE			byteLoop;		// 132 - 133
	BYTE			byteLockX;			// 133 - 134
	BYTE			byteLockY;			// 134 - 135
	BYTE			byteFreeze;		// 135 - 136
	int				iTime;				//  136 - 140
};

struct CActor
{
	BYTE			pad0;				// 0
	int				iSkinID;			// 1 - 5
	CVector			vecSpawnPos;	// 5 - 17
	float			fSpawnAngle;		// 17 - 21
	DWORD			pad4;				// 21 - 25
	DWORD			pad5;				// 25 - 29
	BYTE			byteLoopAnim;		// 29 - 30
	CActorAnim		anim;
	WORD			wTime;				// 170 - 171
	float			fHealth;			// 172 - 176
	DWORD			pad;				// 176 - 180
	float			fAngle;			// 180 - 184
	CVector			vecPos;			// 184 - 196
	DWORD			pad8[3];			// 196 - 208
	BYTE			byteInvulnerable;	// 208 - 209
	WORD			wActorID;			// 209 - 211
};

struct CActorPool
{
	int				iActorVirtualWorld[MAX_ACTORS];
	BOOL			bValidActor[MAX_ACTORS];
	CActor*			pActor[MAX_ACTORS];
	DWORD			dwActorPoolSize;
};

struct CGameMode
{
	AMX				amx;
	bool			bInitialised;
	bool			bSleeping;
	float			fSleepTime;
};

struct CFilterScripts
{
	AMX*			pFilterScripts[MAX_FILTER_SCRIPTS];
	char			szFilterScriptName[MAX_FILTER_SCRIPTS][255];
	int				iFilterScriptCount;
};

struct ScriptTimer_s // sizeof = 0x11B (283)
{
	char szScriptFunc[255];
	int iTotalTime;
	int iRemainingTime;
	BOOL bRepeating;
	BOOL bKilled;
	AMX* pAMX;
	int iParamCount;
	void* cellParams;
};

typedef std::map<DWORD, ScriptTimer_s*> DwordTimerMap;

class CScriptTimers
{
public:
	DwordTimerMap			Timers;
	DWORD					dwTimerCount;
};

struct CNetGame
{
	CGameMode				*pGameModePool;			// 0
	CFilterScripts			*pFilterScriptPool;		// 4
	CPlayerPool				*pPlayerPool;			// 8
	CVehiclePool			*pVehiclePool;			// 12
	CPickupPool				*pPickupPool;			// 16
	CObjectPool				*pObjectPool;			// 20
	CMenuPool				*pMenuPool;				// 24
	CTextDrawPool			*pTextDrawPool;			// 28
	C3DTextPool				*p3DTextPool;			// 32
	CGangZonePool			*pGangZonePool;			// 36 
	CActorPool				*pActorPool;			// 40 
	int						iCurrentGameModeIndex;	// 44
	int						iCurrentGameModeRepeat;	// 48
	BOOL					bFirstGameModeLoaded;	// 52
	BOOL					unkasdasd;				// 56
	CScriptTimers			*pScriptTimers;			// 60
	RakServer				*pRak;					// 64
	DWORD					dwSomethingTick;
	DWORD					dwUnk;
	DWORD					dwUnk1;
	BOOL					bLanMode;				// 
	BOOL					bShowPlayerMarkers;		// 84
	BYTE					byteShowNameTags;		// 
	BYTE					bTirePopping;			// 
	BYTE					byteAllowWeapons;		// 
	BYTE					byteStuntBonus;			// 91 - 92
	BYTE					byteDefaultCameraCollision; // 92 - 93
	BYTE					byteWeather;			// 93 - 94
	int						iGameState;				// 94 - 98
	float					fGravity;				// 98 - 102
	int						iDeathDropMoney;		// 102 - 106
	BYTE					unklofasz;				// 106 - 107
	BYTE					byteMode;				// 107 - 108
	BYTE					bLimitGlobalChatRadius;	// 108 - 109
	BYTE					bUseCJWalk;				// 109 - 110
	float					fGlobalChatRadius;		// 110 - 114
	float					fNameTagDrawDistance;	// 114 - 118
	BYTE					byteDisableEnterExits;	// 118 - 119
	BYTE					byteNameTagLOS;			// 119 - 120
	BYTE					bManulVehicleEngineAndLights; // 120 - 121
	BYTE					bLimitPlayerMarkers;	// 121 - 122
	float					fPlayerMarkesLimit;		// 122 - 126 
	BOOL					bVehicleFriendlyFire;	// 126 - 130
#ifndef _WIN32
	double					dElapsedTime;			// size = 8
#endif
	int						iSpawnsAvailable;		// 130 - 134
	CPlayerSpawnInfo		AvailableSpawns[300];	// 129 - 13929
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PlayerID
{
	unsigned int binaryAddress;
	unsigned short port;
};

const PlayerID UNASSIGNED_PLAYER_ID =
{
	0xFFFFFFFF, 0xFFFF
};

/// All RPC functions have the same parameter list - this structure.
struct RPCParameters
{
	/// The data from the remote system
	unsigned char *input;

	/// How many bits long \a input is
	unsigned int numberOfBitsOfData;

	/// Which system called this RPC
	PlayerID sender;

	/// Which instance of RakPeer (or a derived RakServer or RakClient) got this call
	void *recipient;

	/// You can return values from RPC calls by writing them to this BitStream.
	/// This is only sent back if the RPC call originally passed a BitStream to receive the reply.
	/// If you do so and your send is reliable, it will block until you get a reply or you get disconnected from the system you are sending to, whichever is first.
	/// If your send is not reliable, it will block for triple the ping time, or until you are disconnected, or you get a reply, whichever is first.
	RakNet::BitStream *replyToSender;
};

/// These enumerations are used to describe when packets are delivered.
enum PacketPriority
{
	SYSTEM_PRIORITY,   /// \internal Used by RakNet to send above-high priority messages.
	HIGH_PRIORITY,   /// High priority messages are send before medium priority messages.
	MEDIUM_PRIORITY,   /// Medium priority messages are send before low priority messages.
	LOW_PRIORITY,   /// Low priority messages are only sent when no other messages are waiting.
	NUMBER_OF_PRIORITIES
};

/// These enumerations are used to describe how packets are delivered.
/// \note  Note to self: I write this with 3 bits in the stream.  If I add more remember to change that
enum PacketReliability
{
	UNRELIABLE = 6,   /// Same as regular UDP, except that it will also discard duplicate datagrams.  RakNet adds (6 to 17) + 21 bits of overhead, 16 of which is used to detect duplicate packets and 6 to 17 of which is used for message length.
	UNRELIABLE_SEQUENCED,  /// Regular UDP with a sequence counter.  Out of order messages will be discarded.  This adds an additional 13 bits on top what is used for UNRELIABLE.
	RELIABLE,   /// The message is sent reliably, but not necessarily in any order.  Same overhead as UNRELIABLE.
	RELIABLE_ORDERED,   /// This message is reliable and will arrive in the order you sent it.  Messages will be delayed while waiting for out of order messages.  Same overhead as UNRELIABLE_SEQUENCED.
	RELIABLE_SEQUENCED /// This message is reliable and will arrive in the sequence you sent it.  Out or order messages will be dropped.  Same overhead as UNRELIABLE_SEQUENCED.
};

/// \sa NetworkIDGenerator.h
typedef unsigned char UniqueIDType;
typedef unsigned short PlayerIndex;
typedef unsigned char RPCIndex;
const int MAX_RPC_MAP_SIZE=((RPCIndex)-1)-1;
const int UNDEFINED_RPC_INDEX=((RPCIndex)-1);

/// First byte of a network message
typedef unsigned char MessageID;

typedef unsigned int RakNetTime;
typedef long long RakNetTimeNS;

struct RakNetStatisticsStruct
{
	///  Number of Messages in the send Buffer (high, medium, low priority)
	unsigned messageSendBuffer[ NUMBER_OF_PRIORITIES ];
	///  Number of messages sent (high, medium, low priority)
	unsigned messagesSent[ NUMBER_OF_PRIORITIES ];
	///  Number of data bits used for user messages
	unsigned messageDataBitsSent[ NUMBER_OF_PRIORITIES ];
	///  Number of total bits used for user messages, including headers
	unsigned messageTotalBitsSent[ NUMBER_OF_PRIORITIES ];
	
	///  Number of packets sent containing only acknowledgements
	unsigned packetsContainingOnlyAcknowlegements;
	///  Number of acknowledgements sent
	unsigned acknowlegementsSent;
	///  Number of acknowledgements waiting to be sent
	unsigned acknowlegementsPending;
	///  Number of acknowledgements bits sent
	unsigned acknowlegementBitsSent;
	
	///  Number of packets containing only acknowledgements and resends
	unsigned packetsContainingOnlyAcknowlegementsAndResends;
	
	///  Number of messages resent
	unsigned messageResends;
	///  Number of bits resent of actual data
	unsigned messageDataBitsResent;
	///  Total number of bits resent, including headers
	unsigned messagesTotalBitsResent;
	///  Number of messages waiting for ack (// TODO - rename this)
	unsigned messagesOnResendQueue;
	
	///  Number of messages not split for sending
	unsigned numberOfUnsplitMessages;
	///  Number of messages split for sending
	unsigned numberOfSplitMessages;
	///  Total number of splits done for sending
	unsigned totalSplits;
	
	///  Total packets sent
	unsigned packetsSent;
	
	///  Number of bits added by encryption
	unsigned encryptionBitsSent;
	///  total bits sent
	unsigned totalBitsSent;
	
	///  Number of sequenced messages arrived out of order
	unsigned sequencedMessagesOutOfOrder;
	///  Number of sequenced messages arrived in order
	unsigned sequencedMessagesInOrder;
	
	///  Number of ordered messages arrived out of order
	unsigned orderedMessagesOutOfOrder;
	///  Number of ordered messages arrived in order
	unsigned orderedMessagesInOrder;
	
	///  Packets with a good CRC received
	unsigned packetsReceived;
	///  Packets with a bad CRC received
	unsigned packetsWithBadCRCReceived;
	///  Bits with a good CRC received
	unsigned bitsReceived;
	///  Bits with a bad CRC received
	unsigned bitsWithBadCRCReceived;
	///  Number of acknowledgement messages received for packets we are resending
	unsigned acknowlegementsReceived;
	///  Number of acknowledgement messages received for packets we are not resending
	unsigned duplicateAcknowlegementsReceived;
	///  Number of data messages (anything other than an ack) received that are valid and not duplicate
	unsigned messagesReceived;
	///  Number of data messages (anything other than an ack) received that are invalid
	unsigned invalidMessagesReceived;
	///  Number of data messages (anything other than an ack) received that are duplicate
	unsigned duplicateMessagesReceived;
	///  Number of messages waiting for reassembly
	unsigned messagesWaitingForReassembly;
	///  Number of messages in reliability output queue
	unsigned internalOutputQueueSize;
	///  Current bits per second
	double bitsPerSecond;
	///  connection start time
	RakNetTime connectionStartTime;
};

/// This represents a user message from another system.
struct Packet
{
	/// Server only - this is the index into the player array that this playerId maps to
	PlayerIndex playerIndex; // 0 - 2

	/// The system that send this packet.
	PlayerID playerId; // 2  - 8

	/// The length of the data in bytes
	/// \deprecated You should use bitSize.
	unsigned int length; // 8 - 12

	/// The length of the data in bits
	unsigned int bitSize; // 12 - 16

	/// The data from the sender
	unsigned char* data; // 16 - 20

	/// @internal
	/// Indicates whether to delete the data, or to simply delete the packet.
	bool deleteData;
};

#ifdef _WIN32
class RakServer
{
public:
	~RakServer(); // THIS NOT
	virtual void _0();
	virtual bool Start(unsigned short AllowedPlayers, unsigned int depreciated, int threadSleepTimer, unsigned short port, const char *forceHostAddress = 0) = 0; // 4
	virtual void DisableSecurity( void ); // 8
	virtual void SetPassword( const char *_password );	// 12
	virtual bool HasPassword( void );	// 16
	virtual void Disconnect( unsigned int blockDuration, unsigned char orderingChannel=0 );	// 20
	virtual bool Send_ASD(const char *data, const int length, int priority, int reliability, char orderingChannel, PlayerID playerId, bool broadcast); // 24
	virtual bool Send(RakNet::BitStream* parameters, int priority, int reliability, unsigned orderingChannel, PlayerID playerId, bool broadcast);	// 28
	virtual void _20(); // 
	virtual void _24();
	virtual Packet* ReceiveEx( void );
	virtual void Kick( const PlayerID playerId ); // 0x2C
	virtual void DeallocatePacket(Packet* asd);
	virtual void SetAllowedPlayers(unsigned short numberAllowed); // 0x34
	virtual unsigned short GetAllowedPlayers( void ) const;
	virtual unsigned short GetConnectedPlayers( void );
	virtual void GetPlayerIPFromID( const PlayerID playerId, char returnValue[ 22 ], unsigned short *port );
	virtual void PingPlayer( const PlayerID playerId );
	virtual int GetAveragePing( const PlayerID playerId );
	virtual int GetLastPing( const PlayerID playerId ); // 4C
	virtual int GetLowestPing( const PlayerID playerId );
	virtual void _54();
	virtual void _58();
	virtual void _5C();
	virtual void _60();
	virtual void _64();
	virtual void _68();
	virtual void _6C();
	virtual void _70();
	virtual void RegisterAsRemoteProcedureCall( int* uniqueID, void ( *functionPointer ) ( RPCParameters *rpcParms ) );	// 74
	virtual void RegisterClassMemberRPC( int* uniqueID, void *functionPointer );	// 78
	virtual void UnregisterAsRemoteProcedureCall( int* uniqueID );	// 7C
	virtual bool RPC(int* uniqueID, RakNet::BitStream* parameters, PacketPriority priority, PacketReliability reliability, unsigned orderingChannel, PlayerID playerId, bool broadcast, bool shiftTimestamp); // 80
	virtual void _84();
	virtual void _88();
	virtual void _8C();
	virtual void _90();
	virtual void _94();
	virtual void _98();
	virtual void _9C();
	virtual void _A0();
	virtual void _A4();
	virtual void _A8();
	virtual void _AC();
	virtual void _B0();
	virtual void _B4();
	virtual void _B8();
	virtual void _BC();
	virtual void _C0();
	virtual void _C4();
	virtual void _C8();
	virtual void _CC();
	virtual const char* GetLocalIP( unsigned int index );
	virtual PlayerID GetInternalID( void ) const;
	virtual void PushBackPacket( Packet *packet, bool pushAtHead );
	virtual void SetRouterInterface( void *routerInterface );
	virtual void RemoveRouterInterface( void *routerInterface );
	virtual int GetIndexFromPlayerID( PlayerID playerId ); // E4
	virtual PlayerID GetPlayerIDFromIndex( int index ); // E8 - 232 - 236
	virtual void UNKNOWN(void); // 236 - 240
	virtual void AddToBanList( const char *IP, unsigned int milliseconds=0 ); // 240 - 244
	virtual void RemoveFromBanList( const char *IP );
	virtual void ClearBanList( void );
	virtual bool IsBanned( const char *IP );
	virtual bool IsActivePlayerID( const PlayerID playerId );
	virtual void SetTimeoutTime( RakNetTime timeMS, const PlayerID target );
	virtual bool SetMTUSize( int size );
	virtual int GetMTUSize( void ) const;
	virtual void AdvertiseSystem( const char *host, unsigned short remotePort, const char *data, int dataLength );
	virtual RakNetStatisticsStruct * const GetStatistics( const PlayerID playerId );
	virtual void ApplyNetworkSimulator( double maxSendBPS, unsigned short minExtraPing, unsigned short extraPingVariance);
};

#else

class RakServer
{
public:
	~RakServer();
	virtual bool _0(); 
	virtual void _4();
	virtual bool Start(unsigned short AllowedPlayers, unsigned int depreciated, int threadSleepTimer, unsigned short port, const char *forceHostAddress = 0) = 0;
	virtual void _C();
	virtual void _10();
	virtual void _14();
	virtual void _18();
	virtual void _1C();
	virtual bool SendGECI(RakNet::BitStream* parameters, int priority, int reliability, unsigned orderingChannel, PlayerID playerId, bool broadcast);
	virtual bool Send(RakNet::BitStream* parameters, int priority, int reliability, unsigned orderingChannel, PlayerID playerId, bool broadcast);
	virtual void _28();
	virtual void _2C();
	virtual void _30();
	virtual void _34();
	virtual void SetAllowedPlayers(unsigned short numberAllowed);
	virtual void _3C();
	virtual void _40();
	virtual void _44();
	virtual void _48();
	virtual void _4C();
	virtual int GetLastPing( const PlayerID playerId ); // 0x50
	virtual void _54();
	virtual void _58();
	virtual void _5C();
	virtual const char* GetLocalIP( unsigned int index );
	virtual PlayerID GetInternalID( void ) const;
	virtual void PushBackPacket( Packet *packet, bool pushAtHead );
	virtual void SetRouterInterface( void *routerInterface );
	virtual void RemoveRouterInterface( void *routerInterface );
	virtual void UnregisterAsRemoteProcedureCall_BAD( int* uniqueID );
	virtual void RegisterAsRemoteProcedureCall( int* uniqueID, void ( *functionPointer ) ( RPCParameters *rpcParms ) ); // 120
	virtual void RegisterClassMemberRPC( int* uniqueID, void *functionPointer ); // 124
	virtual void UnregisterAsRemoteProcedureCall( int* uniqueID ); // 128
	virtual void UnregisterAsRemoteProcedureCall_4( int* uniqueID );
	virtual void UnregisterAsRemoteProcedureCall_5( int* uniqueID );
	virtual bool RPC(int* uniqueID, RakNet::BitStream* parameters, int priority, int reliability, unsigned orderingChannel, PlayerID playerId, bool broadcast, bool shiftTimestamp);
	virtual void _90();
	virtual void _94();
	virtual void _98();
	virtual void _9C();
	virtual void _A0();
	virtual void _A4();
	virtual void _A8();
	virtual void _AC();
	virtual void _B0();
	virtual void _B4();
	virtual void _B8();
	virtual void _BC();
	virtual void _C0();
	virtual void _C4();
	virtual void _C8();
	virtual void _CC();
	virtual void _D0();
	virtual void _D4();
	virtual void _D8();
	virtual void _DC();
	virtual void _E0();
	virtual void _E8();
	virtual int GetIndexFromPlayerID( PlayerID playerId ); // EC
	virtual PlayerID GetPlayerIDFromIndex( int index ); // F0 - 240 - 244
	virtual void AddToBanList( const char *IP, unsigned int milliseconds=0 ); // 244 - 248
	virtual void RemoveFromBanList( const char *IP ); // 248 - 252
	virtual void ClearBanList( void );
	virtual bool IsBanned( const char *IP );
	virtual bool IsActivePlayerID( const PlayerID playerId );
	virtual void SetTimeoutTime( RakNetTime timeMS, const PlayerID target );
	virtual bool SetMTUSize( int size );
	virtual int GetMTUSize( void ) const;
	virtual void AdvertiseSystem( const char *host, unsigned short remotePort, const char *data, int dataLength );
	virtual RakNetStatisticsStruct * const GetStatistics( const PlayerID playerId );
	virtual void ApplyNetworkSimulator( double maxSendBPS, unsigned short minExtraPing, unsigned short extraPingVariance);
};
#endif

#endif
