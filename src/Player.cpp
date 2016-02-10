/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: Player.cpp
	- Author(s): OrMisicL

  =========================================*/


#include "Main.h"

extern CServer   *pServer;

CPlayer::CPlayer(EntityId playerId, char *szName)
{
	// Save the player id
	m_playerId = playerId;
	// Save player name
	strlcpy(m_szName, szName, sizeof(m_szName));
	// Reset variables
	m_vecDestination = CVector3();
	m_vecNodeVelocity = CVector3();
	m_vecAimAt = CVector3();
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
	m_byteWeaponId = 0;
	m_wAmmo = 0;
	m_iNodePoint = 0;
	m_iNodeLastPoint = 0;
	m_iLastDamager = INVALID_ENTITY_ID;
	m_wVehicleToEnter = INVALID_ENTITY_ID;
	m_byteSeatToEnter = 0;
}

CPlayer::~CPlayer()
{

}

void CPlayer::Destroy()
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

bool CPlayer::Setup()
{
	if(m_playerId == INVALID_ENTITY_ID)
		return false;

	// Get the player interface
	CSAMPServer *pSAMPServer = (CSAMPServer *)CAddress::VAR_ServerPtr;
	m_pInterface = pSAMPServer->pPlayerPool->pPlayer[m_playerId];
	// Validate the interface
	if(!m_pInterface)
		return false;

	// Set the setup flag
	m_bSetup = true;
	// Reset his state
	SetState(PLAYER_STATE_NONE);
	// Initialize the update tick
	m_dwUpdateTick = GetTickCount();
	return true;
}

bool CPlayer::Spawn(int iSkinId)
{
	// Make sure the player is not already spawned
	if(m_bSpawned)
		return false;

	// Mark spawned
	m_bSpawned = true;
	m_pInterface->bReadyToSpawn = TRUE;
	// Set the player skin
	m_pInterface->iSkinId = iSkinId;
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
	m_pInterface->wVehicleId = INVALID_ENTITY_ID;
	m_pInterface->byteSeatId = 0;
	// Call the NPC spawn callback
	CCallbackManager::OnSpawn((int)m_playerId);
	return true;
}

