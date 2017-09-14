/* =========================================

		FCNPC - Fully Controllable NPC
			----------------------

	- File: Hooks.cpp
	- Author(s): OrMisicL

  =========================================*/

#include "Main.hpp"

extern CServer      *pServer;
extern logprintf_t  logprintf;
extern void         *pAMXFunctions;

BYTE bytePushCount;
char szPreviousFuncName[32];
bool bHookIsExecStart;
bool bHookIsExecEnd;
bool bHookIsPush;
bool bIsPublicFound;

// damage
struct t_OnPlayerDamage
{
	WORD wPlayerId;
	WORD wSecondPlayerId;
	float fHealthLoss;
	BYTE byteWeaponId;
	int iBodypart;
};

bool bTakeDamage;
bool bGiveDamage;

t_OnPlayerDamage pDamage;

// weapon shoot
bool bWeaponShot;

struct t_OnPlayerWeaponShot
{
	WORD wPlayerId;
	BYTE byteWeaponId;
	int iHitType;
	WORD wHitId;
	CVector vecHit;
};

t_OnPlayerWeaponShot pWeaponShot;

// stream in/out
struct t_OnPlayerStream
{
	WORD wPlayerId;
	WORD wForPlayerId;
};

t_OnPlayerStream pStream;

bool bStreamIn;
bool bStreamOut;

// gamemode exit
bool bOnGameModeExit;

// subhook
subhook_t hookFindPublic;
subhook_t hookPush;
subhook_t hookExec;

// amx_FindPublic function definition
typedef int(*amx_FindPublic_t)(AMX *amx, const char *funcname, int *index);
amx_FindPublic_t pfn_amx_FindPublic = NULL;

// amx_Push function definition
typedef int(*amx_Push_t)(AMX *amx, cell value);
amx_Push_t pfn_amx_Push = NULL;

// amx_Exec function definition
typedef int(*amx_Exec_t)(AMX *amx, long *retval, int index);
amx_Exec_t pfn_amx_Exec = NULL;

int amx_FindPublic_Hook(AMX *amx, const char *funcname, int *index)
{
	pfn_amx_FindPublic = (amx_FindPublic_t)(subhook_get_trampoline(hookFindPublic));

	if (bHookIsExecStart) {
		if (!bHookIsExecEnd) {
			return pfn_amx_FindPublic(amx, funcname, index);
		}

		// exec is complete
		bytePushCount = 0;
		bHookIsExecStart = false;
		bHookIsExecEnd = false;
		bHookIsPush = false;
		bIsPublicFound = false;
		szPreviousFuncName[0] = '\0';
		bGiveDamage = false;
		bTakeDamage = false;
		bWeaponShot = false;
		bStreamIn = false;
		bStreamOut = false;
		bOnGameModeExit = false;
	}

	if (!strcmp(funcname, "OnPlayerGiveDamage")) {
		bIsPublicFound = true;
		bGiveDamage = true;
	} else if (!strcmp(funcname, "OnPlayerTakeDamage")) {
		bIsPublicFound = true;
		bTakeDamage = true;
	} else if (!strcmp(funcname, "OnPlayerWeaponShot")) {
		bIsPublicFound = true;
		bWeaponShot = true;
	} else if (!strcmp(funcname, "OnPlayerStreamIn")) {
		bIsPublicFound = true;
		bStreamIn = true;
	} else if (!strcmp(funcname, "OnPlayerStreamOut")) {
		bIsPublicFound = true;
		bStreamOut = true;
	} else if (!strcmp(funcname, "OnGameModeExit")) {
		bIsPublicFound = true;
		bOnGameModeExit = true;
	}

	if (bIsPublicFound) {
		strlcpy(szPreviousFuncName, funcname, sizeof(szPreviousFuncName));
	}

	return pfn_amx_FindPublic(amx, funcname, index);
}

int amx_Push_Hook(AMX *amx, cell value)
{
	pfn_amx_Push = (amx_Push_t)(subhook_get_trampoline(hookPush));

	if (bHookIsExecStart && !bHookIsExecEnd) {
		return pfn_amx_Push(amx, value);
	}

	// Are we retrieving parameters ?
	if (bGiveDamage || bTakeDamage) {
		bHookIsPush = true;

		switch (bytePushCount) {
			case 4:
				pDamage.wPlayerId = static_cast<WORD>(value);
				break;

			case 3:
				pDamage.wSecondPlayerId = static_cast<WORD>(value);
				break;

			case 2:
				pDamage.fHealthLoss = amx_ctof(value);
				break;

			case 1:
				pDamage.byteWeaponId = static_cast<BYTE>(value);
				break;

			case 0:
				pDamage.iBodypart = static_cast<int>(value);
				break;
		}

		// Increase the parameters count
		bytePushCount++;
	} else if (bWeaponShot) {
		bHookIsPush = true;

		switch (bytePushCount) {
			case 6:
				pWeaponShot.wPlayerId = static_cast<WORD>(value);
				break;

			case 5:
				pWeaponShot.byteWeaponId = static_cast<BYTE>(value);
				break;

			case 4:
				pWeaponShot.iHitType = static_cast<int>(value);
				break;

			case 3:
				pWeaponShot.wHitId = static_cast<WORD>(value);
				break;

			case 2:
				pWeaponShot.vecHit.fX = amx_ctof(value);
				break;

			case 1:
				pWeaponShot.vecHit.fY = amx_ctof(value);
				break;

			case 0:
				pWeaponShot.vecHit.fZ = amx_ctof(value);
				break;
		}

		// Increase the parameters count
		bytePushCount++;
	} else if (bStreamIn) {
		bHookIsPush = true;

		switch (bytePushCount) {
			case 1:
				pStream.wPlayerId = static_cast<WORD>(value);
				break;

			case 0:
				pStream.wForPlayerId = static_cast<WORD>(value);
				break;
		}

		// Increase the parameters count
		bytePushCount++;
	} else if (bStreamOut) {
		bHookIsPush = true;

		switch (bytePushCount) {
			case 1:
				pStream.wPlayerId = static_cast<WORD>(value);
				break;

			case 0:
				pStream.wForPlayerId = static_cast<WORD>(value);
				break;
		}

		// Increase the parameters count
		bytePushCount++;
	}

	return pfn_amx_Push(amx, value);
}

