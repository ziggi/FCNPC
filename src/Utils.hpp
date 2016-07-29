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
	static DWORD FindPattern(const char *szPattern, const char *szMask);
	static float RandomFloat(float min, float max);

	template <typename T>
	static T GetNearestValue(T value, std::vector<T> list)
	{
		auto i = std::min_element(begin(list), end(list), [=](T x, T y) {
			return abs(x - value) < abs(y - value);
		});
		return *i;
	}
};

#if defined(LINUX)
	#if defined(GetTickCount)
		#undef GetTickCount
	#endif

	int GetTickCount();
	void LoadTickCount();
#endif

#endif
