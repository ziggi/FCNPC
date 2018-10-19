#ifndef _INCL_NATIVES
#define _INCL_NATIVES

#include "Main.hpp"

class ColAndreasNatives
{
public:
	static cell AMX_NATIVE_CALL CA_Init(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL CA_RayCastLine(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL CA_RayCastLineExtraID(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL CA_RayCastLineID(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL CA_RayCastMultiLine(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL CA_RayCastLineAngle(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL CA_RayCastReflectionVector(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL CA_RayCastLineNormal(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL CA_CreateObject(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL CA_DestroyObject(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL CA_EulerToQuat(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL CA_QuatToEuler(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL CA_RemoveBuilding(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL CA_SetObjectPos(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL CA_SetObjectRot(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL CA_GetModelBoundingSphere(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL CA_GetModelBoundingBox(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL CA_SetObjectExtraID(AMX* amx, cell* params);
	static cell AMX_NATIVE_CALL CA_GetObjectExtraID(AMX* amx, cell* params);
	static cell AMX_NATIVE_CALL CA_ContactTest(AMX *amx, cell *params);

	// Extended natives
	static cell AMX_NATIVE_CALL CA_RayCastLineEx(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL CA_RayCastLineAngleEx(AMX *amx, cell *params);
};
#endif