int amx_Exec_Hook(AMX *amx, long *retval, int index)
{
	pfn_amx_Exec = (amx_Exec_t)(subhook_get_trampoline(hookExec));

	if (bHookIsExecStart && !bHookIsExecEnd) {
		return pfn_amx_Exec(amx, retval, index);
	}

	int ret = 0;

	if (bGiveDamage) {
		bHookIsExecStart = true;

		// get the npc data
		CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(pDamage.wSecondPlayerId);

		// check on invulnerable
		if (!pPlayerData || !pPlayerData->IsInvulnerable()) {
			// call hooked callback
			ret = pfn_amx_Exec(amx, retval, index);

			// call custom callback
			if (pPlayerData) {
				pPlayerData->ProcessDamage(pDamage.wPlayerId, pDamage.fHealthLoss, pDamage.byteWeaponId, pDamage.iBodypart);
			}
		}

		bHookIsExecEnd = true;
	} else if (bTakeDamage) {
		bHookIsExecStart = true;

		// call hooked callback
		ret = pfn_amx_Exec(amx, retval, index);

		// get the player data
		CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(pDamage.wSecondPlayerId);

		// call custom callback
		if (pPlayerData) {
			CCallbackManager::OnGiveDamage(pDamage.wSecondPlayerId, pDamage.wPlayerId, pDamage.byteWeaponId, pDamage.iBodypart, pDamage.fHealthLoss);
		}

		bHookIsExecEnd = true;
	} else if (bWeaponShot) {
		bHookIsExecStart = true;

		// call hooked callback
		ret = pfn_amx_Exec(amx, retval, index);

		// call custom callback
		if (pWeaponShot.iHitType == BULLET_HIT_TYPE_VEHICLE) {
			WORD wPlayerId = pServer->GetVehicleSeatPlayerId(pWeaponShot.wHitId, 0);

			CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(wPlayerId);
			if (pPlayerData) {
				pPlayerData->ProcessVehicleDamage(pWeaponShot.wPlayerId, pWeaponShot.wHitId, pWeaponShot.byteWeaponId, pWeaponShot.vecHit);
			}
		}

		bHookIsExecEnd = true;
	} else if (bStreamIn) {
		bHookIsExecStart = true;

		CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(pStream.wPlayerId);
		if (pPlayerData) {
			pPlayerData->ProcessStreamIn(pStream.wForPlayerId);
		} else {
			ret = pfn_amx_Exec(amx, retval, index);
		}

		bHookIsExecEnd = true;
	} else if (bStreamOut) {
		bHookIsExecStart = true;

		CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(pStream.wPlayerId);
		if (pPlayerData) {
			pPlayerData->ProcessStreamOut(pStream.wForPlayerId);
		} else {
			ret = pfn_amx_Exec(amx, retval, index);
		}

		bHookIsExecEnd = true;
	} else if (bOnGameModeExit) {
		bHookIsExecStart = true;

		// reset all NPCs
		pServer->GetPlayerManager()->ResetAllPlayers();

		// call hooked callback
		ret = pfn_amx_Exec(amx, retval, index);

		bHookIsExecEnd = true;
	} else {
		ret = pfn_amx_Exec(amx, retval, index);
	}

	return ret;
}

void CHooks::InstallHooks()
{
	// Reset public flag
	bHookIsExecStart = false;
	bHookIsExecEnd = false;
	bHookIsPush = false;
	bIsPublicFound = false;
	bGiveDamage = false;
	bTakeDamage = false;
	bWeaponShot = false;
	bStreamIn = false;
	bStreamOut = false;

	// Find the function pointers
	BYTE *pFindPublic = *(BYTE **)((DWORD)pAMXFunctions + PLUGIN_AMX_EXPORT_FindPublic * 4);
	BYTE *pPush = *(BYTE **)((DWORD)pAMXFunctions + PLUGIN_AMX_EXPORT_Push * 4);
	BYTE *pExec = *(BYTE **)((DWORD)pAMXFunctions + PLUGIN_AMX_EXPORT_Exec * 4);

	// Hook for amx_FindPublic
	hookFindPublic = subhook_new(pFindPublic, (BYTE *)&amx_FindPublic_Hook, (subhook_options_t)0);
	subhook_install(hookFindPublic);

	// Hook for amx_Push
	hookPush = subhook_new(pPush, (BYTE *)&amx_Push_Hook, (subhook_options_t)0);
	subhook_install(hookPush);

	// Hook for amx_Exec
	hookExec = subhook_new(pExec, (BYTE *)&amx_Exec_Hook, (subhook_options_t)0);
	subhook_install(hookExec);
}
