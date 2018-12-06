/* =========================================

		FCNPC - Fully Controllable NPC
			----------------------

	- File: CRecord.cpp
	- Author(s): ziggi

  =========================================*/

#include "Main.hpp"

int CRecordManager::Load(char *szFile)
{
	// try to find file in memory
	int index = Find(szFile);
	if (index != INVALID_RECORD_ID) {
		return index;
	}

	// if file is not loaded
	FILE *pFile;
	// Try to open the playback file
	fopen_s(&pFile, szFile, "rb");
	if (!pFile) {
		return INVALID_RECORD_ID;
	}

	// Check the file size
	fseek(pFile, 0, SEEK_END);
	size_t sSize = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);
	if (!sSize) {
		fclose(pFile);
		return INVALID_RECORD_ID;
	}

	size_t sElementsRead = 0;
	// Read the playback type and file info ?
	DWORD dwFile;
	int iPlaybackType = 0;
	sElementsRead = fread(&dwFile, sizeof(DWORD), 1, pFile);
	if (sElementsRead != 1) {
		logprintf("[FCNPC] Error: was not able to copy dwFile data to memory.");
	}

	sElementsRead = fread(&iPlaybackType, sizeof(int), 1, pFile);
	if (sElementsRead != 1) {
		logprintf("[FCNPC] Error: was not able to copy iPlaybackType data to memory.");
	}
	// Check the playback type
	if (iPlaybackType != PLAYBACK_TYPE_DRIVER && iPlaybackType != PLAYBACK_TYPE_ONFOOT) {
		fclose(pFile);
		return INVALID_RECORD_ID;
	}

	// add new record
	DWORD dwTime;
	Record_t recordNew;
	strlcpy(recordNew.szFile, szFile, sizeof(recordNew.szFile));
	recordNew.iPlaybackType = iPlaybackType;

	if (iPlaybackType == PLAYBACK_TYPE_DRIVER) {
		CVehicleSyncData vehicleSyncData;

		while (!feof(pFile)) {
			if (!fread(&dwTime, sizeof(DWORD), 1, pFile)) {
				break;
			} else if (!fread(&vehicleSyncData, sizeof(CVehicleSyncData), 1, pFile)) {
				break;
			}

			recordNew.v_dwTime.push_back(dwTime);
			recordNew.v_vehicleSyncData.push_back(vehicleSyncData);
		}
	} else if (iPlaybackType == PLAYBACK_TYPE_ONFOOT) { //-V547
		CSyncData playerSyncData;

		while (!feof(pFile)) {
			if (!fread(&dwTime, sizeof(DWORD), 1, pFile)) {
				break;
			} else if (!fread(&playerSyncData, sizeof(CSyncData), 1, pFile)) {
				break;
			}

			recordNew.v_dwTime.push_back(dwTime);
			recordNew.v_playerSyncData.push_back(playerSyncData);
		}
	}

	fclose(pFile);

	index = m_vSyncData.size();
	m_vSyncData.push_back(recordNew);
	return static_cast<int>(index);
}

bool CRecordManager::Unload(int iRecordId)
{
	if (!IsValid(iRecordId)) {
		return false;
	}
	m_vSyncData.erase(m_vSyncData.begin() + iRecordId);
	return true;
}

bool CRecordManager::IsValid(int iRecordId)
{
	return iRecordId >= 0 && static_cast<int>(m_vSyncData.size()) > iRecordId;
}

int CRecordManager::Find(char *szFile)
{
	for (auto it = m_vSyncData.begin(); it != m_vSyncData.end(); ++it) {
		if (strcmp(it->szFile, szFile) == 0) {
			return std::distance(m_vSyncData.begin(), it);
		}
	}
	return INVALID_RECORD_ID;
}

Record_t CRecordManager::Get(int iRecordId)
{
	if (!IsValid(iRecordId)) {
		return Record_t();
	}
	return m_vSyncData[iRecordId];
}
