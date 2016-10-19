
//**********************************************************
// 代码编辑器
//**********************************************************

//--------------------------------------------------------------------------
// heroteam.cpp
//--------------------------------------------------------------------------

#include "Hero.h"
#include "GamePlayerSet.h"
#include "Player.h"
#include "GameMsg.h"
#include "AllMsg.h"
#include "GameDataSet.h"

//--------------------------------------------------------------------------
void CHero::TeamCreate()
{
    // 看自己有没有队伍
    int nAmount = m_objTeam.GetMemberInfoAmount();
    if (nAmount != 0)
    {
        OBJID idHaveATeam = 100093;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idHaveATeam));
        return;
    }
    else
    {
        CMsgTeam msg;
        if (msg.Create(this->GetID(), _MSGTEAM_CREATE))
        {
            msg.Send();
        }
    }
}
//--------------------------------------------------------------------------
void CHero::TeamApplyJion(OBJID idTarget)
{
    // 看自己有没有队伍
    int nAmount = m_objTeam.GetMemberInfoAmount();
    if (nAmount != 0)
    {
        OBJID idLeaveTeamFirst = 100094;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idLeaveTeamFirst));
        return;
    }
    else
    {
        CMsgTeam msg;
        if (msg.Create(idTarget, _MSGTEAM_APPLYJOIN))
        {
            msg.Send();
        }
    }
}
//--------------------------------------------------------------------------
void CHero::TeamLeave()
{
    // 看自己有没有队伍
    int nAmount = m_objTeam.GetMemberInfoAmount();
    if (nAmount == 0)
    {
        const OBJID idTeamError = 100095;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idTeamError));
        return;
    }
    TeamMemberInfo* pInfo = m_objTeam.GetMemberInfoByIndex(0);
    if (pInfo->id == this->GetID())
    {
        const OBJID idTeamError = 100096;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idTeamError));
        return;
    }
    CMsgTeam msg;
    if (msg.Create(this->GetID(), _MSGTEAM_LEAVE))
    {
        msg.Send();
    }
}
//--------------------------------------------------------------------------
void CHero::TeamAcceptInvite()
{
    // 看看自己有没有队伍
    int nAmount = m_objTeam.GetMemberInfoAmount();
    if (nAmount != 0)
    {
        OBJID idHaveATeam = 100093;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idHaveATeam));
        return;
    }
    else
    {
        CMsgTeam msg;
        if (msg.Create(m_objTeam.GetLastApplyID(), _MSGTEAM_ACCEPTINVITE))
        {
            msg.Send();
        }
    }
}
//--------------------------------------------------------------------------



