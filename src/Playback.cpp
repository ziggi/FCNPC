/* =========================================

		FCNPC - Fully Controllable NPC
			----------------------

	- File: Playback.cpp
	- Author(s): OrMisicL

  =========================================*/

#include "Main.hpp"

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
	if (m_pFile) {
		fclose(m_pFile);
	}

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
	if (!m_pFile) {
		logprintf("FCNPC Error: file '%s' not found.", szPath);
		return false;
	}

	// Check the file size
	fseek(m_pFile, 0, SEEK_END);
	size_t sSize = ftell(m_pFile);
	fseek(m_pFile, 0, SEEK_SET);
	if (!sSize) {
		fclose(m_pFile);
		return false;
	}
	// Read the playback type and file info ?
	DWORD dwFile;
	int iPlaybackType = 0;
	fread(&dwFile, sizeof(DWORD), 1, m_pFile);
	fread(&iPlaybackType, sizeof(int), 1, m_pFile);
	// Save the playback type
	if (iPlaybackType != PLAYBACK_TYPE_DRIVER && iPlaybackType != PLAYBACK_TYPE_ONFOOT) {
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
	if (m_bPaused) {
		// Process the player
		if (m_iPlaybackType == PLAYBACK_TYPE_DRIVER) {
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
		} else if (m_iPlaybackType == PLAYBACK_TYPE_ONFOOT) {
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
	if ((GetTickCount() - m_dwStartTime) >= m_dwTime) {
		// Read the first recording data
		if (m_iPlaybackType == PLAYBACK_TYPE_DRIVER) {
			// Read the in car sync data
			CVehicleSyncData vehicleSyncData;
			if (!fread(&vehicleSyncData, sizeof(CVehicleSyncData), 1, m_pFile)) {
				return false;
			}

			// Get the vehicle interface
			if (pPlayerData->GetVehicleId() == INVALID_VEHICLE_ID) {
				return false;
			}

			// Apply the sync data
			pPlayerData->SetState(PLAYER_STATE_DRIVER);
			pPlayerData->SetKeys(vehicleSyncData.wUDAnalog, vehicleSyncData.wLRAnalog, vehicleSyncData.wKeys);
			pPlayerData->SetQuaternion(vehicleSyncData.fQuaternion);
			pPlayerData->SetPosition(vehicleSyncData.vecPosition);
			pPlayerData->SetVelocity(vehicleSyncData.vecVelocity);
			pPlayerData->SetVehicleHealth(vehicleSyncData.fHealth);
			pPlayerData->SetHealth(vehicleSyncData.bytePlayerHealth);
			pPlayerData->SetArmour(vehicleSyncData.bytePlayerArmour);
			pPlayerData->SetWeapon(vehicleSyncData.bytePlayerWeapon);
			pPlayerData->SetVehicleSiren(vehicleSyncData.byteSirenState);
			pPlayerData->SetVehicleSync(&vehicleSyncData);

			// Save the last onfoot sync
			memcpy(&m_vehicleSyncData, &vehicleSyncData, sizeof(CVehicleSyncData));

			// Update the player
			pPlayerData->Update(UPDATE_STATE_DRIVER);
		} else if (m_iPlaybackType == PLAYBACK_TYPE_ONFOOT) {
			// Read the on foot sync data
			CSyncData syncData;
			if (!fread(&syncData, sizeof(CSyncData), 1, m_pFile)) {
				return false;
			}

			// Apply the sync data
			pPlayerData->SetState(PLAYER_STATE_ONFOOT);
			pPlayerData->SetKeys(syncData.wUDAnalog, syncData.wLRAnalog, syncData.wKeys);
			pPlayerData->SetPosition(syncData.vecPosition);
			pPlayerData->SetQuaternion(syncData.fQuaternion);
			pPlayerData->SetHealth(syncData.byteHealth);
			pPlayerData->SetArmour(syncData.byteArmour);
			pPlayerData->SetWeapon(syncData.byteWeapon);
			pPlayerData->SetSpecialAction(syncData.byteSpecialAction);
			pPlayerData->SetVelocity(syncData.vecVelocity);
			pPlayerData->SetSurfingOffsets(syncData.vecSurfing);
			pPlayerData->SetSurfingVehicle(syncData.wSurfingInfo);
			pPlayerData->SetOnFootSync(&syncData);

			// Save the last onfoot sync
			memcpy(&m_syncData, &syncData, sizeof(CSyncData));

			// Update the player
			pPlayerData->Update(UPDATE_STATE_ONFOOT);
		}
		// Update the time
		if (!fread(&m_dwTime, sizeof(DWORD), 1, m_pFile)) {
			return false;
		}
	}
	return true;
}

void CPlayback::SetPaused(bool bPaused)
{
	m_bPaused = bPaused;
}