bool CPlayer::Respawn()
{
	// Make sure the player is already spawned
	if(!m_bSpawned)
		return false;

	// Pause Playing any playback
	if(m_bPlaying)
		m_pPlayback->SetPaused(true);

	// Get the last player stats
	CVector3 vecPosition = m_pInterface->vecPosition;
	float fHealth = m_pInterface->fHealth;
	float fArmour = m_pInterface->fArmour;
	WORD wVehicleId = m_pInterface->wVehicleId;
	BYTE byteSeat = m_pInterface->byteSeatId;
	BYTE byteSpecialAction = m_pInterface->syncData.byteSpecialAction;
	// Call the SAMP spawn functions
	m_pInterface->bReadyToSpawn = TRUE;
	CSAMPFunctions::SpawnPlayer((int)m_playerId);
	// Set the player state onfoot
	SetState(PLAYER_STATE_ONFOOT);
	// Reset vehicle and seat id
	m_pInterface->wVehicleId = INVALID_ENTITY_ID;
	m_pInterface->byteSeatId = 0;
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

void CPlayer::SetSpawnPosition(CVector3 vecPosition)
{
	// Set the player position
	m_pInterface->vecSpawnPosition = vecPosition;
}

void CPlayer::UpdateSync(int iState)
{
	// Make sure the state is valid
	if(iState < UPDATE_STATE_NONE || iState > UPDATE_STATE_PASSENGER)
		return;

	// Set the new update state
	m_pInterface->iUpdateState = iState;
}

void CPlayer::Update(int iState)
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
		m_pInterface->syncData.vecPosition = m_pInterface->vecPosition;
		// Set the sync velocity vector
		m_pInterface->syncData.vecVelocity = m_pInterface->vecVelocity;
		// Reset the sync surfing vector
		m_pInterface->syncData.vecSurfing = CVector3(0.0f, 0.0f, 0.0f);
		// Reset the sync surfing information
		m_pInterface->syncData.wSurfingInfo = 0;
		// Set the sync weapon index
		m_pInterface->syncData.byteWeapon = m_byteWeaponId;
		// Set the sync health and armour
		m_pInterface->syncData.byteHealth = (BYTE)m_pInterface->fHealth;
		m_pInterface->syncData.byteArmour = (BYTE)m_pInterface->fArmour;
		// Set the sync keys
		m_pInterface->syncData.wUDAnalog = m_pInterface->wUDAnalog;
		m_pInterface->syncData.wLRAnalog = m_pInterface->wLRAnalog;
		m_pInterface->syncData.wKeys = (WORD)m_pInterface->dwKeys;
		// Update aim data
		UpdateAim();
		// Set the new update state
		m_pInterface->iUpdateState = iState;
	}
	// Update driver state
	else if(iState == UPDATE_STATE_DRIVER && GetState() == PLAYER_STATE_DRIVER)
	{
		// Dont process if we dont have any vehicle
		if(m_pInterface->wVehicleId == INVALID_ENTITY_ID)
			return;

		// Get the player vehicle interface
		CSAMPServer *pSAMPServer = (CSAMPServer *)CAddress::VAR_ServerPtr;
		CSAMPVehicle *pVehicle = pSAMPServer->pVehiclePool->pVehicle[m_pInterface->wVehicleId];
		// Set the player sync vehicle id
		m_pInterface->vehicleSyncData.wVehicleId = m_pInterface->wVehicleId;
		// Set the player sync position
		m_pInterface->vehicleSyncData.vecPosition = pVehicle->vecPosition;
		// Set the player velocity
		m_pInterface->vehicleSyncData.vecVelocity = m_pInterface->vecVelocity;
		// Set the vehicle quaternion
		m_pInterface->vehicleSyncData.vecQuaternion = pVehicle->vecQuaternion;
		m_pInterface->vehicleSyncData.fQuaternionAngle = pVehicle->fQuaternionAngle;
		// Set the player quaternion
		SetQuaternion(pVehicle->vecQuaternion, pVehicle->fQuaternionAngle);
		// Set the player keys
		m_pInterface->vehicleSyncData.wUDAnalog = m_pInterface->wUDAnalog;
		m_pInterface->vehicleSyncData.wLRAnalog = m_pInterface->wLRAnalog;
		m_pInterface->vehicleSyncData.wKeys = (WORD)m_pInterface->dwKeys;
		// Set the player health and armour
		m_pInterface->vehicleSyncData.fHealth = pVehicle->fHealth;
		m_pInterface->vehicleSyncData.bytePlayerHealth = (BYTE)m_pInterface->fHealth;
		m_pInterface->vehicleSyncData.bytePlayerArmour = (BYTE)m_pInterface->fArmour;
		// Set the player weapon
		m_pInterface->vehicleSyncData.bytePlayerWeapon = m_byteWeaponId;
		// Set the new update state
		m_pInterface->iUpdateState = iState;
	}
	// Update passenger state
	else if(iState == UPDATE_STATE_PASSENGER && GetState() == PLAYER_STATE_PASSENGER)
	{
		// Dont process if we dont have any vehicle
		if(m_pInterface->wVehicleId == INVALID_ENTITY_ID)
			return;

		// Get the player vehicle interface
		CSAMPServer *pSAMPServer = (CSAMPServer *)CAddress::VAR_ServerPtr;
		CSAMPVehicle *pVehicle = pSAMPServer->pVehiclePool->pVehicle[m_pInterface->wVehicleId];
		// Set the player position to the vehicle position
		SetPosition(pVehicle->vecPosition);
		// Set the player sync vehicle and seat id
		m_pInterface->passengerSyncData.wVehicleId = m_pInterface->wVehicleId;
		m_pInterface->passengerSyncData.byteSeatId = m_pInterface->byteSeatId;
		// Set the player sync position
		m_pInterface->passengerSyncData.vecPosition = pVehicle->vecPosition;
		// Set the player keys
		m_pInterface->passengerSyncData.wUDAnalog = m_pInterface->wUDAnalog;
		m_pInterface->passengerSyncData.wLRAnalog = m_pInterface->wLRAnalog;
		m_pInterface->passengerSyncData.wKeys = (WORD)m_pInterface->dwKeys;
		// Set the player health and armour
		m_pInterface->passengerSyncData.bytePlayerHealth = (BYTE)m_pInterface->fHealth;
		m_pInterface->passengerSyncData.bytePlayerArmour = (BYTE)m_pInterface->fArmour;
		// Set the player weapon
		m_pInterface->passengerSyncData.bytePlayerWeapon = m_byteWeaponId;
		// Set the new update state
		m_pInterface->iUpdateState = iState;
	}
	// Update the tick count
	m_dwUpdateTick = GetTickCount();
}

void CPlayer::UpdateAim()
{
	if(m_bAiming)
	{
		// Set the camera mode
		m_pInterface->aimSyncData.byteCameraMode = CWeaponInfo::IsDoubleHanded(m_pInterface->syncData.byteWeapon) ? 53 : 7;
		if(m_pInterface->syncData.byteWeapon == 0 || m_pInterface->syncData.byteWeapon == 1)
			m_pInterface->aimSyncData.byteCameraMode = 4;
		else if (m_pInterface->syncData.byteWeapon == 34) // Sniper rifle
			m_pInterface->aimSyncData.byteCameraMode = 7;

		// Set the weapon state
#ifdef _WIN32
		m_pInterface->aimSyncData.byteWeaponState = 191;// (BYTE)eWeaponState::WS_MORE_BULLETS;
		if (m_bReloading)
			m_pInterface->aimSyncData.byteWeaponState = 127;// (BYTE)eWeaponState::WS_RELOADING;
		else if (!m_wAmmo)
			m_pInterface->aimSyncData.byteWeaponState = 0;// (BYTE)eWeaponState::WS_NO_BULLETS;
#else
		// For some reasons, enumerations dosen't work in linux
		m_pInterface->aimSyncData.byteWeaponState = 191;//(BYTE)WS_MORE_BULLETS;
		if (m_bReloading)
			m_pInterface->aimSyncData.byteWeaponState = 127;//(BYTE)WS_RELOADING;
		else if (!m_wAmmo)
			m_pInterface->aimSyncData.byteWeaponState = 0;//(BYTE)WS_NO_BULLETS;
#endif
	}
	else
	{
		// Set the camera mode and weapon state
		m_pInterface->aimSyncData.byteCameraMode = 0;
#ifdef _WIN32
		m_pInterface->aimSyncData.byteWeaponState = (BYTE)eWeaponState::WS_NO_BULLETS;
#else
		m_pInterface->aimSyncData.byteWeaponState = WS_NO_BULLETS;
#endif
		// Convert the player angle to radians
		float fAngle = CMath::DegreeToRadians(GetAngle());
		// Calculate the camera target
		CVector3 vecTarget(m_pInterface->aimSyncData.vecPosition.fX - sin(fAngle) * 0.2f,
			m_pInterface->aimSyncData.vecPosition.fY + cos(fAngle) * 0.2f, m_pInterface->aimSyncData.vecPosition.fZ);
		
		// Calculate the camera front vector
		m_pInterface->aimSyncData.vecFront = vecTarget - m_pInterface->aimSyncData.vecPosition;
	}
	// Set the aim sync flag
	m_pInterface->bHasAimSync = TRUE;
}

