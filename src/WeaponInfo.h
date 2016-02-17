/* =========================================

		FCNPC - Fully Controllable NPC
			----------------------

	- File: WeaponInfo.h
	- Author(s): OrMisicL

  =========================================*/

#ifndef WEAPONINFO_H
#define WEAPONINFO_H

#define WS_NO_BULLETS   0
#define WS_LAST_BULLET  1
#define WS_MORE_BULLETS 191
#define WS_RELOADING    127

class CWeaponInfo
{
public:
	static DWORD    GetWeaponClipSize(int iWeaponId);
	static float    GetWeaponDamage(int iWeaponId);
	static WORD     GetWeaponRateOfFire(int iWeaponId);
	static bool     IsDoubleHanded(BYTE byteWeaponId);
};

#endif
