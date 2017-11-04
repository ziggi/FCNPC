// MapAndreas 1.2.1
// 20.7.2014
// Originally created by Kalcor, updated by Mauzen and Pamdex

#define MAP_ANDREAS_MODE_NONE			0
#define MAP_ANDREAS_MODE_MINIMAL		1
#define MAP_ANDREAS_MODE_MEDIUM			2	// currently unused
#define MAP_ANDREAS_MODE_FULL			3
#define MAP_ANDREAS_MODE_NOBUFFER		4

#define MAP_ANDREAS_ERROR_SUCCESS		0
#define MAP_ANDREAS_ERROR_FAILURE		1
#define MAP_ANDREAS_ERROR_MEMORY		2
#define MAP_ANDREAS_ERROR_DATA_FILES	3
#define MAP_ANDREAS_ERROR_INVALID_AREA	4

native MapAndreas_Init(mode, name[]="", len=sizeof(name));
native MapAndreas_FindZ_For2DCoord(Float:X, Float:Y, &Float:Z);
native MapAndreas_FindAverageZ(Float:X, Float:Y, &Float:Z);
native MapAndreas_Unload();
native MapAndreas_SetZ_For2DCoord(Float:X, Float:Y, Float:Z);
native MapAndreas_SaveCurrentHMap(name[]);
native MapAndreas_GetAddress(); //only for plugins