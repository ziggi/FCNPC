#ifndef YSF_PICKUPPOOL_H
#define YSF_PICKUPPOOL_H

#include "CVector.h"
#include <vector>
#include <bitset>
#include <cstddef>
//#include "Main.h"

#ifdef NEW_PICKUP_SYSTEM
enum ePickupType : BYTE
{
	GLOBAL,
	PLAYER
};

class CPickup
{
public:
	CPickup(int model, int type, CVector pos, int world, ePickupType pickuptype)
	{
		this->iModel = model;
		this->iType = type;
		this->vecPos = pos;
		this->iWorld = world;
		this->type = pickuptype;
	}

	int	iModel;
	int	iType;
	CVector	vecPos;
	int iWorld;
	ePickupType type;
};

#ifdef _WIN32 // xD.. C++11 doesn't like BitStream.h/.cpp on linux
#include <unordered_map>
typedef std::unordered_map<int, CPickup*> PickupMap;
#else
#include <map>
typedef std::map<int, CPickup*> PickupMap;
#endif

class CPickupPool
{
public:
	CPickupPool();
	~CPickupPool();

	void InitializeForPlayer(WORD playerid);

	int New(int modelid, int type, CVector vecPos, int world);
	int New(WORD playerid, int modelid, int type, CVector vecPos, int world);
	void Destroy(int pickupid);
	void Destroy(WORD playerid, int pickupid);

	bool IsStreamed(WORD playerid, CPickup *pPickup);
	CPickup* FindPickup(int pickupid);
	CPickup* FindPickup(WORD playerid, int pickupid);
	int FindPickup(CPickup *pPickup);
	int FindPickup(WORD playerid, CPickup *pPickup);
	
	void Process(void);

	void SetStreamingEnabled(bool enabled);
	bool IsStreamingEnabled(void);
private:
	void ShowPickup(int pickupid, WORD playerid, CPickup *pPickup);
	void HidePickup(int pickupid, WORD playerid);

	PickupMap m_Pickups;
	std::bitset<4096> m_bPickupSlots;

	bool m_bStreamingEnabled;
};
#endif
#endif