bool CPlayer::SetState(BYTE byteState)
{
	// Make sure the state is valid
	if(byteState > PLAYER_STATE_SPAWNED)
		return false;

	// Set the new state
	m_pInterface->byteState = byteState;
	return true;
}

void CPlayer::Kill(int iKillerId, int iWeapon)
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
	SetState(PLAYER_STATE_DEAD);
	// Call the NPC death callback
	CCallbackManager::OnDeath((int)m_playerId, iKillerId, iWeapon);
}

void CPlayer::Process()
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
			CVector3 vecPosition;
			GetPosition(&vecPosition);
			// Get the player velocity
			CVector3 vecVelocity;
			GetVelocity(&vecVelocity);
			// Make sure we still need to move
			if((GetTickCount() - m_dwMoveStartTime) < m_dwMoveTime)
			{
				// Get the time spent since the last update
				DWORD dwThisTick = GetTickCount();
				DWORD dwTime = (dwThisTick - m_dwMoveTickCount);
				// Set the new position
				CVector3 vecNewPosition = vecPosition + (vecVelocity * (float)dwTime);
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
				CVector3 vecNewPosition = vecPosition + (vecVelocity * (float)dwTime);
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
				// Call the vehicle entry complete callback
				CCallbackManager::OnVehicleEntryComplete((int)m_playerId, (int)m_wVehicleToEnter, (int)m_byteSeatToEnter);
				// Set the player vehicle and seat
				m_pInterface->wVehicleId = m_wVehicleToEnter;
				m_pInterface->byteSeatId = m_byteSeatToEnter;
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
			if (dwTime >= 1500)
			{
				// Reset the reloading flag
				m_bReloading = false;
				// Start shooting again
				SetKeys(m_pInterface->wUDAnalog, m_pInterface->wLRAnalog, 4 + 0x80);
				// Update the shoot tick
				m_dwShootTickCount = dwThisTick;
				m_wAmmo = 0;
			}
		}
		// Process player shooting
		else if(m_bAiming && m_pInterface->dwKeys & 4)
		{	
			// Do we still have ammo ?
			if (m_wAmmo == 0)
			{
				// Check for infinite ammo flag
				if (!m_bHasInfiniteAmmo)
					// Stop shooting and aim only
					SetKeys(m_pInterface->wUDAnalog, m_pInterface->wLRAnalog, 0x80);
				else
					// This is done so the NPC would keep reloading even if the infinite ammo flag is set
					m_wAmmo = 500;
			}
			else
			{
				// Check the time spent since the last shoot
				DWORD dwThisTick = GetTickCount();
				DWORD dwTime = (dwThisTick - m_dwShootTickCount);
				if(dwTime >= 800) // TODO: Change that for each weapon rate of fire
				{
					// Decrease the ammo
					m_wAmmo--;
					// Get the weapon clip size
					DWORD dwClip = 0;
					if (m_pInterface->syncData.byteWeapon == 38 || m_pInterface->syncData.byteWeapon == 37 || m_pInterface->syncData.byteWeapon == 34)
						dwClip = m_wAmmo;
					else
						dwClip = CWeaponInfo::GetWeaponClipSize(m_pInterface->syncData.byteWeapon);
					
					
					// Check for reload
					if (m_wAmmo % dwClip == 0 && m_wAmmo != 0 && dwClip != m_wAmmo && m_bHasReload)
					{
						// Set the reload tick count
						m_dwReloadTickCount = GetTickCount();
						// Set reloading flag
						m_bReloading = true;
						// Stop shooting and aim only
						SetKeys(m_pInterface->wUDAnalog, m_pInterface->wLRAnalog, 0x80);
					}
					else
						// Send the bullet
						CSAMPFunctions::PlayerShoot((int)m_playerId, m_vecAimAt);

					// Update the shoot tick
					m_dwShootTickCount = dwThisTick;
				}
			}
		}
		// Process melee attack
		else if(m_bMeleeAttack)
		{
			// Get the time spent since last melee and compare it with the melee delay
			if((GetTickCount() - m_dwShootTickCount) >= m_dwMeleeDelay)
			{
				// Set the melee keys
				SetKeys(m_pInterface->wUDAnalog, m_pInterface->wLRAnalog, 4);
				// Update the tick count
				m_dwShootTickCount = GetTickCount();
			}
			else
				// Reset keys
				SetKeys(m_pInterface->wUDAnalog, m_pInterface->wLRAnalog, 0);
		}
		// Process death
		if(GetHealth() <= 0.0f && GetState() != PLAYER_STATE_DEAD && GetState() != PLAYER_STATE_SPAWNED)
		{
			// Get the last damager weapon
			BYTE byteWeapon = -1;
			if(m_iLastDamager != INVALID_ENTITY_ID)
			{
				CSAMPServer *pSAMPServer = (CSAMPServer *)CAddress::VAR_ServerPtr;
				byteWeapon = pSAMPServer->pPlayerPool->pPlayer[m_iLastDamager]->syncData.byteWeapon;
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
			if(m_pInterface->wVehicleId == INVALID_ENTITY_ID)
				return;

			// Get the player vehicle interface
			CSAMPServer *pSAMPServer = (CSAMPServer *)CAddress::VAR_ServerPtr;
			CSAMPVehicle *pVehicle = pSAMPServer->pVehiclePool->pVehicle[m_pInterface->wVehicleId];
			// Get the vehicle position
			CVector3 vecPosition;
			GetPosition(&vecPosition);
			// Get the vehicle velocity
			CVector3 vecVelocity;
			GetVelocity(&vecVelocity);
			// Make sure we still need to move
			if((GetTickCount() - m_dwMoveStartTime) < m_dwMoveTime)
			{
				// Get the time spent since the last update
				DWORD dwThisTick = GetTickCount();
				DWORD dwTime = (dwThisTick - m_dwMoveTickCount);
				// Set the new position
				CVector3 vecNewPosition = vecPosition + (vecVelocity * (float)dwTime);
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
				CVector3 vecNewPosition = vecPosition + (vecVelocity * (float)dwTime);
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
			CSAMPServer *pSAMPServer = (CSAMPServer *)CAddress::VAR_ServerPtr;
			CVector3 vecVehiclePos = pSAMPServer->pVehiclePool->pVehicle[m_pInterface->wVehicleId]->vecPosition;
			// Get the seat position
			CVector3 *pvecSeat = CSAMPFunctions::GetVehicleModelInfo(pSAMPServer->pVehiclePool->pVehicle[m_pInterface->wVehicleId]->iModelId,
				m_pInterface->byteSeatId == 0 || m_pInterface->byteSeatId == 1 ? VEHICLE_MODEL_INFO_FRONTSEAT : VEHICLE_MODEL_INFO_REARSEAT);
	
			// Adjust the seat vector
			CVector3 vecSeat(pvecSeat->fX + 0.8f, pvecSeat->fY, pvecSeat->fZ);
			if (m_pInterface->byteSeatId == 0 || m_pInterface->byteSeatId == 2)
				vecSeat.fX = -vecSeat.fX;

			// Get vehicle angle
			float fAngle = CMath::GetAngle(-pSAMPServer->pVehiclePool->pVehicle[m_pInterface->wVehicleId]->fRotationX, pSAMPServer->pVehiclePool->pVehicle[m_pInterface->wVehicleId]->fRotationY);
			// This is absolutely bullshit
			float _fAngle = fAngle * 0.01570796326794897f;
			// Calculate the seat position based on vehicle angle
			CVector3 vecSeatPosition(vecSeat.fX * cos(_fAngle) - vecSeat.fY * sin(_fAngle) + vecVehiclePos.fX,
				vecSeat.fX * sin(_fAngle) + vecSeat.fY * cos(_fAngle) + vecVehiclePos.fY, vecSeat.fZ + vecVehiclePos.fZ);

			// Set his position
			SetPosition(vecSeatPosition);
			// Set his angle
			SetAngle(fAngle);
			// Reset the player vehicle and seat id
			m_pInterface->wVehicleId = INVALID_ENTITY_ID;
			m_pInterface->byteSeatId = 0;	
			// Call the vehicle exit complete callback
			CCallbackManager::OnVehicleExitComplete((int)m_playerId);
		}
	}
}

