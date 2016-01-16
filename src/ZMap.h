/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: ZMap.h
	- Author(s): OrMisicL

  =========================================*/

#ifndef ZMAP_H
#define ZMAP_H

class CZMap
{
	public:
		CZMap();
		~CZMap();

		inline bool IsInitialized() { return m_bIsInitialized; };
		inline void SetPath(char *szPath) { strlcpy(m_szFilePath, szPath, sizeof(m_szFilePath)); };

		bool  Initialize();
		float GetGroundForCoord(CVector3 vecCoord);

	private:
		bool m_bIsInitialized;
		FILE *m_pFile;
		char m_szFilePath[MAX_PATH];
};

#endif
