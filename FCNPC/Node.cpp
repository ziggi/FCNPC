/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: Node.cpp
	- Author(s): OrMisicL

  =========================================*/

#include "Main.h"

extern CServer		*pServer;

CNode::CNode(int iNodeId)
{
	// Save the node id
	m_iNodeId = iNodeId;
	// Reset file pointer
	m_pFile = NULL;
	// Reset paused state
	m_bPaused = false;
}

CNode::~CNode()
{
	// Close the file
	if(m_pFile)
		fclose(m_pFile);
}

bool CNode::Initialize()
{
	// Validate the node id
	if(m_iNodeId < 0 || m_iNodeId >= MAX_NODES)
		return false;

	// Format the file path
	char szPath[MAX_PATH];
	sprintf(szPath, "scriptfiles/FCNPC/Nodes/NODES%d.DAT", m_iNodeId);
	// Try to open the node file
	m_pFile = fopen(szPath, "rb");
	if(!m_pFile)
		return false;

	// Check the file size
	fseek(m_pFile, 0, SEEK_END);
	size_t sSize = ftell(m_pFile);
	fseek(m_pFile, 0, SEEK_SET);
	if(!sSize)
	{
		fclose(m_pFile);
		return false;
	}
	// Read the node header
	fread(&m_nodeHeader, sizeof(CSANodeHeader), 1, m_pFile);
	// Read the first path node
	fread(&m_nodePath, sizeof(CSAPathNode), 1, m_pFile);

	return true;
}

void CNode::GetHeaderInfo(unsigned long *pulVehicleNodes, unsigned long *pulPedNodes, unsigned long *pulNaviNodes)
{
	*pulVehicleNodes = m_nodeHeader.ulVehicleNodesNumber;
	*pulPedNodes = m_nodeHeader.ulPedNodesNumber;
	*pulNaviNodes = m_nodeHeader.ulNaviNodesNumber;
}

int CNode::Process(CPlayer *pPlayer, int iPointId, int iLastPoint, int iType, CVector3 vecVelocity)
{
	int iChangeNode = 0;
	// Set the node to the player point
	SetPoint(iPointId);
	// Set it to the next link
	unsigned short usStartLink = GetLinkId();
	unsigned short usLinkCount = GetLinkCount();
	BYTE byteCount = 0;
	// Do we need to change the node ?
	while(!iChangeNode)
	{
		// Generate a random link id
		unsigned short usLinkId = usStartLink + (rand() % usLinkCount);
		// Set the node to the next random link
		SetLink(usLinkId);
		// Keep looping until we get a differente link point
		while(m_nodeLink.usNodeId == iLastPoint && usLinkCount > 1)
		{
			// Increase the attempts count
			byteCount++;
			if(byteCount > 10) 
				break;

			// Generate a random link id
			unsigned short usLinkId = usStartLink + (rand() % usLinkCount);
			// Set the node to the next random link
			SetLink(usLinkId);
		}
		// Check if we need to change the node id
		if(m_nodeLink.usAreaId != m_iNodeId)
		{
			if(m_nodeLink.usAreaId != 65535)
			{
				if((iChangeNode = CCallbackManager::OnChangeNode(pPlayer->GetId(), (int)m_nodeLink.usAreaId)))
					return pPlayer->ChangeNode(m_nodeLink.usAreaId, usLinkId);
			}
			else
				return 0;
		}
		else
		{
			// Set the next point
			SetPoint(m_nodeLink.usNodeId);
			// Get the point position
			CVector3 vecPosition;
			GetPosition(&vecPosition);
			// Set the player velocity
			pPlayer->SetVelocity(vecVelocity);
			// Move the player to it
			pPlayer->GoTo(vecPosition, iType == NODE_TYPE_PED ? MOVE_TYPE_WALK : MOVE_TYPE_DRIVE, true);

			return m_nodeLink.usNodeId;
		}
	}
	return 0;
}

int CNode::ProcessNodeChange(CPlayer *pPlayer, unsigned short usLinkId, int iType, CVector3 vecVelocity)
{
	// Set the node link
	SetLink(usLinkId);
	// Set the next point
	SetPoint(m_nodeLink.usNodeId);
	// Get the point position
	CVector3 vecPosition;
	GetPosition(&vecPosition);
	// Set the player velocity
	pPlayer->SetVelocity(vecVelocity);
	// Move the player to it
	pPlayer->GoTo(vecPosition, iType == NODE_TYPE_PED ? MOVE_TYPE_WALK : MOVE_TYPE_DRIVE, true);

	return m_nodeLink.usNodeId;
}

void CNode::GetPosition(CVector3 *pVecPosition)
{
	// Get the node position
	*pVecPosition = CVector3((float)(m_nodePath.sPositionX / 8), 
		(float)(m_nodePath.sPositionY / 8), (float)(m_nodePath.sPositionZ / 8) + 0.7f);
}

unsigned short CNode::GetLinkId()
{
	return m_nodePath.usLinkId;
}

unsigned short CNode::GetAreaId()
{
	return m_nodePath.usAreaId;
}

unsigned short CNode::GetPointId()
{
	return m_nodePath.usNodeId;
}

unsigned short CNode::GetLinkPoint()
{
	return m_nodeLink.usNodeId;
}

unsigned char CNode::GetPathWidth()
{
	return m_nodePath.ucPathWidth;
}

unsigned short CNode::GetLinkCount()
{
	return (unsigned short)(m_nodePath.ulFlags & 0xF);
}

unsigned char CNode::GetNodeType()
{
	if(m_nodeHeader.ulVehicleNodesNumber != 0 && m_nodeHeader.ulPedNodesNumber == 0)
		return m_nodePath.ucNodeType;
	else if(m_nodeHeader.ulVehicleNodesNumber == 0 && m_nodeHeader.ulPedNodesNumber != 0)
		return NODE_TYPE_PED;
	else
		return m_nodePath.ucNodeType;
}

void CNode::SetLink(unsigned short usLinkId)
{
	// Set the file pointer to the link position
	fseek(m_pFile, sizeof(CSANodeHeader) + ((m_nodeHeader.ulPedNodesNumber + m_nodeHeader.ulVehicleNodesNumber)
		* sizeof(CSAPathNode)) + (m_nodeHeader.ulNaviNodesNumber * sizeof(CSANaviNode)) + (usLinkId * sizeof(CSALinkNode)), SEEK_SET);

	// Read the node link
	fread(&m_nodeLink, sizeof(CSALinkNode), 1, m_pFile);
}

void CNode::SetPoint(unsigned short usPointId)
{
	// Validate the point
	if(usPointId > m_nodeHeader.ulNodesNumber)
		return;

	// Set the file pointer to the point position
	fseek(m_pFile, sizeof(CSANodeHeader) + (usPointId * sizeof(CSAPathNode)), SEEK_SET);
	// Read the node link
	fread(&m_nodePath, sizeof(CSAPathNode), 1, m_pFile);
}