void CPlayer::SetPosition(CVector3 vecPosition)
{
	// Check the player state
	if(GetState() == PLAYER_STATE_DRIVER && m_pInterface->wVehicleId != INVALID_ENTITY_ID)
	{
		// Get the player vehicle interface
		CSAMPServer *pSAMPServer = (CSAMPServer *)CAddress::VAR_ServerPtr;
		CSAMPVehicle *pVehicle = pSAMPServer->pVehiclePool->pVehicle[m_pInterface->wVehicleId];
		// Get the player vehicle position
		pVehicle->vecPosition = vecPosition;
	}
	// Set the player position
	m_pInterface->vecPosition = vecPosition;
}

void CPlayer::GetPosition(CVector3 *pvecPosition)
{
	// Check the player state
	if((GetState() == PLAYER_STATE_DRIVER ||  GetState() == PLAYER_STATE_PASSENGER) && m_pInterface->wVehicleId != INVALID_ENTITY_ID)
	{
		// Get the player vehicle interface
		CSAMPServer *pSAMPServer = (CSAMPServer *)CAddress::VAR_ServerPtr;
		CSAMPVehicle *pVehicle = pSAMPServer->pVehiclePool->pVehicle[m_pInterface->wVehicleId];
		// Get the player vehicle position
		*pvecPosition = pVehicle->vecPosition;
	}
	else
		*pvecPosition = m_pInterface->vecPosition;
}

