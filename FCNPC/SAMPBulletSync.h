/* =========================================

		FCNPC - Fully Controllable NPC
			----------------------

	- File: SAMPBulletSync.h
	- Author(s): kurta999

=========================================*/

#ifndef SAMPBULLETSYNC_H
#define SAMPBULLETSYNC_H

#pragma pack(push, 1)
class CBulletSyncData 
{
public:
	BYTE			byteHitType;		// 0x0000 - 0x0001
	WORD			wHitID;				// 0x0001 - 0x0003
	CVector3		vecHitOrigin;		// 0x0003 - 0x000F
	CVector3		vecHitTarget;		// 0x000F - 0x001B
	CVector3		vecCenterOfHit;		// 0x001B - 0x0027
	BYTE			byteWeaponID;		// 0x0027 - 0x0028
	// Size = 0x28
};
#pragma pack(pop)

#endif