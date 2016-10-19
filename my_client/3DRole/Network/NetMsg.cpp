
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// NetMsg.cpp: implementation of the CNetMsg class.
//
//////////////////////////////////////////////////////////////////////
#include "AllMsg.h"
#include "basefunc.h"
#include "hero.h"
#include "gameplayerset.h"
#include "Entrance.h"
#include "GameDataSet.h"

// global...
CNetwork	g_objNetwork;
CHAR		g_szServerName[256] = "";

//////////////////////////////////////////////////////////////////////
void CNetwork::ProcessNetMsg()
{
    MYASSERT (m_pSocket);
#ifndef _NOTUSETRY
    try
    {
#endif
        //		if (!ClientSocketCheck())	// network failed
        if (!m_pSocket->Check())		// network failed
        {
            extern UINT g_uStatus;
            g_uStatus	= _STATUS_EXIT;
            g_objEntrance.ClearMsg();
            char szMsg[256];
            const OBJID idReConnect = 100116;
            sprintf(szMsg, g_objGameDataSet.GetStr(idReConnect));
            g_objEntrance.AddMsg(szMsg);
            this->Destroy();
            return;
        }
        while (true)
        {
            char buffer[1024];
            int nSize	= sizeof(buffer);
            char* pbufMsg	= buffer;
            if (!m_pSocket->ReceiveMsg(pbufMsg, nSize))
            {
                break;
            }
            while (true)
            {
                DWORD	dwCurMsgSize	= CNetMsg::GetSize(pbufMsg, nSize);
                if ((int)dwCurMsgSize > nSize || dwCurMsgSize <= 0)	// broken msg
                {
                    break;
                }
                CNetMsg* pMsg	= CNetMsg::CreateMsg(pbufMsg, dwCurMsgSize);
                if (pMsg)
                {
                    int nMsgType	= pMsg->GetType();
#ifndef _NOTUSETRY
                    try
                    {
#endif
                        pMsg->Process(NULL);
#ifndef _NOTUSETRY
                    }
                    catch(...)
                    {
                        ::LogMsg("catch error in process msg:%u.", nMsgType);
                        // dump msg
                        char szDumpMsg[4096] = "";
                        BYTE* pBuf = (BYTE*)pMsg->GetBuf();
                        pBuf += 4;
                        int nSize = __min(pMsg->GetSize() - 4, 256);
                        for (int i = 0; i < nSize; i++)
                        {
                            char szValue[256] = "";
                            _itoa(pBuf[i], szValue, 16);
                            strcat(szValue, " ");
                            strcat(szDumpMsg, szValue);
                        }
                        ::LogMsg(szDumpMsg);
                    }
#endif
                    SAFE_DELETE(pMsg);
                    m_dwBytesReceived += dwCurMsgSize;
                }
                pbufMsg	+= dwCurMsgSize;
                nSize	-= dwCurMsgSize;
                if (nSize <= 0)
                {
                    break;
                }
            }
        }
        // flush net msg
        m_pSocket->Flush();
#ifndef _NOTUSETRY
    }
    catch(...)
    {
        ::LogMsg("catch error in process msg.");
    }
#endif
    return;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CNetwork::CNetwork()
{
    m_bReady		= false;
    m_pSocket		= NULL;
}

BOOL CNetwork::Init(const char* pszServerIP, int nServerPort)
{
    if (!pszServerIP || nServerPort < 0)
    {
        return false;
    }
    this->Destroy();
    // init now
    m_dwBytesReceived	= 0;
    m_dwBytesSend		= 0;
    //if (::ClientSocketInit(pszServerIP, nServerPort))
    m_pSocket = new CLoginSocket;
    if (!m_pSocket)
    {
        return false;
    }
    if (m_pSocket->Create((char*)pszServerIP, nServerPort))
    {
        this->m_bReady	= true;
        return true;
    }
    else
    {
        this->m_bReady	= false;
        return false;
    }
    m_dwMsgCount	= 0;
}

//////////////////////////////////////////////////////////////////////
void CNetwork::Destroy()
{
    if (m_bReady)
    {
        // ::ClientSocketDestroy();
        m_pSocket->Flush();
        m_pSocket->Destroy();
        m_bReady	= false;
    }
    SAFE_DELETE(m_pSocket);
}

//////////////////////////////////////////////////////////////////////
BOOL CNetwork::SendMsg(char* pbufMsg, DWORD dwSize)
{
    if (!m_bReady)
    {
        return false;
    }
    BOOL bSucSend;
    try
    {
        //bSucSend	=::ClientSocketSendMsg(pbufMsg, dwSize);
        bSucSend	= m_pSocket->SendMsg(pbufMsg, dwSize);
    }
    catch(...)
    {
        ::LogMsg("catch error in send msg.");
    }
    m_dwBytesSend += dwSize;
    this->AddMsgCount();
    return bSucSend;
}

//////////////////////////////////////////////////////////////////////
DWORD CNetwork::GetMsgCount(void)
{
    if (m_dwMsgCount == 0)
    {
        return 0;
    }
    return m_dwMsgCount ^ g_objHero.GetID();
}

//////////////////////////////////////////////////////////////////////
void CNetwork::	AddMsgCount	(int nAddValue/* = 1*/)
{
    DWORD dwCount = this->GetMsgCount() + nAddValue;
    m_dwMsgCount = dwCount ^ g_objHero.GetID();
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CNetMsg::CNetMsg()
{
    Init();
}

CNetMsg::~CNetMsg()
{
}

//////////////////////////////////////////////////////////////////////
void CNetMsg::Init()
{
    memset(m_bufMsg, 0L, sizeof(m_bufMsg));
    m_unMsgSize	= 0;
    m_unMsgType	= _MSG_NONE;
}

//////////////////////////////////////////////////////////////////////
BOOL CNetMsg::IsValid(void)
{
    if (_MSG_NONE == this->GetType())
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CNetMsg::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!pbufMsg)
    {
        return false;
    }
    if ((unsigned short)dwMsgSize != GetSize(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_NONE == GetType(pbufMsg, dwMsgSize))
    {
        return false;
    }
    memcpy(this->m_bufMsg, pbufMsg, dwMsgSize);
    return true;
}

//////////////////////////////////////////////////////////////////////
void CNetMsg::Process(void* pInfo)
{
    //--------------------
    char szMsg[1024];
    sprintf(szMsg, "Process Msg:%d, Size:%d", m_unMsgType, m_unMsgSize);
    ::LogMsg(szMsg);
}

//////////////////////////////////////////////////////////////////////
void CNetMsg::Send(void)
{
#ifdef _MYDEBUG
    ::LogMsg("SendMsg:%d, size:%d", m_unMsgType, m_unMsgSize);
#endif
    g_objNetwork.SendMsg(m_bufMsg, m_unMsgSize);
}

//////////////////////////////////////////////////////////////////////
// static functions;
//////////////////////////////////////////////////////////////////////
unsigned short	CNetMsg::GetType(char* pbufMsg, DWORD dwMsgSize)
{
    // check it...
    assert(pbufMsg);
    assert((int)dwMsgSize <= CNetMsg::GetMaxSize());
    unsigned short* pUnShort	= (unsigned short* )pbufMsg;
    return pUnShort[1];
}

//////////////////////////////////////////////////////////////////////
unsigned short	CNetMsg::GetSize(char* pbufMsg, DWORD dwMsgSize)
{
    // check it...
    assert(pbufMsg);
    assert((int)dwMsgSize <= CNetMsg::GetMaxSize());
    unsigned short* pUnShort	= (unsigned short* )pbufMsg;
    return pUnShort[0];
}

//////////////////////////////////////////////////////////////////////
CNetMsg* CNetMsg::CreateMsg(char* pbufMsg, DWORD dwMsgSize)
{
    // check it...
    if (!pbufMsg || (int)dwMsgSize > GetMaxSize())
    {
        return NULL;
    }
    assert((int)dwMsgSize == CNetMsg::GetSize(pbufMsg, dwMsgSize));
    // make it...
#ifdef _DEBUG
    ::DebugMsg("MsgType:%d\n", CNetMsg::GetType(pbufMsg,dwMsgSize));
#endif
    CNetMsg* pMsg	= NULL;
    switch(CNetMsg::GetType(pbufMsg, dwMsgSize))
    {
    case _MSG_LOGIN:
        pMsg	= new CMsgLogin;
        break;
    case _MSG_TALK:
        pMsg	= new CMsgTalk;
        break;
    case _MSG_ACTION:
        pMsg	= new CMsgAction;
        break;
    case _MSG_PLAYER:
        pMsg	= new CMsgPlayer;
        break;
    case _MSG_TICK:
        pMsg	= new CMsgTick;
        break;
    case _MSG_USERINFO:
        pMsg	= new CMsgUserInfo;
        break;
    case _MSG_FRIEND:
        pMsg	= new CMsgFriend;
        break;
    case _MSG_ITEM:
        pMsg	= new CMsgItem;
        break;
    case _MSG_ITEMINFO:
        pMsg	= new CMsgItemInfo;
        break;
    case _MSG_INTERACT:
        pMsg	= new CMsgInteract;
        break;
    case _MSG_NAME:
        pMsg	= new CMsgName;
        break;
    case _MSG_USERATTRIB:
        pMsg	= new CMsgUserAttrib;
        break;
    case _MSG_WALK:
        pMsg	= new CMsgWalk;
        break;
    case _MSG_MAPITEM:
        pMsg	= new CMsgMapItem;
        break;
    case _MSG_TRADE:
        pMsg	= new CMsgTrade;
        break;
    case _MSG_NPC:
        pMsg	= new CMsgNpc;
        break;
    case _MSG_NPCINFO:
        pMsg	= new CMsgNpcInfo;
        break;
    case _MSG_PACKAGE:
        pMsg	= new CMsgPackage;
        break;
    case _MSG_WEAPONSKILL:
        pMsg	= new CMsgWeaponSkill;
        break;
    case _MSG_ACCOUNT:
        pMsg	= new CMsgAccount;
        break;
    case _MSG_CONNECT:
        pMsg	= new CMsgConnect;
        break;
    case _MSG_TEAM:
        pMsg	= new CMsgTeam;
        break;
    case _MSG_TEAMMEMBER:
        pMsg	= new CMsgTeamMember;
        break;
    case _MSG_FLUSHEXP:
        pMsg	= new CMsgFlushExp;
        break;
    case _MSG_TASKDIALOG:
        pMsg	= new CMsgTaskDialog;
        break;
    case _MSG_MAGICEFFECT:
        pMsg	= new CMsgMagicEffect;
        break;
    case _MSG_MAGICINFO:
        pMsg	= new CMsgMagicInfo;
        break;
    case _MSG_SYNDICATE:
        pMsg	= new CMsgSyndicate;
        break;
    case _MSG_SYNATTRINFO:
        pMsg	= new CMsgSyndicateAttributeInfo;
        break;
    case _MSG_WEATHER:
        pMsg = new CMsgWeather;
        break;
    case _MSG_FRIENDINFO:
        pMsg	= new CMsgFriendInfo;
        break;
    case _MSG_GEMEMBED:
        pMsg	= new CMsgGemEmbed;
        break;
    case _MSG_TIME:
        pMsg	= new CMsgTime;
        break;
    case _MSG_ITEMINFOEX:
        pMsg	= new CMsgItemInfoEx;
        break;
    case _MSG_NPCINFOEX:
        pMsg	= new CMsgNpcInfoEX;
        break;
    case _MSG_MAPINFO:
        pMsg	= new CMsgMapInfo;
        break;
    case _MSG_MESSAGEBOARD:
        pMsg	= new CMsgMessageBoard;
        break;
    case _MSG_SYNMEMBERINFO:
        pMsg	= new CMsgSynMemberInfo;
        break;
    case _MSG_DICE:
        pMsg	= new CMsgDice;
        break;
    case _MSG_SYNINFO:
        pMsg    = new CMsgSyndicateInfo;
        break;
    case _MSG_MAGICINTONE:
        pMsg    = new CMsgMagicIntone;
        break;
    case _MSG_PETINFO:
        pMsg	= new CMsgPetInfo;
        break;
    case _MSG_DATAARRAY:
        pMsg	= new CMsgDataArray;
        break;
    case _MSG_WALKEX:
        pMsg	= new CMsgWalkEx;
        break;
    case _MSG_EUDEMONATTRIB:
        pMsg	= new CMsgEudemonAttrib;
        break;
    case _MSG_PLAYERTASK:
        pMsg	= new CMsgPlayerTask;
        break;
    case _MSG_MESSAGESCHOOLMEMBER:
        pMsg	= new CMsgSchoolMember;
        break;
    case _MSG_TASK_LIST:
        pMsg    = new CMsgTaskList;
        break;
    case _MSG_ANNOUNCE_INFO:
        pMsg	= new CMsgAnnounceInfo;
        break;
    case _MSG_ANNOUNCE_LIST:
        pMsg	= new CMsgAnnounceList;
        break;
    case _MSG_AUCTION:
        pMsg    = new CMsgAuction;
        break;
    case _MSG_ITEMATTRIB:
        pMsg    = new CMsgItemAttrib;
        break;
    case _MSG_LOGINREPLYEX:
        pMsg = new CMsgLoginReplyEx;
        break;
    case _MSG_LOGINACCOUNTEX:
        pMsg = new CMsgLoginAccountEx;
        break;
    case _MSG_EUDEMONINFO:
        pMsg = new CMsgEudemonInfo;
        break;
    default:
        break;
    }
    if (!pMsg)
    {
        return NULL;
    }
    if (!pMsg->Create(pbufMsg, dwMsgSize))
    {
        SAFE_DELETE(pMsg);
        return NULL;
    }
    else
    {
#ifdef _MYDEBUG
        ::LogMsg("\n--Receive Msg:%u, Size:%u", pMsg->GetType(), pMsg->GetSize());
#endif
        return pMsg;
    }
}