void CPlayer::SetQuaternion(CVector3 vecQuaternion, float fAngle)
{
	// Check the player state
	if(GetState() == PLAYER_STATE_DRIVER && m_pInterface->wVehicleId != INVALID_ENTITY_ID)
	{
		// Get the player vehicle interface
		CSAMPServer *pSAMPServer = (CSAMPServer *)CAddress::VAR_ServerPtr;
		CSAMPVehicle *pVehicle = pSAMPServer->pVehiclePool->pVehicle[m_pInterface->wVehicleId];
		// Set the player vehicle quaternion
		pVehicle->vecQuaternion = vecQuaternion;
		pVehicle->fQuaternionAngle = fAngle;
	}
	// Set the player quaternion
	m_pInterface->syncData.vecQuaternion = vecQuaternion;
	m_pInterface->syncData.fQuaternionAngle = fAngle;
}

void CPlayer::GetQuaternion(CVector3 *pvecQuaternion, float *pfAngle)
{
	// Check the player state
	if((GetState() == PLAYER_STATE_DRIVER ||  GetState() == PLAYER_STATE_PASSENGER) && m_pInterface->wVehicleId != INVALID_ENTITY_ID)
	{
		// Get the player vehicle interface
		CSAMPServer *pSAMPServer = (CSAMPServer *)CAddress::VAR_ServerPtr;
		CSAMPVehicle *pVehicle = pSAMPServer->pVehiclePool->pVehicle[m_pInterface->wVehicleId];
		// Get the player vehicle quaternion
		*pvecQuaternion = pVehicle->vecQuaternion;
		*pfAngle = pVehicle->fQuaternionAngle;
	}
	else
	{
		// Get the player vehicle quaternion
		*pvecQuaternion = m_pInterface->syncData.vecQuaternion;
		*pfAngle = m_pInterface->syncData.fQuaternionAngle;
	}
}

void CPlayer::SetAngle(float fAngle)
{
	// Set the player
	m_pInterface->fAngle = fAngle;
	// Check the player state
	if(GetState() == PLAYER_STATE_DRIVER && m_pInterface->wVehicleId != INVALID_ENTITY_ID)
	{
		// Get the player vehicle interface
		CSAMPServer *pSAMPServer = (CSAMPServer *)CAddress::VAR_ServerPtr;
		CSAMPVehicle *pVehicle = pSAMPServer->pVehiclePool->pVehicle[m_pInterface->wVehicleId];
		// Set the vehicle angle
		pVehicle->vecQuaternion.fZ = CMath::AngleToQuaternion(fAngle);
	}
	// Set the sync angle
	m_pInterface->syncData.vecQuaternion.fZ = CMath::AngleToQuaternion(fAngle);
}

void CPlayer::SetHealth(float fHealth)
{
	// Validate the health
	if(fHealth < 0.0f)
		fHealth = 0.0f;
	else if(fHealth > 200.0f)
		fHealth = 200.0f;

	// Set the player health
	m_pInterface->fHealth = fHealth;
}

void CPlayer::SetArmour(float fArmour)
{
	// Validate the armour
	if(fArmour < 0.0f)
		fArmour = 0.0f;
	else if(fArmour > 200.0f)
		fArmour = 200.0f;

	// Set the player armour
	m_pInterface->fArmour = fArmour;
}

void CPlayer::SetSkin(int iSkin)
{
	// Make sure the skin has changed
	if(iSkin == m_pInterface->iSkinId)
		return;

	// Validate the skin
	if(iSkin > 311 || iSkin < 0)
		return;

	// Set the player skin
	m_pInterface->iSkinId = iSkin;
	// Respawn the player
	Respawn();
}

void CPlayer::SetInterior(int iInterior)
{
	// Set the player interior
	m_pInterface->iInteriorId = iInterior;
}

void CPlayer::SetWeapon(BYTE byteWeaponId)
{
	// Validate the weapon id
	if(byteWeaponId > 46)
		return;

	// Set the player weapon id
	m_byteWeaponId = byteWeaponId;
}

void CPlayer::SetAmmo(WORD wAmmo)
{
	// Validate the ammo
	if(wAmmo > 0xFFFF)
		return;

	// Set the player ammo
	m_wAmmo = wAmmo;
}

void CPlayer::SetSpecialAction(int iActionId)
{
	// Validate the action id
	if(iActionId < 0 || (iActionId > 13 && (iActionId != 20 && iActionId != 21 && iActionId != 22
		&& iActionId != 23 && iActionId != 24 && iActionId != 25 && iActionId != 68)))
		return;

	// Set the player action
	m_pInterface->syncData.byteSpecialAction = (BYTE)iActionId;
}

