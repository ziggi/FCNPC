// Bet this is it here

#ifndef _INCL_COLANDREAS
#define _INCL_COLANDREAS

//#pragma warning (disable:4005 700 996)
#if (defined(WIN32) || defined(_WIN32) || defined(_WIN64))
#include <windows.h>
#else
#include "pthread.h"
#endif
#include "sstream"
#if defined(LINUX) || defined(FREEBSD) || defined(__FreeBSD__) || defined(__OpenBSD__)
#include "stdarg.h"
#endif

// Standard includes
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <vector>

extern bool colInit;
extern bool colDataLoaded;
extern int32_t nullAddress;


#endif