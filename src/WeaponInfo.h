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

	int GetType(int iWeaponId);
	bool SetType(int iWeaponId, int iType);

	float GetDamage(int iWeaponId);
	bool SetDamage(int iWeaponId, float fDamage);

	int GetClipSize(int iWeaponId);
	bool SetClipSize(int iWeaponId, int iClipSize);

	int GetShootTime(int iWeaponId);
	bool SetShootTime(int iWeaponId, int iTime);

	int GetReloadTime(int iWeaponId);
	bool SetReloadTime(int iWeaponId, int iTime);

	bool IsValid(int iWeaponId);
	bool IsDoubleHanded(int iWeaponId);

	SWeaponInfo *m_pWeaponInfo[MAX_WEAPONS];
	const static SWeaponInfo m_sDefaultWeaponInfo[MAX_WEAPONS];
};

#endif
