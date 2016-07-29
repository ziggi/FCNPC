/* =========================================

		FCNPC - Fully Controllable NPC
			----------------------

	- File: CallbackManager.hpp
	- Author(s): OrMisicL

  =========================================*/


#ifndef CALLBACKMANAGER_H
#define CALLBACKMANAGER_H

#include <list>

class CCallbackManager
{
public:

	static void RegisterAMX(AMX *pAMX);
	static void UnregisterAMX(AMX *pAMX);
	// Callbacks
	static void OnCreate(WORD wPlayerId);
	static void OnSpawn(WORD wPlayerId);
	static void OnRespawn(WORD wPlayerId);
	static void OnDeath(WORD wPlayerId, WORD wKillerId, BYTE byteWeaponId);
	static void OnReachDestination(WORD wPlayerId);
	static void OnVehicleEntryComplete(WORD wPlayerId, WORD wVehicleId, int iSeat);
	static void OnVehicleExitComplete(WORD wPlayerId);
	static int  OnTakeDamage(WORD wPlayerId, WORD wDamagerId, BYTE byteWeaponId, int iBodyPart, float fHealthLoss);
	static int  OnVehicleTakeDamage(WORD wPlayerId, WORD wDamagerId, WORD wVehicleId, BYTE byteWeaponId, CVector vecHit);
	static void OnFinishPlayback(WORD wPlayerId);
	static int  OnChangeNode(WORD wPlayerId, int iNodeId);
	static int  OnFinishNodePoint(WORD wPlayerId, int iNodePoint);
	static void OnFinishNode(WORD wPlayerId);
	static void OnStreamIn(WORD wPlayerId, WORD wForPlayerId);
	static void OnStreamOut(WORD wPlayerId, WORD wForPlayerId);

	static std::list<AMX *> m_listAMX;
};

#endif
