//----------------------------------------------------------
//
//   SA-MP Multiplayer Modification For GTA:SA
//   Copyright 2004-2009 SA-MP Team
//
//----------------------------------------------------------

#ifndef MAPANDREAS_H
#define MAPANDREAS_H

#define MAP_ANDREAS_MODE_NONE			0
#define MAP_ANDREAS_MODE_MINIMAL		1
#define MAP_ANDREAS_MODE_MEDIUM			2	// currently unused
#define MAP_ANDREAS_MODE_FULL			3
#define MAP_ANDREAS_MODE_NOBUFFER		4

#define MAP_ANDREAS_MAP_SIZE			(6000*6000)
#define MAP_ANDREAS_POINTS_FULL			MAP_ANDREAS_MAP_SIZE
#define MAP_ANDREAS_POINTS_MEDIUM		(MAP_ANDREAS_MAP_SIZE / 4)
#define MAP_ANDREAS_POINTS_MINIMAL		(MAP_ANDREAS_MAP_SIZE / 9)

#define MAP_ANDREAS_GRID_FULL			1.0F
#define MAP_ANDREAS_GRID_MEDIUM			2.0F
#define MAP_ANDREAS_GRID_MINIMAL		3.0F

#define MAP_ANDREAS_HMAP_FILE_FULL		"scriptfiles/SAfull.hmap"
#define MAP_ANDREAS_HMAP_FILE_MINIMAL	"scriptfiles/SAmin.hmap"

#define MAP_ANDREAS_ERROR_SUCCESS		0
#define MAP_ANDREAS_ERROR_FAILURE		1
#define MAP_ANDREAS_ERROR_MEMORY		2
#define MAP_ANDREAS_ERROR_DATA_FILES	3

#define MAP_ANDREAS_MAX_NAME            128
//----------------------------------------------------------

class CMapAndreas
{
private:
	int	m_iOperatingMode;
	unsigned short *m_pPointData;
	FILE *mapFile;
	float m_gridSize;

public:
	CMapAndreas();
	CMapAndreas(const CMapAndreas *other); //for plugins
	~CMapAndreas();

	int		Init(int iMode, char* cname, int len);
	float	FindZ_For2DCoord(float X, float Y, int dataPos = -1);
	int		SetZ_For2DCoord(float X, float Y, float z);
	int		SaveCurrentHMap(char* name);
	float	GetAverageZ(float x, float y);
	bool	Unload();
	bool	IsInited();
	int		GetOperatingMode();
};

#endif