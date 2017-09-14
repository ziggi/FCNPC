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
	WORD GetId(char *szName);
	CPlayerData *GetAt(WORD wPlayerId);
	bool SetupPlayer(WORD wPlayerId);

	WORD AddPlayer(char *szName);
	bool DeletePlayer(WORD wPlayerId);
	bool ResetPlayer(WORD wPlayerId);
	void ResetAllPlayers();

	void Process();

	bool IsNPC(WORD wPlayerId);

private:
	CPlayerData *m_pNpcArray[MAX_PLAYERS];
	std::vector<WORD> m_vNpcID;
};

#endif
