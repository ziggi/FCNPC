/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: VehicleInfo.cpp
	- Author(s): ziggi

  =========================================*/

#include "Main.h"

bool CVehicleInfo::IsValidPassengerSeat(BYTE seat, WORD model)
{
	if (model < 0 || model > 212)
		return false;

	// TODO
	if (seat < 0 || seat > 3)
		return false;

	return true;
}
