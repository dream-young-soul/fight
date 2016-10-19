
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgSynMemberInfo.cpp: implementation of the CMsgSynMemberInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "MsgSynMemberInfo.h"
#include "Hero.h"
#include "GameDataSet.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgSynMemberInfo::CMsgSynMemberInfo()
{
    Init();
    m_pInfo	= (MSG_Info* )m_bufMsg;
}

CMsgSynMemberInfo::~CMsgSynMemberInfo()
{
}

//---------------------------------------------------------------------

BOOL CMsgSynMemberInfo::Create(char* pMsgBuf, DWORD dwSize)
{
    if (!CNetMsg::Create(pMsgBuf, dwSize))
    {
        return false;
    }
    if (_MSG_SYNMEMBERINFO != this->GetType())
    {
        return false;
    }
    return true;
}
//---------------------------------------------------------------------
BOOL CMsgSynMemberInfo::Create(char* pszName)
{
    this->Init();
    // fill info now
    m_pInfo->unMsgSize	= sizeof(MSG_Info);
    m_pInfo->unMsgType	= _MSG_SYNMEMBERINFO;
    if (!pszName && strlen(pszName) >= _MAX_NAMESIZE)
    {
        return false;
    }
    strcpy(m_pInfo->szName, pszName);
    return true;
}
//---------------------------------------------------------------------
void CMsgSynMemberInfo::Process(void* pInfo)
{
    SynMemberInfo* pMyInfo = g_objHero.GetReturnSynMemberInfo();
    pMyInfo->ucLevel = m_pInfo->ucLevel;
    pMyInfo->nProffer = m_pInfo->nProffer;
    pMyInfo->ucRank = m_pInfo->ucRank;
    pMyInfo->ucProfession = m_pInfo->ucProfession;
    pMyInfo->ucNobilityRank = m_pInfo->ucNobilityRank;
    pMyInfo->sPk = m_pInfo->sPk;
    pMyInfo->dwLookFace = m_pInfo->dwLookFace;
    strcpy(pMyInfo->szMate, m_pInfo->szMate);
    int j;
    BOOL bFound = false;
    for(j = 0; j < RANKAMOUNT; j++)
    {
        if (SYNDICATERANKINFOSET[j].dwRank == m_pInfo->ucRank)
        {
            strcpy(pMyInfo->szRank, (char*)g_objGameDataSet.GetStr(SYNDICATERANKINFOSET[j].idTitle));
            bFound = true;
        }
    }
    // error ...
    if (!bFound)
    {
        for(j = 0; j < RANKAMOUNT; j++)
        {
            if (SYNDICATERANKINFOSET[j].dwRank == RANK_NONE)
            {
                strcpy(pMyInfo->szRank, (char*)g_objGameDataSet.GetStr(SYNDICATERANKINFOSET[j].idTitle));
            }
        }
    }
    ::PostCmd(CMD_OPEN_FLASH_SYN_MEMBER_INFO);
}
//---------------------------------------------------------------------
