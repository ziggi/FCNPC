/* =========================================

		FCNPC - Fully Controllable NPC
			----------------------

	- File: WeaponInfo.cpp
	- Author(s): OrMisicL, ziggi

  =========================================*/

#include "Main.hpp"

static SWeaponInfo g_sDefaultWeaponInfo[MAX_WEAPONS] = {
	{WEAPON_TYPE_MELEE,   5.0f,  1.6f,   0,   250,  0,    1.0f}, // fists (0)
	{WEAPON_TYPE_MELEE,   5.0f,  1.6f,   0,   250,  0,    1.0f}, // WEAPON_BRASSKNUCKLE (1)
	{WEAPON_TYPE_MELEE,   5.0f,  1.6f,   0,   250,  0,    1.0f}, // WEAPON_GOLFCLUB (2)
	{WEAPON_TYPE_MELEE,   5.0f,  1.6f,   0,   250,  0,    1.0f}, // WEAPON_NITESTICK (3)
	{WEAPON_TYPE_MELEE,   5.0f,  1.6f,   0,   250,  0,    1.0f}, // WEAPON_KNIFE (4)
	{WEAPON_TYPE_MELEE,   5.0f,  1.6f,   0,   250,  0,    1.0f}, // WEAPON_BAT (5)
	{WEAPON_TYPE_MELEE,   5.0f,  1.6f,   0,   250,  0,    1.0f}, // WEAPON_SHOVEL (6)
	{WEAPON_TYPE_MELEE,   5.0f,  1.6f,   0,   250,  0,    1.0f}, // WEAPON_POOLSTICK (7)
	{WEAPON_TYPE_MELEE,   5.0f,  1.6f,   0,   250,  0,    1.0f}, // WEAPON_KATANA (8)
	{WEAPON_TYPE_MELEE,   5.0f,  1.6f,   0,   30,   0,    1.0f}, // WEAPON_CHAINSAW (9)
	{WEAPON_TYPE_MELEE,   5.0f,  1.6f,   0,   250,  0,    1.0f}, // WEAPON_DILDO (10)
	{WEAPON_TYPE_MELEE,   5.0f,  1.6f,   0,   250,  0,    1.0f}, // WEAPON_DILDO2 (11)
	{WEAPON_TYPE_MELEE,   5.0f,  1.6f,   0,   250,  0,    1.0f}, // WEAPON_VIBRATOR (12)
	{WEAPON_TYPE_MELEE,   5.0f,  1.6f,   0,   250,  0,    1.0f}, // WEAPON_VIBRATOR2 (13)
	{WEAPON_TYPE_MELEE,   5.0f,  1.6f,   0,   250,  0,    1.0f}, // WEAPON_FLOWER (14)
	{WEAPON_TYPE_MELEE,   5.0f,  1.6f,   0,   250,  0,    1.0f}, // WEAPON_CANE (15)
	{WEAPON_TYPE_THROW,   5.0f,  40.0f,  1,   500,  0,    1.0f}, // WEAPON_GRENADE (16)
	{WEAPON_TYPE_THROW,   5.0f,  40.0f,  1,   500,  0,    1.0f}, // WEAPON_TEARGAS (17)
	{WEAPON_TYPE_THROW,   5.0f,  40.0f,  1,   500,  0,    1.0f}, // WEAPON_MOLTOV (18)
	{WEAPON_TYPE_NONE,    0.0f,  0.0f,   0,   0,    0,    1.0f}, // nothing (19)
	{WEAPON_TYPE_NONE,    0.0f,  0.0f,   0,   0,    0,    1.0f}, // nothing (20)
	{WEAPON_TYPE_NONE,    0.0f,  0.0f,   0,   0,    0,    1.0f}, // nothing (21)
	{WEAPON_TYPE_SHOOT,   8.25f, 35.0f,  17,  350,  1300, 1.0f}, // WEAPON_COLT45 (22)
	{WEAPON_TYPE_SHOOT,   13.2f, 35.0f,  17,  450,  1300, 1.0f}, // WEAPON_SILENCED (23)
	{WEAPON_TYPE_SHOOT,   46.2f, 35.0f,  7,   950,  1300, 1.0f}, // WEAPON_DEAGLE (24)
	{WEAPON_TYPE_SHOOT,   30.0f, 40.0f,  1,   1100, 0,    1.0f}, // WEAPON_SHOTGUN (25)
	{WEAPON_TYPE_SHOOT,   30.0f, 35.0f,  2,   300,  1300, 1.0f}, // WEAPON_SAWEDOFF (26)
	{WEAPON_TYPE_SHOOT,   30.0f, 40.0f,  7,   400,  1500, 1.0f}, // WEAPON_SHOTGSPA (27)
	{WEAPON_TYPE_SHOOT,   6.6f,  35.0f,  50,  110,  1500, 1.0f}, // WEAPON_UZI (28)
	{WEAPON_TYPE_SHOOT,   8.25f, 45.0f,  30,  95,   1650, 1.0f}, // WEAPON_MP5 (29)
	{WEAPON_TYPE_SHOOT,   9.9f,  70.0f,  30,  120,  1900, 1.0f}, // WEAPON_AK47 (30)
	{WEAPON_TYPE_SHOOT,   9.9f,  90.0f,  50,  120,  1900, 1.0f}, // WEAPON_M4 (31)
	{WEAPON_TYPE_SHOOT,   6.6f,  35.0f,  50,  110,  1500, 1.0f}, // WEAPON_TEC9 (32)
	{WEAPON_TYPE_SHOOT,   24.8f, 100.0f, 1,   1050, 0,    1.0f}, // WEAPON_RIFLE (33)
	{WEAPON_TYPE_SHOOT,   41.3f, 100.0f, 1,   1050, 0,    1.0f}, // WEAPON_SNIPER (34)
	{WEAPON_TYPE_ROCKET,  5.0f,  55.0f,  1,   1050, 0,    1.0f}, // WEAPON_ROCKETLAUNCHER (35)
	{WEAPON_TYPE_ROCKET,  5.0f,  55.0f,  1,   1050, 0,    1.0f}, // WEAPON_HEATSEEKER (36)
	{WEAPON_TYPE_SPRAY,   5.0f,  5.1f,   500, 500,  500,  1.0f}, // WEAPON_FLAMETHROWER (37)
	{WEAPON_TYPE_SHOOT,   46.2f, 75.0f,  500, 20,   200,  1.0f}, // WEAPON_MINIGUN (38)
	{WEAPON_TYPE_SPECIAL, 5.0f,  40.0f,  1,   500,  0,    1.0f}, // WEAPON_SATCHEL (39)
	{WEAPON_TYPE_SPECIAL, 5.0f,  25.0f,  1,   500,  0,    1.0f}, // WEAPON_BOMB (40)
	{WEAPON_TYPE_SPRAY,   5.0f,  6.1f,   500, 10,   200,  1.0f}, // WEAPON_SPRAYCAN (41)
	{WEAPON_TYPE_SPRAY,   5.0f,  10.1f,  500, 10,   200,  1.0f}, // WEAPON_FIREEXTINGUISHER (42)
	{WEAPON_TYPE_SPECIAL, 0.0f,  100.0f, 1,   1200, 0,    1.0f}, // WEAPON_CAMERA (43)
	{WEAPON_TYPE_SPECIAL, 0.0f,  200.0f, 0,   1500, 0,    1.0f}, // WEAPON_NIGHTVISION (44)
	{WEAPON_TYPE_SPECIAL, 0.0f,  200.0f, 0,   1500, 0,    1.0f}, // WEAPON_INFRARED (45)

	{WEAPON_TYPE_SPECIAL, 0.0f,  1.6f,  1,   1500, 0,    1.0f} // WEAPON_PARACHUTE (46)
};

