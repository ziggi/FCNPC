/* =========================================

		FCNPC - Fully Controllable NPC
			----------------------

	- File: VehicleInfo.hpp
	- Author(s): ziggi

  =========================================*/

#ifndef VEHICLEINFO_H
#define VEHICLEINFO_H

class CVehicleInfo
{
public:
	static bool IsValidPassengerSeat(BYTE iSeatId, WORD iModelId);
	static bool IsValidModel(WORD iModelId);
	static WORD GetMaxPassengers(WORD iModelId);
};

#endif
