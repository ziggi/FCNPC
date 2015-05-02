/* =========================================

FCNPC - Fully Controllable NPC
----------------------

- File: ExceptionHandler.cpp
- Author(s): OrMisicL

=========================================*/

#include "Main.h"

extern CServer *pServer;

void CExceptionHandler::Install()
{
	// Set the exception handler callback
	SetUnhandledExceptionFilter(ExceptionHandlerCallback);
}

long WINAPI CExceptionHandler::ExceptionHandlerCallback(_EXCEPTION_POINTERS *pExceptionInfo)
{
	// Get the current time
	SYSTEMTIME systemTime;
	GetSystemTime(&systemTime);
	// Create the file name
	char szFileName[MAX_PATH];
	sprintf(szFileName, "fcnpc-crash_%d-%d-%d_%d.%d.%d.txt", systemTime.wDay, systemTime.wMonth, systemTime.wYear, systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
	// Open the log file
	FILE *pFile = fopen(szFileName, "w");
	// Make sure the file is opened
	if (!pFile)
		return EXCEPTION_EXECUTE_HANDLER;

	// Write the exception informations
	fprintf(pFile, "======================== FCNPC Exception Handler ===============================\n");
	fprintf(pFile, "Address: 0x%p\n\n", pExceptionInfo->ExceptionRecord->ExceptionAddress);
	fprintf(pFile, "Registers:\n");
	fprintf(pFile, "EAX: 0x%p - EBX: 0x%p - ECX: 0x%p - EDX: 0x%p\nESI: 0x%p - EDI: 0x%x - EBP: 0x%p - ESP: 0x%p\n\n", pExceptionInfo->ContextRecord->Eax, pExceptionInfo->ContextRecord->Ebx,
		pExceptionInfo->ContextRecord->Ecx, pExceptionInfo->ContextRecord->Edx, pExceptionInfo->ContextRecord->Esi, pExceptionInfo->ContextRecord->Edi,
		pExceptionInfo->ContextRecord->Ebp, pExceptionInfo->ContextRecord->Esp);

	DWORD dwEsp = pExceptionInfo->ContextRecord->Esp;
	fprintf(pFile, "Stack:\n");
	fprintf(pFile, "0x0: 0x%p - 0x4: 0x%p - 0x8: 0x%p - 0xC: 0x%p - 0x10: 0x%p - 0x14: 0x%p\n0x18: 0x%p - 0x1C: 0x%p - 0x20: 0x%p\n", *(DWORD *)(dwEsp), *(DWORD *)(dwEsp + 4),
		*(DWORD *)(dwEsp + 8), *(DWORD *)(dwEsp + 12), *(DWORD *)(dwEsp + 16), *(DWORD *)(dwEsp + 20), *(DWORD *)(dwEsp + 24), *(DWORD *)(dwEsp + 28), *(DWORD *)(dwEsp + 32));

	fprintf(pFile, "\nFunctions:\n");
	fprintf(pFile, "FUNC_CPlayerPool__DeletePlayer: 0x%x\n", CAddress::FUNC_CPlayerPool__DeletePlayer);
	fprintf(pFile, "FUNC_CPlayer__Kill: 0x%x\n", CAddress::FUNC_CPlayer__Kill);
	fprintf(pFile, "FUNC_CPlayer__EnterVehicle: 0x%x\n", CAddress::FUNC_CPlayer__EnterVehicle);
	fprintf(pFile, "FUNC_CPlayer__ExitVehicle: 0x%x\n", CAddress::FUNC_CPlayer__ExitVehicle);
	fprintf(pFile, "FUNC_CPlayer__SpawnForWorld: 0x%x\n", CAddress::FUNC_CPlayer__SpawnForWorld);
	fprintf(pFile, "FUNC_GetVehicleModelInfo: 0x%x\n", CAddress::FUNC_GetVehicleModelInfo);
	fprintf(pFile, "FUNC_CConfig__GetValueAsInteger: 0x%x\n", CAddress::FUNC_CConfig__GetValueAsInteger);
	fprintf(pFile, "FUNC_CreateNPC_RPC: 0x%x\n", CAddress::FUNC_CreateNPC_RPC);

	fprintf(pFile, "\n\nPointers:\n");
	fprintf(pFile, "VAR_ServerPtr: 0x%x\n", CAddress::VAR_ServerPtr);
	fprintf(pFile, "VAR_ConfigPtr: 0x%x\n", CAddress::VAR_ConfigPtr);
	fprintf(pFile, "VAR_RakPeerPtr: 0x%x\n", CAddress::VAR_RakPeerPtr);
	fprintf(pFile, "======================== V:MP Exception Handler ===============================");
	// Close the file
	fclose(pFile);
	return EXCEPTION_EXECUTE_HANDLER;
}
