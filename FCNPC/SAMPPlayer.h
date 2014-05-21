/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: SAMPPlayer.h
	- Author(s): OrMisicL

  =========================================*/

#ifndef SAMPPLAYER_H
#define SAMPPLAYER_H

#pragma pack(push, 1)
class CSyncData
{
	public:
		WORD			wUDAnalog;				// 0x0076 - 0x0078
		WORD			wLRAnalog;				// 0x0078 - 0x007A
		WORD			wKeys;					// 0x007A - 0x007C
		CVector3		vecPosition;			// 0x007C - 0x0088
		float			fQuaternionAngle;		// 0x0088 - 0x008C
		CVector3		vecQuaternion;			// 0x008C - 0x0098
		BYTE			byteHealth;				// 0x0098 - 0x0099
		BYTE			byteArmour;				// 0x0099 - 0x009A
		BYTE			byteWeapon;				// 0x009A - 0x009B
		BYTE			byteSpecialAction;		// 0x009B - 0x009C
		CVector3		vecVelocity;			// 0x009C - 0x00A8
		CVector3		vecSurfing;				// 0x00A8 - 0x00B4
		WORD			wSurfingInfo;			// 0x00B4 - 0x00B6
		int				iAnimationId;			// 0x00B6 - 0x00BA
		// Size = 0x44

};
#pragma pack(pop)

#pragma pack(push, 1)
class CAimSyncData
{
	public:
		BYTE			byteCameraMode;			// 0x0000 - 0x0001
		CVector3		vecFront;				// 0x0001 - 0x000D
		CVector3		vecPosition;			// 0x000D - 0x0019
		float			fZAim;					// 0x0019 - 0x001D
		BYTE			byteWeaponState;		// 0x001D - 0x001E
		PAD(pad0, 1);							// 0x001E - 0x001F
		// Size = 0x1F

};
#pragma pack(pop)

#pragma pack(push, 1)
class CVehicleSyncData
{
	public:
		WORD			wVehicleId;				// 0x001F - 0x0021
		WORD			wUDAnalog;				// 0x0021 - 0x0023
		WORD			wLRAnalog;				// 0x0023 - 0x0025
		WORD			wKeys;					// 0x0025 - 0x0027
		float			fQuaternionAngle;		// 0x0027 - 0x002B
		CVector3		vecQuaternion;			// 0x002B - 0x0037
		CVector3		vecPosition;			// 0x0037 - 0x0043
		CVector3		vecVelocity;			// 0x0043 - 0x004F
		float			fHealth;				// 0x004F - 0x0053
		BYTE			bytePlayerHealth;		// 0x0053 - 0x0054
		BYTE			bytePlayerArmour;		// 0x0054 - 0x0055
		BYTE			bytePlayerWeapon;		// 0x0055 - 0x0056
		BYTE			byteSirenState;			// 0x0056 - 0x0057
		PAD(pad0, 7);							// 0x0057 - 0x005E
		// Size = 0x3F

};
#pragma pack(pop)

#pragma pack(push, 1)
class CPassengerSyncData
{
	public:
		WORD			wVehicleId;				// 0x005E - 0x0060
		BYTE			byteSeatId;				// 0x0060 - 0x0061
		BYTE			bytePlayerWeapon;		// 0x0061 - 0x0062
		BYTE			bytePlayerHealth;		// 0x0062 - 0x0063
		BYTE			bytePlayerArmour;		// 0x0063 - 0x0064
		WORD			wUDAnalog;				// 0x0064 - 0x0066
		WORD			wLRAnalog;				// 0x0066 - 0x0068
		WORD			wKeys;					// 0x0068 - 0x006A
		CVector3		vecPosition;			// 0x006A - 0x0076
		// Size = 0x18

};
#pragma pack(pop)

#pragma pack(push, 1)
class CSAMPPlayer
{
	public:
		CAimSyncData		aimSyncData;		// 0x0000 - 0x001F
		CVehicleSyncData	vehicleSyncData;	// 0x001F - 0x005E
		CPassengerSyncData	passengerSyncData;	// 0x005E - 0x0076
		CSyncData			syncData;			// 0x0076 - 0x00BA
		PAD(pad1, 8803);						// 0x00BA - 0x231D
		BOOL				bUpdateKeys;		// 0x231D - 0x2321
		CVector3			vecPosition;		// 0x2321 - 0x232D
		float				fHealth;			// 0x232D - 0x2331
		float				fArmour;			// 0x2331 - 0x2335
		PAD(pad2, 16);							// 0x2335 - 0x2345
		float				fAngle;				// 0x2345 - 0x2349
		CVector3			vecVelocity;		// 0x2349 - 0x2355
		WORD				wUDAnalog;			// 0x2355 - 0x2357
		WORD				wLRAnalog;			// 0x2357 - 0x2359
		DWORD				dwKeys;				// 0x2359 - 0x235D
		DWORD				dwOldKeys;			// 0x235D - 0x2361
		PAD(pad3, 18);							// 0x2361 - 0x2373
		WORD				wPlayerId;			// 0x2373 - 0x2375 
		int					iUpdateState;		// 0x2375 - 0x2379 
		PAD(pad4, 564);							// 0x2379 - 0x25AD
		BOOL				bHasAimSync;		// 0x25AD - 0x25B1
		BOOL				bHasUnkSync;		// 0x25B1 - 0x25B5
		BOOL				bHasUnk2Sync;		// 0x25B5 - 0x25B9
		BYTE				byteState;			// 0x25B9 - 0x25BA
		PAD(pad5, 84);							// 0x25BA - 0x260E
		int					iSkinId;			// 0x260E - 0x2612
		PAD(pad6, 1);							// 0x2612 - 0x2613
		CVector3			vecSpawnPosition;	// 0x2613 - 0x261F
		PAD(pad7, 28);							// 0x261F - 0x263B
		BOOL				bReadyToSpawn;		// 0x263B - 0x263F
		PAD(pad8, 2);							// 0x263F - 0x2641
		BYTE				byteSeatId;			// 0x2641 - 0x2642
		WORD				wVehicleId;			// 0x2642 - 0x2644
		PAD(pad9, 12);							// 0x2644 - 0x2650
		int					iInteriorId;		// 0x2650 - 0x2654
		WORD				wWeaponAmmo[12];	// 0x2654 - 0x266C
		PAD(pad10, 28);							// 0x266C - 0x2688
		BYTE				byteWeaponId[12];	// 0x2688 - 0x2694
		PAD(pad11, 2);							// 0x2694 - 0x2696
		WORD				wTargetId;			// 0x2696 - 0x2698
		PAD(pad12, 31);							// 0x2698 - 0x26B7
		// Size = 0x26B7

};
#pragma pack(pop)

#endif