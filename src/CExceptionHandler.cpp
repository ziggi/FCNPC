/* =========================================

FCNPC - Fully Controllable NPC
----------------------

- File: ExceptionHandler.cpp
- Author(s): OrMisicL

=========================================*/

#include "Main.hpp"

extern CServer *pServer;

void CExceptionHandler::Install()
{
	// disable the exception handler callback
#if defined(WIN32)
	SetUnhandledExceptionFilter(ExceptionHandlerCallback);
#elif defined(LINUX)
	struct sigaction sigact;

	sigact.sa_sigaction = ExceptionHandlerCallback;
	sigact.sa_flags = SA_RESTART | SA_SIGINFO;

	if (sigaction(SIGSEGV, &sigact, (struct sigaction *)NULL) != 0) {
		fprintf(stderr, "Error setting signal handler for %d (%s)\n", SIGSEGV, strsignal(SIGSEGV));
		exit(EXIT_FAILURE);
	}
#endif
}

void CExceptionHandler::UnInstall()
{
	// Set the exception handler callback
#if defined(WIN32)
	SetUnhandledExceptionFilter(NULL);
#elif defined(LINUX)
	struct sigaction sigact;

	sigact.sa_handler = SIG_DFL;
	sigact.sa_flags = SA_RESTART | SA_SIGINFO;

	sigaction(SIGSEGV, &sigact, (struct sigaction *)NULL);
#endif
}

