//----------------------------------------------------------
//
//   SA-MP Multiplayer Modification For GTA:SA
//   Copyright 2004-2010 SA-MP Team
//
//   Author: Kye 10 Jan 2010
//
//----------------------------------------------------------

#include "Main.hpp"

CMapAndreas MapAndreas;

//----------------------------------------------------------
// native MapAndreas_Init(mode);

cell AMX_NATIVE_CALL CMapAndreasNatives::Init(AMX *amx, cell *params)
{
	char* cname;
	amx_StrParam(amx, params[2], cname);

	int iRetVal = MapAndreas.Init(params[1], cname, params[3]);

	if(iRetVal != MAP_ANDREAS_ERROR_SUCCESS) {
		logprintf("MapAndreas: plugin could not init!");
		logprintf("MapAndreas: check files and make sure you have enough memory!");
		return 0;
	}
	return 1;
}

//----------------------------------------------------------
// native MapAndreas_FindZ_For2DCoord(Float:X, Float:Y, &Float:Z);

cell AMX_NATIVE_CALL CMapAndreasNatives::FindZ_For2DCoord(AMX *amx, cell *params)
{
	float X = amx_ctof(params[1]);
	float Y = amx_ctof(params[2]);
	float Z;

	Z = MapAndreas.FindZ_For2DCoord(X,Y);

	cell* cptr;
	amx_GetAddr(amx, params[3], &cptr);
	*cptr = amx_ftoc(Z);

	if(Z > 0.0f) return 1;

	return 0;
}

//----------------------------------------------------------
// native MapAndreas_SetZ_For2DCoord(Float:X, Float:Y, Float:Z);

cell AMX_NATIVE_CALL CMapAndreasNatives::SetZ_For2DCoord(AMX *amx, cell *params)
{
	float X = amx_ctof(params[1]);
	float Y = amx_ctof(params[2]);
	float Z = amx_ctof(params[3]);

	return MapAndreas.SetZ_For2DCoord(X, Y, Z);
}

//----------------------------------------------------------
// native MapAndreas_SaveCurrentHMap(AMX* amx, cell* params);

cell AMX_NATIVE_CALL CMapAndreasNatives::SaveCurrentHMap(AMX *amx, cell *params)
{
	char* name;

	amx_StrParam(amx, params[1], name);

	return MapAndreas.SaveCurrentHMap(name);
}

//----------------------------------------------------------
// native MapAndreas_Unload();

cell AMX_NATIVE_CALL CMapAndreasNatives::Unload(AMX *amx, cell *params)
{
	return MapAndreas.Unload();
}

//----------------------------------------------------------
// native MapAndreas_FindAverageZ(Float:X, Float:Y, &Float:Z);

cell AMX_NATIVE_CALL CMapAndreasNatives::FindAverageZ(AMX *amx, cell *params)
{
	float X = amx_ctof(params[1]);
	float Y = amx_ctof(params[2]);
	float Z;

	Z = MapAndreas.GetAverageZ(X,Y);

	cell* cptr;
	amx_GetAddr(amx, params[3], &cptr);
	*cptr = amx_ftoc(Z);

	if(Z > 0.0f) return 1;

	return 0;
}

//----------------------------------------------------------
// native MapAndreas_GetAddress();

cell AMX_NATIVE_CALL CMapAndreasNatives::GetAddress(AMX *amx, cell *params)
{
	//Test code
	if(MapAndreas.IsInited()) return (int) &MapAndreas;
	else return NULL;
}

//----------------------------------------------------------
