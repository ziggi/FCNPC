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
#include "Playback.hpp"
#include "Node.hpp"
#include "WeaponInfo.hpp"
#include <stdio.h>

class CPlayback;
struct CSyncData;
struct CVehicle;
struct CVehicleSyncData;
struct CPlayer;

class CPlayerData
{
public:
	CPlayerData(WORD playerId, char *szName);
	~CPlayerData();

	int GetId();

	void Destroy();
	bool Setup();
	void Process();
	bool Spawn(int iSkinId);
	bool Respawn();
	void SetSpawnPosition(CVector vecPosition);
	void Kill(WORD wKillerId, BYTE byteReason);
	void Update(int iState);
	void UpdateSync(int iState);
	void UpdateAim();
	bool IsSpawned();
	bool IsStreamedIn(int iForPlayerId);

	void SetOnFootSync(CSyncData *syncData);
	void SetVehicleSync(CVehicleSyncData *syncData);

	void SetName(char *szName);
	void GetName(char *szName, size_t size);

	bool SetState(BYTE byteState);
	BYTE GetState();

	void SetPosition(CVector vecPosition);
	void GetPosition(CVector *pvecPosition);
	void SetQuaternion(float *fQuaternion);
	void GetQuaternion(float *fQuaternion);
	void SetAngle(float fAngle);
	float GetAngle();
	void SetVelocity(CVector vecVelocity);
	void GetVelocity(CVector *pvecVelocity);

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
	void SetWeaponSkill(int iSkill, int iLevel);
	WORD GetWeaponSkill(int iSkill);
	void SetWeaponState(int iState);
	WORD GetWeaponState();

	int GetWeaponType(int iWeaponId);
	bool SetWeaponReloadTime(int iWeaponId, int iTime);
	int GetWeaponReloadTime(int iWeaponId);
	bool SetWeaponShootTime(int iWeaponId, int iTime);
	int GetWeaponShootTime(int iWeaponId);
	bool SetWeaponClipSize(int iWeaponId, int iSize);
	int GetWeaponClipSize(int iWeaponId);
	bool SetWeaponInfo(int iWeaponId, SWeaponInfo sWeaponInfo);
	SWeaponInfo GetWeaponInfo(int iWeaponId);

	void SetSkin(int iSkin);
	int GetSkin();
	void SetInterior(int iInterior);
	int GetInterior();
	void SetVirtualWorld(int iVirtualWorld);
	int GetVirtualWorld();

	void SetKeys(WORD wUDAnalog, WORD wLRAnalog, DWORD dwKeys);
	void GetKeys(WORD *pwUDAnalog, WORD *pwLRAnalog, DWORD *pdwKeys);

	void SetSpecialAction(BYTE byteActionId);
	int GetSpecialAction();

	void SetFightingStyle(int iStyle);
	int GetFightingStyle();

	void SetAnimation(int iAnimationId, float fDelta, bool bLoop, bool bLockX, bool bLockY, bool bFreeze, int iTime);
	void SetAnimationByName(char *szName, float fDelta, bool bLoop, bool bLockX, bool bLockY, bool bFreeze, int iTime);
	void ResetAnimation();
	void GetAnimation(int *iAnimationId, float *fDelta, bool *bLoop, bool *bLockX, bool *bLockY, bool *bFreeze, int *iTime);
	void ApplyAnimation(char *szAnimationLib, char *szAnimationName, float fDelta, bool bLoop, bool bLockX, bool bLockY, bool bFreeze, int iTime);
	void ClearAnimations();

	bool GoTo(CVector vecPoint, int iType, bool bUseMapAndreas, float fRadius = 0.0f, bool bSetAngle = true, float fSpeed = -1.0f);
	bool GoToPlayer(WORD wPlayerId, int iType, bool bUseMapAndreas, float fRadius = 0.0f, bool bSetAngle = true, float fSpeed = -1.0f);
	void UpdateMovingData(CVector vecDestination, bool bSetAngle, float fSpeed);
	void StopMoving();
	bool IsMoving();
	bool IsMovingAtPlayer(WORD wPlayerId);

	void ToggleReloading(bool bToggle);
	void ToggleInfiniteAmmo(bool bToggle);

