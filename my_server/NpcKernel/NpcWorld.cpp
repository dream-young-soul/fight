
//**********************************************************
// 代码编辑器
//**********************************************************

// NpcWorld.cpp: implementation of the CNpcWorld class.
//
//////////////////////////////////////////////////////////////////////

#include "windows.h"
#include "define.h"
#include "UserManager.h"
#include "NpcManager.h"
#include "MapManager.h"
#include "NpcWorld.h"
#include "AllMsg.h"
#include "Protocol.h"
#include "I_Shell.h"
#include "inifile.h"
#include "I_mydb.h"

//////////////////////////////////////////////////////////////////////
// interface
//////////////////////////////////////////////////////////////////////
IKernel*	CreateNewKernel()
{
    return new CNpcWorld();
}

//////////////////////////////////////////////////////////////////////
// kernel static
//////////////////////////////////////////////////////////////////////
CNpcWorld*	CNpcWorld::m_pNpcWorld;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CNpcWorld::CNpcWorld()
{
    m_pNpcManager		= NULL;
    m_pUserManager		= NULL;
    m_pMapManager		= NULL;
    m_pSynManager		= NULL;
    m_pDatabase			= NULL;
    SetLoginState(STATE_NONE);
}

//////////////////////////////////////////////////////////////////////
CNpcWorld::~CNpcWorld()
{
    if (m_pSynManager)
    {
        SAFE_DELETE(m_pSynManager);
    }
    if (m_pNpcManager)
    {
        SAFE_DELETE(m_pNpcManager);
    }
    if (m_pUserManager)
    {
        SAFE_DELETE(m_pUserManager);
    }
    if (m_pMapManager)
    {
        SAFE_DELETE(m_pMapManager);
    }
    if (m_pDatabase)
    {
        m_pDatabase->Release();
    }
}

//////////////////////////////////////////////////////////////////////
// IKernel
//////////////////////////////////////////////////////////////////////
bool CNpcWorld::Create(IShell* pShell)
{
    CHECKF(pShell);
#ifdef	DUMP_DEBUG_MSG
    char	buf[256] = "syslog\\dump ";
    ::DateTime(buf + strlen(buf));
    strcpy(buf + 22, ".log");
    ::DeleteFile(buf);
#endif
    m_pShell	= pShell;
    m_pNpcWorld	= this;
    m_timeoutLogin.Startup(RESEND_MSGLOGIN_SECS);
    m_tUpdateStateWin.Startup(UPDATESTATEWIN_SECS);
    PrintText("Open the database...");
    CIniFile	ini(CONFIG_INI_FILENAME, "Database");
    IPSTR	szIP, szLogin, szPassword, szLibName;
    ini.GetString(szIP, "DB_IP", IPSTR_SIZE);
    ini.GetString(szLogin, "DB_USER", IPSTR_SIZE);
    ini.GetString(szPassword, "DB_PW", IPSTR_SIZE);
    ini.GetString(szLibName, "DB_NAME", IPSTR_SIZE);
    m_pDatabase		= ::CreateDatabase(szIP, szLogin, szPassword, szLibName);
    if (!m_pDatabase)
    {
        PrintText("ERROR：Can't open database!");
        return false;
    }
    PrintText("Create map set...");
    m_pMapManager	= new CMapManager;
    if (!m_pMapManager || !m_pMapManager->Create())
    {
        return false;
    }
    PrintText("Create player set...");
    m_pUserManager	= new CUserManager;
    if (!m_pUserManager || !m_pUserManager->Create())
    {
        return false;
    }
    PrintText("Create npc set...");
    m_pNpcManager	= new CNpcManager;
    if (!m_pNpcManager || !m_pNpcManager->Create())
    {
        return false;
    }
    PrintText("Create syn set...");
    m_pSynManager	= new CSynManager;
    if (!m_pSynManager || !m_pSynManager->Create())
    {
        return false;
    }
    if (!m_pItemType->Create(m_pDatabase))
    {
        return false;
    }
    PrintText("<<<<<<<< Create Jehad server kernel ok! >>>>>>>>");
    return true;
}

//////////////////////////////////////////////////////////////////////
void CNpcWorld::ProcessMsg(OBJID idPacket, const char* pbufMsg, int nSize, OBJID idNpc)
{
    // TODO: 请在此添加客户端上传消息的处理代码
    if (!pbufMsg || nSize <= 0 || nSize > _MAX_MSGSIZE)
    {
        return ;
    }
#ifdef	PALED_DEBUG_X
    MSGBUF	szMsg;
    sprintf(szMsg, "VVV 接收消息：%u, NpcID：%u", idPacket, idNpc);
    PrintText(szMsg);
#endif
    CNetMsg* pMsg	= CNetMsg::CreateMsg(idNpc, idPacket, pbufMsg, nSize);
    if (pMsg)
    {
        try
        {
            pMsg->Process(this);
        }
        catch(...)
        {
            //			CNetMsg::DumpMsg(pMsg);
            LOGCATCH("CGameSocket::ProcessMsg()! MsgType:%d, NpcID:%u", idPacket, idNpc);
        }
        SAFE_DELETE(pMsg);
    }
    return;
}

