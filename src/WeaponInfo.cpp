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
	if (iWeaponId < 0) {
		return -1;
	}

	// Dont process melee weapons
	if (iWeaponId < 19) {
		return 0;
	}

	// Get the clip size from the weapon id
	switch(iWeaponId) {
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
		/*case 25:
		case 33:
		case 34:
		case 35:
		case 36:
			return 1;*/

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

// The fastest possible gap between weapon shots in milliseconds by oscar-broman
WORD CWeaponInfo::GetWeaponRateOfFire(int iWeaponId)
{
	// Validate the weapon id
	if (iWeaponId < 0 || iWeaponId > 46) {
		return -1;
	}

	// Get the clip size from the weapon id
	switch (iWeaponId) {
		case 0: // Fist
		case 1: // Brass knuckles
		case 2: // Golf club
		case 3: // Nitestick
		case 4: // Knife
		case 5: // Bat
		case 6: // Shovel
		case 7: // Pool cue
		case 8: // Katana
		case 10: // Dildo
		case 11: // Dildo 2
		case 12: // Vibrator
		case 13: // Vibrator 2
		case 14: // Flowers
		case 15: // Cane
			return 250;

		case 9:
			return 30;

		case 16: // Grenade
		case 17: // Teargas
		case 18: // Molotov
		case 19:
		case 20:
		case 21:
		case 35: // Rocket launcher
		case 36: // Heatseeker
		case 37: // Flamethrower
		case 39: // Satchel
		case 40: // Detonator
		case 43: // Camera
		case 44: // Night vision
		case 45: // Infrared
		case 46: // Parachute
			return 0;

		case 38: // Minigun
			return 20;

		case 22: // Colt 45
			return 160;

		case 23: // Silenced
		case 24: // Deagle
		case 26: // Sawed-off
		case 27: // Spas
			return 120;

		case 25: // Shotgun
		case 33: // Cuntgun
			return 800;

		case 28: // UZI
			return 50;

		case 29: // MP5
		case 30: // AK47
		case 31: // M4
			return 90;

		case 32: // Tec9
			return 70;

		case 34: // Sniper
			return 900;

		case 41: // Spraycan
		case 42: // Fire extinguisher
			return 10;
	}
	return 0;
}

// Gets the weapon damage per bullet
float CWeaponInfo::GetWeaponDamage(int iWeaponId)
{
	// Validate the weapon id
	if (iWeaponId < 0) {
		return 0.0f;
	}

	// Get the clip size from the weapon id
	switch(iWeaponId) {
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

bool CWeaponInfo::IsDoubleHanded(BYTE byteWeaponId)
{
	if (byteWeaponId == 22 || byteWeaponId == 26 || byteWeaponId == 28 || byteWeaponId == 32) {
		return true;
	}

	return false;
}