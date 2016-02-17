/* =========================================

		FCNPC - Fully Controllable NPC
			----------------------

	- File: Playback.h
	- Author(s): OrMisicL

  =========================================*/

#ifndef PLAYBACK_H
#define PLAYBACK_H

#include "Structs.h"

class CPlayerData;

class CPlayback
{
public:
	CPlayback(char *szFile);
	~CPlayback();

	bool Initialize();
	bool Process(CPlayerData *pPlayerData);

	void SetPaused(bool bPaused)
	{
		m_bPaused = bPaused;
	};

private:
	char				m_szFile[MAX_PATH];
	FILE				*m_pFile;
	int					m_iPlaybackType;
	DWORD				m_dwTime;
	DWORD				m_dwStartTime;
	bool				m_bPaused;
	CSyncData			m_syncData;
	CVehicleSyncData	m_vehicleSyncData;
};

#endif
