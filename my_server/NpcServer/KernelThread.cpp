
//**********************************************************
// 代码编辑器
//**********************************************************

// KernelThread.cpp: implementation of the CKernelThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NpcServer.h"
#include "NpcServerDlg.h"
#include "KernelThread.h"
#include "inifile.h"
#include "typedef.h"
#include "protocol.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CKernelThread::CKernelThread()
{
    m_pKernel		= NULL;
    m_tNextClock	= 0;
}

//////////////////////////////////////////////////////////////////////
CKernelThread::~CKernelThread()
{
}

//////////////////////////////////////////////////////////////////////
// dynamic link
//////////////////////////////////////////////////////////////////////
void CKernelThread::OnInit()
{
    m_tNextClock = clock();
}

//////////////////////////////////////////////////////////////////////
bool CKernelThread::OnProcess()
{
    clock_t	tStart = clock();
    if (!m_sockClient.IsOpen())
    {
        m_pKernel->SetDisconnect();
        m_pDialog->PrintText("Connect to game server again . . .");
        LOGDUMP("Connect to game server again . . .");
        if (!m_sockClient.Open(CONNECT_DELAY_MS))
        {
            return true;    // 不继续
        }
    }
    const char* pBuf;
    int			nSize;
    while(( pBuf = m_sockClient.GetPacket(&nSize) ))
    {
        if (!nSize)
        {
            break;
        }
        char*		pMsg;
        int			nMsgSize;
        OBJID		idPacket;
        OBJID		idNpc;
        int			nNetPacketSize = SplitNpcPacket(pBuf, nSize, &idPacket, &pMsg, &nMsgSize, &idNpc);
        if (nNetPacketSize)
        {
            DEBUG_TRY
            m_pKernel->ProcessMsg(idPacket, pMsg, nMsgSize, idNpc);
            DEBUG_CATCH("m_pKernel->ProcessMsg()")
            IF_NOT(m_sockClient.ClearPacket(nNetPacketSize))
            {
                ::LOGERROR("Clear packet failed [type=%u, packetsize=%u, bufsize=%u, msgsize=%u]",
                           idPacket, nNetPacketSize, nSize, nMsgSize);
                break;
            }
        }
        else
        {
            break;    // while
        }
    }
    clock_t	tCurr = clock();
    if (tCurr >= m_tNextClock)
    {
        m_tNextClock	+= SYSTEM_ONTIMER_MS;
        // 自动修正
        {
            int nOffset	= (tCurr - m_tNextClock) / CLOCKS_PER_SEC;
            if (abs(nOffset) > TIMER_OFFSET_LIMIT)
            {
                m_tNextClock = tCurr + CLOCKS_PER_SEC;
                LOGWARNING("CKernelThread::OnProcess() 时钟偏移%d秒，已自动修正。", nOffset);
            }
        }
        DEBUG_TRY
        m_pKernel->OnTimer(tCurr);
        DEBUG_CATCH("m_pKernel->OnTimer()")
    }
    m_sockClient.Flush();
    {
        // stat
        extern struct STAT_ST	g_stat;
        int	 tUsed = clock() - tStart;
        InterlockedIncrement(&g_stat.nKernelCount);
        InterlockedExchangeAdd(&g_stat.nKernelSum, tUsed);
        if (tUsed > g_stat.nKernelMax)
        {
            InterlockedExchange(&g_stat.nKernelMax, tUsed);
        }
    }
    Sleep(50);			//@ 可减到0
    return true;
}

//////////////////////////////////////////////////////////////////////
// static link
//////////////////////////////////////////////////////////////////////
bool CKernelThread::CreateThread(IDialog* pDialog, bool bRun /*= true*/)		// false: 暂不运行，用 ResumeThread() 运行
{
    m_pDialog	= pDialog;
    CIniFile	ini(CONFIG_INI_FILENAME, "GameServer");
    IPSTR	szIP;
    DWORD	nPort;
    int		nDelay	= CONNECT_DELAY_MS * 3;
    int		nSndSize;
    int		nRcvSize;
    ini.GetString(szIP, "GAMESERVER_IP", IPSTR_SIZE);
    nPort		= ini.GetInt("GAMESERVER_PORT");
    nSndSize	= ini.GetInt("SND_SIZE");
    nRcvSize	= ini.GetInt("RCV_SIZE");
    m_sockClient.Open(szIP, nPort, nDelay, nSndSize, nRcvSize);		// return false : no complete
    m_pKernel	= ::CreateNewKernel();
    if (!m_pKernel)
    {
        return false;
    }
    if (!m_pKernel->Create(this))
    {
        m_pKernel->Release();
        return false;
    }
    return CThreadBase::CreateThread(bRun);
}

//////////////////////////////////////////////////////////////////////
void CKernelThread::OnDestroy()
{
    if (m_pKernel)
    {
        m_pKernel->Release();
        m_pKernel = NULL;
    }
    m_sockClient.Close();
}




