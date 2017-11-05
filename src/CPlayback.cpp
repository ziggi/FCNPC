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
	// Save the record data
	char szPath[MAX_PATH];
	snprintf(szPath, sizeof(szPath), "%s%s.rec", szPlayingPath, szFile);
	m_iRecordId = pServer->GetRecordManager()->Load(szPath);
	if (m_iRecordId == INVALID_RECORD_ID) {
		logprintf("[FCNPC] Error: Playback initialization failed. Playback file '%s' not found.", szPath);
	}
	m_bAutoUnload = bAutoUnload;
	// Reset variables
	m_dwStartTime = 0;
	m_bPaused = false;
	m_recordData = Record_t();
	m_iCurrentIndex = 0;
}

CPlayback::CPlayback(int iRecordId, bool bAutoUnload)
{
	// Save the record data
	m_iRecordId = iRecordId;
	m_bAutoUnload = bAutoUnload;
	// Reset variables
	m_dwStartTime = 0;
	m_bPaused = false;
	m_recordData = Record_t();
	m_iCurrentIndex = 0;
}

CPlayback::~CPlayback()
{
	// Reset variables
	if (m_bAutoUnload) {
		pServer->GetRecordManager()->Unload(m_iRecordId);
	}
}

bool CPlayback::Initialize(CVector vecPoint, float *fQuaternion)
{
	if (m_iRecordId == INVALID_RECORD_ID) {
		return false;
	}

	m_recordData = pServer->GetRecordManager()->Get(m_iRecordId);

	if (vecPoint != CVector()) {
		int iSize = static_cast<int>(m_recordData.v_dwTime.size());

		if (m_recordData.iPlaybackType == PLAYBACK_TYPE_DRIVER) {
			CVector vecPointOffset = m_recordData.v_vehicleSyncData[0].vecPosition - vecPoint;

			for (int i = 0; i < iSize; i++) {
				m_recordData.v_vehicleSyncData[i].vecPosition -= vecPointOffset;
			}
		} else if (m_recordData.iPlaybackType == PLAYBACK_TYPE_ONFOOT) {
			CVector vecPointOffset = m_recordData.v_playerSyncData[0].vecPosition - vecPoint;

			for (int i = 0; i < iSize; i++) {
				m_recordData.v_playerSyncData[i].vecPosition -= vecPointOffset;
			}
		}
	}
	
	m_dwStartTime = GetTickCount();
	return true;
}

bool CPlayback::Process(CPlayerData *pPlayerData)
{
	if (m_iCurrentIndex >= static_cast<int>(m_recordData.v_dwTime.size())) {
		return false;
	}
	// Dont process if its paused
	if (m_bPaused) {
		if (m_recordData.iPlaybackType == PLAYBACK_TYPE_DRIVER) {
			pPlayerData->ResetSyncMoving(UPDATE_STATE_DRIVER);
		} else if (m_recordData.iPlaybackType == PLAYBACK_TYPE_ONFOOT) {
			pPlayerData->ResetSyncMoving(UPDATE_STATE_ONFOOT);
		}
		// Update the starting time
		m_dwStartTime = (GetTickCount() - m_recordData.v_dwTime[m_iCurrentIndex]);
		return true;
	}
	// Check the time
	if ((GetTickCount() - m_dwStartTime) >= m_recordData.v_dwTime[m_iCurrentIndex]) {
		// Read the first recording data
		if (m_recordData.iPlaybackType == PLAYBACK_TYPE_DRIVER) {
			// Get the vehicle interface
			CVehicle *pVehicle = pPlayerData->GetVehicle();
			if (!pVehicle) {
				return false;
			}

			// Get the in car sync data
			CVehicleSyncData vehicleSyncData = m_recordData.v_vehicleSyncData[m_iCurrentIndex];

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
		} else if (m_recordData.iPlaybackType == PLAYBACK_TYPE_ONFOOT) {
			// Get the on foot sync data
			CSyncData syncData = m_recordData.v_playerSyncData[m_iCurrentIndex];

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
