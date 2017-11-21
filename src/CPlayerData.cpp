/* =========================================

		FCNPC - Fully Controllable NPC
			----------------------

	- File: Player.cpp
	- Author(s): OrMisicL

  =========================================*/


#include "Main.hpp"

struct CPlayer;

extern CServer  *pServer;
extern CNetGame *pNetGame;

CPlayerData::CPlayerData(WORD playerId, char *szName)
{
	// Save the player id
	m_wPlayerId = playerId;
	// Save player name
	SetName(szName);
	// Reset variables
	m_vecDestination = CVector();
	m_vecAimAt = CVector();
	m_vecAimOffset = CVector();
	m_vecAimOffsetFrom = CVector();
	m_bSetup = false;
	m_bSpawned = false;
	m_bMoving = false;
	m_fMinHeightPos = -1.0f;
	m_bAiming = false;
	m_bReloading = false;
	m_bShooting = false;
	m_bEntering = false;
	m_bJacking = false;
	m_bExiting = false;
	m_bHasReload = true;
	m_bHasInfiniteAmmo = false;
	m_bPlaying = false;
	m_bPlayingNode = false;
	m_bIsPlayingNodePaused = false;
	m_bMeleeAttack = false;
	m_bMeleeFightstyle = false;
	m_bIsInvulnerable = false;
	m_byteWeaponId = 0;
	m_wAmmo = 0;
	m_wAmmoInClip = 0;
	m_wNodePoint = 0;
	m_wNodeLastPoint = 0;
	m_wLastDamagerId = INVALID_PLAYER_ID;
	m_byteLastDamagerWeapon = 255;
	m_wVehicleToEnter = INVALID_VEHICLE_ID;
	m_byteSeatToEnter = 0;
	m_wHitId = INVALID_PLAYER_ID;
	m_byteHitType = BULLET_HIT_TYPE_NONE;
	m_vecSurfing = CVector();
	m_wSurfingInfo = 0;
	m_pWeaponInfo = new CWeaponInfo();
	m_wMoveId = INVALID_PLAYER_ID;
	m_iMovePath = INVALID_MOVEPATH_ID;
	m_iMovePoint = 0;
	m_iMoveType = MOVE_TYPE_AUTO;
	m_iMoveMode = MOVE_MODE_AUTO;
	m_fMoveRadius = 0.0f;
	m_bMoveSetAngle = false;
	m_fMoveSpeed = MOVE_SPEED_AUTO;
	m_dwMoveStopDelay = 0;
	m_fDistOffset = 0.0f;
	m_iNodeMoveType = MOVE_TYPE_AUTO;
	m_iNodeMoveMode = MOVE_MODE_AUTO;
	m_fNodeMoveRadius = 0.0f;
	m_bNodeMoveSetAngle = false;
	m_fNodeMoveSpeed = MOVE_SPEED_AUTO;
	m_vecNodeLastPos = CVector();
	m_vecMovePlayerPosition = CVector();
	m_fDistCheck = 0.0f;
	m_wHydraThrustAngle[0] =
		m_wHydraThrustAngle[1] = 5000;
	m_fTrainSpeed = 0.0f;
	m_byteGearState = 0;
	m_bVelocityUpdatePos = false;
	SetPlayingPlaybackPath((char *)"npcmodes/recordings/");
}

CPlayerData::~CPlayerData()
{
	SAFE_DELETE(m_pWeaponInfo);
}

WORD CPlayerData::GetId()
{
	return m_wPlayerId;
}

void CPlayerData::Destroy()
{
	// Stop Playing any playback
	if (m_bPlaying) {
		StopPlayingPlayback();
	}

	// Stop playing any node
	if (m_bPlayingNode) {
		StopPlayingNode();
	}

	// Set the player state to none
	SetState(PLAYER_STATE_NONE);
	// Remove the player from the SAMP playerpool
	CFunctions::DeletePlayer(m_wPlayerId);
	// Reset the setup flag
	m_bSetup = false;
}

bool CPlayerData::Setup()
{
	if (m_wPlayerId == INVALID_PLAYER_ID) {
		return false;
	}

	// Get the player interface
	m_pPlayer = pNetGame->pPlayerPool->pPlayer[m_wPlayerId];
	// Validate the interface
	if (!m_pPlayer) {
		return false;
	}

	// Set the setup flag
	m_bSetup = true;

	// Reset his data
	SetState(PLAYER_STATE_NONE);
	m_pPlayer->spawn.byteTeam = NO_TEAM;

	// Initialize the update tick
	m_dwUpdateTick = GetTickCount();
	return true;
}

bool CPlayerData::Spawn(int iSkinId)
{
	// Make sure the player is not already spawned
	if (m_bSpawned) {
		return false;
	}

	// Mark spawned
	m_bSpawned = true;
	m_pPlayer->bReadyToSpawn = true;
	// Set the player skin
	m_pPlayer->spawn.iSkin = iSkinId;
	// Call the SAMP spawn functions
	CFunctions::SpawnPlayer(m_pPlayer);
	// Set the player stats
	SetHealth(100.0f);
	SetArmour(0.0f);
	SetSpecialAction(SPECIAL_ACTION_NONE);
	SetWeapon(0);
	SetAmmo(0);
	// Set the player state onfoot
	SetState(PLAYER_STATE_ONFOOT);
	// Reset stats
	m_wLastDamagerId = INVALID_PLAYER_ID;
	m_byteLastDamagerWeapon = 255;
	SetVehicle(INVALID_VEHICLE_ID, 0);
	// Call the NPC spawn callback
	CCallbackManager::OnSpawn(m_wPlayerId);
	return true;
}

bool CPlayerData::Respawn()
{
	// Make sure the player is already spawned
	if (!m_bSpawned) {
		return false;
	}

	// Pause Playing any playback
	if (m_bPlaying) {
		m_pPlayback->SetPaused(true);
	}

	// Get the last player stats
	CVector vecPosition = m_pPlayer->vecPosition;
	float fAngle = m_pPlayer->fAngle;
	float fHealth = m_pPlayer->fHealth;
	float fArmour = m_pPlayer->fArmour;
	WORD wVehicleId = m_pPlayer->wVehicleId;
	BYTE byteSeat = m_pPlayer->byteSeatId;
	BYTE byteSpecialAction = m_pPlayer->syncData.byteSpecialAction;
	// Call the SAMP spawn functions
	m_pPlayer->bReadyToSpawn = true;
	CFunctions::SpawnPlayer(m_pPlayer);
	// Set the player state onfoot
	SetState(PLAYER_STATE_ONFOOT);
	// Reset vehicle and seat id
	SetVehicle(INVALID_VEHICLE_ID, 0);
	// Set the player stats back
	if (fHealth == 0.0f) {
		SetHealth(100.0f);
		SetArmour(0.0f);
	} else {
		SetHealth(fHealth);
		SetArmour(fArmour);
	}
	SetPosition(vecPosition);
	SetAngle(fAngle);
	SetSpecialAction(byteSpecialAction);
	if (wVehicleId != INVALID_VEHICLE_ID) {
		PutInVehicle(wVehicleId, byteSeat);
	}

	// Resume any playback
	if (m_bPlaying) {
		m_pPlayback->SetPaused(false);
	}

	// Reset stats
	m_wLastDamagerId = INVALID_PLAYER_ID;
	m_byteLastDamagerWeapon = 255;
	// Call the NPC spawn callback
	CCallbackManager::OnRespawn(m_wPlayerId);
	return true;
}

void CPlayerData::SetSpawnPosition(CVector vecPosition)
{
	// Set the player position
	m_pPlayer->spawn.vecPos = vecPosition;
}

void CPlayerData::SetOnFootSync(CSyncData *syncData)
{
	memcpy(&m_pPlayer->syncData, syncData, sizeof(CSyncData));
}

void CPlayerData::SetVehicleSync(CVehicleSyncData *syncData)
{
	memcpy(&m_pPlayer->vehicleSyncData, syncData, sizeof(CVehicleSyncData));
}

void CPlayerData::SetBulletSync(CBulletSyncData *syncData)
{
	memcpy(&m_pPlayer->bulletSyncData, syncData, sizeof(CBulletSyncData));
}

void CPlayerData::GetName(char *szName, size_t size)
{
	strlcpy(szName, m_szName, size);
}

void CPlayerData::SetName(char *szName)
{
	strlcpy(m_szName, szName, sizeof(m_szName));
}

void CPlayerData::UpdateSync(int iState)
{
	// Make sure the state is valid
	if (iState < UPDATE_STATE_NONE || iState > UPDATE_STATE_PASSENGER) {
		return;
	}

	// Set the new update state
	m_pPlayer->iUpdateState = iState;
}

