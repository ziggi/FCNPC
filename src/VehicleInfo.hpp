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
	static bool IsValidPassengerSeat(BYTE byteSeatId, WORD wModelId);
	static bool IsValidModel(WORD wModelId);
	static WORD GetMaxPassengers(WORD wModelId);
	static bool IsATrainLoco(WORD wModelId);
	static bool IsATrainPart(WORD wModelId);
	static bool IsAHydra(WORD wModelId);
};

#endif
