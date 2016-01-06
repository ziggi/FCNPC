/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: SAMPVehiclePool.h
	- Author(s): OrMisicL

  =========================================*/

#ifndef SAMPVEHICLEPOOL_H
#define SAMPVEHICLEPOOL_H

#include "SAMPVehicle.h"

#pragma pack(push, 1)
class CSAMPVehiclePool
{
	public:
		PAD(pad0, 16212);										// 0x0000 - 0x3F54
		CSAMPVehicle		*pVehicle[MAX_VEHICLES];			// 0x3F54 - 0x5E94
};
#pragma pack(pop)

#endif