
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgSynAttrInfo.cpp: interface for the CMsgSynInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "Syndicate.h"
#include "MapGroup.h"
#pragma	warning(disable:4786)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgSynInfo::CMsgSynInfo()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgSynInfo::~CMsgSynInfo()
{
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgSynInfo::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_SYNINFO != this->GetType())
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgSynInfo::Create(CSyndicate* pSyn)		// pSyn may be null
{
    // init
    this->Init();
    // fill info now
    m_pInfo->idSyn			= pSyn->GetID();
    m_pInfo->idFealty		= pSyn->GetInt(SYNDATA_FEALTY);
    m_pInfo->dwSyndicateFund		= pSyn->GetInt(SYNDATA_MONEY);
    m_pInfo->dwSyndicatePopulation	= pSyn->GetSynAmount();
    m_pInfo->ucRank			= pSyn->GetInt(SYNDATA_RANK);
    m_pInfo->ucSaint		= pSyn->GetInt(SYNDATA_SAINT);
    SafeCopy(m_pInfo->szName, pSyn->GetStr(SYNDATA_NAME), _MAX_NAMESIZE);
    SafeCopy(m_pInfo->szLeader, pSyn->GetStr(SYNDATA_LEADERNAME), _MAX_NAMESIZE);
    m_unMsgType	= _MSG_SYNINFO;
    m_unMsgSize	= sizeof(MSG_Info);
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgSynInfo::Process(void* pInfo)
{
    ASSERT(!"CMsgSynInfo::Process()");
    return;
#ifdef _MSGDEBUG
    ::LogMsg("Process CMsgSynInfo, id:%u", m_info.id);
#endif
}
