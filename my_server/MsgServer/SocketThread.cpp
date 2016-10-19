
//**********************************************************
// 代码编辑器
//**********************************************************

// SocketThread.cpp: implementation of the CSocketThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "I_Shell.h"
#include "SocketThread.h"
#include "MessagePort.h"
#include "MsgServer.h"
#include "protocol.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSocketThread::CSocketThread(IMessagePort* pPort)
{
    ASSERT(pPort);
    m_pMsgPort		= pPort;
    m_pServerSocket	= NULL;
    m_tNextClock	= 0;
    m_hMutexThread	= NULL;
}

CSocketThread::~CSocketThread()
{
}

//////////////////////////////////////////////////////////////////////
// overload
//////////////////////////////////////////////////////////////////////
bool CSocketThread::CreateThread(bool bRun /*= true*/)
{
#ifdef	DEBUG_MULTITHREAD
    m_hMutexThread    =::CreateMutex(NULL, false, "FW_DEBUG_MULTITHREAD");
    if (!m_hMutexThread)
    {
        LOGERROR("创建mutex句柄失败!");
        return false;
    }
#endif
    if (!CThreadBase::CreateThread(bRun))
    {
        return false;
    }
    m_pServerSocket	= ISocketKernel::CreateNew();
    return m_pServerSocket->Create(m_pMsgPort);
}

//////////////////////////////////////////////////////////////////////
void CSocketThread::OnDestroy()
{
    if (m_pServerSocket)
    {
        m_pServerSocket->Release();
    }
    m_pServerSocket = NULL;
#ifdef	DEBUG_MULTITHREAD
    if (m_hMutexThread)
    {
        ::CloseHandle(m_hMutexThread);
        m_hMutexThread = NULL;
    }
#endif
}

//////////////////////////////////////////////////////////////////////
void CSocketThread::OnInit()
{
    m_tNextClock = clock() + CLOCKS_PER_SEC;
    char	szText[1024];
    sprintf(szText, "#%u: Socket kernel thread running.", m_pMsgPort->GetID());
    m_pMsgPort->Send(MSGPORT_SHELL, SHELL_PRINTTEXT, STRING_TYPE(szText), szText);
}

//////////////////////////////////////////////////////////////////////
bool CSocketThread::OnProcess()
{
#ifdef	DEBUG_MULTITHREAD
    if (::WaitForSingleObject(m_hMutexThread, INFINITE) == WAIT_ABANDONED)
    {
        return false;
    }
#endif
    char	buf[MAX_MESSAGESIZE];
    CMessageStatus	cStatus;
    clock_t	clkStart = clock();
    clock_t	clkDebug = clock();		// debug
    // 内部消息处理
    DEBUG_TRY	// VVVVVVVVVVVVVV
    while(m_pMsgPort->Recv(PORT_ANY, PACKET_ANY, STRUCT_TYPE(buf), buf, &cStatus))
    {
        clock_t	clkDebug = clock();		// debug
        if (!m_pServerSocket->ProcessMsg(cStatus.m_nPacket, buf, cStatus.m_nVarType, cStatus.m_nPortFrom))
        {
            LOGERROR("CSocketThread内部消息出错");
        }
        {
            extern struct STAT_STRUCT	g_stat;
            InterlockedExchangeAdd(&g_stat.setDebug[1], clock() - clkDebug);		// debug
        }
    }
    DEBUG_CATCH("CMapGroupThread::OnProcess(msg)")	// AAAAAAAAAAA
    {
        extern struct STAT_STRUCT	g_stat;
        InterlockedExchangeAdd(&g_stat.setDebug[2], clock() - clkDebug);		// debug
        clkDebug = clock();		// debug
    }
    // 时钟处理
    clock_t	tCurr = clock();
    if (tCurr >= m_tNextClock)
    {
        m_tNextClock	+= SOCKETKERNEL_ONTIMER_MS;
        // 自动修正
        {
            int nOffset	= (tCurr - m_tNextClock) / CLOCKS_PER_SEC;
            if (abs(nOffset) > TIMER_OFFSET_LIMIT)
            {
                m_tNextClock = tCurr + SOCKETKERNEL_ONTIMER_MS;
                LOGWARNING("CSocketThread::OnProcess() 时钟偏移%d秒，已自动修正。", nOffset);
            }
        }
        DEBUG_TRY
        m_pServerSocket->OnTimer(tCurr);
        DEBUG_CATCH("m_pServerSocket->OnTimer()")
    }
    // SOCKET消息处理
    clkDebug = clock();		// debug
    DEBUG_TRY	// VVVVVVVVVVVVVV
    if (m_tProcessSocket.ToNextTime(SOCKET_PROCESS_INTERVAL))
    {
        if (!m_pServerSocket->ProcessSocket())
        {
            LOGERROR("CSocketThread的SOCKET处理出错");
        }
    }
    DEBUG_CATCH("CMapGroupThread::OnProcess(socket)")	// AAAAAAAAAAA
    {
        extern struct STAT_STRUCT	g_stat;
        //		InterlockedExchangeAdd(&g_stat.setDebug[3], clock() - clkDebug);		// debug
    }
    {
        extern struct STAT_STRUCT	g_stat;
        InterlockedExchangeAdd(&g_stat.setDebug[4], clock() - clkStart);		// debug
    }
#ifdef	DEBUG_MULTITHREAD
    ::ReleaseMutex(m_hMutexThread);
#endif
    //	Sleep(0);
    // 延时
    clock_t	 clkLeave	= SOCKET_PROCESS_INTERVAL - (clock() - clkStart);
    if (clkLeave < 0 || clkLeave > SOCKET_PROCESS_INTERVAL)
    {
        clkLeave = 0;
    }
    {
        // stat
        clock_t	 clkUsed	= clock() - clkStart;
        extern struct STAT_STRUCT	g_stat;
        InterlockedExchangeAdd(&g_stat.nAllSocketMS, clkUsed);
        InterlockedIncrement(&g_stat.nSocketTimes);
        if (g_stat.nMaxSocketMS < clkUsed)
        {
            g_stat.nMaxSocketMS = clkUsed;
        }
    }
    DEBUG_TRY	// VVVVVVVVVVVVVV
    m_pMsgPort->WaitMsg(clkLeave);			//@ 应该探测MsgList的数量
    DEBUG_CATCH("CMapGroupThread::OnProcess(wait msg)")	// AAAAAAAAAAA
    return true;
}




