/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: Player.h
	- Author(s): OrMisicL

  =========================================*/


#ifndef PLAYER_H
#define PLAYER_H

#include "Common.h"
#include "SAMPPlayer.h"
#include "Playback.h"
#include "Node.h"
#include <stdio.h>

class CPlayer
{
	public:
		CPlayer(EntityId playerId, char *szName);
		~CPlayer();

		int			GetId() { return (int)m_playerId; };

		void		Destroy();
		bool		Setup();
		void		Process();
		bool		Spawn(int iSkinId);
		bool		Respawn();
		void		Kill(int iKillerId, int iWeapon);
		void		Update(int iState);
		void		UpdateSync(int iState);
		void		UpdateAim();
		bool		IsSpawned() { return m_bSpawned; };
		void		SetSpawnPosition(CVector3 vecPosition);

		void		SetOnFootSync(CSyncData syncData) { memcpy(&m_pInterface->syncData, &syncData, sizeof(CSyncData)); };
		void		SetVehicleSync(CVehicleSyncData syncData) { memcpy(&m_pInterface->vehicleSyncData, &syncData, sizeof(CVehicleSyncData)); };

		void		GetName(char *szName) { strcpy(szName, m_szName); };
		void		SetName(char *szName);

		bool		SetState(BYTE byteState);
		BYTE		GetState() { return m_pInterface->byteState; };

		void		SetPosition(CVector3 vecPosition);
		void		GetPosition(CVector3 *pvecPosition);
		void		SetQuaternion(CVector3 vecQuaternion, float fAngle);
		void		GetQuaternion(CVector3 *pvecQuaternion, float *pfAngle);
		void		SetAngle(float fAngle);
		float		GetAngle() { return m_pInterface->fAngle; };
		void		SetVelocity(CVector3 vecVelocity);
		void		GetVelocity(CVector3 *pvecVelocity);

		void		SetHealth(float fHealth);
		float		GetHealth() { return m_pInterface->fHealth; };
		void		SetArmour(float fArmour);
		float		GetArmour() { return m_pInterface->fArmour; };

		void		SetWeapon(BYTE byteWeaponId);
		BYTE		GetWeapon() { return m_byteWeaponId; };
		void		SetAmmo(WORD wAmmo);
		WORD		GetAmmo() { return m_wAmmo; };

		void		SetSkin(int iSkin);
		int			GetSkin() { return m_pInterface->iSkinId; };
		void		SetInterior(int iInterior);
		int			GetInterior() { return m_pInterface->iInteriorId; };

		void		SetKeys(WORD wUDAnalog, WORD wLRAnalog, DWORD dwKeys);
		void		GetKeys(WORD *pwUDAnalog, WORD *pwLRAnalog, DWORD *pdwKeys);

		void		SetSpecialAction(int iActionId);
		int			GetSpecialAction() { return m_pInterface->syncData.byteSpecialAction; };

		void		GoTo(CVector3 vecPoint, int iType, bool bUseZMap);
		void		StopMoving();
		bool		IsMoving() { return m_bMoving; };

		void		AimAt(CVector3 vecPoint, bool bShoot);
		void		StopAim();
		void		MeleeAttack(DWORD dwTime);
		void		StopAttack();
		bool		IsAiming() { return m_bAiming; };
		bool		IsShooting() { return (m_bAiming && m_pInterface->dwKeys == 4) ? true : false; };
		bool		IsReloading() { return m_bReloading; };


		void		ProcessDamage(int iDamagerId, float fHealthLoss, int iWeaponId, int iBodypart);
		
		bool		EnterVehicle(int iVehicleId, int iSeatId, int iType);
		bool		ExitVehicle();
		bool		PutInVehicle(int iVehicleId, int iSeatId);
		bool		RemoveFromVehicle();
		bool		IsInVehicle() { return m_pInterface->wVehicleId == INVALID_ENTITY_ID ? false : true; };
		int			GetVehicleId() { return m_pInterface->wVehicleId; };
		int			GetSeatId() { return m_pInterface->byteSeatId; };

		bool		StartRecordingPlayback(char *szFile);
		void		StopRecordingPlayback();
		void		PauseRecordingPlayback();
		void		ResumeRecordingPlayback();

		bool		PlayNode(int iNodeId, int iType);
		void		StopPlayingNode();
		int			ChangeNode(int iNodeId, unsigned short usLinkId);

	private:
		EntityId		m_playerId;
		bool			m_bSetup;
		char			m_szName[MAX_NAME_LENGTH];
		bool			m_bSpawned;
		bool			m_bMoving;
		bool			m_bUseZMap;
		bool			m_bAiming;
		bool			m_bReloading;
		bool			m_bEntering;
		bool			m_bJacking;
		bool			m_bExiting;
		bool			m_bRecording;
		bool			m_bPlayingNode;
		bool			m_bMeleeAttack;
		float			m_fLastDistance;
		DWORD			m_dwUpdateTick;
		DWORD			m_dwMoveTickCount;
		DWORD			m_dwReloadTickCount;
		DWORD			m_dwShootTickCount;
		DWORD			m_dwEnterExitTickCount;
		DWORD			m_dwMoveStartTime;
		DWORD			m_dwMoveTime;
		DWORD			m_dwMeleeDelay;
		CVector3		m_vecDestination;
		BYTE			m_byteWeaponId;
		WORD			m_wAmmo;
		WORD			m_wVehicleToEnter;
		BYTE			m_byteSeatToEnter;
		int				m_iLastDamager;
		int				m_iNodePoint;
		int				m_iNodeLastPoint;
		int				m_iNodeType;
		CVector3		m_vecNodeVelocity;
		CPlayback		*m_pPlayback;
		CNode			*m_pNode;
		CSAMPPlayer		*m_pInterface;

};

#endif