/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: Node.h
	- Author(s): OrMisicL

  =========================================*/

#ifndef NODE_H
#define NODE_H

#include "SAMPPlayer.h"
#include "SANode.h"

class CPlayer;

class CNode
{
	public:
		CNode(int iNodeId);
		~CNode();

		bool Initialize();
		int  Process(CPlayer *pPlayer, int iPointId, int iLastPoint, int iType, CVector3 vecVelocity);
		int  ProcessNodeChange(CPlayer *pPlayer, unsigned short usLinkId, int iType, CVector3 vecVelocity);

		void SetPaused(bool bPaused) { m_bPaused = bPaused; };

		void				GetPosition(CVector3 *pVecPosition);

		int					GetNodesNumber() { return m_nodeHeader.ulNodesNumber; };
		void				GetHeaderInfo(unsigned long *pulVehicleNodes, unsigned long *pulPedNodes, unsigned long *pulNaviNodes);

		unsigned short		GetLinkId();
		unsigned short		GetAreaId();
		unsigned short		GetPointId();
		unsigned short		GetLinkCount();
		unsigned char		GetPathWidth();
		unsigned char		GetNodeType();

		unsigned short		GetLinkPoint();

		void				SetLink(unsigned short usLinkId);
		void				SetPoint(unsigned short usPointId);

	private:
		int					m_iNodeId;
		FILE				*m_pFile;
		bool				m_bPaused;
		CSANodeHeader		m_nodeHeader;
		CSAPathNode			m_nodePath;
		CSALinkNode			m_nodeLink;
};

#endif