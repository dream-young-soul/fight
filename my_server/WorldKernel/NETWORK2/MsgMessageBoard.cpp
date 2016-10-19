
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgTalk.cpp: implementation of the CMsgMessageBoard class.
//
//////////////////////////////////////////////////////////////////////

#define _WINSOCKAPI_
#include "AllMsg.h"
#ifdef	WORLD_KERNEL
#include "WorldKernel.h"
#include "MessageBoard.h"
using namespace world_kernel;
#include "userlist.h"
#else
#pragma warning(disable:4786)
#include "usermanager.h"
#endif

//WORLDKERNEL_BEGIN

const int	MSGTITLE_SIZE		= 32;
const int	MSGLIST_SIZE		= 8;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgMessageBoard::CMsgMessageBoard()
{
    Init();
    m_pInfo	= (MSG_Info* )m_bufMsg;
    m_StrPacker.SetBuf(m_pInfo->szBuf, _MAX_MSGSIZE - sizeof(MSG_Info) + 1);
}

CMsgMessageBoard::~CMsgMessageBoard()
{
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgMessageBoard::Create(int nAction, int nChannel, int nIndex, CMessageBoard* pObj)
{
    // fill
    this->Init();
    // fill structure
    m_pInfo->ucAction		= nAction;
    m_pInfo->usChannel		= nChannel;
    m_pInfo->usIndex		= nIndex;
    BOOL bSucMake	= true;
    if (pObj)
    {
        for(int i = m_pInfo->usIndex; i < m_pInfo->usIndex + MSGLIST_SIZE; i++)
        {
            if (!pObj->IsValid(i))
            {
                break;
            }
            bSucMake	&= m_StrPacker.AddString(pObj->GetMsgName(i));
            char	buf[MSGTITLE_SIZE];
            SafeCopy(buf, pObj->GetMsgWords(i), MSGTITLE_SIZE);
            bSucMake	&= m_StrPacker.AddString(buf);
            char	bufTime[_MAX_NAMESIZE];
            pObj->GetMsgTime(i, bufTime);
            bSucMake	&= m_StrPacker.AddString(bufTime);
        }
    }
    m_unMsgType	= _MSG_MESSAGEBOARD;
    m_unMsgSize	= sizeof(MSG_Info) - 1 + m_StrPacker.GetSize();
    return bSucMake;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgMessageBoard::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!pbufMsg)
    {
        return false;
    }
    memcpy(this->m_bufMsg, pbufMsg, dwMsgSize);
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgMessageBoard::Process(void* pInfo)
{
#ifdef _MSGDEBUG
    ::LogMsg("Process CMsgMessageBoard, Sender:%s, Receiver:%s, Words:%s",
             szSender,
             szReceiver,
             szWords);
#endif
    char szSender[_MAX_NAMESIZE] = "";
    m_StrPacker.GetString(0, szSender, sizeof(szSender));
    CMessageBoard* pObj = NULL;
    switch(m_pInfo->usChannel)
    {
    case _TXTATR_MSG_TRADE:
        {
            pObj = GameWorld()->QueryTradeMsgBd();
        }
        break;
    case _TXTATR_MSG_FRIEND:
        {
            pObj = GameWorld()->QueryFriendMsgBd();
        }
        break;
    case _TXTATR_MSG_TEAM:
        {
            pObj = GameWorld()->QueryTeamMsgBd();
        }
        break;
    case _TXTATR_MSG_SYN:
        {
            OBJID idSyn = GetTransData();
            CSyndicateWorld* pSyn = SynWorldManager()->QuerySyndicate(idSyn);
            IF_OK(pSyn)
            pObj = pSyn->QueryMessageBoard();
        }
        break;
    case _TXTATR_MSG_OTHER:
        {
            pObj = GameWorld()->QueryOtherMsgBd();
        }
        break;
    case _TXTATR_MSG_SYSTEM:
        {
            pObj = GameWorld()->QuerySystemMsgBd();
        }
        break;
    }
    if (!pObj)
    {
        return ;
    }
    CPlayer* pUser = UserList()->GetPlayerBySocket(GetSocketID());
    if (!pUser)
    {
        return ;
    }
    switch(m_pInfo->ucAction)
    {
    case	MESSAGEBOARD_DEL:
        {
            pObj->DelMsg(pUser->GetID());
        }
        break;
    case	MESSAGEBOARD_GETLIST:
        {
            if (pObj->IsValid(m_pInfo->usIndex))
            {
                CMsgMessageBoard	msg;
                IF_OK(msg.Create(MESSAGEBOARD_LIST, m_pInfo->usChannel, m_pInfo->usIndex, pObj))
                SendMsg(&msg);
            }
        }
        break;
    case	MESSAGEBOARD_GETWORDS:
        {
            CHECK(strlen(szSender) > 0);
            CMessageBoard::MessageInfo* pInfo;
            pInfo = pObj->GetMsgInfo(szSender);
            if (pInfo)
            {
                NAMESTR		buf;
                ::DateTimeStamp(buf, pInfo->time);
                CMsgTalk	msg;
                IF_OK(msg.Create((char*)pInfo->name, ALLUSERS_NAME, (char*)pInfo->words, buf, 0xFFFFFF, m_pInfo->usChannel))
                SendMsg(&msg);
            }
        }
        break;
    }
}

//WORLDKERNEL_END
