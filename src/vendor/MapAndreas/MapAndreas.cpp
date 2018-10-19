//----------------------------------------------------------
//
//   SA-MP Multiplayer Modification For GTA:SA
//   Copyright 2004-2010 SA-MP Team
//
//   Author: Kye 10 Jan 2010
//
//----------------------------------------------------------

#include "Main.hpp"

//----------------------------------------------------------

CMapAndreas::CMapAndreas()
{
	m_iOperatingMode = MAP_ANDREAS_MODE_NONE;
	m_pPointData = NULL;
}

CMapAndreas::CMapAndreas(const CMapAndreas *other)
{
	//Pointer copy :D
	this->mapFile = other->mapFile;
	this->m_pPointData = other->m_pPointData;
	this->m_gridSize = other->m_gridSize;
	this->m_iOperatingMode = other->m_iOperatingMode;
}

//----------------------------------------------------------

CMapAndreas::~CMapAndreas()
{
	Unload();
}

//----------------------------------------------------------

int CMapAndreas::Init(int iMode, char* cname, int len)
{
	// check if already inited
	if (this->IsInited()) {
		return MAP_ANDREAS_ERROR_SUCCESS;
	}

	char* name = new char [MAP_ANDREAS_MAX_NAME];
	if (len > 1) {
		name = cname;
	} else {
		switch (iMode) {
			case MAP_ANDREAS_MODE_NOBUFFER:
			case MAP_ANDREAS_MODE_FULL:
				strlcpy(name, MAP_ANDREAS_HMAP_FILE_FULL, MAP_ANDREAS_MAX_NAME);
				break;
			case MAP_ANDREAS_MODE_MINIMAL:
				strlcpy(name, MAP_ANDREAS_HMAP_FILE_MINIMAL, MAP_ANDREAS_MAX_NAME);
				break;
		}
	}
	if (iMode == MAP_ANDREAS_MODE_FULL) {
		// allocate the memory we need
		m_pPointData = (unsigned short *)calloc(MAP_ANDREAS_POINTS_FULL, sizeof(unsigned short));
		if (NULL == m_pPointData) {
			return MAP_ANDREAS_ERROR_MEMORY;
		}

		// load the file contents in to our point data buffer
		FILE *fileInput = NULL;
		fopen_s(&fileInput, name, "rb");
		if (NULL == fileInput) {
			return MAP_ANDREAS_ERROR_DATA_FILES;
		}

		fread(&m_pPointData[0], MAP_ANDREAS_POINTS_FULL, sizeof(unsigned short), fileInput);

		fclose(fileInput);

		m_iOperatingMode = MAP_ANDREAS_MODE_FULL;
		m_gridSize = MAP_ANDREAS_GRID_FULL;
		return MAP_ANDREAS_ERROR_SUCCESS;
	} else if (iMode == MAP_ANDREAS_MODE_MINIMAL) {
		m_pPointData = (unsigned short *)calloc(MAP_ANDREAS_POINTS_MINIMAL, sizeof(unsigned short));
		if (NULL == m_pPointData) {
			return MAP_ANDREAS_ERROR_MEMORY;
		}

		FILE *fileInput = NULL;
		fopen_s(&fileInput, name, "rb");
		if (NULL == fileInput) {
			return MAP_ANDREAS_ERROR_DATA_FILES;
		}

		fread(&m_pPointData[0], MAP_ANDREAS_POINTS_MINIMAL, sizeof(unsigned short), fileInput);

		fclose(fileInput);

		m_iOperatingMode = MAP_ANDREAS_MODE_MINIMAL;
		m_gridSize = MAP_ANDREAS_GRID_MINIMAL;
		return MAP_ANDREAS_ERROR_SUCCESS;
	} else if (iMode == MAP_ANDREAS_MODE_NOBUFFER) {
		m_iOperatingMode = MAP_ANDREAS_MODE_NOBUFFER;
		m_gridSize = MAP_ANDREAS_GRID_FULL;
		m_pPointData = (unsigned short *)calloc(1, sizeof(unsigned short));
		fopen_s(&mapFile, name, "rb");
		if (NULL == mapFile) {
			return MAP_ANDREAS_ERROR_DATA_FILES;
		}
		return MAP_ANDREAS_ERROR_SUCCESS;
	}

	return MAP_ANDREAS_ERROR_FAILURE;
}

//----------------------------------------------------------

