
//**********************************************************
// 代码编辑器
//**********************************************************

// Npc.cpp: implementation of the CNpc class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "Npc.h"
#include "NpcWorld.h"
#include "DynaNpcGen.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CNpc::CNpc()
    : m_bLocked(false)
{
    SetObjType(OBJ_NPC);
    m_id				= ID_NONE;
    m_pType				= NULL;
    m_nActMode			= MODE_IDLE;
    m_tAction.SetInterval(0);				// 休眠状态不检查超时
    m_pMap				= NULL;
    m_nPosX, m_nPosY	= 0;
    m_i64Effect			= 0;
    m_nDir				= 0;
    m_nPose				= 0;
    m_idAtkMe			= ID_NONE;
    m_bAtkFirst			= ID_NONE;
    m_idActTarget		= ID_NONE;
    m_idMoveTarget		= ID_NONE;
    //	m_pGenerator		= NULL;
    m_bAheadPath		= true;
    m_nPathLen			= 0;
    m_tMove.SetInterval(0);
    m_pMap				= NULL;
    m_bDie				= false;
    m_lnkThis.Init(Cast<IRole>(this));
}

//////////////////////////////////////////////////////////////////////
CNpc::~CNpc()
{
    if (GetMap())
    {
        GetMap()->LeaveMap(this);
    }
    if (m_pGenerator != NULL)
    {
        if (this->GetID() != MONSTERID_FIRST)
        {
            NpcManager()->QueryRecycle()->push(this->GetID());
        }
        if (!m_bDie)
        {
            m_pGenerator->DeleteNpc(this->GetID());
        }
    }
}

