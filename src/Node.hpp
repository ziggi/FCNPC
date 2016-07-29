/* =========================================

		FCNPC - Fully Controllable NPC
			----------------------

	- File: Node.hpp
	- Author(s): OrMisicL

  =========================================*/

#ifndef NODE_H
#define NODE_H

#include "Structs.h"
#include "CNode.hpp"

class CPlayerData;

class CNode
{
public:
	CNode(int iNodeId);
	~CNode();

	bool Initialize();
	int Process(CPlayerData *pPlayerData, int iPointId, int iLastPoint, int iType, CVector vecVelocity);
	int ProcessNodeChange(CPlayerData *pPlayerData, WORD wLinkId, int iType, CVector vecVelocity);

	void SetPaused(bool bPaused);
	void GetPosition(CVector *pVecPosition);
	int GetNodesNumber();
	void GetHeaderInfo(DWORD *dwVehicleNodes, DWORD *dwPedNodes, DWORD *dwNaviNodes);

	WORD GetLinkId();
	WORD GetAreaId();
	WORD GetPointId();
	WORD GetLinkCount();
	BYTE GetPathWidth();
	BYTE GetNodeType();

	WORD GetLinkPoint();

	void SetLink(WORD wLinkId);
	void SetPoint(WORD usPointId);

private:
	int m_iNodeId;
	FILE *m_pFile;
	bool m_bPaused;
	CNodeHeader m_nodeHeader;
	CPathNode m_nodePath;
	CLinkNode m_nodeLink;
};

#endif
