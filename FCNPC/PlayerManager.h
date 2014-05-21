/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: PlayerManager.h
	- Author(s): OrMisicL

  =========================================*/

#ifndef PLAYERMANAGER_H
#define PLAYERMANAGER_H

#include "Player.h"

class CPlayerManager
{
	public:
		CPlayerManager();
		~CPlayerManager();

		bool IsPlayerConnected(EntityId playerId)
		{ 
			if(playerId > MAX_PLAYERS || playerId < 0) 
				return false; 
			else 
				return m_bConnected[playerId]; 
		};
		inline CPlayer *GetAt(EntityId playerId) { return m_pPlayer[playerId]; };
		bool			SetupPlayer(EntityId playerId);
		
		EntityId		AddPlayer(char *szName);
		bool			DeletePlayer(EntityId playerId);

		void			Process();

		bool			IsNPC(int iPlayerId);

	private:
		EntityId		m_players;
		bool			m_bConnected[MAX_PLAYERS];
		CPlayer			*m_pPlayer[MAX_PLAYERS];
};

#endif