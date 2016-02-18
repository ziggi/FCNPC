/* =========================================

		FCNPC - Fully Controllable NPC
			----------------------

	- File: WeaponInfo.h
	- Author(s): OrMisicL, ziggi

  =========================================*/

#ifndef WEAPONINFO_H
#define WEAPONINFO_H

#define MAX_WEAPONS 47

#define WEAPON_TYPE_NONE       0
#define WEAPON_TYPE_MELEE      1
#define WEAPON_TYPE_SHOOT      2
#define WEAPON_TYPE_ROCKET     3
#define WEAPON_TYPE_SPRAY      4
#define WEAPON_TYPE_SPECIAL    5

struct SWeaponInfo
{
	int iType;
	float fDamage;
	int iClipSize;
	int iShootTime;
	int iReloadTime;
};

class CWeaponInfo
{
public:
	CWeaponInfo();

	static bool SetDefaultInfo(int iWeaponId, SWeaponInfo sWeaponInfo);
	static SWeaponInfo GetDefaultInfo(int iWeaponId);

	bool SetInfo(int iWeaponId, SWeaponInfo sWeaponInfo);
	SWeaponInfo GetInfo(int iWeaponId);

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

	static bool IsValid(int iWeaponId);
	bool IsDoubleHanded(int iWeaponId);
	int GetSkillID(int iWeaponId);

private:
	SWeaponInfo *m_pWeaponInfo[MAX_WEAPONS];
};

#endif
