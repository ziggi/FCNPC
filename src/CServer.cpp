/* =========================================

		FCNPC - Fully Controllable NPC
			----------------------

	- File: Server.cpp
	- Author(s): OrMisicL

  =========================================*/

#include "Main.hpp"

extern logprintf_t logprintf;
extern CNetGame *pNetGame;

CServer::CServer(eSAMPVersion version)
{
	m_iTicks = 0;
	m_iTickRate = DEFAULT_TICK_RATE;

	m_Version = version;
	// Reset instances
	m_pPlayerDataManager = NULL;
	m_pNodeManager = NULL;
	m_pMovePath = NULL;
	m_pRecordManager = NULL;
	m_pMapAndreas = NULL;
	// Initialize the update rate
	m_dwUpdateRate = DEFAULT_UPDATE_RATE;
	// enable crashlog by default
	m_bCrashLogCreation = true;
	// Initialize random seed
	srand(static_cast<unsigned int>(time(NULL)));
}

CServer::~CServer()
{
	// Delete instance
	SAFE_DELETE(m_pPlayerDataManager);
	SAFE_DELETE(m_pNodeManager);
	SAFE_DELETE(m_pMovePath);
	SAFE_DELETE(m_pRecordManager);
	SAFE_DELETE(m_pMapAndreas);
}

BYTE CServer::Initialize(AMX *pAMX)
{
	// Check for native usage
	AMX_HEADER * pAmxHeader = reinterpret_cast<AMX_HEADER *>(pAMX->base);
	AMX_FUNCSTUBNT *pAmxNativeTable = reinterpret_cast<AMX_FUNCSTUBNT *>(pAMX->base + pAmxHeader->natives);
	char *szName;
	bool bIsHaveNatives = false;
	int iNativesCount;
	amx_NumNatives(pAMX, &iNativesCount);

	for (int i = 0; i < iNativesCount; i++) {
		szName = reinterpret_cast<char *>(pAMX->base + pAmxNativeTable[i].nameofs);
		if (strstr(szName, "FCNPC_") != NULL) {
			bIsHaveNatives = true;
			break;
		}
	}

	// Check include version
	if (bIsHaveNatives) {
		int iIncludeVersion = 0;
		cell cellIndex;
		if (!amx_FindPubVar(pAMX, "FCNPC_IncludeVersion", &cellIndex)) {
			cell *pAddress = NULL;
			if (!amx_GetAddr(pAMX, cellIndex, &pAddress)) {
				iIncludeVersion = *pAddress;
			}
		}
		if (iIncludeVersion != INCLUDE_VERSION) {
			return ERROR_INCLUDE_VERSION;
		}
	}

	// Initialize necessary samp functions
	CFunctions::PreInitialize();
	// Initialize addresses
	CAddress::Initialize(CServer::GetVersion());
	// Initialize SAMP Functions
	CFunctions::Initialize();
	// Install hooks
	CHooks::InstallHooks();
	// Create the player manager instance
	m_pPlayerDataManager = new CPlayerManager();
	if (!m_pPlayerDataManager) {
		return ERROR_PLAYER_MANAGER_FAIL;
	}

	// Create the node manager instance
	m_pNodeManager = new CNodeManager();
	if (!m_pNodeManager) {
		return ERROR_NODE_MANAGER_FAIL;
	}

	// Create the move path instance
	m_pMovePath = new CMovePath;

	// Create the record instance
	m_pRecordManager = new CRecordManager;

	// Check the maxnpc from the config
	if (CFunctions::GetMaxNPC() == 0) {
		logprintf("[FCNPC] Warning: Unable to create NPCs. The maxnpc limit in server.cfg is 0.");
	}
	// Check the maxnpc and maxplayers in the config
	else if (CFunctions::GetMaxPlayers() < CFunctions::GetMaxNPC()) {
		logprintf("[FCNPC] Warning: Crash possible. The maxplayers limit in server.cfg is less than the maxnpc limit.");
	}

	return ERROR_NO;
}

void CServer::Process()
{
	if (m_iTickRate == -1) {
		return;
	}

	if (++m_iTicks >= m_iTickRate) {
		m_iTicks = 0;
		// Process the player manager
		pServer->GetPlayerManager()->Process();
	}
}

CPlayerManager *CServer::GetPlayerManager()
{
	return m_pPlayerDataManager;
}

CNodeManager *CServer::GetNodeManager()
{
	return m_pNodeManager;
}

CRecordManager *CServer::GetRecordManager()
{
	return m_pRecordManager;
}

CMovePath *CServer::GetMovePath()
{
	return m_pMovePath;
}

void CServer::ToggleCrashLogCreation(bool enabled)
{
	if (m_bCrashLogCreation) {
		if (!enabled) {
			CExceptionHandler::UnInstall();
		}
	} else {
		if (enabled) {
			CExceptionHandler::Install();
		}
	}

	m_bCrashLogCreation = enabled;
}

bool CServer::GetCrashLogCreation()
{
	return m_bCrashLogCreation;
}

bool CServer::IsValidNickName(char *szName)
{
	int iLength = strlen(szName);
	if (iLength < 1 || iLength > MAX_PLAYER_NAME) {
		return false;
	}
	return true;
}

