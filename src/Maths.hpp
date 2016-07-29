/* =========================================

		FCNPC - Fully Controllable NPC
			----------------------

	- File: Maths.hpp
	- Author(s): OrMisicL

  =========================================*/

#ifndef MATHS_H
#define MATHS_H

#include "CVector.h"

#ifndef M_PI
	#define M_PI 3.14159265358979323846f
#endif

class CMath
{
public:
	static float GetDistanceBetween3DPoints(CVector vecPosition, CVector _vecPosition);
	static float GetDistanceFromRayToPoint(CVector vecStartPosition, CVector vecEndPosition, CVector vecPoint);

	static void GetQuaternionFromMatrix(MATRIX4X4 matrix, float *fQuaternion);
	static void GetMatrixFromQuaternion(float *fQuaternion, MATRIX4X4 *matrix);
	static void QuaternionRotateZ(MATRIX4X4 *m, double angle);

	static float AngleToQuaternion(float fAngle);
	static float RadiansToDegree(float fRadian);
	static float RadiansToDegree2(float fRadian);
	static float DegreeToRadians(float fDegree);

	static bool IsInRange(float fRange1, float fRange2, float fRadius);

	static float GetAngle(float fRotationX, float fRotationY);

	static float Max(const float a, const float b);
};

#endif
