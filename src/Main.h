/* =========================================

		FCNPC - Fully Controllable NPC
			----------------------

	- File: Main.h
	- Author(s): OrMisicL

  =========================================*/

// OS includes
#ifdef _WIN32
	#include <windows.h>
#else
	#include "pthread.h"
	#include "unistd.h"
	#include <sys/mman.h>
#endif

// Define some linux types
#ifndef _WIN32
	#define BOOL    int
	#define DWORD   unsigned long
	#define BYTE    unsigned char
	#define WORD    unsigned short
	#define TRUE    (1)
	#define FALSE   (0)
	#define STDCALL
	#define THISCALL
	#define MAX_PATH 260
#else
	#define STDCALL __stdcall
	#define THISCALL __thiscall
#endif

// Standard includes
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>
#include "subhook/subhook.h"
#include "strlcpy/strlcpy.h"
#ifdef _WIN32
	#include "snprintf/snprintf.h"
#else
	#include "fopen_s/fopen_s.h"
#endif
// ExceptionHandler includes
#include "ExceptionHandler.h"
// SDK includes
#include <sdk/plugin.h>
// Utils includes
#include "Utils.h"
#include "Common.h"
#include "CVector.h"
#include "Thread.h"
#include "raknet/BitStream.h"
// Nodes includes
#include "Playback.h"
#include "MapAndreas.h"
#include "Node.h"
// Server includes
#include "Address.h"
#include "CServer.h"
#include "Patches.h"
#include "Hooks.h"
#include "VehicleInfo.h"
#include "WeaponInfo.h"
#include "ThreadFunctions.h"
// SAMP includes
#include "SAMPRakPeer.h"
#include "CFunctions.h"
#include "Structs.h"
#include "RPCs.h"
// SA includes
#include "SANode.h"
#include "Maths.h"

// Managers includes
#include "PlayerManager.h"
#include "NodeManager.h"
#include "CallbackManager.h"
// Entity includes
#include "CPlayerData.h"
// Scripting includes
#include "Natives.h"

// externals
extern CServer          *pServer;
extern void             **ppPluginData;
extern CNetGame         *pNetGame;
extern void             *pConsole;
extern RakServer        *pRakServer;
