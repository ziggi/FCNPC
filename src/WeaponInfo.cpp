/* =========================================

		FCNPC - Fully Controllable NPC
			----------------------

	- File: WeaponInfo.cpp
	- Author(s): OrMisicL, ziggi

  =========================================*/

#include "Main.hpp"

static SWeaponInfo g_sDefaultWeaponInfo[MAX_WEAPONS] = {
	{WEAPON_TYPE_MELEE,   5.0f,  0,   250,  0}, // fists (0)
	{WEAPON_TYPE_MELEE,   5.0f,  0,   250,  0}, // WEAPON_BRASSKNUCKLE (1)
	{WEAPON_TYPE_MELEE,   5.0f,  0,   250,  0}, // WEAPON_GOLFCLUK (2)
	{WEAPON_TYPE_MELEE,   5.0f,  0,   250,  0}, // WEAPON_NITESTICE (3)
	{WEAPON_TYPE_MELEE,   5.0f,  0,   250,  0}, // WEAPON_KNIFT (4)
	{WEAPON_TYPE_MELEE,   5.0f,  0,   250,  0}, // WEAPON_BAL (5)
	{WEAPON_TYPE_MELEE,   5.0f,  0,   250,  0}, // WEAPON_SHOVEK (6)
	{WEAPON_TYPE_MELEE,   5.0f,  0,   250,  0}, // WEAPON_POOLSTICA (7)
	{WEAPON_TYPE_MELEE,   5.0f,  0,   250,  0}, // WEAPON_KATANW (8)
	{WEAPON_TYPE_MELEE,   5.0f,  0,   30,   0}, // WEAPON_CHAINSAO (9)
	{WEAPON_TYPE_MELEE,   5.0f,  0,   250,  0}, // WEAPON_DILD (10)
	{WEAPON_TYPE_MELEE,   5.0f,  0,   250,  0}, // WEAPON_DILDO2 (11)
	{WEAPON_TYPE_MELEE,   5.0f,  0,   250,  0}, // WEAPON_VIBRATOR (12)
	{WEAPON_TYPE_MELEE,   5.0f,  0,   250,  0}, // WEAPON_VIBRATOR2 (13)
	{WEAPON_TYPE_MELEE,   5.0f,  0,   250,  0}, // WEAPON_FLOWER (14)
	{WEAPON_TYPE_MELEE,   5.0f,  0,   250,  0}, // WEAPON_CANE (15)
	{WEAPON_TYPE_THROW,   5.0f,  1,   500,  0}, // WEAPON_GRENADE (16)
	{WEAPON_TYPE_THROW,   5.0f,  1,   500,  0}, // WEAPON_TEARGAS (17)
	{WEAPON_TYPE_THROW,   5.0f,  1,   500,  0}, // WEAPON_MOLTOV (18)
	{WEAPON_TYPE_NONE,    0.0f,  0,   0,    0}, // nothing (19)
	{WEAPON_TYPE_NONE,    0.0f,  0,   0,    0}, // nothing (20)
	{WEAPON_TYPE_NONE,    0.0f,  0,   0,    0}, // nothing (21)
	{WEAPON_TYPE_SHOOT,   8.25f, 17,  160,  1500}, // WEAPON_COLT45 (22)
	{WEAPON_TYPE_SHOOT,   13.2f, 17,  120,  1500}, // WEAPON_SILENCED (23)
	{WEAPON_TYPE_SHOOT,   46.2f, 7,   120,  1500}, // WEAPON_DEAGLE (24)
	{WEAPON_TYPE_SHOOT,   30.0f, 1,   800,  0}, // WEAPON_SHOTGUN (25)
	{WEAPON_TYPE_SHOOT,   30.0f, 2,   120,  1500}, // WEAPON_SAWEDOFF (26)
	{WEAPON_TYPE_SHOOT,   30.0f, 7,   120,  1500}, // WEAPON_SHOTGSPA (27)
	{WEAPON_TYPE_SHOOT,   6.6f,  50,  50,   1500}, // WEAPON_UZI (28)
	{WEAPON_TYPE_SHOOT,   8.25f, 30,  90,   1500}, // WEAPON_MP5 (29)
	{WEAPON_TYPE_SHOOT,   9.9f,  30,  90,   1500}, // WEAPON_AK47 (30)
	{WEAPON_TYPE_SHOOT,   9.9f,  50,  90,   1500}, // WEAPON_M4 (31)
	{WEAPON_TYPE_SHOOT,   6.6f,  50,  70,   1500}, // WEAPON_TEC9 (32)
	{WEAPON_TYPE_SHOOT,   24.8f, 1,   1500, 0}, // WEAPON_RIFLE (33)
	{WEAPON_TYPE_SHOOT,   41.3f, 1,   1500, 0}, // WEAPON_SNIPER (34)
	{WEAPON_TYPE_ROCKET,  5.0f,  1,   1500, 0}, // WEAPON_ROCKETLAUNCHER (35)
	{WEAPON_TYPE_ROCKET,  5.0f,  1,   1500, 0}, // WEAPON_HEATSEEKER (36)
	{WEAPON_TYPE_SPRAY,   5.0f,  500, 500,  1000}, // WEAPON_FLAMETHROWER (37)
	{WEAPON_TYPE_SHOOT,   46.2f, 500, 20,   1000}, // WEAPON_MINIGUN (38)
	{WEAPON_TYPE_SPECIAL, 5.0f,  1,   500,  0}, // WEAPON_SATCHEL (39)
	{WEAPON_TYPE_SPECIAL, 5.0f,  1,   500,  0}, // WEAPON_BOMB (40)
	{WEAPON_TYPE_SPRAY,   5.0f,  500, 10,   1000}, // WEAPON_SPRAYCAN (41)
	{WEAPON_TYPE_SPRAY,   5.0f,  500, 10,   1000}, // WEAPON_FIREEXTINGUISHER (42)
	{WEAPON_TYPE_SPECIAL, 0.0f,  1,   500,  0}, // WEAPON_CAMERA (43)
	{WEAPON_TYPE_SPECIAL, 0.0f,  0,   500,  0}, // WEAPON_NIGHTVISION (44)
	{WEAPON_TYPE_SPECIAL, 0.0f,  0,   500,  0}, // WEAPON_INFRARED (45)
	{WEAPON_TYPE_SPECIAL, 0.0f,  1,   500,  0} // WEAPON_PARACHUTE (46)
};

