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

// give damage
bool bGiveDamage;

struct t_OnPlayerGiveDamage {
	int iPlayerId;
	int iDamagedId;
	float fHealthLoss;
	int iWeapon;
	int iBodypart;
};

t_OnPlayerGiveDamage pGiveDamage;

// take damage
bool bTakeDamage;
bool bTakeDamageInternalCall;

struct t_OnPlayerTakeDamage {
	int iPlayerId;
	int iDamagerId;
	float fHealthLoss;
	int iWeapon;
	int iBodypart;
};

t_OnPlayerTakeDamage pTakeDamage;

// weapon shoot
bool bWeaponShot;

struct t_OnPlayerWeaponShot {
	int iPlayerId;
	int iWeaponId;
	int iHitType;
	int iHitId;
	CVector vecHit;
};

t_OnPlayerWeaponShot pWeaponShot;

// stream in/out
struct t_OnPlayerStream {
	int iPlayerId;
	int iForPlayerId;
};

t_OnPlayerStream pStreamIn;
t_OnPlayerStream pStreamOut;

bool bStreamIn;
bool bStreamOut;

// subhook
subhook_t hookFindPublic;
subhook_t hookPush;
subhook_t hookExec;

// amx_FindPublic function definition
typedef int (* amx_FindPublic_t)(AMX *amx, const char *funcname, int *index);
amx_FindPublic_t pfn_amx_FindPublic = NULL;

// amx_Push function definition
typedef int(*amx_Push_t)(AMX *amx, cell value);
amx_Push_t pfn_amx_Push = NULL;

// amx_Exec function definition
typedef int(*amx_Exec_t)(AMX *amx, long *retval, int index);
amx_Exec_t pfn_amx_Exec = NULL;

int amx_FindPublic_Hook(AMX *amx, const char *funcname, int *index)
{
	if (!strcmp(funcname, "OnPlayerGiveDamage")) {
		bGiveDamage = true;
	} else if (!strcmp(funcname, "OnPlayerTakeDamage")) {
		if (bTakeDamageInternalCall) {
			bTakeDamageInternalCall = false;
		} else {
			bTakeDamage = true;
		}
	} else if (!strcmp(funcname, "OnPlayerWeaponShot")) {
		bWeaponShot = true;
	} else if (!strcmp(funcname, "OnPlayerStreamIn")) {
		bStreamIn = true;
	} else if (!strcmp(funcname, "OnPlayerStreamOut")) {
		bStreamOut = true;
	}

	bytePushCount = 0;

	pfn_amx_FindPublic = (amx_FindPublic_t)(subhook_get_trampoline(hookFindPublic));

	return pfn_amx_FindPublic(amx, funcname, index);
}

