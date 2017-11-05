/* =========================================

		FCNPC - Fully Controllable NPC
			----------------------

	- File: Maths.cpp
	- Author(s): OrMisicL

  =========================================*/

#include "Main.hpp"
#include "CUtils.hpp"

float CMath::GetDistanceBetween3DPoints(CVector vecPosition, CVector _vecPosition)
{
	// Get the distance between the two vectors
	float fSX = (_vecPosition.fX - vecPosition.fX) * (_vecPosition.fX - vecPosition.fX);
	float fSY = (_vecPosition.fY - vecPosition.fY) * (_vecPosition.fY - vecPosition.fY);
	float fSZ = (_vecPosition.fZ - vecPosition.fZ) * (_vecPosition.fZ - vecPosition.fZ);
	return ((float)sqrt(fSX + fSY + fSZ));
}

float CMath::GetDistanceFromRayToPoint(CVector vecStartPosition, CVector vecEndPosition, CVector vecPoint)
{
	return GetDistanceBetween3DPoints(GetNearestPointToRay(vecStartPosition, vecEndPosition, vecPoint), vecPoint);
}

CVector CMath::GetNearestPointToRay(CVector vecStartPosition, CVector vecEndPosition, CVector vecPoint)
{
	CVector vecDirection = (vecEndPosition - vecStartPosition) / GetDistanceBetween3DPoints(vecStartPosition, vecEndPosition);
	return vecDirection * GetDistanceBetween3DPoints(vecStartPosition, vecPoint) + vecStartPosition;
}

float CMath::Max(const float a, const float b)
{
	return a > b ? a : b;
}

// based on Quat function from MTA SA
void CMath::GetQuaternionFromMatrix(MATRIX4X4 m, float *fQuaternion)
{
	fQuaternion[0] = sqrt(Max((float)0, 1.0f + m.right.fX + m.up.fY + m.at.fZ)) * 0.5f;
	fQuaternion[1] = sqrt(Max((float)0, 1.0f + m.right.fX - m.up.fY - m.at.fZ)) * 0.5f;
	fQuaternion[2] = sqrt(Max((float)0, 1.0f - m.right.fX + m.up.fY - m.at.fZ)) * 0.5f;
	fQuaternion[3] = sqrt(Max((float)0, 1.0f - m.right.fX - m.up.fY + m.at.fZ)) * 0.5f;

	fQuaternion[1] = static_cast < float > (copysign(fQuaternion[1], m.at.fY - m.up.fZ));
	fQuaternion[2] = static_cast < float > (copysign(fQuaternion[2], m.right.fZ - m.at.fX));
	fQuaternion[3] = static_cast < float > (copysign(fQuaternion[3], m.up.fX - m.right.fY));
}

// based on ToMatrix function from MTA SA
void CMath::GetMatrixFromQuaternion(float *fQuaternion, MATRIX4X4 *m)
{
	float xx = fQuaternion[1] * fQuaternion[1];
	float xy = fQuaternion[1] * fQuaternion[2];
	float xz = fQuaternion[1] * fQuaternion[3];
	float xw = fQuaternion[1] * fQuaternion[0];

	float yy = fQuaternion[2] * fQuaternion[2];
	float yz = fQuaternion[2] * fQuaternion[3];
	float yw = fQuaternion[2] * fQuaternion[0];

	float zz = fQuaternion[3] * fQuaternion[3];
	float zw = fQuaternion[3] * fQuaternion[0];

	m->right.fX = 1.0f - 2.0f * (yy + zz);
	m->right.fY = 2.0f * (xy - zw);
	m->right.fZ = 2.0f * (xz + yw);

	m->up.fX = 2.0f * (xy + zw);
	m->up.fY = 1.0f - 2.0f * (xx + zz);
	m->up.fZ = 2.0f * (yz - xw);

	m->at.fX = 2.0f * (xz - yw);
	m->at.fY = 2.0f * (yz + xw);
	m->at.fZ = 1.0f - 2.0f * (xx + yy);
}

void CMath::QuaternionRotateZ(MATRIX4X4 *m, float angle)
{
	float c = cos(angle);
	float s = sin(angle);

	m->right.fX = c;
	m->right.fY = s;
	m->right.fZ = 0.0;

	m->up.fX = -s;
	m->up.fY = c;
	m->up.fZ = 0.0;

	m->at.fX = 0.0;
	m->at.fY = 0.0;
	m->at.fZ = 1.0;
}

float CMath::AngleToQuaternion(float fAngle)
{
	return sin((fAngle * (M_PI / 180.0f)) / 2) * (fAngle < 180.0f ? -1.0f : 1.0f);
}

float CMath::RadiansToDegree(float fRadian)
{
	float fDegree = (fRadian * (180.0f / M_PI)) + 270.0f;
	return fDegree > 360.0f ? fDegree - 360.0f : fDegree;
}

float CMath::DegreeToRadians(float fDegree)
{
	return (fDegree * M_PI) / 180.0f;
}

bool CMath::IsInRange(float fRange1, float fRange2, float fRadius)
{
	float fDis = (fRange1 - fRange2);
	if ((fDis < fRadius) && (fDis > -fRadius)) {
		return true;
	}

	return false;
}

float CMath::GetAngle(float fRotationX, float fRotationY)
{
	float fReturn = atan2(fRotationY, fRotationX) * (180.0f / M_PI) + 270.0f;
	if (fReturn >= 360.0f) {
		fReturn -= 360.0f;
	} else if (fReturn < 0.0f) {
		fReturn += 360.0f;
	}
	return fReturn;
}

void CMath::GetCoordsInFront(float fX, float fY, float fA, float fDistance, float &fResX, float &fResY)
{
	float fAngleRadian = DegreeToRadians(-fA);
	fResX = fX + (fDistance * sin(fAngleRadian));
	fResY = fY + (fDistance * cos(fAngleRadian));
}
