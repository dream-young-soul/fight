
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// file name: herofriend.cpp

// include head file(s) here
#include "hero.h"
#include "GameMsg.h"
#include "allmsg.h"
#include "GamePlayerset.h"
#include "GameDataSet.h"

// the operate interface of friend to hero

///////////////////////////////////////////////////////////////

BOOL CHero::AddFriend(CFriend* pFriend)
{
    if (!pFriend)
    {
        return false;
    }
    m_setFriend.push_back(pFriend);
    {
        ::PostCmd(CMD_FLASHFRIENDLIST);
        return true;
    }
}
///////////////////////////////////////////////////////////////

BOOL CHero::DelFriend(OBJID id)
{
    if (id == ID_NONE)
    {
        return false;
    }
    int nAmount = m_setFriend.size();
    for(int i = 0; i < nAmount; i++)
    {
        CFriend* pFriend = m_setFriend[i];
        if (pFriend && pFriend->GetID() == id)
        {
            SAFE_DELETE(pFriend);
            m_setFriend.erase(m_setFriend.begin() + i);
            ::PostCmd(CMD_FLASHFRIENDLIST);
            return true;
        }
    }
    return false;
}

///////////////////////////////////////////////////////////////
BOOL CHero::MakeFriend(OBJID id)
{
    CPlayer* pPlayer = g_objPlayerSet.GetPlayer(id);
    if (!pPlayer)
    {
        return false;
    }
    int nRealFrinedAmount = 0;
    int nAmount = this->GetFriendAmount();
    for(int i = 0; i < nAmount; i++)
    {
        CFriend* pFriend = this->GetFriendByIndex(i);
        if ((pFriend) && (pFriend->GetID() != ID_NONE))
        {
            nRealFrinedAmount++;
        }
    }
    // check amount
    if (nRealFrinedAmount >= _MAX_FRIEND_AMOUNT)
    {
        char szTemp[255];
        OBJID idCanNotMakeMoreFriend = 100041;
        sprintf(szTemp, g_objGameDataSet.GetStr(idCanNotMakeMoreFriend),
                _MAX_FRIEND_AMOUNT);
        g_objGameMsg.AddMsg(szTemp);
        return false;
    }
    // check the friend list of hero
    CFriend* pFriend = this->GetFriend(id);
    if (pFriend)
    {
        char szTemp[255];
        OBJID idIsFriend = 100042;
        sprintf(szTemp, g_objGameDataSet.GetStr(idIsFriend), pPlayer->GetName());
        g_objGameMsg.AddMsg(szTemp);
        return false;
    }
    // send msg
    CMsgFriend msg;
    MYASSERT (msg.Create( _FRIEND_APPLY, id, false, pPlayer->GetName()));
    msg.Send();
    return true;
}

///////////////////////////////////////////////////////////////
BOOL CHero::BreakOffFriend(OBJID id)
{
    // check id
    if (id == ID_NONE)
    {
        return false;
    }
    // check the friend list of hero
    CFriend* pFriend = this->GetFriend(id);
    if (!pFriend)
    {
        //char szMsg[]	="´íÎó£ºÄãÃ»ÓÐÕâ¸ö½á°ÝÐÖµÜ£¡";
        OBJID idIsNotFriend = 100043;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idIsNotFriend));
        return false;
    }
    // send msg
    CMsgFriend msg;
    MYASSERT (msg.Create( _FRIEND_BREAK, id, false, pFriend->GetName()));
    msg.Send();
    return true;
}

///////////////////////////////////////////////////////////////
int  CHero::GetFriendAmount()
{
    return m_setFriend.size();
}

///////////////////////////////////////////////////////////////
CFriend* CHero::GetFriend(OBJID id)
{
    if (id == ID_NONE)
    {
        return NULL;
    }
    int nAmount = m_setFriend.size();
    for(int i = 0; i < nAmount; i++)
    {
        CFriend* pFriend = m_setFriend[i];
        if (pFriend && pFriend->GetID() == id)
        {
            return pFriend;
        }
    }
    return NULL;
}
///////////////////////////////////////////////////////////////

CFriend* CHero::GetFriendByIndex(int nIndex)
{
    if (nIndex < 0)
    {
        return NULL;
    }
    int nAmount = m_setFriend.size();
    if (nIndex >= nAmount)
    {
        return NULL;
    }
    return m_setFriend[nIndex];
}

///////////////////////////////////////////////////////////////
void CHero::ClearFriendList()
{
    int nAmount = m_setFriend.size();
    for(int i = 0; i < nAmount; i++)
    {
        CFriend* pFriend = m_setFriend[i];
        SAFE_DELETE(pFriend);
    }
    m_setFriend.clear();
}

