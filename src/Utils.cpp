/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: Utils.cpp
	- Author(s): OrMisicL

  =========================================*/

#include "Main.h"

#ifdef WIN32
	#include <Psapi.h>
#else
	#include "sys/time.h"
	timeval startTime;
	timeval currentTime;
#endif

void CUtils::GetPluginError(BYTE byteError, char *szError, size_t sSize)
{
	switch(byteError)
	{
		case 1:
			strlcpy(szError, "File \"scriptfiles/FCNPC/ZMap.hmap\" is not found", sSize);
			break;

		case 2:
			strlcpy(szError, "Failed to create PlayerManager instance", sSize);
			break;

		case 3:
			strlcpy(szError, "Failed to create NodeManager instance", sSize);
			break;

		case 4:
			strlcpy(szError, "Failed to create damage thread", sSize);
			break;

		case 5:
			strlcpy(szError, "Failed to create RPCParams instance", sSize);
			break;

		default:
			strlcpy(szError, "", sSize);
			break;
	}
}

void CUtils::UnProtect(DWORD dwAddress, size_t sSize)
{
	// Unprotect the address
#ifdef _WIN32
	DWORD dwOldProtection;
	VirtualProtect((LPVOID)dwAddress, sSize, PAGE_EXECUTE_READWRITE, &dwOldProtection);
#else
	mprotect((void*)(((int)dwAddress / 4096) * 4096), 4096, PROT_WRITE | PROT_READ | PROT_EXEC);
#endif
}

void CUtils::FCNPCSleep(DWORD dwMs)
{
	// Sleep
#ifdef _WIN32
	Sleep(dwMs);
#else
	usleep((dwMs) * 1000);
#endif
}
// Thanks to kurta999 - YSF project
DWORD CUtils::FindPattern(char *szPattern, char *szMask)
{
#ifdef WIN32
	// Get the current process information
	MODULEINFO mInfo = {0};
	GetModuleInformation(GetCurrentProcess(), GetModuleHandle(NULL), &mInfo, sizeof(MODULEINFO));
	// Find the base address
	DWORD dwBase = (DWORD)mInfo.lpBaseOfDll;
	DWORD dwSize =  (DWORD)mInfo.SizeOfImage;
#else
	DWORD dwBase = 0x804b480;
	DWORD dwSize = 0x8128B80 - dwBase;
#endif
	// Get the pattern length
	DWORD dwPatternLength = (DWORD)strlen(szMask);
	// Loop through all the process
	for(DWORD i = 0; i < dwSize - dwPatternLength; i++)
	{
		bool bFound = true;
		// Loop through the pattern caracters
		for (DWORD j = 0; j < dwPatternLength; j++)
			bFound &= szMask[j] == '?' || szPattern[j] == *(char*)(dwBase + i + j);

		// If found return the current address
		if(bFound)
			return dwBase + i;
	}
	// Return null
	return NULL;
}

// Linux GetTickCount
#ifndef _WIN32

void CUtils::LoadTickCount()
{
	// Get the starting time
	gettimeofday(&startTime, 0);
}

int GetTickCount()
{
	// Get the time elapsed since the start
	gettimeofday(&currentTime, 0);
	return (currentTime.tv_usec - startTime.tv_usec) / 1000 + 1000 * (currentTime.tv_sec - startTime.tv_sec);
}

#endif
