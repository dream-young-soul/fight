
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// NetMsg.cpp: implementation of the CNetMsg class.
//
//////////////////////////////////////////////////////////////////////
#include "AllMsg.h"
#include "common.h"
#include "NpcWorld.h"

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
    memcpy(this->m_bufMsg, pbufMsg, dwMsgSize);
    return true;
}

//////////////////////////////////////////////////////////////////////
void CNetMsg::Process(void* pInfo)
{
    //--------------------
    char szMsg[1024];
    sprintf(szMsg, "Process Msg:%d, Size:%d", m_unMsgType, m_unMsgSize);
    LOGERROR(szMsg);
}

//////////////////////////////////////////////////////////////////////
bool CNetMsg::SendMsg(CNetMsg* pMsg)
{
#ifdef _MYDEBUG
    //	LOGERROR("SendMsg:%d, size:%d", m_unMsgType, m_unMsgSize);
#endif
    return NpcWorld()->SendMsg(m_idNpc, pMsg);
}

//////////////////////////////////////////////////////////////////////
// static functions;
//////////////////////////////////////////////////////////////////////
CNetMsg* CNetMsg::CreateMsg(OBJID idNpc, OBJID idMsg, const char* pbufMsg, DWORD dwMsgSize)
{
    // check it...
    if (idMsg == _MSG_NONE || !pbufMsg || (int)dwMsgSize > GetMaxSize())
    {
        return NULL;
    }
    // make it...
    CNetMsg* pMsg	= NULL;
    switch(idMsg)
    {
    case	_MSG_PLAYER:
        pMsg	= new CMsgPlayer;
        break;
    case _MSG_TALK:
        pMsg	= new CMsgTalk;
        break;
    case _MSG_NPC:
        pMsg	= new CMsgNpc;
        break;
    case _MSG_AINPCINFO:
        pMsg	= new CMsgAiNpcInfo;
        break;
    case _MSG_LOGIN:
        pMsg	= new CMsgLogin;
        break;
    case _MSG_ACTION:
        pMsg	= new CMsgAction;
        break;
    case _MSG_INTERACT:
        pMsg	= new CMsgInteract;
        break;
    case _MSG_WALK:
        pMsg	= new CMsgWalk;
        break;
    case _MSG_USERATTRIB:
        pMsg	= new CMsgUserAttrib;
        break;
    case _MSG_SYNDICATE:
        pMsg	= new CMsgSyndicate;
        break;
    case _MSG_MAGICEFFECT:
        pMsg	= new CMsgMagicEffect;
        break;
    case _MSG_USERINFO:
        pMsg	= new CMsgUserInfo;
        break;
    case _MSG_ITEM:
        pMsg	= new CMsgItem;
        break;
    case _MSG_ITEMINFO:
        pMsg	= new CMsgItemInfo;
        break;
    case _MSG_TEAM:
        pMsg	= new CMsgTeam;
        break;
    case _MSG_TEAMMEMBER:
        pMsg	= new CMsgTeamMember;
        break;
    case _MSG_WALKEX:
        pMsg	= new CMsgWalkEx;
        break;
    default:
        break;
    }
    if (!pMsg)
    {
        return NULL;
    }
    pMsg->m_idNpc		= idNpc;
    pMsg->m_unMsgType	= idMsg;
    pMsg->m_unMsgSize	= dwMsgSize;
    if (!pMsg->CNetMsg::Create((char*)pbufMsg, dwMsgSize))
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
