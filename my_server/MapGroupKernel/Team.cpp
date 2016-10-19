
//**********************************************************
// 代码编辑器
//**********************************************************

// Team.cpp: implementation of the CTeam class.
//
//////////////////////////////////////////////////////////////////////

#include "allmsg.h"
#include "Team.h"
#include "mapgroup.h"

#define GUIDING_TIME  60*60		//一个小时


MYHEAP_IMPLEMENTATION(CTeam, s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTeam::CTeam(PROCESS_ID idProcess)
{
    m_idProcess	= idProcess;
    this->m_tGuidingTime.Startup(GUIDING_TIME);
}

CTeam::~CTeam()
{
}

//////////////////////////////////////////////////////////////////////
CTeam* CTeam::CreateNew(PROCESS_ID idProcess, OBJID m_idTeam, OBJID idLeader)
{
    CTeam* pTeam	= new CTeam(idProcess);
    if (!pTeam)
    {
        return NULL;
    }
    if (!pTeam->Create(idLeader))
    {
        delete pTeam;
        return NULL;
    }
    pTeam->m_id = m_idTeam;
    return pTeam;
}

//////////////////////////////////////////////////////////////////////
CTeam* CTeam::CreateNew(PROCESS_ID idProcess, TeamInfoStruct* pInfo)
{
    CTeam* pTeam	= new CTeam(idProcess);
    IF_NOT(pTeam)
    return NULL;
    IF_NOT(pTeam->Create(pInfo->idLeader))
    {
        delete pTeam;
        return NULL;
    }
    pTeam->m_id = pInfo->id;
    for(int i = 0; i < _MAX_TEAMAMOUNT; i++)
    {
        if (pInfo->setMember[i] != ID_NONE && pInfo->setMember[i] != pInfo->idLeader)
        {
            pTeam->AddMemberOnly(pInfo->setMember[i]);
        }
    }
    pTeam->m_bCloseMoney	= pInfo->bCloseMoney;
    pTeam->m_bCloseItem		= pInfo->bCloseItem;
    return pTeam;
}

//////////////////////////////////////////////////////////////////////
BOOL CTeam::Create(OBJID idLeader)
{
    this->Init();
    m_idLeader	= idLeader;
    this->AddMemberOnly(idLeader);
    CUser* pPlayer	= UserManager()->GetUser(idLeader);
    if (pPlayer)			// may be not current mapgroup
    {
        pPlayer->SetStatus(STATUS_TEAMLEADER);
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CTeam::AddMember(OBJID idMember)
{
    if (m_fClosed)
    {
        return false;
    }
    int nOldAmount = this->GetMemberAmount();
    if (nOldAmount >= _MAX_TEAMAMOUNT)
    {
        return false;
    }
    if (this->IsTeamMember(idMember))
    {
        return false;
    }
    CUser* pMember = UserManager()->GetUser(idMember);
    if (!pMember)
    {
        return false;
    }
    int nSize = m_setIdUser.size();
    for(int i = 0; i < nSize; i++)
    {
        CUser* pPlayer = UserManager()->GetUser(m_setIdUser[i]);
        if (pPlayer)
        {
            IF_OK(pPlayer->GetTeam())
            pPlayer->GetTeam()->AddMemberOnly(idMember);
        }
        else
        {
            MapGroup(PID)->QueryIntraMsg()->ChangeTeam(CHANGETEAM_ADD, GetID(), idMember);
        }
    }
    int nNewAmount = this->GetMemberAmount();
    if (nOldAmount != nNewAmount
            && (nOldAmount >= TEAM_STATUS_REQ_ROLES || nNewAmount >= TEAM_STATUS_REQ_ROLES))
    {
        AdjustMemberStatus(nOldAmount, nNewAmount);
    }
    pMember->SetTeam(this->CloneNew());				//? 只会在本地
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CTeam::DelMember(OBJID idMember)
{
    // 从队伍里删除队员ID，不需要检验ID的有效性。
    int nOldAmount = this->GetMemberAmount();
    int nSize = nOldAmount;	//this->GetMemberAmount();
    for (int i = nSize - 1; i >= 0; i--)
    {
        // 倒序，删除
        CUser* pPlayer	= UserManager()->GetUser(m_setIdUser[i]);
        if (pPlayer)
        {
            IF_OK(pPlayer->GetTeam())
            {
                pPlayer->GetTeam()->DelMemberOnly(idMember);
            }
        }
        else
        {
            MapGroup(PID)->QueryIntraMsg()->ChangeTeam(CHANGETEAM_DEL, GetID(), idMember);
        }
    }
    int nNewAmount = this->GetMemberAmount();
    if (nOldAmount != nNewAmount
            && (nOldAmount >= TEAM_STATUS_REQ_ROLES || nNewAmount >= TEAM_STATUS_REQ_ROLES))
    {
        AdjustMemberStatus(nOldAmount, nNewAmount);
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
void CTeam::Dismiss(OBJID idLeader)
{
    CUser* pLeader = UserManager()->GetUser(idLeader);
    if (pLeader)
    {
        for (int i = STATUS_TEAM_BEGIN; i <= STATUS_TEAM_END; i++)
        {
            if (pLeader->QueryStatus(i))
            {
                CMsgTalk msg;
                if (msg.Create(SYSTEM_NAME, ALLUSERS_NAME, STR_TEAM_STATUS_DISAPPEAR, NULL, 0xff0000, _TXTATR_SYSTEM))
                {
                    this->BroadcastTeamMsg(&msg);
                }
                break;
            }
        }
    }
    int nSize = this->GetMemberAmount();
    for (int i = nSize - 1; i >= 0; i--)
    {
        // 倒序，删除
        CUser* pPlayer	= UserManager()->GetUser(m_setIdUser[i]);
        if (pPlayer)
        {
            IF_OK(pPlayer->GetTeam())
            {
                if (pPlayer->GetID() != idLeader)	// owner在外面删除
                {
                    pPlayer->ClrTeam();    //? 可能在远程，CUser::DelTeamMember()中会删除
                }
            }
        }
        else
        {
            MapGroup(PID)->QueryIntraMsg()->ChangeTeam(CHANGETEAM_DISMISS, GetID(), idLeader);
        }
    }
}

//////////////////////////////////////////////////////////////////////
OBJID CTeam::GetMemberByIndex(int nIndex)
{
    if (nIndex >= m_setIdUser.size() || nIndex < 0)
    {
        return ID_NONE;
    }
    return m_setIdUser[nIndex];
}

//////////////////////////////////////////////////////////////////////
void CTeam::Destroy()
{
    this->Init();
}

//////////////////////////////////////////////////////////////////////
void CTeam::Init()
{
    m_setIdUser.clear();
    m_fClosed			= false;
    m_bCloseMoney		= false;
    m_bCloseItem		= true;
    m_bCloseGemAccess	= true;
    m_idLeader			= ID_NONE;
}

//////////////////////////////////////////////////////////////////////
void CTeam::SetLeader(OBJID idLeader)
{
    if (m_idLeader != ID_NONE)	// 已经设置过队长了。
    {
        return;
    }
    CUser* pPlayer	= UserManager()->GetUser(idLeader);
    if (!pPlayer)
    {
        return;
    }
    m_idLeader	= idLeader;
}

//////////////////////////////////////////////////////////////////////
OBJID CTeam::GetLeader()
{
    return m_idLeader;
}

//////////////////////////////////////////////////////////////////////
int CTeam::GetMemberAmount()
{
    return __min(_MAX_TEAMAMOUNT, m_setIdUser.size());
}

//////////////////////////////////////////////////////////////////////
void CTeam::BroadcastTeamMsg(CNetMsg* pMsg, CUser* pSender/*=NULL*/)
{
    if (!pMsg)
    {
        return;
    }
    for (int i = 0; i < this->GetMemberAmount(); i++)
    {
        CUser* pUser	= NULL;
        OBJID idUser	= this->GetMemberByIndex(i);
        pUser			= UserManager()->GetUser(idUser);
        if (pUser)
        {
            if (pUser != pSender)
            {
                pUser->SendMsg(pMsg);
            }
        }
        else // 跨进程组
        {
            MapGroup(PID)->QueryIntraMsg()->TransmitWorldMsg(pMsg, idUser);
        }
    }
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
void CTeam::OpenMoney()
{
    m_bCloseMoney	= false;
    char szBuf[1024];
    sprintf(szBuf, STR_TEAM_MONEY_s, STR_OPEN);
    for (int i = 0; i < this->GetMemberAmount(); i++)
    {
        OBJID idTarget = this->GetMemberByIndex(i);
        CUser* pTarget = UserManager()->GetUser(idTarget);
        if (pTarget)			//? may be another mapgroup
        {
            CTeam* pTeam = pTarget->GetTeam();
            IF_OK(pTeam)
            {
                pTeam->SetCloseMoney(false);
                pTarget->SendSysMsg(_TXTATR_TEAM, szBuf);
            }
        }
        else
        {
            // remote call
            char	buf[1024];
            REMOTE_CALL0* pInfo = (REMOTE_CALL0*)&buf;
            pInfo->idUser		= idTarget;
            pInfo->ucFuncType	= REMOTE_CALL_TEAM_CLOSE_MONEY;
            pInfo->IntParam[0]	= false;
            pInfo->nSize		= sizeof(REMOTE_CALL0) + sizeof(int);
            MapGroup(PID)->QueryIntraMsg()->RemoteCall(pInfo);
        }
    }
}

//////////////////////////////////////////////////////////////////////
void CTeam::CloseMoney()
{
    m_bCloseMoney	= true;
    char szBuf[1024];
    sprintf(szBuf, STR_TEAM_MONEY_s, STR_CLOSE);
    for (int i = 0; i < this->GetMemberAmount(); i++)
    {
        OBJID idTarget = this->GetMemberByIndex(i);
        CUser* pTarget = UserManager()->GetUser(idTarget);
        if (pTarget)			//? may be another mapgroup
        {
            CTeam* pTeam = pTarget->GetTeam();
            IF_OK(pTeam)
            {
                pTeam->SetCloseMoney(true);
                pTarget->SendSysMsg(_TXTATR_TEAM, szBuf);
            }
        }
        else
        {
            // remote call
            char	buf[1024];
            REMOTE_CALL0* pInfo = (REMOTE_CALL0*)&buf;
            pInfo->idUser		= idTarget;
            pInfo->ucFuncType	= REMOTE_CALL_TEAM_CLOSE_MONEY;
            pInfo->IntParam[0]	= true;
            pInfo->nSize		= sizeof(REMOTE_CALL0) + sizeof(int);
            MapGroup(PID)->QueryIntraMsg()->RemoteCall(pInfo);
        }
    }
}

//////////////////////////////////////////////////////////////////////
void CTeam::OpenItem()
{
    m_bCloseItem	= false;
    char szBuf[1024];
    sprintf(szBuf, STR_TEAM_ITEM_s, STR_OPEN);
    for (int i = 0; i < this->GetMemberAmount(); i++)
    {
        OBJID idTarget = this->GetMemberByIndex(i);
        CUser* pTarget = UserManager()->GetUser(idTarget);
        if (pTarget)			//? may be another mapgroup
        {
            CTeam* pTeam = pTarget->GetTeam();
            IF_OK(pTeam)
            {
                pTeam->SetCloseItem(false);
                pTarget->SendSysMsg(_TXTATR_TEAM, szBuf);
            }
        }
        else
        {
            // remote call
            char	buf[1024];
            REMOTE_CALL0* pInfo = (REMOTE_CALL0*)&buf;
            pInfo->idUser		= idTarget;
            pInfo->ucFuncType	= REMOTE_CALL_TEAM_CLOSE_ITEM;
            pInfo->IntParam[0]	= false;
            pInfo->nSize		= sizeof(REMOTE_CALL0) + sizeof(int);
            MapGroup(PID)->QueryIntraMsg()->RemoteCall(pInfo);
        }
    }
}

//////////////////////////////////////////////////////////////////////
void CTeam::CloseItem()
{
    m_bCloseItem	= true;
    char szBuf[1024];
    sprintf(szBuf, STR_TEAM_ITEM_s, STR_CLOSE);
    for (int i = 0; i < this->GetMemberAmount(); i++)
    {
        OBJID idTarget = this->GetMemberByIndex(i);
        CUser* pTarget = UserManager()->GetUser(idTarget);
        if (pTarget)			//? may be another mapgroup
        {
            CTeam* pTeam = pTarget->GetTeam();
            IF_OK(pTeam)
            {
                pTeam->SetCloseItem(true);
                pTarget->SendSysMsg(_TXTATR_TEAM, szBuf);
            }
        }
        else
        {
            // remote call
            char	buf[1024];
            REMOTE_CALL0* pInfo = (REMOTE_CALL0*)&buf;
            pInfo->idUser		= idTarget;
            pInfo->ucFuncType	= REMOTE_CALL_TEAM_CLOSE_ITEM;
            pInfo->IntParam[0]	= true;
            pInfo->nSize		= sizeof(REMOTE_CALL0) + sizeof(int);
            MapGroup(PID)->QueryIntraMsg()->RemoteCall(pInfo);
        }
    }
}

//////////////////////////////////////////////////////////////////////
void CTeam::OpenGemAccess()
{
    m_bCloseGemAccess	= false;
    char szBuf[1024];
    sprintf(szBuf, STR_TEAM_GEM_s, STR_OPEN);
    for (int i = 0; i < this->GetMemberAmount(); i++)
    {
        OBJID idTarget = this->GetMemberByIndex(i);
        CUser* pTarget = UserManager()->GetUser(idTarget);
        if (pTarget)			//? may be another mapgroup
        {
            CTeam* pTeam = pTarget->GetTeam();
            IF_OK(pTeam)
            {
                pTeam->SetCloseGemAccess(false);
                pTarget->SendSysMsg(_TXTATR_TEAM, szBuf);
            }
        }
        else
        {
            // remote call
            char	buf[1024];
            REMOTE_CALL0* pInfo = (REMOTE_CALL0*)&buf;
            pInfo->idUser		= idTarget;
            pInfo->ucFuncType	= REMOTE_CALL_TEAM_CLOSE_GEM_ACCESS;
            pInfo->IntParam[0]	= false;
            pInfo->nSize		= sizeof(REMOTE_CALL0) + sizeof(int);
            MapGroup(PID)->QueryIntraMsg()->RemoteCall(pInfo);
        }
    }
}

//////////////////////////////////////////////////////////////////////
void CTeam::CloseGemAccess()
{
    m_bCloseGemAccess	= true;
    char szBuf[1024];
    sprintf(szBuf, STR_TEAM_GEM_s, STR_CLOSE);
    for (int i = 0; i < this->GetMemberAmount(); i++)
    {
        OBJID idTarget = this->GetMemberByIndex(i);
        CUser* pTarget = UserManager()->GetUser(idTarget);
        if (pTarget)			//? may be another mapgroup
        {
            CTeam* pTeam = pTarget->GetTeam();
            IF_OK(pTeam)
            {
                pTeam->SetCloseGemAccess(true);
                pTarget->SendSysMsg(_TXTATR_TEAM, szBuf);
            }
        }
        else
        {
            // remote call
            char	buf[1024];
            REMOTE_CALL0* pInfo = (REMOTE_CALL0*)&buf;
            pInfo->idUser		= idTarget;
            pInfo->ucFuncType	= REMOTE_CALL_TEAM_CLOSE_GEM_ACCESS;
            pInfo->IntParam[0]	= true;
            pInfo->nSize		= sizeof(REMOTE_CALL0) + sizeof(int);
            MapGroup(PID)->QueryIntraMsg()->RemoteCall(pInfo);
        }
    }
}

//////////////////////////////////////////////////////////////////////
BOOL CTeam::IsTeamWithNewbie(OBJID idKiller, IRole* pTarget)
{
    CHECKF (pTarget);
    BOOL bNewbieBonus = false;
    int nMonsterLev = pTarget->GetLev();
    for (int i = 0; i < this->GetMemberAmount(); i++)
    {
        CUser* pUser = UserManager()->GetUser(this->GetMemberByIndex(i));
        if (pUser)			//? may be another mapgroup
        {
            if (!pUser->IsAlive())
            {
                continue;
            }
            if (pUser->GetMapID() != pTarget->GetMap()->GetID())
            {
                continue;
            }
            if (abs(pUser->GetPosX() - pTarget->GetPosX()) > _RANGE_EXPSHARE ||
                    abs(pUser->GetPosY() - pTarget->GetPosY()) > _RANGE_EXPSHARE)
            {
                continue;    // out of share range
            }
            if (pUser->GetLev() + 20 < nMonsterLev)
            {
                bNewbieBonus = true;
                break;
            }
        }
    }
    return bNewbieBonus;
}

BOOL CTeam::IsTeamWithTutor(OBJID idKiller, IRole* pTarget)
{
    CHECKF (pTarget);
    bool bWithTutor = false;
    for (int i = 0; i < this->GetMemberAmount(); i++)
    {
        CUser* pUser = UserManager()->GetUser(this->GetMemberByIndex(i));
        if (pUser)			//? may be another mapgroup
        {
            if (!pUser->IsAlive())
            {
                continue;
            }
            if (pUser->GetMapID() != pTarget->GetMap()->GetID())
            {
                continue;
            }
            if (abs(pUser->GetPosX() - pTarget->GetPosX()) > _RANGE_EXPSHARE ||
                    abs(pUser->GetPosY() - pTarget->GetPosY()) > _RANGE_EXPSHARE)
            {
                continue;    // out of share range
            }
            if (pUser->GetStudent(idKiller))
            {
                bWithTutor = true;
                break;
            }
        }
    }
    return bWithTutor;
}

//////////////////////////////////////////////////////////////////////
void CTeam::AwardMemberExp(OBJID idKiller, IRole* pTarget, int nExp)
{
    // 1. idKiller代表杀死怪物的玩家ID，这个玩家不加组队经验了。
    // 2. nAddExp代表杀死怪物玩家获取的奖励经验
    CHECK (pTarget);
    CUser*	pKiller	= UserManager()->GetUser(idKiller);
    IF_NOT (pKiller)
    return;
    int nMonsterLev = pTarget->GetLev();
    for (int i = 0; i < this->GetMemberAmount(); i++)
    {
        OBJID idUser	= this->GetMemberByIndex(i);
        CUser* pUser	= UserManager()->GetUser(idUser);
        if (!pUser)				//? may be in another mapgroup.
        {
            continue;
        }
        // map, no self
        if (pUser->GetMapID() != pTarget->GetMap()->GetID() || pUser->GetID() == idKiller)
        {
            continue;
        }
        // chk all eudemons exclude killer's
        for (int i = 0; i < pUser->GetEudemonAmount(); i++)
        {
            CMonster* pEudemon = pUser->QueryEudemonByIndex(i);
            if (pEudemon && pEudemon->IsAlive() &&
                    (abs(pEudemon->GetPosX() - pTarget->GetPosX()) <= _RANGE_EXPSHARE
                     || abs(pEudemon->GetPosY() - pTarget->GetPosY()) <= _RANGE_EXPSHARE))
            {
                int nAwardExp = CBattleSystem::AdjustExp(nExp, pEudemon->GetLev(), pTarget->GetLev());
                bool bIncludeOwner = false;
                pEudemon->AwardBattleExp(nExp, true, bIncludeOwner);
            }
        }
        // no self
        if (!(pUser->IsAlive() && pUser->GetID() != idKiller))
        {
            continue;
        }
        // distance
        if (abs(pUser->GetPosX() - pTarget->GetPosX()) > _RANGE_EXPSHARE ||
                abs(pUser->GetPosY() - pTarget->GetPosY()) > _RANGE_EXPSHARE)
        {
            continue;    // out of share range
        }
        int nAddExp	= pUser->AdjustExp(pTarget, nExp);
        // 下面这几行如果可能最好并入AdjustExp
        CLevupexpData* pLevupexp	= LevupexpSet()->GetObj(EXP_TYPE_USER * _EXP_TYPE + pUser->GetLev());
        if (!pLevupexp)
        {
            return;
        }
        int nMaxStuExp = pLevupexp->GetInt(LEVUPEXPDATA_STU_EXP);
        nAddExp = __min(nAddExp, nMaxStuExp);
        if (!pKiller->IsNewbie() && pUser->IsNewbie())
        {
            // 老玩家带新玩家练级，可以得到导师经验
            int nTutorExp = nAddExp * 10 / nMaxStuExp;
            if (pKiller->GetStudent(pUser->GetID()))	// 如果新玩家是自己的学徒，导师经验翻倍
            {
                nTutorExp *= 2;
            }
            // award tutor exp
            pKiller->AwardTutorExp(nTutorExp);
            // msg
            pKiller->SendSysMsg(STR_TEACHER_EXP, nTutorExp);
        }
        // max exp
        if (nAddExp > pUser->GetLev() * MAX_TEAM_EXP_TIMES)
        {
            nAddExp = pUser->GetLev() * MAX_TEAM_EXP_TIMES;
        }
        // double exp
        if (pUser->IsMasterPrentice(pKiller) || pUser->IsMate(pKiller))
        {
            nAddExp *= 2;
        }
        // award exp now
        pUser->AwardBattleExp(nAddExp);
        // msg
        pUser->SendSysMsg(STR_TEAM_EXPERIENCE, nAddExp);
    }
}

//////////////////////////////////////////////////////////////////////
void CTeam::BroadcastMemberLife(CUser* pMember, bool bMaxLife)
{
    if (!pMember)
    {
        return;
    }
    for (int i = 0; i < this->GetMemberAmount(); i++)
    {
        CUser* pUser	= NULL;
        OBJID idUser	= this->GetMemberByIndex(i);
        DEBUG_TRY
        pUser	= UserManager()->GetUser(idUser);
        DEBUG_CATCH("Team broadcast member life get user by id crash.")
        if (pUser && pUser->GetID() != pMember->GetID())
        {
            CMsgUserAttrib msg;
            if (msg.Create(pMember->GetID(), _USERATTRIB_LIFE, pMember->GetLife()))
            {
                if (bMaxLife)
                {
                    msg.Append(_USERATTRIB_MAXLIFE, pMember->GetMaxLife());
                }
                pUser->SendMsg(&msg);
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////
BOOL CTeam::IsTeamMember(OBJID idMember)
{
    for(int i = 0; i < GetMemberAmount(); i++)
    {
        if (GetMemberByIndex(i) == idMember)
        {
            return TRUE;
        }
    }
    return FALSE;
}

//////////////////////////////////////////////////////////////////////
CTeam* CTeam::CloneNew()
{
    CTeam* pTeam	= new CTeam(m_idProcess);
    CHECKF(pTeam);
    *pTeam = *this;
    return pTeam;
}

//////////////////////////////////////////////////////////////////////
void CTeam::GetInfo(TeamInfoStruct* pInfo)
{
    memset(pInfo, 0, sizeof(TeamInfoStruct));
    pInfo->id		= m_id;
    pInfo->idLeader	= m_idLeader;
    for(int i = 0; i < m_setIdUser.size() && i < _MAX_TEAMAMOUNT; i++)
    {
        pInfo->setMember[i]	= m_setIdUser[i];
    }
    pInfo->bCloseMoney	= m_bCloseMoney;
    pInfo->bCloseItem	= m_bCloseItem;
}

//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
void CTeam::AddMemberOnly		(OBJID idMember)
{
    m_setIdUser.push_back(idMember);
}

//////////////////////////////////////////////////////////////////////
void CTeam::DelMemberOnly		(OBJID idMember)
{
    USERIDSET::iterator iter = find(m_setIdUser.begin(), m_setIdUser.end(), idMember);
    if (iter != m_setIdUser.end())
    {
        m_setIdUser.erase(iter);
    }
}


//////////////////////////////////////////////////////////////////////
void CTeam::AttachMemberStatus(int nStatus, int nPower, int nSecs, int nTimes, OBJID idExclude)
{
    CUser* pLeader = UserManager()->GetUser(this->GetLeader());
    if (!pLeader || !pLeader->IsAlive())
    {
        return ;    // 队长不存在或者死了
    }
    for (int i = 0; i < this->GetMemberAmount(); i++)
    {
        CUser*	pUser = UserManager()->GetUser(this->GetMemberByIndex(i));
        if (pUser)				//? may be another mapgroup
        {
            // 给幻兽加状态
            for (int i = 0; i < pUser->GetEudemonAmount(); i++)
            {
                CMonster* pEudemon = pUser->QueryEudemonByIndex(i);
                if (pEudemon
                        && pEudemon->GetMap()->GetID() == pLeader->GetMapID()
                        && (pEudemon->GetDistance(pLeader->QueryMapThing()) <= _RANGE_TEAM_STATUS || STATUS_ADD_EXP == nStatus))
                {
                    if (!pEudemon->QueryStatus(nStatus))
                    {
                        CRole::AttachStatus(pEudemon->QueryRole(), nStatus, nPower, nSecs, nTimes);
                    }
                }
            }
            // 给玩家加状态
            if (pUser->GetID() != idExclude)
            {
                if (pUser->IsAlive()
                        && pUser->GetMapID() == pLeader->GetMapID()
                        && (pUser->GetDistance(pLeader->QueryMapThing()) <= _RANGE_TEAM_STATUS || STATUS_ADD_EXP == nStatus))
                {
                    if (!pUser->QueryStatus(nStatus))
                    {
                        CRole::AttachStatus(pUser->QueryRole(), nStatus, nPower, nSecs, nTimes);
                    }
                }
            }
        }
    }
}
//////////////////////////////////////////////////////////////////////
void CTeam::AdjustMemberStatus(int nOldAmount, int nNewAmount)
{
    USERIDSET::iterator it = m_setIdUser.begin();
    for (; it != m_setIdUser.end(); it++)
    {
        CUser* pUser = UserManager()->GetUser(*it);
        if (pUser)
        {
            // 只有速度需要通知客户端，其他界结状态的影响效果都是即时计算的无需通知客户端
            if (pUser->QueryStatus(STATUS_TEAMSPEED))
            {
                CMsgUserAttrib msg;
                IF_OK (msg.Create(pUser->GetID(), _USERATTRIB_SPEED, pUser->AdjustSpeed(pUser->GetSpeed())))
                pUser->BroadcastRoomMsg(&msg, INCLUDE_SELF);
            }
        }
    }
    CUser* pLeader = UserManager()->GetUser(this->GetLeader());
    if (pLeader)
    {
        for (int i = STATUS_TEAM_BEGIN; i <= STATUS_TEAM_END; i++)
        {
            if (pLeader->QueryStatus(i))
            {
                char szMsg[256] = "";
                if (nNewAmount < TEAM_STATUS_REQ_ROLES)
                {
                    ::SafeCopy(szMsg, STR_TEAM_STATUS_DISAPPEAR, 256);
                }
                else
                {
                    if (nOldAmount < nNewAmount)
                    {
                        sprintf(szMsg, STR_TEAM_STATUS_POWER_INC, 10 * (nNewAmount - nOldAmount));
                    }
                    else
                    {
                        sprintf(szMsg, STR_TEAM_STATUS_POWER_DEC, 10 * (nOldAmount - nNewAmount));
                    }
                }
                CMsgTalk msg;
                if (msg.Create(SYSTEM_NAME, ALLUSERS_NAME, szMsg, NULL, 0xff0000, _TXTATR_SYSTEM))
                {
                    this->BroadcastTeamMsg(&msg);
                }
                break;
            }
        }
    }
}
//////////////////////////////////////////////////////////////////////
// 平分伤害，返回修改后的实际伤害
int CTeam::ShareDamage(int nDamage, OBJID idExclude)
{
    typedef std::vector<IRole*>		ROLE_SET;
    ROLE_SET	setMember;
    for (int i = 0; i < this->GetMemberAmount(); i++)
    {
        OBJID idMember = this->GetMemberByIndex(i);
        CUser* pMember = UserManager()->GetUser(idMember);
        // 与带有心灵结界的其他队伍成员(包括幻兽)平分伤害 -- 只考虑当前地图组
        if (pMember)
        {
            for (int i = 0; i < pMember->GetEudemonAmount(); i++)
            {
                CMonster* pEudemon = pMember->QueryEudemonByIndex(i);
                if (pEudemon && pEudemon->GetLife() > 1 && pEudemon->QueryStatus(STATUS_TEAMSPIRIT))
                {
                    setMember.push_back(pEudemon->QueryRole());
                }
            }
            if (pMember->GetID() != idExclude && pMember->GetLife() > 1 && pMember->QueryStatus(STATUS_TEAMSPIRIT))
            {
                setMember.push_back(pMember->QueryRole());
            }
        }
    }
    nDamage	= __max(1, nDamage / (setMember.size() + 1));	// 平均伤害值
    for (ROLE_SET::iterator it = setMember.begin(); it != setMember.end(); it++)
    {
        IRole* pRole = *it;
        ASSERT(pRole);
        // TODO：
        // ?? 幻兽的生命是灵力，所以这里恐怕针对幻兽得做修改 -- zlong 2004.11.26
        int nLifeLost = __min(pRole->GetLife() - 1, nDamage);
        pRole->AddAttrib(_USERATTRIB_LIFE, -1 * nLifeLost, SYNCHRO_TRUE);
    }
    setMember.clear();
    return nDamage;
}

//带其他玩家练级，每带其他玩家练级一个小时，军团声望增加100点。
//void CTeam::DealTeamLeaderInfo()
//{
//	DEBUG_TRY
//
//	if (this->m_tGuidingTime.ToNextTime())
//	{
//		CUser * pLeader = UserManager()->GetUser(this->GetLeader());
//		if (pLeader)
//		{
//			CSyndicate * pSyn = pLeader->GetSyndicate();
//			if (pSyn)
//			{
//				pSyn->QueryModify()->AddData(SYNDATA_REPUTE, TEAMLEADER_WITHNEWBIE_REPUTEINC, true);
//				//添加声望增量
//			//	pLeader->QuerySynAttr()->AddReputeInc(TEAMLEADER_WITHNEWBIE_REPUTEINC);
//			}
//		}
//	}
//
//	DEBUG_CATCH("void CTeam::DealTeamLeaderInfo()")
//}
