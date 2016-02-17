/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: Hooks.cpp
	- Author(s): OrMisicL

  =========================================*/

#include "Main.h"

extern CServer      *pServer;
extern logprintf_t  logprintf;
extern void         *pAMXFunctions;

BYTE bytePushCount;

// give damage
bool bGiveDamage;

struct t_OnPlayerGiveDamage {
	int iPlayerId;
	int iDamagerId;
	float fHealthLoss;
	int iWeapon;
	int iBodypart;
};

t_OnPlayerGiveDamage pGiveDamage;

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
	if (!strcmp(funcname, "OnPlayerGiveDamage"))
		bGiveDamage = true;
	else if (!strcmp(funcname, "OnPlayerStreamIn"))
		bStreamIn = true;
	else if (!strcmp(funcname, "OnPlayerStreamOut"))
		bStreamOut = true;

	bytePushCount = 0;

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
				pGiveDamage.iPlayerId = value;
				break;

			case 3:
				pGiveDamage.iDamagerId = value;
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
	}
	else if (bStreamIn)
	{
		switch (bytePushCount)
		{
			case 1:
				pStreamIn.iPlayerId = value;
				break;

			case 0:
				pStreamIn.iForPlayerId = value;
				break;
		}
		// Increase the parameters count
		bytePushCount++;
	}
	else if (bStreamOut)
	{
		switch (bytePushCount)
		{
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

	if (bGiveDamage)
	{
		bGiveDamage = false;

		if (pServer->GetPlayerManager()->GetAt(pGiveDamage.iDamagerId)->IsInvulnerable())
			return ret;
		
		// call hooked callback
		ret = pfn_amx_Exec(amx, retval, index);

		// call custom callback
		if (pServer->GetPlayerManager()->IsPlayerConnectedEx(pGiveDamage.iDamagerId))
		{
			pServer->GetPlayerManager()->GetAt(pGiveDamage.iDamagerId)->ProcessDamage(
				pGiveDamage.iPlayerId, pGiveDamage.fHealthLoss, pGiveDamage.iWeapon, pGiveDamage.iBodypart);
		}
	}
	else if (bStreamIn)
	{
		bStreamIn = false;
		
		// call hooked callback
		ret = pfn_amx_Exec(amx, retval, index);

		// call custom callback
		if (pServer->GetPlayerManager()->IsPlayerConnectedEx(pStreamIn.iPlayerId))
		{
			CCallbackManager::OnStreamIn(pStreamIn.iPlayerId, pStreamIn.iForPlayerId);
		}
	}
	else if (bStreamOut)
	{
		bStreamOut = false;

		// call hooked callback
		ret = pfn_amx_Exec(amx, retval, index);

		// call custom callback
		if (pServer->GetPlayerManager()->IsPlayerConnectedEx(pStreamIn.iPlayerId))
		{
			CCallbackManager::OnStreamOut(pStreamIn.iPlayerId, pStreamIn.iForPlayerId);
		}
	}
	else
	{
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