float CMapAndreas::FindZ_For2DCoord(float X, float Y, int dataPos)
{
	if (dataPos != -1) {
		// get data
		if (m_iOperatingMode == MAP_ANDREAS_MODE_FULL) {
			return (float)m_pPointData[dataPos] / 100.0f; // the data is a float stored as ushort * 100
		} else if (m_iOperatingMode == MAP_ANDREAS_MODE_MINIMAL) {
			return (float)m_pPointData[dataPos] / 100.0f;
		} else if (m_iOperatingMode == MAP_ANDREAS_MODE_NOBUFFER) {
			// Jump to the position in the file and read it
			fseek(mapFile, dataPos * sizeof(unsigned short), SEEK_SET);
			fread(&m_pPointData[0], 1, sizeof(unsigned short), mapFile);

			return (float)m_pPointData[0] / 100.0f;
		}
		return 0.0f;
	} else {
		// check for a co-ord outside the map
		if (X < -3000.0f || X > 3000.0f || Y > 3000.0f || Y < -3000.0f) {
			return 0.0f;
		}

		// get row/col on 6000x6000 grid
		int iGridX = ((int)X) + 3000;
		int iGridY = -(((int)Y) - 3000);
		int iDataPos;

		// get data
		if (m_iOperatingMode == MAP_ANDREAS_MODE_FULL) {
			iDataPos = (iGridY * 6000) + iGridX; // for every Y, increment by the number of cols, add the col index.
			return (float)m_pPointData[iDataPos] / 100.0f; // the data is a float stored as ushort * 100
		} else if (m_iOperatingMode == MAP_ANDREAS_MODE_MINIMAL) {
			iDataPos = ((iGridY / 3) * 2000) + iGridX / 3;	// skip every 2nd and 3rd line
			return (float)m_pPointData[iDataPos] / 100.0f;
		} else if (m_iOperatingMode == MAP_ANDREAS_MODE_NOBUFFER) {
			iDataPos = (iGridY * 6000) + iGridX;

			// Jump to the position in the file and read it
			fseek(mapFile, iDataPos * sizeof(unsigned short), SEEK_SET);
			fread(&m_pPointData[0], 1, sizeof(unsigned short), mapFile);

			return (float)m_pPointData[0] / 100.0f;
		}
		return 0.0f;
	}
}

int CMapAndreas::SetZ_For2DCoord(float X, float Y, float z)
{
	// check for a co-ord outside the map, or non-saveable z value
	if (X < -3000.0f || X > 3000.0f || Y > 3000.0f || Y < -3000.0f || z < 0.0f) {
		return 0;
	}

	// get row/col on 6000x6000 grid
	int iGridX = ((int)X) + 3000;
	int iGridY = -(((int)Y) - 3000);
	int iDataPos;

	if (m_iOperatingMode == MAP_ANDREAS_MODE_FULL) {
		iDataPos = (iGridY * 6000) + iGridX;
		m_pPointData[iDataPos] = (short)(z * 100.0f + 0.5); // Add 0.5 to round it properly
		return 1;
	} else {
		if (m_iOperatingMode == MAP_ANDREAS_MODE_MINIMAL) {
			iDataPos = ((iGridY / 3) * 2000) + iGridX / 3;	// skip every 2nd and 3rd line
			m_pPointData[iDataPos] = (short)(z * 100.0f + 0.5); // Add 0.5 to round it properly
			return 1;
		} else if (m_iOperatingMode == MAP_ANDREAS_MODE_NOBUFFER) {
			return 0;
		}
	}

	return 0;
}

int CMapAndreas::SaveCurrentHMap(char* name)
{
	int values = 0;
	switch (m_iOperatingMode) {
		case MAP_ANDREAS_MODE_NONE:
			return 0;
		case MAP_ANDREAS_MODE_FULL:
			values = MAP_ANDREAS_POINTS_FULL;
			break;
		case MAP_ANDREAS_MODE_MEDIUM:
			values = MAP_ANDREAS_POINTS_MEDIUM;
			break;
		case MAP_ANDREAS_MODE_MINIMAL:
			values = MAP_ANDREAS_POINTS_MINIMAL;
			break;
	}

	FILE *fileInput = NULL;
	fopen_s(&fileInput, name, "wb");
	if (NULL == fileInput) {
		return MAP_ANDREAS_ERROR_DATA_FILES;
	}

	fwrite(&m_pPointData[0], values, sizeof(unsigned short), fileInput);

	fclose(fileInput);

	return 1;
}

float CMapAndreas::GetAverageZ(float x, float y)
{
	// Get the Z value of 2 neighbor grids
	float p1 = FindZ_For2DCoord(x, y);

	float p2 = FindZ_For2DCoord(x + (x < 0.0f ? m_gridSize : -m_gridSize), y);
	float p3 = FindZ_For2DCoord(x, y + (y < 0.0f ? m_gridSize : -m_gridSize));

	// Filter the decimal part only
	double temp;
	float xx = (float)modf((double)x, &temp); // Linux compatibility
	float yy = (float)modf((double)y, &temp);
	if (xx < 0.0f) {
		x = -xx;
	}
	if (yy < 0.0f) {
		y = -yy;
	}

	// Calculate a linear approximation of the z coordinate
	temp = p1 + xx * (p1 - p2) + yy * (p1 - p3);

	return (float)temp;
}

bool CMapAndreas::Unload()
{
	// Close the file if neccessary
	if (mapFile != NULL) {
		fclose(mapFile);
	}
	// Free the used memory
	if (m_pPointData) {
		free(m_pPointData);
		m_pPointData = NULL;
	}
	m_iOperatingMode = MAP_ANDREAS_MODE_NONE;
	return true;
}

bool CMapAndreas::IsInited()
{
	// check if already inited
	if (m_iOperatingMode != MAP_ANDREAS_MODE_NONE) {
		return true;
	}
	return false;
}

int CMapAndreas::GetOperatingMode()
{
	return m_iOperatingMode;
}

//----------------------------------------------------------