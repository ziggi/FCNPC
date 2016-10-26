/* =========================================

		FCNPC - Fully Controllable NPC
			----------------------

	- File: CRecordManager.hpp
	- Author(s): ziggi

  =========================================*/

#ifndef RECORDMANAGER_H
#define RECORDMANAGER_H

struct Record_t
{
	char szFile[MAX_PATH];
	int iPlaybackType;
	std::vector<DWORD> v_dwTime;
	std::vector<CSyncData> v_playerSyncData;
	std::vector<CVehicleSyncData> v_vehicleSyncData;
};

class CRecordManager
{
public:
	int Load(char *szFile);
	bool Unload(int iRecordId);
	bool IsValid(int iRecordId);
	int Find(char *szFile);
	Record_t Get(int iRecordId);

private:
	std::vector<Record_t> m_vSyncData;
};

#endif
