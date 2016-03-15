#ifndef YSF_CGANGZONEPOOL_H
#define YSF_CGANGZONEPOOL_H

//#include "Main.hpp"
//#include "Structs.h"
#include <cstddef>

class CGangZone
{
public:
	float fGangZone[4];
};

class CGangZonePool
{
public:
	CGangZone*				pGangZone[1024];

public:
	CGangZonePool();
	~CGangZonePool();

	WORD New(float fMinX, float fMinY, float fMaxX, float fMaxY);
	WORD New(WORD playerid, float fMinX, float fMinY, float fMaxX, float fMaxY);
	void Delete(WORD wZone);
	void Delete(WORD playerid, WORD wZone);

	bool ShowForPlayer(WORD bytePlayer, WORD wZone, DWORD dwColor, bool bPlayerZone = false);
	void ShowForAll(WORD wZone, DWORD dwColor);
	bool HideForPlayer(WORD bytePlayer, WORD wZone, bool bPlayerZone = false);
	void HideForAll(WORD wZone);
	void FlashForPlayer(WORD bytePlayer, WORD wZone, DWORD dwColor, bool bPlayerZone = false);
	void FlashForAll(WORD wZone, DWORD dwColor);
	void StopFlashForPlayer(WORD bytePlayer, WORD wZone, bool bPlayerZone = false);
	void StopFlashForAll(WORD wZone);

	bool GetSlotState(WORD wZone)
	{
		if (wZone >= 1024) {
			return 0;
		}
		return pGangZone[wZone] != NULL;
	};

};

#endif
