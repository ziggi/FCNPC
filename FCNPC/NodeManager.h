/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: NodeManager.h
	- Author(s): OrMisicL

  =========================================*/

#ifndef	NODEMANAGER_H
#define NODEMANAGER_H

#include "Node.h"

class CNodeManager
{
	public:
		CNodeManager();
		~CNodeManager();

		bool IsNodeOpen(int iNodeId)
		{ 
			if(iNodeId >= MAX_NODES || iNodeId < 0) 
				return false; 
			else 
				return m_bOpened[iNodeId]; 
		};
		inline CNode	*GetAt(int iNodeId) { return m_pNode[iNodeId]; };
		
		bool			OpenNode(int iNodeId);
		void			CloseNode(int iNodeId);

	private:
		bool			m_bOpened[MAX_NODES];
		CNode			*m_pNode[MAX_NODES];
};

#endif