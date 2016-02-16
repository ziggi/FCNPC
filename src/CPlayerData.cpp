/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: Player.cpp
	- Author(s): OrMisicL

  =========================================*/


#include "Main.h"

struct CPlayer;

extern CServer  *pServer;
extern CNetGame *pNetGame;

CPlayerData::CPlayerData(WORD playerId, char *szName)
{
	// Save the player id
	m_playerId = playerId;
	// Save player name
	SetName(szName);
	// Reset variables
	m_vecDestination = CVector();
	m_vecNodeVelocity = CVector();
	m_vecAimAt = CVector();
	m_bSetup = false;
	m_bSpawned = false;
	m_bMoving = false;
	m_bUseZMap = false;
	m_bAiming = false;
	m_bReloading = false;
	m_bEntering = false;
	m_bJacking = false;
	m_bExiting = false;
	m_bHasReload = true;
	m_bHasInfiniteAmmo = false;
	m_bPlaying = false;
	m_bPlayingNode = false;
	m_bMeleeAttack = false;
	m_bMeleeFightstyle = false;
	m_byteWeaponId = 0;
	m_wAmmo = 0;
	m_iNodePoint = 0;
	m_iNodeLastPoint = 0;
	m_iLastDamager = INVALID_ENTITY_ID;
	m_wVehicleToEnter = INVALID_ENTITY_ID;
	m_byteSeatToEnter = 0;
	m_wHitId = INVALID_ENTITY_ID;
	m_byteHitType = BULLET_HIT_TYPE_NONE;
	m_vecSurfing = CVector();
	m_wSurfingInfo = 0;
}

CPlayerData::~CPlayerData()
{

}

int CPlayerData::GetId()
{
	return (int)m_playerId;
}

void CPlayerData::Destroy()
{
	// Stop Playing any playback
	if(m_bPlaying)
		StopPlayingPlayback();

	// Stop playing any node
	if(m_bPlayingNode)
		StopPlayingNode();

	// Set the player state to none
	SetState(PLAYER_STATE_NONE);
	// Remove the player from the SAMP playerpool
	CSAMPFunctions::DeletePlayer(m_playerId);
	// Reset the setup flag
	m_bSetup = false;
}

bool CPlayerData::Setup()
{
	if(m_playerId == INVALID_ENTITY_ID)
		return false;

	// Get the player interface
	m_pPlayer = pNetGame->pPlayerPool->pPlayer[m_playerId];
	// Validate the interface
	if(!m_pPlayer)
		return false;

	// Set the setup flag
	m_bSetup = true;
	// Reset his state
	SetState(PLAYER_STATE_NONE);
	// Initialize the update tick
	m_dwUpdateTick = GetTickCount();
	return true;
}

bool CPlayerData::Spawn(int iSkinId)
{
	// Make sure the player is not already spawned
	if(m_bSpawned)
		return false;

	// Mark spawned
	m_bSpawned = true;
	m_pPlayer->bReadyToSpawn = TRUE;
	// Set the player skin
	m_pPlayer->spawn.iSkin = iSkinId;
	// Call the SAMP spawn functions
	CSAMPFunctions::SpawnPlayer((int)m_playerId);
	// Set the player stats
	SetHealth(100.0f);
	SetArmour(0.0f);
	SetSpecialAction(0);
	SetWeapon(0);
	SetAmmo(0);
	// Set the player state onfoot
	SetState(PLAYER_STATE_ONFOOT);
	// Reset stats
	m_iLastDamager = INVALID_ENTITY_ID;
	m_pPlayer->wVehicleId = INVALID_ENTITY_ID;
	m_pPlayer->byteSeatId = 0;
	// Call the NPC spawn callback
	CCallbackManager::OnSpawn((int)m_playerId);
	return true;
}

bool CPlayerData::Respawn()
{
	// Make sure the player is already spawned
	if(!m_bSpawned)
		return false;

	// Pause Playing any playback
	if(m_bPlaying)
		m_pPlayback->SetPaused(true);

	// Get the last player stats
	CVector vecPosition = m_pPlayer->vecPosition;
	float fHealth = m_pPlayer->fHealth;
	float fArmour = m_pPlayer->fArmour;
	WORD wVehicleId = m_pPlayer->wVehicleId;
	BYTE byteSeat = m_pPlayer->byteSeatId;
	BYTE byteSpecialAction = m_pPlayer->syncData.byteSpecialAction;
	// Call the SAMP spawn functions
	m_pPlayer->bReadyToSpawn = TRUE;
	CSAMPFunctions::SpawnPlayer((int)m_playerId);
	// Set the player state onfoot
	SetState(PLAYER_STATE_ONFOOT);
	// Reset vehicle and seat id
	m_pPlayer->wVehicleId = INVALID_ENTITY_ID;
	m_pPlayer->byteSeatId = 0;
	// Set the player stats back
	if(fHealth == 0.0f)
	{
		SetHealth(100.0f);
		SetArmour(0.0f);
	}
	else
	{
		SetHealth(fHealth);
		SetArmour(fArmour);
	}
	SetPosition(vecPosition);
	SetSpecialAction(byteSpecialAction);
	if(wVehicleId != INVALID_ENTITY_ID)
		PutInVehicle(wVehicleId, byteSeat);

	// Resume any playback
	if(m_bPlaying)
		m_pPlayback->SetPaused(false);

	// Reset stats
	m_iLastDamager = INVALID_ENTITY_ID;
	// Call the NPC spawn callback
	CCallbackManager::OnRespawn((int)m_playerId);
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
	if(iState < UPDATE_STATE_NONE || iState > UPDATE_STATE_PASSENGER)
		return;

	// Set the new update state
	m_pPlayer->iUpdateState = iState;
}

