/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: SAMPBullet.h
	- Author(s): OrMisicL

  =========================================*/

#ifndef SAMPBULLET_H
#define SAMPBULLET_H

#pragma pack(push, 1)
class CBulletSyncData
{
	public:
		BYTE				byteHitType;	// 0x0000 - 0x0001
		unsigned short		usHitTargetId;	// 0x0001 - 0x0003
		CVector3			vecHitOrigin;	// 0x0003 - 0x000C
		CVector3			vecHitTarget;	// 0x000C - 0x0015
		CVector3			vecCenterOfHit;	// 0x0015 - 0x001E
		// Size = 0x1E

};
#pragma pack(pop)

#endif