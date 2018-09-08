/* =========================================

FCNPC - Fully Controllable NPC
----------------------

- File: Hooks.cpp
- Author(s): OrMisicL

=========================================*/

#include "Main.hpp"

extern CServer      *pServer;
extern logprintf_t  logprintf;

subhook_t CGameMode__OnPlayerGiveDamage_hook;
subhook_t CGameMode__OnPlayerTakeDamage_hook;
subhook_t CGameMode__OnPlayerWeaponShot_hook;
subhook_t CGameMode__OnPlayerStreamIn_hook;
subhook_t CGameMode__OnPlayerStreamOut_hook;
subhook_t CGameMode__OnGameModeExit_hook;

//----------------------------------------------------

typedef int(THISCALL* FUNC_CGameMode__OnPlayerGiveDamage)(CGameMode *thisptr, cell playerid, cell damagedid, cell amount, cell weaponid, cell bodypart);
#ifdef _WIN32
int FASTCALL HOOK_CGameMode__OnPlayerGiveDamage(CGameMode *thisptr, void *padding, cell playerid, cell damagedid, cell amount, cell weaponid, cell bodypart)
#else
int __attribute__((__cdecl__)) HOOK_CGameMode__OnPlayerGiveDamage(CGameMode *thisptr, cell playerid, cell damagedid, cell amount, cell weaponid, cell bodypart)
#endif
{
	subhook_remove(CGameMode__OnPlayerGiveDamage_hook);

	int ret = 0;

	// get the npc data
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(static_cast<WORD>(damagedid));

	// check on invulnerable
	if (!pPlayerData || !pPlayerData->IsInvulnerable()) {
		// call hooked callback
		ret = ((FUNC_CGameMode__OnPlayerGiveDamage)CAddress::FUNC_CGameMode__OnPlayerGiveDamage)(thisptr, playerid, damagedid, amount, weaponid, bodypart);

		// call custom callback
		if (pPlayerData) {
			pPlayerData->ProcessDamage(static_cast<WORD>(playerid), amx_ctof(amount), static_cast<BYTE>(weaponid), static_cast<int>(bodypart));
		}
	}

	subhook_install(CGameMode__OnPlayerGiveDamage_hook);
	return ret;
}

//----------------------------------------------------

typedef int(THISCALL* FUNC_CGameMode__OnPlayerTakeDamage)(CGameMode *thisptr, cell playerid, cell damagedid, cell amount, cell weaponid, cell bodypart);
#ifdef _WIN32
int FASTCALL HOOK_CGameMode__OnPlayerTakeDamage(CGameMode *thisptr, void *padding, cell playerid, cell damagedid, cell amount, cell weaponid, cell bodypart)
#else
int __attribute__((__cdecl__)) HOOK_CGameMode__OnPlayerTakeDamage(CGameMode *thisptr, cell playerid, cell damagedid, cell amount, cell weaponid, cell bodypart)
#endif
{
	subhook_remove(CGameMode__OnPlayerTakeDamage_hook);

	// call hooked callback
	int ret = ((FUNC_CGameMode__OnPlayerTakeDamage)CAddress::FUNC_CGameMode__OnPlayerTakeDamage)(thisptr, playerid, damagedid, amount, weaponid, bodypart);

	// get the player data
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(static_cast<WORD>(damagedid));

	// call custom callback
	if (pPlayerData) {
		CCallbackManager::OnGiveDamage(static_cast<WORD>(damagedid), static_cast<WORD>(playerid), static_cast<BYTE>(weaponid), static_cast<int>(bodypart), amx_ctof(amount));
	}

	subhook_install(CGameMode__OnPlayerTakeDamage_hook);
	return ret;
}

//----------------------------------------------------

typedef int(THISCALL* FUNC_CGameMode__OnPlayerWeaponShot)(CGameMode *thisptr, cell playerid, cell weaponid, cell hittype, cell hitid, cell *pPosition);
#ifdef _WIN32
int FASTCALL HOOK_CGameMode__OnPlayerWeaponShot(CGameMode *thisptr, void *padding, cell playerid, cell weaponid, cell hittype, cell hitid, cell *pPosition)
#else
int __attribute__((__cdecl__)) HOOK_CGameMode__OnPlayerWeaponShot(CGameMode *thisptr, cell playerid, cell weaponid, cell hittype, cell hitid, cell *pPosition)
#endif
{
	subhook_remove(CGameMode__OnPlayerWeaponShot_hook);

	// call hooked callback
	int ret = ((FUNC_CGameMode__OnPlayerWeaponShot)CAddress::FUNC_CGameMode__OnPlayerWeaponShot)(thisptr, playerid, weaponid, hittype, hitid, pPosition);

	// call custom callback 
	if (hittype == BULLET_HIT_TYPE_VEHICLE) {
		WORD wPlayerId = pServer->GetVehicleSeatPlayerId(static_cast<WORD>(hitid), 0);

		CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wPlayerId);
		if (pPlayerData) {
			CVector vecPos = CVector(amx_ctof(pPosition[0]), amx_ctof(pPosition[1]), amx_ctof(pPosition[2]));
			pPlayerData->ProcessVehicleDamage(static_cast<WORD>(playerid), static_cast<WORD>(hitid), static_cast<BYTE>(weaponid), vecPos);
		}
	}

	subhook_install(CGameMode__OnPlayerWeaponShot_hook);
	return ret;
}