void CPlayerData::Update(int iState)
{
	// Validate the player
	if(!m_bSetup || !m_bSpawned)
		return;

	// Make sure the state is valid
	if(iState < UPDATE_STATE_NONE || iState > UPDATE_STATE_PASSENGER)
		return;

	// Check the time spent since the last update
	if((GetTickCount() - m_dwUpdateTick) < pServer->GetUpdateRate())
		return;

	// Update onfoot state
	if(iState == UPDATE_STATE_ONFOOT && (GetState() == PLAYER_STATE_ONFOOT || GetState() == PLAYER_STATE_ENTER_VEHICLE_DRIVER || GetState() == PLAYER_STATE_ENTER_VEHICLE_PASSENGER))
	{
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
		m_pPlayer->syncData.byteHealth = (BYTE)m_pPlayer->fHealth;
		m_pPlayer->syncData.byteArmour = (BYTE)m_pPlayer->fArmour;
		// Set the sync keys
		m_pPlayer->syncData.wUDAnalog = m_pPlayer->wUDAnalog;
		m_pPlayer->syncData.wLRAnalog = m_pPlayer->wLRAnalog;
		m_pPlayer->syncData.wKeys = (WORD)m_pPlayer->dwKeys;
		// Update aim data
		UpdateAim();
		// Set the new update state
		m_pPlayer->iUpdateState = iState;
	}
	// Update driver state
	else if(iState == UPDATE_STATE_DRIVER && GetState() == PLAYER_STATE_DRIVER)
	{
		// Dont process if we dont have any vehicle
		if(m_pPlayer->wVehicleId == INVALID_ENTITY_ID)
			return;

		// Get the player vehicle interface
		CVehicle *pVehicle = pNetGame->pVehiclePool->pVehicle[m_pPlayer->wVehicleId];
		// Set the player sync vehicle id
		m_pPlayer->vehicleSyncData.wVehicleId = m_pPlayer->wVehicleId;
		// Set the player sync position
		m_pPlayer->vehicleSyncData.vecPosition = pVehicle->vecPosition;
		// Set the player velocity
		m_pPlayer->vehicleSyncData.vecVelocity = m_pPlayer->vecVelocity;
		// Set the player keys
		m_pPlayer->vehicleSyncData.wUDAnalog = m_pPlayer->wUDAnalog;
		m_pPlayer->vehicleSyncData.wLRAnalog = m_pPlayer->wLRAnalog;
		m_pPlayer->vehicleSyncData.wKeys = (WORD)m_pPlayer->dwKeys;
		// Set the player health and armour
		m_pPlayer->vehicleSyncData.fHealth = pVehicle->fHealth;
		m_pPlayer->vehicleSyncData.bytePlayerHealth = (BYTE)m_pPlayer->fHealth;
		m_pPlayer->vehicleSyncData.bytePlayerArmour = (BYTE)m_pPlayer->fArmour;
		// Set the player weapon
		m_pPlayer->vehicleSyncData.bytePlayerWeapon = m_byteWeaponId;
		// Set the new update state
		m_pPlayer->iUpdateState = iState;
	}
	// Update passenger state
	else if(iState == UPDATE_STATE_PASSENGER && GetState() == PLAYER_STATE_PASSENGER)
	{
		// Dont process if we dont have any vehicle
		if(m_pPlayer->wVehicleId == INVALID_ENTITY_ID)
			return;

		// Get the player vehicle interface
		CVehicle *pVehicle = pNetGame->pVehiclePool->pVehicle[m_pPlayer->wVehicleId];
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
		m_pPlayer->passengerSyncData.wKeys = (WORD)m_pPlayer->dwKeys;
		// Set the player health and armour
		m_pPlayer->passengerSyncData.bytePlayerHealth = (BYTE)m_pPlayer->fHealth;
		m_pPlayer->passengerSyncData.bytePlayerArmour = (BYTE)m_pPlayer->fArmour;
		// Set the player weapon
		m_pPlayer->passengerSyncData.bytePlayerWeapon = m_byteWeaponId;
		// Set the new update state
		m_pPlayer->iUpdateState = iState;
	}
	// Update the tick count
	m_dwUpdateTick = GetTickCount();
}

void CPlayerData::UpdateAim()
{
	if(m_bAiming)
	{
		// Set the camera mode
		m_pPlayer->aimSyncData.byteCameraMode = CWeaponInfo::IsDoubleHanded(m_pPlayer->syncData.byteWeapon) ? 53 : 7;
		if(m_pPlayer->syncData.byteWeapon == 0 || m_pPlayer->syncData.byteWeapon == 1)
			m_pPlayer->aimSyncData.byteCameraMode = 4;
		else if (m_pPlayer->syncData.byteWeapon == 34) // Sniper rifle
			m_pPlayer->aimSyncData.byteCameraMode = 7;

		// Set the weapon state
		m_pPlayer->aimSyncData.byteWeaponState = WS_MORE_BULLETS;
		if (m_bReloading)
			m_pPlayer->aimSyncData.byteWeaponState = WS_RELOADING;
		else if (!m_wAmmo)
			m_pPlayer->aimSyncData.byteWeaponState = WS_NO_BULLETS;

		// Update vector pos
		if (m_byteHitType == BULLET_HIT_TYPE_PLAYER && m_wHitId != INVALID_ENTITY_ID)
		{
			CPlayer *pPlayer = pNetGame->pPlayerPool->pPlayer[m_wHitId];
			if (pPlayer)
			{
				CPlayerData::AimAt(pPlayer->vecPosition, m_pPlayer->dwKeys & 4);
			}
			else
			{
				CPlayerData::StopAim();
			}
		}
	}
	else
	{
		// Set the camera mode and weapon state
		m_pPlayer->aimSyncData.byteCameraMode = 0;
		m_pPlayer->aimSyncData.byteWeaponState = WS_NO_BULLETS;
		// Convert the player angle to radians
		float fAngle = CMath::DegreeToRadians(GetAngle());
		// Calculate the camera target
		CVector vecTarget(m_pPlayer->aimSyncData.vecPosition.fX - sin(fAngle) * 0.2f,
			m_pPlayer->aimSyncData.vecPosition.fY + cos(fAngle) * 0.2f, m_pPlayer->aimSyncData.vecPosition.fZ);
		
		// Calculate the camera front vector
		m_pPlayer->aimSyncData.vecFront = vecTarget - m_pPlayer->aimSyncData.vecPosition;
	}
	// Set the aim sync flag
	m_pPlayer->bHasAimSync = TRUE;
}

bool CPlayerData::IsSpawned()
{
	return m_bSpawned;
}

bool CPlayerData::SetState(BYTE byteState)
{
	// Make sure the state is valid
	if(byteState > PLAYER_STATE_SPAWNED)
		return false;

	// Set the new state
	m_pPlayer->byteState = byteState;
	return true;
}

BYTE CPlayerData::GetState()
{
	return m_pPlayer->byteState;
}

void CPlayerData::Kill(int iKillerId, int iWeapon)
{
	// Stop Playing any playback
	if(m_bPlaying)
		StopPlayingPlayback();

	// Stop playing any node
	if(m_bPlayingNode)
		StopPlayingNode();

	// Stop moving and aiming
	StopMoving();
	StopAim();
	// Kill the NPC
	CSAMPFunctions::KillPlayer(m_playerId, iWeapon, iKillerId);
	// Set the NPC state
	SetState(PLAYER_STATE_WASTED);
	// Call the NPC death callback
	CCallbackManager::OnDeath((int)m_playerId, iKillerId, iWeapon);
}

