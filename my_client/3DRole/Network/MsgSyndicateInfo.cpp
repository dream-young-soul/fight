
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgSyndicateInfo.cpp: implementation of the CMsgSyndicateInfo class.
//
//////////////////////////////////////////////////////////////////////
#include "hero.h"
#include "MsgSyndicateInfo.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgSyndicateInfo::CMsgSyndicateInfo()
{
    Init();
    m_pInfo	= (MSG_Info* )m_bufMsg;
}

CMsgSyndicateInfo::~CMsgSyndicateInfo()
{
}

BOOL	CMsgSyndicateInfo::Create(char* pMsgBuf, DWORD dwSize)
{
    if (!CNetMsg::Create(pMsgBuf, dwSize))
    {
        return false;
    }
    if (_MSG_SYNINFO != this->GetType())
    {
        return false;
    }
    return true;
}

BOOL    CMsgSyndicateInfo::Create(OBJID idSyn)
{
    this->Init();
    // fill info now
    m_pInfo->unMsgSize	= sizeof(MSG_Info);
    m_pInfo->unMsgType	= _MSG_SYNINFO;
    if (idSyn == 0)
    {
        return false;
    }
    m_pInfo->idSyn = idSyn;
    return true;
}

void	CMsgSyndicateInfo::Process(void* pInfo)
{
    SyndicateInfo* pSynInfo = g_objHero.GetReturnSyndicateInfo();
    if (pSynInfo )
    {
        pSynInfo->dwSyndicateFund = m_pInfo->dwSyndicateFund;
        pSynInfo->dwSyndicatePopulation = m_pInfo->dwSyndicatePopulation;
        pSynInfo->idSyn = m_pInfo->idSyn;
        pSynInfo->idFealty = m_pInfo->idFealty;
        strcpy(pSynInfo->szLeader , m_pInfo->szLeader);
        strcpy(pSynInfo->szName, m_pInfo->szName);
        pSynInfo->ucRank = m_pInfo->ucRank;
        pSynInfo->ucSaint = pSynInfo->ucSaint;
        PostCmd(CMD_OPEN_FLASH_SYNINFO);
    }
}
