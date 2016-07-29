/* =========================================

		FCNPC - Fully Controllable NPC
			----------------------

	- File: Main.hpp
	- Author(s): OrMisicL

  =========================================*/

// OS includes
#if defined(WIN32)
	#include <windows.h>

	#define STDCALL __stdcall
	#define THISCALL __thiscall
#elif defined(LINUX)
	#include "pthread.h"
	#include "unistd.h"
	#include <sys/mman.h>

	#define BOOL    int
	#define DWORD   unsigned long
	#define BYTE    unsigned char
	#define WORD    unsigned short
	#define TRUE    (1)
	#define FALSE   (0)
	#define STDCALL
	#define THISCALL
	#define MAX_PATH 260
#endif

// Standard includes
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <cmath>
#include <cstring>
#include "subhook/subhook.h"
#include "strlcpy/strlcpy.h"
#if defined(WIN32)
	#include "snprintf/snprintf.h"
#elif defined(LINUX)
	#include "fopen_s/fopen_s.h"
#endif
// ExceptionHandler includes
#include "ExceptionHandler.hpp"
// SDK includes
#include <sdk/plugin.h>
// Utils includes
#include "Utils.hpp"
#include "Common.h"
#include "CVector.h"
#include "raknet/BitStream.h"
// Nodes includes
#include "Playback.hpp"
#include "MapAndreas.h"
#include "Node.hpp"
// Server includes
#include "Address.hpp"
#include "CServer.hpp"
#include "Patches.hpp"
#include "Hooks.hpp"
#include "VehicleInfo.hpp"
#include "WeaponInfo.hpp"
#include "AnimationInfo.hpp"
// SAMP includes
#include "SAMPRakPeer.hpp"
#include "CFunctions.hpp"
#include "Structs.h"
#include "RPCs.h"
// SA includes
#include "CNode.hpp"
#include "Maths.hpp"

// Managers includes
#include "PlayerManager.hpp"
#include "NodeManager.hpp"
#include "CallbackManager.hpp"
// Entity includes
#include "CPlayerData.hpp"
// Scripting includes
#include "Natives.hpp"

// externals
extern CServer      *pServer;
extern void         **ppPluginData;
extern CNetGame     *pNetGame;
extern void         *pConsole;
extern RakServer    *pRakServer;
extern char         szSampVersion[64];