int amx_Push_Hook(AMX *amx, cell value)
{
	// Are we retrieving parameters ?
	if (bGiveDamage) {
		switch (bytePushCount) {
			case 4:
				pGiveDamage.iPlayerId = value;
				break;

			case 3:
				pGiveDamage.iDamagedId = value;
				break;

			case 2:
				pGiveDamage.fHealthLoss = amx_ctof(value);
				break;

			case 1:
				pGiveDamage.iWeapon = value;
				break;

			case 0:
				pGiveDamage.iBodypart = value;
				break;
		}
		// Increase the parameters count
		bytePushCount++;
	} else if (bTakeDamage) {
		switch (bytePushCount) {
			case 4:
				pTakeDamage.iPlayerId = value;
				break;

			case 3:
				pTakeDamage.iDamagerId = value;
				break;

			case 2:
				pTakeDamage.fHealthLoss = amx_ctof(value);
				break;

			case 1:
				pTakeDamage.iWeapon = value;
				break;

			case 0:
				pTakeDamage.iBodypart = value;
				break;
		}
		// Increase the parameters count
		bytePushCount++;
	} else if (bWeaponShot) {
		switch (bytePushCount) {
			case 6:
				pWeaponShot.iPlayerId = value;
				break;

			case 5:
				pWeaponShot.iWeaponId = value;
				break;

			case 4:
				pWeaponShot.iHitType = value;
				break;

			case 3:
				pWeaponShot.iHitId = value;
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
		switch (bytePushCount) {
			case 1:
				pStreamIn.iPlayerId = value;
				break;

			case 0:
				pStreamIn.iForPlayerId = value;
				break;
		}
		// Increase the parameters count
		bytePushCount++;
	} else if (bStreamOut) {
		switch (bytePushCount) {
			case 1:
				pStreamOut.iPlayerId = value;
				break;

			case 0:
				pStreamOut.iForPlayerId = value;
				break;
		}
		// Increase the parameters count
		bytePushCount++;
	}

	pfn_amx_Push = (amx_Push_t)(subhook_get_trampoline(hookPush));

	return pfn_amx_Push(amx, value);
}

int amx_Exec_Hook(AMX *amx, long *retval, int index)
{
	pfn_amx_Exec = (amx_Exec_t)(subhook_get_trampoline(hookExec));

	int ret = 0;

	if (bGiveDamage) {
		bGiveDamage = false;

		// get the player data
		CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(pGiveDamage.iDamagedId);

		// check on invulnerable
		if (pPlayerData && pPlayerData->IsInvulnerable()) {
			return ret;
		}

		// call hooked callback
		ret = pfn_amx_Exec(amx, retval, index);

		// call custom callback
		if (pPlayerData) {
			pPlayerData->ProcessDamage(pGiveDamage.iPlayerId, pGiveDamage.fHealthLoss, pGiveDamage.iWeapon, pGiveDamage.iBodypart);
		}
	} else if (bTakeDamage) {
		bTakeDamage = false;

		// get the player data
		CPlayerData *pPlayerData = pServer->GetPlayerManager()->GetAt(pTakeDamage.iDamagerId);

		if (pPlayerData) {
			// call custom callback
			pPlayerData->ProcessGiveDamage(pTakeDamage.iPlayerId, pTakeDamage.fHealthLoss, pTakeDamage.iWeapon, pTakeDamage.iBodypart);
		} else {
			// call hooked callback
			ret = pfn_amx_Exec(amx, retval, index);
		}
	} else if (bWeaponShot) {
		bWeaponShot = false;

		// call hooked callback
		ret = pfn_amx_Exec(amx, retval, index);

		// call custom callback
		if (pWeaponShot.iHitType == BULLET_HIT_TYPE_VEHICLE) {
			WORD wPlayerId = pServer->GetVehicleSeatPlayerId(pWeaponShot.iHitId, 0);

			if (pServer->GetPlayerManager()->IsPlayerConnectedEx(wPlayerId)) {
				pServer->GetPlayerManager()->GetAt(wPlayerId)->ProcessVehicleDamage(
				    pWeaponShot.iPlayerId, pWeaponShot.iHitId, pWeaponShot.iWeaponId, pWeaponShot.vecHit);
			}
		}
	} else if (bStreamIn) {
		bStreamIn = false;

		if (pServer->GetPlayerManager()->IsPlayerConnectedEx(pStreamIn.iPlayerId)) {
			// call custom callback
			CCallbackManager::OnStreamIn(pStreamIn.iPlayerId, pStreamIn.iForPlayerId);
		} else {
			// call hooked callback
			ret = pfn_amx_Exec(amx, retval, index);
		}
	} else if (bStreamOut) {
		bStreamOut = false;

		if (pServer->GetPlayerManager()->IsPlayerConnectedEx(pStreamIn.iPlayerId)) {
			// call custom callback
			CCallbackManager::OnStreamOut(pStreamIn.iPlayerId, pStreamIn.iForPlayerId);
		} else {
			// call hooked callback
			ret = pfn_amx_Exec(amx, retval, index);
		}
	} else {
		ret = pfn_amx_Exec(amx, retval, index);
	}

	return ret;
}

void CHooks::InstallHooks()
{
	// Reset public flag
	bGiveDamage = false;

	// Find the function pointers
	BYTE *pFindPublic = *(BYTE **)((DWORD)pAMXFunctions + PLUGIN_AMX_EXPORT_FindPublic * 4);
	BYTE *pPush = *(BYTE **)((DWORD)pAMXFunctions + PLUGIN_AMX_EXPORT_Push * 4);
	BYTE *pExec = *(BYTE **)((DWORD)pAMXFunctions + PLUGIN_AMX_EXPORT_Exec * 4);

	// Hook for amx_FindPublic
	hookFindPublic = subhook_new(pFindPublic, (BYTE *)&amx_FindPublic_Hook);
	subhook_install(hookFindPublic);

	// Hook for amx_Push
	hookPush = subhook_new(pPush, (BYTE *)&amx_Push_Hook);
	subhook_install(hookPush);

	// Hook for amx_Exec
	hookExec = subhook_new(pExec, (BYTE *)&amx_Exec_Hook);
	subhook_install(hookExec);
}

void CHooks::InstallCallHook(DWORD dwInstallAddress, DWORD dwHookFunction)
{
	// Unprotect the address
	CUtils::UnProtect(dwInstallAddress, 5);
	// Calculate the installing address
	DWORD dwFunction = dwHookFunction - (dwInstallAddress + 5);
	// Write the call instruction
	*(BYTE *)dwInstallAddress = 0xE8;
	// Write the hook function address
	*(DWORD *)(dwInstallAddress + 1) = dwFunction;
}

void CHooks::InstallJmpHook(DWORD dwInstallAddress, DWORD dwHookFunction)
{
	// Unprotect the address
	CUtils::UnProtect(dwInstallAddress, 5);
	// Calculate the installing address
	DWORD dwFunction = dwHookFunction - (dwInstallAddress + 5);
	// Write the jmp instruction
	*(BYTE *)dwInstallAddress = 0xE9;
	// Write the hook function address
	*(DWORD *)(dwInstallAddress + 1) = dwFunction;
}
