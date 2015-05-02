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
	static long WINAPI ExceptionHandlerCallback(_EXCEPTION_POINTERS *pExceptionInfo);

};

#endif