void CPlayer::SetVelocity(CVector3 vecVelocity)
{
	// Check the player state
	if(GetState() == PLAYER_STATE_DRIVER && m_pInterface->wVehicleId != INVALID_ENTITY_ID)
	{
		// Get the player vehicle interface
		CSAMPServer *pSAMPServer = (CSAMPServer *)CAddress::VAR_ServerPtr;
		CSAMPVehicle *pVehicle = pSAMPServer->pVehiclePool->pVehicle[m_pInterface->wVehicleId];
		// Set the player vehicle velocity
		pVehicle->vecVelocity = vecVelocity;
	}
	// Set the player velocity
	m_pInterface->vecVelocity = vecVelocity;
}

void CPlayer::GetVelocity(CVector3 *pvecVelocity)
{
	// Check the player state
	if((GetState() == PLAYER_STATE_DRIVER ||  GetState() == PLAYER_STATE_PASSENGER) && m_pInterface->wVehicleId != INVALID_ENTITY_ID)
	{
		// Get the player vehicle interface
		CSAMPServer *pSAMPServer = (CSAMPServer *)CAddress::VAR_ServerPtr;
		CSAMPVehicle *pVehicle = pSAMPServer->pVehiclePool->pVehicle[m_pInterface->wVehicleId];
		// Get the player vehicle position
		*pvecVelocity = pVehicle->vecVelocity;
	}
	else
		*pvecVelocity = m_pInterface->vecVelocity;
}

void CPlayer::SetKeys(WORD wUDAnalog, WORD wLRAnalog, DWORD dwKeys)
{
	// Save the keys
	m_pInterface->wUDAnalog = wUDAnalog;
	m_pInterface->wLRAnalog = wLRAnalog;
	m_pInterface->dwKeys = dwKeys;
}

void CPlayer::GetKeys(WORD *pwUDAnalog, WORD *pwLRAnalog, DWORD *pdwKeys)
{
	// Gzt the keys
	*pwUDAnalog = m_pInterface->wUDAnalog;
	*pwLRAnalog = m_pInterface->wLRAnalog;
	*pdwKeys = m_pInterface->dwKeys;
}

void CPlayer::GoTo(CVector3 vecPoint, int iType, bool bUseZMap)
{
	// Validate the mouvement type
	if(iType > MOVE_TYPE_DRIVE || iType < MOVE_TYPE_WALK)
		return;

	// Save the destination point
	m_vecDestination = vecPoint;
	// Get the moving type key and speed
	DWORD dwMoveKey = 0;
	float fMoveSpeed = MOVE_SPEED_RUN;
	if(iType == MOVE_TYPE_WALK)
	{
		fMoveSpeed = MOVE_SPEED_WALK;
		dwMoveKey = 0x400;
	}
	else if(iType == MOVE_TYPE_SPRINT)
	{
		fMoveSpeed = MOVE_SPEED_SPRINT;
		dwMoveKey = 0x8;
	}
	else if(iType == MOVE_TYPE_DRIVE)
	{
		fMoveSpeed = m_pInterface->vecVelocity.fX;
		dwMoveKey = 0;
	}
	// Set the moving keys
	if(iType != MOVE_TYPE_DRIVE)
		SetKeys(0x8000, 0x0000, dwMoveKey);
	else
		SetKeys(0x0000, 0x0000, 0);

	// Get the player position
	CVector3 vecPosition;
	GetPosition(&vecPosition);
	// Get the moving front vector
	CVector3 vecFront = m_vecDestination - vecPosition;
	// Get the distance to the destination point
	float fDistance = CMath::GetDistanceBetween3DPoints(vecPosition, m_vecDestination);	
	// Set the player to the destination angle
	vecFront /= fDistance;
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
}

void CPlayer::StopMoving()
{
	// Make sure the player is moving
	if(!m_bMoving)
		return;

	// Reset moving flag
	m_bMoving = false;
	// Reset analogs
	SetKeys(0, 0, m_pInterface->dwKeys);
	// Reset other moving variables
	m_dwMoveTime = 0;
	m_dwMoveStartTime = 0;
	memset(&m_vecDestination, 0, sizeof(CVector3));
	// Reset the ZMap usage
	m_bUseZMap = false;
}

void CPlayer::AimAt(CVector3 vecPoint, bool bShoot)
{
	// Set the aiming flag
	if(!m_bAiming)
	{
		// Get the shooting start tick
		m_dwShootTickCount = GetTickCount();
		m_bReloading = false;
	}
	// Adjust the player position
	CVector3 vecPosition = m_pInterface->vecPosition;
	// Save the aiming point
	m_vecAimAt = vecPoint;
	// Get the aiming distance
	CVector3 vecDistance = vecPoint - vecPosition;
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
	m_pInterface->aimSyncData.fZAim = fZAngle;
	// Set the player aiming vector
	m_pInterface->aimSyncData.vecFront = vecDistance;
	// Set the player camera position
	m_pInterface->aimSyncData.vecPosition = vecPosition;
	// Set keys
	if (!m_bAiming)
		SetKeys(m_pInterface->wUDAnalog, m_pInterface->wLRAnalog, bShoot ? 4 + 0x80 : 0x80);

	// Mark as aiming
	m_bAiming = true;
}

