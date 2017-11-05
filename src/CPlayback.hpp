/* =========================================

		FCNPC - Fully Controllable NPC
			----------------------

	- File: Playback.hpp
	- Author(s): OrMisicL

  =========================================*/

#ifndef PLAYBACK_H
#define PLAYBACK_H

#include "Structs.h"

class CPlayerData;

class CPlayback
{
public:
	CPlayback(char *szFile, char *szPlayingPath, bool bAutoUnload);
	CPlayback(int iRecordId, bool bAutoUnload);
	~CPlayback();

	bool Initialize(CVector vecPoint, float *fQuaternion);
	bool Process(CPlayerData *pPlayerData);
	void UnloadRecord();
	void SetPaused(bool bPaused);

private:
	bool m_bAutoUnload;
	int m_iRecordId;
	DWORD m_dwStartTime;
	bool m_bPaused;
	Record_t m_recordData;
	int m_iCurrentIndex;
};

#endif
