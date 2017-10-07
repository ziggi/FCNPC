/* =========================================

		FCNPC - Fully Controllable NPC
			----------------------

	- File: Node.cpp
	- Author(s): OrMisicL

  =========================================*/

#include "Main.hpp"

extern CServer		*pServer;

CNode::CNode(int iNodeId)
{
	// Save the node id
	m_iNodeId = iNodeId;
	// Reset file pointer
	m_pFile = NULL;
}

CNode::~CNode()
{
	// Close the file
	if (m_pFile) {
		fclose(m_pFile);
	}
}

bool CNode::Initialize()
{
	// Validate the node id
	if (m_iNodeId < 0 || m_iNodeId >= MAX_NODES) {
		return false;
	}

	// Format the file path
	char szPath[MAX_PATH];
	snprintf(szPath, sizeof(szPath), "scriptfiles/FCNPC/nodes/NODES%d.DAT", m_iNodeId);
	// Try to open the node file
	fopen_s(&m_pFile, szPath, "rb");
	if (!m_pFile) {
		return false;
	}

	// Check the file size
	fseek(m_pFile, 0, SEEK_END);
	size_t sSize = ftell(m_pFile);
	fseek(m_pFile, 0, SEEK_SET);
	if (!sSize) {
		fclose(m_pFile);
		return false;
	}
	// Read the node header
	fread(&m_nodeHeader, sizeof(CNodeHeader), 1, m_pFile);
	// Read the first path node
	fread(&m_nodePath, sizeof(CPathNode), 1, m_pFile);

	return true;
}

int CNode::GetNodesNumber()
{
	return m_nodeHeader.dwNodesNumber;
}

void CNode::GetHeaderInfo(DWORD *dwVehicleNodes, DWORD *dwPedNodes, DWORD *dwNaviNodes)
{
	*dwVehicleNodes = m_nodeHeader.dwVehicleNodesNumber;
	*dwPedNodes = m_nodeHeader.dwPedNodesNumber;
	*dwNaviNodes = m_nodeHeader.dwNaviNodesNumber;
}

WORD CNode::Process(CPlayerData *pPlayerData, WORD wPointId, WORD wLastPoint)
{
	int iChangeNode = 0;
	// Set the node to the player point
	SetPoint(wPointId);
	// Set it to the next link
	WORD wStartLink = GetLinkId();
	WORD wLinkCount = GetLinkCount();
	BYTE byteCount = 0;
	WORD wLinkId = wStartLink;
	// Do we need to change the node ?
	while (!iChangeNode) {
		// Keep looping until we get a differente link point
		do {
			// Increase the attempts count
			byteCount++;
			if (byteCount > 10) {
				break;
			}

			// Generate a random link id
			wLinkId = wStartLink + (rand() % wLinkCount);
			// Set the node to the next random link
			SetLink(wLinkId);
		} while (m_nodeLink.wNodeId == wLastPoint && wLinkCount > 1);

		// Check if we need to change the node id
		if (m_nodeLink.wAreaId != m_iNodeId) {
			if (m_nodeLink.wAreaId != 65535) {
				iChangeNode = CCallbackManager::OnChangeNode(pPlayerData->GetId(), m_nodeLink.wAreaId);
				if (iChangeNode) {
					return pPlayerData->ChangeNode(m_nodeLink.wAreaId, wLinkId);
				}
			} else {
				return 0;
			}
		} else {
			pPlayerData->UpdateNodePoint(m_nodeLink.wNodeId);
			return m_nodeLink.wNodeId;
		}
	}
	return 0;
}

WORD CNode::ProcessNodeChange(CPlayerData *pPlayerData, WORD wLinkId)
{
	SetLink(wLinkId);
	pPlayerData->UpdateNodePoint(m_nodeLink.wNodeId);
	return m_nodeLink.wNodeId;
}

void CNode::GetPosition(CVector *pVecPosition)
{
	// Get the node position
	*pVecPosition = CVector(static_cast<float>(m_nodePath.sPositionX / 8),
	                        static_cast<float>(m_nodePath.sPositionY / 8),
	                        static_cast<float>(m_nodePath.sPositionZ / 8) + 1.2f);
}

WORD CNode::GetLinkId()
{
	return m_nodePath.wLinkId;
}

WORD CNode::GetAreaId()
{
	return m_nodePath.wAreaId;
}

WORD CNode::GetPointId()
{
	return m_nodePath.wNodeId;
}

WORD CNode::GetLinkPoint()
{
	return m_nodeLink.wNodeId;
}

BYTE CNode::GetPathWidth()
{
	return m_nodePath.bytePathWidth;
}

WORD CNode::GetLinkCount()
{
	return (WORD)(m_nodePath.dwFlags & 0xF);
}

BYTE CNode::GetNodeType()
{
	return m_nodePath.byteNodeType;
}

void CNode::SetLink(WORD wLinkId)
{
	// Set the file pointer to the link position
	fseek(m_pFile, sizeof(CNodeHeader)
	               + (m_nodeHeader.dwPedNodesNumber + m_nodeHeader.dwVehicleNodesNumber) * sizeof(CPathNode)
	               + (m_nodeHeader.dwNaviNodesNumber * sizeof(CNaviNode))
	               + (wLinkId * sizeof(CLinkNode))
	             , SEEK_SET);

	// Read the node link
	fread(&m_nodeLink, sizeof(CLinkNode), 1, m_pFile);
}

void CNode::SetPoint(WORD wPointId)
{
	// Validate the point
	if (wPointId > m_nodeHeader.dwNodesNumber) {
		return;
	}

	// Set the file pointer to the point position
	fseek(m_pFile, sizeof(CNodeHeader) + (wPointId * sizeof(CPathNode)), SEEK_SET);
	// Read the node link
	fread(&m_nodePath, sizeof(CPathNode), 1, m_pFile);
}