void CPlayerData::Update(int iState)
{
	// Validate the player
	if (!m_bSetup || !m_bSpawned) {
		return;
	}

	// Make sure the state is valid
	if (iState < UPDATE_STATE_NONE || iState > UPDATE_STATE_PASSENGER) {
		return;
	}

	// Check the time spent since the last update
	DWORD dwThisTick = GetTickCount();

	if ((dwThisTick - m_dwUpdateTick) < pServer->GetUpdateRate()) {
		return;
	}

	BYTE byteState = GetState();

	// Update onfoot state
	if (iState == UPDATE_STATE_ONFOOT && (byteState == PLAYER_STATE_ONFOOT || byteState == PLAYER_STATE_ENTER_VEHICLE_DRIVER || byteState == PLAYER_STATE_ENTER_VEHICLE_PASSENGER)) {
		// Set the sync position
		m_pPlayer->syncData.vecPosition = m_pPlayer->vecPosition;
		// Set the sync velocity vector
		m_pPlayer->syncData.vecVelocity = m_pPlayer->vecVelocity;
		// Set the sync surfing vector
		m_pPlayer->syncData.vecSurfing = m_vecSurfing;
		// Set the sync surfing information
		m_pPlayer->syncData.wSurfingInfo = m_wSurfingInfo;
		// Set the sync weapon index
		m_pPlayer->syncData.byteWeapon = m_byteWeaponId;
		// Set the sync health and armour
		m_pPlayer->syncData.byteHealth = static_cast<BYTE>(m_pPlayer->fHealth);
		m_pPlayer->syncData.byteArmour = static_cast<BYTE>(m_pPlayer->fArmour);
		// Set the sync keys1
		m_pPlayer->syncData.wUDAnalog = m_pPlayer->wUDAnalog;
		m_pPlayer->syncData.wLRAnalog = m_pPlayer->wLRAnalog;
		m_pPlayer->syncData.wKeys = static_cast<WORD>(m_pPlayer->dwKeys);
		// Update aim data
		UpdateAim();
		// Set the new update state
		m_pPlayer->iUpdateState = iState;
	}
	// Update driver state
	else if (iState == UPDATE_STATE_DRIVER && byteState == PLAYER_STATE_DRIVER) {
		// Dont process if we dont have any vehicle
		CVehicle *pVehicle = GetVehicle();
		if (!pVehicle) {
			return;
		}

		m_pPlayer->vehicleSyncData.wVehicleId = m_pPlayer->wVehicleId;
		m_pPlayer->vehicleSyncData.vecPosition = pVehicle->vecPosition;
		m_pPlayer->vehicleSyncData.vecVelocity = m_pPlayer->vecVelocity;
		m_pPlayer->vehicleSyncData.wUDAnalog = m_pPlayer->wUDAnalog;
		m_pPlayer->vehicleSyncData.wLRAnalog = m_pPlayer->wLRAnalog;
		m_pPlayer->vehicleSyncData.wKeys = static_cast<WORD>(m_pPlayer->dwKeys);
		// Set the vehicle occupied data
		if (pVehicle->bOccupied) {
			m_pPlayer->vehicleSyncData.fHealth = pVehicle->fHealth;
			pVehicle->vehOccupiedTick = dwThisTick;
		}

		if (pVehicle->fHealth < 250.0f) {
			if (m_dwVehicleDeadTick == 0) {
				m_dwVehicleDeadTick = GetTickCount();
			} else {
				if (!pVehicle->bDead && (dwThisTick - m_dwVehicleDeadTick) > 5000) {
					Kill(pVehicle->wKillerID, 255);

					pVehicle->bDead = true;
					m_dwVehicleDeadTick = 0;
					SetVehicle(INVALID_VEHICLE_ID, 0);
				}
			}
		}

		m_pPlayer->vehicleSyncData.bytePlayerHealth = static_cast<BYTE>(m_pPlayer->fHealth);
		m_pPlayer->vehicleSyncData.bytePlayerArmour = static_cast<BYTE>(m_pPlayer->fArmour);
		m_pPlayer->vehicleSyncData.bytePlayerWeapon = m_byteWeaponId;
		m_pPlayer->vehicleSyncData.byteGearState = m_byteGearState;

		if (CVehicleInfo::IsAHydra(static_cast<WORD>(pVehicle->customSpawn.dwModelID))) {
			m_pPlayer->vehicleSyncData.wHydraReactorAngle[0] = m_wHydraThrustAngle[0];
			m_pPlayer->vehicleSyncData.wHydraReactorAngle[1] = m_wHydraThrustAngle[1];
		} else if (CVehicleInfo::IsATrainPart(static_cast<WORD>(pVehicle->customSpawn.dwModelID))) {
			m_pPlayer->vehicleSyncData.fTrainSpeed = m_fTrainSpeed;
		}

		// Set the new update state
		m_pPlayer->iUpdateState = iState;
	}
	// Update passenger state
	else if (iState == UPDATE_STATE_PASSENGER && byteState == PLAYER_STATE_PASSENGER) {
		// Dont process if we dont have any vehicle
		CVehicle *pVehicle = GetVehicle();
		if (!pVehicle) {
			return;
		}

		// Set the player position to the vehicle position
		SetPosition(pVehicle->vecPosition);
		// Set the player sync vehicle and seat id
		m_pPlayer->passengerSyncData.wVehicleId = m_pPlayer->wVehicleId;
		m_pPlayer->passengerSyncData.byteSeatFlags = m_pPlayer->byteSeatId;
		// Set the player sync position
		m_pPlayer->passengerSyncData.vecPosition = pVehicle->vecPosition;
		// Set the player keys
		m_pPlayer->passengerSyncData.wUDAnalog = m_pPlayer->wUDAnalog;
		m_pPlayer->passengerSyncData.wLRAnalog = m_pPlayer->wLRAnalog;
		m_pPlayer->passengerSyncData.wKeys = static_cast<WORD>(m_pPlayer->dwKeys);
		// Set the player health and armour
		m_pPlayer->passengerSyncData.bytePlayerHealth = static_cast<BYTE>(m_pPlayer->fHealth);
		m_pPlayer->passengerSyncData.bytePlayerArmour = static_cast<BYTE>(m_pPlayer->fArmour);
		// Set the player weapon
		m_pPlayer->passengerSyncData.bytePlayerWeapon = m_byteWeaponId;
		// Set the new update state
		m_pPlayer->iUpdateState = iState;
	}
	// Update the tick count
	m_dwUpdateTick = dwThisTick;
	// Call update callback
	if (!CCallbackManager::OnUpdate(m_wPlayerId)) {
		m_pPlayer->iUpdateState = UPDATE_STATE_NONE;
	}
}
void CPlayerData::ResetSyncMoving(int iState)
{
	if (iState == UPDATE_STATE_DRIVER) {
		// Set the state
		SetState(PLAYER_STATE_DRIVER);
		// Pause the sync data
		CVehicleSyncData vehicleSyncData = m_pPlayer->vehicleSyncData;
		vehicleSyncData.wUDAnalog = 0;
		vehicleSyncData.wLRAnalog = 0;
		vehicleSyncData.wKeys = 0;
		vehicleSyncData.vecVelocity = CVector(0.0f, 0.0f, 0.0f);
		// Set vehicle sync data
		SetVehicleSync(&vehicleSyncData);
		// Update the player
		UpdateSync(UPDATE_STATE_DRIVER);
	} else if (iState == UPDATE_STATE_ONFOOT) {
		// Set the state
		SetState(PLAYER_STATE_ONFOOT);
		// Pause the sync data
		CSyncData playerSyncData = m_pPlayer->syncData;
		playerSyncData.wUDAnalog = 0;
		playerSyncData.wLRAnalog = 0;
		playerSyncData.wKeys = 0;
		playerSyncData.vecVelocity = CVector(0.0f, 0.0f, 0.0f);
		// Set vehicle sync data
		SetOnFootSync(&playerSyncData);
		// Update the player
		UpdateSync(UPDATE_STATE_ONFOOT);
	}
}

void CPlayerData::UpdateAim()
{
	if (m_bAiming) {
		BYTE byteWeaponId = m_pPlayer->syncData.byteWeapon;
		int iWeaponType = GetWeaponType(m_pPlayer->syncData.byteWeapon);

		// Set the camera mode
		if (iWeaponType == WEAPON_TYPE_MELEE) {
			m_pPlayer->aimSyncData.byteCameraMode = 4;
		} else if (byteWeaponId == WEAPON_SNIPER) {
			m_pPlayer->aimSyncData.byteCameraMode = 7;
		} else if (byteWeaponId == WEAPON_CAMERA) {
			m_pPlayer->aimSyncData.byteCameraMode = 46;
		} else if (byteWeaponId == WEAPON_ROCKETLAUNCHER) {
			m_pPlayer->aimSyncData.byteCameraMode = 8;
		} else if (byteWeaponId == WEAPON_HEATSEEKER) {
			m_pPlayer->aimSyncData.byteCameraMode = 51;
		} else {
			m_pPlayer->aimSyncData.byteCameraMode = 53;
		}
	} else {
		// Set the camera mode and weapon state
		m_pPlayer->aimSyncData.byteCameraMode = 0;
		// Convert the player angle to radians
		float fAngle = CMath::DegreeToRadians(GetAngle());
		// Calculate the camera target
		CVector vecTarget(m_pPlayer->aimSyncData.vecPosition.fX - sin(fAngle) * 0.2f,
		                  m_pPlayer->aimSyncData.vecPosition.fY + cos(fAngle) * 0.2f,
		                  m_pPlayer->aimSyncData.vecPosition.fZ);

		// Calculate the camera front vector
		m_pPlayer->aimSyncData.vecFront = vecTarget - m_pPlayer->aimSyncData.vecPosition;
	}

	// Update the weapon state
	UpdateWeaponState();
	// Set the aim sync flag
	m_pPlayer->bHasAimSync = true;
}

void CPlayerData::UpdateWeaponState()
{
	BYTE byteWeaponId = m_pPlayer->syncData.byteWeapon;
	switch (byteWeaponId) {
		case 0:
		case WEAPON_BRASSKNUCKLE:
		case WEAPON_GOLFCLUB:
		case WEAPON_NITESTICK:
		case WEAPON_KNIFE:
		case WEAPON_BAT:
		case WEAPON_SHOVEL:
		case WEAPON_POOLSTICK:
		case WEAPON_KATANA:
		case WEAPON_CHAINSAW:
		case WEAPON_DILDO:
		case WEAPON_DILDO2:
		case WEAPON_VIBRATOR:
		case WEAPON_VIBRATOR2:
		case WEAPON_FLOWER:
		case WEAPON_CANE:
		case WEAPON_BOMB:
		case WEAPON_CAMERA:
		case WEAPON_NIGHTVISION:
		case WEAPON_INFRARED:
		case WEAPON_PARACHUTE:
			SetWeaponState(WEAPONSTATE_NO_BULLETS);
			break;

		case WEAPON_GRENADE:
		case WEAPON_TEARGAS:
		case WEAPON_MOLTOV:
		case WEAPON_RIFLE:
		case WEAPON_SNIPER:
		case WEAPON_ROCKETLAUNCHER:
		case WEAPON_HEATSEEKER:
		case WEAPON_SATCHEL:
			SetWeaponState(WEAPONSTATE_LAST_BULLET);
			break;

		case WEAPON_COLT45:
		case WEAPON_SILENCED:
		case WEAPON_DEAGLE:
		case WEAPON_SAWEDOFF:
		case WEAPON_SHOTGSPA:
		case WEAPON_UZI:
		case WEAPON_MP5:
		case WEAPON_AK47:
		case WEAPON_M4:
		case WEAPON_TEC9:
		case WEAPON_FLAMETHROWER:
		case WEAPON_MINIGUN:
		case WEAPON_SPRAYCAN:
		case WEAPON_FIREEXTINGUISHER:
			if (m_bReloading) {
				SetWeaponState(WEAPONSTATE_RELOADING);
			} else if (m_wAmmoInClip == 1) {
				SetWeaponState(WEAPONSTATE_LAST_BULLET);
			} else if (m_wAmmo == 0 && !m_bHasInfiniteAmmo) {
				SetWeaponState(WEAPONSTATE_NO_BULLETS);
			} else if (m_wAmmoInClip > 1) {
				SetWeaponState(WEAPONSTATE_MORE_BULLETS);
			}
			break;

		case WEAPON_SHOTGUN:
			if (m_bReloading) {
				SetWeaponState(WEAPONSTATE_RELOADING);
			} else if (m_wAmmo == 0 && !m_bHasInfiniteAmmo) {
				SetWeaponState(WEAPONSTATE_NO_BULLETS);
			} else if (m_wAmmoInClip == 1) {
				SetWeaponState(WEAPONSTATE_LAST_BULLET);
			}
			break;

		default:
			SetWeaponState(WEAPONSTATE_NO_BULLETS);
			break;
	}
}

bool CPlayerData::IsSpawned()
{
	return m_bSpawned;
}

void CPlayerData::SetSpawnedStatus(bool status)
{
	m_bSpawned = status;
}

bool CPlayerData::IsStreamedIn(WORD wForPlayerId)
{
	return pNetGame->pPlayerPool->pPlayer[wForPlayerId]->byteStreamedIn[m_wPlayerId] != 0;
}

void CPlayerData::ShowForPlayer(WORD wPlayerId)
{
	RakNet::BitStream bStream;
	bStream.Write(m_wPlayerId);
	bStream.Write(m_pPlayer->spawn.byteTeam);
	bStream.Write(m_pPlayer->spawn.iSkin);
	bStream.Write(m_pPlayer->vecPosition);
	bStream.Write(m_pPlayer->fAngle);
	bStream.Write(m_pPlayer->dwNickNameColor);
	bStream.Write(m_pPlayer->byteFightingStyle);
	bStream.Write((char*)m_pPlayer->wSkillLevel, sizeof(WORD) * 11);
	// does not update player objects, because for NPC this is not needed
	CFunctions::PlayerRPC(&RPC_ShowPlayerForPlayer, &bStream, 0);
}

void CPlayerData::ShowForStreamedPlayers()
{
	for (WORD i = 0; i <= pNetGame->pPlayerPool->dwPlayerPoolSize; i++) {
		if (!pServer->GetPlayerManager()->IsPlayerConnected(i) || !IsStreamedIn(i)) {
			continue;
		}

		ShowForPlayer(i);
	}
}

