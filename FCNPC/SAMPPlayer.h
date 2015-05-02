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
		PAD(pad0, 8);
		CAimSyncData		aimSyncData;		// 0x0000 - 0x001F
		CVehicleSyncData	vehicleSyncData;	// 0x001F - 0x005E
		CPassengerSyncData	passengerSyncData;	// 0x005E - 0x0076
		CSyncData			syncData;			// 0x0076 - 0x00BA
		PAD(pad1, 10319);						// 0x00C2 - 0x2911
		BOOL				bUpdateKeys;		// 0x2911 - 0x2915
		CVector3			vecPosition;		// 0x2915 - 0x2921
		float				fHealth;			// 0x2921 - 0x2925
		float				fArmour;			// 0x2925 - 0x2929
		PAD(pad2, 16);							// 0x2929 - 0x2939
		float				fAngle;				// 0x2939 - 0x293D
		CVector3			vecVelocity;		// 0x293D - 0x2949
		WORD				wUDAnalog;			// 0x2949 - 0x294B
		WORD				wLRAnalog;			// 0x294B - 0x294D
		DWORD				dwKeys;				// 0x294D - 0x2951
		DWORD				dwOldKeys;			// 0x2951 - 0x2955
		PAD(pad3, 18);							// 0x2955 - 0x2967
		WORD				wPlayerId;			// 0x2967 - 0x2969 
		int					iUpdateState;		// 0x2969 - 0x296D
		PAD(pad4, 560);							// 0x296D - 0x2B9D
		BOOL				bHasAimSync;		// 0x2B9D - 0x2BA1
		BOOL				bHasUnkSync;		// 0x2BA1 - 0x2BA5
		BOOL				bHasUnk2Sync;		// 0x2BA5 - 0x2BA9
		BYTE				byteState;			// 0x2BA9 - 0x2BAA
		PAD(pad5, 84);							// 0x2BAA - 0x2BFE
		int					iSkinId;			// 0x2BFE - 0x2C02
		PAD(pad6, 1);							// 0x2C02 - 0x2C03
		CVector3			vecSpawnPosition;	// 0x2C03 - 0x2C0F
		PAD(pad7, 28);							// 0x2C0F - 0x2C2B
		BOOL				bReadyToSpawn;		// 0x2C2B - 0x2C2F
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