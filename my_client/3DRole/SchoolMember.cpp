
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// SchoolMember.cpp: implementation of the CSchoolMember class.
//
//////////////////////////////////////////////////////////////////////

#include "SchoolMember.h"
#include "Hero.h"
#include "GamePlayerSet.h"
#include "GameDataSet.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSchoolMember::CSchoolMember()
{
    m_idMember = ID_NONE;
    strcpy(m_szName, "");
    m_ucRelation = 0;
}

CSchoolMember::~CSchoolMember()
{
}

/////////////////////////////////////////////////////////////////////

CSchoolMember* CSchoolMember::CreateNew(OBJID id, UCHAR ucStatus, char* pszName, UCHAR ucRelation, char* pszMate, UCHAR ucLevel, UCHAR ucProfession, USHORT usPkValue, UCHAR ucNobilityRank, DWORD dwSynID_Rank)
{
    CSchoolMember* pSchoolMember = new CSchoolMember;
    assert(pSchoolMember);
    //fill info
    pSchoolMember->SetID(id);
    pSchoolMember->SetStatus(ucStatus);
    pSchoolMember->SetName(pszName);
    pSchoolMember->SetRelation(ucRelation);
    pSchoolMember->SetMate(pszMate);
    pSchoolMember->SetLevel(ucLevel);
    pSchoolMember->SetNobilityRank(ucNobilityRank);
    pSchoolMember->SetProfession(ucProfession);
    pSchoolMember->SetPk(usPkValue);
    pSchoolMember->SetSynRank(dwSynID_Rank);
    return pSchoolMember;
}

char* CSchoolMember::GetProfession()
{
    return g_objHero.GetProfessionalName(m_ucProfession);
}

char* CSchoolMember::GetNobilityRank()
{
    return (char*)g_obj3DRoleData.GetNobilityTitle(m_ucNobilityRank);
}

char* CSchoolMember::GetSynName()
{
    OBJID idSyndicate = m_dwSynID_Rank & 0x00ffffff;
    return g_objPlayerSet.GetSyndicateName(idSyndicate);
}

char* CSchoolMember::GetSynRank()
{
    DWORD dwRank = m_dwSynID_Rank >> 24;
    int i;
    for(i = 0; i < RANKAMOUNT; i++)
    {
        if (SYNDICATERANKINFOSET[i].dwRank == dwRank)
        {
            return (char*)g_objGameDataSet.GetStr(SYNDICATERANKINFOSET[i].idTitle);
        }
    }
    // error ...
    for(i = 0; i < RANKAMOUNT; i++)
    {
        if (SYNDICATERANKINFOSET[i].dwRank == RANK_NONE)
        {
            return (char*)g_objGameDataSet.GetStr(SYNDICATERANKINFOSET[i].idTitle);
        }
    }
    return NULL;
}
