
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgFriend.cpp: interface for the CMsgFriend class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "MsgFriend.h"
#include "MapGroup.h"
#pragma	warning(disable:4786)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgFriend::CMsgFriend()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgFriend::~CMsgFriend()
{
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgFriend::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_FRIEND != this->GetType())
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgFriend::Create(int nAction, OBJID idFriend, LPCTSTR szName, bool bOnline /*= false*/, DWORD nLookFace/*=0*/, int nLevel/*=0*/)
{
    CHECKF(idFriend && nAction && szName);
    // init
    this->Init();
    // fill info now
    m_pInfo->idFriend		= idFriend;
    m_pInfo->dwLookFace		= nLookFace;
    m_pInfo->ucAction		= nAction;
    m_pInfo->ucOnlineFlag	= bOnline;
    m_pInfo->ucLevel		= nLevel;
    SafeCopy(m_pInfo->szName, szName, _MAX_NAMESIZE);
    m_unMsgType	= _MSG_FRIEND;
    m_unMsgSize	= sizeof(MSG_Info);
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgFriend::Process(void* pInfo)
{
#ifdef _MSGDEBUG
    ::LogMsg("Process CMsgFriend, id:%u", m_pInfo->id);
#endif
    CUserPtr pUser = UserManager()->GetUser(this);
    if (!pUser)
    {
        if (IsNpcMsg())
        {
            return;
        }
        // TransmitMsg
        switch(m_pInfo->ucAction)
        {
        case _FRIEND_GETINFO:
            {
                OBJID idFriend = m_pInfo->idFriend;
                CUser* pTarget = UserManager()->GetUser(idFriend);
                if (pTarget)
                {
                    IF_OK(Create(_FRIEND_ONLINE, idFriend, pTarget->GetName(), ONLINE_STATUS, pTarget->GetLookFace(), pTarget->GetLev()))
                    SendMsg(this);
                }
            }
            break;
        case _ENEMY_ADD:
            {
                OBJID idFriend = m_pInfo->idFriend;
                CUser* pTarget = UserManager()->GetUser(idFriend);
                if (pTarget)
                {
                    IF_OK(Create(_ENEMY_ONLINE, idFriend, m_pInfo->szName, ONLINE_STATUS, pTarget->GetLookFace(), pTarget->GetLev()))
                    SendMsg(this);
                }
            }
            break;
        case _FRIEND_BREAK:
            {
                OBJID idFriend = m_pInfo->idFriend;
                CUser* pTarget = UserManager()->GetUser(idFriend);
                if (pTarget)
                {
                    pTarget->DelFriend(m_pInfo->idTransmit);
                }
            }
            break;
        }
        return;
    }
    if (!pUser->IsAlive())
    {
        pUser->SendSysMsg(STR_DIE);
        return;
    }
    switch(m_pInfo->ucAction)
    {
    case _FRIEND_APPLY:
        {
            CUser* pTarget = NULL;
            if (!pUser->GetMap()->QueryObj(pUser->GetPosX(), pUser->GetPosY(), OBJ_USER, m_pInfo->idFriend, IPP_OF(pTarget)))
            {
                break;
            }
            if (pUser->GetFriendAmount() >= _MAX_USERFRIENDSIZE)
            {
                pUser->SendSysMsg(STR_FRIEND_LIST_FULL);
                break;
            }
            if (pUser->GetFriend(m_pInfo->idFriend))
            {
                pUser->SendSysMsg(STR_YOUR_FRIEND_ALREADY);
                break;
            }
            if (pTarget->FetchApply(CUser::APPLY_FRIEND) != pUser->GetID())
            {
                // 如果对方未曾向自己发出过加入好友请求
                pUser->SetApply(CUser::APPLY_FRIEND, m_pInfo->idFriend);
                IF_OK(Create(_FRIEND_APPLY, pUser->GetID(), pUser->GetName(), true))
                pTarget->SendMsg(this);
                pTarget->SendSysMsg(STR_TO_MAKE_FRIEND, pUser->GetName());
                pUser->SendSysMsg(STR_MAKE_FRIEND_SENT);
            }
            else
            {
                // 如果对方已经向自己发出过请求，直接为双方加好友
                if (pUser->AddFriend(m_pInfo->idFriend, m_pInfo->szName) &&
                        pTarget->AddFriend(pUser->GetID(), pUser->GetName()))
                {
                    if (this->Create(_FRIEND_GETINFO, pTarget->GetID(), pTarget->GetName(), ONLINE_STATUS, pTarget->GetLookFace(), pTarget->GetLev()))
                    {
                        pUser->SendMsg(this);
                    }
                    if (this->Create(_FRIEND_GETINFO, pUser->GetID(), pUser->GetName(), ONLINE_STATUS, pUser->GetLookFace(), pUser->GetLev()))
                    {
                        pTarget->SendMsg(this);
                    }
                    MSGBUF	szMsg;
                    sprintf(szMsg, STR_MAKE_FRIEND, pUser->GetName(), pTarget->GetName());
                    pUser->BroadcastRoomMsg(szMsg, true);
                }
                else
                {
                    ::LogSave("Error: add friend failed");
                }
            }
        }
        break;
    case _FRIEND_ONLINE:
        {
            ASSERT(!"_FRIEND_ONLINE");
        }
        break;
    case _FRIEND_OFFLINE:
        {
            ASSERT(!"_FRIEND_OFFLINE");
        }
        break;
    case _FRIEND_BREAK:
        {
            CUser* pTarget = UserManager()->GetUser(m_pInfo->idFriend);		// 可能为NULL
            CFriendPtr pFriend = pUser->GetFriend(m_pInfo->idFriend);
            if (!pFriend)
            {
                if (pTarget)
                {
                    pUser->SendSysMsg(STR_NOT_YOUR_FRIEND, pTarget->GetName());
                }
                break;
            }
            NAMESTR	szFriend = "";
            SafeCopy(szFriend, pFriend->GetFriendName(), _MAX_NAMESIZE);
            if (pUser->DelFriend(m_pInfo->idFriend))
            {
                MSGBUF	szMsg;
                sprintf(szMsg, STR_BREAK_FRIEND, pUser->GetName(), szFriend);
                pUser->BroadcastRoomMsg(szMsg, true);
                pUser->SendMsg(this);
            }
            if (pTarget)
            {
                if (pTarget->DelFriend(pUser->GetID()))
                {
                    if (this->Create(_FRIEND_BREAK, pUser->GetID(), pUser->GetName()))
                    {
                        pTarget->SendMsg(this);
                    }
                }
            }
            else	// 对方不在线
            {
                SQLBUF	szSQL;
                sprintf(szSQL, "DELETE FROM %s WHERE userid=%u && friend=%u", _TBL_FRIEND, m_pInfo->idFriend, pUser->GetID());
                Database()->ExecuteSQL(szSQL);
                m_pInfo->idTransmit	= pUser->GetID();
                MapGroup(PID)->QueryIntraMsg()->TransmitMsg(this, GetSocketID(), GetNpcID());
            }
        }
        break;
    case _FRIEND_GETINFO:
        {
            ASSERT(!"_FRIEND_GETINFO");
        }
        break;
    case _ENEMY_DEL:
        {
            pUser->QueryEnemy()->Del(m_pInfo->idFriend, UPDATE_TRUE);
        }
        break;
    }
}
