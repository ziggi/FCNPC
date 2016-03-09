/* =========================================

		FCNPC - Fully Controllable NPC
			----------------------

	- File: Thread.hpp
	- Author(s): OrMisicL

  =========================================*/

#ifndef THREAD_H
#define THREAD_H

typedef void (*ThreadCallback_t)(void *pThread);

class CThread
{
public:
	CThread(ThreadCallback_t pfn_ThreadCallback);
	~CThread();

	bool Start();
	void Stop();
	bool IsStarted()
	{
		return m_bStarted;
	};
	void SetStarted(bool bState)
	{
		m_bStarted = bState;
	};
	void CallCallback(CThread *pThread)
	{
		m_pfnThreadCallback((void *)pThread);
	};
#if defined(WIN32)
	static void ThreadFunction(LPVOID lpThread);
#elif defined(LINUX)
	static void *ThreadFunction(void * pVoidThis);
#endif

	template <typename T>
	void SetParam(T param)
	{
		m_pParam = (void *)param;
	};

	template <typename T>
	T GetParam()
	{
		return (T)(m_pParam);
	};

private:
	bool                m_bStarted;
	ThreadCallback_t    m_pfnThreadCallback;
#if defined(WIN32)
	HANDLE              m_hThread;
#elif defined(LINUX)
	pthread_t           m_Thread;
#endif
	void                *m_pParam;
};

#endif
