
//**********************************************************
// 代码编辑器
//**********************************************************

// Team.cpp: implementation of the CTeam class.
//
//////////////////////////////////////////////////////////////////////

#include "allmsg.h"
#include "Team.h"
#include "npcworld.h"
#include "Agent.h"
#include "Sense.h"

MYHEAP_IMPLEMENTATION(CTeam, s_heap)
const int _RANGE_EXPSHARE = 32;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTeam::CTeam()
{
}

CTeam::~CTeam()
{
}

//////////////////////////////////////////////////////////////////////
CTeam* CTeam::CreateNew()
{
    CTeam* pTeam	= new CTeam();
    return pTeam;
}

//////////////////////////////////////////////////////////////////////
BOOL CTeam::Create(CAgent* pAgent)
{
    this->Init();
    m_pOwner	= pAgent;
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CTeam::Apply(OBJID idUser)
{
    CSense* pSense = Cast<CSense>(m_pOwner);
    if (pSense)
    {
        pSense->TeamApply(idUser);
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CTeam::DelMember(OBJID idMember)
{
    // 从队伍里删除队员ID，不需要检验ID的有效性。
    for(int i = 0; i < m_setMember.size(); i++)
    {
        if (m_setMember[i].id == idMember)
        {
            m_setMember.erase(m_setMember.begin() + i);
            return true;
        }
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
TeamMemberInfo* CTeam::GetMemberByIndex(int nIndex)
{
    CHECKF(nIndex >= 0 && nIndex < m_setMember.size());
    return &m_setMember[nIndex];
}

//////////////////////////////////////////////////////////////////////
void CTeam::Destroy()
{
    this->Init();
}

//////////////////////////////////////////////////////////////////////
void CTeam::Init()
{
    m_setMember.clear();
}

//////////////////////////////////////////////////////////////////////
OBJID CTeam::GetLeaderID()
{
    CHECKF(m_setMember.size());
    return m_setMember[0].id;
}

//////////////////////////////////////////////////////////////////////
int CTeam::GetMemberAmount()
{
    return __min(_MAX_TEAMAMOUNT, m_setMember.size());
}

//////////////////////////////////////////////////////////////////////
void CTeam::Open()
{
    m_fClosed	= false;
}

//////////////////////////////////////////////////////////////////////
void CTeam::Close()
{
    m_fClosed	= true;
}

//////////////////////////////////////////////////////////////////////
BOOL CTeam::IsClosed()
{
    return m_fClosed;
}

//////////////////////////////////////////////////////////////////////
BOOL CTeam::IsTeamMember(OBJID idMember)
{
    for(int i = 0; i < GetMemberAmount(); i++)
    {
        if (GetMemberByIndex(i)->id == idMember)
        {
            return TRUE;
        }
    }
    return FALSE;
}

//////////////////////////////////////////////////////////////////////
bool CTeam::AddMember(int nAmount, const TeamMemberInfo* setInfo)
{
    CHECKF(nAmount <= _MAX_TEAMAMOUNT);
    for(int i = 0; i < nAmount; i++)
    {
        m_setMember.push_back(setInfo[i]);
    }
    return true;
}