	void AimAt(CVector vecPoint, bool bShoot, int iShootDelay, bool bSetAngle);
	void AimAtPlayer(WORD wHitId, bool bShoot, int iShootDelay, bool bSetAngle);
	void UpdateAimingData(CVector vecPoint, bool bSetAngle);
	void StopAim();
	bool MeleeAttack(int iTime, bool bUseFightstyle);
	void StopAttack();
	bool IsAttacking();
	bool IsAiming();
	bool IsAimingAtPlayer(WORD wPlayerId);
	bool IsShooting();
	bool IsReloading();

	void ProcessDamage(int iDamagerId, float fHealthLoss, int iWeaponId, int iBodypart);
	void ProcessVehicleDamage(int iDamagerId, int iVehicleId, int iWeaponId, CVector vecHit);
	void ProcessStreamIn(int iForPlayerId);
	void ProcessStreamOut(int iForPlayerId);

	bool EnterVehicle(int iVehicleId, int iSeatId, int iType);
	bool ExitVehicle();
	bool PutInVehicle(int iVehicleId, int iSeatId);
	bool RemoveFromVehicle();
	bool IsInVehicle();
	int GetVehicleId();
	int GetSeatId();
	void SetVehicle(WORD wVehicleId, BYTE byteSeatId);
	CVehicle *GetVehicle();
	void SetVehicleSiren(bool bState);
	bool IsVehicleSiren();
	void SetVehicleHealth(float fHealth);
	float GetVehicleHealth();

	void SetSurfingOffsets(CVector vecOffsets);
	void GetSurfingOffsets(CVector *vecOffsets);
	void SetSurfingVehicle(int iVehicleId);
	int GetSurfingVehicle();
	void SetSurfingObject(int iObjectId);
	int GetSurfingObject();
	void SetSurfingPlayerObject(int iObjectId);
	int GetSurfingPlayerObject();
	void StopSurfing();

	bool StartPlayingPlayback(char *szFile);
	void StopPlayingPlayback();
	void PausePlayingPlayback();
	void ResumePlayingPlayback();

	bool PlayNode(int iNodeId, int iType);
	void StopPlayingNode();
	int ChangeNode(int iNodeId, unsigned short usLinkId);

private:
	WORD m_playerId;
	bool m_bSetup;
	char m_szName[MAX_PLAYER_NAME];
	bool m_bSpawned;
	bool m_bMoving;
	bool m_bUseMapAndreas;
	bool m_bAiming;
	bool m_bReloading;
	bool m_bShooting;
	bool m_bEntering;
	bool m_bJacking;
	bool m_bExiting;
	bool m_bPlaying;
	bool m_bPlayingNode;
	bool m_bMeleeAttack;
	bool m_bMeleeFightstyle;
	bool m_bIsInvulnerable;
	float m_fLastDistance;
	DWORD m_dwUpdateTick;
	DWORD m_dwMoveTickCount;
	DWORD m_dwReloadTickCount;
	DWORD m_dwShootTickCount;
	int m_iShootDelay;
	DWORD m_dwEnterExitTickCount;
	DWORD m_dwMoveStartTime;
	DWORD m_dwMoveTime;
	DWORD m_dwMeleeDelay;
	DWORD m_dwKillVehicleTickCount;
	DWORD m_dwVehicleDeadTick;
	CVector m_vecDestination;
	BYTE m_byteWeaponId;
	WORD m_wAmmo;
	WORD m_wHitId;
	bool m_bAimSetAngle;
	BYTE m_byteHitType;
	WORD m_wVehicleToEnter;
	BYTE m_byteSeatToEnter;
	int m_iLastDamager;
	int m_iNodePoint;
	int m_iNodeLastPoint;
	int m_iNodeType;
	bool m_bHasReload;
	bool m_bHasInfiniteAmmo;
	CVector m_vecNodeVelocity;
	CPlayback *m_pPlayback;
	CNode *m_pNode;
	CVector m_vecAimAt;
	CPlayer *m_pPlayer;
	CVector m_vecSurfing;
	WORD m_wSurfingInfo;
	CWeaponInfo *m_pWeaponInfo;
	WORD m_wMoveId;
	int m_iMoveType;
	float m_fMoveRadius;
	bool m_bMoveSetAngle;
	float m_fMoveSpeed;
};

#endif
