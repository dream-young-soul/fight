
//**********************************************************
// 代码编辑器
//**********************************************************

//
//
// UserTeam.cpp: implementation of the CUser class.
//
//////////////////////////////////////////////////////////////////////


#include "AllMsg.h"
#include "User.h"
#include "MapGroup.h"
#include "netmsg.h"

//////////////////////////////////////////////////////////////////////
BOOL CUser::CreateTeam()
{
    // 判断玩家是否已经有队伍了。
    if (this->GetTeam())
    {
        this->SendSysMsg(STR_TEAMMATE_CANNOT_CREATE);
        return false;
    }
    CTeam* pTeam	= CTeam::CreateNew(m_idProcess, UserManager()->SpawnNewTeamID(), this->GetID());
    if (!pTeam)
    {
        this->SendSysMsg(STR_CREATE_TEAM_FAILED);
        return false;
    }
    this->SetTeam(pTeam);
    return true;
}

//////////////////////////////////////////////////////////////////////
CTeam* CUser::GetTeam()
{
    if (!m_pTeam)
    {
        return NULL;
    }
    return m_pTeam;
}

//////////////////////////////////////////////////////////////////////
BOOL CUser::AcceptTeamMember(CUser* pMember)
{
    if (!pMember)
    {
        return false;
    }
    if (m_pTeam->AddMember(pMember->GetID()))
    {
        this->SendShow(pMember);
        pMember->SendSysMsg(_TXTATR_TEAM, STR_TEAM_MONEY_s, m_pTeam->IsCloseMoney() ? STR_CLOSE : STR_OPEN);
        pMember->SendSysMsg(_TXTATR_TEAM, STR_TEAM_ITEM_s, m_pTeam->IsCloseItem() ? STR_CLOSE : STR_OPEN);
        pMember->SendSysMsg(_TXTATR_TEAM, STR_TEAM_GEM_s, m_pTeam->IsCloseGemAccess() ? STR_CLOSE : STR_OPEN);
        return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
BOOL CUser::AcceptTeamInvit(CUser* pLeader)
{
    if (!pLeader)
    {
        return false;
    }
    CTeam* pTeam	= pLeader->GetTeam();
    if (pTeam->AddMember(this->GetID()))
    {
        pLeader->SendShow(this);
        this->SendSysMsg(_TXTATR_TEAM, STR_TEAM_MONEY_s, pTeam->IsCloseMoney() ? STR_CLOSE : STR_OPEN);
        this->SendSysMsg(_TXTATR_TEAM, STR_TEAM_ITEM_s, pTeam->IsCloseItem() ? STR_CLOSE : STR_OPEN);
        this->SendSysMsg(_TXTATR_TEAM, STR_TEAM_GEM_s, pTeam->IsCloseGemAccess() ? STR_CLOSE : STR_OPEN);
        return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
BOOL CUser::QuitTeam()
{
    if (!m_pTeam)
    {
        return false;
    }
    if (this->GetID() == m_pTeam->GetLeader())
    {
        // 队长，解散队伍
        return this->DismissTeam();
    }
    CMsgTeam msg;
    if (msg.Create(this->GetID(), _MSGTEAM_LEAVE))
    {
        m_pTeam->BroadcastTeamMsg(&msg);
    }
    // 普通队员退出队伍。
    m_pTeam->DelMember(this->GetID());
    ClrTeam();
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CUser::KickOut(OBJID idMember)
{
    if (!m_pTeam || idMember == ID_NONE)
    {
        return false;
    }
    CMsgTeam msg;
    if (msg.Create(idMember, _MSGTEAM_KICKOUT))
    {
        m_pTeam->BroadcastTeamMsg(&msg);
    }
    m_pTeam->DelMember(idMember);
    CUser* pMember = UserManager()->GetUser(idMember);
    if (pMember)
    {
        pMember->ClrTeam();    //? 可能在远程，CUser::DelTeamMember()中会删除
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CUser::DismissTeam()
{
    if (!m_pTeam)
    {
        return false;
    }
    CMsgTeam msg;
    if (msg.Create(this->GetID(), _MSGTEAM_DISMISS))
    {
        m_pTeam->BroadcastTeamMsg(&msg);
    }
    this->ClsStatus(STATUS_TEAMLEADER);
    m_pTeam->Dismiss(this->GetID());
    this->ClrTeam();
    return true;
}

//////////////////////////////////////////////////////////////////////
void CUser::SetTeam(CTeam* pTeam)
{
    ASSERT(m_pTeam == 0 && pTeam);
    m_pTeam	= pTeam;
}

//////////////////////////////////////////////////////////////////////
void CUser::ClrTeam()
{
    if (m_pTeam)
    {
        m_pTeam->ReleaseByOwner();
    }
    m_pTeam = NULL;
}

//////////////////////////////////////////////////////////////////////
void CUser::ProcessTeamTimer()
{
    CTeam* pTeam = this->GetTeam();
    if (!pTeam)
    {
        return;
    }
    OBJID idLeader = pTeam->GetLeader();
    if (this->GetID() != idLeader)
    {
        return;
    }
    int nPosX = this->GetPosX();
    int nPosY = this->GetPosY();
    CMsgAction msg;
    IF_OK (msg.Create(this->GetID(), nPosX, nPosY, 0, actionTeamMemeberPos, this->GetMapID()))
    {
        int nTeamMembers = pTeam->GetMemberAmount();
        for (int i = 1; i < nTeamMembers; i++)
        {
            OBJID idUser = pTeam->GetMemberByIndex(i);
            CUser* pUser = UserManager()->GetUser(idUser);
            if (pUser && pUser->GetMapID() == this->GetMapID())
            {
                pUser->SendMsg(&msg);
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////
void CUser::SendShow(CUser* pTarget)
{
    CHECK(GetTeam());
    // msg
    TeamMemberInfo	info;
    info.id			= pTarget->GetID();
    info.dwLookFace	= pTarget->GetLookFace();
    info.usHp		= pTarget->GetLife();
    info.usMaxHp	= pTarget->GetMaxLife();
    strcpy(info.szName, pTarget->GetName());
    for(int i = 0; i < GetTeam()->GetMemberAmount(); i++)
    {
        OBJID idMember = GetTeam()->GetMemberByIndex(i);
        CUser* pMember = UserManager()->GetUser(idMember);
        if (pMember)
        {
            info.id			= pMember->GetID();
            info.dwLookFace	= pMember->GetLookFace();
            info.usHp		= pMember->GetLife();
            info.usMaxHp	= pMember->GetMaxLife();
            strcpy(info.szName, pMember->GetName());
            CMsgTeamMember msg;
            IF_OK(msg.Create(_MSG_TEAMMEMBER_ADDMEMBER, info))
            pTarget->SendMsg(&msg);
        }
        else // transmit
        {
            MapGroup(PID)->QueryIntraMsg()->ChangeTeam(CHANGETEAM_INFO, GetTeam()->GetID(), pTarget->GetID(), pTarget->GetSocketID());
        }
    }
    CMsgTeamMember msg;
    if (msg.Create(_MSG_TEAMMEMBER_ADDMEMBER, info))
    {
        GetTeam()->BroadcastTeamMsg(&msg);
    }
}

//////////////////////////////////////////////////////////////////////
// IChangeTeam
//////////////////////////////////////////////////////////////////////
bool CUser::AddTeamMember(OBJID idUser)
{
    CHECKF(GetTeam());
    GetTeam()->AddMemberOnly(idUser);
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::DelTeamMember(OBJID idUser)
{
    CHECKF(GetTeam());
    if (idUser == GetID())
    {
        this->ClrTeam();
    }
    else
    {
        GetTeam()->DelMemberOnly(idUser);
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::DelTeam()
{
    this->ClrTeam();
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::SendInfo(SOCKET_ID idSocket, OBJID idAgent)
{
    CHECKF(GetTeam());
    // msg
    TeamMemberInfo	info;
    info.id			= this->GetID();
    info.dwLookFace	= this->GetLookFace();
    info.usHp		= this->GetLife();
    info.usMaxHp	= this->GetMaxLife();
    strcpy(info.szName, this->GetName());
    CMsgTeamMember msg;
    if (msg.Create(_MSG_TEAMMEMBER_ADDMEMBER, info))
    {
        if (idSocket == SOCKET_NONE)
        {
            MapGroup(PID)->QueryIntraMsg()->SendNpcMsg(idAgent, &msg);
        }
        else
        {
            MapGroup(PID)->QueryIntraMsg()->SendClientMsg(idSocket, &msg);
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
void CUser::AppendTeam(TeamInfoStruct* pInfo)
{
    m_pTeam = CTeam::CreateNew(m_idProcess, pInfo);
    CHECK(m_pTeam);
}