bool CServer::DoesNameExist(char *szName)
{
	// Loop through all the players
	for (WORD i = 0; i <= pNetGame->pPlayerPool->dwPlayerPoolSize; i++) {
		// Ignore non connected players
		if (!pNetGame->pPlayerPool->bIsPlayerConnectedEx[i]) {
			continue;
		}

		// Compare names
		if (!strcmp(szName, pNetGame->pPlayerPool->szName[i])) {
			return true;
		}
	}
	return false;
}

bool CServer::SetTickRate(int iRate)
{
	if (iRate < 0) {
		return false;
	}
	m_iTickRate = iRate;
	return true;
}

int CServer::GetTickRate()
{
	return m_iTickRate;
}

bool CServer::SetUpdateRate(DWORD dwRate)
{
	if (dwRate < 0) {
		return false;
	}
	m_dwUpdateRate = dwRate;
	return true;
}

DWORD CServer::GetUpdateRate()
{
	return m_dwUpdateRate;
}

void CServer::SetMapAndreas(CMapAndreas *pMapAndreas)
{
	m_pMapAndreas = pMapAndreas;
}

CMapAndreas *CServer::GetMapAndreas()
{
	return m_pMapAndreas;
}

bool CServer::IsMapAndreasInited()
{
	return m_pMapAndreas != NULL && m_pMapAndreas->IsInited();
}

bool CServer::IsVehicleSeatOccupied(WORD wPlayerId, WORD wVehicleId, BYTE byteSeatId)
{
	WORD wSeatPlayerId = GetVehicleSeatPlayerId(wVehicleId, byteSeatId);

	if (wSeatPlayerId != wPlayerId && wSeatPlayerId != INVALID_PLAYER_ID) {
		return true;
	}

	return false;
}

WORD CServer::GetVehicleSeatPlayerId(WORD wVehicleId, BYTE byteSeatId)
{
	if (wVehicleId < 1 || wVehicleId > MAX_VEHICLES) {
		return INVALID_PLAYER_ID;
	}

	CPlayer *pPlayer;

	// Loop through all the players
	for (WORD i = 0; i <= pNetGame->pPlayerPool->dwPlayerPoolSize; i++) {
		// Ignore non connected players
		if (!pServer->GetPlayerManager()->IsPlayerConnected(i)) {
			continue;
		}

		// Get the player interface
		pPlayer = pNetGame->pPlayerPool->pPlayer[i];

		// Check vehicle and seat
		if (pPlayer->wVehicleId == wVehicleId && pPlayer->byteSeatId == byteSeatId) {
			return pPlayer->wPlayerId;
		}
	}

	return INVALID_PLAYER_ID;
}

float CServer::GetVehicleAngle(CVehicle *pVehicle)
{
	float fAngle;

	bool bIsBadMatrix = pVehicle->vehMatrix.up.fX == 0.0f && pVehicle->vehMatrix.up.fY == 0.0f;
	bool bIsTrain = pVehicle->customSpawn.dwModelID == 537 || pVehicle->customSpawn.dwModelID == 538;

	if (bIsBadMatrix || bIsTrain) {
		fAngle = pVehicle->customSpawn.fRot;
	} else {
		fAngle = CMath::GetAngle(pVehicle->vehMatrix.up.fX, pVehicle->vehMatrix.up.fY);
	}

	return fAngle;
}

CVector CServer::GetVehiclePos(CVehicle *pVehicle)
{
	if (pVehicle->vehMatrix.up.fX == 0.0f && pVehicle->vehMatrix.up.fY == 0.0f) {
		return pVehicle->customSpawn.vecPos;
	}

	return pVehicle->vecPosition;
}

CVector CServer::GetVehicleSeatPos(CVehicle *pVehicle, BYTE byteSeatId)
{
	// Get the seat position
	CVector *pvecSeat;

	if (byteSeatId == 0 || byteSeatId == 1) {
		pvecSeat = CFunctions::GetVehicleModelInfoEx(pVehicle->customSpawn.dwModelID, VEHICLE_MODEL_INFO_FRONTSEAT);
	} else {
		pvecSeat = CFunctions::GetVehicleModelInfoEx(pVehicle->customSpawn.dwModelID, VEHICLE_MODEL_INFO_REARSEAT);
	}

	// Adjust the seat vector
	CVector vecSeat(pvecSeat->fX + 1.3f, pvecSeat->fY - 0.6f, pvecSeat->fZ);

	if (byteSeatId == 0 || byteSeatId == 2) {
		vecSeat.fX = -vecSeat.fX;
	}

	// Get vehicle angle
	float fAngle = pServer->GetVehicleAngle(pVehicle);
	float _fAngle = fAngle * 0.01570796326794897f;

	CVector vecSeatPosition(vecSeat.fX * cos(_fAngle) - vecSeat.fY * sin(_fAngle),
	                        vecSeat.fX * sin(_fAngle) + vecSeat.fY * cos(_fAngle),
	                        vecSeat.fZ);

	return vecSeatPosition + pServer->GetVehiclePos(pVehicle);
}

eSAMPVersion CServer::GetVersion()
{
	return m_Version;
}
