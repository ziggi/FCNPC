/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: Maths.h
	- Author(s): OrMisicL

  =========================================*/

#ifndef MATHS_H
#define MATHS_H

#include "Common.h"

#define M_PI           3.14159265358979323846f

class CMath
{
	public:
		static float GetDistanceBetween3DPoints(CVector3 vecPosition, CVector3 _vecPosition);
		static float GetDistanceFromRayToPoint(const CVector3 p, const CVector3 v, const CVector3 w);

		static float AngleToQuaternion(float fAngle);
		static float RadiansToDegree(float fRadian);
		static float RadiansToDegree2(float fRadian);
		static float DegreeToRadians(float fDegree);

		static bool	 IsInRange(float fRange1, float fRange2, float fRadius);

		static float GetAngle(float fRotationX, float fRotationY);
};

#endif