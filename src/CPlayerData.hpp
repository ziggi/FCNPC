/* =========================================

		FCNPC - Fully Controllable NPC
			----------------------

	- File: Player.hpp
	- Author(s): OrMisicL

  =========================================*/


#ifndef PLAYER_H
#define PLAYER_H

#include "CVector.h"
#include "Structs.h"
#include "CPlayback.hpp"
#include "CNode.hpp"
#include "CWeaponInfo.hpp"
#include <stdio.h>

class CPlayback;
struct CPlayer;
struct CVehicle;
struct CSyncData;
struct CVehicleSyncData;
struct CBulletSyncData;

class CPlayerData
{
public:
	CPlayerData(WORD playerId, char *szName);
	~CPlayerData();

	WORD GetId();

	void Destroy();
	bool Setup();
	void Process();
	bool Spawn(int iSkinId);
	bool Respawn();
	void SetSpawnPosition(CVector vecPosition);
	bool Kill(WORD wKillerId, BYTE byteReason);
	void Update(int iState);
	void UpdateSync(int iState);
	void ResetSyncMoving(int iState);
	void UpdateAim();
	bool IsSpawned();
	void SetSpawnedStatus(bool status);
	bool IsStreamedIn(WORD wForPlayerId);
	void ShowForPlayer(WORD wPlayerId);
	void ShowForStreamedPlayers();

	void SetOnFootSync(CSyncData *syncData);
	void SetVehicleSync(CVehicleSyncData *syncData);
	void SetBulletSync(CBulletSyncData *syncData);

	void SetName(char *szName);
	void GetName(char *szName, size_t size);

	bool SetState(BYTE byteState);
	BYTE GetState();

	void SetPosition(CVector vecPosition);
	void GetPosition(CVector *pvecPosition);
	void UpdateHeightPos(CVector *pvecPosition);
	void SetQuaternion(float *fQuaternion);
	void GetQuaternion(float *fQuaternion);
	void SetAngle(float fAngle);
	float GetAngle();
	void SetVelocity(CVector vecVelocity, bool bUpdatePos = false);
	void GetVelocity(CVector *pvecVelocity);
	void SetSpeed(float fSpeed);
	float GetSpeed();

	void SetHealth(float fHealth);
	float GetHealth();
	void SetArmour(float fArmour);
	float GetArmour();

	void SetInvulnerable(bool bInvulnerable);
	bool IsInvulnerable();

	void SetWeapon(BYTE byteWeaponId);
	BYTE GetWeapon();
	void SetAmmo(WORD wAmmo);
	WORD GetAmmo();
	void SetAmmoInClip(WORD wAmmo);
	WORD GetAmmoInClip();
	void SetWeaponSkill(DWORD dwSkill, WORD wLevel);
	WORD GetWeaponSkill(DWORD dwSkill);
	void SetWeaponState(int iState);
	int GetWeaponState();
	void UpdateWeaponState();

	int GetWeaponType(BYTE byteWeaponId);
	bool SetWeaponReloadTime(BYTE byteWeaponId, int iTime);
	int GetWeaponReloadTime(BYTE byteWeaponId);
	int GetWeaponActualReloadTime(BYTE byteWeaponId);
	bool SetWeaponShootTime(BYTE byteWeaponId, int iTime);
	int GetWeaponShootTime(BYTE byteWeaponId);
	bool SetWeaponClipSize(BYTE byteWeaponId, int iSize);
	int GetWeaponClipSize(BYTE byteWeaponId);
	int GetWeaponActualClipSize(BYTE byteWeaponId);
	bool IsWeaponDoubleHanded(BYTE byteWeaponId);
	bool SetWeaponAccuracy(BYTE byteWeaponId, float fAccuracy);
	float GetWeaponAccuracy(BYTE byteWeaponId);
	bool SetWeaponInfo(BYTE byteWeaponId, SWeaponInfo sWeaponInfo);
	SWeaponInfo GetWeaponInfo(BYTE byteWeaponId);

	void SetSkin(int iSkin);
	int GetSkin();
	void SetInterior(int iInterior);
	int GetInterior();
	void SetVirtualWorld(int iVirtualWorld);
	int GetVirtualWorld();

	void SetKeys(WORD wUDAnalog, WORD wLRAnalog, DWORD dwKeys);
	void GetKeys(WORD *pwUDAnalog, WORD *pwLRAnalog, DWORD *pdwKeys);