///////////////////////////////////////////////////////////////
CFriend* CHero::GetFriendByName(const char* pszName)
{
    if (!pszName)
    {
        return NULL;
    }
    int nAmount = this->GetFriendAmount();
    for(int i = 0; i < nAmount; i++)
    {
        CFriend* pFriend = this->GetFriendByIndex(i);
        if ((pFriend) && (strcmp(pFriend->GetName(), pszName) == 0 ))
        {
            return pFriend;
        }
    }
    return NULL;
}

///////////////////////////////////////////////////////////////
void CHero::Court(CPlayer* pTarget)
{
    if (!pTarget)
    {
        return;
    }
    CMyPos posHero;
    this->GetPos(posHero);
    CMsgInteract msg;
    MYASSERT (msg.Create(INTERACT_COURT, this->GetID(), pTarget->GetID(), posHero.x, posHero.y));
    msg.Send();
}

///////////////////////////////////////////////////////////////
void CHero::Marriage(CPlayer* pTarget)
{
    if (!pTarget)
    {
        return;
    }
    CMyPos posHero;
    this->GetPos(posHero);
    CMsgInteract msg;
    MYASSERT (msg.Create(INTERACT_MARRY, this->GetID(), pTarget->GetID(), posHero.x, posHero.y));
    msg.Send();
}

///////////////////////////////////////////////////////////////
void CHero::Divorce(CPlayer* pTarget)
{
    if (!pTarget)
    {
        return;
    }
    CMyPos posHero;
    this->GetPos(posHero);
    CMsgInteract msg;
    MYASSERT (msg.Create(INTERACT_DIVORCE, this->GetID(), pTarget->GetID(), posHero.x, posHero.y));
    msg.Send();
}
///////////////////////////////////////////////////////////////
void CHero::QueryFriend	(OBJID id)
{
    if (m_bFriendQueryBusy)
    {
        return;
    }
    // stary busy
    m_bFriendQueryBusy = true;
    m_dwFriendBeginQuery = ::TimeGet();
    m_idFriendQuery = id;
    m_bFriendQueryReturn = false;
    // send msg
    CMsgAction msg;
    if (msg.Create(this->GetID(), 0, 0, 0, actionQueryFriendInfo, id))
    {
        msg.Send();
    }
}

///////////////////////////////////////////////////////////////
void CHero::ProcessFriend()
{
    const DWORD dwTimeout = 8 * 1000;
    if (!m_bFriendQueryBusy)
    {
        return;
    }
    if (m_bFriendQueryReturn)
    {
        // is syndicatename ready?
        CFriend* pFriend = this->GetFriend(m_idFriendQuery);
        if (pFriend)
        {
            if (strlen(pFriend->GetSyndicateName()) != 0)
            {
                m_bFriendQueryBusy = false;
                ::PostCmd(CMD_QUERYFRIEND_OK);
            }
        }
    }
    else if (::TimeGet() - m_dwFriendBeginQuery > dwTimeout)
    {
        m_bFriendQueryBusy = false;
        m_idFriendQuery = ID_NONE;
        ::PostCmd(CMD_QUERYFRIEND_TIMEOUT);
    }
}
///////////////////////////////////////////////////////////////
void CHero::LeaveWords2Friend (OBJID idFriend, char* pszWords)
{
    if (!pszWords)
    {
        return;
    }
    CFriend* pFriend = this->GetFriend(idFriend);
    if (!pFriend || pFriend->IsOnline())
    {
        return;
    }
    g_objHero.Talk(pFriend->GetName(), pszWords, NULL, 0xffffff00, _TXTATR_LEAVEWORD);
}
///////////////////////////////////////////////////////////////
OBJID	CHero::GetIdFriendLastApply()
{
    return m_idFriendLastApply;
}
///////////////////////////////////////////////////////////////
void	CHero::SetIdFriendLastApply(OBJID id)
{
    m_idFriendLastApply = id;
}
///////////////////////////////////////////////////////////////
const   char*	CHero::GetFriendLastApplyInfo()
{
    return m_szFriendLastApplyInfo;
}
///////////////////////////////////////////////////////////////
void	CHero::SetFriendLastApplyInfo(const char* pszInfo)
{
    if (pszInfo)
    {
        strcpy(m_szFriendLastApplyInfo, pszInfo);
    }
}
///////////////////////////////////////////////////////////////
const	char*	CHero::GetFriendLastAcceptInfo()
{
    return m_szFriendLastAcceptInfo;
}
///////////////////////////////////////////////////////////////
void	CHero::SetFriendLastAcceptInfo(const char* pszInfo)
{
    if (pszInfo)
    {
        strcpy(m_szFriendLastAcceptInfo, pszInfo);
    }
}
///////////////////////////////////////////////////////////////