CWeaponInfo::CWeaponInfo()
{
	for (BYTE i = 0; i < (sizeof(m_pWeaponInfo) / sizeof(m_pWeaponInfo[0])); i++) {
		m_pWeaponInfo[i] = new SWeaponInfo(GetDefaultInfo(i));
	}
}

CWeaponInfo::~CWeaponInfo()
{
	for (BYTE i = 0; i < (sizeof(m_pWeaponInfo) / sizeof(m_pWeaponInfo[0])); i++) {
		SAFE_DELETE(m_pWeaponInfo[i]);
	}
}

bool CWeaponInfo::SetDefaultInfo(BYTE byteWeaponId, const SWeaponInfo &sWeaponInfo)
{
	if (!CWeaponInfo::IsValid(byteWeaponId)) {
		return false;
	}

	g_sDefaultWeaponInfo[byteWeaponId] = sWeaponInfo;
	return true;
}

SWeaponInfo CWeaponInfo::GetDefaultInfo(BYTE byteWeaponId)
{
	if (!CWeaponInfo::IsValid(byteWeaponId)) {
		return SWeaponInfo();
	}

	return g_sDefaultWeaponInfo[byteWeaponId];
}

bool CWeaponInfo::SetInfo(BYTE byteWeaponId, const SWeaponInfo &sWeaponInfo)
{
	if (!IsValid(byteWeaponId)) {
		return false;
	}

	*m_pWeaponInfo[byteWeaponId] = sWeaponInfo;
	return true;
}

