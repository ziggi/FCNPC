/* =========================================

		FCNPC - Fully Controllable NPC
			----------------------

	- File: Hooks.h
	- Author(s): OrMisicL

  =========================================*/
#ifndef HOOKS_H
#define HOOKS_H

extern bool bTakeDamageInternalCall;

class CHooks
{
public:
	static void InstallHooks();

	static void InstallCallHook(DWORD dwInstallAddress, DWORD dwHookFunction);
	static void InstallJmpHook(DWORD dwInstallAddress, DWORD dwHookFunction);
};

#endif
