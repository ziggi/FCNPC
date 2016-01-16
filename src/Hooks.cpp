/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: Hooks.cpp
	- Author(s): OrMisicL

  =========================================*/

#include "Main.h"

extern CServer		*pServer;
extern logprintf_t	logprintf;
extern void			*pAMXFunctions;

// Parameters for "OnPlayerGiveDamage" function
bool bGiveDamage;
BYTE bytePushCount;
int iPlayerId;
int iDamagerId;
float fHealthLoss;
int iWeapon;
int iBodypart;

subhook_t	hookFindPublic;
subhook_t	hookPush;

// amx_FindPublic function definition
typedef int (* amx_FindPublic_t)(AMX *amx, const char *funcname, int *index);
amx_FindPublic_t pfn_amx_FindPublic = NULL;
// amx_Push function definition
typedef int(*amx_Push_t)(AMX *amx, cell value);
amx_Push_t pfn_amx_Push = NULL;

DWORD test;

int amx_FindPublic_Hook(AMX *amx, const char *funcname, int *index)
{
	// Is it "OnPlayerGiveDamage"
	if (!strcmp(funcname, "OnPlayerGiveDamage"))
	{
		// Set parameter flags
		bGiveDamage = true;
		bytePushCount = 0;
	}

	pfn_amx_FindPublic = (amx_FindPublic_t)(subhook_get_trampoline(hookFindPublic));

	return pfn_amx_FindPublic(amx, funcname, index);
}

int amx_Push_Hook(AMX *amx, cell value)
{
	// Are we retrieving parameters ?
	if (bGiveDamage)
	{
		switch (bytePushCount)
		{
			case 4:
				iPlayerId = value;
				break;

			case 3:
				iDamagerId = value;
				break;

			case 2:
				fHealthLoss = amx_ctof(value);
				break;

			case 1:
				iWeapon = value;
				break;

			case 0:
				iBodypart = value;
				break;
		}
		// Increase the parameters count
		bytePushCount++;
		// If we have finished then execute the function
		if (bytePushCount == 5)
		{
			if (pServer->GetPlayerManager()->IsPlayerConnected(iDamagerId))
				pServer->GetPlayerManager()->GetAt(iDamagerId)->ProcessDamage(iPlayerId, fHealthLoss, iWeapon, iBodypart);

			bGiveDamage = false;
		}
	}

	pfn_amx_Push = (amx_Push_t)(subhook_get_trampoline(hookPush));

	return pfn_amx_Push(amx, value);
}

void CHooks::InstallHooks()
{
	// Reset public flag
	bGiveDamage = false;
	BYTE *pFindPublic = *(BYTE **)((DWORD)pAMXFunctions + PLUGIN_AMX_EXPORT_FindPublic * 4);
	// Find the amx_Push function pointer
	BYTE *pPush = *(BYTE **)((DWORD)pAMXFunctions + PLUGIN_AMX_EXPORT_Push * 4);
	// Find the amx_FindPublic function pointer
	hookFindPublic = subhook_new(pFindPublic, (BYTE *)&amx_FindPublic_Hook);
	subhook_install(hookFindPublic);
	// Hook for amx_Push
	hookPush = subhook_new(pPush, (BYTE *)&amx_Push_Hook);
	subhook_install(hookPush);
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
