//----------------------------------------------------------
//
//   SA-MP Multiplayer Modification For GTA:SA
//   Copyright 2004-2010 SA-MP Team
//
//   Author: Kye 10 Jan 2010
//
//----------------------------------------------------------

#ifndef MAPANDREAS_NATIVES_H
#define MAPANDREAS_NATIVES_H

class CMapAndreasNatives
{
public:
	static cell AMX_NATIVE_CALL Init(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL FindZ_For2DCoord(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL FindAverageZ(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL Unload(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL SetZ_For2DCoord(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL SaveCurrentHMap(AMX *amx, cell *params);
	static cell AMX_NATIVE_CALL GetAddress(AMX *amx, cell *params);
};

#endif