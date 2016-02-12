/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: Utils.cpp
	- Author(s): OrMisicL

  =========================================*/

#ifndef UTILS_H
#define UTILS_H

class CUtils
{
	public:
		static void  GetPluginError(BYTE byteError, char *szError, size_t sSize);
		static void  UnProtect(DWORD dwAddress, size_t sSize);
		static void  FCNPCSleep(DWORD dwMs);
		static DWORD FindPattern(char *szPattern, char *szMask);
};

// Linux
#ifndef _WIN32

#undef GetTickCount
int GetTickCount();
void LoadTickCount();

#endif

#endif
