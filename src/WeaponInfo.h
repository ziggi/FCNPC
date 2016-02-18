/* =========================================

		FCNPC - Fully Controllable NPC
			----------------------

	- File: WeaponInfo.h
	- Author(s): OrMisicL, ziggi

  =========================================*/

#ifndef WEAPONINFO_H
#define WEAPONINFO_H

#define MAX_WEAPONS 46

#define WEAPON_TYPE_NONE       0
#define WEAPON_TYPE_MELEE      1
#define WEAPON_TYPE_SHOOT      2
#define WEAPON_TYPE_ROCKET     3
#define WEAPON_TYPE_SPRAY      4
#define WEAPON_TYPE_SPECIAL    5

class CWeaponInfo
{
public:
	struct SWeaponInfo
	{
		int iType;
		float fDamage;
		int iClipSize;
		int iShootTime;
		int iReloadTime;
	};

	CWeaponInfo();

	bool SetDefaultInfo(int iWeaponId);
	SWeaponInfo GetDefaultInfo(int iWeaponId);

	int GetWeaponType(int iWeaponId);
	bool SetWeaponType(int iWeaponId, int iType);

	float GetWeaponDamage(int iWeaponId);
	bool SetWeaponDamage(int iWeaponId, float fDamage);

	int GetWeaponClipSize(int iWeaponId);
	bool SetWeaponClipSize(int iWeaponId, int iClipSize);

	int GetWeaponShootTime(int iWeaponId);
	bool SetWeaponShootTime(int iWeaponId, int iTime);

	int GetWeaponReloadTime(int iWeaponId);
	bool SetWeaponReloadTime(int iWeaponId, int iTime);

	bool IsValidWeapon(int iWeaponId);
	bool IsDoubleHanded(int iWeaponId);

	SWeaponInfo *m_pWeaponInfo[MAX_WEAPONS];
	const static SWeaponInfo m_sDefaultWeaponInfo[MAX_WEAPONS];
};

#endif