bool CPlayerData::SetState(BYTE byteState)
{
	// Make sure the state is valid
	if (byteState > PLAYER_STATE_SPAWNED) {
		return false;
	}

	// Set the new state
	m_pPlayer->byteState = byteState;
	return true;
}

BYTE CPlayerData::GetState()
{
	return m_pPlayer->byteState;
}

bool CPlayerData::Kill(WORD wKillerId, BYTE byteReason)
{
	if (m_pPlayer->byteState == PLAYER_STATE_WASTED) {
		return false;
	}

	// Stop Playing any playback
	if (m_bPlaying) {
		StopPlayingPlayback();
	}

	// Stop playing any node
	if (m_bPlayingNode) {
		StopPlayingNode();
	}

	// Stop moving and aiming
	StopMoving();
	StopAim();
	SetKeys(KEY_NONE, KEY_NONE, KEY_NONE);
	// Kill the NPC
	CFunctions::KillPlayer(m_pPlayer, byteReason, wKillerId);
	// Set the NPC state
	SetState(PLAYER_STATE_WASTED);
	// Call the NPC death callback
	CCallbackManager::OnDeath(m_wPlayerId, wKillerId, byteReason);

	return true;
}

void CPlayerData::Process()
{
	// Make sure the player is setup
	if (!m_bSetup) {
		return;
	}

	if (!pServer->GetPlayerManager()->IsPlayerConnected(m_wPlayerId)) {
		pServer->GetPlayerManager()->DeletePlayer(m_wPlayerId);
		return;
	}

	// Process Playing
	if (m_bPlaying) {
		// Process the player playback
		if (!m_pPlayback->Process(this)) {
			StopPlayingPlayback();
		}
		return;
	}

	// get data
	DWORD dwThisTick = GetTickCount();
	DWORD dwUpdateRate = pServer->GetUpdateRate();
	BYTE byteState = GetState();

	// Process death
	if (GetHealth() <= 0.0f && byteState != PLAYER_STATE_WASTED && byteState != PLAYER_STATE_SPAWNED) {
		// check on vehicle
		if (byteState == PLAYER_STATE_DRIVER || byteState == PLAYER_STATE_PASSENGER) {
			RemoveFromVehicle();
			m_dwKillVehicleTickCount = dwThisTick;
		}

		// Kill the player
		if (dwThisTick - m_dwKillVehicleTickCount >= dwUpdateRate) {
			m_dwKillVehicleTickCount = 0;
			Kill(m_wLastDamagerId, m_byteLastDamagerWeapon);
		}
	}

	// Update the player depending on his state
	if (byteState == PLAYER_STATE_ONFOOT || byteState == PLAYER_STATE_DRIVER) {
		// Is the player moving at player
		if (pServer->GetPlayerManager()->IsPlayerConnected(m_wMoveId) && IsMovingAtPlayer(m_wMoveId)) {
			CPlayer *pPlayer = pNetGame->pPlayerPool->pPlayer[m_wMoveId];
			if (pPlayer) {
				if (CMath::GetDistanceBetween3DPoints(m_vecMovePlayerPosition, pPlayer->vecPosition) > m_fDistCheck) {
					m_vecMovePlayerPosition = pPlayer->vecPosition;
					UpdateMovingData(pPlayer->vecPosition, m_fMoveRadius, m_bMoveSetAngle, m_fMoveSpeed, m_fDistOffset);
				}
			} else {
				StopMoving();
			}
		}

		// Is player moving
		if (m_bMoving && byteState == PLAYER_STATE_DRIVER && !GetVehicle()) {
			StopMoving();
			SetState(PLAYER_STATE_ONFOOT);
			SetVehicle(INVALID_VEHICLE_ID, 0);
		} else if (m_bMoving) {
			DWORD dwMoveTick = dwThisTick - m_dwMoveStartTime;

			if (dwMoveTick < m_dwMoveTime) {
				CVector vecNewPosition;
				CVector vecVelocity;

				GetPosition(&vecNewPosition);
				GetVelocity(&vecVelocity);

				int iTickDiff = dwThisTick - m_dwMoveTickCount;
				if (iTickDiff > 0) {
					vecNewPosition += vecVelocity * static_cast<float>(iTickDiff);
				}

				UpdateHeightPos(&vecNewPosition);
				SetPosition(vecNewPosition);

				m_dwMoveTickCount = dwThisTick;
			} else if (IsMovingByMovePath(m_iMovePath)) {
				CCallbackManager::OnFinishMovePathPoint(m_wPlayerId, m_iMovePath, m_iMovePoint);

				m_iMovePoint++;
				CVector *vecPoint = pServer->GetMovePath()->GetPoint(m_iMovePath, m_iMovePoint);
				if (vecPoint) {
					UpdateMovingData(*vecPoint, m_fMoveRadius, m_bMoveSetAngle, m_fMoveSpeed, m_fDistOffset);
				} else {
					int iMovePath = m_iMovePath;
					StopMoving();
					CCallbackManager::OnFinishMovePath(m_wPlayerId, iMovePath);
				}
			} else if (m_bPlayingNode && !m_bIsPlayingNodePaused) {
				if (CCallbackManager::OnFinishNodePoint(m_wPlayerId, m_wNodePoint)) {
					WORD wNewPoint = m_pNode->Process(this, m_wNodePoint, m_wNodeLastPoint);
					m_wNodeLastPoint = m_wNodePoint;
					m_wNodePoint = wNewPoint;
				} else {
					StopPlayingNode();
				}
			} else if (dwMoveTick > m_dwMoveTime + m_dwMoveStopDelay) {
				StopMoving();

				if (m_wVehicleToEnter != INVALID_VEHICLE_ID) {
					m_dwEnterExitTickCount = dwThisTick;
					m_bEntering = true;

					if (pServer->IsVehicleSeatOccupied(m_wPlayerId, m_wVehicleToEnter, m_byteSeatToEnter)) {
						m_bJacking = true;
					}

					CFunctions::PlayerEnterVehicle(m_pPlayer, m_wVehicleToEnter, m_byteSeatToEnter);
				} else {
					CCallbackManager::OnReachDestination(m_wPlayerId);
				}
			}
		}

		// if an NPC has removed in callback
		if (!pServer->GetPlayerManager()->IsNpcConnected(m_wPlayerId)) {
			return;
		}

		// Is velocity updates position
		if (m_bVelocityUpdatePos) {
			CVector vecPosition;
			CVector vecVelocity;

			GetPosition(&vecPosition);
			GetVelocity(&vecVelocity);

			vecPosition += vecVelocity;

			UpdateHeightPos(&vecPosition);
			SetPosition(vecPosition);
		}
	}
	if (byteState == PLAYER_STATE_ONFOOT) {
		// Process the player surfing
		if (m_wSurfingInfo != 0) {
			WORD wVehicleId = m_wSurfingInfo;
			if (wVehicleId > 0 && wVehicleId < MAX_VEHICLES) {
				m_pPlayer->vecPosition = pNetGame->pVehiclePool->pVehicle[wVehicleId]->vecPosition + m_vecSurfing;
			} else {
				WORD wObjectId = m_wSurfingInfo - MAX_VEHICLES;
				if (wObjectId > 0 && wObjectId < MAX_OBJECTS) {
					CObject *pObject = NULL;
					CVector vecPos;

					if (pNetGame->pObjectPool->bObjectSlotState[wObjectId]) {
						pObject = pNetGame->pObjectPool->pObjects[wObjectId];
					} else if (pNetGame->pObjectPool->bPlayerObjectSlotState[m_wPlayerId][wObjectId]) {
						pObject = pNetGame->pObjectPool->pPlayerObjects[m_wPlayerId][wObjectId];
					}

					if (pObject) {
						if (pObject->wAttachedObjectID != INVALID_OBJECT_ID) {
							vecPos = pObject->vecAttachedOffset + pNetGame->pObjectPool->pObjects[pObject->wAttachedObjectID]->matWorld.pos;
						} else if (pObject->wAttachedVehicleID != INVALID_VEHICLE_ID) {
							vecPos = pObject->vecAttachedOffset + pNetGame->pVehiclePool->pVehicle[pObject->wAttachedVehicleID]->vecPosition;
						} else {
							vecPos = pObject->matWorld.pos;
						}

						m_pPlayer->vecPosition = vecPos + m_vecSurfing;
					}
				}
			}
		}
		// Are we performing the entry animation ?
		if (m_bEntering) {
			if (dwThisTick - m_dwEnterExitTickCount > static_cast<DWORD>(m_bJacking ? 5800 : 2500)) {
				CVehicle *pVehicle = pNetGame->pVehiclePool->pVehicle[m_wVehicleToEnter];
				if (pVehicle) {
					SetState(m_byteSeatToEnter == 0 ? PLAYER_STATE_DRIVER : PLAYER_STATE_PASSENGER);
					SetVehicle(m_wVehicleToEnter, m_byteSeatToEnter);
					SetAngle(pServer->GetVehicleAngle(pVehicle));

					CCallbackManager::OnVehicleEntryComplete(m_wPlayerId, m_wVehicleToEnter, m_byteSeatToEnter);

					// if an NPC has removed in callback
					if (!pServer->GetPlayerManager()->IsNpcConnected(m_wPlayerId)) {
						return;
					}
				}

				m_bEntering = false;
				m_bJacking = false;
				m_wVehicleToEnter = INVALID_VEHICLE_ID;
				m_byteSeatToEnter = 0;
			}
		}

		if (m_bAiming) {
			bool bIsSurfing = m_wSurfingInfo != 0;
			if (bIsSurfing) {
				UpdateAimingData(m_vecAimAt, m_bAimSetAngle);
			}

			if (pServer->GetPlayerManager()->IsPlayerConnected(m_wHitId) && IsAimingAtPlayer(m_wHitId)) {
				CPlayer *pPlayer = pNetGame->pPlayerPool->pPlayer[m_wHitId];
				if (pPlayer) {
					CVector vecAimPoint = pPlayer->vecPosition + m_vecAimOffset;
					if (m_vecAimAt != vecAimPoint) {
						UpdateAimingData(vecAimPoint, m_bAimSetAngle);
					}
				} else {
					StopAim();
				}
			}
		}

		if (m_bReloading) {
			int iReloadTime = GetWeaponActualReloadTime(m_byteWeaponId);
			bool bIsReloadFinished = iReloadTime != -1 && (dwThisTick - m_dwReloadTickCount) >= static_cast<DWORD>(iReloadTime);

			if (bIsReloadFinished) {
				m_dwShootTickCount = dwThisTick;
				m_bReloading = false;
				m_bShooting = true;
				m_wAmmoInClip = static_cast<WORD>(GetWeaponActualClipSize(m_byteWeaponId));
			} else {
				SetKeys(m_pPlayer->wUDAnalog, m_pPlayer->wLRAnalog, (m_pPlayer->dwKeys & ~KEY_FIRE) | KEY_AIM);
			}
		} else if (m_bShooting) {
			if (m_wAmmo == 0 && !m_bHasInfiniteAmmo) {
				m_bShooting = false;
				SetKeys(m_pPlayer->wUDAnalog, m_pPlayer->wLRAnalog, (m_pPlayer->dwKeys & ~KEY_FIRE) | KEY_AIM);
			} else {
				// Get the shoot time
				int iShootTime = GetWeaponShootTime(m_byteWeaponId);

				// shoot delay
				if (iShootTime != -1 && iShootTime < static_cast<int>(m_dwShootDelay)) {
					iShootTime = static_cast<int>(m_dwShootDelay);
				}

				DWORD dwLastShootTime = dwThisTick - m_dwShootTickCount;

				if (dwLastShootTime >= m_dwShootDelay) {
					SetKeys(m_pPlayer->wUDAnalog, m_pPlayer->wLRAnalog, (m_pPlayer->dwKeys & ~KEY_FIRE) | KEY_AIM);
				}

				// shoot time
				if (iShootTime != -1 && iShootTime <= static_cast<int>(dwLastShootTime)) {
					// Send bullet
					if (m_wAmmoInClip != 0) {
						if (GetWeaponType(m_byteWeaponId) == WEAPON_TYPE_SHOOT) {
							bool bIsHit = rand() % 100 < static_cast<int>(GetWeaponAccuracy(m_byteWeaponId) * 100.0f);

							CFunctions::PlayerShoot(m_wPlayerId, m_wHitId, m_byteHitType, m_byteWeaponId, m_vecAimAt, m_vecAimOffsetFrom, bIsHit);
						}

						SetKeys(m_pPlayer->wUDAnalog, m_pPlayer->wLRAnalog, m_pPlayer->dwKeys | KEY_AIM | KEY_FIRE);

						// decrease the ammo
						if (!m_bHasInfiniteAmmo) {
							m_wAmmo--;
						}

						m_wAmmoInClip--;
					}

					// Check for reload
					bool bIsNeedToReload = m_bHasReload
					                       && GetWeaponActualClipSize(m_byteWeaponId) > 0
					                       && (m_wAmmo != 0 || m_bHasInfiniteAmmo)
					                       && m_wAmmoInClip == 0;

					if (bIsNeedToReload) {
						m_dwReloadTickCount = dwThisTick;
						m_bReloading = true;
						m_bShooting = false;
					}

					// Update the shoot tick
					m_dwShootTickCount = dwThisTick;
				}
			}
		} else if (m_bMeleeAttack) {
			if ((m_dwUpdateTick - m_dwShootTickCount) >= m_dwMeleeDelay) {
				SetKeys(m_pPlayer->wUDAnalog, m_pPlayer->wLRAnalog, m_pPlayer->dwKeys | (m_bMeleeFightstyle ? KEY_AIM | KEY_SECONDARY_ATTACK : KEY_FIRE));
				m_dwShootTickCount = m_dwUpdateTick;
			} else if (m_dwUpdateTick > m_dwShootTickCount) {
				SetKeys(m_pPlayer->wUDAnalog, m_pPlayer->wLRAnalog, m_pPlayer->dwKeys & ~(m_bMeleeFightstyle ? KEY_SECONDARY_ATTACK : KEY_FIRE));
			}
		}

		Update(UPDATE_STATE_ONFOOT);
	} else if (byteState == PLAYER_STATE_DRIVER) {
		Update(UPDATE_STATE_DRIVER);
	} else if (byteState == PLAYER_STATE_PASSENGER) {
		Update(UPDATE_STATE_PASSENGER);
	} else if (byteState == PLAYER_STATE_EXIT_VEHICLE) {
		if ((dwThisTick - m_dwEnterExitTickCount) > 1500) {
			RemoveFromVehicle();
			CCallbackManager::OnVehicleExitComplete(m_wPlayerId);
		}
	}
}

