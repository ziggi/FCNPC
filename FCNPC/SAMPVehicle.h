/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: SAMPVehicle.h
	- Author(s): OrMisicL

  =========================================*/

#ifndef SAMPVEHICLE_H
#define SAMPVEHICLE_H

#pragma pack(push, 1)
class CSAMPVehicle
{
	public:
		CVector3		vecPosition;		// 0x0000 - 0x000C
		float			fQuaternionAngle;	// 0x000C - 0x0010
		CVector3		vecQuaternion;		// 0x0010 - 0x001C
		PAD(pad0, 48);						// 0x001C - 0x004C
		CVector3		vecVelocity;		// 0x004C - 0x0058
		PAD(pad1, 42);						// 0x0058 - 0x0082
		int				iModelId;			// 0x0082 - 0x0086
		PAD(pad2, 32);						// 0x0086 - 0x00A6
		float			fHealth;			// 0x00A6 - 0x00AA


};
#pragma pack(pop)

#endif