/* =========================================

		FCNPC - Fully Controllable NPC
			----------------------

	- File: CMovePath.cpp
	- Author(s): ziggi

  =========================================*/

#include "Main.hpp"

int CMovePath::Create()
{
	size_t index = m_vPath.size();
	m_vPath.push_back(std::vector<CVector>());
	return static_cast<int>(index);
}

bool CMovePath::Destroy(int iPathId)
{
	if (!IsPathValid(iPathId)) {
		return false;
	}
	m_vPath.erase(m_vPath.begin() + iPathId);
	return true;
}

bool CMovePath::IsPathValid(int iPathId)
{
	return static_cast<int>(m_vPath.size()) >= iPathId;
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
	size_t index = m_vPath.size();
	m_vPath[iPathId].push_back(vecPoint);
	return static_cast<int>(index);
}

bool CMovePath::RemovePoint(int iPathId, int iPointId)
{
	if (IsPointValid(iPathId, iPointId)) {
		return false;
	}
	m_vPath[iPathId].erase(m_vPath[iPathId].begin() + iPointId);
	return true;
}

bool CMovePath::IsPointValid(int iPathId, int iPointId)
{
	if (!IsPathValid(iPathId)) {
		return false;
	}
	return static_cast<int>(m_vPath[iPathId].size()) >= iPointId;
}

CVector *CMovePath::GetPoint(int iPathId, int iPointId)
{
	return &m_vPath[iPathId].at(iPointId);
}
