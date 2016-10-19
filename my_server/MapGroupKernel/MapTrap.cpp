
//**********************************************************
// 代码编辑器
//**********************************************************

// MapTrap.cpp: implementation of the CMapTrap class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "MapTrap.h"
#include "MapGroup.h"

MYHEAP_IMPLEMENTATION(CMapTrap, s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMapTrap::CMapTrap()
{
    SetObjType(OBJ_TRAP);
    memset(&m_info, 0, sizeof(m_info));
    //m_tFight;
    m_bDie			= false;
    m_nTimes		= 0;
    m_pType			= NULL;
    m_pBattleSystem	= NULL;
    m_tLifePeriod.Clear();
    //	m_pData			= NULL;
}

//////////////////////////////////////////////////////////////////////
CMapTrap::~CMapTrap()
{
    if (m_pBattleSystem)
    {
        delete m_pBattleSystem;
    }
    SAFE_RELEASE (m_setStatus);
    if (m_pMap)
    {
        m_pMap->LeaveRoom(QueryMapThing());
        ClrBroadcastSet();
    }
    //	if (m_pData)
    //		m_pData->Release();
    if (IsDynaTrapID(GetID()))
    {
        MapManager()->RecycleTrapID(GetID());
    }
}

//////////////////////////////////////////////////////////////////////
bool CMapTrap::Init()
{
    m_nTimes	= m_pType->GetInt(TRAPTYPEDATA_TIMES);
    m_setStatus	= CStatusSet::CreateNew(true);
    CHECKF(m_setStatus);
    m_pBattleSystem	= new CBattleSystem(this->m_idProcess, QueryRole());
    CHECKF(m_pBattleSystem);
    // enter map
    m_pMap	= MapManager()->QueryMap(m_info.idMap);
    CHECKF(m_pMap);
    m_pMap->EnterRoom(QueryMapThing());
    UpdateBroadcastSet();
    return true;
}

//////////////////////////////////////////////////////////////////////
CMapTrap* CMapTrap::CreateNew(PROCESS_ID idProcess, TRAP_INFO* pInfo, CUser* pUser/*= NULL*/)			// 玩家放火墙类
{
    CMapTrap*	pTrap	= new CMapTrap;
    CHECKF(pTrap);
    pTrap->m_idProcess	= idProcess;
    pTrap->m_pType	= TrapTypeSet()->GetObj(pInfo->usType);
    CHECKF(pTrap->m_pType);
    // info
    memcpy(&pTrap->m_info, pInfo, sizeof(pTrap->m_info));
    ASSERT(pTrap->m_info.id == ID_NONE);
    pTrap->m_info.id	= MapGroup(idProcess)->GetMapManager()->SpawnNewTrapID();
    pTrap->m_info.usLook	= ::MakeLookFace(::MaskLook(pTrap->m_pType->GetInt(TRAPTYPEDATA_LOOK)), ::MaskDir(pTrap->m_info.usLook));
    CHECKF(pTrap->Init());
    if (pUser)
    {
        pTrap->m_pUser	= pUser->QueryLink();
    }
    // synchro
    CMsgMapItem	msg;
    IF_OK(msg.Create(MSGMAPITEM_CAST_TRAP, pTrap))
    pTrap->BroadcastRoomMsg(&msg);
    return pTrap;
}

//////////////////////////////////////////////////////////////////////
CMapTrap* CMapTrap::CreateNew(PROCESS_ID idProcess, IRecord* pRes)			// 系统类
{
    CMapTrap*	pTrap	= new CMapTrap;
    CHECKF(pTrap);
    pTrap->m_idProcess	= idProcess;
    if (!MapGroup(idProcess)->GetMapManager()->GetGameMap(pRes->GetInt(TRAPDATA_MAPID)))		//? Can't combine to Init()
    {
        pTrap->ReleaseByOwner();
        return NULL;		//? may be other map group
    }
    pTrap->m_pType	= TrapTypeSet()->GetObj(pRes->GetInt(TRAPDATA_TYPE));
    if (!pTrap->m_pType)
    {
        LOGERROR("invalid trap type %d in %d", pRes->GetInt(TRAPDATA_TYPE), pRes->GetInt(TRAPDATA_ID_));
        return NULL;
    }
    // info
    pTrap->m_info.id			= pRes->GetInt(TRAPDATA_ID_);
    pTrap->m_info.usType		= pRes->GetInt(TRAPDATA_TYPE);
    pTrap->m_info.usLook		= pRes->GetInt(TRAPDATA_LOOK);
    pTrap->m_info.idOwner		= pRes->GetInt(TRAPDATA_OWNERID);
    pTrap->m_info.idMap			= pRes->GetInt(TRAPDATA_MAPID);
    pTrap->m_info.usPosX		= pRes->GetInt(TRAPDATA_POS_X);
    pTrap->m_info.usPosY		= pRes->GetInt(TRAPDATA_POS_Y);
    CHECKF(pTrap->Init());
    return pTrap;
}

//////////////////////////////////////////////////////////////////////
bool CMapTrap::ChangeType(int nNewType)
{
    m_pType	= TrapTypeSet()->GetObj(nNewType);
    CHECKF(m_pType);
    // info
    m_info.usType	= nNewType;
    // synchro
    CMsgMapItem	msg;
    IF_OK(msg.Create(MSGMAPITEM_CAST_TRAP, this))
    BroadcastRoomMsg(&msg);
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CMapTrap::ChangeLook(int nNewLook)
{
    // info
    m_info.usLook	= nNewLook;
    // synchro
    CMsgMapItem	msg;
    IF_OK(msg.Create(MSGMAPITEM_CAST_TRAP, this))
    BroadcastRoomMsg(&msg);
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMapTrap::SendShow(IRole* pRole)							// 向玩家发送物件外形数据
{
    CMsgMapItem	msg;
    IF_OK(msg.Create(MSGMAPITEM_SYNCHRO_TRAP, this))
    pRole->SendMsg(&msg);
}

//////////////////////////////////////////////////////////////////////
void CMapTrap::BroadcastRoomMsg(CNetMsg* pMsg, bool bSendSelf /*= false*/)
{
    CRole::BroadcastRoomMsg(pMsg, bSendSelf);
    //	CHECK(GetMap());
    //	GetMap()->BroadcastBlockMsg(QueryMapThing(), pMsg, bSendSelf);
}

//////////////////////////////////////////////////////////////////////
void CMapTrap::BroadcastRoomMsg(LPCTSTR szMsg, bool bSendSelf /*= false*/)
{
    CMsgTalk	msg;
    if (msg.Create(GetName(), ALLUSERS_NAME, szMsg, NULL, 0xff0000, _TXTATR_SYSTEM))
    {
        this->BroadcastRoomMsg(&msg, bSendSelf);
    }
}

//////////////////////////////////////////////////////////////////////
void CMapTrap::OnTimer(time_t tCurr)
{
    if (!IsAlive())
    {
        return ;
    }
    if (m_tLifePeriod.IsActive())
    {
        if (m_tLifePeriod.ToNextTime())
        {
            DelTrap();
            m_tLifePeriod.Clear();
        }
    }
    if (IsAutoSort())
    {
        // 自动攻击 ------------------------------------------------------
        DEBUG_TRY
        if (/*m_pBattleSystem && m_pBattleSystem->IsActived() &&*/ m_tFight.ToNextTick(GetInterAtkRate()))		// + m_nFightPause
        {
            if (m_pType->GetInt(TRAPTYPEDATA_TIMES))			// 0: forever effect
            {
                --m_nTimes;
            }
            MagicAttack();
            // m_pBattleSystem->ProcAttack_Hand2Hand();
            //m_nFightPause	= 0;
            if (m_pType->GetInt(TRAPTYPEDATA_TIMES))			// 0: forever effect
            {
                if (m_nTimes <= 0)
                {
                    DelTrap();
                }
            }
        }
        DEBUG_CATCH("CMapTrap ProcAttack_Hand2Hand");
    }
}

//////////////////////////////////////////////////////////////////////
void CMapTrap::SaveInfo	()
{
}

//////////////////////////////////////////////////////////////////////
bool CMapTrap::DelTrap()
{
    CHECKF(!IsDeleted());
    // synchro
    CMsgMapItem	msg;
    IF_OK(msg.Create(MSGMAPITEM_DROP_TRAP, this))
    BroadcastRoomMsg(&msg);
    m_bDie	= true;
    return true;
}

//////////////////////////////////////////////////////////////////////
// attack
//////////////////////////////////////////////////////////////////////
void CMapTrap::TrapAttack(IRole* pTarget)
{
    CHECK(IsTrapSort());
    if (!m_tFight.ToNextTick(GetInterAtkRate()))
    {
        return ;
    }
    if (m_pType->GetInt(TRAPTYPEDATA_TIMES))			// 0: forever effect
    {
        --m_nTimes;
    }
    if (!pTarget->IsBeAtkable())
    {
        return ;
    }
    if (m_pUser && m_pUser->IsImmunity(pTarget))
    {
        return ;
    }
    int nAtkMode = m_pType->GetInt(TRAPTYPEDATA_ATKMODE);
    void* pTemp;
    if (!( (nAtkMode & ATKMODE_USER) && pTarget->QueryObj(OBJ_USER, &pTemp)
            || (nAtkMode & ATKMODE_MONSTER) && pTarget->QueryObj(OBJ_MONSTER, &pTemp) ))
    {
        return ;
    }
    if (m_pUser && !pTarget->IsEvil())
    {
        m_pUser->SetCrimeStatus();
    }
    if ((nAtkMode & ATKMODE_NOATTACK) == 0)
    {
        m_pBattleSystem->CreateBattle(pTarget->GetID());
        m_pBattleSystem->ProcAttack_Hand2Hand();
    }
    OBJID idAction = m_pType->GetInt(TRAPTYPEDATA_ACTION);
    if (idAction != ID_NONE)
    {
        if ((nAtkMode & ATKMODE_USER))
        {
            CUser* pUser = NULL;
            pTarget->QueryObj(OBJ_USER, IPP_OF(pUser));
            GameAction()->ProcessAction(idAction, pUser, NULL);
        }
        else if (nAtkMode & ATKMODE_MONSTER)
        {
            CMonster* pMonster = NULL;
            pTarget->QueryObj(OBJ_MONSTER, IPP_OF(pMonster));
            GameAction()->ProcessAction(idAction, NULL, pMonster->QueryRole());
        }
    }
    if (m_pType->GetInt(TRAPTYPEDATA_TIMES))			// 0: forever effect
    {
        if (m_nTimes <= 0)
        {
            DelTrap();
        }
    }
}

//////////////////////////////////////////////////////////////////////
void CMapTrap::MagicAttack	()
{
    CHECK(IsAutoSort());
    switch(m_pType->GetInt(TRAPTYPEDATA_SORT))
    {
    case	TRAPSORT_FIREWALL:
        {
            ProcessFireWall();
        }
        break;
    }
}

//////////////////////////////////////////////////////////////////////
bool CMapTrap::IsAtkable(IRole* pTarget, bool bSendHint)
{
    if (!pTarget)
    {
        return false;
    }
    if (!this->IsAlive())
    {
        if (bSendHint)
        {
            this->SendSysMsg(STR_DIE_FORGIVE);
        }
        return false;
    }
    if (!pTarget->IsAlive())
    {
        //if (bSendHint)
        //	this->SendSysMsg(STR_DIE_STOP_ATTACK);
        return false;
    }
    /*
    if (pTarget->IsGM())
    {
    	if (bSendHint)
    		this->SendSysMsg(STR_ATTACK_POLICE);
    	return false;
    }
    */
    if (GetID() == pTarget->GetID())
    {
        return false;
    }
    if (!pTarget->IsBeAtkable())
    {
        return false;
    }
    // map check
    if (!(this->GetMap() && pTarget->GetMap() && this->GetMap() == pTarget->GetMap()))
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
int	 CMapTrap::Attack(IRole* pTarget)
{
    CHECKF(pTarget);
    int nTempAdjust = 0;
    int nDamage = CBattleSystem::CalcPower(IsSimpleMagicAtk(), QueryRole(), pTarget, nTempAdjust, true);
    // 目标护盾状态转移伤害
    if (nDamage > 0 && pTarget->TransferShield(IsSimpleMagicAtk(), QueryRole(), nDamage))
    {
    }
    else
    {
        int nLoseLife = ::CutOverflow(nDamage, (int)pTarget->GetLife());
        if (nLoseLife > 0)
        {
            pTarget->AddAttrib(_USERATTRIB_LIFE, -1 * nLoseLife, SYNCHRO_TRUE);
        }
        pTarget->BeAttack(IsSimpleMagicAtk(), QueryRole(), nDamage);
    }
    return nDamage;
}

//////////////////////////////////////////////////////////////////////
void CMapTrap::SendDamageMsg(OBJID idTarget, int nDamage)
{
    if (IsSimpleMagicAtk())
    {
        CMsgMagicEffect	msg;
        IF_OK(msg.Create(GetID(), m_pType->GetInt(TRAPTYPEDATA_MAGICTYPE), 0, idTarget, nDamage, GetDir()))
        BroadcastRoomMsg(&msg, INCLUDE_SELF);
    }
    else if (IsBowman())
    {
        CMsgInteract msg;
        IF_OK(msg.Create(INTERACT_SHOOT, GetID(), idTarget, GetPosX(), GetPosY(), nDamage))
        BroadcastRoomMsg(&msg, INCLUDE_SELF);
    }
    else
    {
        CMsgInteract msg;
        IF_OK(msg.Create(INTERACT_ATTACK, GetID(), idTarget, GetPosX(), GetPosY(), nDamage))
        BroadcastRoomMsg(&msg, INCLUDE_SELF);
    }
}

//////////////////////////////////////////////////////////////////////
void CMapTrap::Kill(IRole* pTarget, DWORD dwDieWay)
{
    CHECK(pTarget);
    if (m_pUser)
    {
        m_pUser->Kill(pTarget, dwDieWay);			// 转玩家杀人
        return ;
    }
    CMsgInteract msg;
    IF_OK(msg.Create(INTERACT_KILL, this->GetID(), pTarget->GetID(), pTarget->GetPosX(), pTarget->GetPosY(), dwDieWay))
    pTarget->BroadcastRoomMsg(&msg, INCLUDE_SELF);
#ifdef	LOCAL_DEBUG
    CMonster* pMonster;
    if (pTarget->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)))
    {
        LOGERROR("陷阱[%u]杀死了[%s][%d]", GetID(), pMonster->GetName(), pMonster->GetType());
    }
#endif
    pTarget->BeKill(this->QueryRole());
}

//////////////////////////////////////////////////////////////////////
// process user magic
//////////////////////////////////////////////////////////////////////
void CMapTrap::ProcessFireWall()
{
    int	 nRange		= m_pType->GetInt(TRAPTYPEDATA_SIZE) / 2;
    bool bVertical	= ((m_info.usLook % 4) / 2) == 0;		// 0:  horizontal, 1: vertical
    // 其它MapThing
    int x, y, z;
    CGameMap* pMap = GetMap();
    FOR_9_BLOCKTHINGS(pMap, GetPosX(), GetPosY())
    {
        IMapThing* pTarget = pMap->QueryThingByIndex(x, y, z);
        void* pTemp;
        IRole* pRole = NULL;
        if (pTarget && GetDistance(pTarget) < nRange && pTarget->QueryRole(IPP_OF(pRole))
                && !pTarget->QueryObj(OBJ_TRAP, &pTemp) && !pTarget->QueryObj(OBJ_NPC, &pTemp))
        {
            if (!pRole->IsBeAtkable())
            {
                continue;
            }
            if (m_pUser && m_pUser->IsImmunity(pRole))
            {
                continue;
            }
            if (!bVertical && pTarget->GetPosX() == GetPosX()
                    || bVertical && pTarget->GetPosY() == GetPosY() )
            {
                if (m_pUser && !pRole->IsEvil())
                {
                    m_pUser->SetCrimeStatus();
                }
                m_pBattleSystem->CreateBattle(pTarget->GetID());
                m_pBattleSystem->ProcAttack_Hand2Hand();
            }
        }
    }
}


//设置存在的时间 ： S
void CMapTrap::SetLifePeriod(UINT uiLifePeriod)
{
    m_tLifePeriod.Startup(uiLifePeriod);
}