void CPlayerData::SetPosition(CVector vecPosition)
{
	// Check the player state
	if (GetState() == PLAYER_STATE_DRIVER && m_pPlayer->wVehicleId != INVALID_VEHICLE_ID) {
		// Get the player vehicle interface
		CVehicle *pVehicle = GetVehicle();
		// Get the player vehicle position
		pVehicle->vecPosition = vecPosition;
	}
	// Set the player position
	m_pPlayer->vecPosition = vecPosition;
}

void CPlayerData::GetPosition(CVector *pvecPosition)
{
	// Check the player state
	if ((GetState() == PLAYER_STATE_DRIVER || GetState() == PLAYER_STATE_PASSENGER) && m_pPlayer->wVehicleId != INVALID_VEHICLE_ID) {
		CVehicle *pVehicle = GetVehicle();
		*pvecPosition = pServer->GetVehiclePos(pVehicle);
	} else {
		*pvecPosition = m_pPlayer->vecPosition;
	}
}

void CPlayerData::UpdateHeightPos(CVector *pvecPosition)
{
	if (m_iMoveMode == MOVE_MODE_MAPANDREAS && pServer->IsMapAndreasInited() && pvecPosition->fZ >= 0.0f && m_pPlayer->iInteriorId == 0) {
		float fNewZ = pServer->GetMapAndreas()->FindZ_For2DCoord(pvecPosition->fX, pvecPosition->fY) + 0.5f;
		if (m_fMinHeightPos < 0.0f) {
			pvecPosition->fZ = fNewZ;
		} else if (m_fMinHeightPos <= std::abs(fNewZ - pvecPosition->fZ)) {
			if (CCallbackManager::OnChangeHeightPos(m_wPlayerId, fNewZ, pvecPosition->fZ)) {
				pvecPosition->fZ = fNewZ;
			}
		}
	}
}

void CPlayerData::SetQuaternion(float *fQuaternion)
{
	// Check the player state
	if (GetState() == PLAYER_STATE_DRIVER && m_pPlayer->wVehicleId != INVALID_VEHICLE_ID) {
		// get vehicle interface
		CVehicle *pVehicle = GetVehicle();
		// update matrix
		CMath::GetMatrixFromQuaternion(fQuaternion, &pVehicle->vehMatrix);
		// update sync data
		memcpy(m_pPlayer->vehicleSyncData.fQuaternion, fQuaternion, 4 * sizeof(float));
	}
	// Set the player quaternion
	memcpy(m_pPlayer->syncData.fQuaternion, fQuaternion, 4 * sizeof(float));
}

void CPlayerData::GetQuaternion(float *fQuaternion)
{
	// Check the player state
	if ((GetState() == PLAYER_STATE_DRIVER || GetState() == PLAYER_STATE_PASSENGER) && m_pPlayer->wVehicleId != INVALID_VEHICLE_ID) {
		// Get the player vehicle interface
		CVehicle *pVehicle = GetVehicle();
		// Get the player vehicle quaternion
		CMath::GetQuaternionFromMatrix(pVehicle->vehMatrix, fQuaternion);
	} else {
		// Get the player quaternion
		memcpy(fQuaternion, m_pPlayer->syncData.fQuaternion, 4 * sizeof(float));
	}
}

void CPlayerData::SetAngle(float fAngle)
{
	// Set the player
	m_pPlayer->fAngle = fAngle;
	// Rotate new quaternion matrix
	MATRIX4X4 matrix;
	float *fQuaternion = new float[4];

	CMath::QuaternionRotateZ(&matrix, CMath::DegreeToRadians(fAngle));
	CMath::GetQuaternionFromMatrix(matrix, fQuaternion);
	// Update quaternion
	SetQuaternion(fQuaternion);

	SAFE_DELETE(fQuaternion);
}

float CPlayerData::GetAngle()
{
	return m_pPlayer->fAngle;
}

void CPlayerData::SetHealth(float fHealth)
{
	// Validate the health
	if (fHealth < 0.0f) {
		fHealth = 0.0f;
	}

	// Set the player health
	m_pPlayer->fHealth = fHealth;
}

float CPlayerData::GetHealth()
{
	return m_pPlayer->fHealth;
}

void CPlayerData::SetArmour(float fArmour)
{
	// Validate the armour
	if (fArmour < 0.0f) {
		fArmour = 0.0f;
	}

	// Set the player armour
	m_pPlayer->fArmour = fArmour;
}

float CPlayerData::GetArmour()
{
	return m_pPlayer->fArmour;
}

void CPlayerData::SetInvulnerable(bool bInvulnerable)
{
	m_bIsInvulnerable = bInvulnerable;
}

bool CPlayerData::IsInvulnerable()
{
	return m_bIsInvulnerable;
}

void CPlayerData::SetSkin(int iSkin)
{
	// Make sure the skin has changed
	if (iSkin == m_pPlayer->spawn.iSkin) {
		return;
	}

	// Validate the skin
	if (iSkin > 311 || iSkin < 0 || iSkin == 74) {
		return;
	}

	// Send RPC
	if (m_pPlayer->bReadyToSpawn) {
		RakNet::BitStream bsData;
		bsData.Write(static_cast<DWORD>(m_pPlayer->wPlayerId));
		bsData.Write(iSkin);
		CFunctions::AddedPlayersRPC(&RPC_SetPlayerSkin, &bsData, m_wPlayerId);
	}

	// Set the player skin
	m_pPlayer->spawn.iSkin = iSkin;
}

int CPlayerData::GetSkin()
{
	return m_pPlayer->spawn.iSkin;
}

void CPlayerData::SetInterior(int iInterior)
{
	// Set the player interior
	m_pPlayer->iInteriorId = iInterior;
}

int CPlayerData::GetInterior()
{
	return m_pPlayer->iInteriorId;
}

void CPlayerData::SetVirtualWorld(int iVirtualWorld)
{
	pNetGame->pPlayerPool->dwVirtualWorld[m_wPlayerId] = iVirtualWorld;
}

int CPlayerData::GetVirtualWorld()
{
	return pNetGame->pPlayerPool->dwVirtualWorld[m_wPlayerId];
}

void CPlayerData::SetWeapon(BYTE byteWeaponId)
{
	if (byteWeaponId < 0 || byteWeaponId > 46) {
		return;
	}

	if (byteWeaponId != m_byteWeaponId) {
		m_wAmmoInClip = 0;
	}
	
	m_byteWeaponId = byteWeaponId;
}

BYTE CPlayerData::GetWeapon()
{
	return m_byteWeaponId;
}

void CPlayerData::SetAmmo(WORD wAmmo)
{
	// set the player ammo
	m_wAmmo = wAmmo;

	// set the player ammo in clip
	if (m_wAmmo < m_wAmmoInClip) {
		m_wAmmoInClip = m_wAmmo;
	}
}

WORD CPlayerData::GetAmmo()
{
	return m_wAmmo;
}

void CPlayerData::SetAmmoInClip(WORD wAmmo)
{
	WORD wClipSize = static_cast<WORD>(GetWeaponActualClipSize(m_byteWeaponId));

	m_wAmmoInClip = wAmmo < wClipSize ? wAmmo : wClipSize;
}

WORD CPlayerData::GetAmmoInClip()
{
	return m_wAmmoInClip;
}

void CPlayerData::SetWeaponSkill(DWORD dwSkill, WORD wLevel)
{
	if (m_pPlayer->byteState < 11) {
		WORD wClipSize = static_cast<WORD>(GetWeaponClipSize(m_byteWeaponId));

		if (dwSkill == static_cast<DWORD>(m_pWeaponInfo->GetSkillID(m_byteWeaponId))
		    && IsWeaponDoubleHanded(m_byteWeaponId)
		    && wLevel < 999
		    && m_wAmmoInClip > wClipSize
		    ) {
			if (m_wAmmo < m_wAmmoInClip) {
				m_wAmmoInClip = m_wAmmo;
			}

			if (m_wAmmoInClip > wClipSize) {
				m_wAmmoInClip = wClipSize;
			}
		}
		
		m_pPlayer->wSkillLevel[dwSkill] = wLevel;

		RakNet::BitStream bsData;
		bsData.Write(m_wPlayerId);
		bsData.Write(dwSkill);
		bsData.Write(wLevel);

		CFunctions::PlayerRPC(&RPC_SetPlayerSkillLevel, &bsData, m_wPlayerId);
		if (m_pPlayer->byteState > 0 && m_pPlayer->byteState != PLAYER_STATE_SPECTATING) {
			CFunctions::AddedPlayersRPC(&RPC_SetPlayerSkillLevel, &bsData, m_wPlayerId);
		}
	}
}

