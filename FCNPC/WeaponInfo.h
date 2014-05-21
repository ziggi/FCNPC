/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: WeaponInfo.h
	- Author(s): OrMisicL

  =========================================*/

#ifndef WEAPONINFO_H
#define WEAPONINFO_H

class CWeaponInfo
{
	public:
		static DWORD	GetWeaponClipSize(int iWeaponId);
		static float	GetWeaponDamage(int iWeaponId);
		static WORD		GetWeaponRateOfFire(int iWeaponId);
};

#endif