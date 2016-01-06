/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: SAMPServer.h
	- Author(s): OrMisicL

  =========================================*/

#ifndef SAMPSERVER_H
#define SAMPSERVER_H

#include "SAMPPlayerPool.h"
#include "SAMPVehiclePool.h"

#pragma pack(push, 1)
class CSAMPFilterscriptPool
{
	public:
		AMX					*pAMX[MAX_FILTERSCRIPTS];		// 0x0000 - 0x0040
};
#pragma pack(pop)

#pragma pack(push, 1)
class CSAMPServer
{
	public:	
		void					*pGameModePool;			// 0x0000 - 0x0004
		CSAMPFilterscriptPool	*pFilterScriptPool;		// 0x0004 - 0x0008
		CSAMPPlayerPool			*pPlayerPool;			// 0x0008 - 0x000C
		CSAMPVehiclePool		*pVehiclePool;			// 0x000C - 0x0010
};
#pragma pack(pop)

#endif