WORD CPlayerData::GetWeaponSkill(DWORD dwSkill)
{
	return m_pPlayer->wSkillLevel[dwSkill];
}

void CPlayerData::SetWeaponState(int iState)
{
	if (iState == WEAPONSTATE_UNKNOWN) {
		return;
	}

	int iOldState = m_pPlayer->aimSyncData.byteWeaponState;
	m_pPlayer->aimSyncData.byteWeaponState = iState;

	switch (iState) {
		case WEAPONSTATE_LAST_BULLET:
			if (m_wAmmo > 0) {
				m_wAmmoInClip = 1;
			}
			break;
		case WEAPONSTATE_MORE_BULLETS:
			if (m_wAmmo > 1 && m_wAmmoInClip <= 1) {
				m_wAmmoInClip = static_cast<WORD>(GetWeaponActualClipSize(m_byteWeaponId));
			}
			break;
		case WEAPONSTATE_NO_BULLETS:
			m_wAmmoInClip = 0;
			break;
		case WEAPONSTATE_RELOADING:
			if (!m_bReloading) {
				m_dwReloadTickCount = GetTickCount();
				m_bReloading = true;
				m_bShooting = false;
			}
			break;
	}

	if (iOldState != iState) {
		CCallbackManager::OnWeaponStateChange(m_wPlayerId, iState);
	}
}

int CPlayerData::GetWeaponState()
{
	if (GetState() == PLAYER_STATE_ONFOOT) {
		return m_pPlayer->aimSyncData.byteWeaponState;
	}

	return WEAPONSTATE_UNKNOWN;
}

int CPlayerData::GetWeaponType(BYTE byteWeaponId)
{
	return m_pWeaponInfo->GetType(byteWeaponId);
}

bool CPlayerData::SetWeaponReloadTime(BYTE byteWeaponId, int iTime)
{
	return m_pWeaponInfo->SetReloadTime(byteWeaponId, iTime);
}

int CPlayerData::GetWeaponReloadTime(BYTE byteWeaponId)
{
	return m_pWeaponInfo->GetReloadTime(byteWeaponId);
}

int CPlayerData::GetWeaponActualReloadTime(BYTE byteWeaponId)
{
	int iTime = m_pWeaponInfo->GetReloadTime(byteWeaponId);
	DWORD dwSkill = static_cast<DWORD>(m_pWeaponInfo->GetSkillID(byteWeaponId));

	if (IsWeaponDoubleHanded(byteWeaponId) && GetWeaponSkill(dwSkill) >= 999) {
		iTime += 700;
	}

	return iTime;
}

bool CPlayerData::SetWeaponShootTime(BYTE byteWeaponId, int iTime)
{
	return m_pWeaponInfo->SetShootTime(byteWeaponId, iTime);
}

int CPlayerData::GetWeaponShootTime(BYTE byteWeaponId)
{
	return m_pWeaponInfo->GetShootTime(byteWeaponId);
}

bool CPlayerData::SetWeaponClipSize(BYTE byteWeaponId, int iSize)
{
	bool bSuccess = m_pWeaponInfo->SetClipSize(byteWeaponId, iSize);

	if (bSuccess && m_byteWeaponId == byteWeaponId) {
		m_wAmmoInClip = static_cast<WORD>(GetWeaponActualClipSize(byteWeaponId));
	}

	return bSuccess;
}

int CPlayerData::GetWeaponClipSize(BYTE byteWeaponId)
{
	return m_pWeaponInfo->GetClipSize(byteWeaponId);
}

int CPlayerData::GetWeaponActualClipSize(BYTE byteWeaponId)
{
	int iSize = m_pWeaponInfo->GetClipSize(byteWeaponId);

	if (IsWeaponDoubleHanded(byteWeaponId)) {
		iSize *= 2;
	}

	if (m_wAmmo < iSize && !m_bHasInfiniteAmmo) {
		iSize = m_wAmmo;
	}

	return iSize;
}

bool CPlayerData::IsWeaponDoubleHanded(BYTE byteWeaponId)
{
	return m_pWeaponInfo->IsDoubleHanded(byteWeaponId) && GetWeaponSkill(m_pWeaponInfo->GetSkillID(byteWeaponId)) >= 999;
}

bool CPlayerData::SetWeaponAccuracy(BYTE byteWeaponId, float fAccuracy)
{
	return m_pWeaponInfo->SetAccuracy(byteWeaponId, fAccuracy);
}

float CPlayerData::GetWeaponAccuracy(BYTE byteWeaponId)
{
	return m_pWeaponInfo->GetAccuracy(byteWeaponId);
}

bool CPlayerData::SetWeaponInfo(BYTE byteWeaponId, SWeaponInfo sWeaponInfo)
{
	bool bSuccess = m_pWeaponInfo->SetInfo(byteWeaponId, sWeaponInfo);

	if (bSuccess && m_byteWeaponId == byteWeaponId) {
		m_wAmmoInClip = static_cast<WORD>(GetWeaponActualClipSize(byteWeaponId));
	}

	return bSuccess;
}

SWeaponInfo CPlayerData::GetWeaponInfo(BYTE byteWeaponId)
{
	return m_pWeaponInfo->GetInfo(byteWeaponId);
}

void CPlayerData::SetSpecialAction(BYTE byteActionId)
{
	// Validate the action id
	if (byteActionId < 0 || (byteActionId > 13 && byteActionId < 20) || (byteActionId > 25 && byteActionId != 68)) {
		return;
	}

	// Set the player action
	m_pPlayer->syncData.byteSpecialAction = byteActionId;
}

BYTE CPlayerData::GetSpecialAction()
{
	return m_pPlayer->syncData.byteSpecialAction;
}

void CPlayerData::SetFightingStyle(BYTE byteStyle)
{
	m_pPlayer->byteFightingStyle = byteStyle;

	RakNet::BitStream bsData;
	bsData.Write(m_wPlayerId);
	bsData.Write(byteStyle);

	CFunctions::AddedPlayersRPC(&RPC_SetPlayerFightingStyle, &bsData, m_wPlayerId);
	CFunctions::PlayerRPC(&RPC_SetPlayerFightingStyle, &bsData, m_wPlayerId);
}

BYTE CPlayerData::GetFightingStyle()
{
	return m_pPlayer->byteFightingStyle;
}

void CPlayerData::SetAnimation(WORD wAnimationId, float fDelta, bool bLoop, bool bLockX, bool bLockY, bool bFreeze, int iTime)
{
	m_pPlayer->syncData.wAnimIndex = wAnimationId;

	if (wAnimationId == 0) {
		m_pPlayer->syncData.wAnimFlags = 0;
	} else {
		// TODO: convert fDelta to minifloat (8-bit float) format
		m_pPlayer->syncData.wAnimFlags =   (static_cast<BYTE>(fDelta)& 0xFF)
		                                 | (bLoop << 8)
		                                 | (bLockX << 9)
		                                 | (bLockY << 10)
		                                 | (bFreeze << 11)
		                                 | (static_cast<BYTE>(iTime) << 12);
	}
}

void CPlayerData::SetAnimationByName(char *szName, float fDelta, bool bLoop, bool bLockX, bool bLockY, bool bFreeze, int iTime)
{
	WORD wAnimationId = CAnimationInfo::GetIndexByName(szName);
	SetAnimation(wAnimationId, fDelta, bLoop, bLockX, bLockY, bFreeze, iTime);
}

void CPlayerData::ResetAnimation()
{
	m_pPlayer->syncData.wAnimIndex = 0;
	m_pPlayer->syncData.wAnimFlags = 0;
}

void CPlayerData::GetAnimation(WORD *wAnimationId, float *fDelta, bool *bLoop, bool *bLockX, bool *bLockY, bool *bFreeze, int *iTime)
{
	*wAnimationId = m_pPlayer->syncData.wAnimIndex;
	*fDelta = static_cast<float>(m_pPlayer->syncData.wAnimFlags & 0xFF);
	*bLoop = (m_pPlayer->syncData.wAnimFlags >> 8 & 0x1) != 0;
	*bLockX = (m_pPlayer->syncData.wAnimFlags >> 9 & 0x1) != 0;
	*bLockY = (m_pPlayer->syncData.wAnimFlags >> 10 & 0x1) != 0;
	*bFreeze = (m_pPlayer->syncData.wAnimFlags >> 11 & 0x1) != 0;
	*iTime = (m_pPlayer->syncData.wAnimFlags >> 12 & 0xF);
}

void CPlayerData::ApplyAnimation(char *szAnimationLib, char *szAnimationName, float fDelta, bool bLoop, bool bLockX, bool bLockY, bool bFreeze, int iTime)
{
	RakNet::BitStream bsData;

	bsData.Write(m_wPlayerId);
	bsData.Write(static_cast<BYTE>(strlen(szAnimationLib)));
	bsData.Write(reinterpret_cast<char *>(szAnimationLib), strlen(szAnimationLib));
	bsData.Write(static_cast<BYTE>(strlen(szAnimationName)));
	bsData.Write(reinterpret_cast<char *>(szAnimationName), strlen(szAnimationName));
	bsData.Write(fDelta);
	bsData.Write(bLoop);
	bsData.Write(bLockX);
	bsData.Write(bLockY);
	bsData.Write(bFreeze);
	bsData.Write(iTime);

	CFunctions::AddedPlayersRPC(&RPC_ApplyAnimation, &bsData, m_wPlayerId);
}

void CPlayerData::ClearAnimations()
{
	RakNet::BitStream bsData;
	bsData.Write(m_wPlayerId);
	CFunctions::AddedPlayersRPC(&RPC_ClearAnimations, &bsData, m_wPlayerId);
}

void CPlayerData::SetVelocity(CVector vecVelocity, bool bUpdatePos)
{
	CVehicle *pVehicle = GetVehicle();
	if (pVehicle) {
		pVehicle->vecVelocity = vecVelocity;
	}
	m_pPlayer->vecVelocity = vecVelocity;
	m_bVelocityUpdatePos = bUpdatePos;
}

void CPlayerData::GetVelocity(CVector *pvecVelocity)
{
	CVehicle *pVehicle = GetVehicle();
	if (pVehicle) {
		*pvecVelocity = pVehicle->vecVelocity;
	} else {
		*pvecVelocity = m_pPlayer->vecVelocity;
	}
}

void CPlayerData::SetSpeed(float fSpeed)
{
	if (m_bPlayingNode) {
		UpdateMovingData(m_vecNodeLastPos, m_fNodeMoveRadius, m_bNodeMoveSetAngle, fSpeed, 0.0f);
	} else {
		UpdateMovingData(m_vecDestination, m_fMoveRadius, m_bMoveSetAngle, fSpeed, m_fDistOffset);
	}
}

float CPlayerData::GetSpeed()
{
	if (m_bPlayingNode) {
		return m_fNodeMoveSpeed;
	}
	return m_fMoveSpeed;
}

void CPlayerData::SetKeys(WORD wUDAnalog, WORD wLRAnalog, DWORD dwKeys)
{
	// Save the keys
	m_pPlayer->wUDAnalog = wUDAnalog;
	m_pPlayer->wLRAnalog = wLRAnalog;
	m_pPlayer->dwKeys = dwKeys;
}

void CPlayerData::GetKeys(WORD *pwUDAnalog, WORD *pwLRAnalog, DWORD *pdwKeys)
{
	// Gzt the keys
	*pwUDAnalog = m_pPlayer->wUDAnalog;
	*pwLRAnalog = m_pPlayer->wLRAnalog;
	*pdwKeys = m_pPlayer->dwKeys;
}

