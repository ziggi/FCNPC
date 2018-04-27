/* =========================================

		FCNPC - Fully Controllable NPC
			----------------------

	- File: NodeManager.cpp
	- Author(s): OrMisicL

  =========================================*/


#include "Main.hpp"

extern CServer			*pServer;

CNodeManager::CNodeManager()
{
	// Reset node values
	for (WORD i = 0; i < MAX_NODES; i++) {
		m_bOpened[i] = false;
		m_pNode[i] = NULL;
	}
}

CNodeManager::~CNodeManager()
{
	// Reset player values
	for (WORD i = 0; i < MAX_NODES; i++) {
		if (m_pNode[i]) {
			delete m_pNode[i];
			m_pNode[i] = NULL;
		}
	}
}

bool CNodeManager::IsNodeOpen(int iNodeId)
{
	if (iNodeId >= MAX_NODES || iNodeId < 0) {
		return false;
	} else {
		return m_bOpened[iNodeId];
	}
}

CNode *CNodeManager::GetAt(int iNodeId)
{
	return m_pNode[iNodeId];
}

bool CNodeManager::OpenNode(int iNodeId)
{
	// Validate the node
	if (iNodeId < 0 || iNodeId >= MAX_NODES) {
		return false;
	}

	// Make sure the node is not already open
	if (m_bOpened[iNodeId]) {
		return false;
	}

	// Create the node instance
	m_pNode[iNodeId] = new CNode(iNodeId);

	// Try to initialize the node
	if (!m_pNode[iNodeId]->Initialize()) {
		SAFE_DELETE(m_pNode[iNodeId]);
		return false;
	}
	// Mark opened
	m_bOpened[iNodeId] = true;
	return true;
}

void CNodeManager::CloseNode(int iNodeId)
{
	// Validate the node
	if (iNodeId < 0 || iNodeId >= MAX_NODES) {
		return;
	}

	// Make sure the node is opened
	if (!m_bOpened[iNodeId]) {
		return;
	}

	// Delete the node instance
	SAFE_DELETE(m_pNode[iNodeId]);
	// Mark not opened
	m_bOpened[iNodeId] = false;
}