	void SetSpecialAction(BYTE byteActionId);
	BYTE GetSpecialAction();

	void SetFightingStyle(BYTE byteStyle);
	BYTE GetFightingStyle();

	void SetAnimation(WORD wAnimationId, float fDelta, bool bLoop, bool bLockX, bool bLockY, bool bFreeze, int iTime);
	void SetAnimationByName(char *szName, float fDelta, bool bLoop, bool bLockX, bool bLockY, bool bFreeze, int iTime);
	void ResetAnimation();
	void GetAnimation(WORD *wAnimationId, float *fDelta, bool *bLoop, bool *bLockX, bool *bLockY, bool *bFreeze, int *iTime);
	void ApplyAnimation(char *szAnimationLib, char *szAnimationName, float fDelta, bool bLoop, bool bLockX, bool bLockY, bool bFreeze, int iTime);
	void ClearAnimations();

	bool GoTo(CVector vecPoint, int iType, int iMode, float fRadius = 0.0f, bool bSetAngle = true, float fSpeed = -1.0f, float fDistOffset = 0.0f, DWORD dwStopDelay = 250);
	bool GoToPlayer(WORD wPlayerId, int iType, int iMode, float fRadius = 0.0f, bool bSetAngle = true, float fSpeed = -1.0f, float fDistOffset = 0.0f, float fDistCheck = 1.5f, DWORD dwStopDelay = 250);
	bool GoByMovePath(int iPathId, int iPointId, int iType, int iMode, float fRadius = 0.0f, bool bSetAngle = true, float fSpeed = -1.0f, float fDistOffset = 0.0f);
	void UpdateMovingData(CVector vecDestination, float fRadius, bool bSetAngle, float fSpeed, float fDistOffset);
	void GetDestination(CVector *pvecDestination);
	void StopMoving();
	bool IsMoving();
	bool IsMovingAtPlayer(WORD wPlayerId);
	bool IsMovingByMovePath(int iMovePath);
	int GetMovingType();

	void ToggleReloading(bool bToggle);
	void ToggleInfiniteAmmo(bool bToggle);

	void AimAt(CVector vecPoint, bool bShoot, int iShootDelay, bool bSetAngle, CVector vecOffsetFrom);
	void AimAtPlayer(WORD wHitId, bool bShoot, int iShootDelay, bool bSetAngle, CVector vecOffset, CVector vecOffsetFrom);
	void UpdateAimingData(CVector vecPoint, bool bSetAngle);
	void StopAim();
	bool MeleeAttack(int iTime, bool bUseFightstyle);
	void StopAttack();
	bool IsAttacking();
	bool IsAiming();
	bool IsAimingAtPlayer(WORD wPlayerId);
	WORD GetAimingPlayer();
	bool IsShooting();
	bool IsReloading();

	void ProcessDamage(WORD wDamagerId, float fHealthLoss, BYTE byteWeaponId, int iBodypart);
	void ProcessVehicleDamage(WORD wDamagerId, WORD wVehicleId, BYTE byteWeaponId, CVector vecHit);
	void ProcessStreamIn(WORD wForPlayerId);
	void ProcessStreamOut(WORD wForPlayerId);

	bool EnterVehicle(WORD wVehicleId, BYTE byteSeatId, int iType);
	bool ExitVehicle();
	bool PutInVehicle(WORD wVehicleId, BYTE byteSeatId);
	bool RemoveFromVehicle();
	bool IsInVehicle();
	WORD GetVehicleId();
	BYTE GetSeatId();
	void SetVehicle(WORD wVehicleId, BYTE byteSeatId);
	CVehicle *GetVehicle();
	void SetVehicleSiren(bool bState);
	bool IsVehicleSiren();
	void SetVehicleHealth(float fHealth);
	float GetVehicleHealth();
	void SetVehicleHydraThrusters(WORD wDirection);
	WORD GetVehicleHydraThrusters();
	void SetTrainSpeed(float fTrainSpeed);
	float GetTrainSpeed();
	void SetVehicleGearState(BYTE byteGearState);
	BYTE GetVehicleGearState();

