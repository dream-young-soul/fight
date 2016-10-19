
//**********************************************************\\
// 代码编辑器\\
//**********************************************************\\
\\
// DBThread.cpp: implementation of the CDBThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MessagePort.h"
#include "MsgServer.h"
#include "I_Shell.h"
#include "protocol.h"
#include "DBThread.h"
#include "common.h"
#include "DBKernel/I_DBService.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDBThread::CDBThread(IMessagePort* pPort)
    : m_pDBService(NULL)
{
    ASSERT(pPort);
    m_pMsgPort		= pPort;
    m_tNextClock	= 0;
}

CDBThread::~CDBThread()
{
    SAFE_RELEASE (m_pDBService);
}

//////////////////////////////////////////////////////////////////////
// overload
//////////////////////////////////////////////////////////////////////
bool CDBThread::CreateThread(bool bRun /*= true*/)
{
    m_pDBService	= IDBService::CreateNew();
    if (m_pDBService)
    {
        if (!m_pDBService->Create(m_pMsgPort))
        {
            return false;
        }
    }
    if(!CThreadBase::CreateThread(bRun))
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
void CDBThread::OnDestroy()
{
}

//////////////////////////////////////////////////////////////////////
void CDBThread::OnInit()
{
    m_tNextClock = clock() + CLOCKS_PER_SEC;
    char	szText[1024];
    sprintf(szText, "#%u: DB kernel thread running.", m_pMsgPort->GetID());
    m_pMsgPort->Send(MSGPORT_SHELL, SHELL_PRINTTEXT, STRING_TYPE(szText), szText);
}

//////////////////////////////////////////////////////////////////////
bool CDBThread::OnProcess()
{
    char	buf[MAX_MESSAGESIZE];
    CMessageStatus	cStatus;
    clock_t	clkStart = clock();
    DEBUG_TRY	// VVVVVVVVVVVVVV
    if(m_pMsgPort->Recv(PORT_ANY, PACKET_ANY, STRUCT_TYPE(buf), buf, &cStatus))
    {
        if(!m_pDBService->ProcessMsg(cStatus.m_nPacket, buf, cStatus.m_nVarType, cStatus.m_nPortFrom))
        {
            LOGERROR("m_pDBService->ProcessMsg内部消息出错");
            //			return false;
        }
    }
    DEBUG_CATCH("CDBThread::OnProcess()")	// AAAAAAAAAAA
    clock_t	tCurr = clock();
    if(tCurr >= m_tNextClock)
    {
        m_tNextClock	+= WORLDKERNEL_ONTIMER_MS;
        // 自动修正
        {
            int nOffset	= (tCurr - m_tNextClock) / CLOCKS_PER_SEC;
            if(abs(nOffset) > TIMER_OFFSET_LIMIT)
            {
                m_tNextClock = tCurr + WORLDKERNEL_ONTIMER_MS;
                LOGWARNING("CDBThread::OnProcess() 时钟偏移%d秒，已自动修正。", nOffset);
            }
        }
        DEBUG_TRY
        m_pDBService->OnTimer(tCurr);
        DEBUG_CATCH("m_pDBService->OnTimer()")
    }
    {
        /*
        		// stat
        		clock_t	 clkUsed	= clock() - clkStart;
        		extern struct STAT_STRUCT	g_stat;
        		InterlockedExchangeAdd(&g_stat.nAllWorldMS, clkUsed);
        		InterlockedIncrement(&g_stat.nWorldTimes);
        		if(g_stat.nMaxWorldMS < clkUsed)
        			g_stat.nMaxWorldMS = clkUsed;
        */
    }
    DEBUG_TRY	// VVVVVVVVVVVVVV
    m_pMsgPort->WaitMsg(__min(THREADDELAY_MS, WORLDKERNEL_ONTIMER_MS));		// 响应“关闭线程消息”的频度
    DEBUG_CATCH("CDBThread::OnProcess(2)")	// AAAAAAAAAAA
    return true;
}