//////////////////////////////////////////////////////////////////////
void CNpcWorld::OnTimer(DWORD nCurr)
{
    if (!m_pShell->IsConnectOK())
    {
        return ;
    }
    if (m_nLoginState == STATE_NORMAL)
    {
        if (m_timeoutLogin.IsActive())
        {
            if (m_timeoutLogin.IsTimeOut(LOGIN_DELAY_SECS))
            {
                m_timeoutLogin.Clear();
                PrintText("Generator begin.");
            }
            return ;
        }
        m_pMapManager->OnTimer(nCurr);
        m_pUserManager->OnTimer(nCurr);
        m_pNpcManager->OnTimer(nCurr);
        if (m_tUpdateStateWin.ToNextTime())
        {
            int nAmount		= 0;
            int nGenAmount	= 0;
            int	nGenActive	= 0;
            for(int i = 0; i < NpcManager()->QueryGenSet()->GetAmount(); i++)
            {
                CGenPtr pGen = NpcManager()->QueryGenSet()->GetObjByIndex(i);
                if (pGen)
                {
                    nAmount		+= pGen->GetAmount();
                    nGenAmount	+= pGen->GetGenAmount();
                    if (pGen->GetMaxNpc() > nGenAmount)
                    {
                        nGenActive++;
                    }
                }
            }
            char	buf[STATESTR_SIZE];
            sprintf(buf, "player set in view range：%u\r\n"
                    "npc set：%u(%u)\r\n"
                    "born from generator：%u\r\n"
                    "born ok from generator：%u\r\n"
                    "active generators：%u\r\n",
                    UserManager()->QuerySet()->GetAmount(),
                    NpcManager()->QuerySet()->GetAmount(), NpcManager()->CountActiveNpc(),
                    nGenAmount, nAmount, nGenActive);
            m_pShell->SetState(buf);
        }
    }
    else if (m_nLoginState == STATE_LOGINING)
    {
        if (m_timeoutLogin.ToNextTime(RESEND_MSGLOGIN_SECS))
        {
            PrintText("Login game server . . .");
        }
    }
    else if (m_nLoginState == STATE_NONE)
    {
        m_nLoginState = STATE_LOGINING;
        CMsgLogin	msg;
        if (msg.Create(0, NPCSERVER_ACCOUNT, NPCSERVER_PASSWORD))
        {
            SendMsg(NPCSERVER_VERSION, &msg);
        }
        LOGDUMP("CMsgLogin STATE_NONE");
    }
    else if (m_nLoginState == STATE_BREAK)
    {
        m_nLoginState = STATE_LOGINING;
        CMsgLogin	msg;
        if (msg.Create(0, NPCSERVER_ACCOUNT, NPCSERVER_PASSWORD))
        {
            SendMsg(NPCSERVER_VERSION_RELOGIN, &msg);
        }
        LOGDUMP("CMsgLogin STATE_BREAK");
    }
    else
    {
        ASSERT(!"CNpcWorld::OnTimer");
    }
}

//////////////////////////////////////////////////////////////////////
void CNpcWorld::ShellCommond(LPCTSTR)
{
    //@ 注意多线程互斥
}

//////////////////////////////////////////////////////////////////////
bool CNpcWorld::SendMsg(OBJID idNpc, CNetMsg* pMsg)
{
#ifdef	DUMP_DEBUG_MSG
    if (pMsg->GetType() == 1002)
    {
        LOGDUMP("Agent send login msg");
    }
    else if (!IsNpcID(idNpc) && idNpc != ID_NONE)
    {
        LOGDUMP("Agent send %u msg", pMsg->GetType());
    }
#endif
    NETMSGBUF	buf;
    int			nNetPacketSize = UniteNpcPacket(buf, sizeof(buf), pMsg->GetType(), pMsg->GetBuf(), pMsg->GetSize(), idNpc);
    ASSERT(nNetPacketSize);
    if (nNetPacketSize)
    {
#ifdef	PALED_DEBUG_X
        MSGBUF	szMsg;
        sprintf(szMsg, "111 发送消息：%u, NpcID：%u", pMsg->GetType(), idNpc);
        PrintText(szMsg);
#endif
        DEBUG_TRY
        if (!m_pShell->SendPacket(buf, nNetPacketSize))
        {
            SetLoginState(STATE_BREAK);
        }
        DEBUG_CATCH("m_pShell->SendPacket()")
        return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
void CNpcWorld::LoginOK()
{
    m_nLoginState = STATE_NORMAL;
    m_timeoutLogin.Update();
    char	szMsg[1024];
    sprintf(szMsg, "Login OK. generator monster after %d secs", LOGIN_DELAY_SECS);
    PrintText(szMsg);
    LOGDUMP(szMsg);
}







