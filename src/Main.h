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
#else
	#define STDCALL __stdcall
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
#include "SDK/plugin.h"
// Utils includes
#include "Utils.h"
#include "Maths.h"
#include "Common.h"
#include "Thread.h"
#include "BitStream.h"
// Nodes includes
#include "Playback.h"
#include "ZMap.h"
#include "Node.h"
// Server includes
#include "Address.h"
#include "Server.h"
#include "Patches.h"
#include "Hooks.h"
#include "VehicleInfo.h"
#include "WeaponInfo.h"
#include "ThreadFunctions.h"
	// SAMP includes
	#include "SAMPServer.h"
	#include "SAMPPlayerPool.h"
	#include "SAMPVehiclePool.h"
	#include "SAMPPlayer.h"
	#include "SAMPVehicle.h"
	#include "SAMPRakPeer.h"
	#include "SAMPRPCParams.h"
	#include "SAMPFunctions.h"
	#include "SAMPBulletSync.h"
	// SA includes
	#include "SANode.h"

// Managers includes
#include "PlayerManager.h"
#include "NodeManager.h"
#include "CallbackManager.h"
// Entity includes
#include "Player.h"
// Scripting includes
#include "Natives.h"

