/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: Playback.cpp
	- Author(s): OrMisicL

  =========================================*/

#include "Main.h"

extern CNetGame      *pNetGame;

CPlayback::CPlayback(char *szFile)
{
	// Save the file name
	strlcpy(m_szFile, szFile, sizeof(m_szFile));
	// Reset variables
	m_iPlaybackType = PLAYBACK_TYPE_NONE;
	m_bPaused = false;
	memset(&m_syncData, 0, sizeof(CSyncData));
	memset(&m_vehicleSyncData, 0, sizeof(CVehicleSyncData));
}

CPlayback::~CPlayback()
{
	// Close the file
	if (m_pFile)
		fclose(m_pFile);

	// Reset variables
	m_iPlaybackType = PLAYBACK_TYPE_NONE;
	m_bPaused = false;
	memset(&m_syncData, 0, sizeof(CSyncData));
	memset(&m_vehicleSyncData, 0, sizeof(CVehicleSyncData));
}

bool CPlayback::Initialize()
{
	// Format the file path
	char szPath[MAX_PATH];
	snprintf(szPath, sizeof(szPath), "npcmodes/recordings/%s.rec", m_szFile);
	// Try to open the playback file
	fopen_s(&m_pFile, szPath, "rb");
	if (!m_pFile)
		return false;

	// Check the file size
	fseek(m_pFile, 0, SEEK_END);
	size_t sSize = ftell(m_pFile);
	fseek(m_pFile, 0, SEEK_SET);
	if (!sSize)
	{
		fclose(m_pFile);
		return false;
	}
	// Read the playback type and file info ?
	DWORD dwFile;
	int iPlaybackType = 0;
	fread(&dwFile, sizeof(DWORD), 1, m_pFile);
	fread(&iPlaybackType, sizeof(int), 1, m_pFile);
	// Save the playback type
	if (iPlaybackType < PLAYBACK_TYPE_DRIVER || iPlaybackType > PLAYBACK_TYPE_ONFOOT)
	{
		fclose(m_pFile);
		return false;
	}
	// Save the playback type
	m_iPlaybackType = iPlaybackType;
	// Read the first Playing time
	DWORD dwTime;
	fread(&dwTime, sizeof(DWORD), 1, m_pFile);
	m_dwTime = dwTime;
	// Get the starting time
	m_dwStartTime = GetTickCount();
	return true;
}

bool CPlayback::Process(CPlayerData *pPlayerData)
{
	// Dont process if its paused
	if (m_bPaused)
	{
		// Process the player
		if (m_iPlaybackType == PLAYBACK_TYPE_DRIVER)
		{
			// Set the state
			pPlayerData->SetState(PLAYER_STATE_DRIVER);
			// Pause the sync data
			m_vehicleSyncData.wUDAnalog = 0;
			m_vehicleSyncData.wLRAnalog = 0;
			m_vehicleSyncData.wKeys = 0;
			m_vehicleSyncData.vecVelocity = CVector(0.0f, 0.0f, 0.0f);
			// Set vehicle sync data
			pPlayerData->SetVehicleSync(&m_vehicleSyncData);		
			// Update the player
			pPlayerData->UpdateSync(UPDATE_STATE_DRIVER);
		}
		else
		{
			// Set the state
			pPlayerData->SetState(PLAYER_STATE_ONFOOT);
			// Pause the sync data
			m_syncData.wUDAnalog = 0;
			m_syncData.wLRAnalog = 0;
			m_syncData.wKeys = 0;
			m_syncData.vecVelocity = CVector(0.0f, 0.0f, 0.0f);
			// Set vehicle sync data
			pPlayerData->SetOnFootSync(&m_syncData);		
			// Update the player
			pPlayerData->UpdateSync(UPDATE_STATE_ONFOOT);
		}
		// Update the starting time
		m_dwStartTime = (GetTickCount() - m_dwTime);
		return true;
	}
	// Check the time
	if ((GetTickCount() - m_dwStartTime) >= m_dwTime)
	{
		// Read the first recording data
		if (m_iPlaybackType == PLAYBACK_TYPE_DRIVER)
		{
			// Read the in car sync data
			CVehicleSyncData vehicleSyncData;
			if (!fread(&vehicleSyncData, sizeof(CVehicleSyncData), 1, m_pFile))
				return false;

			// Get the vehicle interface
			CVehicle *pVehicle = pNetGame->pVehiclePool->pVehicle[pPlayerData->GetVehicleId()];
			// Set the data
			pVehicle->vecPosition = vehicleSyncData.vecPosition;
			pVehicle->vecVelocity = vehicleSyncData.vecVelocity;
			// Apply the sync data
			pPlayerData->SetState(PLAYER_STATE_DRIVER);
			// Save the position
			pPlayerData->SetPosition(vehicleSyncData.vecPosition);
			// Save the quaternion
			pPlayerData->SetQuaternion(vehicleSyncData.fQuaternion);
			// Set vehicle sync data
			pPlayerData->SetVehicleSync(&vehicleSyncData);
			// Save the last onfoot sync
			memcpy(&m_vehicleSyncData, &vehicleSyncData, sizeof(CVehicleSyncData));
			// Update the player
			pPlayerData->UpdateSync(UPDATE_STATE_DRIVER);
		}
		else
		{
			// Read the in car sync data
			CSyncData syncData;
			if (!fread(&syncData, sizeof(CSyncData), 1, m_pFile))
				return false;

			// Apply the sync data
			pPlayerData->SetState(PLAYER_STATE_ONFOOT);
			// Save the position
			pPlayerData->SetPosition(syncData.vecPosition);
			// Save the quaternion
			pPlayerData->SetQuaternion(syncData.fQuaternion);
			// Set onfoot data
			pPlayerData->SetOnFootSync(&syncData);
			// Save the last onfoot sync
			memcpy(&m_syncData, &syncData, sizeof(CSyncData));
			// Update the player
			pPlayerData->UpdateSync(UPDATE_STATE_ONFOOT);
		}
		// Update the time
		if (!fread(&m_dwTime, sizeof(DWORD), 1, m_pFile))
			return false;
	}
	return true;
}
