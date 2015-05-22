/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: WeaponInfo.h
	- Author(s): OrMisicL

  =========================================*/

#ifndef WEAPONINFO_H
#define WEAPONINFO_H

#ifdef _WIN32
enum eWeaponState
{
	WS_NO_BULLETS = 0,
	WS_LAST_BULLET = 1,
	WS_MORE_BULLETS = 2,
	WS_RELOADING = 3,
};
#else
#define WS_NO_BULLETS 0
#define WS_LAST_BULLET 1
#define WS_MORE_BULLETS 2
#define WS_RELOADING 3
#endif

class CWeaponInfo
{
	public:
		static DWORD	GetWeaponClipSize(int iWeaponId);
		static float	GetWeaponDamage(int iWeaponId);
		static WORD		GetWeaponRateOfFire(int iWeaponId);
};

#endif