void CPlayerData::Process()
{
	// Make sure the player is setup
	if(!m_bSetup)
		return;

	// Process Playing
	if(m_bPlaying)
	{
		// Process the player playback
		if(!m_pPlayback->Process(this))
			StopPlayingPlayback();

		return;
	}
	// Update the player depending on his state
	if(GetState() == PLAYER_STATE_ONFOOT)
	{
		// Process the player mouvement
		if(m_bMoving)
		{
			// Get the player position
			CVector vecPosition;
			GetPosition(&vecPosition);
			// Get the player velocity
			CVector vecVelocity;
			GetVelocity(&vecVelocity);
			// Make sure we still need to move
			if((GetTickCount() - m_dwMoveStartTime) < m_dwMoveTime)
			{
				// Get the time spent since the last update
				DWORD dwThisTick = GetTickCount();
				DWORD dwTime = (dwThisTick - m_dwMoveTickCount);
				// Set the new position
				CVector vecNewPosition = vecPosition + (vecVelocity * (float)dwTime);
				// Set the Z ground (is using ZMap)
				if(m_bUseZMap && pServer->GetZMap()->IsInitialized())
					vecNewPosition.fZ = pServer->GetZMap()->GetGroundForCoord(vecNewPosition) + 0.5f;
				
				// Set the position
				SetPosition(vecNewPosition);
				// Update the tick count
				m_dwMoveTickCount = dwThisTick;
			}
			else
			{
				// Get the time spent since the last update
				DWORD dwThisTick = GetTickCount();
				DWORD dwTime = (dwThisTick - m_dwMoveTickCount);
				dwTime -= (dwThisTick - m_dwMoveStartTime) - m_dwMoveTime;
				// Set the new position
				CVector vecNewPosition = vecPosition + (vecVelocity * (float)dwTime);
				// Set the Z ground (is using ZMap)
				if(m_bUseZMap && pServer->GetZMap()->IsInitialized())
					vecNewPosition.fZ = pServer->GetZMap()->GetGroundForCoord(vecNewPosition) + 0.5f;
					
				// Set the position
				SetPosition(vecNewPosition);
				// Stop the player
				StopMoving();
				// Are we entering a vehicle ?
				if(m_wVehicleToEnter != INVALID_ENTITY_ID)
				{
					// Wait until the entry animation is finished
					m_dwEnterExitTickCount = GetTickCount();
					m_bEntering = true;
					// Check whether the player is jacking the vehicle or not
					if(pServer->IsVehicleSeatOccupied((int)m_playerId, (int)m_wVehicleToEnter, (int)m_byteSeatToEnter))
						m_bJacking = true;

					// Call the SAMP enter vehicle function
					CSAMPFunctions::PlayerEnterVehicle((int)m_playerId, (int)m_wVehicleToEnter, (int)m_byteSeatToEnter);
				}
				else
				{
					// Are we playing a node ?
					if(m_bPlayingNode)
					{
						// Check the return value of the point finish callback
						if(CCallbackManager::OnFinishNodePoint((int)m_playerId, m_iNodePoint))
						{
							// Process the next position
							int iNewPoint = m_pNode->Process(this, m_iNodePoint, m_iNodeLastPoint, m_iNodeType, m_vecNodeVelocity);
							// Update the points
							m_iNodeLastPoint = m_iNodePoint;
							m_iNodePoint = iNewPoint;
						}
						else
							StopPlayingNode();
					}
					else
						// Call the reach destination callback
						CCallbackManager::OnReachDestination((int)m_playerId);
				}
			}
		}
		// Are we performing the entry animation ?
		if(m_bEntering)
		{
			// Get the time spent since the last update
			DWORD dwThisTick = GetTickCount();
			DWORD dwTime = (dwThisTick - m_dwEnterExitTickCount);
			if(dwTime > (DWORD)(m_bJacking ? 5800 : 2500))
			{
				// Change the player state
				SetState(m_byteSeatToEnter == 0 ? PLAYER_STATE_DRIVER : PLAYER_STATE_PASSENGER);
				// Reset entering variables
				m_bEntering = false;
				m_bJacking = false;
				// Set the angle
				CVehicle *pVehicle = pNetGame->pVehiclePool->pVehicle[m_wVehicleToEnter];
				SetAngle(CMath::GetAngle(-pVehicle->vehMatrix.up.fX, pVehicle->vehMatrix.up.fY));
				// Call the vehicle entry complete callback
				CCallbackManager::OnVehicleEntryComplete((int)m_playerId, (int)m_wVehicleToEnter, (int)m_byteSeatToEnter);
				// Set the player vehicle and seat
				m_pPlayer->wVehicleId = m_wVehicleToEnter;
				m_pPlayer->byteSeatId = m_byteSeatToEnter;
				// Reset entering values
				m_wVehicleToEnter = INVALID_ENTITY_ID;
				m_byteSeatToEnter = 0;
			}
		}
		// Process player reloading
		if(m_bReloading)
		{
			// Get the time spent since the reload start
			DWORD dwThisTick = GetTickCount();
			DWORD dwTime = (dwThisTick - m_dwReloadTickCount);
			// Have we finished reloading ?
			if (dwTime >= 2500)
			{
				// Reset the reloading flag
				m_bReloading = false;
				// Start shooting again
				SetKeys(m_pPlayer->wUDAnalog, m_pPlayer->wLRAnalog, KEY_FIRE | KEY_AIM);
				// Update the shoot tick
				m_dwShootTickCount = dwThisTick;
			}
		}
		// Process player shooting
		else if(m_bAiming && m_pPlayer->dwKeys & 4)
		{	
			// Do we still have ammo ?
			if (m_wAmmo == 0)
			{
				// Check for infinite ammo flag
				if (!m_bHasInfiniteAmmo)
					// Stop shooting and aim only
					SetKeys(m_pPlayer->wUDAnalog, m_pPlayer->wLRAnalog, KEY_AIM);
				else
					// This is done so the NPC would keep reloading even if the infinite ammo flag is set
					m_wAmmo = 500;
			}
			else
			{
				// Check the time spent since the last shoot
				DWORD dwThisTick = GetTickCount();
				DWORD dwTime = (dwThisTick - m_dwShootTickCount);
				if (dwTime >= CWeaponInfo::GetWeaponRateOfFire(m_byteWeaponId) * 2)
				{
					// Decrease the ammo
					m_wAmmo--;
					// Get the weapon clip size
					DWORD dwClip = 0;
					if (m_byteWeaponId == 38 || m_byteWeaponId == 37 || m_byteWeaponId == 34)
						dwClip = m_wAmmo;
					else
						dwClip = CWeaponInfo::GetWeaponClipSize(m_byteWeaponId);
					
					// Check for reload
					if (m_wAmmo % dwClip == 0 && m_wAmmo != 0 && dwClip != m_wAmmo && m_bHasReload)
					{
						// Set the reload tick count
						m_dwReloadTickCount = GetTickCount();
						// Set reloading flag
						m_bReloading = true;
						// Stop shooting and aim only
						SetKeys(m_pPlayer->wUDAnalog, m_pPlayer->wLRAnalog, KEY_AIM);
					}
					else
					{
						// Send the bullet
						CSAMPFunctions::PlayerShoot((int)m_playerId, m_wHitId, m_byteHitType, m_byteWeaponId, m_vecAimAt);
					}

					// Update the shoot tick
					m_dwShootTickCount = dwThisTick;
				}
			}
		}
		// Process melee attack
		else if (m_bMeleeAttack)
		{
			// Get the time spent since last melee and compare it with the melee delay
			if ((GetTickCount() - m_dwShootTickCount) >= m_dwMeleeDelay)
			{
				// Set the melee keys
				SetKeys(m_pPlayer->wUDAnalog, m_pPlayer->wLRAnalog, m_bMeleeFightstyle ? KEY_AIM | KEY_SECONDARY_ATTACK : KEY_FIRE);
				// Update the tick count
				m_dwShootTickCount = GetTickCount();
			}
			else
			{
				// Reset keys
				SetKeys(m_pPlayer->wUDAnalog, m_pPlayer->wLRAnalog, m_bMeleeFightstyle ? KEY_AIM : KEY_NONE);
			}
		}
		// Process death
		if(GetHealth() <= 0.0f && GetState() != PLAYER_STATE_WASTED && GetState() != PLAYER_STATE_SPAWNED)
		{
			// Get the last damager weapon
			BYTE byteWeapon = -1;
			if(m_iLastDamager != INVALID_ENTITY_ID)
			{
				byteWeapon = pNetGame->pPlayerPool->pPlayer[m_iLastDamager]->syncData.byteWeapon;
			}
			// Kill the player
			Kill(m_iLastDamager, (int)byteWeapon);
		}
		// Update the player
		Update(UPDATE_STATE_ONFOOT);
	}
	// Process driver state
	else if(GetState() == PLAYER_STATE_DRIVER)
	{
		// Process driving
		if(m_bMoving)
		{
			// Validate the player vehicle
			if(m_pPlayer->wVehicleId == INVALID_ENTITY_ID)
				return;

			// Get the player vehicle interface
			CVehicle *pVehicle = pNetGame->pVehiclePool->pVehicle[m_pPlayer->wVehicleId];
			// Get the vehicle position
			CVector vecPosition;
			GetPosition(&vecPosition);
			// Get the vehicle velocity
			CVector vecVelocity;
			GetVelocity(&vecVelocity);
			// Make sure we still need to move
			if((GetTickCount() - m_dwMoveStartTime) < m_dwMoveTime)
			{
				// Get the time spent since the last update
				DWORD dwThisTick = GetTickCount();
				DWORD dwTime = (dwThisTick - m_dwMoveTickCount);
				// Set the new position
				CVector vecNewPosition = vecPosition + (vecVelocity * (float)dwTime);
				// Set the Z ground (is using ZMap)
				if(m_bUseZMap && pServer->GetZMap()->IsInitialized())
					vecNewPosition.fZ = pServer->GetZMap()->GetGroundForCoord(vecNewPosition) + 0.5f;
				
				// Set the vehicle position
				SetPosition(vecNewPosition);
				// Update the tick count
				m_dwMoveTickCount = dwThisTick;
			}
			else
			{
				// Get the time spent since the last update
				DWORD dwThisTick = GetTickCount();
				DWORD dwTime = (dwThisTick - m_dwMoveTickCount);
				dwTime -= (dwThisTick - m_dwMoveStartTime) - m_dwMoveTime;
				// Set the new position
				CVector vecNewPosition = vecPosition + (vecVelocity * (float)dwTime);
				// Set the Z ground (is using ZMap)
				if(m_bUseZMap && pServer->GetZMap()->IsInitialized())
					vecNewPosition.fZ = pServer->GetZMap()->GetGroundForCoord(vecNewPosition) + 0.5f;
				
				// Set the position
				SetPosition(vecNewPosition);
				// Stop the player
				StopMoving();
				// Are we playing a node ?
				if(m_bPlayingNode)
				{
					// Check the return value of the point finish callback
					if(CCallbackManager::OnFinishNodePoint((int)m_playerId, m_iNodePoint))
					{
						// Process the next position
						int iNewPoint = m_pNode->Process(this, m_iNodePoint, m_iNodeLastPoint, m_iNodeType, m_vecNodeVelocity);
						// Update the points
						m_iNodeLastPoint = m_iNodePoint;
						m_iNodePoint = iNewPoint;
					}
					else
						StopPlayingNode();
				}
				else
					// Call the reach destination callback
					CCallbackManager::OnReachDestination((int)m_playerId);
			}	
		}
		// Update the player
		Update(UPDATE_STATE_DRIVER);
	}
	// Process passenger state
	else if(GetState() == PLAYER_STATE_PASSENGER)
	{
		// Update the player
		Update(UPDATE_STATE_PASSENGER);
	}
	// Process vehicle exiting
	if(GetState() == PLAYER_STATE_EXIT_VEHICLE)
	{
		if((GetTickCount() - m_dwEnterExitTickCount) > 1500)
		{
			// Reset the player state
			SetState(PLAYER_STATE_ONFOOT);
			// Get the vehicle position
			CVehicle *pVehicle = pNetGame->pVehiclePool->pVehicle[m_pPlayer->wVehicleId];
			CVector vecVehiclePos = pVehicle->vecPosition;
			// Get the seat position
			CVector *pvecSeat = CSAMPFunctions::GetVehicleModelInfoEx(pVehicle->customSpawn.iModelID,
				m_pPlayer->byteSeatId == 0 || m_pPlayer->byteSeatId == 1 ? VEHICLE_MODEL_INFO_FRONTSEAT : VEHICLE_MODEL_INFO_REARSEAT);
	
			// Adjust the seat vector
			CVector vecSeat(pvecSeat->fX + 0.8f, pvecSeat->fY, pvecSeat->fZ);
			if (m_pPlayer->byteSeatId == 0 || m_pPlayer->byteSeatId == 2)
				vecSeat.fX = -vecSeat.fX;

			// Get vehicle angle
			float fAngle = CMath::GetAngle(-pVehicle->vehMatrix.up.fX, pVehicle->vehMatrix.up.fY);
			// This is absolutely bullshit
			float _fAngle = fAngle * 0.01570796326794897f;
			// Calculate the seat position based on vehicle angle
			CVector vecSeatPosition(vecSeat.fX * cos(_fAngle) - vecSeat.fY * sin(_fAngle) + vecVehiclePos.fX,
				vecSeat.fX * sin(_fAngle) + vecSeat.fY * cos(_fAngle) + vecVehiclePos.fY, vecSeat.fZ + vecVehiclePos.fZ);

			// Set his position
			SetPosition(vecSeatPosition);
			// Set his angle
			SetAngle(fAngle);
			// Reset the player vehicle and seat id
			m_pPlayer->wVehicleId = INVALID_ENTITY_ID;
			m_pPlayer->byteSeatId = 0;	
			// Call the vehicle exit complete callback
			CCallbackManager::OnVehicleExitComplete((int)m_playerId);
		}
	}
}

