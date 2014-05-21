/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: SAMPPlayerPool.h
	- Author(s): OrMisicL

  =========================================*/

#ifndef SAMPPLAYERPOOL_H
#define SAMPPLAYERPOOL_H

#include "SAMPPlayer.h"

#pragma pack(push, 1)
class CSAMPPlayerPool
{
	public:
		PAD(pad0, 75012);											// 0x00000 - 0x12504
		BOOL				bIsPlayerConnected[MAX_PLAYERS];		// 0x12504 - 0x12CD4
		CSAMPPlayer			*pPlayer[MAX_PLAYERS];					// 0x12CD4 - 0x134A4
		char				szName[MAX_PLAYERS][MAX_NAME_LENGTH];	// 0x134A4 - 0x16384
};
#pragma pack(pop)

#endif