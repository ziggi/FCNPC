/* =========================================

		FCNPC - Fully Controllable NPC
			----------------------

	- File: Utils.cpp
	- Author(s): OrMisicL

  =========================================*/

#include "Main.hpp"

#if defined(WIN32)
	#include <Psapi.h>
#elif defined(LINUX)
	#include "sys/time.h"

	timeval startTime;
	timeval currentTime;

	void LoadTickCount()
	{
		gettimeofday(&startTime, 0);
	}

	int GetTickCount()
	{
		gettimeofday(&currentTime, 0);
		return (currentTime.tv_usec - startTime.tv_usec) / 1000 + 1000 * (currentTime.tv_sec - startTime.tv_sec);
	}
#endif

void CUtils::GetPluginError(BYTE byteError, char *szError, size_t sSize)
{
	switch (byteError) {
		case ERROR_INCLUDE_VERSION:
			strlcpy(szError, "Include file version does not match plugin version", sSize);
			break;

		default:
			strlcpy(szError, "", sSize);
			break;
	}
}

void CUtils::UnProtect(DWORD dwAddress, size_t sSize)
{
	// Unprotect the address
#if defined(WIN32)
	DWORD dwOldProtection;
	VirtualProtect((LPVOID)dwAddress, sSize, PAGE_EXECUTE_READWRITE, &dwOldProtection);
#elif defined(LINUX)
	mprotect((void*)(((int)dwAddress / 4096) * 4096), 4096, PROT_WRITE | PROT_READ | PROT_EXEC);
#endif
}

// Thanks to kurta999 - YSF project
DWORD CUtils::FindPattern(const char *szPattern, const char *szMask)
{
#if defined(WIN32)
	// Get the current process information
	MODULEINFO mInfo = {0};
	GetModuleInformation(GetCurrentProcess(), GetModuleHandle(NULL), &mInfo, sizeof(MODULEINFO));
	// Find the base address
	DWORD dwBase = (DWORD)mInfo.lpBaseOfDll;
	DWORD dwSize =  (DWORD)mInfo.SizeOfImage;
#elif defined(LINUX)
	DWORD dwBase = 0x804b480;
	DWORD dwSize = 0x8128B80 - dwBase;
#endif
	// Get the pattern length
	DWORD dwPatternLength = (DWORD)strlen(szMask);
	// Loop through all the process
	for (DWORD i = 0; i < dwSize - dwPatternLength; i++) {
		bool bFound = true;
		// Loop through the pattern caracters
		for (DWORD j = 0; j < dwPatternLength; j++) {
			bFound &= szMask[j] == '?' || szPattern[j] == *(char*)(dwBase + i + j);
		}

		// If found return the current address
		if (bFound) {
			return dwBase + i;
		}
	}
	// Return 0
	return 0;
}

float CUtils::RandomFloat(float min, float max)
{
	assert(max > min);
	float random = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	float range = max - min;
	return (random * range) + min;
}

float CUtils::GetNearestFloatValue(float value, float *array, const size_t size)
{
	assert(size > 0);
	float nearest = array[0];

	for (uint32_t i = 1; i < size; i++) {
		if (std::abs(array[i] - value) < std::abs(nearest - value)) {
			nearest = array[i];
		}
	}

	return nearest;
}
