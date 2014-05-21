/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: Patches.cpp
	- Author(s): OrMisicL

  =========================================*/

#include "Main.h"

void CPatches::InstallFunctionDisablePatch(DWORD dwFunction, BYTE byteReturn = 0)
{
	// Unprotect the the function
	CUtils::UnProtect(dwFunction, byteReturn == 0 ? 1 : 2);
	// Write a retn patch
	*(BYTE *)(dwFunction) = 0xC3;
	// Write the return value
	if(byteReturn != 0)
		*(BYTE *)(dwFunction + 1) = byteReturn;
}

void CPatches::InstallNopPatch(DWORD dwAddress, int iSize)
{
	// Unprotect the the address
	CUtils::UnProtect(dwAddress, iSize);
	// Write a NOP patch
	memset((void *)dwAddress, 0x90, iSize);
}

void CPatches::InstallCustomPatch(DWORD dwAddress, BYTE bytePatch[], int iLength)
{
	// Unprotect the address
	CUtils::UnProtect(dwAddress, iLength);
	// Write the patch
	memcpy((void *)dwAddress, bytePatch, iLength);
}

void CPatches::InstallPatches()
{
	// No patches for now
}
