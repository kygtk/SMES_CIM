// Implementation of the CThread class.
//
//////////////////////////////////////////////////////////////////////

#include "Thread.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CThread::CThread()
{
	m_hThread		= NULL;
	m_dwThreadId	= 0;
	m_bThreadRun	= FALSE;
}

CThread::~CThread()
{
	if (m_bThreadRun)	// 실행중인 외부 thread가 있다면
	{
		if (!EndThread())
			KillThread();
	}
}

BOOL CThread::StartThread(void *pThreadProc, void* pParam, int nPriority/*=THREAD_PRIORITY_NORMAL*/)
{
	if (IsRunning())
	{
		KillThread();
		return FALSE;
	}

	m_Param.pParam1 = (void*)this;
	m_Param.pParam2 = pParam;
	m_hThread = ::CreateThread(	NULL,
								0,
								(LPTHREAD_START_ROUTINE)pThreadProc,
								(LPVOID)&m_Param,
								CREATE_SUSPENDED,
								&m_dwThreadId );

	if (m_hThread == NULL)
		return FALSE;

	m_bThreadRun	= TRUE;

	::SetThreadPriority(m_hThread, nPriority);
	::ResumeThread(m_hThread);

	return TRUE;
}

BOOL CThread::EndThread()
{
	m_bThreadRun = FALSE;

	//  Wait until terminated thread(wait max: 100msec)
	return (::WaitForSingleObject(m_hThread, 100) == WAIT_OBJECT_0);
}

void CThread::KillThread()
{
	::TerminateThread(m_hThread, -1);

	m_bThreadRun	= FALSE;
	m_hThread		= NULL;
}

BOOL CThread::IsRunning()
{
	DWORD	dwExitCode;
	::GetExitCodeThread(m_hThread, &dwExitCode);

	return m_bThreadRun & (STILL_ACTIVE == dwExitCode);
}