	void SetSurfingOffsets(CVector vecOffsets);
	void GetSurfingOffsets(CVector *vecOffsets);
	void SetSurfingVehicle(WORD wVehicleId);
	int GetSurfingVehicle();
	void SetSurfingObject(WORD wObjectId);
	int GetSurfingObject();
	void SetSurfingPlayerObject(WORD wObjectId);
	int GetSurfingPlayerObject();
	void StopSurfing();

	bool StartPlayingPlayback(char *szFile, int iRecordId, bool bAutoUnload, CVector vecPoint, float *fQuaternion);
	void StopPlayingPlayback();
	void PausePlayingPlayback();
	void ResumePlayingPlayback();
	void SetPlayingPlaybackPath(char *szFile);
	void GetPlayingPlaybackPath(char *szFile, size_t size);

	bool PlayNode(int iNodeId, int iMoveType, int iMode, float fRadius, bool bSetAngle, float fSpeed);
	void StopPlayingNode();
	void PausePlayingNode();
	void ResumePlayingNode();
	bool IsPlayingNodePaused();
	bool IsPlayingNode();
	WORD ChangeNode(int iNodeId, WORD wLinkId);
	bool UpdateNodePoint(WORD wPointId);

	void SetMoveMode(int iMoveMode);
	int GetMoveMode();
	void SetMinHeightPosCall(float fHeight);
	float GetMinHeightPosCall();

	CPlayer *GetInterface();

private:
	WORD m_wPlayerId;
	bool m_bSetup;
	char m_szName[MAX_PLAYER_NAME];
	bool m_bSpawned;
	bool m_bMoving;
	float m_fMinHeightPos;
	bool m_bAiming;
	bool m_bReloading;
	bool m_bShooting;
	bool m_bEntering;
	bool m_bJacking;
	bool m_bExiting;
	bool m_bPlaying;
	bool m_bPlayingNode;
	bool m_bIsPlayingNodePaused;
	bool m_bMeleeAttack;
	bool m_bMeleeFightstyle;
	bool m_bIsInvulnerable;
	float m_fLastDistance;
	DWORD m_dwUpdateTick;
	DWORD m_dwMoveTickCount;
	DWORD m_dwReloadTickCount;
	DWORD m_dwShootTickCount;
	DWORD m_dwShootDelay;
	DWORD m_dwEnterExitTickCount;
	DWORD m_dwMoveStartTime;
	DWORD m_dwMoveTime;
	DWORD m_dwMoveStopDelay;
	DWORD m_dwMeleeDelay;
	DWORD m_dwKillVehicleTickCount;
	DWORD m_dwVehicleDeadTick;
	CVector m_vecDestination;
	BYTE m_byteWeaponId;
	WORD m_wAmmo;
	WORD m_wAmmoInClip;
	WORD m_wHitId;
	bool m_bAimSetAngle;
	BYTE m_byteHitType;
	WORD m_wVehicleToEnter;
	BYTE m_byteSeatToEnter;
	WORD m_wLastDamagerId;
	BYTE m_byteLastDamagerWeapon;
	WORD m_wNodePoint;
	WORD m_wNodeLastPoint;
	bool m_bHasReload;
	bool m_bHasInfiniteAmmo;
	CPlayback *m_pPlayback;
	CNode *m_pNode;
	CVector m_vecAimAt;
	CVector m_vecAimOffset;
	CVector m_vecAimOffsetFrom;
	CPlayer *m_pPlayer;
	CVector m_vecSurfing;
	WORD m_wSurfingInfo;
	CWeaponInfo *m_pWeaponInfo;
	WORD m_wMoveId;
	int m_iMovePath;
	int m_iMovePoint;
	int m_iMoveType;
	int m_iMoveMode;
	float m_fMoveRadius;
	bool m_bMoveSetAngle;
	float m_fMoveSpeed;
	float m_fDistOffset;
	int m_iNodeMoveType;
	int m_iNodeMoveMode;
	float m_fNodeMoveRadius;
	bool m_bNodeMoveSetAngle;
	float m_fNodeMoveSpeed;
	CVector m_vecNodeLastPos;
	CVector m_vecMovePlayerPosition;
	float m_fDistCheck;
	WORD m_wHydraThrustAngle[2];
	float m_fTrainSpeed;
	BYTE m_byteGearState;
	bool m_bVelocityUpdatePos;
	char m_szPlayingPath[MAX_PATH];
};

#endif
