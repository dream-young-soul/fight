
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// Friend.cpp: implementation of the CFriend class.
//
//////////////////////////////////////////////////////////////////////

#include "Friend.h"
#include "Hero.h"
#include "GamePlayerSet.h"
#include "GameDataSet.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFriend::CFriend()
{
    m_idFriend	=	ID_NONE;
    strcpy(m_szName, "");
    m_bFlagOnline	=	false;
    m_bFlagNewMsg = false;
    strcpy(m_szMate, "");
}

CFriend::~CFriend()
{
}
//////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------
DWORD	CFriend::GetProfessional()
{
    //	switch(m_dwLook)
    //	{
    //	case 1:
    //	case 2:
    //		return CPlayer::PROFSORT_SOLDIER*10;
    //	case 3:
    //	case 4:
    //		return CPlayer::PROFSORT_ARCHER*10;
    //	case 5:
    //	case 6:
    //		return CPlayer::PROFSORT_MAGICIAN*10;
    //	}
    //	return CPlayer::PROFSORT_SOLDIER*10;
    return m_dwProfessional;
}
//---------------------------------------------------------------------
CFriend* CFriend::CreateNew(OBJID id, char* pszName, DWORD dwLookFace, BOOL m_bFlagOnline)
{
    CFriend* pFriend = new(CFriend);
    assert(pFriend);
    // fill info
    pFriend->SetID(id);
    if (pszName)
    {
        pFriend->SetName(pszName);
    }
    if (m_bFlagOnline)
    {
        pFriend->Online();
    }
    else
    {
        pFriend->Offline();
    }
    pFriend->SetLook(dwLookFace % 1000);
    pFriend->SetFace((dwLookFace / 10000) % 1000);
    pFriend->SetRace((dwLookFace % 1000 + 1) / 2);
    return pFriend;
}
//---------------------------------------------------------------------
char* CFriend::GetProfessionalName()
{
    return g_objHero.GetProfessionalName(this->GetProfessional());
}
//---------------------------------------------------------------------
char* CFriend::GetSyndicateName()
{
    return g_objPlayerSet.GetSyndicateName(m_IDSyndicate);
}
//---------------------------------------------------------------------
char* CFriend::GetRankName()
{
    for(int i = 0; i < RANKAMOUNT; i++)
    {
        InfoSyndicateRank infoRank = SYNDICATERANKINFOSET[i];
        if (infoRank.dwRank == m_dwRank)
        {
            return (char*)g_objGameDataSet.GetStr(infoRank.idTitle);
        }
    }
    return (char*)g_objGameDataSet.GetStr(SYNDICATERANKINFOSET[RANKAMOUNT-1].idTitle);
}
//---------------------------------------------------------------------
DWORD	CFriend::GetRace()
{
    return m_dwRace;
}
//---------------------------------------------------------------------
DWORD	CFriend::GetLook()
{
    return m_dwLook;
}
//---------------------------------------------------------------------