//----------------------------------------------------

typedef int(THISCALL* FUNC_CGameMode__OnPlayerStreamIn)(CGameMode *thisptr, cell playerid, cell forplayerid);
#ifdef _WIN32
int FASTCALL HOOK_CGameMode__OnPlayerStreamIn(CGameMode *thisptr, void *padding, cell playerid, cell forplayerid)
#else
int __attribute__((__cdecl__)) HOOK_CGameMode__OnPlayerStreamIn(CGameMode *thisptr, cell playerid, cell forplayerid)
#endif
{
	subhook_remove(CGameMode__OnPlayerStreamIn_hook);

	int ret = 0;
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(static_cast<WORD>(playerid));
	if (pPlayerData) {
		pPlayerData->ProcessStreamIn(static_cast<WORD>(forplayerid));
	} else {
		ret = ((FUNC_CGameMode__OnPlayerStreamIn)CAddress::FUNC_CGameMode__OnPlayerStreamIn)(thisptr, playerid, forplayerid);
	}

	subhook_install(CGameMode__OnPlayerStreamIn_hook);
	return ret;
}

//----------------------------------------------------

typedef int(THISCALL* FUNC_CGameMode__OnPlayerStreamOut)(CGameMode *thisptr, cell playerid, cell forplayerid);
#ifdef _WIN32
int FASTCALL HOOK_CGameMode__OnPlayerStreamOut(CGameMode *thisptr, void *padding, cell playerid, cell forplayerid)
#else
int __attribute__((__cdecl__)) HOOK_CGameMode__OnPlayerStreamOut(CGameMode *thisptr, cell playerid, cell forplayerid)
#endif
{
	subhook_remove(CGameMode__OnPlayerStreamOut_hook);

	int ret = 0;
	CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(static_cast<WORD>(playerid));
	if (pPlayerData) {
		pPlayerData->ProcessStreamOut(static_cast<WORD>(forplayerid));
	} else {
		ret = ((FUNC_CGameMode__OnPlayerStreamOut)CAddress::FUNC_CGameMode__OnPlayerStreamOut)(thisptr, playerid, forplayerid);
	}

	subhook_install(CGameMode__OnPlayerStreamOut_hook);
	return ret;
}

//----------------------------------------------------

typedef int(THISCALL* FUNC_CGameMode__OnGameModeExit)(CGameMode *thisptr);
#ifdef _WIN32
int FASTCALL HOOK_CGameMode__OnGameModeExit(CGameMode *thisptr, void *padding)
#else
int __attribute__((__cdecl__)) HOOK_CGameMode__OnGameModeExit(CGameMode *thisptr)
#endif
{
	subhook_remove(CGameMode__OnGameModeExit_hook);

	pServer->GetPlayerManager()->ResetAllPlayers();

	int ret = ((FUNC_CGameMode__OnGameModeExit)CAddress::FUNC_CGameMode__OnGameModeExit)(thisptr);
	subhook_install(CGameMode__OnGameModeExit_hook);
	return ret;
}

//----------------------------------------------------

void CHooks::InstallHooks()
{
	CGameMode__OnPlayerGiveDamage_hook = subhook_new(reinterpret_cast<void*>(CAddress::FUNC_CGameMode__OnPlayerGiveDamage), reinterpret_cast<void*>(HOOK_CGameMode__OnPlayerGiveDamage), static_cast<subhook_flags_t>(NULL));
	subhook_install(CGameMode__OnPlayerGiveDamage_hook);

	CGameMode__OnPlayerTakeDamage_hook = subhook_new(reinterpret_cast<void*>(CAddress::FUNC_CGameMode__OnPlayerTakeDamage), reinterpret_cast<void*>(HOOK_CGameMode__OnPlayerTakeDamage), static_cast<subhook_flags_t>(NULL));
	subhook_install(CGameMode__OnPlayerTakeDamage_hook);

	CGameMode__OnPlayerWeaponShot_hook = subhook_new(reinterpret_cast<void*>(CAddress::FUNC_CGameMode__OnPlayerWeaponShot), reinterpret_cast<void*>(HOOK_CGameMode__OnPlayerWeaponShot), static_cast<subhook_flags_t>(NULL));
	subhook_install(CGameMode__OnPlayerWeaponShot_hook);

	CGameMode__OnPlayerStreamIn_hook = subhook_new(reinterpret_cast<void*>(CAddress::FUNC_CGameMode__OnPlayerStreamIn), reinterpret_cast<void*>(HOOK_CGameMode__OnPlayerStreamIn), static_cast<subhook_flags_t>(NULL));
	subhook_install(CGameMode__OnPlayerStreamIn_hook);

	CGameMode__OnPlayerStreamOut_hook = subhook_new(reinterpret_cast<void*>(CAddress::FUNC_CGameMode__OnPlayerStreamOut), reinterpret_cast<void*>(HOOK_CGameMode__OnPlayerStreamOut), static_cast<subhook_flags_t>(NULL));
	subhook_install(CGameMode__OnPlayerStreamOut_hook);

	CGameMode__OnGameModeExit_hook = subhook_new(reinterpret_cast<void*>(CAddress::FUNC_CGameMode__OnGameModeExit), reinterpret_cast<void*>(HOOK_CGameMode__OnGameModeExit), static_cast<subhook_flags_t>(NULL));
	subhook_install(CGameMode__OnGameModeExit_hook);
}
