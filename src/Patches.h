/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: Patches.h
	- Author(s): OrMisicL

  =========================================*/

#ifndef PATCHES_H
#define PATCHES_H

class CPatches
{
	public:
		static void InstallFunctionDisablePatch(DWORD dwFunction, BYTE byteReturn);
		static void InstallCustomPatch(DWORD dwAddress, BYTE bytePatch[], int iLength);
		static void InstallNopPatch(DWORD dwAddress, int iSize);

		static void InstallPatches();

};

#endif