CWeaponInfo::CWeaponInfo()
{
	for (unsigned int i = 0; i < (sizeof(m_pWeaponInfo) / sizeof(m_pWeaponInfo[0])); i++) {
		m_pWeaponInfo[i] = new SWeaponInfo(GetDefaultInfo(i));
	}
}

CWeaponInfo::~CWeaponInfo()
{
	for (unsigned int i = 0; i < (sizeof(m_pWeaponInfo) / sizeof(m_pWeaponInfo[0])); i++) {
		SAFE_DELETE(m_pWeaponInfo[i]);
	}
}

bool CWeaponInfo::SetDefaultInfo(int iWeaponId, SWeaponInfo sWeaponInfo)
{
	if (!CWeaponInfo::IsValid(iWeaponId)) {
		return false;
	}

	g_sDefaultWeaponInfo[iWeaponId] = sWeaponInfo;
	return true;
}

SWeaponInfo CWeaponInfo::GetDefaultInfo(int iWeaponId)
{
	if (!CWeaponInfo::IsValid(iWeaponId)) {
		return SWeaponInfo();
	}

	return g_sDefaultWeaponInfo[iWeaponId];
}

bool CWeaponInfo::SetInfo(int iWeaponId, SWeaponInfo sWeaponInfo)
{
	if (!IsValid(iWeaponId)) {
		return false;
	}

	*m_pWeaponInfo[iWeaponId] = sWeaponInfo;
	return true;
}

