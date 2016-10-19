
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgTalk.cpp: implementation of the CMsgTalk class.
//
//////////////////////////////////////////////////////////////////////

#define _WINSOCKAPI_
#include "AllMsg.h"
#ifdef	WORLD_KERNEL
#include "WorldKernel.h"
using namespace world_kernel;
#include "userlist.h"
#include "MessageBoard.h"
#else
#pragma warning(disable:4786)
#include "usermanager.h"
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgTalk::CMsgTalk()
{
    Init();
    m_pInfo	= (MSG_Info* )m_bufMsg;
    m_StrPacker.SetBuf(m_pInfo->szBuf, _MAX_MSGSIZE - sizeof(MSG_Info) + 1);
}

CMsgTalk::~CMsgTalk()
{
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgTalk::Create(char* pszSpeaker, char* pszHearer, char* pszWords,
                      char* pszEmotion, DWORD dwColor, unsigned short unAttribute,
                      unsigned short unStyle)
{
    if (!pszSpeaker || strlen(pszSpeaker) >= _MAX_NAMESIZE)
    {
        return false;
    }
    if (!pszHearer || strlen(pszHearer) >= _MAX_NAMESIZE)
    {
        return false;
    }
    if (!pszWords || strlen(pszWords) >= _MAX_WORDSSIZE)
    {
        return false;
    }
    // can be null
    if (pszEmotion && strlen(pszEmotion) >= _MAX_NAMESIZE)
    {
        return false;
    }
    // fill
    this->Init();
    // fill structure
    m_pInfo->dwWordsColor	= dwColor;
    m_pInfo->unTxtAttribute	= unAttribute;
    m_pInfo->unTxtStyle		= unStyle;
    m_pInfo->dwTime			=::SysTimeGet();
    BOOL bSucMake	= true;
    bSucMake	&= m_StrPacker.AddString(pszSpeaker);
    bSucMake	&= m_StrPacker.AddString(pszHearer);
    bSucMake	&= m_StrPacker.AddString(pszEmotion);
    bSucMake	&= m_StrPacker.AddString(pszWords);
    m_unMsgType	= _MSG_TALK;
    m_unMsgSize	= sizeof(MSG_Info) - 1 + m_StrPacker.GetSize();
    return bSucMake;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgTalk::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!pbufMsg)
    {
        return false;
    }
    memcpy(this->m_bufMsg, pbufMsg, dwMsgSize);
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgTalk::Process(void* pInfo)
{
#ifdef _MSGDEBUG
    ::LogMsg("Process MsgTalk, Sender:%s, Receiver:%s, Words:%s",
             szSender,
             szReceiver,
             szWords);
#endif
    char szSender[_MAX_NAMESIZE];
    char szReceiver[_MAX_NAMESIZE];
    char szEmotion[_MAX_NAMESIZE];
    char szWords[_MAX_WORDSSIZE];
    m_StrPacker.GetString(0, szSender, sizeof(szSender));
    m_StrPacker.GetString(1, szReceiver, sizeof(szReceiver));
    m_StrPacker.GetString(2, szEmotion, sizeof(szEmotion));
    m_StrPacker.GetString(3, szWords, sizeof(szWords));
    CPlayer* pUser = UserList()->GetPlayerBySocket(GetSocketID());
    CHECK(pUser || m_pInfo->unTxtAttribute == _TXTATR_MSG_SYSTEM);
    switch(m_pInfo->unTxtAttribute)
    {
    case _TXTATR_WEBPAGE:
        break;
    case _TXTATR_PRIVATE:
        {
            /*/ 转发耳语消息
            CPlayer* pPlayer = g_UserList.GetPlayer(szReceiver);
            if (pPlayer)
            {
            	pPlayer->SendMsg(this);
            }
            else
            {
            	CPlayer* pSender = g_UserList.GetPlayerBySocket(GetSocketID());
            	if (pSender)
            	{
            		if (this->Create(SYSTEM_NAME, pSender->szName, "对方不在线。"))
            			pSender->SendMsg(this);
            	}
            }//*/
        }
        break;
    case _TXTATR_MSG_TRADE:
        {
            GameWorld()->QueryTradeMsgBd()->AddMsg(pUser->GetID(), szSender, szWords);
        }
        break;
    case _TXTATR_MSG_FRIEND:
        {
            GameWorld()->QueryFriendMsgBd()->AddMsg(pUser->GetID(), szSender, szWords);
        }
        break;
    case _TXTATR_MSG_TEAM:
        {
            GameWorld()->QueryTeamMsgBd()->AddMsg(pUser->GetID(), szSender, szWords);
        }
        break;
    case _TXTATR_MSG_SYN:
        {
            OBJID idSyn = GetTransData();
            CSyndicateWorld* pSyn = SynWorldManager()->QuerySyndicate(idSyn);
            IF_OK(pSyn)
            pSyn->QueryMessageBoard()->AddMsg(pUser->GetID(), szSender, szWords);
        }
        break;
    case _TXTATR_MSG_OTHER:
        {
            GameWorld()->QueryOtherMsgBd()->AddMsg(pUser->GetID(), szSender, szWords);
        }
        break;
    case _TXTATR_MSG_SYSTEM:
        {
            OBJID idUser = GetTransData();
            GameWorld()->QuerySystemMsgBd()->AddMsg(idUser, szSender, szWords);
        }
        break;
    case _TXTATR_SERVE:
        {
            CPlayer* pPlayer = UserList()->GetGM();
            if (pPlayer)
            {
                pPlayer->SendMsg(this);
            }
            else
            {
                CPlayer* pSpeaker = UserList()->GetPlayerBySocket(GetSocketID());
                if (pSpeaker)
                {
                    CMsgTalk	msg;
                    if (msg.Create(SYSTEM_NAME, pSpeaker->szName, STR_TARGET_OFFLINE))
                    {
                        pSpeaker->SendMsg(&msg);
                    }
                }
            }
        }
        break;
    default:
        break;
    }
}
