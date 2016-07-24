/* =========================================

		FCNPC - Fully Controllable NPC
			----------------------

	- File: WeaponInfo.hpp
	- Author(s): OrMisicL, ziggi

  =========================================*/

#ifndef WEAPONINFO_H
#define WEAPONINFO_H

#define MAX_WEAPONS 47

enum {
	WEAPON_TYPE_NONE,
	WEAPON_TYPE_MELEE,
	WEAPON_TYPE_SHOOT,
	WEAPON_TYPE_ROCKET,
	WEAPON_TYPE_THROW,
	WEAPON_TYPE_SPRAY,
	WEAPON_TYPE_SPECIAL,
};

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
	~CWeaponInfo();

	static bool SetDefaultInfo(BYTE byteWeaponId, SWeaponInfo sWeaponInfo);
	static SWeaponInfo GetDefaultInfo(BYTE byteWeaponId);

	bool SetInfo(BYTE byteWeaponId, SWeaponInfo sWeaponInfo);
	SWeaponInfo GetInfo(BYTE byteWeaponId);

	int GetType(BYTE byteWeaponId);
	bool SetType(BYTE byteWeaponId, int iType);

	float GetDamage(BYTE byteWeaponId);
	bool SetDamage(BYTE byteWeaponId, float fDamage);

	int GetClipSize(BYTE byteWeaponId);
	bool SetClipSize(BYTE byteWeaponId, int iClipSize);

	int GetShootTime(BYTE byteWeaponId);
	bool SetShootTime(BYTE byteWeaponId, int iTime);

	int GetReloadTime(BYTE byteWeaponId);
	bool SetReloadTime(BYTE byteWeaponId, int iTime);

	static bool IsValid(BYTE byteWeaponId);
	bool IsDoubleHanded(BYTE byteWeaponId);
	int GetSkillID(BYTE byteWeaponId);

private:
	SWeaponInfo *m_pWeaponInfo[MAX_WEAPONS];
};

#endif
