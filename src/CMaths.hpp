/* =========================================

		FCNPC - Fully Controllable NPC
			----------------------

	- File: Maths.hpp
	- Author(s): OrMisicL

  =========================================*/

#ifndef MATHS_H
#define MATHS_H

#include "CVector.h"
#include "Structs.h"

#ifndef M_PI
	#define M_PI 3.14159265358979323846f
#endif

class CMath
{
public:
	static float GetDistanceBetween3DPoints(const CVector &vecPosition, const CVector &_vecPosition);
	static float GetDistanceFromRayToPoint(const CVector &vecStartPosition, const CVector &vecEndPosition, const CVector &vecPoint);
	static CVector GetNearestPointToRay(const CVector &vecStartPosition, const CVector &vecEndPosition, const CVector &vecPoint);

	static float Max(const float a, const float b);

	static void GetQuaternionFromMatrix(const MATRIX4X4 &matrix, float *fQuaternion);
	static void GetMatrixFromQuaternion(float *fQuaternion, MATRIX4X4 *matrix);
	static void QuaternionRotateZ(MATRIX4X4 *m, float angle);

	static float AngleToQuaternion(float fAngle);
	static float RadiansToDegree(float fRadian);
	static float DegreeToRadians(float fDegree);

	static bool IsInRange(float fRange1, float fRange2, float fRadius);

	static float GetAngle(float fRotationX, float fRotationY);
	static void GetCoordsInFront(float fX, float fY, float fA, float fDistance, float &fResX, float &fResY);

	static bool IsEqual(float a, float b);
};

#endif
