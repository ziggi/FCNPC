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
		PAD(pad0, 150012);											// 0x00000 - 0x249FC
		BOOL				bIsPlayerConnected[MAX_PLAYERS];		// 0x249FC - 0x2599C
		CSAMPPlayer			*pPlayer[MAX_PLAYERS];					// 0x2599C - 0x2693C
		char				szName[MAX_PLAYERS][MAX_NAME_LENGTH];	// 0x2693C - 0x2C6FC
};
#pragma pack(pop)

#endif