bool CPlayerData::GoTo(CVector vecPoint, int iType, int iMode, float fRadius, bool bSetAngle, float fSpeed, float fDistOffset, DWORD dwStopDelay)
{
	// Validate the movement
	if (iType == MOVE_TYPE_AUTO && GetState() == PLAYER_STATE_DRIVER) {
		iType = MOVE_TYPE_DRIVE;
	}

	if (iType != MOVE_TYPE_AUTO && iType != MOVE_TYPE_WALK && iType != MOVE_TYPE_RUN && iType != MOVE_TYPE_SPRINT && iType != MOVE_TYPE_DRIVE) {
		return false;
	}

	if (iType == MOVE_TYPE_SPRINT && m_bAiming) {
		StopAim();
	}

	// Get the moving type key and speed
	WORD wUDKey = m_pPlayer->wUDAnalog;
	WORD wLRKey = m_pPlayer->wLRAnalog;
	DWORD dwMoveKey = m_pPlayer->dwKeys & ~(KEY_WALK | KEY_SPRINT);

	if (iType == MOVE_TYPE_AUTO || iType == MOVE_TYPE_WALK || iType == MOVE_TYPE_RUN || iType == MOVE_TYPE_SPRINT) {
		wUDKey |= static_cast<WORD>(KEY_UP);

		if (iType == MOVE_TYPE_AUTO && fSpeed == MOVE_SPEED_AUTO) {
			iType = MOVE_TYPE_RUN;
		}

		if (fSpeed == MOVE_SPEED_AUTO) {
			if (iType == MOVE_TYPE_RUN) {
				fSpeed = MOVE_SPEED_RUN;
			} else if (iType == MOVE_TYPE_WALK) {
				fSpeed = MOVE_SPEED_WALK;
			} else if (iType == MOVE_TYPE_SPRINT) {
				fSpeed = MOVE_SPEED_SPRINT;
			}
		} else if (iType == MOVE_TYPE_AUTO) {
			float fSpeedValues[] = {MOVE_SPEED_WALK, MOVE_SPEED_RUN, MOVE_SPEED_SPRINT};
			float fNearestSpeed = CUtils::GetNearestFloatValue(fSpeed, fSpeedValues, sizeof(fSpeedValues) / sizeof(float));

			if (fNearestSpeed == MOVE_SPEED_SPRINT) {
				iType = MOVE_TYPE_SPRINT;
			} else if (fNearestSpeed == MOVE_SPEED_RUN) {
				iType = MOVE_TYPE_RUN;
			} else if (fNearestSpeed == MOVE_SPEED_WALK) {
				iType = MOVE_TYPE_WALK;
			}
		}

		if (iType == MOVE_TYPE_RUN) {
			dwMoveKey |= KEY_NONE;
		} else if (iType == MOVE_TYPE_WALK) {
			dwMoveKey |= KEY_WALK;
		} else if (iType == MOVE_TYPE_SPRINT) {
			dwMoveKey |= KEY_SPRINT;
		}
	} else if (iType == MOVE_TYPE_DRIVE) {
		dwMoveKey |= KEY_SPRINT;

		if (fSpeed == MOVE_SPEED_AUTO) {
			fSpeed = 1.0f;
		}
	}

	// Set the moving keys
	SetKeys(wUDKey, wLRKey, dwMoveKey);
	// Update moving data
	UpdateMovingData(vecPoint, fRadius, bSetAngle, fSpeed, fDistOffset);
	// Mark as moving
	m_bMoving = true;
	// Save the data
	SetMoveMode(iMode);
	m_iMoveType = iType;
	m_dwMoveStopDelay = dwStopDelay;
	return true;
}

bool CPlayerData::GoToPlayer(WORD wPlayerId, int iType, int iMode, float fRadius, bool bSetAngle, float fSpeed, float fDistOffset, float fDistCheck, DWORD dwStopDelay)
{
	CVector vecPos = pNetGame->pPlayerPool->pPlayer[wPlayerId]->vecPosition;
	if (GoTo(vecPos, iType, iMode, fRadius, bSetAngle, fSpeed, fDistOffset)) {
		m_wMoveId = wPlayerId;
		m_vecMovePlayerPosition = vecPos;
		m_fDistCheck = fDistCheck;
		m_dwMoveStopDelay = dwStopDelay;
		return true;
	}
	return false;
}

bool CPlayerData::GoByMovePath(int iPathId, int iPointId, int iType, int iMode, float fRadius, bool bSetAngle, float fSpeed, float fDistOffset)
{
	CVector *vecPos = pServer->GetMovePath()->GetPoint(iPathId, 0);
	if (GoTo(*vecPos, iType, iMode, fRadius, bSetAngle, fSpeed, fDistOffset)) {
		m_iMovePath = iPathId;
		m_iMovePoint = iPointId;
		return true;
	}
	return false;
}

void CPlayerData::UpdateMovingData(CVector vecDestination, float fRadius, bool bSetAngle, float fSpeed, float fDistOffset)
{
	if (fRadius != 0.0f) {
		vecDestination -= CVector(CUtils::RandomFloat(-fRadius, fRadius), CUtils::RandomFloat(-fRadius, fRadius), 0.0);
	}

	CVector vecPosition;
	GetPosition(&vecPosition);

	float fDistance = CMath::GetDistanceBetween3DPoints(vecPosition, vecDestination);

	CVector vecFront = CVector();
	if (fDistance != 0.0f) {
		vecFront = (vecDestination - vecPosition) / fDistance;
	}

	float fAngle = CMath::GetAngle(vecFront.fX, vecFront.fY);

	if (fDistOffset != 0.0f) {
		fDistance += fDistOffset;
		CMath::GetCoordsInFront(vecPosition.fX, vecPosition.fY, fAngle, fDistance, vecDestination.fX, vecDestination.fY);

		if (fDistance != 0.0f) {
			vecFront = (vecDestination - vecPosition) / fDistance;
		}
	}

	if (bSetAngle) {
		SetAngle(fAngle);
	}

	// Set the moving velocity
	vecFront *= (fSpeed / 100.0f); // Step per 1ms
	SetVelocity(vecFront);
	// Calculate the moving time
	if (vecFront.Length() != 0.0f) {
		m_dwMoveTime = static_cast<DWORD>(fDistance / vecFront.Length());
	} else {
		m_dwMoveTime = 0;
	}
	// Get the start move tick
	m_dwMoveTickCount =
		m_dwMoveStartTime = GetTickCount();
	// Save the flags
	m_vecDestination = vecDestination;
	m_fMoveRadius = fRadius;
	m_bMoveSetAngle = bSetAngle;
	m_fMoveSpeed = fSpeed;
	m_fDistOffset = fDistOffset;
}

void CPlayerData::GetDestination(CVector *pvecDestination)
{
	*pvecDestination = m_vecDestination;
}

void CPlayerData::StopMoving()
{
	// Make sure the player is moving
	if (!m_bMoving) {
		return;
	}

	// Reset moving flag
	m_bMoving = false;
	m_wMoveId = INVALID_PLAYER_ID;
	m_iMovePath = INVALID_MOVEPATH_ID;
	m_iMovePoint = 0;
	m_iMoveMode = MOVE_MODE_AUTO;
	// Reset the player data
	SetVelocity(CVector(0.0f, 0.0f, 0.0f));
	SetTrainSpeed(0.0f);
	if (GetState() == PLAYER_STATE_DRIVER) {
		SetKeys(m_pPlayer->wUDAnalog, m_pPlayer->wLRAnalog, m_pPlayer->dwKeys & ~KEY_SPRINT);
	} else {
		SetKeys(m_pPlayer->wUDAnalog & ~KEY_UP, m_pPlayer->wLRAnalog, m_pPlayer->dwKeys);
	}
	// Reset other moving variables
	m_dwMoveTime = 0;
	m_dwMoveStartTime = 0;
	memset(&m_vecDestination, 0, sizeof(CVector));
}

bool CPlayerData::IsMoving()
{
	return m_bMoving;
}

bool CPlayerData::IsMovingAtPlayer(WORD wPlayerId)
{
	return m_bMoving && m_wMoveId != INVALID_PLAYER_ID && m_wMoveId == wPlayerId;
}

bool CPlayerData::IsMovingByMovePath(int iMovePath)
{
	return m_bMoving && m_iMovePath != INVALID_MOVEPATH_ID && m_iMovePath == iMovePath;
}

int CPlayerData::GetMovingType()
{
	return m_iMoveType;
}

void CPlayerData::ToggleReloading(bool bToggle)
{
	m_bHasReload = bToggle;
}

void CPlayerData::ToggleInfiniteAmmo(bool bToggle)
{
	m_bHasInfiniteAmmo = bToggle;
}

void CPlayerData::AimAt(CVector vecPoint, bool bShoot, int iShootDelay, bool bSetAngle, CVector vecOffsetFrom)
{
	if (m_bMoving && m_iMoveType == MOVE_TYPE_SPRINT) {
		return;
	}

	// Set the aiming flag
	if (!m_bAiming) {
		// Get the shooting start tick
		m_dwShootTickCount = GetTickCount();
		m_bReloading = false;
	}

	// Update aiming data
	m_vecAimOffsetFrom = vecOffsetFrom;
	UpdateAimingData(vecPoint, bSetAngle);

	// Set keys
	if (!m_bAiming) {
		m_bAiming = true;
		SetKeys(m_pPlayer->wUDAnalog, m_pPlayer->wLRAnalog, m_pPlayer->dwKeys | KEY_AIM);
	}

	// set the shoot delay
	DWORD dwUpdateRate = pServer->GetUpdateRate();
	if (iShootDelay <= static_cast<int>(dwUpdateRate)) {
		m_dwShootDelay = dwUpdateRate + 5;
	} else {
		m_dwShootDelay = static_cast<DWORD>(iShootDelay);
	}

	// set the shooting flag
	m_bShooting = bShoot;
}

void CPlayerData::AimAtPlayer(WORD wHitId, bool bShoot, int iShootDelay, bool bSetAngle, CVector vecOffset, CVector vecOffsetFrom)
{
	CPlayer *pPlayer = pNetGame->pPlayerPool->pPlayer[wHitId];
	AimAt(pPlayer->vecPosition + vecOffset, bShoot, iShootDelay, bSetAngle, vecOffsetFrom);
	m_wHitId = wHitId;
	m_byteHitType = BULLET_HIT_TYPE_PLAYER;
	m_vecAimOffset = vecOffset;
}

void CPlayerData::UpdateAimingData(CVector vecPoint, bool bSetAngle)
{
	// Adjust the player position
	CVector vecPosition = m_pPlayer->vecPosition + m_vecAimOffsetFrom;
	// Get the aiming distance
	CVector vecDistance = vecPoint - vecPosition;
	// Get the distance to the destination point
	float fDistance = CMath::GetDistanceBetween3DPoints(vecPosition, vecPoint);
	// Calculate the aiming Z angle
	float fXSquare = vecDistance.fX * vecDistance.fX;
	float fYSquare = vecDistance.fY * vecDistance.fY;
	float fZSquare = vecDistance.fZ * vecDistance.fZ;
	float fZAngle = (fXSquare + fYSquare) / (sqrt(fXSquare + fYSquare + fZSquare) * sqrt(fXSquare + fYSquare));
	if (fZAngle > 1.0) {
		fZAngle = 1.0;
	} else if (fZAngle < -1.0) {
		fZAngle = -1.0;
	}
	if (vecDistance.fZ < 0) {
		fZAngle = acos(fZAngle);
	} else {
		fZAngle = -acos(fZAngle);
	}

	// Get the destination angle
	vecDistance /= fDistance;

	if (bSetAngle) {
		SetAngle(CMath::GetAngle(vecDistance.fX, vecDistance.fY));
	}

	// Set the aim sync data
	m_pPlayer->aimSyncData.fZAim = fZAngle;
	m_pPlayer->aimSyncData.vecFront = vecDistance;
	m_pPlayer->aimSyncData.vecPosition = vecPosition;

	// set the flags
	m_vecAimAt = vecPoint;
	m_bAimSetAngle = bSetAngle;
}

