/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: WeaponInfo.cpp
	- Author(s): OrMisicL

  =========================================*/

#include "Main.h"

DWORD CWeaponInfo::GetWeaponClipSize(int iWeaponId)
{
	// Validate the weapon id
	if(iWeaponId < 0)
		return -1;

	// Dont process melee weapons
	if(iWeaponId < 19)
		return 0;

	// Get the clip size from the weapon id
	switch(iWeaponId)
	{
		// 9mm and Silenced 9mm
		case 22:
		case 23:
			return 17;

		// Desert Eagle and Combat Shotgun
		case 24:
		case 27:
			return 7;

		// Tec-9 and Uzi
		case 32:
		case 28:
			return 20;

		// MP5 and AK47
		case 29:
		case 30:
			return 30;

		// Shotgun and Country Rifle and Sniper Rifle and RPG and HS Rocket
		case 25:
		case 33:
		case 34:
		case 35:
		case 36:
			return 1;

		// Sawnoff Shotgun
		case 26:
			return 2;

		// M4
		case 31:
			return 50;

		// Flamethrower and Minigun
		case 37:
		case 38:
			return 500;
	}
	return 0;
}

// Gets the bullets shot per second (not accurate)
WORD CWeaponInfo::GetWeaponRateOfFire(int iWeaponId)
{
	// Validate the weapon id
	if(iWeaponId < 0)
		return -1;

	// Dont process melee weapons
	if(iWeaponId < 19)
		return 0;

	// Get the clip size from the weapon id
	switch(iWeaponId)
	{
		// 9mm and Silenced 9mm
		case 22:
		case 23:
			return 2;

		// Desert Eagle and Combat Shotgun
		case 24:
		case 27:
			return 1;

		// Tec-9 and Uzi
		case 32:
		case 28:
			return 5;

		// MP5 and AK47
		case 29:
		case 30:
			return 8;

		// Shotgun and Country Rifle and Sniper Rifle and RPG and HS Rocket
		case 25:
		case 33:
		case 34:
		case 35:
		case 36:
			return 1;

		// Sawnoff Shotgun
		case 26:
			return 2;

		// M4
		case 31:
			return 9;

		// Flamethrower and Minigun
		case 37:
		case 38:
			return 9;
	}
	return 0;
}

// Gets the weapon damage per bullet
float CWeaponInfo::GetWeaponDamage(int iWeaponId)
{
	// Validate the weapon id
	if(iWeaponId < 0)
		return 0.0f;

	// Get the clip size from the weapon id
	switch(iWeaponId)
	{
		case 0:
		case 41:
		case 42:
			return 1.0f;

		case 1:
		case 2:
		case 3:
			return 5.0f;

		case 4:
		case 27:
			return 15.0f;

		case 5:
		case 6:
		case 7:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
		case 25:
		case 26:
			return 10.0f;

		case 8:
		case 30:
		case 31:
			return 30.0f;

		case 9:
		case 23:
			return 40.0f;

		case 16:
		case 18:
		case 33:
		case 35:
		case 36:
		case 39:
			return 75.0f;

		case 17:
		case 19:
		case 20:
		case 21:
		case 43:
		case 44:
		case 45:
		case 46:
			return 0.0f;

		case 22:
		case 32:
		case 37:
			return 25.0f;

		case 24:
			return 70.0f;

		case 28:
		case 29:
			return 20.0f;

		case 34:
		case 38:
			return 100.0f;

		case 40:
		case 47:
			return 50.0f;
	}
	return 0.0f;
}