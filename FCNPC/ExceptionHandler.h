/* =========================================

FCNPC - Fully Controllable NPC
----------------------

- File: ExceptionHandler.h
- Author(s): OrMisicL

=========================================*/

#ifndef EXCEPTIONHANDLER_H
#define EXCEPTIONHANDLER_H

class CExceptionHandler
{
public:
	static void Install();

	// Exception handler callback
#ifdef _WIN32
	static long WINAPI ExceptionHandlerCallback(_EXCEPTION_POINTERS *pExceptionInfo);
#else 
	static void ExceptionHandlerCallback(int param);
#endif 

};

#endif