void CPlayer::StopAim()
{
	// Make sure the player is aiming
	if(!m_bAiming)
		return;

	// Reset aiming flags
	m_bAiming = false;
	m_bReloading = false;
	// Reset keys
	SetKeys(m_pInterface->wUDAnalog, m_pInterface->wLRAnalog, 0);
}

void CPlayer::MeleeAttack(DWORD dwTime)
{
	// Make sure the player is not melee attacking
	if(m_bMeleeAttack)
		return;

	// Validate the player state
	if(GetState() != PLAYER_STATE_ONFOOT)
		return;

	// Validate the player weapon
	if(m_pInterface->syncData.byteWeapon >= 0 && m_pInterface->syncData.byteWeapon <= 15)
	{
		// Set the attacking delay
		m_dwMeleeDelay = dwTime;
		if(dwTime <= pServer->GetUpdateRate())
			m_dwMeleeDelay = pServer->GetUpdateRate() + 5;

		// Get the starting tick
		m_dwShootTickCount = GetTickCount();
		// Set the melee flag
		m_bMeleeAttack = true;
		// Reset the aiming flag
		m_bAiming = false;
		// Set the melee keys
		SetKeys(m_pInterface->wUDAnalog, m_pInterface->wLRAnalog, 4);
	}
}

void CPlayer::StopAttack()
{
	// Make sure the player is attacking
	if(!m_bMeleeAttack)
		return;

	// Reset attacking flag
	m_bMeleeAttack = false;
	// Reset keys
	SetKeys(m_pInterface->wUDAnalog, m_pInterface->wLRAnalog, 0);
}

void CPlayer::ProcessDamage(int iDamagerId, float fHealthLoss, int iWeaponId, int iBodypart)
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

bool CPlayer::EnterVehicle(int iVehicleId, int iSeatId, int iType)
{
	// Validate the vehicle
	if(iVehicleId < 1 || iVehicleId > MAX_VEHICLES)
		return false;

	// Validate the player state
	if(GetState() != PLAYER_STATE_ONFOOT)
		return false;

	// Validate the entering type
	if(iType > MOVE_TYPE_SPRINT || iType < MOVE_TYPE_WALK)
		iType = MOVE_TYPE_RUN;

	// Validate the vehicle
	CSAMPServer *pSAMPServer = (CSAMPServer *)CAddress::VAR_ServerPtr;
	if(!pSAMPServer->pVehiclePool->pVehicle[iVehicleId])
		return false;

	// TODO: Validate the seat id
	if(iSeatId < 0 || iSeatId > 3)
		return false;

	// Validate the distance to enter
	if(CMath::GetDistanceBetween3DPoints(pSAMPServer->pVehiclePool->pVehicle[iVehicleId]->vecPosition,
		m_pInterface->vecPosition) > MAX_DISTANCE_TO_ENTER_VEHICLE)
		return false;

	// Save the entering stats
	m_wVehicleToEnter = (WORD)iVehicleId;
	m_byteSeatToEnter = (BYTE)iSeatId;
	// Get the seat position
	CVector3 *pvecSeat = CSAMPFunctions::GetVehicleModelInfo(pSAMPServer->pVehiclePool->pVehicle[iVehicleId]->iModelId,
		iSeatId == 0 || iSeatId == 1 ? VEHICLE_MODEL_INFO_FRONTSEAT : VEHICLE_MODEL_INFO_REARSEAT);

	// Adjust the seat vector
	CVector3 vecSeat(pvecSeat->fX + 0.7f, pvecSeat->fY, pvecSeat->fZ);
	if (iSeatId == 0 || iSeatId == 2)
		vecSeat.fX = -vecSeat.fX;

	// Get vehicle angle
	float fAngle = CMath::GetAngle(-pSAMPServer->pVehiclePool->pVehicle[iVehicleId]->fRotationX, pSAMPServer->pVehiclePool->pVehicle[iVehicleId]->fRotationY);
	// This is absolutely bullshit
	float _fAngle = fAngle * 0.01570796326794897f;
	// Calculate the seat position based on vehicle angle
	CVector3 vecSeatPosition(vecSeat.fX * cos(_fAngle) - vecSeat.fY * sin(_fAngle), vecSeat.fX * sin(_fAngle) + vecSeat.fY * cos(_fAngle), vecSeat.fZ);
	// Calculate the destination point
	CVector3 vecDestination = pSAMPServer->pVehiclePool->pVehicle[iVehicleId]->vecPosition + vecSeatPosition;
	// Go to the vehicle
	GoTo(vecDestination, iType, true);
	return true;
}

bool CPlayer::ExitVehicle()
{
	// Validate the player state
	if(GetState() != PLAYER_STATE_DRIVER && GetState() != PLAYER_STATE_PASSENGER)
		return false;

	// Validate the player vehicle
	CSAMPServer *pSAMPServer = (CSAMPServer *)CAddress::VAR_ServerPtr;
	if(!pSAMPServer->pVehiclePool->pVehicle[m_pInterface->wVehicleId])
		return false;

	// Call the SAMP exit vehicle function
	CSAMPFunctions::PlayerExitVehicle(m_playerId, m_pInterface->wVehicleId);
	// Set the player state
	SetState(PLAYER_STATE_EXIT_VEHICLE);
	// Set the exit start tick
	m_dwEnterExitTickCount = GetTickCount();
	return true;
}

