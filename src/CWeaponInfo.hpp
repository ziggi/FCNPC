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
	float fRange;
	int iClipSize;
	int iShootTime;
	int iReloadTime;
	float fAccuracy;
};

class CWeaponInfo
{
public:
	CWeaponInfo();
	~CWeaponInfo();

	static bool SetDefaultInfo(BYTE byteWeaponId, const SWeaponInfo &sWeaponInfo);
	static SWeaponInfo GetDefaultInfo(BYTE byteWeaponId);

	bool SetInfo(BYTE byteWeaponId, const SWeaponInfo &sWeaponInfo);
	SWeaponInfo GetInfo(BYTE byteWeaponId);

	int GetType(BYTE byteWeaponId);
	bool SetType(BYTE byteWeaponId, int iType);

	float GetDamage(BYTE byteWeaponId);
	bool SetDamage(BYTE byteWeaponId, float fDamage);

	float GetRange(BYTE byteWeaponId);
	bool SetRange(BYTE byteWeaponId, float fRange);

	int GetClipSize(BYTE byteWeaponId);
	bool SetClipSize(BYTE byteWeaponId, int iClipSize);

	int GetShootTime(BYTE byteWeaponId);
	bool SetShootTime(BYTE byteWeaponId, int iTime);

	int GetReloadTime(BYTE byteWeaponId);
	bool SetReloadTime(BYTE byteWeaponId, int iTime);

	float GetAccuracy(BYTE byteWeaponId);
	bool SetAccuracy(BYTE byteWeaponId, float fAccuracy);

	static bool IsValid(BYTE byteWeaponId);
	bool IsDoubleHanded(BYTE byteWeaponId);
	int GetSkillID(BYTE byteWeaponId);

private:
	SWeaponInfo *m_pWeaponInfo[MAX_WEAPONS];
};

#endif
