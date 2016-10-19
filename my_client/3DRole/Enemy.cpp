
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// Enemy.cpp: implementation of the CEnemy class.
//
//////////////////////////////////////////////////////////////////////

#include "Enemy.h"
#include "Hero.h"
#include "GamePlayerSet.h"
#include "GameDataSet.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEnemy::CEnemy()
{
    m_idEnemy	=	ID_NONE;
    strcpy(m_szName, "");
    m_bFlagOnline	=	false;
}

CEnemy::~CEnemy()
{
}

//////////////////////////////////////////////////////////////////////
CEnemy* CEnemy::CreateNew(OBJID id, char* pszName, DWORD dwLookFace, BOOL m_bFlagOnline)
{
    CEnemy* pEnemy = new(CEnemy);
    assert(pEnemy);
    // fill info
    pEnemy->SetID(id);
    if (pszName)
    {
        pEnemy->SetName(pszName);
    }
    if (m_bFlagOnline)
    {
        pEnemy->Online();
    }
    else
    {
        pEnemy->Offline();
    }
    pEnemy->SetLook(dwLookFace % 1000);
    pEnemy->SetFace((dwLookFace / 10000) % 1000);
    pEnemy->SetRace((dwLookFace % 1000 + 1) / 2);
    return pEnemy;
}
//---------------------------------------------------------------------
DWORD	CEnemy::GetProfessional()
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
char* CEnemy::GetProfessionalName()
{
    return g_objHero.GetProfessionalName(this->GetProfessional());
}
//---------------------------------------------------------------------
char* CEnemy::GetSyndicateName()
{
    return g_objPlayerSet.GetSyndicateName(m_IDSyndicate);
}
//---------------------------------------------------------------------
char* CEnemy::GetRankName()
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
