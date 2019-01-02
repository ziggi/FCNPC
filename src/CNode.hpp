/* =========================================

		FCNPC - Fully Controllable NPC
			----------------------

	- File: Node.hpp
	- Author(s): OrMisicL

  =========================================*/

#ifndef NODE_H
#define NODE_H

#include "Structs.h"

class CPlayerData;

#pragma pack(push, 1)

// Header
struct CNodeHeader
{
	DWORD dwNodesNumber;        // 0x0000 - 0x0004
	DWORD dwVehicleNodesNumber; // 0x0004 - 0x0008
	DWORD dwPedNodesNumber;     // 0x0008 - 0x000C
	DWORD dwNaviNodesNumber;    // 0x000C - 0x0010
	DWORD dwLinksNumber;        // 0x0010 - 0x0014
	// Size = 0x14
};

// Path Node
struct CPathNode
{
	DWORD dwMemAddress; // 0x0000 - 0x0004
	DWORD dwUnknown1;   // 0x0004 - 0x0008
	short sPositionX;   // 0x0008 - 0x000A
	short sPositionY;   // 0x000A - 0x000C
	short sPositionZ;   // 0x000C - 0x000E
	WORD wUnknown2;     // 0x000E - 0x0010
	WORD wLinkId;       // 0x0010 - 0x0012
	WORD wAreaId;       // 0x0012 - 0x0014
	WORD wNodeId;       // 0x0014 - 0x0016
	BYTE bytePathWidth; // 0x0016 - 0x0017
	BYTE byteNodeType;  // 0x0017 - 0x0018
	DWORD dwFlags;      // 0x0018 - 0x001C
	// Size = 0x1C
};

// Navi Node
struct CNaviNode
{
	short sPositionX;    // 0x0000 - 0x0002
	short sPositionY;    // 0x0002 - 0x0004
	WORD wAreaId;        // 0x0004 - 0x0006
	WORD wNodeId;        // 0x0006 - 0x0008
	BYTE byteDirectionX; // 0x0008 - 0x0009
	BYTE byteDirectionY; // 0x0009 - 0x000A
	DWORD dwFlags;       // 0x000A - 0x000E
	// Size = 0x0E
};

// Link Node
struct CLinkNode
{
	WORD wAreaId; // 0x0000 - 0x0002
	WORD wNodeId; // 0x0002 - 0x0004
	// Size = 0x4
};

#pragma pack(pop)

class CNode
{
public:
	CNode(int iNodeId);
	~CNode();

	bool Initialize();
	WORD Process(CPlayerData *pPlayerData, WORD wPointId, WORD wLastPoint);
	WORD ProcessNodeChange(CPlayerData *pPlayerData, WORD wLinkId);

	void GetPosition(CVector *pVecPosition);
	int GetNodesNumber();
	void GetHeaderInfo(DWORD *dwVehicleNodes, DWORD *dwPedNodes, DWORD *dwNaviNodes);

	int GetNodeId();
	WORD GetLinkId();
	WORD GetAreaId();
	WORD GetPointId();
	WORD GetLinkCount();
	BYTE GetPathWidth();
	BYTE GetNodeType();

	WORD GetLinkPoint();

	bool SetLink(WORD wLinkId);
	bool SetPoint(WORD wPointId);

private:
	int m_iNodeId;
	FILE *m_pFile;
	CNodeHeader m_nodeHeader;
	CPathNode m_nodePath;
	CLinkNode m_nodeLink;
};

#endif
