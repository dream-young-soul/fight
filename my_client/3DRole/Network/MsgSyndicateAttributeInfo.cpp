
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgSyndicateAttributeInfo.cpp: implementation of the CMsgSyndicateAttributeInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "MsgSyndicateAttributeInfo.h"
#include "Hero.h"
#include "GamePlayerSet.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgSyndicateAttributeInfo::CMsgSyndicateAttributeInfo()
{
    Init();
    m_pInfo	= (MSG_Info* )m_bufMsg;
}

CMsgSyndicateAttributeInfo::~CMsgSyndicateAttributeInfo()
{
}

//---------------------------------------------------------------------

BOOL CMsgSyndicateAttributeInfo::Create(char* pMsgBuf, DWORD dwSize)
{
    if (!CNetMsg::Create(pMsgBuf, dwSize))
    {
        return false;
    }
    if (_MSG_SYNATTRINFO != this->GetType())
    {
        return false;
    }
    return true;
}

//---------------------------------------------------------------------

void CMsgSyndicateAttributeInfo::Process(void* pInfo)
{
    if (g_objHero.GetLastSyndicateID() != ID_NONE && m_pInfo->idSyndicate == ID_NONE)
    {
        g_objPlayerSet.ClearAllSyndicateStatus();
    }
    g_objHero.SetSyndicateID(m_pInfo->idSyndicate);
    g_objHero.SetSyndicateRank(m_pInfo->ucRank);
    g_objHero.SetSyndicateProffer(m_pInfo->nProffer);
    g_objHero.SetSyndicateFund(m_pInfo->dwSyndicateFund);
    g_objHero.SetSyndicateFamer(m_pInfo->dwSyndicateFamer);
    g_objHero.SetSyndicateSynRank(m_pInfo->ucSynRank);
    g_objHero.SetSyndicateLevel(m_pInfo->ucLevel);
    g_objHero.SetSyndicateLeaderName(m_pInfo->szLeader);
    g_objHero.SetSyndicatePopulation(m_pInfo->dwSyndicatePopulation);
    if (m_pInfo->usMantle == 0)
    {
        g_objHero.SetMantle(0);
    }
    else
    {
        g_objHero.SetMantle(m_pInfo->usMantle + 999000);
    }
    if (m_pInfo->idSyndicate != ID_NONE)
    {
        g_objPlayerSet.SetSyndicateStatus(m_pInfo->idSyndicate, SYNDICATE_SELF);
    }
    ::PostCmd(CMD_FLASH_SYNDICATE);
}

//---------------------------------------------------------------------

