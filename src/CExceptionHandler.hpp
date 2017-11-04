/* =========================================

FCNPC - Fully Controllable NPC
----------------------

- File: ExceptionHandler.hpp
- Author(s): OrMisicL

=========================================*/

#ifndef EXCEPTIONHANDLER_H
#define EXCEPTIONHANDLER_H

#if defined(LINUX)
	#include <signal.h>
	#include <time.h>
	#include <execinfo.h>

	typedef struct _sig_ucontext {
		unsigned long     uc_flags;
		struct ucontext   *uc_link;
		stack_t           uc_stack;
		struct sigcontext uc_mcontext;
		sigset_t          uc_sigmask;
	} sig_ucontext_t;
#endif

class CExceptionHandler
{
public:
	static void Install();
	static void UnInstall();

	// Exception handler callback
#if defined(WIN32)
	static long WINAPI ExceptionHandlerCallback(_EXCEPTION_POINTERS *pExceptionInfo);
#elif defined(LINUX)
	static void ExceptionHandlerCallback(int param, siginfo_t * info, void * ucontext);
#endif
};

#endif
