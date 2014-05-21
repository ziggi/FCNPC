/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: Server.cpp
	- Author(s): OrMisicL

  =========================================*/

#include "Main.h"

CSAMPRPCParams		*pCreateNPCParams;
extern logprintf_t	logprintf;

CServer::CServer()
{
	// Reset instances
	m_pPlayerManager = NULL;
	m_pNodeManager = NULL;
	m_pDamageThread = NULL;
	m_pZMap = NULL;
	pCreateNPCParams = NULL;
	// Initialize the update rate
	m_dwUpdateRate = DEFAULT_UPDATE_RATE;
}

CServer::~CServer()
{
	// Delete instance
	SAFE_DELETE(m_pPlayerManager);
	SAFE_DELETE(m_pNodeManager);
	SAFE_DELETE(m_pDamageThread);
	SAFE_DELETE(m_pZMap);
	SAFE_DELETE(pCreateNPCParams);
}

BYTE CServer::Initialize()
{
	// Detect samp server version
	eSAMPVersion sampVersion = SAMP_VERSION_UNKNOWN;
#ifdef _WIN32
	if (*(DWORD *)0x45A1B0 == 0x24748B56) 
	{ 
		sampVersion = SAMP_VERSION_03Z_R2; 
		logprintf("SAMP Server version 0.3z R2-2 (Windows) detected. Initializing ..."); 
	} 
	else
		return 6;
#else
	if(*(DWORD *)0x80A7577 == 0x01F4FB81)
	{
		sampVersion = SAMP_VERSION_03Z_R2;
		logprintf("SAMP Server version 0.3z R2-2 (Linux) detected. Initializing ...");
	}
	else if(*(DWORD *)0x80A7577 == 0x03E8FB81)
	{
		sampVersion = SAMP_VERSION_03Z_R2_1000P;
		logprintf("SAMP Server version 0.3z R2-2 1000p (Linux) detected. Initializing ...");
	}
	else
		return 6;
#endif

	// Create the ZMap instance
	m_pZMap = new CZMap();
	if(!m_pZMap)
		return 1;

	// Initialize addresses
	CAddress::Initialize(sampVersion);
	// Initialize SAMP Functions
	CSAMPFunctions::Initialize();
	// Install hooks
	CHooks::InstallHooks();
	// Install patches
	CPatches::InstallPatches();
	// Create the player manager instance
	m_pPlayerManager = new CPlayerManager();
	if(!m_pPlayerManager)
		return 2;

	// Create the node manager instance
	m_pNodeManager = new CNodeManager();
	if(!m_pNodeManager)
		return 3;

/*	// Create threads
	m_pDamageThread = new CThread(CThreadFunctions::DamageThread);
	if(!m_pDamageThread || !m_pDamageThread->Start())
		return 4;*/

	// Create RPC params instance (We do it here only once saves us from news and deletes for each NPC)
	pCreateNPCParams = new CSAMPRPCParams();
	if(!pCreateNPCParams)
		return 5;

	// Push all the basic parameters
	pCreateNPCParams->Write<int>(SAMP_NETVERSION);
	pCreateNPCParams->Write<BYTE>(1);
	// Check the maxnpc from the config
	if(CSAMPFunctions::GetMaxNPC() == 0)
		// Display a warning
		logprintf("Warning: the maxnpc limit is 0 (you will not be able to create NPCs unless you change it)");
	// Check the maxnpc and maxplayers in the config
	else if(CSAMPFunctions::GetMaxPlayers() < CSAMPFunctions::GetMaxNPC())
		// Display a warning
		logprintf("Warning: the maxplayers limit is less than maxnpc (possible crash)");

	return 0;
}

/*int CServer::ExecuteCallback(char *szCallback, char *szFormat, ...)
{
	logprintf("start");
	int iReturn = 1;
	// Loop through all the AMX instances
	int iIndex = -1;
	for(std::list<AMX *>::iterator i = m_listAMX.begin(); i != m_listAMX.end(); i++)
	{
		// Get the function index
		if(!amx_FindPublic((*i), szCallback, &iIndex))
		{
			logprintf("found public");
			// Copy the format string
			char *_szFormat = szFormat;
			cell cStringAddr = NULL;
			// Create the arguments list
			va_list vArgs;
			va_start(vArgs, szFormat);
			// Keep looping until we finish processing all arguments
			while(*_szFormat != NULL)
			{
				logprintf("proc format (%s) on %c", _szFormat, *_szFormat);
				if(*_szFormat == 'i') // Integer
					amx_Push((*i), (cell)va_arg(vArgs, int));
				else if(*_szFormat == 'f') // Float
				{
					float fValue = va_arg(vArgs, float);
					amx_Push((*i), amx_ftoc(fValue));
				}
				else if(*_szFormat == 's') // String
				{
					logprintf("string it is");
					const char *szValue = va_arg(vArgs, const char *);
					logprintf("have string (%s)", szValue);
					//amx_PushString((*i), &cStringAddr, NULL, szValue, NULL, NULL);
					// NOTE: For some reason, amx_PushString seems to crash the server, so push it as an array for now
					// Allocate the heap for the string
					cell *pPhysAddr;
					amx_Allot((*i), strlen(szValue), &cStringAddr, &pPhysAddr);
					logprintf("allocated %d on 0x%x", strlen(szValue), pPhysAddr);
					// Copy the string into our allocated heap
					memcpy(pPhysAddr, szValue, strlen(szValue));
					logprintf("copied");
					// Push it in the arguments stack
					amx_Push((*i), cStringAddr);
					logprintf("pushed");

				}
				// Increase the format current character
				_szFormat++; 
			}
			logprintf("out");
			// End the arguments list
			va_end(vArgs);
			// Execute the callback
			cell cReturn;
			amx_Exec((*i), &cReturn, iIndex);
			logprintf("called");
			// Release the string heap
			if(cStringAddr)
				amx_Release((*i), cStringAddr);

			// Save it for the last return
			if(!cReturn)
				iReturn = cReturn;
		}
	}
	return iReturn;
}*/

bool CServer::DoesNameExist(char *szName)
{
	// Get the server interface
	CSAMPServer *pSAMPServer = *(CSAMPServer **)CAddress::VAR_ServerPtr;
	// Loop through all the players
	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		// Ignore non connected players
		if(!pSAMPServer->pPlayerPool->bIsPlayerConnected[i])
			continue;

		// Compare names
		if(!strcmp(szName, pSAMPServer->pPlayerPool->szName[i]))
			return true;
	}
	return false;
}

bool CServer::IsVehicleSeatOccupied(int iPlayerId, WORD wVehicleId, BYTE byteSeatId)
{
	// Get the server interface
	CSAMPServer *pSAMPServer = *(CSAMPServer **)CAddress::VAR_ServerPtr;
	// Loop through all the players
	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		// Ignore non connected players and the same player
		if(!pSAMPServer->pPlayerPool->bIsPlayerConnected[i] || iPlayerId == i)
			continue;

		// Get the player interface
		CSAMPPlayer *pPlayer = pSAMPServer->pPlayerPool->pPlayer[i];
		// Check vehicle and seat
		if(pPlayer->wVehicleId == wVehicleId && pPlayer->byteSeatId == byteSeatId) 
			return true;
	}
	return false;
}