/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: Maths.cpp
	- Author(s): OrMisicL

  =========================================*/

#include "Main.h"

float CMath::GetDistanceBetween3DPoints(CVector vecPosition, CVector _vecPosition)
{
	// Get the distance between the two vectors
	float fSX = (_vecPosition.fX - vecPosition.fX) * (_vecPosition.fX - vecPosition.fX);
	float fSY = (_vecPosition.fY - vecPosition.fY) * (_vecPosition.fY - vecPosition.fY);
	float fSZ = (_vecPosition.fZ - vecPosition.fZ) * (_vecPosition.fZ - vecPosition.fZ);
	return ((float)sqrt(fSX + fSY + fSZ));	
}

float CMath::GetDistanceFromRayToPoint(CVector p, CVector a, CVector b)
{
	CVector n = b - a;
	CVector pa = a - p;

	float c = n.DotProduct(&pa);

	if (c > 0.0f)
		return pa.DotProduct(&pa);

	CVector e = pa - n * (c / n.DotProduct(&n));

	return e.DotProduct(&e);
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
	if((fDis < fRadius) && (fDis > -fRadius))
		return true;

	return false;
}

float CMath::GetAngle(float fRotationX, float fRotationY)
{
	float fReturn = atan2(fRotationX, fRotationY) * 57.295776f;
	if (fReturn > 360.0f)
		fReturn -= 360.0f;
	else if (fReturn < 0.0f)
		fReturn += 360.0f;

	return fReturn;
}