//--------------------------------------------------------------------------
void CHero::TeamInvite(OBJID idTarget)
{
    // 看自己有没有队伍
    int nAmount = m_objTeam.GetMemberInfoAmount();
    if (nAmount == 0)
    {
        const OBJID idTeamError = 100097;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idTeamError));
        return;
    }
    TeamMemberInfo* pInfo = m_objTeam.GetMemberInfoByIndex(0);
    if (pInfo->id != this->GetID())
    {
        const OBJID idTeamError = 100098;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idTeamError));
        return;
    }
    CMsgTeam msg;
    if (msg.Create(idTarget, _MSGTEAM_INVITE))
    {
        msg.Send();
    }
}
//--------------------------------------------------------------------------
void CHero::TeamKickOut(OBJID idTarget)
{
    // 看自己有没有队伍
    int nAmount = m_objTeam.GetMemberInfoAmount();
    if (nAmount == 0)
    {
        const OBJID idTeamError = 100097;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idTeamError));
        return;
    }
    TeamMemberInfo* pInfo = m_objTeam.GetMemberInfoByIndex(0);
    if (pInfo->id != this->GetID())
    {
        const OBJID idTeamError = 100098;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idTeamError));
        return;
    }
    CMsgTeam msg;
    if (msg.Create(idTarget, _MSGTEAM_KICKOUT))
    {
        msg.Send();
    }
}
//--------------------------------------------------------------------------
void CHero::TeamDismiss()
{
    // 看自己有没有队伍
    int nAmount = m_objTeam.GetMemberInfoAmount();
    if (nAmount == 0)
    {
        const OBJID idTeamError = 100097;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idTeamError));
        return;
    }
    TeamMemberInfo* pInfo = m_objTeam.GetMemberInfoByIndex(0);
    if (pInfo->id != this->GetID())
    {
        const OBJID idTeamError = 100098;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idTeamError));
        return;
    }
    CMsgTeam msg;
    if (msg.Create(this->GetID(), _MSGTEAM_DISMISS))
    {
        msg.Send();
    }
}
//--------------------------------------------------------------------------
void CHero::TeamClose()
{
    // 看自己有没有队伍
    int nAmount = m_objTeam.GetMemberInfoAmount();
    if (nAmount == 0)
    {
        const OBJID idTeamError = 100097;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idTeamError));
        return;
    }
    TeamMemberInfo* pInfo = m_objTeam.GetMemberInfoByIndex(0);
    if (pInfo->id != this->GetID())
    {
        const OBJID idTeamError = 100098;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idTeamError));
        return;
    }
    CMsgTeam msg;
    if (msg.Create(this->GetID(), _MSGTEAM_CLOSETEAM))
    {
        const OBJID idTeamClose = 100147;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idTeamClose));
        msg.Send();
    }
}
//--------------------------------------------------------------------------
void CHero::TeamOpen()
{
    // 看自己有没有队伍
    int nAmount = m_objTeam.GetMemberInfoAmount();
    if (nAmount == 0)
    {
        const OBJID idTeamError = 100097;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idTeamError));
        return;
    }
    TeamMemberInfo* pInfo = m_objTeam.GetMemberInfoByIndex(0);
    if (pInfo->id != this->GetID())
    {
        const OBJID idTeamError = 100098;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idTeamError));
        return;
    }
    CMsgTeam msg;
    if (msg.Create(this->GetID(), _MSGTEAM_OPENTEAM))
    {
        const OBJID idTeamOpen = 100148;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idTeamOpen));
        msg.Send();
    }
}
//--------------------------------------------------------------------------
void CHero::TeamAgreeApply()
{
    // 看自己有没有队伍
    int nAmount = m_objTeam.GetMemberInfoAmount();
    if (nAmount == 0)
    {
        const OBJID idTeamError = 100097;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idTeamError));
        return;
    }
    TeamMemberInfo* pInfo = m_objTeam.GetMemberInfoByIndex(0);
    if (pInfo->id != this->GetID())
    {
        const OBJID idTeamError = 100098;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idTeamError));
        return;
    }
    CMsgTeam msg;
    if (msg.Create(m_objTeam.GetLastApplyID(), _MSGTEAM_AGREEJOIN))
    {
        msg.Send();
    }
}
//--------------------------------------------------------------------------
void CHero::ClearTeamMember()
{
    m_objTeam.Destroy();
}
//--------------------------------------------------------------------------
void CHero::SetTeamMemberHp(OBJID idMember, int nHp)
{
    TeamMemberInfo* pInfo = m_objTeam.GetMemberInfo(idMember);
    if (pInfo)
    {
        pInfo->dwHp = nHp;
    }
}
//--------------------------------------------------------------------------
void CHero::SetTeamMemberMaxHp(OBJID idMember, int nMaxHp)
{
    TeamMemberInfo* pInfo = m_objTeam.GetMemberInfo(idMember);
    if (pInfo)
    {
        pInfo->dwMaxHp = nMaxHp;
    }
}
//--------------------------------------------------------------------------
int CHero::GetTeamMemberAmount()
{
    return m_objTeam.GetMemberInfoAmount();
}
//--------------------------------------------------------------------------
TeamMemberInfo* CHero::GetTeamMemberInfo(int nIndex)
{
    return m_objTeam.GetMemberInfoByIndex(nIndex);
}
//--------------------------------------------------------------------------
BOOL CHero::SetTeamLastApply(OBJID idPlayer)
{
    return m_objTeam.SetLastApplyID(idPlayer);
}
//--------------------------------------------------------------------------
void CHero::AddTeamMember(TeamMemberInfo* pInfo)
{
    if (pInfo)
    {
        m_objTeam.AddMember(pInfo);
    }
}
//--------------------------------------------------------------------------
void CHero::CloseTeam()
{
    m_objTeam.CloseTeam();
    const OBJID idMsg = 100147;
    g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idMsg));
}
//--------------------------------------------------------------------------
void CHero::OpenTeam()
{
    m_objTeam.OpenTeam();
    const OBJID idMsg = 100148;
    g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idMsg));
}
//--------------------------------------------------------------------------
void CHero::DelTeamMember(OBJID id)
{
    m_objTeam.DelMember(id);
}
//--------------------------------------------------------------------------
void CHero::ShowTeam(CMyPos posShow)
{
    m_objTeam.Show(posShow);
}
//--------------------------------------------------------------------------
char* CHero::GetTeamApplyerName()
{
    return m_objTeam.GetLastApplyName();
}
//--------------------------------------------------------------------------
BOOL CHero::IsTeamMember(char* pszPlayer)
{
    if (!pszPlayer)
    {
        return false;
    }
    int nAmount = m_objTeam.GetMemberInfoAmount();
    for(int i = 0; i < nAmount; i ++)
    {
        TeamMemberInfo* pInfo = m_objTeam.GetMemberInfoByIndex(i);
        if (pInfo && strcmp(pInfo->szName, pszPlayer) == 0)
        {
            return true;
        }
    }
    return false;
}
//--------------------------------------------------------------------------
BOOL CHero::IsTeamMember(OBJID idPlayer)
{
    if (m_objTeam.GetMemberInfo(idPlayer))
    {
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------
TeamMemberInfo* CHero::GetTeamMemberInfoByID(OBJID idMember)
{
    return m_objTeam.GetMemberInfo(idMember);
}
//--------------------------------------------------------------------------
BOOL CHero::IsTeamLeader()
{
    int nAmount = m_objTeam.GetMemberInfoAmount();
    if (nAmount <= 0)
    {
        return false;
    }
    TeamMemberInfo* pInfo = m_objTeam.GetMemberInfoByIndex(0);
    if (pInfo && (pInfo->id == this->GetID()))
    {
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------
void CHero::SetTeamMemberFace(OBJID idMember, int nFace)
{
    TeamMemberInfo* pInfo = m_objTeam.GetMemberInfo(idMember);
    if (pInfo)
    {
        pInfo->dwFace = nFace;
        ::PostCmd(CMD_FLASH_TEAM_FACE);
    }
}
//--------------------------------------------------------------------------
void CHero::SetTeamMoneyAccess(BOOL bAccess)
{
    if (!this->IsTeamLeader())
    {
        return;
    }
    int nAction;
    if (bAccess)
    {
        nAction = _MSGTEAM_OPENMONEYACCESS;
        const OBJID idMsg = 100149;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idMsg));
    }
    else
    {
        nAction = _MSGTEAM_CLOSEMONEYACCESS;
        const OBJID idMsg = 100150;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idMsg));
    }
    CMsgTeam msg;
    if (msg.Create(this->GetID(), nAction))
    {
        msg.Send();
    }
}
//--------------------------------------------------------------------------
void CHero::SetTeamItemAccess(BOOL bAccess)
{
    if (!this->IsTeamLeader())
    {
        return;
    }
    int nAction;
    if (bAccess)
    {
        nAction = _MSGTEAM_OPENITEMACCESS;
        const OBJID idMsg = 100151;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idMsg));
    }
    else
    {
        nAction = _MSGTEAM_CLOSEITEMACCESS;
        const OBJID idMsg = 100152;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idMsg));
    }
    CMsgTeam msg;
    if (msg.Create(this->GetID(), nAction))
    {
        msg.Send();
    }
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void CHero::SetTeamGemAccess(BOOL bAccess)
{
    if ( !this->IsTeamLeader() )
    {
        return ;
    }
    int nAction;
    if (bAccess)
    {
        nAction = _MSGTEAM_OPENGEMACCESS;
        const OBJID idMsg = 100158;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idMsg));
    }
    else
    {
        nAction = _MSGTEAM_CLOSEGEMACCESS;
        const OBJID idMsg = 100159;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idMsg));
    }
    CMsgTeam msg;
    if (msg.Create(this->GetID(), nAction))
    {
        msg.Send();
    }
}
//--------------------------------------------------------------------------
void CHero::QueryTeamMemberPos(OBJID idTarget)
{
    CMsgAction msg;
    if (msg.Create(this->GetID(), 0, 0, 0, actionQueryTeamMember, idTarget))
    {
        msg.Send();
        this->m_dwFlashTeamMemberID = idTarget;
    }
}
//--------------------------------------------------------------------------
OBJID CHero::GetTeamMemberFlashID()
{
    if (::TimeGet() - this->GetTeamLeaderFlashTime() > 3 * 1000)
    {
        m_dwFlashTeamMemberID;
    }
    return m_dwFlashTeamMemberID;
}
//--------------------------------------------------------------------------
