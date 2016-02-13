/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: Maths.h
	- Author(s): OrMisicL

  =========================================*/

#ifndef MATHS_H
#define MATHS_H

#include "CVector.h"
#include "vectormath_aos.h"

#define M_PI           3.14159265358979323846f

class CMath
{
	public:
		static float GetDistanceBetween3DPoints(CVector vecPosition, CVector _vecPosition);
		static float GetDistanceFromRayToPoint(const CVector p, const CVector v, const CVector w);

		static void  GetQuaternionFromMatrix(MATRIX4X4 matrix, float *fQuaternion);
		static void  GetMatrixFromQuaternion(float *fQuaternion, MATRIX4X4 *matrix);

		static float AngleToQuaternion(float fAngle);
		static float RadiansToDegree(float fRadian);
		static float RadiansToDegree2(float fRadian);
		static float DegreeToRadians(float fDegree);

		static bool	 IsInRange(float fRange1, float fRange2, float fRadius);

		static float GetAngle(float fRotationX, float fRotationY);

		static float Max(const float a, const float b);
};

#endif
