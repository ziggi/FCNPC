/* =========================================

		FCNPC - Fully Controllable NPC
			----------------------

	- File: NodeManager.hpp
	- Author(s): OrMisicL

  =========================================*/

#ifndef	NODEMANAGER_H
#define NODEMANAGER_H

#include "CNode.hpp"

class CNodeManager
{
public:
	CNodeManager();
	~CNodeManager();

	bool IsNodeOpen(int iNodeId);
	CNode *GetAt(int iNodeId);

	bool OpenNode(int iNodeId);
	void CloseNode(int iNodeId);

private:
	bool m_bOpened[MAX_NODES];
	CNode *m_pNode[MAX_NODES];
};

#endif
