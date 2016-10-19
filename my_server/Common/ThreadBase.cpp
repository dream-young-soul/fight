
//**********************************************************
// 代码编辑器
//**********************************************************

// 线程基类
// 仙剑修，2001.11.15

#include "logfile.h"
#include "ThreadBase.h"

//*********************************************************
//default constructor for class CThread
//*********************************************************
CThreadBase::CThreadBase()
{
    m_hThread		= NULL;
    m_hCloseThreadEvent	= NULL;
}

//*********************************************************
//destructor for class CObject
//*********************************************************
CThreadBase::~CThreadBase()
{
    if (m_hThread)
    {
        if (::WaitForSingleObject (m_hThread, 0) == WAIT_TIMEOUT)
        {
            ::TerminateThread (m_hThread, 5);
            LOGWARNING("Hard terminate thread, may be lost some resources.");
        }
        ::CloseHandle(m_hThread);
    }
    if (m_hCloseThreadEvent)
    {
        ::CloseHandle(m_hCloseThreadEvent);
    }
}

DWORD CThreadBase::ThreadProc ()
{
    DWORD	ret = 0;
    OnInit();
    while(!IsCloseEvent())
    {
        if (!OnProcess())
        {
            ret = 2;
            break;
        }
    }
    OnDestroy();
    return ret;
}

bool CThreadBase::CreateThread (bool bRun /*= true*/)
{
    if (!IsCreated ())
    {
        // 创建关闭事件句柄
        m_hCloseThreadEvent = ::CreateEvent(NULL, false, false, NULL);		// "Close Thread"
        if (m_hCloseThreadEvent == NULL)
        {
            LOGERROR("Create close thread event failed.");
            return false;
        }
        DWORD	dwThreadID;		// 不用，丢弃
        DWORD	dwCreationFlags = bRun ? 0 : CREATE_SUSPENDED;
        m_hThread = ::CreateThread (NULL, 0, (DWORD (WINAPI*)(LPVOID))TrueThreadProc,
                                    (void*)(CThreadBase*)this, dwCreationFlags, &dwThreadID);
        if (m_hThread)
        {
            return true;
        }
        else
        {
            ::CloseHandle(m_hCloseThreadEvent);
            LOGERROR("Create thread failed.");
            return false;
        }
    }
    else
    {
        LOGERROR("Can't create thread since already created.");
        return false;
    }
}

bool	CThreadBase::ResumeThread()	// return false: 失败
{
    if (m_hThread)
    {
        if (::ResumeThread(m_hThread) != -1)
        {
            return true;
        }
    }
    else
    {
        int err = GetLastError();
        LOGERROR("::ResumeThread() failed[%d] in ResumeThread().", err);
    }
    return false;
}


bool CThreadBase::CloseThread(long nMilliseconds /*= 0*/)
{
    if (m_hThread)
    {
        ::ResumeThread(m_hThread);			// 冗余
        ::SetEvent(m_hCloseThreadEvent);
        if (::WaitForSingleObject(m_hThread, nMilliseconds) != WAIT_TIMEOUT)		// 调用失败时也关闭。
        {
            ::CloseHandle(m_hThread);
            m_hThread = NULL;
            ::CloseHandle(m_hCloseThreadEvent);
            m_hCloseThreadEvent = NULL;
        }
        else
        {
            return false;
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
bool	CThreadBase::IsCloseEvent(long nMilliseconds /*= 0*/)			// 检查关闭线程事件是否触发。用于 ThreadProc() 函数调用
{
    if (::WaitForSingleObject(m_hCloseThreadEvent, nMilliseconds) != WAIT_TIMEOUT)
    {
        return true;
    }
    else
    {
        return false;
    }
}


//*********************************************************
//creates the thread
//*********************************************************
DWORD WINAPI CThreadBase::TrueThreadProc(LPVOID pParam)
{
    CThreadBase* 	pThread;
    pThread			= (CThreadBase*)pParam;
    return pThread->ThreadProc();
}





