/* =========================================

		FCNPC - Fully Controllable NPC
			----------------------

	- File: PlayerManager.hpp
	- Author(s): OrMisicL

  =========================================*/

#ifndef PLAYERMANAGER_H
#define PLAYERMANAGER_H

#include "CPlayerData.hpp"

class CPlayerManager
{
public:
	CPlayerManager();
	~CPlayerManager();

	bool IsNpcConnected(WORD wPlayerId);
	bool IsPlayerConnected(WORD wPlayerId);
	CPlayerData *GetAt(WORD wPlayerId);
	bool SetupPlayer(WORD wPlayerId);

	WORD AddPlayer(char *szName);
	bool DeletePlayer(WORD wPlayerId);

	void Process();

	bool IsNPC(WORD wPlayerId);

private:
	CPlayerData *m_pNpcArray[MAX_PLAYERS];
	std::vector<int> m_vNpcID;
};

#endif
