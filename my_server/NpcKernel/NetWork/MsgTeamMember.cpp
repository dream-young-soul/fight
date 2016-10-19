
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgTeamMember.cpp: implementation of the CMsgTeamMember class.
//
//////////////////////////////////////////////////////////////////////

#include "allmsg.h"
#include "team.h"
#include "npcWorld.h"
#include "Agent.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgTeamMember::CMsgTeamMember()
{
    this->Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgTeamMember::~CMsgTeamMember()
{
}

/*/////////////////////////////////////////////////////////////////////
BOOL CMsgTeamMember::Create(CUser* pMember)
{
	if (!pMember)
		return false;

	CTeam* pTeam	= pMember->GetTeam();
	if (!pTeam)
		return false;

	TeamMemberInfo info;
	memset(&info, 0L, sizeof(TeamMemberInfo));
	info.id			= pMember->GetID();
	info.dwLookFace	= pMember->GetLookFace();
	info.usHp		= pMember->GetLife();
	info.usMaxHp	= pMember->GetMaxLife();
	strcpy(info.szName, pMember->GetName());

	// init
	this->Init();

	// fill info now
	m_unMsgSize	= sizeof(MSG_Info) - sizeof(TeamMemberInfo)*(_MAX_TEAMMEMBER-1);
	m_unMsgType	= _MSG_TEAMMEMBER;

	m_pInfo->ucAction	= _MSG_TEAMMEMBER_ADDMEMBER;
	m_pInfo->ucAmount	= 1;
	memcpy(m_pInfo->setMember, &info, sizeof(TeamMemberInfo));
	return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgTeamMember::Create(int nAction, const TeamMemberInfo& setInfo)
{
	// init
	this->Init();

	// fill info now
	m_unMsgSize	=sizeof(MSG_Info);
	m_unMsgType	=_MSG_TEAMMEMBER;

	m_pInfo->ucAction	= nAction;
	m_pInfo->ucAmount	= 1;
	memcpy(m_pInfo->setMember, &setInfo, sizeof(TeamMemberInfo));
	return true;
}
*/
//////////////////////////////////////////////////////////////////////
BOOL CMsgTeamMember::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!pbufMsg)
    {
        return false;
    }
    memcpy(this->m_bufMsg, pbufMsg, dwMsgSize);
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgTeamMember::Process(void* pInfo)
{
    CAgent* pAgent = NpcManager()->QueryAgent(GetNpcID());
    if (!pAgent)
    {
        return ;
    }
    Cast<CTeam>(pAgent)->AddMember(m_pInfo->ucAmount, m_pInfo->setMember);
}