#if defined(WIN32)
long WINAPI CExceptionHandler::ExceptionHandlerCallback(_EXCEPTION_POINTERS *pExceptionInfo)
{
	// Get the current time
	SYSTEMTIME systemTime;
	GetSystemTime(&systemTime);
	// Create the file name
	char szFileName[MAX_PATH];
	snprintf(szFileName, sizeof(szFileName), "fcnpc-crash_%04d-%02d-%02d_%02d.%02d.%02d.txt", systemTime.wYear, systemTime.wMonth, systemTime.wDay, systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
	// Open the log file
	FILE *pFile;
	fopen_s(&pFile, szFileName, "w");
	// Make sure the file is opened
	if (!pFile) {
		return EXCEPTION_EXECUTE_HANDLER;
	}

	// Write the exception informations
	fprintf(pFile, "=========================== FCNPC Exception Handler ============================\n");
	fprintf(pFile, "Address: 0x%x\n\n", (DWORD)pExceptionInfo->ExceptionRecord->ExceptionAddress);
	fprintf(pFile, "Registers:\n");
	fprintf(pFile, "EAX: 0x%x - EBX: 0x%x - ECX: 0x%x - EDX: 0x%x\nESI: 0x%x - EDI: 0x%lx - EBP: 0x%x - ESP: 0x%x\n\n", pExceptionInfo->ContextRecord->Eax, pExceptionInfo->ContextRecord->Ebx,
	        pExceptionInfo->ContextRecord->Ecx, pExceptionInfo->ContextRecord->Edx, pExceptionInfo->ContextRecord->Esi, pExceptionInfo->ContextRecord->Edi,
	        pExceptionInfo->ContextRecord->Ebp, pExceptionInfo->ContextRecord->Esp);

	DWORD dwEsp = pExceptionInfo->ContextRecord->Esp;
	fprintf(pFile, "Stack:\n");
	fprintf(pFile, "0x0: 0x%x - 0x4: 0x%x - 0x8: 0x%x - 0xC: 0x%x - 0x10: 0x%x - 0x14: 0x%x\n0x18: 0x%x - 0x1C: 0x%x - 0x20: 0x%x\n", *(DWORD *)(dwEsp), *(DWORD *)(dwEsp + 4),
	        *(DWORD *)(dwEsp + 8), *(DWORD *)(dwEsp + 12), *(DWORD *)(dwEsp + 16), *(DWORD *)(dwEsp + 20), *(DWORD *)(dwEsp + 24), *(DWORD *)(dwEsp + 28), *(DWORD *)(dwEsp + 32));

	fprintf(pFile, "\nInformation:\n");
	fprintf(pFile, "  System: %s\n", SYSTEM_NAME);
	fprintf(pFile, "  %s: %s\n", szSampClient, szSampVersion);
	fprintf(pFile, "  FCNPC: %s\n", PLUGIN_VERSION);

	fprintf(pFile, "\nFunctions:\n");
	fprintf(pFile, "  FUNC_CPlayerPool__DeletePlayer: 0x%x\n", CAddress::FUNC_CPlayerPool__DeletePlayer);
	fprintf(pFile, "  FUNC_CPlayer__Kill: 0x%x\n", CAddress::FUNC_CPlayer__Kill);
	fprintf(pFile, "  FUNC_CPlayer__EnterVehicle: 0x%x\n", CAddress::FUNC_CPlayer__EnterVehicle);
	fprintf(pFile, "  FUNC_CPlayer__ExitVehicle: 0x%x\n", CAddress::FUNC_CPlayer__ExitVehicle);
	fprintf(pFile, "  FUNC_CPlayer__SpawnForWorld: 0x%x\n", CAddress::FUNC_CPlayer__SpawnForWorld);
	fprintf(pFile, "  FUNC_GetVehicleModelInfo: 0x%x\n", CAddress::FUNC_GetVehicleModelInfo);
	fprintf(pFile, "  FUNC_CConsole__GetIntVariable: 0x%x\n", CAddress::FUNC_CConsole__GetIntVariable);
	fprintf(pFile, "  FUNC_ClientJoin_RPC: 0x%x\n", CAddress::FUNC_ClientJoin_RPC);

	fprintf(pFile, "\nPointers:\n");
	fprintf(pFile, "  VAR_ServerAuthentication: 0x%x\n", CAddress::VAR_ServerAuthentication);
	fprintf(pFile, "  VAR_NetVersion: 0x%x\n", CAddress::VAR_NetVersion);

	fprintf(pFile, "\nOffsets:\n");
	fprintf(pFile, "  OFFSET_RemoteSystemManager: 0x%x\n", CAddress::OFFSET_RemoteSystemManager);
	fprintf(pFile, "  OFFSET_RemoteSystemSize: 0x%x\n", CAddress::OFFSET_RemoteSystemSize);
	fprintf(pFile, "  OFFSET_RemoteSystem__ConnectMode: 0x%x\n", CAddress::OFFSET_RemoteSystem__ConnectMode);
	fprintf(pFile, "  OFFSET_RemoteSystem__Unknown: 0x%x\n", CAddress::OFFSET_RemoteSystem__Unknown);
	fprintf(pFile, "=========================== FCNPC Exception Handler ============================");
	// Close the file
	fclose(pFile);
	return EXCEPTION_EXECUTE_HANDLER;
}
#elif defined(LINUX)
void CExceptionHandler::ExceptionHandlerCallback(int signum, siginfo_t * info, void * ucontext)
{
	time_t rawtime;
	struct tm *timeinfo;
	// Get the current time
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	// Create the file name
	char szFileName[MAX_PATH];
	sprintf(szFileName, "fcnpc-crash_%d-%02d-%02d_%02d.%02d.%02d.txt", timeinfo->tm_year + 1900, timeinfo->tm_mon, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
	// Open the log file
	FILE *pFile = fopen(szFileName, "w");
	// Make sure the file is opened
	if (!pFile) {
		return;
	}

	// Write the exception informations
	fprintf(pFile, "=========================== FCNPC Exception Handler ============================");

	// signal
	sig_ucontext_t *uc = (sig_ucontext_t *)ucontext;
	fprintf(pFile, "\nSignal %d (%s), address is %x from %x\n",
	        signum,
	        strsignal(signum),
	        (DWORD)info->si_addr,
	        (DWORD)uc->uc_mcontext.eip);

	// backtrace
	int i, nptrs;
	void *buffer[10];
	char **strings;

	nptrs = backtrace(buffer, 10);
	strings = backtrace_symbols(buffer, nptrs);

	fprintf(pFile, "\nBacktrace:\n");
	for (i = 0; i < nptrs; i++) {
		fprintf(pFile, "  #%d in %s\n", i, strings[i]);
	}

	fprintf(pFile, "\nInformation:\n");
	fprintf(pFile, "  System: %s\n", SYSTEM_NAME);
	fprintf(pFile, "  %s: %s\n", szSampClient, szSampVersion);
	fprintf(pFile, "  FCNPC: %s\n", PLUGIN_VERSION);

	fprintf(pFile, "\nFunctions:\n");
	fprintf(pFile, "  FUNC_CPlayerPool__DeletePlayer: 0x%x\n", CAddress::FUNC_CPlayerPool__DeletePlayer);
	fprintf(pFile, "  FUNC_CPlayer__Kill: 0x%x\n", CAddress::FUNC_CPlayer__Kill);
	fprintf(pFile, "  FUNC_CPlayer__EnterVehicle: 0x%x\n", CAddress::FUNC_CPlayer__EnterVehicle);
	fprintf(pFile, "  FUNC_CPlayer__ExitVehicle: 0x%x\n", CAddress::FUNC_CPlayer__ExitVehicle);
	fprintf(pFile, "  FUNC_CPlayer__SpawnForWorld: 0x%x\n", CAddress::FUNC_CPlayer__SpawnForWorld);
	fprintf(pFile, "  FUNC_GetVehicleModelInfo: 0x%x\n", CAddress::FUNC_GetVehicleModelInfo);
	fprintf(pFile, "  FUNC_CConsole__GetIntVariable: 0x%x\n", CAddress::FUNC_CConsole__GetIntVariable);
	fprintf(pFile, "  FUNC_ClientJoin_RPC: 0x%x\n", CAddress::FUNC_ClientJoin_RPC);

	fprintf(pFile, "\nPointers:\n");
	fprintf(pFile, "  VAR_ServerAuthentication: 0x%x\n", CAddress::VAR_ServerAuthentication);
	fprintf(pFile, "  VAR_NetVersion: 0x%x\n", CAddress::VAR_NetVersion);

	fprintf(pFile, "\nOffsets:\n");
	fprintf(pFile, "  OFFSET_RemoteSystemManager: 0x%x\n", CAddress::OFFSET_RemoteSystemManager);
	fprintf(pFile, "  OFFSET_RemoteSystemSize: 0x%x\n", CAddress::OFFSET_RemoteSystemSize);
	fprintf(pFile, "  OFFSET_RemoteSystem__ConnectMode: 0x%x\n", CAddress::OFFSET_RemoteSystem__ConnectMode);
	fprintf(pFile, "  OFFSET_RemoteSystem__Unknown: 0x%x\n", CAddress::OFFSET_RemoteSystem__Unknown);
	fprintf(pFile, "=========================== FCNPC Exception Handler ============================");

	// Close the file
	fclose(pFile);

	// close program
	exit(signum);
	return;
}
#endif
