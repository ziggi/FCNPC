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

	static void Init();
	static void RegisterAMX(AMX *pAmx);
	static void UnregisterAMX(AMX *pAmx);
	// Callbacks
	static void OnCreate(WORD wPlayerId);
	static void OnDestroy(WORD wPlayerId);
	static void OnSpawn(WORD wPlayerId);
	static void OnRespawn(WORD wPlayerId);
	static void OnDeath(WORD wPlayerId, WORD wKillerId, BYTE byteWeaponId);
	static void OnReachDestination(WORD wPlayerId);
	static void OnVehicleEntryComplete(WORD wPlayerId, WORD wVehicleId, int iSeat);
	static void OnVehicleExitComplete(WORD wPlayerId, WORD wVehicleId);
	static int  OnTakeDamage(WORD wPlayerId, WORD wDamagerId, BYTE byteWeaponId, int iBodyPart, float fHealthLoss);
	static void OnGiveDamage(WORD wPlayerId, WORD wDamagedId, BYTE byteWeaponId, int iBodyPart, float fHealthLoss);
	static int  OnWeaponShot(WORD wPlayerId, BYTE byteWeaponId, BYTE byteHitType, WORD wHitId, CVector vecPoint);
	static void OnWeaponStateChange(WORD wPlayerId, int iWeaponState);
	static int  OnVehicleTakeDamage(WORD wPlayerId, WORD wDamagerId, WORD wVehicleId, BYTE byteWeaponId, CVector vecHit, float fHealthLoss);
	static void OnFinishPlayback(WORD wPlayerId);
	static int  OnChangeNode(WORD wPlayerId, WORD wNodeId, WORD wOldNodeId);
	static int  OnFinishNodePoint(WORD wPlayerId, WORD wNodeId, WORD wNodePoint);
	static void OnFinishNode(WORD wPlayerId, WORD wNodeId);
	static void OnStreamIn(WORD wPlayerId, WORD wForPlayerId);
	static void OnStreamOut(WORD wPlayerId, WORD wForPlayerId);
	static int  OnUpdate(WORD wPlayerId);
	static void OnFinishMovePath(WORD wPlayerId, int iMovePath);
	static int OnFinishMovePathPoint(WORD wPlayerId, int iMovePath, int iMovePoint);
	static int OnChangeHeightPos(WORD wPlayerId, float fNewZ, float fOldZ);

	static std::vector<AMX *> m_vAmx;
	static std::queue<AMX *> m_vAmxLoadQueue;

	enum Callbacks {
		FCNPC_OnCreate,
		FCNPC_OnDestroy,
		FCNPC_OnSpawn,
		FCNPC_OnRespawn,
		FCNPC_OnDeath,
		FCNPC_OnReachDestination,
		FCNPC_OnVehicleEntryComplete,
		FCNPC_OnVehicleExitComplete,
		FCNPC_OnTakeDamage,
		FCNPC_OnGiveDamage,
		FCNPC_OnWeaponShot,
		FCNPC_OnWeaponStateChange,
		FCNPC_OnVehicleTakeDamage,
		FCNPC_OnFinishPlayback,
		FCNPC_OnChangeNode,
		FCNPC_OnFinishNodePoint,
		FCNPC_OnFinishNode,
		FCNPC_OnStreamIn,
		FCNPC_OnStreamOut,
		FCNPC_OnUpdate,
		FCNPC_OnFinishMovePath,
		FCNPC_OnFinishMovePathPoint,
		FCNPC_OnChangeHeightPos,
		CallbacksCount,
	};

	static std::map<AMX *, std::vector<int>> m_mapCallbacks;
	static std::array<char *, Callbacks::CallbacksCount> m_aCallbackNames;
};

#endif
