
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgFriendInfo.cpp: implementation of the CMsgFriendInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "MsgFriendInfo.h"
#include "Hero.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgFriendInfo::CMsgFriendInfo()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgFriendInfo::~CMsgFriendInfo()
{
}
//--------------------------------------------------------------------
BOOL CMsgFriendInfo::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_FRIENDINFO != this->GetType())
    {
        return false;
    }
    return true;
}

//--------------------------------------------------------------------
void CMsgFriendInfo::Process(void* pInfo)
{
    if (m_pInfo->ucAction == _FRIEND_INFO)
    {
        CFriend* pFriend = g_objHero.GetFriend(m_pInfo->idFriend);
        if (!pFriend)
        {
            return;
        }
        pFriend->SetLevel((DWORD)m_pInfo->ucLevel);
        pFriend->SetFace((DWORD)(m_pInfo->dwLook / 10000) % 1000);
        pFriend->SetLook((DWORD)m_pInfo->dwLook % 1000);
        pFriend->SetPK((int)m_pInfo->sPk);
        pFriend->SetProfessional((DWORD)m_pInfo->ucProfession);
        OBJID idSyndicate = m_pInfo->dwSynID_Rank & 0x00ffffff;
        DWORD dwRank = m_pInfo->dwSynID_Rank >> 24;
        pFriend->SetSyndicateID(idSyndicate);
        pFriend->SetRank(dwRank);
        pFriend->SetMate(m_pInfo->szMate);
        pFriend->SetNobilityRank(m_pInfo->ucNobilityRank);
        if (pFriend->GetID() == g_objHero.GetCurrentQueryFriendID())
        {
            g_objHero.QueryFriendReturn();
        }
    }
    else
    {
        CEnemy* pEnemy = g_objHero.GetEnemy(m_pInfo->idFriend);
        if (!pEnemy)
        {
            return;
        }
        pEnemy->SetLevel((DWORD)m_pInfo->ucLevel);
        pEnemy->SetFace((DWORD)(m_pInfo->dwLook / 10000) % 1000);
        pEnemy->SetLook((DWORD)m_pInfo->dwLook % 1000);
        pEnemy->SetPK((int)m_pInfo->sPk);
        pEnemy->SetProfessional((DWORD)m_pInfo->ucProfession);
        OBJID idSyndicate = m_pInfo->dwSynID_Rank & 0x00ffffff;
        DWORD dwRank = m_pInfo->dwSynID_Rank >> 24;
        pEnemy->SetSyndicateID(idSyndicate);
        pEnemy->SetRank(dwRank);
        pEnemy->SetMate(m_pInfo->szMate);
        pEnemy->SetNobilityRank(m_pInfo->ucNobilityRank);
        if (pEnemy->GetID() == g_objHero.GetCurrentQueryEnemyID())
        {
            g_objHero.QueryEnemyReturn();
        }
    }
}
//--------------------------------------------------------------------
