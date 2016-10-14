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
	switch(byteError) {
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

		case 6:
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

static size_t writer(void *contents, size_t size, size_t nmemb, void *userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

bool CUtils::GetLatestVersion(char szVersion[16])
{
	CURL *curl;
	CURLcode res;
	std::string temp;

	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, "https://api.github.com/repos/ziggi/FCNPC/releases/latest");
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/53.0.2785.143 Safari/537.36");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &temp);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);

		if (res == CURLE_OK) {
			std::size_t start_pos = temp.find("tag_name") + 13;
			std::size_t length = temp.find_first_of("\"", start_pos + 1) - start_pos;

			length = temp.copy(szVersion, length, start_pos);
			szVersion[length] = '\0';
			return true;
		}
	}
	return false;
}
