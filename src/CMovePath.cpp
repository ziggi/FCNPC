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
	for (auto const &value : m_mMovePath) {
		if (value.first != id) {
			break;
		}
		id++;
	}

	m_mMovePath.insert({ id, std::map<int, CVector>()});
	return id;
}

bool CMovePath::Destroy(int iPathId)
{
	if (!IsPathValid(iPathId)) {
		return false;
	}

	m_mMovePath.erase(iPathId);
	return true;
}

bool CMovePath::IsPathValid(int iPathId)
{
	return m_mMovePath.count(iPathId) > 0;
}

std::map<int, CVector> *CMovePath::GetPoints(int iPathId)
{
	return &m_mMovePath[iPathId];
}

int CMovePath::AddPoint(int iPathId, const CVector &vecPoint)
{
	if (!IsPathValid(iPathId)) {
		return -1;
	}

	int id = 0;

	// find free path id
	for (auto const &value : m_mMovePath[iPathId]) {
		if (value.first != id) {
			break;
		}
		id++;
	}

	m_mMovePath[iPathId].insert({ id, vecPoint });
	return id;
}

bool CMovePath::RemovePoint(int iPathId, int iPointId)
{
	if (!IsPointValid(iPathId, iPointId)) {
		return false;
	}

	m_mMovePath[iPathId].erase(iPointId);
	return true;
}

bool CMovePath::IsPointValid(int iPathId, int iPointId)
{
	if (!IsPathValid(iPathId)) {
		return false;
	}

	return m_mMovePath[iPathId].count(iPointId) > 0;
}

CVector *CMovePath::GetPoint(int iPathId, int iPointId)
{
	if (!IsPointValid(iPathId, iPointId)) {
		return NULL;
	}

	return &m_mMovePath[iPathId].at(iPointId);
}
