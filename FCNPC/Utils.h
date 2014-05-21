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
		static void  GetPluginError(BYTE byteError, char *szError);
		static void  UnProtect(DWORD dwAddress, size_t sSize);
		static void  FCNPCSleep(DWORD dwMs);
		static DWORD FindPattern(char *szPattern, char *szMask);
#ifndef _WIN32
		static void LoadTickCount();
		static int  GetTickCount();
#endif
};

// Linux
#ifndef _WIN32

int GetTickCount();

#endif

#endif