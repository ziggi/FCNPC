/* =========================================

		FCNPC - Fully Controllable NPC
			----------------------

	- File: VehicleInfo.cpp
	- Author(s): ziggi

  =========================================*/

#include "Main.h"

// thanks to MTA SA team for this array
static const BYTE wMaxPassengers[212] = {
	3, 1, 1, 1, 3, 3, 0, 1, 1, 3, 1, 1, 1, 3, 1, 1,              // 400->415
	3, 1, 3, 1, 3, 3, 1, 1, 1, 0, 3, 3, 3, 1, 0, 8,              // 416->431
	0, 1, 1, 255, 1, 8, 3, 1, 3, 0, 1, 1, 1, 3, 0, 1,            // 432->447
	0, 1, 255, 1, 0, 0, 0, 1, 1, 1, 3, 3, 1, 1, 1,               // 448->462
	1, 1, 1, 3, 3, 1, 1, 3, 1, 0, 0, 1, 1, 0, 1, 1,              // 463->478
	3, 1, 0, 3, 1, 0, 0, 0, 3, 1, 1, 3, 1, 3, 0, 1,              // 479->494
	1, 1, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 0, 0,              // 495->510
	1, 0, 0, 1, 1, 3, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1,              // 511->526
	1, 1, 3, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 3, 1,                 // 527->541
	1, 1, 1, 1, 3, 3, 1, 1, 3, 3, 1, 0, 1, 1, 1, 1,              // 542->557
	1, 1, 3, 3, 1, 1, 0, 1, 3, 3, 0, 255, 1, 0, 0,               // 558->572
	1, 0, 1, 1, 1, 1, 3, 3, 1, 3, 0, 255, 3, 1, 1, 1,            // 573->588
	1, 255, 255, 1, 1, 1, 0, 3, 3, 3, 1, 1, 1, 1, 1,             // 589->604
	3, 1, 255, 255, 255, 3, 255, 255
};                          // 605->611

bool CVehicleInfo::IsValidPassengerSeat(BYTE iSeatId, WORD iModelId)
{
	if (!IsValidModel(iModelId)) {
		return false;
	}

	WORD maxPassengers = GetMaxPassengers(iModelId);

	if (iSeatId < 0 || iSeatId > maxPassengers || maxPassengers == 0xFF) {
		return false;
	}

	return true;
}

WORD CVehicleInfo::GetMaxPassengers(WORD iModelId)
{
	if (IsValidModel(iModelId)) {
		return wMaxPassengers[iModelId - 400];
	}

	return 0xFF;
}

bool CVehicleInfo::IsValidModel(WORD iModelId)
{
	return iModelId >= 400 && iModelId <= 611;
}
