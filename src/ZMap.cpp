/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: ZMap.cpp
	- Author(s): OrMisicL

  =========================================*/

#include "Main.h"

CZMap::CZMap()
{
	// Reset parameters
	m_pFile = NULL;
	m_bIsInitialized = false;
	strcpy(m_szFilePath, "");
}

CZMap::~CZMap()
{
	// Close the file if its opened
	if(m_pFile)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}
	m_bIsInitialized = false;
}

bool CZMap::Initialize()
{
	// Close the previous file (if its already open)
	if(m_pFile)
		fclose(m_pFile);

	// Try to open the file
	m_pFile = fopen(m_szFilePath, "rb");
	if(!m_pFile)
		return false;

	m_bIsInitialized = true;
	return true;
}

float CZMap::GetGroundForCoord(CVector3 vecCoord) // From MapAndreas plugin by Kalcor
{
	// Make sure the ZMap is initialized
	if(!m_bIsInitialized)
		return 0.0f;

	// Validate the coordinates
	if(vecCoord.fX < -3000.0f || vecCoord.fX > 3000.0f || vecCoord.fY > 3000.0f || vecCoord.fY < -3000.0f) 
		return 0.0f;

	// Get the the coordinates grid
	int iGridX = ((int)vecCoord.fX) + 3000;
	int iGridY = (((int)vecCoord.fY) - 3000) * -1;
	// Find the grid point
	int iPoint = ((iGridY * 6000) + iGridX) * 2; 
	// Set the file pointer to the point
	fseek(m_pFile, iPoint, SEEK_SET);
	// Read the point
	unsigned short usPointData;
	fread(&usPointData, 1, sizeof(unsigned short), m_pFile);
	// Return the Z coordinate
	return (float)usPointData / 100.0f;
}
