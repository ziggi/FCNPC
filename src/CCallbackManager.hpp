/* =========================================

		FCNPC - Fully Controllable NPC
			----------------------

	- File: CallbackManager.hpp
	- Author(s): OrMisicL

  =========================================*/


#ifndef CALLBACKMANAGER_H
#define CALLBACKMANAGER_H

class CCallbackManager
{
public:

	static void RegisterAMX(AMX *pAMX);
	static void UnregisterAMX(AMX *pAMX);
	// Callbacks
	static void OnCreate(WORD wPlayerId);
	static void OnDestroy(WORD wPlayerId);
	static void OnSpawn(WORD wPlayerId);
	static void OnRespawn(WORD wPlayerId);
	static void OnDeath(WORD wPlayerId, WORD wKillerId, BYTE byteWeaponId);
	static void OnReachDestination(WORD wPlayerId);
	static void OnVehicleEntryComplete(WORD wPlayerId, WORD wVehicleId, int iSeat);
	static void OnVehicleExitComplete(WORD wPlayerId);
	static int  OnTakeDamage(WORD wPlayerId, WORD wDamagerId, BYTE byteWeaponId, int iBodyPart, float fHealthLoss);
	static void OnGiveDamage(WORD wPlayerId, WORD wDamagedId, BYTE byteWeaponId, int iBodyPart, float fHealthLoss);
	static int  OnWeaponShot(WORD wPlayerId, WORD wHitId, BYTE byteHitType, BYTE byteWeaponId, CVector vecPoint);
	static void OnWeaponStateChange(WORD wPlayerId, int iWeaponState);
	static int  OnVehicleTakeDamage(WORD wPlayerId, WORD wDamagerId, WORD wVehicleId, BYTE byteWeaponId, CVector vecHit);
	static void OnFinishPlayback(WORD wPlayerId);
	static int  OnChangeNode(WORD wPlayerId, WORD wNodeId);
	static int  OnFinishNodePoint(WORD wPlayerId, WORD wNodePoint);
	static void OnFinishNode(WORD wPlayerId);
	static void OnStreamIn(WORD wPlayerId, WORD wForPlayerId);
	static void OnStreamOut(WORD wPlayerId, WORD wForPlayerId);
	static int  OnUpdate(WORD wPlayerId);
	static void OnFinishMovePath(WORD wPlayerId, int iMovePath);
	static void OnFinishMovePathPoint(WORD wPlayerId, int iMovePath, int iMovePoint);
	static int OnChangeHeightPos(WORD wPlayerId, float fNewZ, float fOldZ);

	static std::vector<AMX *> m_vAMX;
};

#endif