bool CPlayer::PutInVehicle(int iVehicleId, int iSeatId)
{
	// Validate the vehicle
	if(iVehicleId < 0 || iVehicleId >= MAX_VEHICLES)
		return false;

	// Validate the player state
	if(GetState() != PLAYER_STATE_ONFOOT)
		return false;

	// Validate the vehicle
	CSAMPServer *pSAMPServer = (CSAMPServer *)CAddress::VAR_ServerPtr;
	if(!pSAMPServer->pVehiclePool->pVehicle[iVehicleId])
		return false;

	// TODO: Validate the seat id
	if(iSeatId < 0 || iSeatId > 3)
		return false;

	// Set the player vehicle and seat id
	m_pInterface->wVehicleId = (WORD)iVehicleId;
	m_pInterface->byteSeatId = (BYTE)iSeatId;
	// Set the player state
	SetState(iSeatId == 0 ? PLAYER_STATE_DRIVER : PLAYER_STATE_PASSENGER);
	return true;
}

bool CPlayer::RemoveFromVehicle()
{
	// Validate the player state
	if(GetState() != PLAYER_STATE_DRIVER && GetState() != PLAYER_STATE_PASSENGER)
		return false;

	// Validate the player vehicle
	CSAMPServer *pSAMPServer = (CSAMPServer *)CAddress::VAR_ServerPtr;
	if(!pSAMPServer->pVehiclePool->pVehicle[m_pInterface->wVehicleId])
		return false;

	// Set the player state
	SetState(PLAYER_STATE_ONFOOT);
	CVector3 vecVehiclePos = pSAMPServer->pVehiclePool->pVehicle[m_pInterface->wVehicleId]->vecPosition;
	SetPosition(CVector3(vecVehiclePos.fX + 2.0f, vecVehiclePos.fY + 2.0f, vecVehiclePos.fZ));
	// Reset the player vehicle and seat id
	m_pInterface->wVehicleId = INVALID_ENTITY_ID;
	m_pInterface->byteSeatId = 0;
	return true;
}

bool CPlayer::StartPlayingPlayback(char *szFile)
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

void CPlayer::StopPlayingPlayback()
{
	// Make sure the player is Playing
	if(!m_bPlaying)
		return;

	// Delete the playback instance
	SAFE_DELETE(m_pPlayback);
	// Reset the player data
	SetVelocity(CVector3(0.0f, 0.0f, 0.0f));
	SetKeys(0, 0, 0);
	// Reset the Playing flag
	m_bPlaying = false;
	// Call the playback finish callback
	CCallbackManager::OnFinishPlayback((int)m_playerId);
}

void CPlayer::PausePlayingPlayback()
{
	// Make sure the player is Playing
	if(!m_bPlaying)
		return;

	// Pause the playback
	m_pPlayback->SetPaused(true);
}

void CPlayer::ResumePlayingPlayback()
{
	// Make sure the player is Playing
	if(!m_bPlaying)
		return;

	// Resume the playback
	m_pPlayback->SetPaused(false);
}

bool CPlayer::PlayNode(int iNodeId, int iType)
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
	CVector3 vecStart;
	m_pNode->GetPosition(&vecStart);
	// Save the node moving velocity
	m_vecNodeVelocity = m_pInterface->vecVelocity;
	// Set the player in the starting point
	SetPosition(vecStart);
	// Get the node link and set it to the next position
	m_pNode->SetLink(m_pNode->GetLinkId());
	m_pNode->SetPoint(m_pNode->GetLinkPoint());
	// Save the current point
	m_iNodePoint = m_pNode->GetLinkPoint();
	// Get the destination point
	CVector3 vecDestination;
	m_pNode->GetPosition(&vecDestination);
	// Move the player to it
	GoTo(vecDestination, iType == NODE_TYPE_PED ? MOVE_TYPE_WALK : MOVE_TYPE_DRIVE, false);
	// Set the player as playing node
	m_bPlayingNode = true;
	return true;
}

void CPlayer::StopPlayingNode()
{
	// Make sure the player is playing a node
	if(!m_bPlayingNode)
		return;

	// Reset the node instance
	m_pNode = NULL;
	// Reset the player data
	SetKeys(0, 0, 0);
	SetVelocity(CVector3(0.0f, 0.0f, 0.0f));
	m_vecNodeVelocity = CVector3();
	// Reset the node flag
	m_bPlayingNode = false;
	m_iNodePoint = 0;
	m_iNodeLastPoint = 0;
	m_iNodeType = 0;
	// Call the node finish callback
	CCallbackManager::OnFinishNode((int)m_playerId);
}

int CPlayer::ChangeNode(int iNodeId, unsigned short usLinkId)
{
	// Make sure the player is playing a node
	if(!m_bPlayingNode)
		return 0;

	// Get the node instance
	m_pNode = pServer->GetNodeManager()->GetAt(iNodeId);
	// Process the node change
	return m_pNode->ProcessNodeChange(this, usLinkId, m_iNodeType, m_vecNodeVelocity);
}
