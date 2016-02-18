/* =========================================

		FCNPC - Fully Controllable NPC
			----------------------

	- File: WeaponInfo.cpp
	- Author(s): OrMisicL, ziggi

  =========================================*/

#include "Main.h"

CWeaponInfo::CWeaponInfo()
{
	for (int i = 0; i < (sizeof(m_pWeaponInfo) / sizeof(m_pWeaponInfo[0])); i++) {
		m_pWeaponInfo[i] = new SWeaponInfo(m_sDefaultWeaponInfo[i]);
	}
}

int CWeaponInfo::GetWeaponType(int iWeaponId)
{
	if (!IsValidWeapon(iWeaponId)) {
		return 0.0f;
	}

	return m_pWeaponInfo[iWeaponId]->iType;
}

bool CWeaponInfo::SetWeaponType(int iWeaponId, int iType)
{
	if (!IsValidWeapon(iWeaponId)) {
		return false;
	}

	m_pWeaponInfo[iWeaponId]->iType = iType;
	return true;
}

float CWeaponInfo::GetWeaponDamage(int iWeaponId)
{
	if (!IsValidWeapon(iWeaponId)) {
		return 0.0f;
	}

	return m_pWeaponInfo[iWeaponId]->fDamage;
}

bool CWeaponInfo::SetWeaponDamage(int iWeaponId, float fDamage)
{
	if (!IsValidWeapon(iWeaponId)) {
		return false;
	}

	m_pWeaponInfo[iWeaponId]->fDamage = fDamage;
	return true;
}

int CWeaponInfo::GetWeaponClipSize(int iWeaponId)
{
	if (!IsValidWeapon(iWeaponId)) {
		return -1;
	}

	return m_pWeaponInfo[iWeaponId]->iClipSize;
}

bool CWeaponInfo::SetWeaponClipSize(int iWeaponId, int iClipSize)
{
	if (!IsValidWeapon(iWeaponId)) {
		return false;
	}

	m_pWeaponInfo[iWeaponId]->iClipSize = iClipSize;
	return true;
}

// The fastest possible gap between weapon shots in milliseconds by oscar-broman
int CWeaponInfo::GetWeaponShootTime(int iWeaponId)
{
	if (!IsValidWeapon(iWeaponId)) {
		return -1;
	}

	return m_pWeaponInfo[iWeaponId]->iShootTime;
}

bool CWeaponInfo::SetWeaponShootTime(int iWeaponId, int iShootTime)
{
	if (!IsValidWeapon(iWeaponId)) {
		return false;
	}

	m_pWeaponInfo[iWeaponId]->iShootTime = iShootTime;
	return true;
}

int CWeaponInfo::GetWeaponReloadTime(int iWeaponId)
{
	if (!IsValidWeapon(iWeaponId)) {
		return -1;
	}

	return m_pWeaponInfo[iWeaponId]->iReloadTime;
}

bool CWeaponInfo::SetWeaponReloadTime(int iWeaponId, int iReloadTime)
{
	if (!IsValidWeapon(iWeaponId)) {
		return false;
	}

	m_pWeaponInfo[iWeaponId]->iReloadTime = iReloadTime;
	return true;
}

bool CWeaponInfo::IsValidWeapon(int iWeaponId)
{
	return iWeaponId > 0 && iWeaponId < MAX_WEAPONS;
}

bool CWeaponInfo::IsDoubleHanded(int iWeaponId)
{
	if (iWeaponId == 22 || iWeaponId == 26 || iWeaponId == 28 || iWeaponId == 32) {
		return true;
	}

	return false;
}