void CPlayerData::SetPosition(CVector vecPosition)
{
	// Check the player state
	if(GetState() == PLAYER_STATE_DRIVER && m_pPlayer->wVehicleId != INVALID_ENTITY_ID)
	{
		// Get the player vehicle interface
		CVehicle *pVehicle = pNetGame->pVehiclePool->pVehicle[m_pPlayer->wVehicleId];
		// Get the player vehicle position
		pVehicle->vecPosition = vecPosition;
	}
	// Set the player position
	m_pPlayer->vecPosition = vecPosition;
}

void CPlayerData::GetPosition(CVector *pvecPosition)
{
	// Check the player state
	if((GetState() == PLAYER_STATE_DRIVER ||  GetState() == PLAYER_STATE_PASSENGER) && m_pPlayer->wVehicleId != INVALID_ENTITY_ID)
	{
		// Get the player vehicle interface
		CVehicle *pVehicle = pNetGame->pVehiclePool->pVehicle[m_pPlayer->wVehicleId];
		// Get the player vehicle position
		*pvecPosition = pVehicle->vecPosition;
	}
	else
		*pvecPosition = m_pPlayer->vecPosition;
}

void CPlayerData::SetQuaternion(float *fQuaternion)
{
	// Check the player state
	if (GetState() == PLAYER_STATE_DRIVER && m_pPlayer->wVehicleId != INVALID_ENTITY_ID)
	{
		// get vehicle interface
		CVehicle *pVehicle = pNetGame->pVehiclePool->pVehicle[m_pPlayer->wVehicleId];
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
	if((GetState() == PLAYER_STATE_DRIVER ||  GetState() == PLAYER_STATE_PASSENGER) && m_pPlayer->wVehicleId != INVALID_ENTITY_ID)
	{
		// Get the player vehicle interface
		CVehicle *pVehicle = pNetGame->pVehiclePool->pVehicle[m_pPlayer->wVehicleId];
		// Get the player vehicle quaternion
		CMath::GetQuaternionFromMatrix(pVehicle->vehMatrix, fQuaternion);
	}
	else
	{
		// Get the player quaternion
		memcpy(fQuaternion, m_pPlayer->syncData.fQuaternion, 4 * sizeof(float));
	}
}

void CPlayerData::SetAngle(float fAngle)
{
	// Set the player
	m_pPlayer->fAngle = fAngle;
	// Rotate new quaternion matrix
	MATRIX4X4 *matrix = new MATRIX4X4;
	float *fQuaternion = new float[4];

	CMath::QuaternionRotateZ(matrix, CMath::DegreeToRadians(fAngle));	
	CMath::GetQuaternionFromMatrix(*matrix, fQuaternion);
	// Update quaternion
	SetQuaternion(fQuaternion);
}

float CPlayerData::GetAngle()
{
	return m_pPlayer->fAngle;
}

void CPlayerData::SetHealth(float fHealth)
{
	// Validate the health
	if(fHealth < 0.0f)
		fHealth = 0.0f;
	else if(fHealth > 200.0f)
		fHealth = 200.0f;

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
	if(fArmour < 0.0f)
		fArmour = 0.0f;
	else if(fArmour > 200.0f)
		fArmour = 200.0f;

	// Set the player armour
	m_pPlayer->fArmour = fArmour;
}

float CPlayerData::GetArmour()
{
	return m_pPlayer->fArmour;
}

void CPlayerData::SetSkin(int iSkin)
{
	// Make sure the skin has changed
	if(iSkin == m_pPlayer->spawn.iSkin)
		return;

	// Validate the skin
	if(iSkin > 311 || iSkin < 0)
		return;

	// Send RPC
	if (m_pPlayer->bReadyToSpawn)
	{
		// RPC send not working, set skin to 0 (CJ), but fixed after restreaming
		//RakNet::BitStream bsData;
		//bsData.Write(m_pPlayer->wPlayerId);
		//bsData.Write(iSkin);

		for (int i = 0; i < MAX_PLAYERS; i++)
		{
			if (pNetGame->pPlayerPool->bIsPlayerConnectedEx[i] && i != m_playerId)
			{
				CPlayer *pPlayer = pNetGame->pPlayerPool->pPlayer[i];
				pPlayer->byteStreamedIn[m_playerId] = 0;

				//if (pPlayer && pPlayer->byteStreamedIn[m_playerId])
				//	pRakServer->RPC(&RPC_SetPlayerSkin, &bsData, HIGH_PRIORITY, RELIABLE_ORDERED, 2, pRakServer->GetPlayerIDFromIndex(i), false, false);
			}
		}

		//pRakServer->RPC(&RPC_SetPlayerSkin, &bsData, HIGH_PRIORITY, RELIABLE_ORDERED, 2, pRakServer->GetPlayerIDFromIndex(m_playerId), false, false);
		
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
	pNetGame->pPlayerPool->dwVirtualWorld[m_pPlayer->wPlayerId] = iVirtualWorld;
}

int CPlayerData::GetVirtualWorld()
{
	return pNetGame->pPlayerPool->dwVirtualWorld[m_pPlayer->wPlayerId];
}

void CPlayerData::SetWeapon(BYTE byteWeaponId)
{
	// Validate the weapon id
	if(byteWeaponId > 46)
		return;

	// Set the player weapon id
	m_byteWeaponId = byteWeaponId;
}

BYTE CPlayerData::GetWeapon()
{
	return m_byteWeaponId;
}

void CPlayerData::SetAmmo(WORD wAmmo)
{
	// Validate the ammo
	if(wAmmo > 0xFFFF)
		return;

	// Set the player ammo
	m_wAmmo = wAmmo;
}

WORD CPlayerData::GetAmmo()
{
	return m_wAmmo;
}

void CPlayerData::SetWeaponSkill(int iSkill, int iLevel)
{
	m_pPlayer->wSkillLevel[iSkill] = iLevel;

	if (m_pPlayer->byteState != 9 && m_pPlayer->byteState != 0)
	{
		RakNet::BitStream bsData;
		bsData.Write(m_pPlayer->wPlayerId);
		bsData.Write(iSkill);
		bsData.Write(iLevel);

		pRakServer->RPC(&RPC_SetPlayerSkillLevel, &bsData, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_PLAYER_ID, true, false);
	}
}

WORD CPlayerData::GetWeaponSkill(int iSkill)
{
	return m_pPlayer->wSkillLevel[iSkill];
}

void CPlayerData::SetSpecialAction(int iActionId)
{
	// Validate the action id
	if(iActionId < 0 || (iActionId > 13 && (iActionId != 20 && iActionId != 21 && iActionId != 22
		&& iActionId != 23 && iActionId != 24 && iActionId != 25 && iActionId != 68)))
		return;

	// Set the player action
	m_pPlayer->syncData.byteSpecialAction = (BYTE)iActionId;
}

int CPlayerData::GetSpecialAction()
{
	return m_pPlayer->syncData.byteSpecialAction;
}

void CPlayerData::SetAnimation(int iAnimationId, float fDelta, bool bLoop, bool bLockX, bool bLockY, bool bFreeze, int iTime)
{
	m_pPlayer->syncData.wAnimIndex = (WORD)iAnimationId;

	if (iAnimationId == 0) {
		m_pPlayer->syncData.wAnimFlags = 0;
	} else {
		m_pPlayer->syncData.wAnimFlags = ((BYTE)fDelta & 0xFF) | (bLoop << 8) | (bLockX << 9) | (bLockY << 10) | (bFreeze << 11) | (iTime << 12);
	}
}

void CPlayerData::GetAnimation(int *iAnimationId, float *fDelta, bool *bLoop, bool *bLockX, bool *bLockY, bool *bFreeze, int *iTime)
{
	*iAnimationId = m_pPlayer->syncData.wAnimIndex;
	*fDelta = m_pPlayer->syncData.wAnimFlags & 0xFF;
	*bLoop = m_pPlayer->syncData.wAnimFlags >> 8 & 0x1 != 0;
	*bLockX = m_pPlayer->syncData.wAnimFlags >> 9 & 0x1 != 0;
	*bLockY = m_pPlayer->syncData.wAnimFlags >> 10 & 0x1 != 0;
	*bFreeze = m_pPlayer->syncData.wAnimFlags >> 11 & 0x1 != 0;
	*iTime = m_pPlayer->syncData.wAnimFlags >> 12 & 0xF;
}

void CPlayerData::SetVelocity(CVector vecVelocity)
{
	// Check the player state
	if(GetState() == PLAYER_STATE_DRIVER && m_pPlayer->wVehicleId != INVALID_ENTITY_ID)
	{
		// Get the player vehicle interface
		CVehicle *pVehicle = pNetGame->pVehiclePool->pVehicle[m_pPlayer->wVehicleId];
		// Set the player vehicle velocity
		pVehicle->vecVelocity = vecVelocity;
	}
	// Set the player velocity
	m_pPlayer->vecVelocity = vecVelocity;
}

void CPlayerData::GetVelocity(CVector *pvecVelocity)
{
	// Check the player state
	if((GetState() == PLAYER_STATE_DRIVER ||  GetState() == PLAYER_STATE_PASSENGER) && m_pPlayer->wVehicleId != INVALID_ENTITY_ID)
	{
		// Get the player vehicle interface
		CVehicle *pVehicle = pNetGame->pVehiclePool->pVehicle[m_pPlayer->wVehicleId];
		// Get the player vehicle position
		*pvecVelocity = pVehicle->vecVelocity;
	}
	else
		*pvecVelocity = m_pPlayer->vecVelocity;
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

bool CPlayerData::GoTo(CVector vecPoint, int iType, bool bUseZMap, float fRadius, bool bGetAngle)
{
	// Validate the mouvement type
	if (iType == MOVE_TYPE_AUTO)
		iType = GetState() == PLAYER_STATE_DRIVER ? MOVE_TYPE_DRIVE : MOVE_TYPE_RUN;

	if (iType > MOVE_TYPE_DRIVE || iType < MOVE_TYPE_WALK)
		return false;

	// Save the destination point
	m_vecDestination = vecPoint;
	// Add radius
	if (fRadius != 0.0f)
	{
		m_vecDestination -= CVector(CUtils::RandomFloat(-fRadius, fRadius), CUtils::RandomFloat(-fRadius, fRadius), 0.0);
	}
	// Get the moving type key and speed
	DWORD dwMoveKey = KEY_NONE;
	float fMoveSpeed = MOVE_SPEED_RUN;
	if (iType == MOVE_TYPE_WALK)
	{
		fMoveSpeed = MOVE_SPEED_WALK;
		dwMoveKey = KEY_WALK;
	}
	else if (iType == MOVE_TYPE_SPRINT)
	{
		fMoveSpeed = MOVE_SPEED_SPRINT;
		dwMoveKey = KEY_SPRINT;
	}
	else if (iType == MOVE_TYPE_DRIVE)
	{
		fMoveSpeed = m_pPlayer->vecVelocity.fX;
		dwMoveKey = KEY_NONE;
	}
	// Set the moving keys
	if (iType != MOVE_TYPE_DRIVE)
		SetKeys(0x8000, KEY_NONE, dwMoveKey);
	else
		SetKeys(KEY_NONE, KEY_NONE, KEY_NONE);

	// Get the player position
	CVector vecPosition;
	GetPosition(&vecPosition);
	// Get the moving front vector
	CVector vecFront = m_vecDestination - vecPosition;
	// Get the distance to the destination point
	float fDistance = CMath::GetDistanceBetween3DPoints(vecPosition, m_vecDestination);	
	// Set the player to the destination angle
	vecFront /= fDistance;
	if (bGetAngle)
		SetAngle(CMath::RadiansToDegree(atan2(vecFront.fY, vecFront.fX)));
	// Set the moving velocity
	vecFront *= (fMoveSpeed / 100.0f); // Step per 1ms
	SetVelocity(vecFront);
	// Calculate the moving time
	m_dwMoveTime = (DWORD)(fDistance / vecFront.Length());
	// Get the start move tick
	m_dwMoveTickCount = GetTickCount();
	m_dwMoveStartTime = GetTickCount();
	// Mark as moving
	m_bMoving = true;
	// Save the ZMap usage
	m_bUseZMap = bUseZMap;
	return true;
}

void CPlayerData::StopMoving()
{
	// Make sure the player is moving
	if(!m_bMoving)
		return;

	// Reset moving flag
	m_bMoving = false;
	// Reset analogs
	SetKeys(0, 0, m_pPlayer->dwKeys);
	// Reset other moving variables
	m_dwMoveTime = 0;
	m_dwMoveStartTime = 0;
	memset(&m_vecDestination, 0, sizeof(CVector));
	// Reset the ZMap usage
	m_bUseZMap = false;
}

bool CPlayerData::IsMoving()
{
	return m_bMoving;
}

void CPlayerData::ToggleReloading(bool bToggle)
{
	m_bHasReload = bToggle;
}

void CPlayerData::ToggleInfiniteAmmo(bool bToggle)
{
	m_bHasInfiniteAmmo = bToggle;
}

void CPlayerData::AimAt(CVector vecPoint, bool bShoot)
{
	// Set the aiming flag
	if(!m_bAiming)
	{
		// Get the shooting start tick
		m_dwShootTickCount = GetTickCount();
		m_bReloading = false;
	}
	// Adjust the player position
	CVector vecPosition = m_pPlayer->vecPosition;
	// Save the aiming point
	m_vecAimAt = vecPoint;
	// Get the aiming distance
	CVector vecDistance = vecPoint - vecPosition;
	// Get the distance to the destination point
	float fDistance = CMath::GetDistanceBetween3DPoints(vecPosition, vecPoint);
	// Calculate the aiming Z angle
	float fZAngle = -acos(((vecDistance.fX * vecDistance.fX) + (vecDistance.fY * vecDistance.fY))
		/ (sqrt((vecDistance.fX * vecDistance.fX) + (vecDistance.fY * vecDistance.fY) + (vecDistance.fZ * vecDistance.fZ))
		* sqrt((vecDistance.fX * vecDistance.fX) + (vecDistance.fY * vecDistance.fY)))) + 0.1f;
	
	// Get the destination angle
	vecDistance /= fDistance;
	SetAngle(CMath::RadiansToDegree(atan2(vecDistance.fY, vecDistance.fX)));
	// Set the Z angle
	m_pPlayer->aimSyncData.fZAim = fZAngle;
	// Set the player aiming vector
	m_pPlayer->aimSyncData.vecFront = vecDistance;
	// Set the player camera position
	m_pPlayer->aimSyncData.vecPosition = vecPosition;
	// Set keys
	if (!m_bAiming)
		SetKeys(m_pPlayer->wUDAnalog, m_pPlayer->wLRAnalog, bShoot ? KEY_FIRE | KEY_AIM : KEY_AIM);

	// Mark as aiming
	m_bAiming = true;
}

void CPlayerData::AimAtPlayer(WORD wHitId, bool bShoot)
{
	CPlayer *pPlayer = pNetGame->pPlayerPool->pPlayer[wHitId];
	AimAt(pPlayer->vecPosition, bShoot);
	m_wHitId = wHitId;
	m_byteHitType = BULLET_HIT_TYPE_PLAYER;
}

void CPlayerData::StopAim()
{
	// Make sure the player is aiming
	if(!m_bAiming)
		return;

	// Reset aiming flags
	m_bAiming = false;
	m_bReloading = false;
	m_wHitId = INVALID_ENTITY_ID;
	m_byteHitType = BULLET_HIT_TYPE_NONE;
	// Reset keys
	SetKeys(m_pPlayer->wUDAnalog, m_pPlayer->wLRAnalog, KEY_NONE);
}

bool CPlayerData::MeleeAttack(DWORD dwTime, bool bUseFightstyle)
{
	// Make sure the player is not melee attacking
	if (m_bMeleeAttack)
		return false;

	// Validate the player state
	if (GetState() != PLAYER_STATE_ONFOOT)
		return false;

	// Validate the player weapon
	if (m_pPlayer->syncData.byteWeapon < 0 || m_pPlayer->syncData.byteWeapon > 15)
		return false;

	// Set the attacking delay
	m_dwMeleeDelay = dwTime;
	if (dwTime <= pServer->GetUpdateRate())
		m_dwMeleeDelay = pServer->GetUpdateRate() + 5;

	// Get the starting tick
	m_dwShootTickCount = GetTickCount();
	// Set the melee flag
	m_bMeleeAttack = true;
	// Set the melee use fightstyle flag
	m_bMeleeFightstyle = bUseFightstyle;
	// Reset the aiming flag
	m_bAiming = false;
	m_wHitId = INVALID_ENTITY_ID;
	m_byteHitType = BULLET_HIT_TYPE_NONE;
	// Set the melee keys
	SetKeys(m_pPlayer->wUDAnalog, m_pPlayer->wLRAnalog, KEY_FIRE);

	return true;
}

void CPlayerData::StopAttack()
{
	// Make sure the player is attacking
	if(!m_bMeleeAttack)
		return;

	// Reset attacking flag
	m_bMeleeAttack = false;
	m_bMeleeFightstyle = false;
	// Reset keys
	SetKeys(m_pPlayer->wUDAnalog, m_pPlayer->wLRAnalog, KEY_NONE);
}

bool CPlayerData::IsAiming()
{
	return m_bAiming;
}

bool CPlayerData::IsShooting()
{
	return m_bAiming && m_pPlayer->dwKeys == 4;
}

bool CPlayerData::IsReloading()
{
	return m_bReloading;
}

void CPlayerData::ProcessDamage(int iDamagerId, float fHealthLoss, int iWeaponId, int iBodypart)
{
	// Call the on take damage callback
	int iReturn = CCallbackManager::OnTakeDamage((int)m_playerId, iDamagerId, iWeaponId, iBodypart, fHealthLoss);
	// Check the returned value
	if(iReturn)
	{
		// Check the armour
		if(GetArmour() > 0.0f)
		{
			// Save the old armour
			float fArmour = GetArmour();
			// Decrease the armor
			SetArmour(fArmour - fHealthLoss);
			// If the damage is bigger than the armour then decrease the health aswell
			if(fArmour - fHealthLoss < 0.0f)
				SetHealth(GetHealth() - (fHealthLoss - fArmour));
		}
		else
			SetHealth(GetHealth() - fHealthLoss);
	}
	// Save the last damager
	m_iLastDamager = iDamagerId;
}

bool CPlayerData::EnterVehicle(int iVehicleId, int iSeatId, int iType)
{
	// Validate the vehicle
	if (iVehicleId < 1 || iVehicleId > MAX_VEHICLES)
		return false;

	// Validate the player state
	if (GetState() != PLAYER_STATE_ONFOOT)
		return false;

	// Validate the entering type
	if (iType > MOVE_TYPE_SPRINT || iType < MOVE_TYPE_WALK)
		iType = MOVE_TYPE_RUN;

	// Validate the vehicle
	CVehicle *pVehicle = pNetGame->pVehiclePool->pVehicle[iVehicleId];

	if (!pVehicle)
		return false;

	// Validate the seat id
	if (!CVehicleInfo::IsValidPassengerSeat(iSeatId, pVehicle->customSpawn.iModelID))
		return false;

	// Validate the distance to enter
	if (CMath::GetDistanceBetween3DPoints(pVehicle->vecPosition,
		m_pPlayer->vecPosition) > MAX_DISTANCE_TO_ENTER_VEHICLE)
		return false;

	// Save the entering stats
	m_wVehicleToEnter = (WORD)iVehicleId;
	m_byteSeatToEnter = (BYTE)iSeatId;
	// Get the seat position
	CVector *pvecSeat = CSAMPFunctions::GetVehicleModelInfoEx(pVehicle->customSpawn.iModelID,
		iSeatId == 0 || iSeatId == 1 ? VEHICLE_MODEL_INFO_FRONTSEAT : VEHICLE_MODEL_INFO_REARSEAT);

	// Adjust the seat vector
	CVector vecSeat(pvecSeat->fX + 0.7f, pvecSeat->fY, pvecSeat->fZ);
	if (iSeatId == 0 || iSeatId == 2)
		vecSeat.fX = -vecSeat.fX;

	// Get vehicle angle
	float fAngle = CMath::GetAngle(-pVehicle->vehMatrix.up.fX, pVehicle->vehMatrix.up.fY);
	// This is absolutely bullshit
	float _fAngle = fAngle * 0.01570796326794897f;
	// Calculate the seat position based on vehicle angle
	CVector vecSeatPosition(vecSeat.fX * cos(_fAngle) - vecSeat.fY * sin(_fAngle), vecSeat.fX * sin(_fAngle) + vecSeat.fY * cos(_fAngle), vecSeat.fZ);
	// Calculate the destination point
	CVector vecDestination = pVehicle->vecPosition + vecSeatPosition;
	// Go to the vehicle
	GoTo(vecDestination, iType, true);
	return true;
}

bool CPlayerData::ExitVehicle()
{
	// Validate the player state
	if(GetState() != PLAYER_STATE_DRIVER && GetState() != PLAYER_STATE_PASSENGER)
		return false;

	// Validate the player vehicle
	if(!pNetGame->pVehiclePool->pVehicle[m_pPlayer->wVehicleId])
		return false;

	// Call the SAMP exit vehicle function
	CSAMPFunctions::PlayerExitVehicle(m_playerId, m_pPlayer->wVehicleId);
	// Set the player state
	SetState(PLAYER_STATE_EXIT_VEHICLE);
	// Set the exit start tick
	m_dwEnterExitTickCount = GetTickCount();
	return true;
}

bool CPlayerData::PutInVehicle(int iVehicleId, int iSeatId)
{
	// Validate the vehicle
	if (iVehicleId < 1 || iVehicleId > MAX_VEHICLES)
		return false;

	// Validate the player state
	if (GetState() != PLAYER_STATE_ONFOOT)
		return false;

	// Validate the vehicle
	CVehicle *pVehicle = pNetGame->pVehiclePool->pVehicle[iVehicleId];

	if (!pVehicle)
		return false;

	// Validate the seat id
	if (!CVehicleInfo::IsValidPassengerSeat(iSeatId, pVehicle->customSpawn.iModelID))
		return false;

	// Set the player vehicle and seat id
	m_pPlayer->wVehicleId = (WORD)iVehicleId;
	m_pPlayer->byteSeatId = (BYTE)iSeatId;

	// Set the player state
	SetState(iSeatId == 0 ? PLAYER_STATE_DRIVER : PLAYER_STATE_PASSENGER);

	// Set the angle
	SetAngle(CMath::GetAngle(-pVehicle->vehMatrix.up.fX, pVehicle->vehMatrix.up.fY));
	return true;
}

bool CPlayerData::RemoveFromVehicle()
{
	// Validate the player state
	if(GetState() != PLAYER_STATE_DRIVER && GetState() != PLAYER_STATE_PASSENGER)
		return false;

	// Validate the player vehicle
	if(!pNetGame->pVehiclePool->pVehicle[m_pPlayer->wVehicleId])
		return false;

	// Set the player state
	SetState(PLAYER_STATE_ONFOOT);
	CVector vecVehiclePos = pNetGame->pVehiclePool->pVehicle[m_pPlayer->wVehicleId]->vecPosition;
	SetPosition(CVector(vecVehiclePos.fX + 2.0f, vecVehiclePos.fY + 2.0f, vecVehiclePos.fZ));
	// Reset the player vehicle and seat id
	m_pPlayer->wVehicleId = INVALID_ENTITY_ID;
	m_pPlayer->byteSeatId = 0;
	return true;
}

bool CPlayerData::IsInVehicle()
{
	return m_pPlayer->wVehicleId == INVALID_ENTITY_ID;
}

int CPlayerData::GetVehicleId()
{
	return m_pPlayer->wVehicleId;
}

int CPlayerData::GetSeatId()
{
	return m_pPlayer->byteSeatId;
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
	if (fHealth < 0.0f)
		fHealth = 0.0f;
	else if (fHealth > 1000.0f)
		fHealth = 1000.0f;

	m_pPlayer->vehicleSyncData.fHealth = fHealth;
	pNetGame->pVehiclePool->pVehicle[ m_pPlayer->vehicleSyncData.wVehicleId ]->fHealth = fHealth;
}

float CPlayerData::GetVehicleHealth()
{
	return m_pPlayer->vehicleSyncData.fHealth;
}

void CPlayerData::SetPassengerDriveBy(bool bState)
{
	m_pPlayer->passengerSyncData.byteDriveBy = bState ? 1 : 0;
}

bool CPlayerData::IsPassengerDriveBy()
{
	return m_pPlayer->passengerSyncData.byteDriveBy != 0;
}

void CPlayerData::SetSurfingOffsets(CVector vecOffsets)
{
	m_vecSurfing = vecOffsets;
}

void CPlayerData::GetSurfingOffsets(CVector *vecOffsets)
{
	*vecOffsets = m_vecSurfing;
}

void CPlayerData::SetSurfingVehicle(int iVehicleId)
{
	m_wSurfingInfo = iVehicleId;
}

int CPlayerData::GetSurfingVehicle()
{
	return m_wSurfingInfo;
}

void CPlayerData::SetSurfingObject(int iObjectId)
{
	m_wSurfingInfo = MAX_VEHICLES + iObjectId;
	pNetGame->pObjectPool->bObjectSlotState[iObjectId] = true;
}

int CPlayerData::GetSurfingObject()
{
	int objectid = m_wSurfingInfo - MAX_VEHICLES;
	if (objectid >= 0 && objectid < MAX_OBJECTS)
	{
		if (pNetGame->pObjectPool->bObjectSlotState[objectid])
			return objectid;
	}
	return INVALID_OBJECT_ID;
}

bool CPlayerData::StartPlayingPlayback(char *szFile)
{
	// Make sure the player is not already Playing
	if(m_bPlaying)
		return false;

	// Create a new playback instance
	m_pPlayback = new CPlayback(szFile);
	// Initialize it
	if(!m_pPlayback || !m_pPlayback->Initialize())
		return false;

	// Mark as Playing
	m_bPlaying = true;
	return true;
}

void CPlayerData::StopPlayingPlayback()
{
	// Make sure the player is Playing
	if(!m_bPlaying)
		return;

	// Delete the playback instance
	SAFE_DELETE(m_pPlayback);
	// Reset the player data
	SetVelocity(CVector(0.0f, 0.0f, 0.0f));
	SetKeys(KEY_NONE, KEY_NONE, KEY_NONE);
	// Reset the Playing flag
	m_bPlaying = false;
	// Call the playback finish callback
	CCallbackManager::OnFinishPlayback((int)m_playerId);
}

void CPlayerData::PausePlayingPlayback()
{
	// Make sure the player is Playing
	if(!m_bPlaying)
		return;

	// Pause the playback
	m_pPlayback->SetPaused(true);
}

void CPlayerData::ResumePlayingPlayback()
{
	// Make sure the player is Playing
	if(!m_bPlaying)
		return;

	// Resume the playback
	m_pPlayback->SetPaused(false);
}

bool CPlayerData::PlayNode(int iNodeId, int iType)
{	
	// Stop the player playback if he's playing one
	if(m_bPlaying)
		StopPlayingPlayback();

	// Stop the last node if any node exists
	if(m_bPlayingNode)
		StopPlayingNode();

	// Get the node instance
	m_pNode = pServer->GetNodeManager()->GetAt(iNodeId);
	// Save the last point
	m_iNodeLastPoint = m_pNode->GetPointId();
	// Save the node type
	m_iNodeType = iType;
	// Get the starting point
	CVector vecStart;
	m_pNode->GetPosition(&vecStart);
	// Save the node moving velocity
	m_vecNodeVelocity = m_pPlayer->vecVelocity;
	// Set the player in the starting point
	SetPosition(vecStart);
	// Get the node link and set it to the next position
	m_pNode->SetLink(m_pNode->GetLinkId());
	m_pNode->SetPoint(m_pNode->GetLinkPoint());
	// Save the current point
	m_iNodePoint = m_pNode->GetLinkPoint();
	// Get the destination point
	CVector vecDestination;
	m_pNode->GetPosition(&vecDestination);
	// Move the player to it
	GoTo(vecDestination, iType == NODE_TYPE_PED ? MOVE_TYPE_WALK : MOVE_TYPE_DRIVE, false);
	// Set the player as playing node
	m_bPlayingNode = true;
	return true;
}

void CPlayerData::StopPlayingNode()
{
	// Make sure the player is playing a node
	if(!m_bPlayingNode)
		return;

	// Reset the node instance
	m_pNode = NULL;
	// Reset the player data
	SetKeys(KEY_NONE, KEY_NONE, KEY_NONE);
	SetVelocity(CVector(0.0f, 0.0f, 0.0f));
	m_vecNodeVelocity = CVector();
	// Reset the node flag
	m_bPlayingNode = false;
	m_iNodePoint = 0;
	m_iNodeLastPoint = 0;
	m_iNodeType = 0;
	// Call the node finish callback
	CCallbackManager::OnFinishNode((int)m_playerId);
}

int CPlayerData::ChangeNode(int iNodeId, unsigned short usLinkId)
{
	// Make sure the player is playing a node
	if(!m_bPlayingNode)
		return 0;

	// Get the node instance
	m_pNode = pServer->GetNodeManager()->GetAt(iNodeId);
	// Process the node change
	return m_pNode->ProcessNodeChange(this, usLinkId, m_iNodeType, m_vecNodeVelocity);
}
