/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: Playback.h
	- Author(s): OrMisicL

  =========================================*/

#ifndef PLAYBACK_H
#define PLAYBACK_H

#include "SAMPPlayer.h"

class CPlayer;

#ifndef _WIN32
#	define MAX_PATH 260
#endif

class CPlayback
{
	public:
		CPlayback(char *szFile);
		~CPlayback();

		bool Initialize();
		bool Process(CPlayer *pPlayer);

		void SetPaused(bool bPaused) { m_bPaused = bPaused; };

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