//////////////////////////////////////////////////////////////////////
bool CNpc::Create(OBJID idNpc, int nType, OBJID idMap, int nPosX, int nPosY, OBJID idGenerator,
                  OBJID idOwner, int nOwnerType, int nData)
{
    CHECKF(idNpc != ID_NONE);
    m_pType		= NpcManager()->QueryNpcTypeSet()->GetObj(nType);
    CHECKF(m_pType);
    IGenerator* pGenerator	= NpcManager()->QueryGenSet()->GetObj(idGenerator)->QueryInterface();
    CHECKF(pGenerator);
    if (!IsMonsterID(idNpc) && pGenerator->GetPosX() == 0 && pGenerator->GetPosY())
    {
        m_pGenerator	= CDynaNpcGen::CreateNew(pGenerator, idMap, nPosX, nPosY);
        CHECKF(m_pGenerator);
    }
    else
    {
        m_pGenerator	= pGenerator;
    }
    m_id			= idNpc;
    m_pMap			= MapManager()->QueryMap(idMap);
    m_nPosX			= nPosX;
    m_nPosY			= nPosY;
    m_i64Effect		= KEEPEFFECT_NORMAL;
    m_nCurrLife		= m_pType->GetInt(NPCTYPEDATA_LIFE);
    m_nCurrPower	= m_pType->GetInt(NPCTYPEDATA_MANA);
    m_tAction.SetInterval(::CutTrail((m_pType->GetInt(NPCTYPEDATA_ATKSPEED) * 3) / 1000, THINK_SECS));
    m_tAction.Update();
    m_tMove.SetInterval(m_pType->GetInt(NPCTYPEDATA_MOVESPEED));
    m_tMove.Update();
    m_tDormancy.SetInterval(DORMANCY_SECS);
    m_tDormancy.Update();
    m_idOwner		= idOwner;
    m_nOwnerType	= nOwnerType;
    if (m_idOwner == ID_NONE)
    {
        m_nOwnerType	= OWNER_NONE;
    }
    if (IsLockUser())
    {
        m_idActTarget	= (OBJID)nData;
        m_idMoveTarget	= m_idActTarget;
        m_nActMode		= MODE_ATTACK;
    }
    if (!GetMap())
    {
        return false;
    }
    GetMap()->EnterMap(this);
    if (IsDormancyEnable())
    {
        if (GetMap()->SetDormancy(GetPosX(), GetPosY()))
        {
            this->ChangeMode(MODE_DORMANCY);
        }
    }
    m_pGenerator->GeneratorOK(this->GetID());
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CNpc::ProcessEscape()
{
    if (!IsEscapeEnable())
    {
        ASSERT(!"CNpc::ProcessEscape()");
        ChangeMode(MODE_IDLE);
        return true;
    }
    {
        IRole* pRole = UserManager()->QueryRole(m_idActTarget);
        if ((IsGuard() || IsPkKiller()) && pRole)
        {
            JumpPos(pRole->GetPosX(), pRole->GetPosY(), GetDir());
            this->ChangeMode(MODE_FORWARD);
            return true;
        }
    }
    if (m_nPathLen == 0)
    {
        FindPath(GetInt(NPCTYPEDATA_VIEWRANGE) * 2);    // range : escape。如果目标消失或越逃越近，path_len置为0
    }
    if (m_idActTarget == ID_NONE)									// 目标消失->休息状态
    {
        ChangeMode(MODE_IDLE);
        return true;
    }
    else if (m_idActTarget != ID_NONE && m_nPathLen == 0)			// 逃不了,有目标->冲锋状态
    {
        ChangeMode(MODE_FORWARD);
        return true;
    }
    else // if (m_idActTarget != ID_NONE && m_nPathLen != 0)
    {
        PathMove(WALKMODE_RUN);
        return true;
    }
}

//////////////////////////////////////////////////////////////////////
bool CNpc::ProcessAttack()
{
    // keep attack
    IRole* pRole = UserManager()->QueryRole(m_idActTarget);
    if (pRole && pRole->IsAlive() && (pRole->GetEffect() & KEEPEFFECT_LURKER) == 0
            && this->Distance(pRole->GetPosX(), pRole->GetPosY()) <= GetAttackRange(pRole->GetSizeAdd()))
    {
        if (m_tAction.ToNextTime())
        {
            if (GetMap()->IsSuperposition(QueryRole()))
            {
                m_bAheadPath = false;
                DetectPath(-1);
                m_bAheadPath = true;
                if (m_nPathLen)
                {
                    PathMove(WALKMODE_SHIFT);
                }
            }
            if (m_bAttackFlag)
            {
                m_bAttackFlag = false;		// 正在攻击
            }
            else
            {
                ChangeMode(MODE_FORWARD);			// MODE_IDLE	// 不休息，否则被震晕后会逃
                return true;
            }
        }
        return true;
    }
    // change mode
    if (FindNewTarget())
    {
        IRole* pRole = UserManager()->QueryRole(m_idMoveTarget);
        if (pRole && pRole->IsAlive()
                && this->Distance(pRole->GetPosX(), pRole->GetPosY()) > GetAttackRange(pRole->GetSizeAdd()))
        {
            if (m_nPathLen != 0 && IsMoveEnable())					// 有路，目标可见->冲锋
            {
                ChangeMode(MODE_FORWARD);
                return true;
            }
            else // if (m_nPathLen == 0)
            {
                ChangeMode(MODE_IDLE);
                return true;
            }
        }
        return true;
    }
    // not target
    ChangeMode(MODE_IDLE);
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CNpc::ProcessForward()
{
    // test attack enable
    IRole* pRole = UserManager()->QueryRole(m_idMoveTarget);
    if (pRole && pRole->IsAlive() && (pRole->GetEffect() & KEEPEFFECT_LURKER) == 0
            && this->Distance(pRole->GetPosX(), pRole->GetPosY()) <= GetAttackRange(pRole->GetSizeAdd()))
    {
        if (!IsGuard() && !IsMoveEnable() && IsFarWeapen() && !m_bAheadPath && m_nPathLen > 0)			// 处理弓箭手后撤
        {
            if (PathMove(WALKMODE_RUN))
            {
                return true;
            }
        }
        ChangeMode(MODE_ATTACK);
        return true;
    }
    // process forward
    //	   too far
    POINT pos = m_pGenerator->GetCenter();
    if ((IsGuard() || IsPkKiller() || IsFastBack()) && m_pGenerator->IsTooFar(GetPosX(), GetPosY(), GUARD_LEAVEDISTANCE))
    {
        m_idActTarget	= ID_NONE;					// 清除攻击者
        m_idMoveTarget	= m_idActTarget;
        FarJump(pos.x, pos.y, GetDir());
        ClearPath();
        ChangeMode(MODE_IDLE);
        return true;
    }
    //	  guard jump
    if ((IsGuard() || IsPkKiller() || IsEvilKiller()) && pRole && Distance(pRole->GetPosX(), pRole->GetPosY()) >= PKKILLER_JUMPDISTANCE)
    {
        JumpPos(pRole->GetPosX(), pRole->GetPosY(), GetDir());
        return true;
    }
    //	  forward
    if (m_nPathLen == 0)
    {
        if (FindNewTarget())		// 同时FindPath()
        {
            if (m_nPathLen == 0)									// 无路，目标可见->逃跑状态
            {
                if (IsJumpEnable())
                {
                    IRole* pRole = UserManager()->QueryRole(m_idMoveTarget);
                    IF_OK(pRole)
                    JumpBlock(pRole->GetPosX(), pRole->GetPosY(), GetDir());
                    return true;
                }
                else
                {
                    ChangeMode(MODE_IDLE);
                    return true;
                }
            }
            else // if (m_nPathLen != 0)
            {
                return false;	// 重新调用
            }
        }
        else
        {
            ChangeMode(MODE_IDLE);
            return true;
        }
    }
    else // if (m_nPathLen != 0)											// 有路，就要走
    {
        if (m_idMoveTarget != ID_NONE)
        {
            PathMove(WALKMODE_RUN);
        }
        else
        {
            PathMove(WALKMODE_MOVE);
        }
        return true;
    }
}

//////////////////////////////////////////////////////////////////////
bool CNpc::ProcessIdle()
{
    if (IsLockUser())
    {
        CMsgAction	msg;
        IF_OK(msg.Create(GetID(), GetPosX(), GetPosY(), GetDir(), actionDie))
        SendMsg(&msg);
    }
    if (!( IsGuard() || IsPkKiller() || IsEvilKiller() ))
    {
        // walk pass path
        if (m_nPathLen > 0)
        {
            PathMove(WALKMODE_MOVE);
            return true;
        }
        if (!( IsFastBack() && !m_pGenerator->IsInRegion(GetPosX(), GetPosY()) ))
        {
            if (!m_tAction.ToNextTime())
            {
                return true;
            }
        }
    }
    // change to other mode
    m_idAtkMe	= ID_NONE;
    m_bAtkFirst	= false;
    if (IsActiveEnable() && FindNewTarget())
    {
        IRole* pRole = UserManager()->QueryRole(m_idMoveTarget);
        ASSERT(pRole);
        if (pRole)
        {
            int nDistance = this->Distance(pRole->GetPosX(), pRole->GetPosY());
            if (nDistance <= GetAttackRange(pRole->GetSizeAdd()))
            {
                ChangeMode(MODE_ATTACK);
                return false;
            }
            else if (IsMoveEnable())
            {
                if (m_nPathLen == 0)								// 无路，目标可见->逃跑状态
                {
                    if (!IsEscapeEnable() || ::RandGet(100) < 80)		// keep idel
                    {
                        return true;
                    }
                    ChangeMode(MODE_ESCAPE);
                    return false;
                }
                else //if (m_nPathLen != 0)
                {
                    ChangeMode(MODE_FORWARD);
                    return false;
                }
            }
        }
        else
        {
            // DEBUG???@@@
            LOGWARNING("npc: [%d][%d], target id: [%d]", GetID(), m_pType->GetID(), m_idMoveTarget);
        }
    }
    if (( IsGuard() || IsPkKiller() || IsEvilKiller() ))
    {
        // walk pass path
        if (m_nPathLen > 0)
        {
            PathMove(WALKMODE_MOVE);
            return true;
        }
        if (!m_tAction.ToNextTime())
        {
            return true;
        }
    }
    if (!IsMoveEnable())
    {
        return true;
    }
    // keep idle
    if (m_pGenerator->IsInRegion(GetPosX(), GetPosY()))
    {
        if (IsGuard() || IsPkKiller() || IsEvilKiller())
        {
            // patrol
            if (::RandGet(GUARD_RANDOMMOVE_RATE) == 0
                    && m_pGenerator->GetWidth() > 1 || m_pGenerator->GetHeight() > 1)
            {
                int x = ::RandGet(m_pGenerator->GetWidth()) + m_pGenerator->GetPosX();
                int y = ::RandGet(m_pGenerator->GetHeight()) + m_pGenerator->GetPosY();
                if (FindPath(x, y))
                {
                    PathMove(WALKMODE_MOVE);
                }
            }
        }
        else
        {
            if (::RandGet(RANDOMMOVE_RATE) == 0)
            {
                int nDir = ::RandGet(MAX_DIRSIZE);
                if (TestPath(nDir))
                {
                    PathMove(WALKMODE_MOVE);
                }
            }
        }
    }
    else // out of range
    {
        // move back
        if ((IsGuard() || IsPkKiller() || IsFastBack()) || ::RandGet(RANDOMMOVE_RATE) == 0)
        {
            POINT pos;
            pos = m_pGenerator->GetCenter();
            if (FindPath(pos.x, pos.y))
            {
                PathMove(WALKMODE_MOVE);
            }
            else if (IsGuard() || IsPkKiller() || IsJumpEnable())
            {
                JumpBlock(pos.x, pos.y, GetDir());
            }
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CNpc::CheckDormancy()
{
    if (!m_tDormancy.ToNextTime())
    {
        return false;
    }
    if (GetMap() && GetMap()->SetDormancy(GetPosX(), GetPosY()))
    {
        ChangeMode(MODE_DORMANCY);
        return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
void CNpc::BeAttack(IRole* pRole)
{
    m_idAtkMe	= pRole->GetID();
    if (!IsAlive())
    {
        return ;
    }
    if (!IsMoveEnable())
    {
        return ;
    }
    if (IsEscapeEnable())
    {
        // 只会逃跑
        if ((GetInt(NPCTYPEDATA_ATKUSER) & ATKUSER_PASSIVE) == 0)
        {
            m_idActTarget	= pRole->GetID();			// 新目标
            m_idMoveTarget	= m_idActTarget;
            ChangeMode(MODE_ESCAPE);
            return ;
        }
        // 血少逃跑
        if (GetLife() < GetInt(NPCTYPEDATA_ESCAPELIFE))
        {
            ChangeMode(MODE_ESCAPE);
            return ;
        }
    }
    // 弓箭手回撤
    int nDistance = this->Distance(pRole->GetPosX(), pRole->GetPosY());
    if (IsFarWeapen() && !pRole->IsFarWeapen() && nDistance <= SHORTWEAPON_RANGE_LIMIT)
    {
        m_idActTarget	= pRole->GetID();			// 新目标
        m_idMoveTarget	= m_idActTarget;
        int	nSteps = GetAttackRange(pRole->GetSizeAdd()) - nDistance;
        FindPath(nSteps);
        if (m_nPathLen > nSteps)
        {
            m_nPathLen	= nSteps;
        }
        ChangeMode(MODE_FORWARD);
        return;
    }
    // 冲锋
    if (!IsAttackEnable(pRole))
    {
        return ;
    }
    if (m_nActMode < MODE_FORWARD)
    {
        if (m_nActMode == MODE_ESCAPE && ::RandGet(100) < 80)		// 20% 机会反击
        {
            return ;
        }
        m_idActTarget	= pRole->GetID();			// 新目标
        m_idMoveTarget	= m_idActTarget;
        if (::RandGet(100) < 80)
        {
            ChangeMode(MODE_FORWARD);
            FindPath(pRole->GetPosX(), pRole->GetPosY());			// 不检查可视范围，直接Find坐标。
            if (m_nPathLen > 0)
            {
                return ;
            }
        }
        if (IsEscapeEnable())
        {
            ChangeMode(MODE_ESCAPE);
            FindPath(pRole->GetPosX(), pRole->GetPosY(), GetInt(NPCTYPEDATA_VIEWRANGE));			// 不检查可视范围，直接Find坐标。
        }
        return ;
    }
}

//////////////////////////////////////////////////////////////////////
void CNpc::OnTimer()
{
    if (CheckDormancy())				// 如超时，休眠
    {
        return ;
    }
    if (!IsAlive() || IsLocked() || !IsActive())	// 死亡或被锁定状态或不能活动状态
    {
        return;
    }
    for(int i = 0; i < 10; i++)			// 10 : 每次最多10 个AI分析，防死循环
    {
        switch(m_nActMode)
        {
        case	MODE_ESCAPE:
            if (ProcessEscape())
            {
                return;
            }
            break;
        case	MODE_ATTACK:
            if (ProcessAttack())
            {
                return;
            }
            break;
        case	MODE_FORWARD:
            if (ProcessForward())
            {
                return;
            }
            break;
        case	MODE_IDLE:
            if (ProcessIdle())
            {
                return;
            }
            break;
        }
    }
}

//////////////////////////////////////////////////////////////////////
bool CNpc::ChangeMode(int nNewMode)			// return false : 失败
{
    switch(nNewMode)
    {
    case	MODE_DORMANCY:
        {
            m_nCurrLife		= m_pType->GetInt(NPCTYPEDATA_LIFE);
            m_nCurrPower	= m_pType->GetInt(NPCTYPEDATA_MANA);
        }
        break;
    case	MODE_ATTACK:
        {
            Attack();
        }
        break;
    case	MODE_IDLE:
        {
            //			m_tAction.SetInterval(THINK_SECS);
            //			m_tAction.Update();
        }
        break;
    }
    if (nNewMode != MODE_FORWARD)
    {
        ClearPath();
    }
    m_nActMode	= nNewMode;
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CNpc::FindPath(int nEscapeSteps /*= 0*/)
{
    CHECKF(GetMap());
    m_bAheadPath	= (nEscapeSteps == 0);
    ClearPath();
    // 检查目标是否有效
    IRole* pRole = UserManager()->QueryRole(m_idMoveTarget);
    if (!pRole || !pRole->IsAlive() || this->Distance(pRole->GetPosX(), pRole->GetPosY()) > GetInt(NPCTYPEDATA_VIEWRANGE))
    {
        m_idActTarget = ID_NONE;
        m_idMoveTarget	= m_idActTarget;
        ClearPath();
        return m_nPathLen != 0;
    }
    if (!FindPath(pRole->GetPosX(), pRole->GetPosY(), nEscapeSteps))
    {
        return false;
    }
    // 检查寻路是否有效
    if (m_nPathLen > 0)
    {
        int nDir	= m_bufPath[0] % MAX_DIRSIZE;
        if (!GetMap()->IsMoveEnable(GetPosX(), GetPosY(), nDir, GetSizeAdd()))
        {
            DetectPath(nDir);
            return m_nPathLen != 0;
        }
        if (m_bAheadPath && pRole->Distance(m_posTarget.x, m_posTarget.y)
                >= pRole->Distance(GetPosX(), GetPosY())
                || !m_bAheadPath && pRole->Distance(m_posTarget.x, m_posTarget.y)
                <= pRole->Distance(GetPosX(), GetPosY()) )
        {
            ClearPath();
            return m_nPathLen != 0;
        }
    }
    return m_nPathLen != 0;
}

//////////////////////////////////////////////////////////////////////
bool CNpc::FindPath(int x, int y, int nEscapeSteps /*= 0*/)
{
    ClearPath();
    if (x == GetPosX() && y == GetPosY())
    {
        return false;
    }
    char	buf[_MAX_PATHSTEP + sizeof(PASSPATH0)];
    PASSPATH0* pPath = (PASSPATH0*)buf;
    POINT	posSource, posTarget;
    posSource.x	= GetPosX();
    posSource.y	= GetPosY();
    posTarget.x	= x;
    posTarget.y	= y;
    GetMap()->QueryMapData()->FindPath(pPath, sizeof(buf), posSource, posTarget, QueryRoleAttr(), nEscapeSteps);
    m_posTarget		= pPath->posStop;
    memcpy(m_bufPath, pPath->pPath, pPath->nSize);
    m_nPathLen		= pPath->nSize;
    return m_nPathLen > 0;
}

//////////////////////////////////////////////////////////////////////
bool CNpc::DetectPath(int nNoDir)		// -1: 全允许
{
    ClearPath();
    POINT posTarget;
    if (m_idMoveTarget != ID_NONE)
    {
        IRole* pRole = UserManager()->QueryRole(m_idMoveTarget);		//@ 只打玩家
        if (pRole)
        {
            posTarget.x = pRole->GetPosX();
            posTarget.y = pRole->GetPosY();
        }
    }
    else
    {
        posTarget = m_pGenerator->GetCenter();
    }
    int nDistOld = Distance(posTarget.x, posTarget.y);
    int nBestDist = nDistOld;
    int	nBestDir	= -1;			// -1: invalid dir
    int	nFirstDir	= ::RandGet(MAX_DIRSIZE);
    for(int i = nFirstDir; i < nFirstDir + MAX_DIRSIZE; i++)
    {
        int nDir = i % MAX_DIRSIZE;
        if (nDir != nNoDir)
        {
            if (GetMap() && GetMap()->IsMoveEnable(GetPosX(), GetPosY(), nDir, GetSizeAdd(), NPC_CLIMBCAP))
            {
                int nNewX = GetPosX() + _DELTA_X[nDir];
                int nNewY = GetPosY() + _DELTA_Y[nDir];
                int nDist = ::Distance(nNewX, nNewY, posTarget.x, posTarget.y);
                if ((nBestDist - nDist) * (m_bAheadPath ? 1 : -1) > 0)
                {
                    nBestDist	= nDist;
                    nBestDir	= nDir;
                }
            }
        }
    }
    if (nBestDir != -1)			// -1: invalid dir
    {
        m_bufPath[0]	= nBestDir;
        m_nPathLen		= 1;
        return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
bool CNpc::TestPath(int nDir)
{
    CHECKF(nDir >= 0 && nDir < MAX_DIRSIZE);
    if (GetMap() && GetMap()->IsMoveEnable(GetPosX(), GetPosY(), nDir, GetSizeAdd(), NPC_CLIMBCAP))
    {
        m_bufPath[0]	= nDir;
        m_nPathLen		= 1;
        return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
bool CNpc::PathMove(int nMode)		// false: 没路了
{
    if (nMode == WALKMODE_MOVE)
    {
        if (!m_tMove.ToNextTick(GetInt(NPCTYPEDATA_MOVESPEED)))
        {
            return true;
        }
    }
    else
    {
        if (!m_tMove.ToNextTick(GetInt(NPCTYPEDATA_RUNSPEED)))
        {
            return true;
        }
    }
    if (m_nPathLen <= 0)
    {
        return false;
    }
    int nDir	= m_bufPath[0] % MAX_DIRSIZE;
    m_nPathLen--;
    if (m_nPathLen)
    {
        memcpy(m_bufPath, m_bufPath + 1, m_nPathLen);
    }
    if (MoveForward(nDir, nMode))
    {
        return true;
    }
    if (DetectPath(nDir) && m_nPathLen)
    {
        m_nPathLen = 0;
        return MoveForward(m_bufPath[0], nMode);
    }
    if (IsJumpEnable())
    {
        POINT pos = m_pGenerator->GetCenter();
        return JumpBlock(pos.x, pos.y, GetDir());
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
void CNpc::JumpPos(int nPosX, int nPosY, int nDir)
{
    IF_NOT(GetMap()->IsValidPoint(nPosX, nPosY))
    return ;
    ClearPath();
    SetPos(nPosX, nPosY);
    SetDir(nDir);
    {
        CMsgAction	msg;
        if (msg.Create(GetID(), GetPosX(), GetPosY(), nDir, actionJump, nPosX, nPosY))
        {
            SendMsg(&msg);
        }
    }
}

//////////////////////////////////////////////////////////////////////
OBJID CNpc::GetMapID()
{
    if (GetMap())
    {
        return GetMap()->GetID();
    }
    return ID_NONE;
}

//////////////////////////////////////////////////////////////////////
bool CNpc::MoveForward(int nDir, int nMode /*= WALKMODE_MOVE*/)
{
    m_nDir	= nDir % MAX_DIRSIZE;
    int	nDirX	= _DELTA_X[m_nDir];
    int	nDirY	= _DELTA_Y[m_nDir];
    int nNewPosX = GetPosX() + nDirX;
    int nNewPosY = GetPosY() + nDirY;
    IF_NOT(GetMap()->IsValidPoint(nNewPosX, nNewPosY))
    return false;
    if (GetMap() && GetMap()->IsMoveEnable(GetPosX(), GetPosY(), GetDir(), GetSizeAdd()))
    {
        CMsgWalk	msg;
        CHECKF(msg.Create(GetID(), nDir, nMode));
        SendMsg(&msg);
        SetPos(m_nPosX + nDirX, m_nPosY + nDirY);
#ifdef	PALED_DEBUG_X
        MSGBUF	szMsg;
        sprintf(szMsg, "(%d,%d)-%u", m_nPosX, m_nPosY, GetID());
        SendMsg(szMsg);
#endif
        return true;
    }
    else
    {
        return false;
    }
}

//////////////////////////////////////////////////////////////////////
bool CNpc::KickBack(int nTargX, int nTargY)
{
    IF_NOT(GetMap()->IsValidPoint(nTargX, nTargY))
    return false;
    //!	if (GetPosX() != nTargX || GetPosY() != nTargY)
    {
        CMsgAction	msg;
        IF_OK(msg.Create(GetID(), nTargX, nTargY, GetDir(), actionSynchro, nTargX, nTargY))
        SendMsg(&msg);
    }
    ClearPath();
    SetPos(nTargX, nTargY);
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CNpc::SynchroPos(int nSourX, int nSourY, int nTargX, int nTargY)
{
    if (GetPosX() != nSourX || GetPosY() != nSourY)
    {
        CMsgAction	msg;
        IF_OK(msg.Create(GetID(), GetPosX(), GetPosY(), GetDir(), actionSynchro, GetPosX(), GetPosY()))
        SendMsg(&msg);
    }
    else if (GetPosX() != nTargX || GetPosY() != nTargY)
    {
        IF_NOT(GetMap()->IsValidPoint(nTargX, nTargY))
        return false;
        ClearPath();
        SetPos(nTargX, nTargY);
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
void CNpc::SetPos(int nPosX, int nPosY)
{
    IF_OK(GetMap())
    GetMap()->MoveTo(this, nPosX, nPosY);
    m_nPosX = nPosX;
    m_nPosY = nPosY;
}

//////////////////////////////////////////////////////////////////////
void CNpc::Attack()
{
    if (m_idActTarget == ID_NONE)
    {
        return;
    }
    CMsgInteract	msg;
    if (msg.Create(INTERACT_ATTACK, GetID(), m_idActTarget, GetPosX(), GetPosY()))
    {
        SendMsg(&msg);
    }
    if (m_idAtkMe != m_idActTarget)
    {
        m_bAtkFirst = true;
    }
}

//////////////////////////////////////////////////////////////////////
bool CNpc::CheckTarget()
{
    if (m_idActTarget == ID_NONE)
    {
        m_idActTarget	= ID_NONE;
        m_idMoveTarget	= m_idActTarget;
        return false;
    }
    IRole* pRole = UserManager()->QueryRole(m_idActTarget);
    if (!pRole || !pRole->IsAlive())
    {
        m_idActTarget	= ID_NONE;
        m_idMoveTarget	= m_idActTarget;
        return false;
    }
    if (pRole->IsWing() && !IsWing() && IsCloseAttack())
    {
        return false;
    }
    int	nDistance	= GetInt(NPCTYPEDATA_VIEWRANGE);
    int	nAtkDistance = ::CutOverflow(nDistance, GetAttackRange(pRole->GetSizeAdd()));
    int nDist =  this->Distance(pRole->GetPosX(), pRole->GetPosY());
    if (!( nDist <= nDistance && !pRole->IsLurker() || nDist <= nAtkDistance && GetAttackRange(0) > 1 ))
    {
        m_idActTarget	= ID_NONE;
        m_idMoveTarget	= m_idActTarget;
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CNpc::FindNewTarget()
{
    if (!GetMap())
    {
        return false;
    }
    // lock target
    if (IsLockUser() || IsLockOne())
    {
        if (CheckTarget())
        {
            if (IsLockOne())
            {
                return true;
            }
        }
        else
        {
            if (IsLockUser())
            {
                return false;
            }
        }
    }
    OBJID idOldTarget =  m_idActTarget;
    m_idActTarget = ID_NONE;
    m_idMoveTarget	= m_idActTarget;
    CUserPtr	pTargetUser = NULL;
    // no_profession	CUserPtr	pProfession = NULL;
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // 修改：
    //     为了支持“吸引怪物”的效果，不论是否主动攻击怪物都主动搜索目标
    //     这样会降低NPC服务器的效率
    //     -- zlong 2005-01-04
    ///////////////////////////////////////////////////////////////////////////////////////////////
    //	if ((GetInt(NPCTYPEDATA_ATKUSER) & ATKUSER_ACTIVE) != 0)
    {
        //		IRole* pRole = UserManager()->QueryRole(m_idActTarget);
        //		if (!( pRole && pRole->IsAlive()
        //				&& this->Distance(pRole->GetPosX(), pRole->GetPosY()) <= GetInt(NPCTYPEDATA_VIEWRANGE) ))
        //			m_idActTarget	= ID_NONE;
        int x, y;
        int	nDistance	= GetInt(NPCTYPEDATA_VIEWRANGE);
        // no_profession		int nProfDistance	= nDistance;
        FOR_9_BLOCKS(GetMap(), GetPosX(), GetPosY())
        {
            IUserSet* pUserSet	= GetMap()->BlockByIndex(x, y).QueryUserSet();
            for(int i = 0; i < pUserSet->GetAmount(); i++)
            {
                CUserPtr pUser = pUserSet->GetObjByIndex(i);
                if (pUser && pUser->IsAlive() && (pUser->GetEffect() & KEEPEFFECT_LURKER) == 0
                        && pUser->GetID() != this->GetID())
                {
                    if (	IsGuard() && pUser->IsCrime()
                            || IsPkKiller() && pUser->IsPker()
                            || IsEvilKiller() && pUser->IsVirtuous()
                            || IsEvil() && !(IsPkKiller() || IsEvilKiller())
                            || IsSynPet() && (IsOpposedSyn(pUser->GetSynID()) || (pUser->GetEffect()&KEEPEFFECT_SYNCRIME))
                            || (pUser->GetEffect()&KEEPEFFECT_ATTRACT_MONSTER)	// 目标带吸引怪物的状态
                            || (GetEffect()&KEEPEFFECT_CONFUSION))		// 自己是混乱状态
                    {
                        if (!IsAttackEnable(pUser->QueryRole()))
                        {
                            continue;
                        }
                        int	nAtkDistance = ::CutOverflow(nDistance, GetAttackRange(pUser->GetSizeAdd()));
                        int nDist =  this->Distance(pUser->GetPosX(), pUser->GetPosY());
                        if (nDist <= nDistance && !pUser->IsLurker()
                                || nDist <= nAtkDistance && GetAttackRange(0) > 1
                                || ((pUser->GetEffect()&KEEPEFFECT_ATTRACT_MONSTER) && (!pTargetUser || 0 == (pTargetUser->GetEffect()&KEEPEFFECT_ATTRACT_MONSTER))))	// 带吸引怪物状态且距离近的目标优先攻击
                        {
                            nDistance	= nDist;
                            m_idActTarget	= pUser->GetID();			// 新目标
                            m_idMoveTarget	= m_idActTarget;
                            pTargetUser		= pUser;
                        }
                        /* no_profession
                        						if ((IsFighterFirst() && pUser->IsFighter() || IsWizardFirst() && pUser->IsWizard() || IsBowmanFirst() && pUser->IsBowman()))
                        						{
                        							if (nDist <= nProfDistance && !pUser->IsLurker() || nDist <= nAtkDistance && GetAttackRange(0) > 1)
                        							{
                        								nProfDistance	= nDist;
                        								pProfession		= pUser;
                        							}
                        						}*/
                    }
                }
            }
            INpcSet* pNpcSet	= GetMap()->BlockByIndex(x, y).QueryNpcSet();
            for(int i = 0; i < pNpcSet->GetAmount(); i++)
            {
                CNpcPtr pNpc = pNpcSet->GetObjByIndex(i);
                if (Cast<CAgent>(pNpc))
                {
                    pNpc->GetID();
                }
                if (pNpc && pNpc->IsAlive() && pNpc->GetID() != this->GetID()
                        && (pNpc->GetEffect() & KEEPEFFECT_LURKER) == 0
                        && (IsEvil() && pNpc->IsRighteous()
                            || IsRighteous() && pNpc->IsEvil()
                            || (GetEffect()&KEEPEFFECT_CONFUSION)))		// 自己是混乱状态
                {
                    if (pNpc->IsWing() && !IsWing() && IsCloseAttack())
                    {
                        continue;
                    }
                    int nDist =  this->Distance(pNpc->GetPosX(), pNpc->GetPosY());
                    if (nDist <= nDistance)
                    {
                        nDistance	= nDist;
                        m_idActTarget	= pNpc->GetID();			// 新目标
                        m_idMoveTarget	= m_idActTarget;
#ifdef	LOCAL_DEBUG
                        CNpc* pMonster = Cast<CNpc>(pNpc);
                        if (pMonster)
                            LOGERROR("正邪怪物区域冲突。怪物(类型[%d]发生器[%d])开始攻击怪物(类型[%d]发生器[%d])",
                                     GetType(), GetGenerator()->GetID(), pMonster->GetType(), pMonster->GetGenerator()->GetID());
#endif
                    }
                }
            }
        }
    } // 主动攻击
    /* no_profession
    	if (pProfession)		// && (IsFighterFirst() || IsWizardFirst() || IsBowmanFirst())
    		m_idActTarget	= pProfession->GetID();
    */
    if (m_idActTarget != ID_NONE)
    {
        if (pTargetUser && idOldTarget != pTargetUser->GetID())
        {
            if (IsGuard() && pTargetUser->IsCrime())
            {
                MSGBUF	szMsg;
                sprintf(szMsg, STR_GUARD_CALL_s, pTargetUser->GetName());
                SendMsg(szMsg);
            }
            else if (IsPkKiller() && pTargetUser->IsPker() && m_nActMode == MODE_IDLE)
            {
                MSGBUF	szMsg;
                sprintf(szMsg, STR_PPKER_CALL_s, pTargetUser->GetName());
                SendMsg(szMsg);
            }
        }
        FindPath();					// 没有PATH也返回TRUE
        if ((GetInt(NPCTYPEDATA_ATKUSER) & ATKUSER_ACTIVE) == 0		// 不是主动攻击怪物
                && (GetEffect() & KEEPEFFECT_CONFUSION) == 0)			// 并且不是混乱状态，清除目标
        {
            m_idActTarget = ID_NONE;
        }
        //		return m_idActTarget != ID_NONE;
        return m_idMoveTarget != ID_NONE;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
bool CNpc::SendMsg(CNetMsg* pMsg)
{
    return NpcWorld()->SendMsg(GetID(), pMsg);
}

//////////////////////////////////////////////////////////////////////
bool CNpc::SendMsg(LPCTSTR szMsg)
{
    CHECKF(szMsg);
    CMsgTalk	msg;
    if (msg.Create(GetStr(NPCTYPEDATA_NAME), ALLUSERS_NAME, szMsg))
    {
        return NpcWorld()->SendMsg(GetID(), &msg);
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
bool CNpc::JumpBlock(int x, int y, int dir)
{
    int nSteps = Distance(x, y);
    //	ASSERT(nSteps <= GUARD_LEAVEDISTANCE*2);
    if (nSteps <= 0)
    {
        return true;
    }
    for(int i = 1; i < nSteps + 1; i++)
    {
        POINT pos;
        pos.x = GetPosX() + (x - GetPosX()) * i / nSteps;
        pos.y = GetPosY() + (y - GetPosY()) * i / nSteps;
        if (GetMap()->IsStandEnable(pos.x, pos.y))
        {
            JumpPos(pos.x, pos.y, dir);
            return true;
        }
    }
    if (GetMap()->IsStandEnable(x, y))
    {
        JumpPos(x, y, dir);
        return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
bool CNpc::FarJump(int x, int y, int dir)
{
    int nSteps = Distance(x, y);
    //	ASSERT(nSteps <= GUARD_LEAVEDISTANCE*2);
    if (nSteps <= 0)
    {
        return true;
    }
    if (GetMap()->IsStandEnable(x, y))
    {
        JumpPos(x, y, dir);
        return true;
    }
    for(int i = 1; i < nSteps + 1; i++)
    {
        POINT pos;
        pos.x = x + (GetPosX() - x) * i / nSteps;
        pos.y = y + (GetPosY() - y) * i / nSteps;
        if (GetMap()->IsStandEnable(pos.x, pos.y))
        {
            JumpPos(pos.x, pos.y, dir);
            return true;
        }
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
bool CNpc::IsOpposedSyn(OBJID idSyn)
{
    CHECKF(IsSynPet());
    CSyndicate* pSyn = SynManager()->QuerySyndicate(GetOwnerID());
    CHECKF(pSyn);
    if (GetMap()->IsWarTime())
    {
        return !pSyn->IsFriendly(idSyn);
    }
    else
    {
        return pSyn->IsHostile(idSyn);
    }
}

/*/////////////////////////////////////////////////////////////////////
bool CNpc::IsAttackEnable(IRole* pRole)
{
	int	nDistance	= GetInt(NPCTYPEDATA_VIEWRANGE);
	int	nAtkDistance = ::CutOverflow(nDistance, GetAttackRange(pRole->GetSizeAdd()));
	if (pRole->IsLurker())
	{
		if (GetAttackRange(0) > 1 && Distance(pRole->GetPosX(), pRole->GetPosY()) <= nAtkDistance)
			return false;
	}
	else if (nDist < nDistance)
		return false;

	if (pRole->IsWing() && !IsWing() && IsCloseAttack())
		return false;

	return true;
}*/

//////////////////////////////////////////////////////////////////////
void CNpc::LockMove(int nMilliSecs)
{
    m_tMove.IncInterval(nMilliSecs, nMilliSecs);
}

//////////////////////////////////////////////////////////////////////
bool CNpc::IsAttackEnable(IRole* pTarget)
{
    if (pTarget->IsWing() && !IsWing() && IsCloseAttack())
    {
        return false;
    }
    //	if (pTarget->GetEffect()&KEEPEFFECT_INSTEAD)
    //		return false;
    return true;
}

bool CNpc::IsActive()
{
    return ((GetEffect() & KEEPEFFECT_FREEZE) == 0
            && (GetEffect()&KEEPEFFECT_FAINT) == 0);
}