SWeaponInfo CWeaponInfo::GetInfo(int iWeaponId)
{
	if (!IsValid(iWeaponId)) {
		return SWeaponInfo();
	}

	return *m_pWeaponInfo[iWeaponId];
}

int CWeaponInfo::GetType(int iWeaponId)
{
	if (!IsValid(iWeaponId)) {
		return 0.0f;
	}

	return m_pWeaponInfo[iWeaponId]->iType;
}

bool CWeaponInfo::SetType(int iWeaponId, int iType)
{
	if (!IsValid(iWeaponId)) {
		return false;
	}

	m_pWeaponInfo[iWeaponId]->iType = iType;
	return true;
}

float CWeaponInfo::GetDamage(int iWeaponId)
{
	if (!IsValid(iWeaponId)) {
		return 0.0f;
	}

	return m_pWeaponInfo[iWeaponId]->fDamage;
}

bool CWeaponInfo::SetDamage(int iWeaponId, float fDamage)
{
	if (!IsValid(iWeaponId)) {
		return false;
	}

	m_pWeaponInfo[iWeaponId]->fDamage = fDamage;
	return true;
}

int CWeaponInfo::GetClipSize(int iWeaponId)
{
	if (!IsValid(iWeaponId)) {
		return -1;
	}

	return m_pWeaponInfo[iWeaponId]->iClipSize;
}

bool CWeaponInfo::SetClipSize(int iWeaponId, int iClipSize)
{
	if (!IsValid(iWeaponId)) {
		return false;
	}

	m_pWeaponInfo[iWeaponId]->iClipSize = iClipSize;
	return true;
}

// The fastest possible gap between weapon shots in milliseconds by oscar-broman
int CWeaponInfo::GetShootTime(int iWeaponId)
{
	if (!IsValid(iWeaponId)) {
		return -1;
	}

	return m_pWeaponInfo[iWeaponId]->iShootTime;
}

bool CWeaponInfo::SetShootTime(int iWeaponId, int iShootTime)
{
	if (!IsValid(iWeaponId)) {
		return false;
	}

	m_pWeaponInfo[iWeaponId]->iShootTime = iShootTime;
	return true;
}

int CWeaponInfo::GetReloadTime(int iWeaponId)
{
	if (!IsValid(iWeaponId)) {
		return -1;
	}

	return m_pWeaponInfo[iWeaponId]->iReloadTime;
}

bool CWeaponInfo::SetReloadTime(int iWeaponId, int iReloadTime)
{
	if (!IsValid(iWeaponId)) {
		return false;
	}

	m_pWeaponInfo[iWeaponId]->iReloadTime = iReloadTime;
	return true;
}

bool CWeaponInfo::IsValid(int iWeaponId)
{
	return iWeaponId >= 0 && iWeaponId < MAX_WEAPONS;
}

bool CWeaponInfo::IsDoubleHanded(int iWeaponId)
{
	if (iWeaponId == 22 || iWeaponId == 26 || iWeaponId == 28 || iWeaponId == 32) {
		return true;
	}

	return false;
}

int CWeaponInfo::GetSkillID(int iWeaponId)
{
	static int iSkills[] = {
		WEAPONSKILL_PISTOL, // 22
		WEAPONSKILL_PISTOL_SILENCED, // 23
		WEAPONSKILL_DESERT_EAGLE, // 24
		WEAPONSKILL_SHOTGUN, // 25
		WEAPONSKILL_SAWNOFF_SHOTGUN, // 26
		WEAPONSKILL_SPAS12_SHOTGUN, // 27
		WEAPONSKILL_MICRO_UZI, // 28
		WEAPONSKILL_MP5, // 29
		WEAPONSKILL_AK47, // 30
		WEAPONSKILL_M4, // 31
		WEAPONSKILL_MICRO_UZI, // 32
		WEAPONSKILL_SNIPERRIFLE, // 33
		WEAPONSKILL_SNIPERRIFLE // 34
	};

	if (!IsValid(iWeaponId)) {
		return -1;
	}

	return iSkills[iWeaponId - 22];
}