SWeaponInfo CWeaponInfo::GetInfo(BYTE byteWeaponId)
{
	if (!IsValid(byteWeaponId)) {
		return SWeaponInfo();
	}

	return *m_pWeaponInfo[byteWeaponId];
}

int CWeaponInfo::GetType(BYTE byteWeaponId)
{
	if (!IsValid(byteWeaponId)) {
		return 0;
	}

	return m_pWeaponInfo[byteWeaponId]->iType;
}

bool CWeaponInfo::SetType(BYTE byteWeaponId, int iType)
{
	if (!IsValid(byteWeaponId)) {
		return false;
	}

	m_pWeaponInfo[byteWeaponId]->iType = iType;
	return true;
}

float CWeaponInfo::GetDamage(BYTE byteWeaponId)
{
	if (!IsValid(byteWeaponId)) {
		return 0.0f;
	}

	return m_pWeaponInfo[byteWeaponId]->fDamage;
}

bool CWeaponInfo::SetDamage(BYTE byteWeaponId, float fDamage)
{
	if (!IsValid(byteWeaponId)) {
		return false;
	}

	m_pWeaponInfo[byteWeaponId]->fDamage = fDamage;
	return true;
}

float CWeaponInfo::GetRange(BYTE byteWeaponId)
{
	if (!IsValid(byteWeaponId)) {
		return 0.0f;
	}

	return m_pWeaponInfo[byteWeaponId]->fRange;
}

bool CWeaponInfo::SetRange(BYTE byteWeaponId, float fRange)
{
	if (!IsValid(byteWeaponId)) {
		return false;
	}

	m_pWeaponInfo[byteWeaponId]->fRange = fRange;
	return true;
}

int CWeaponInfo::GetClipSize(BYTE byteWeaponId)
{
	if (!IsValid(byteWeaponId)) {
		return -1;
	}

	return m_pWeaponInfo[byteWeaponId]->iClipSize;
}

bool CWeaponInfo::SetClipSize(BYTE byteWeaponId, int iClipSize)
{
	if (!IsValid(byteWeaponId)) {
		return false;
	}

	m_pWeaponInfo[byteWeaponId]->iClipSize = iClipSize;
	return true;
}

// The fastest possible gap between weapon shots in milliseconds by oscar-broman
int CWeaponInfo::GetShootTime(BYTE byteWeaponId)
{
	if (!IsValid(byteWeaponId)) {
		return -1;
	}

	return m_pWeaponInfo[byteWeaponId]->iShootTime;
}

bool CWeaponInfo::SetShootTime(BYTE byteWeaponId, int iShootTime)
{
	if (!IsValid(byteWeaponId)) {
		return false;
	}

	m_pWeaponInfo[byteWeaponId]->iShootTime = iShootTime;
	return true;
}

int CWeaponInfo::GetReloadTime(BYTE byteWeaponId)
{
	if (!IsValid(byteWeaponId)) {
		return -1;
	}

	return m_pWeaponInfo[byteWeaponId]->iReloadTime;
}

bool CWeaponInfo::SetReloadTime(BYTE byteWeaponId, int iReloadTime)
{
	if (!IsValid(byteWeaponId)) {
		return false;
	}

	m_pWeaponInfo[byteWeaponId]->iReloadTime = iReloadTime;
	return true;
}

float CWeaponInfo::GetAccuracy(BYTE byteWeaponId)
{
	if (!IsValid(byteWeaponId)) {
		return 0.0f;
	}

	return m_pWeaponInfo[byteWeaponId]->fAccuracy;
}

bool CWeaponInfo::SetAccuracy(BYTE byteWeaponId, float fAccuracy)
{
	if (!IsValid(byteWeaponId)) {
		return false;
	}

	m_pWeaponInfo[byteWeaponId]->fAccuracy = fAccuracy;
	return true;
}

bool CWeaponInfo::IsValid(BYTE byteWeaponId)
{
	return byteWeaponId < MAX_WEAPONS;
}

bool CWeaponInfo::IsDoubleHanded(BYTE byteWeaponId)
{
	switch (byteWeaponId) {
		case WEAPON_COLT45:
		case WEAPON_SAWEDOFF:
		case WEAPON_UZI:
		case WEAPON_TEC9:
			return true;
	}

	return false;
}

int CWeaponInfo::GetSkillID(BYTE byteWeaponId)
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

	if (!IsValid(byteWeaponId)) {
		return -1;
	}

	return iSkills[byteWeaponId - 22];
}
