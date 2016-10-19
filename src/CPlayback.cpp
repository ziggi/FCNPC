/* =========================================

		FCNPC - Fully Controllable NPC
			----------------------

	- File: Playback.cpp
	- Author(s): OrMisicL

  =========================================*/

#include "Main.hpp"

extern CServer  *pServer;
extern CNetGame *pNetGame;

CPlayback::CPlayback(char *szFile, char *szPlayingPath, bool bAutoUnload)
{
	// Save the file name
	char szPath[MAX_PATH];
	snprintf(szPath, sizeof(szPath), "%s%s.rec", szPlayingPath, szFile);
	m_iRecordId = pServer->GetRecordManager()->Load(szPath);
	if (m_iRecordId == INVALID_RECORD_ID) {
		logprintf("FCNPC Error: file '%s' not found.", szPath);
	}
	m_bAutoUnload = bAutoUnload;
	// Reset variables
	m_dwStartTime = 0;
	m_bPaused = false;
	m_pRecordData = NULL;
	m_iCurrentIndex = 0;
}

CPlayback::CPlayback(int iRecordId, bool bAutoUnload)
{
	// Save the record id
	m_iRecordId = iRecordId;
	m_bAutoUnload = bAutoUnload;
	// Reset variables
	m_dwStartTime = 0;
	m_bPaused = false;
	m_pRecordData = NULL;
	m_iCurrentIndex = 0;
}

CPlayback::~CPlayback()
{
	// Reset variables
	if (m_bAutoUnload) {
		pServer->GetRecordManager()->Unload(m_iRecordId);
	}
}

bool CPlayback::Initialize()
{
	if (m_iRecordId == INVALID_RECORD_ID) {
		return false;
	}
	m_pRecordData = pServer->GetRecordManager()->Get(m_iRecordId);
	m_dwStartTime = GetTickCount();
	return true;
}

bool CPlayback::Process(CPlayerData *pPlayerData)
{
	if (m_iCurrentIndex >= static_cast<int>(m_pRecordData->v_dwTime.size())) {
		return false;
	}
	// Dont process if its paused
	if (m_bPaused) {
		// Process the player
		if (m_pRecordData->iPlaybackType == PLAYBACK_TYPE_DRIVER) {
			// Set the state
			pPlayerData->SetState(PLAYER_STATE_DRIVER);
			// Pause the sync data
			CVehicleSyncData *vehicleSyncData = &m_pRecordData->v_vehicleSyncData.at(m_iCurrentIndex);
			vehicleSyncData->wUDAnalog = 0;
			vehicleSyncData->wLRAnalog = 0;
			vehicleSyncData->wKeys = 0;
			vehicleSyncData->vecVelocity = CVector(0.0f, 0.0f, 0.0f);
			// Set vehicle sync data
			pPlayerData->SetVehicleSync(vehicleSyncData);
			// Update the player
			pPlayerData->UpdateSync(UPDATE_STATE_DRIVER);
		} else if (m_pRecordData->iPlaybackType == PLAYBACK_TYPE_ONFOOT) {
			// Set the state
			pPlayerData->SetState(PLAYER_STATE_ONFOOT);
			// Pause the sync data
			CSyncData *playerSyncData = &m_pRecordData->v_playerSyncData.at(m_iCurrentIndex);
			playerSyncData->wUDAnalog = 0;
			playerSyncData->wLRAnalog = 0;
			playerSyncData->wKeys = 0;
			playerSyncData->vecVelocity = CVector(0.0f, 0.0f, 0.0f);
			// Set vehicle sync data
			pPlayerData->SetOnFootSync(playerSyncData);
			// Update the player
			pPlayerData->UpdateSync(UPDATE_STATE_ONFOOT);
		}
		// Update the starting time
		m_dwStartTime = (GetTickCount() - m_pRecordData->v_dwTime[m_iCurrentIndex]);
		return true;
	}
	// Check the time
	if ((GetTickCount() - m_dwStartTime) >= m_pRecordData->v_dwTime[m_iCurrentIndex]) {
		// Read the first recording data
		if (m_pRecordData->iPlaybackType == PLAYBACK_TYPE_DRIVER) {
			// Read the in car sync data
			CVehicleSyncData vehicleSyncData;
			memcpy(&vehicleSyncData, &m_pRecordData->v_vehicleSyncData[m_iCurrentIndex], sizeof(CVehicleSyncData));

			// Get the vehicle interface
			CVehicle *pVehicle = pPlayerData->GetVehicle();
			if (!pVehicle) {
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
			pPlayerData->SetVehicleSiren(vehicleSyncData.byteSirenState != 0);

			if (CVehicleInfo::IsAHydra(static_cast<WORD>(pVehicle->customSpawn.dwModelID))) {
				pPlayerData->SetVehicleHydraThrusters(vehicleSyncData.wHydraReactorAngle[0]);
			} else if (CVehicleInfo::IsATrainPart(static_cast<WORD>(pVehicle->customSpawn.dwModelID))) {
				pPlayerData->SetTrainSpeed(vehicleSyncData.fTrainSpeed);
			}

			pPlayerData->SetVehicleSync(&vehicleSyncData);

			// Update the player
			pPlayerData->Update(UPDATE_STATE_DRIVER);
		} else if (m_pRecordData->iPlaybackType == PLAYBACK_TYPE_ONFOOT) {
			// Read the on foot sync data
			CSyncData syncData;
			memcpy(&syncData, &m_pRecordData->v_playerSyncData[m_iCurrentIndex], sizeof(CSyncData));

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

			// Update the player
			pPlayerData->Update(UPDATE_STATE_ONFOOT);
		}
		// Update the index
		m_iCurrentIndex++;
	}
	return true;
}

void CPlayback::UnloadRecord()
{
	pServer->GetRecordManager()->Unload(m_iRecordId);
}

void CPlayback::SetPaused(bool bPaused)
{
	m_bPaused = bPaused;
}
