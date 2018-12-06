/* =========================================

		FCNPC - Fully Controllable NPC
			----------------------

	- File: CMovePath.hpp
	- Author(s): ziggi

  =========================================*/

#ifndef MOVEPATH_H
#define MOVEPATH_H

#include <map>

class CMovePath
{
public:
	int Create();
	bool Destroy(int iPathId);
	bool IsPathValid(int iPathId);
	std::map<int, CVector> *GetPoints(int iPathId);

	int AddPoint(int iPathId, const CVector &vecPoint);
	bool RemovePoint(int iPathId, int iPointId);
	bool IsPointValid(int iPathId, int iPointId);
	CVector *GetPoint(int iPathId, int iPointId);

private:
	std::map<int, std::map<int, CVector>> m_mMovePath;
};

#endif
