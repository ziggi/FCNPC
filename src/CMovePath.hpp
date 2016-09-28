/* =========================================

		FCNPC - Fully Controllable NPC
			----------------------

	- File: CMovePath.hpp
	- Author(s): ziggi

  =========================================*/

#ifndef MOVEPATH_H
#define MOVEPATH_H

class CMovePath
{
public:
	int Create();
	bool Destroy(int iPathId);
	bool IsPathValid(int iPathId);
	std::vector<CVector> *GetPoints(int iPathId);

	int AddPoint(int iPathId, CVector vecPoint);
	bool RemovePoint(int iPathId, int iPointId);
	bool IsPointValid(int iPathId, int iPointId);
	CVector *GetPoint(int iPathId, int iPointId);

private:
	std::vector<std::vector<CVector>> m_vPath;
};

#endif
