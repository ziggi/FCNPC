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
	static void GetPluginError(BYTE byteError, char *szError, size_t sSize);
	static void UnProtect(DWORD dwAddress, size_t sSize);
	static DWORD FindPattern(const char *szPattern, const char *szMask);
	static float RandomFloat(float min, float max);
	static float GetNearestFloatValue(float value, float *array, const size_t size);
	static bool GetLatestVersion(char szVersion[16]);
};

#if defined(LINUX)
	#if defined(GetTickCount)
		#undef GetTickCount
	#endif

	int GetTickCount();
	void LoadTickCount();
#endif

#endif
