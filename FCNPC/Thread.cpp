/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: Thread.cpp
	- Author(s): OrMisicL

  =========================================*/

#include "Main.h"

CThread::CThread(ThreadCallback_t pfn_ThreadCallback)
{
	// Reset thread handle
#ifdef _WIN32
	m_hThread = NULL;
#else
	m_Thread = NULL;
#endif
	// Reset flags
	m_bStarted = false;
	// Save thread callback
	m_pfnThreadCallback = pfn_ThreadCallback;
}

CThread::~CThread()
{
	// Stop the thread if running
	Stop();
}

bool CThread::Start()
{
	// Exit if we are already running
	if(m_bStarted)
		return false;

	// Start the thread
#ifdef _WIN32
	m_hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ThreadFunction, (LPVOID)this, NULL, NULL); 
	if(!m_hThread)
		return false;
#else
	pthread_create(&m_Thread, NULL, ThreadFunction, (void *)this);
	if(!m_Thread)
		return false;
#endif

	return true;
}

void CThread::Stop()
{
	// If the thread is not started then dont bother go any further
	if(!m_bStarted)
		return;

	// Terminate and delete the thread
#ifdef _WIN32
	TerminateThread(m_hThread, 0);
	CloseHandle(m_hThread);
	m_hThread = NULL;
#else
	pthread_cancel(m_Thread);
	m_Thread = NULL;
#endif
	// Mark not started
	m_bStarted = false;
}

#ifdef _WIN32
void CThread::ThreadFunction(LPVOID lpThread)
#else
void *CThread::ThreadFunction(void *lpThread)
#endif
{
	// Get thread pointer
	CThread *pThisThread = (CThread *)lpThread;
	// Mark started
	pThisThread->SetStarted(true);
	// Call the function callback
	pThisThread->CallCallback(pThisThread);
}