void CPlayerData::StopAim()
{
	// Make sure the player is aiming
	if (!m_bAiming) {
		return;
	}

	if (m_bReloading) {
		m_wAmmoInClip = static_cast<WORD>(GetWeaponActualClipSize(m_byteWeaponId));
	}

	// Reset aiming flags
	m_bAiming = false;
	m_bReloading = false;
	m_bShooting = false;
	m_wHitId = INVALID_PLAYER_ID;
	m_bAimSetAngle = false;
	m_byteHitType = BULLET_HIT_TYPE_NONE;
	// Reset keys
	SetKeys(m_pPlayer->wUDAnalog, m_pPlayer->wLRAnalog, m_pPlayer->dwKeys & ~(KEY_AIM | KEY_FIRE));
}

bool CPlayerData::MeleeAttack(int iTime, bool bUseFightstyle)
{
	// Make sure the player is not melee attacking
	if (m_bMeleeAttack) {
		return false;
	}

	// Validate the player state
	if (GetState() != PLAYER_STATE_ONFOOT) {
		return false;
	}

	// Validate the player weapon
	if (GetWeaponType(m_byteWeaponId) != WEAPON_TYPE_MELEE) {
		return false;
	}

	// Set the attacking delay
	if (iTime == -1) {
		m_dwMeleeDelay = GetWeaponShootTime(m_byteWeaponId);
	} else {
		m_dwMeleeDelay = static_cast<DWORD>(iTime);
	}

	if (m_dwMeleeDelay <= pServer->GetUpdateRate()) {
		m_dwMeleeDelay = pServer->GetUpdateRate() + 5;
	}

	// Get the starting tick
	m_dwShootTickCount = GetTickCount();
	// Set the melee flag
	m_bMeleeAttack = true;
	// Set the melee use fightstyle flag
	m_bMeleeFightstyle = bUseFightstyle;
	// Set the melee keys
	SetKeys(m_pPlayer->wUDAnalog, m_pPlayer->wLRAnalog, m_pPlayer->dwKeys | (m_bMeleeFightstyle ? KEY_AIM | KEY_SECONDARY_ATTACK : KEY_FIRE));
	return true;
}

void CPlayerData::StopAttack()
{
	// Make sure the player is attacking
	if (!m_bMeleeAttack) {
		return;
	}

	// Reset keys
	SetKeys(m_pPlayer->wUDAnalog, m_pPlayer->wLRAnalog, m_pPlayer->dwKeys & ~(m_bMeleeFightstyle ? KEY_AIM | KEY_SECONDARY_ATTACK : KEY_FIRE));
	// Reset attacking flag
	m_bMeleeAttack = false;
	m_bMeleeFightstyle = false;
}

bool CPlayerData::IsAttacking()
{
	return m_bMeleeAttack;
}

bool CPlayerData::IsAiming()
{
	return m_bAiming;
}

bool CPlayerData::IsAimingAtPlayer(WORD wPlayerId)
{
	return m_bAiming && m_byteHitType == BULLET_HIT_TYPE_PLAYER && m_wHitId == wPlayerId;
}

WORD CPlayerData::GetAimingPlayer()
{
	if (m_bAiming && m_byteHitType == BULLET_HIT_TYPE_PLAYER) {
		return m_wHitId;
	}
	return INVALID_PLAYER_ID;
}

bool CPlayerData::IsShooting()
{
	return m_bAiming && m_bShooting;
}

bool CPlayerData::IsReloading()
{
	return m_bReloading;
}

void CPlayerData::ProcessDamage(WORD wDamagerId, float fHealthLoss, BYTE byteWeaponId, int iBodypart)
{
	// Call the on take damage callback
	int iReturn = CCallbackManager::OnTakeDamage(m_wPlayerId, wDamagerId, byteWeaponId, iBodypart, fHealthLoss);
	// Check the returned value
	if (iReturn) {
		// Check the armour
		if (GetArmour() > 0.0f) {
			// Save the old armour
			float fArmour = GetArmour();
			// Decrease the armor
			SetArmour(fArmour - fHealthLoss);
			// If the damage is bigger than the armour then decrease the health aswell
			if (fArmour - fHealthLoss < 0.0f) {
				SetHealth(GetHealth() - (fHealthLoss - fArmour));
			}
		} else {
			SetHealth(GetHealth() - fHealthLoss);
		}
	}
	// Save the last damager
	m_wLastDamagerId = wDamagerId;
	m_byteLastDamagerWeapon = byteWeaponId;
}

void CPlayerData::ProcessVehicleDamage(WORD wDamagerId, WORD wVehicleId, BYTE byteWeaponId, CVector vecHit)
{
	int iReturn = CCallbackManager::OnVehicleTakeDamage(m_wPlayerId, wDamagerId, wVehicleId, byteWeaponId, vecHit);

	if (iReturn) {
		CVehicle *pVehicle = GetVehicle();

		if (!pVehicle) {
			return;
		}

		float fHealth = GetVehicleHealth();
		SWeaponInfo sWeaponInfo = CWeaponInfo::GetDefaultInfo(byteWeaponId);

		if (fHealth > 0.0f) {
			fHealth -= sWeaponInfo.fDamage;

			if (fHealth < 0.0f) {
				fHealth = 0.0f;
			}
		}

		SetVehicleHealth(fHealth);

		if (fHealth < 250.0f) {
			pVehicle->wKillerID = wDamagerId;
		}
	}
}

void CPlayerData::ProcessStreamIn(WORD wForPlayerId)
{
	CCallbackManager::OnStreamIn(m_wPlayerId, wForPlayerId);

	if (GetState() == PLAYER_STATE_WASTED) {
		RakNet::BitStream bsData;
		bsData.Write(m_wPlayerId);
		CFunctions::PlayerRPC(&RPC_WorldPlayerDeath, &bsData, wForPlayerId);
	}
}

void CPlayerData::ProcessStreamOut(WORD wForPlayerId)
{
	CCallbackManager::OnStreamOut(m_wPlayerId, wForPlayerId);
}

bool CPlayerData::EnterVehicle(WORD wVehicleId, BYTE byteSeatId, int iType)
{
	// Validate the player state
	if (GetState() != PLAYER_STATE_ONFOOT) {
		return false;
	}

	// Validate the entering type
	if (iType > MOVE_TYPE_SPRINT || iType < MOVE_TYPE_WALK) {
		iType = MOVE_TYPE_RUN;
	}

	// Validate the vehicle
	CVehicle *pVehicle = pNetGame->pVehiclePool->pVehicle[wVehicleId];
	if (!pVehicle) {
		return false;
	}

	// Validate the seat id
	if (!CVehicleInfo::IsValidPassengerSeat(byteSeatId, static_cast<WORD>(pVehicle->customSpawn.dwModelID))) {
		return false;
	}

	// Validate the distance to enter
	CVector vecDestination = pServer->GetVehicleSeatPos(pVehicle, byteSeatId);

	float fDistance = CMath::GetDistanceBetween3DPoints(m_pPlayer->vecPosition, vecDestination);
	if (fDistance > MAX_DISTANCE_TO_ENTER_VEHICLE) {
		return false;
	}

	// Save the entering stats
	m_wVehicleToEnter = wVehicleId;
	m_byteSeatToEnter = byteSeatId;

	// Check distance
	if (fDistance < 0.5f) {
		// Wait until the entry animation is finished
		m_dwEnterExitTickCount = GetTickCount();
		m_bEntering = true;

		// Check whether the player is jacking the vehicle or not
		if (pServer->IsVehicleSeatOccupied(m_wPlayerId, m_wVehicleToEnter, m_byteSeatToEnter)) {
			m_bJacking = true;
		}

		// Call the SAMP enter vehicle function
		CFunctions::PlayerEnterVehicle(m_pPlayer, m_wVehicleToEnter, m_byteSeatToEnter);
	} else {
		// Go to the vehicle
		GoTo(vecDestination, iType, true);
	}

	return true;
}

bool CPlayerData::ExitVehicle()
{
	// Validate the player state
	if (GetState() != PLAYER_STATE_DRIVER && GetState() != PLAYER_STATE_PASSENGER) {
		return false;
	}

	// Validate the player vehicle
	if (!GetVehicle()) {
		return false;
	}

	// Call the SAMP exit vehicle function
	CFunctions::PlayerExitVehicle(m_pPlayer, m_pPlayer->wVehicleId);
	// Set the player state
	SetState(PLAYER_STATE_EXIT_VEHICLE);
	// Set the exit start tick
	m_dwEnterExitTickCount = GetTickCount();
	return true;
}

bool CPlayerData::PutInVehicle(WORD wVehicleId, BYTE byteSeatId)
{
	// Validate the player state
	if (GetState() != PLAYER_STATE_ONFOOT) {
		ShowForStreamedPlayers();
	}

	// Validate the vehicle
	CVehicle *pVehicle = pNetGame->pVehiclePool->pVehicle[wVehicleId];
	if (!pVehicle) {
		return false;
	}

	// Validate the seat id
	if (!CVehicleInfo::IsValidPassengerSeat(byteSeatId, static_cast<WORD>(pVehicle->customSpawn.dwModelID))) {
		return false;
	}

	// Set the player params
	SetVehicle(wVehicleId, byteSeatId);
	SetPosition(pServer->GetVehiclePos(pVehicle));
	SetState(byteSeatId == 0 ? PLAYER_STATE_DRIVER : PLAYER_STATE_PASSENGER);
	SetAngle(pServer->GetVehicleAngle(pVehicle));
	return true;
}

bool CPlayerData::RemoveFromVehicle()
{
	// Validate the player vehicle
	if (!GetVehicle()) {
		return false;
	}

	// Set the player state
	SetState(PLAYER_STATE_ONFOOT);

	// Get the vehicle
	CVehicle *pVehicle = GetVehicle();

	// Set his position
	SetPosition(pServer->GetVehicleSeatPos(pVehicle, m_pPlayer->byteSeatId));

	// Reset the player vehicle and seat id
	SetVehicle(INVALID_VEHICLE_ID, 0);
	return true;
}

bool CPlayerData::IsInVehicle()
{
	return m_pPlayer->wVehicleId == INVALID_VEHICLE_ID;
}

WORD CPlayerData::GetVehicleId()
{
	return m_pPlayer->wVehicleId;
}

BYTE CPlayerData::GetSeatId()
{
	return m_pPlayer->byteSeatId;
}

void CPlayerData::SetVehicle(WORD wVehicleId, BYTE byteSeatId)
{
	// set the player params
	m_pPlayer->wVehicleId = wVehicleId;
	m_pPlayer->byteSeatId = byteSeatId;
	m_dwVehicleDeadTick = 0;
	if (byteSeatId == 0) {
		SetWeaponState(WEAPONSTATE_UNKNOWN);
	}

	if (wVehicleId == INVALID_VEHICLE_ID && m_pPlayer->wVehicleId != INVALID_VEHICLE_ID) {
		CVehicle *pVehicle = GetVehicle();
		pVehicle->bOccupied = false;
		pVehicle->vehActive = false;
	}

	CVehicle *pVehicle = GetVehicle();
	if (!pVehicle) {
		return;
	}

	// set the vehicle params
	pVehicle->wLastDriverID = m_wPlayerId;
	pVehicle->bOccupied = true;
	pVehicle->vehOccupiedTick = GetTickCount();
	pVehicle->vehActive = true;
}

