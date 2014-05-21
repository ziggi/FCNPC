/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: Hooks.h
	- Author(s): OrMisicL

  =========================================*/
#ifndef HOOKS_H
#define HOOKS_H

class CHooks
{
	public:
		static void InstallHooks();

		static void InstallCallHook(DWORD dwInstallAddress, DWORD dwHookFunction);
		static void InstallJmpHook(DWORD dwInstallAddress, DWORD dwHookFunction);
};

#endif