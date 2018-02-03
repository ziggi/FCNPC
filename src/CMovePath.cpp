/* =========================================

		FCNPC - Fully Controllable NPC
			----------------------

	- File: CMovePath.cpp
	- Author(s): ziggi

  =========================================*/

#include "Main.hpp"

int CMovePath::Create()
{
	int id = 0;

	// find free path id
	for (auto const &value : m_vPathID) {
		if (value != id) {
			break;
		}
		id++;
	}

	try {
		if (static_cast<size_t>(id) == m_vPath.size()) {
			m_vPath.push_back(std::vector<CVector>());
			m_vPointsID.push_back(std::vector<int>());
			m_vPathID.push_back(id);
		} else {
			m_vPath.insert(m_vPath.begin() + id, std::vector<CVector>());
			m_vPointsID.insert(m_vPointsID.begin() + id, std::vector<int>());
			m_vPathID.insert(m_vPathID.begin() + id, id);
		}
	} catch (...) {
		return INVALID_MOVEPATH_ID;
	}
	return id;
}

bool CMovePath::Destroy(int iPathId)
{
	if (!IsPathValid(iPathId)) {
		return false;
	}
	m_vPath.erase(m_vPath.begin() + iPathId);
	m_vPointsID.erase(m_vPointsID.begin() + iPathId);
	m_vPathID.erase(m_vPathID.begin() + iPathId);
	return true;
}

bool CMovePath::IsPathValid(int iPathId)
{
	// is not in range
	if (m_vPathID.size() == 0) {
		return false;
	}

	if (iPathId < 0 || iPathId > m_vPathID.back()) {
		return false;
	}

	// try to find the path
	return std::find(m_vPathID.begin(), m_vPathID.end(), iPathId) != m_vPathID.end();
}

std::vector<CVector> *CMovePath::GetPoints(int iPathId)
{
	return &m_vPath[iPathId];
}

int CMovePath::AddPoint(int iPathId, CVector vecPoint)
{
	if (!IsPathValid(iPathId)) {
		return -1;
	}

	int id = 0;

	// find free point id
	for (auto const &value : m_vPointsID[iPathId]) {
		if (value != id) {
			break;
		}
		id++;
	}

	try {
		if (static_cast<size_t>(id) == m_vPath.size()) {
			m_vPath[iPathId].push_back(vecPoint);
			m_vPointsID[iPathId].push_back(id);
		} else {
			m_vPath[iPathId].insert(m_vPath[iPathId].begin() + id, vecPoint);
			m_vPointsID[iPathId].insert(m_vPointsID[iPathId].begin() + id, id);
		}
	} catch (...) {
		return -1;
	}
	return id;
}

bool CMovePath::RemovePoint(int iPathId, int iPointId)
{
	if (IsPointValid(iPathId, iPointId)) {
		return false;
	}
	m_vPath[iPathId].erase(m_vPath[iPathId].begin() + iPointId);
	m_vPointsID[iPathId].erase(m_vPointsID[iPathId].begin() + iPointId);
	return true;
}

bool CMovePath::IsPointValid(int iPathId, int iPointId)
{
	if (!IsPathValid(iPathId)) {
		return false;
	}

	// is not in range
	if (m_vPointsID[iPathId].size() == 0) {
		return false;
	}

	if (iPointId < 0 || iPointId > m_vPointsID[iPathId].back()) {
		return false;
	}

	// try to find the path point
	return std::find(m_vPointsID[iPathId].begin(), m_vPointsID[iPathId].end(), iPathId) != m_vPointsID[iPathId].end();
}

CVector *CMovePath::GetPoint(int iPathId, int iPointId)
{
	if (!IsPointValid(iPathId, iPointId)) {
		return NULL;
	}
	return &m_vPath[iPathId].at(iPointId);
}
