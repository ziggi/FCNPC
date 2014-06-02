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

/*int STDCALL CFilterScriptPool__OnPlayerGiveDamage(int iPlayerId, int iDamagerId, float fHealthLoss, int iWeapon, int iBodypart)
{
	// Call our "OnTakeDamage" callback
	if(pServer->GetPlayerManager()->IsPlayerConnected(iDamagerId))
		pServer->GetPlayerManager()->GetAt(iDamagerId)->ProcessDamage(iPlayerId, fHealthLoss, iWeapon, iBodypart);

	CSAMPServer *pSAMPServer = (CSAMPServer *)CAddress::VAR_ServerPtr;
	cell ret = 0;
	// Call it for all the filterscripts
	for(int i = 0; i < MAX_FILTERSCRIPTS; i++)
	{
		if(!pSAMPServer->pFilterScriptPool->pAMX[i])
			continue;

		// Get the function index
		int iIndex;
		if(!amx_FindPublic(pSAMPServer->pFilterScriptPool->pAMX[i], "OnPlayerGiveDamage", &iIndex))
		{
			// Push the parameters
			amx_Push(pSAMPServer->pFilterScriptPool->pAMX[i], iBodypart);
			amx_Push(pSAMPServer->pFilterScriptPool->pAMX[i], iWeapon);
			amx_Push(pSAMPServer->pFilterScriptPool->pAMX[i], amx_ftoc(fHealthLoss));
			amx_Push(pSAMPServer->pFilterScriptPool->pAMX[i], iDamagerId);
			amx_Push(pSAMPServer->pFilterScriptPool->pAMX[i], iPlayerId);
			// Execute the callback
			amx_Exec(pSAMPServer->pFilterScriptPool->pAMX[i], &ret, iIndex);
			// Return if the return is true
			if(ret)
				break;
		}
	}
	return ret;
}*/

void CHooks::InstallHooks()
{
	// Since we cant hook this callback under Linux, i've decided to do this task from inside the include
	// Thanks to [uL]Pottus for the idea
	// Hook for CFilterScriptPool__OnPlayerGiveDamage
	//InstallCallHook(CAddress::CALLBACK_CFilterScriptPool__OnPlayerGiveDamage, (DWORD)CFilterScriptPool__OnPlayerGiveDamage);
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