CVehicle *CPlayerData::GetVehicle()
{
	if (m_pPlayer->wVehicleId < 1 || m_pPlayer->wVehicleId > MAX_VEHICLES) {
		return NULL;
	}

	CVehicle *pVehicle = pNetGame->pVehiclePool->pVehicle[m_pPlayer->wVehicleId];
	if (!pVehicle) {
		return NULL;
	}

	return pVehicle;
}

void CPlayerData::SetVehicleSiren(bool bState)
{
	m_pPlayer->vehicleSyncData.byteSirenState = bState ? 1 : 0;
}

bool CPlayerData::IsVehicleSiren()
{
	return m_pPlayer->vehicleSyncData.byteSirenState != 0;
}

void CPlayerData::SetVehicleHealth(float fHealth)
{
	// Validate the health
	if (fHealth < 0.0f) {
		fHealth = 0.0f;
	}

	CVehicle *pVehicle = GetVehicle();
	if (pVehicle) {
		pVehicle->fHealth = fHealth;
	}

	m_pPlayer->vehicleSyncData.fHealth = fHealth;
}

float CPlayerData::GetVehicleHealth()
{
	return m_pPlayer->vehicleSyncData.fHealth;
}

void CPlayerData::SetVehicleHydraThrusters(WORD wDirection)
{
	m_wHydraThrustAngle[0] =
		m_wHydraThrustAngle[1] = wDirection;
}

WORD CPlayerData::GetVehicleHydraThrusters()
{
	return m_wHydraThrustAngle[0];
}

void CPlayerData::SetTrainSpeed(float fTrainSpeed)
{
	m_fTrainSpeed = fTrainSpeed;
}

float CPlayerData::GetTrainSpeed()
{
	return m_fTrainSpeed;
}

void CPlayerData::SetVehicleGearState(BYTE byteGearState)
{
	m_byteGearState = byteGearState;
}

BYTE CPlayerData::GetVehicleGearState()
{
	return m_byteGearState;
}

void CPlayerData::SetSurfingOffsets(CVector vecOffsets)
{
	m_vecSurfing = vecOffsets;
}

void CPlayerData::GetSurfingOffsets(CVector *vecOffsets)
{
	*vecOffsets = m_vecSurfing;
}

void CPlayerData::SetSurfingVehicle(WORD wVehicleId)
{
	m_wSurfingInfo = wVehicleId;
}

int CPlayerData::GetSurfingVehicle()
{
	return m_wSurfingInfo;
}

void CPlayerData::SetSurfingObject(WORD wObjectId)
{
	m_wSurfingInfo = MAX_VEHICLES + wObjectId;
	pNetGame->pObjectPool->bObjectSlotState[wObjectId] = true;
}

int CPlayerData::GetSurfingObject()
{
	WORD wObjectId = m_wSurfingInfo - MAX_VEHICLES;
	if (wObjectId > 0 && wObjectId < MAX_OBJECTS) {
		if (pNetGame->pObjectPool->bObjectSlotState[wObjectId]) {
			return wObjectId;
		}
	}
	return INVALID_OBJECT_ID;
}

void CPlayerData::SetSurfingPlayerObject(WORD wObjectId)
{
	m_wSurfingInfo = MAX_VEHICLES + wObjectId;
	pNetGame->pObjectPool->bPlayerObjectSlotState[m_wPlayerId][wObjectId] = true;
}

int CPlayerData::GetSurfingPlayerObject()
{
	WORD wObjectId = m_wSurfingInfo - MAX_VEHICLES;
	if (wObjectId > 0 && wObjectId < MAX_OBJECTS) {
		if (pNetGame->pObjectPool->bPlayerObjectSlotState[m_wPlayerId][wObjectId]) {
			return wObjectId;
		}
	}
	return INVALID_OBJECT_ID;
}

void CPlayerData::StopSurfing()
{
	WORD wObjectId = m_wSurfingInfo - MAX_VEHICLES;
	if (wObjectId > 0 && wObjectId < MAX_OBJECTS) {
		pNetGame->pObjectPool->bObjectSlotState[wObjectId] = false;
		pNetGame->pObjectPool->bPlayerObjectSlotState[m_wPlayerId][wObjectId] = false;
	}

	m_wSurfingInfo = 0;
	m_vecSurfing = CVector(0.0f, 0.0f, 0.0f);
}

bool CPlayerData::StartPlayingPlayback(char *szFile, int iRecordId, bool bAutoUnload, CVector vecPoint, float *fQuaternion)
{
	// Make sure the player is not already Playing
	if (m_bPlaying) {
		return false;
	}

	// Create a new playback instance
	if (iRecordId != INVALID_RECORD_ID) {
		m_pPlayback = new CPlayback(iRecordId, bAutoUnload);
	} else if (szFile) {
		m_pPlayback = new CPlayback(szFile, m_szPlayingPath, bAutoUnload);
	}

	// Initialize it
	if (!m_pPlayback) {
		return false;
	}

	if (!m_pPlayback->Initialize(vecPoint, fQuaternion)) {
		SAFE_DELETE(m_pPlayback);
		return false;
	}

	// Mark as Playing
	m_bPlaying = true;
	return true;
}

void CPlayerData::StopPlayingPlayback()
{
	// Make sure the player is Playing
	if (!m_bPlaying) {
		return;
	}

	// Delete the playback instance
	SAFE_DELETE(m_pPlayback);
	// Reset the player data
	SetVelocity(CVector(0.0f, 0.0f, 0.0f));
	SetKeys(KEY_NONE, KEY_NONE, KEY_NONE);
	SetTrainSpeed(0.0f);
	// Reset the Playing flag
	m_bPlaying = false;
	// Call the playback finish callback
	CCallbackManager::OnFinishPlayback(m_wPlayerId);
}

void CPlayerData::PausePlayingPlayback()
{
	// Make sure the player is Playing
	if (!m_bPlaying) {
		return;
	}

	// Pause the playback
	m_pPlayback->SetPaused(true);
}

void CPlayerData::ResumePlayingPlayback()
{
	// Make sure the player is Playing
	if (!m_bPlaying) {
		return;
	}

	// Resume the playback
	m_pPlayback->SetPaused(false);
}

void CPlayerData::SetPlayingPlaybackPath(char *szFile)
{
	strlcpy(m_szPlayingPath, szFile, sizeof(m_szPlayingPath));
}

void CPlayerData::GetPlayingPlaybackPath(char *szFile, size_t size)
{
	strlcpy(szFile, m_szPlayingPath, size);
}

bool CPlayerData::PlayNode(int iNodeId, int iMoveType, int iMode, float fRadius, bool bSetAngle, float fSpeed)
{
	// Stop the player playback if he's playing one
	if (m_bPlaying) {
		StopPlayingPlayback();
	}

	// Stop the last node if any node exists
	if (m_bPlayingNode) {
		StopPlayingNode();
	}

	// save data
	m_iNodeMoveType = iMoveType;
	m_iNodeMoveMode = iMode;
	m_fNodeMoveRadius = fRadius;
	m_bNodeMoveSetAngle = bSetAngle;
	m_fNodeMoveSpeed = fSpeed;

	// Get the node instance
	m_pNode = pServer->GetNodeManager()->GetAt(iNodeId);

	CVector vecPos;
	m_pNode->GetPosition(&vecPos);
	SetPosition(vecPos);

	m_pNode->SetLink(m_pNode->GetLinkId());
	m_wNodePoint = m_pNode->GetLinkPoint();
	m_wNodeLastPoint = m_pNode->GetPointId();
	m_bPlayingNode = true;

	UpdateNodePoint(m_wNodePoint);

	m_pNode->GetPosition(&vecPos);
	GoTo(vecPos, iMoveType, iMode, fRadius, bSetAngle, fSpeed);
	return true;
}

void CPlayerData::StopPlayingNode()
{
	// Make sure the player is playing a node
	if (!m_bPlayingNode) {
		return;
	}

	// Reset the node instance
	m_pNode = NULL;
	// Reset the player data
	StopMoving();
	SetKeys(KEY_NONE, KEY_NONE, KEY_NONE);
	// Reset the node flag
	m_bPlayingNode = false;
	m_bIsPlayingNodePaused = false;
	m_wNodePoint = 0;
	m_wNodeLastPoint = 0;
	m_iNodeMoveType = MOVE_TYPE_AUTO;
	m_iNodeMoveMode = MOVE_MODE_AUTO;
	m_fNodeMoveRadius = 0.0f;
	m_bNodeMoveSetAngle = true;
	m_fNodeMoveSpeed = MOVE_SPEED_AUTO;
	m_vecNodeLastPos = CVector();
	// Call the node finish callback
	CCallbackManager::OnFinishNode(m_wPlayerId);
}

void CPlayerData::PausePlayingNode()
{
	if (!m_bPlayingNode || m_bIsPlayingNodePaused) {
		return;
	}

	StopMoving();
	GetPosition(&m_vecNodeLastPos);

	m_bIsPlayingNodePaused = true;
}

void CPlayerData::ResumePlayingNode()
{
	if (!m_bPlayingNode || !m_bIsPlayingNodePaused) {
		return;
	}

	m_bIsPlayingNodePaused = false;
	GoTo(m_vecNodeLastPos, m_iNodeMoveType, m_iNodeMoveMode, m_fNodeMoveRadius, m_bNodeMoveSetAngle, m_fNodeMoveSpeed);
}

bool CPlayerData::IsPlayingNodePaused()
{
	return m_bIsPlayingNodePaused;
}

bool CPlayerData::IsPlayingNode()
{
	return m_bPlayingNode;
}

WORD CPlayerData::ChangeNode(int iNodeId, WORD wLinkId)
{
	// Make sure the player is playing a node
	if (!m_bPlayingNode) {
		return 0;
	}

	// Get the node instance
	m_pNode = pServer->GetNodeManager()->GetAt(iNodeId);
	// Process the node change
	return m_pNode->ProcessNodeChange(this, wLinkId);
}

bool CPlayerData::UpdateNodePoint(WORD wPointId)
{
	if (!m_bPlayingNode) {
		return false;
	}

	CVector vecPosition;

	m_pNode->SetPoint(wPointId);
	m_pNode->GetPosition(&vecPosition);

	UpdateMovingData(vecPosition, m_fMoveRadius, m_bMoveSetAngle, m_fMoveSpeed, m_fDistOffset);
	return true;
}

void CPlayerData::SetMoveMode(int iMoveMode)
{
	if (iMoveMode == MOVE_MODE_AUTO) {
		if (pServer->IsMapAndreasInited()) {
			m_iMoveMode = MOVE_MODE_MAPANDREAS;
			m_iNodeMoveMode = MOVE_MODE_MAPANDREAS;
		} else {
			m_iMoveMode = MOVE_MODE_NONE;
			m_iNodeMoveMode = MOVE_MODE_NONE;
		}
	}
}

int CPlayerData::GetMoveMode()
{
	return m_iMoveMode;
}

void CPlayerData::SetMinHeightPosCall(float fHeight)
{
	m_fMinHeightPos = fHeight;
}

float CPlayerData::GetMinHeightPosCall()
{
	return m_fMinHeightPos;
}

CPlayer *CPlayerData::GetInterface()
{
	return m_pPlayer;
}