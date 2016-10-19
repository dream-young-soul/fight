
//**********************************************************
// 代码编辑器
//**********************************************************

// Magic.cpp: implementation of the CMagic class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "GameObj.h"
#include "Magic.h"
#include "MapGroup.h"
#include "StatusOnce.h"
#include "StatusMore.h"
#include "RoleManager.h"
//#include "UserTable.h"
#include "AiNpc.h"

char			szMagicTable[] = _TBL_MAGIC;
const OBJID		MIN_TEMPORARY_MAGICID	= (DWORD)40 * 10000 * 10000 + 1;
long			CMagic::m_lockNextID	= (long)MIN_TEMPORARY_MAGICID;		// temporary magic id, use for "next magic" id.
//////////////////////////////////////////////////////////////////////
unsigned char	Prof2MaskTable[] =
{
    10,			// 魔法师
    11,			// 魔法师转职
    20,			// 战士
    21,			// 战士转职
    30,			// 异能者
    31,			// 异能者转职
};

const int	MAX_USER_PROFS = sizeof(Prof2MaskTable) / sizeof(Prof2MaskTable[0]);

//////////////////////////////////////////////////////////////////////
const DWORD MASK_ALLPROF	= 0;
DWORD	Prof2Mask(int nProf)		// return ERROR_MASK : error
{
    for(int i = 0; i < MAX_USER_PROFS; i++)
        if (Prof2MaskTable[i] == nProf)
        {
            return 1 << i;
        }
    return 0;				// 其它职业
}

MYHEAP_IMPLEMENTATION(CMagicData, s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMagicData::~CMagicData()
{
    SAFE_RELEASE (m_pData);
}

//////////////////////////////////////////////////////////////////////
IMagicData* IMagicData::CreateNew()
{
    // 缺省是创建CMagicData
    return CMagicData::CreateNew();
}

//////////////////////////////////////////////////////////////////////
bool CMagicData::Create(IRecordset* pDefaultRecord, OBJID idNewKey)
{
    m_pData	= CMagicDataBase::CreateNew();
    if (m_pData && m_pData->Create(pDefaultRecord, idNewKey))
    {
        return true;
    }
    SAFE_RELEASE (m_pData);
    return false;
}

//////////////////////////////////////////////////////////////////////
bool CMagicData::Create(IRecordset* pRecordset)
{
    m_pData	= CMagicDataBase::CreateNew();
    if (m_pData && m_pData->Create(pRecordset))
    {
        SetDelay();
        return true;
    }
    SAFE_RELEASE (m_pData);
    return false;
}

//////////////////////////////////////////////////////////////////////
bool CMagicData::Create(ST_MAGIC_DATA* pMagicData)
{
    ASSERT(!"CMagicData::Create(ST_MAGIC_DATA* pMagicData)");
    return false;
}

//////////////////////////////////////////////////////////////////////
int CMagicData::GetInt(MAGICDATA nDataName)
{
    //	CHECKF(m_pType);
    CHECKF (m_pData);
    if (m_pType == NULL)
    {
        IF_NOT(FindType())
        {
            LOGERROR("魔法找不到类型。type[%d],level[%d]", m_pData->GetInt(MAGICDATA_TYPE), m_pData->GetInt(MAGICDATA_LEVEL));
            return false;
        }
    }
    if (nDataName < MAGICTYPEDATA_OFFSET)
    {
        return m_pData->GetInt(nDataName);
    }
    return m_pType->GetInt((MAGICTYPEDATA)(nDataName - MAGICTYPEDATA_OFFSET));
}

//////////////////////////////////////////////////////////////////////
void CMagicData::SetInt(MAGICDATA idx, int nData)
{
    ASSERT(idx < MAGICTYPEDATA_OFFSET);
    m_pData->SetInt(idx, nData);
    if (idx == MAGICDATA_LEVEL)
    {
        CHECK(FindType());
    }
}

//////////////////////////////////////////////////////////////////////
bool CMagicData::FindType()
{
    m_pType = CMagic::FindMagicType(m_pData->GetInt(MAGICDATA_TYPE), m_pData->GetInt(MAGICDATA_LEVEL));
    return (m_pType != NULL);
}

//////////////////////////////////////////////////////////////////////
// 本来因该在MagicData.cpp中实现的
// 由于旧有代码的头文件包含比较复杂，暂时简单处理写在这里
bool CMagicTempData::FindType()
{
    m_pType = CMagic::FindMagicType(m_data.usType, m_data.usLevel);
    return (m_pType != NULL);
}

//////////////////////////////////////////////////////////////////////
//int CMagicData::GetApplyMS()
//{
//	int nApplyMS = 0;
//	if (this->GetSTrackAmount() > 0)
//	{
//		for (int i=0; i<this->GetSTrackAmount(); i++)
//		{
//			const ST_TRACK&	track = this->GetSTrackByIndex(i);
//			nApplyMS += track.nApplyMS;
//		}
//	}
//	else
//		nApplyMS = GetInt(MAGICDATA_APPLY_MS);
//
//	return nApplyMS;
//}

//////////////////////////////////////////////////////////////////////
//int CMagicData::GetLockSecs()
//{
//	return (GetApplyMS()/1000+1);
//}

MYHEAP_IMPLEMENTATION(CMagic, s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMagic::CMagic(PROCESS_ID idProcess, IRole* pRole)
    : m_nMagicState(MAGICSTATE_NONE), m_nApplyTimes(0), m_bTargetLocked(false)
{
    ASSERT (pRole);
    m_idProcess = idProcess;
    m_pOwner		= pRole;
    m_setData	= CMagicSet::CreateNew(true);
    m_pMagic	= NULL;
    //m_nRawDelay = MAGIC_DELAY - MAGIC_DECDELAY_PER_LEVEL*m_pOwner->GetProfessionLevel();
    //ASSERT(m_nRawDelay >= 500);
    m_nRawDelay = 800;
    m_tDelay.SetInterval(m_nRawDelay);
    m_nDelay	= m_nRawDelay;//MulDiv(m_nRawDelay, AUTO_MAGIC_DELAY_PERCENT, 100);
    m_tIntone.Clear();
    m_tApply.Clear();
}

//////////////////////////////////////////////////////////////////////
CMagic::~CMagic()
{
    if (m_setData)
    {
        m_setData->Release();
    }
}

//////////////////////////////////////////////////////////////////////
int CMagic::GetDieMode()
{
    return HitByMagic() ? DIE_MAGIC : (m_pOwner->IsBowman() ? DIE_ARROWSHOOT : DIE_NORMAL);
}

//////////////////////////////////////////////////////////////////////
CMagic* CMagic::CreateNew(PROCESS_ID idProcess, IRole* pRole)
{
    CHECKF (pRole);
    CMagic* pMagic = new CMagic(idProcess, pRole);
    CHECKF(pMagic);
    pMagic->m_idProcess = idProcess;
    pMagic->m_pOwner		= pRole;
    return pMagic;
}

//////////////////////////////////////////////////////////////////////
// login
//////////////////////////////////////////////////////////////////////
bool CMagic::CreateAll()
{
    CHECKF(CMapGroup::IsValidMapGroupProcessID(m_idProcess));
    CHECKF(m_setData);
    CHECKF(m_pOwner);
    SQLBUF	szSQL;
#ifdef	PALED_DEBUG_X
    sprintf(szSQL, "SELECT * FROM %s WHERE ownerid=%u && unlearn=0 XXXXXXXX", _TBL_MAGIC, m_pOwner->GetID());
#else
    sprintf(szSQL, "SELECT * FROM %s WHERE ownerid=%u && unlearn=0", _TBL_MAGIC, m_pOwner->GetID());
#endif
    CHECKF(m_setData->Create(szSQL, Database()));
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::SendInfoToClient()
{
    for(int i = m_setData->GetAmount() - 1; i >= 0; i--)
    {
        IMagicData* pData = m_setData->GetObjByIndex(i);
        if (pData && pData->GetInt(MAGICDATA_TYPE))
        {
            if (!IsWeaponMagic(pData->GetInt(MAGICDATA_TYPE)))
            {
                if (CMagic::FindMagicType(pData->GetInt(MAGICDATA_TYPE), pData->GetInt(MAGICDATA_LEVEL)))
                {
                    // 含AUTOACTIVE_HIDDEN掩码的魔法隐藏不显示
                    if ((pData->GetInt(MAGICDATA_AUTO_ACTIVE) & AUTOACTIVE_HIDDEN) == 0)
                    {
                        CMsgMagicInfo msg;
                        IF_OK(msg.Create(m_pOwner->GetID(),
                                         pData->GetInt(MAGICDATA_TYPE),
                                         pData->GetInt(MAGICDATA_LEVEL),
                                         pData->GetInt(MAGICDATA_EXP)))
                        m_pOwner->SendMsg(&msg);
                    }
                }
                else
                {
                    LOGWARNING("角色学习了非法魔法: [%s][%d].", m_pOwner->GetName(), m_pOwner->GetID());
                    m_setData->DelObj(pData->GetID());
                }
            }
        }
        else
        {
            LOGWARNING("角色学习了非法魔法: [%s][%d]", m_pOwner->GetName(), m_pOwner->GetID());
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
// MapGroup
//////////////////////////////////////////////////////////////////////
bool CMagic::SaveInfo()
{
    for(int i = 0; i < m_setData->GetAmount(); i++)
    {
        IMagicData* pData = m_setData->GetObjByIndex(i);
        IF_OK(pData)
        {
            if (!IsWeaponMagic(pData->GetInt(MAGICDATA_TYPE)))
            {
                pData->Update();
            }
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::GetInfoByIndex(MagicInfoStruct* pInfo, int idx)
{
    CHECKF(pInfo);
    CHECKF(idx >= 0 && idx < m_setData->GetAmount());
    IMagicData* pData = m_setData->GetObjByIndex(idx);
    IF_OK(pData)
    {
        pInfo->id		= pData->GetID();
        pInfo->idOwner	= m_pOwner->GetID();
        pInfo->nType	= pData->GetInt(MAGICDATA_TYPE);
        pInfo->nLevel	= pData->GetInt(MAGICDATA_LEVEL);
        pInfo->dwExp	= pData->GetInt(MAGICDATA_EXP);
        return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::AppendMagic(const MagicInfoStruct* pInfo)
{
    IMagicData* pData = NULL;
    if (pInfo->id >= MIN_TEMPORARY_MAGICID)
    {
        pData = CMagicTempData::CreateNew();
        CHECKF (pData);
        ST_MAGIC_DATA	data;
        memset(&data, 0L, sizeof(ST_MAGIC_DATA));
        data.idMagic	= pInfo->id;
        IF_NOT (pData->Create(&data))
        return false;
    }
    else
    {
        pData = CMagicData::CreateNew();
        CHECKF(pData);
        IF_NOT(pData->Create(GameDataDefault()->GetMagicData(), pInfo->id))
        return false;
    }
    pData->SetInt(MAGICDATA_OWNERID, pInfo->idOwner);
    pData->SetInt(MAGICDATA_TYPE, pInfo->nType);
    pData->SetInt(MAGICDATA_LEVEL, pInfo->nLevel);
    pData->SetInt(MAGICDATA_EXP, pInfo->dwExp);
    pData->ClearUpdateFlags();
    pData->FindType();
    m_setData->AddObj(pData);
    return true;
}

//////////////////////////////////////////////////////////////////////
// const
//////////////////////////////////////////////////////////////////////
bool CMagic::IsImmunity(IRole* pRole)
{
    CHECKF(m_pMagic);
    CHECKF(pRole);
    if (!m_pOwner->IsAtkable(pRole))
    {
        return true;
    }
    if (pRole->IsWing() && !m_pOwner->IsWing() && m_pMagic->GetInt(MAGICDATA_WEAPON_HIT) && m_pMagic->GetInt(MAGICDATA_WEAPON_SUBTYPE) != BOW_SUBTYPE)
    {
        return true;
    }
    return m_pOwner->IsImmunity(pRole);
}

//////////////////////////////////////////////////////////////////////
// application
//////////////////////////////////////////////////////////////////////
IMagicData* CMagic::FindMagic(int nType)
{
    for(int i = 0; i < m_setData->GetAmount(); i++)
    {
        IMagicData* pData = m_setData->GetObjByIndex(i);
        if (pData && pData->GetInt(MAGICDATA_TYPE) == nType)
        {
            return pData;
        }
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////
CMagicTypeData* CMagic::FindMagicType(int nType, int nLevel)
{
    for(int i = 0; i < MagicTypeSet()->GetAmount(); i++)
    {
        CMagicTypeData* pType = MagicTypeSet()->GetObjByIndex(i);
        if (pType && pType->GetInt(MAGICTYPEDATA_TYPE) == nType && pType->GetInt(MAGICTYPEDATA_LEVEL) == nLevel)
        {
            return pType;
        }
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::AwardExpOfLife(IRole* pTarget, int nLifeLost, BOOL bMagicRecruit/*=false*/)
{
    CNpc* pNpc;
    pTarget->QueryObj(OBJ_NPC, IPP_OF(pNpc));
    if (pNpc && pNpc->IsAwardScore() && !bMagicRecruit)
    {
        m_pOwner->AwardSynWarScore(pNpc, nLifeLost);
    }
    if (pTarget->IsMonster() || pNpc && pNpc->IsGoal())
    {
        // correct exp here
        int nExp = m_pOwner->AdjustExp(pTarget, nLifeLost);
        // kill bonus
        if (!pTarget->IsAlive() && !bMagicRecruit) // target was killed
        {
            int	nBonusExp	= pTarget->GetMaxLife() * KILLBONUS_PERCENT / 100;
            nExp += nBonusExp;
            // 有组队，并且队伍中有其他队员，则奖励其他队员经验
            OtherMemberAwardExp(pTarget, nBonusExp);
            if (!m_pOwner->GetMap()->IsTrainMap() && nBonusExp > 0)
            {
                m_pOwner->SendSysMsg(STR_KILLING_EXPERIENCE, nBonusExp);
            }
        }
        this->AwardExp(nExp);
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::AwardExp(int nType, int nBattleExp, int nExp)
{
    IMagicData* pData = FindMagic(nType);
    if (!pData)
    {
        return false;
    }
    IMagicData* pMagic = m_pMagic;
    m_pMagic	= pData;
    bool bRet = false;
    DEBUG_TRY
    bRet = AwardExp(nBattleExp, nExp, true);
    DEBUG_CATCH("CMagic::AwardExp(nBattleExp, nExp)")
    m_pMagic	= pMagic;
    return bRet;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::AwardExp(int nBattleExp, int nExp, bool bIgnoreFlag)
{
    CHECKF(m_pMagic);
    if (nExp <= 0)
    {
        return false;
    }
    if (m_pOwner->GetMap()->IsTrainMap())
    {
        //		if (nExp == AWARDEXP_BY_TIMES &&
        //			(m_pMagic->GetInt(MAGICDATA_SORT) == MAGICSORT_ATTACHSTATUS
        //				|| m_pMagic->GetInt(MAGICDATA_SORT)	== MAGICSORT_TEAM_MAGIC
        //				|| m_pMagic->GetInt(MAGICDATA_SORT) == MAGICSORT_TRANSFORM
        //				|| m_pMagic->GetInt(MAGICDATA_SORT) == MAGICSORT_ATKSTATUS
        //				))
        //		{
        //			if (::RandGet(20) == 0)
        //				nExp = AWARDEXP_BY_TIMES;
        //			else
        //				nExp = 0;
        //		}
        //		else
        //		{
        //			if (m_pOwner->IsBowman())
        //				nExp	/= 2;
        //			nExp = ::CutTrail(1, MulDiv(nExp, GOAL_EXP_PERCENT, 100));
        //		}
        if (nBattleExp)
        {
            if (m_pOwner->IsBowman())
            {
                nBattleExp	/= 2;
            }
            nBattleExp = ::CutTrail(1, MulDiv(nBattleExp, GOAL_EXP_PERCENT, 100));
        }
    }
    // 战斗经验值
    if (nBattleExp)
    {
        m_pOwner->AwardBattleExp(nBattleExp);
    }
    if (!CheckAwardExpEnable(m_pOwner->GetProfession()))
    {
        return false;
    }
    if (m_pMagic->GetInt(MAGICDATA_NEED_EXP) >= 0									// >=0 : 可以升级
            && ((m_pMagic->GetInt(MAGICDATA_AUTO_ACTIVE)&AUTOACTIVE_EXPDISABLE) == 0)	// 可以得到经验
            || bIgnoreFlag)
    {
        // ....
        int	nAllExp = m_pMagic->GetInt(MAGICDATA_EXP) + nExp;
        m_pMagic->SetInt(MAGICDATA_EXP, nAllExp);
        // inform client
        if ((m_pMagic->GetInt(MAGICDATA_AUTO_ACTIVE) & AUTOACTIVE_HIDDEN) == 0)
        {
            CMsgFlushExp	msg;
            IF_OK(msg.Create(MSGFLUSHEXP_MAGIC, m_pMagic->GetInt(MAGICDATA_TYPE), nAllExp))
            m_pOwner->SendMsg(&msg);
        }
        UpLevel(SYNCHRO_TRUE);
        return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::UpLevel(bool bSynchro)
{
    CHECKF(m_pMagic);
    CHECKF(!IsWeaponMagic(m_pMagic->GetInt(MAGICDATA_TYPE)));
    int	nNeedExp = m_pMagic->GetInt(MAGICDATA_NEED_EXP);
    if (!( nNeedExp > 0 		// nNeedExp > 0 : 可以升级
            && ( m_pMagic->GetInt(MAGICDATA_EXP) >= nNeedExp
                 || m_pMagic->GetInt(MAGICDATA_OLD_LEVEL)			// 顿悟
                 && m_pMagic->GetInt(MAGICDATA_LEVEL) >= m_pMagic->GetInt(MAGICDATA_OLD_LEVEL) / 2
                 && m_pMagic->GetInt(MAGICDATA_LEVEL) < m_pMagic->GetInt(MAGICDATA_OLD_LEVEL) ) ))
    {
        return false;    // 不用检查职业，因为职业不符不会长EXP
    }
    int	nNewLevel = m_pMagic->GetInt(MAGICDATA_LEVEL) + 1;
    m_pMagic->SetInt(MAGICDATA_EXP, 0);
    m_pMagic->SetInt(MAGICDATA_LEVEL, nNewLevel);
    m_pMagic->Update();
    if (bSynchro && (m_pMagic->GetInt(MAGICDATA_AUTO_ACTIVE) & AUTOACTIVE_HIDDEN) == 0)
    {
        CMsgMagicInfo msg;
        IF_OK(msg.Create(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), nNewLevel, 0))
        m_pOwner->SendMsg(&msg);
    }
    //	CHECKF(m_pMagic->FindType());
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::MagicAttack(int nType, OBJID idTarget, int x, int y, UCHAR ucAutoActive /*= 0*/)
{
    switch (m_nMagicState)
    {
    case MAGICSTATE_INTONE:
        {
#ifdef ZLONG_DEBUG
            m_pOwner->SendSysMsg("Debug: 魔法吟唱中被打断！准备使用新魔法。");
#endif
            AbortMagic(true);		// true: send faild msg
        }
        break;
    case MAGICSTATE_LAUNCH:
        {
#ifdef ZLONG_DEBUG
            m_pOwner->SendSysMsg("Debug: 魔法施展中，不能施展其它魔法。");
#endif
            return false;
        }
        break;
    case MAGICSTATE_DELAY:
        {
#ifdef ZLONG_DEBUG
            m_pOwner->SendSysMsg("Debug: 魔法延迟尚未结束，不能施展其他魔法。");
#endif
            //			AbortMagic(true);		// true: send faild msg
            return false;
        }
        break;
    }
    // 原来的被动技能是简单魔法攻击
    // 为了使修改后的被动技能在执行结束后继续自动攻击目标，这里不清除被攻击目标
    // 同时，CUser::ProcessAutoAttack中通过判断Magic的IsActive以避免魔法攻击的同时进行物理攻击
    // -- zlong 2005-01-04 注释说明
    //	m_pOwner->SetAttackTarget(NULL);
    m_bTargetLocked	= false;
    IMagicData* pData = FindMagic(nType);
    if (!( pData && (0 == ucAutoActive || pData->GetInt(MAGICDATA_AUTO_ACTIVE) & ucAutoActive )))
    {
        LOGCHEAT("invalid magic type: %d, user[%s][%d]", nType, m_pOwner->GetName(), m_pOwner->GetID());
        AbortMagic(true);		// true: send faild msg
        return false;
    }
    if (!CheckCondition(pData, idTarget, x, y))				//??? don't return false above this line.
    {
        if (pData->GetInt(MAGICDATA_SORT) == MAGICSORT_COLLIDE)
        {
            ProcessCollideFail(x, y, idTarget);		// idTarget: dir
        }
        else if (pData->GetInt(MAGICDATA_SORT) == MAGICSORT_JUMPATTACK)
        {
            IF_NOT(m_pOwner->GetDistance(x, y) <= pData->GetInt(MAGICDATA_DISTANCE))
            {
                m_pOwner->SendSysMsg(STR_INVALID_MSG);
                CUser* pUser = NULL;
                if (m_pOwner->QueryObj(OBJ_USER, IPP_OF(pUser)))
                {
                    UserManager()->KickOutSocket(pUser->GetSocketID(), "JUMPATTACK 太远!");
                }
                return false;
            }
            m_pMagic		= pData;
            ProcessJumpAttackFaild(x, y);
        }
        AbortMagic(true);		// true: send faild msg
        return false;
    }
    // set all member variable
    m_pMagic		= pData;
    m_idTarget		= idTarget;
    if (pData->GetInt(MAGICDATA_GROUND))
    {
        m_idTarget	= ID_NONE;
    }
    m_bAutoAttack	= true;
    if (MAGICSORT_COLLIDE == pData->GetInt(MAGICDATA_SORT))
    {
        m_nData	= (int)idTarget;
    }
    m_pos.x			= x;
    m_pos.y			= y;
    m_nMagicState	= MAGICSTATE_INTONE;
    m_tIntone.Startup(m_pMagic->GetInt(MAGICDATA_INTONE));
#ifdef ZLONG_DEBUG
    m_pOwner->SendSysMsg("Debug: 魔法吟唱开始……");
#endif
    // spend resource
    IRole* pRole = RoleManager()->QueryRole(m_idTarget);
    if (!m_pOwner->GetMap()->IsTrainMap())
    {
        if (m_pMagic->GetInt(MAGICDATA_USE_MP) > 0)
        {
            m_pOwner->AddAttrib(_USERATTRIB_MANA, -1 * m_pMagic->GetInt(MAGICDATA_USE_MP), SYNCHRO_TRUE);
        }
        if (m_pMagic->GetInt(MAGICDATA_USE_EP) > 0)
        {
            m_pOwner->AddAttrib(_USERATTRIB_ENERGY, -1 * m_pMagic->GetInt(MAGICDATA_USE_EP), SYNCHRO_TRUE);
        }
        if (pData->GetInt(MAGICDATA_USE_ITEM) && pData->GetInt(MAGICDATA_ITEM_NUM) > 0)
        {
            m_pOwner->SpendEquipItem(pData->GetInt(MAGICDATA_USE_ITEM), pData->GetInt(MAGICDATA_ITEM_NUM), SYNCHRO_TRUE);
        }
        if (m_pMagic->GetInt(MAGICDATA_USE_POTENTIAL) > 0)
        {
            m_pOwner->AddAttrib(_USERATTRIB_POTENTIAL, -1 * m_pMagic->GetInt(MAGICDATA_USE_POTENTIAL), SYNCHRO_TRUE);
        }
    }
    if (pRole && pData->GetInt(MAGICDATA_NEXT_MAGIC) == pData->GetInt(MAGICDATA_TYPE))
    {
        CUser* pTargetUser = NULL;
        if (pRole->QueryObj(OBJ_USER, IPP_OF(pTargetUser)))
        {
            m_bAutoAttack	= false;
        }
    }
    // broadcast msg
    if (!IsWeaponMagic(m_pMagic->GetInt(MAGICDATA_TYPE)))
    {
        CMsgInteract	msg;
        IF_OK(msg.Create(INTERACT_MAGICATTACK, m_pOwner->GetID(), idTarget, x, y, m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetInt(MAGICDATA_LEVEL)))
        m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
    }
    // equipment durability cost
    if (m_pMagic->GetInt(MAGICDATA_USE_MP) != 0)
    {
        if (!m_pOwner->GetMap()->IsTrainMap())
        {
            m_pOwner->DecEquipmentDurability(ATTACK_TIME, HitByMagic(), m_pMagic->GetInt(MAGICDATA_DURABILITY));
        }
        if (::RandGet(10) == 7)
        {
            m_pOwner->SendGemEffect();
        }
    }
    // launch immediately
    if (m_pMagic->GetInt(MAGICDATA_INTONE) <= 0)
    {
        m_tIntone.Clear();
#ifdef ZLONG_DEBUG
        m_pOwner->SendSysMsg("Debug: 魔法吟唱立刻结束，开始施展魔法……");
#endif
        m_nMagicState	= MAGICSTATE_LAUNCH;
        m_nApplyTimes	= __max(1, m_pMagic->GetSTrackAmount());
        if (!Launch())	// 魔法执行失败就结束——这里包括攻击前目标已经死亡、目标消失等情况
        {
            LockTarget(false);
            m_tApply.Clear();
            ResetDelay();
        }
        else
        {
            m_tApply.Startup(m_pMagic->GetApplyMS());
        }
        return true;
    }
    return true;	// 不会立即伤血
}

//////////////////////////////////////////////////////////////////////
void CMagic::BreakAutoAttack()
{
    m_bAutoAttack	= false;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::AbortMagic(bool bSynchro)
{
    // 魔法施展过程中禁止中止 —— 暂时这么设定
    if (m_nMagicState == MAGICSTATE_LAUNCH)
    {
#ifdef ZLONG_DEBUG
        m_pOwner->SendSysMsg("Debug: 魔法施放过程中，不能中止！");
#endif
        //		m_tApply.Clear();
        return false;
    }
    BreakAutoAttack();
    if (m_nMagicState == MAGICSTATE_DELAY)
    {
#ifdef ZLONG_DEBUG
        m_pOwner->SendSysMsg("Debug: 魔法延迟中。");
#endif
        //		m_tDelay.Clear();
        return false;
    }
    m_pMagic = NULL;
    if (m_nMagicState == MAGICSTATE_INTONE)
    {
        m_tIntone.Clear();
#ifdef ZLONG_DEBUG
        m_pOwner->SendSysMsg("Debug: 魔法吟唱被中止！");
#endif
    }
    m_nMagicState	= MAGICSTATE_NONE;
    if (bSynchro)
    {
        CMsgAction msg;
        if (msg.Create(m_pOwner->GetID(), 0, 0, 0, actionAbordMagic))
        {
            m_pOwner->SendMsg(&msg);
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMagic::ShowMiss()
{
    CHECK(m_pMagic);
    CMsgMagicEffect	msg;
    if (m_pMagic->GetInt(MAGICDATA_GROUND))
    {
        msg.CreateByPos(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetInt(MAGICDATA_LEVEL), m_pos.x, m_pos.y, m_pOwner->GetDir());
    }
    else
    {
        msg.Create(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetInt(MAGICDATA_LEVEL), m_idTarget, 0, m_pOwner->GetDir());
    }
    m_pOwner->BroadcastRoomMsg(&msg, true);
}

//////////////////////////////////////////////////////////////////////
bool CMagic::CheckCondition(IMagicData* pData, OBJID idTarget, int x, int y)
{
    // delay
    if (!m_tDelay.IsTimeOut(m_nDelay) && MAGICSORT_COLLIDE != pData->GetInt(MAGICDATA_SORT))
    {
        return false;
    }
    if (!pData->IsReady())		// 检查魔法的单独延迟时间
    {
        return false;
    }
    // 命中率判断 -- 满足下面条件的魔法不需要判断命中率
    if (!((pData->GetInt(MAGICDATA_AUTO_ACTIVE) & AUTOACTIVE_KILL) == AUTOACTIVE_KILL
            || (pData->GetInt(MAGICDATA_AUTO_ACTIVE) & AUTOACTIVE_RANDOM) == AUTOACTIVE_RANDOM))
    {
        if (::RandGet(100) >= pData->GetInt(MAGICDATA_PERCENT))
        {
            return false;
        }
    }
    //---jinggy---装备镶嵌的宝石技能---begin
    //检验技能条件是否需要  宝石类型
    int nNeedGemType = pData->GetInt(MAGICDATA_NEED_GEMTYPE);
    if (nNeedGemType)
    {
        //判断是否是嵌入玩家装备的宝石类型
        IF_NOT(m_pOwner->IsEmbedGemType(nNeedGemType))
        return false;
    }
    //---jinggy---装备镶嵌的宝石技能---end
    if (m_pOwner->GetMap()->IsInRegionType(REGION_PK_PROTECTED, m_pOwner->GetPosX(), m_pOwner->GetPosY()))
    {
        if (pData->GetInt(MAGICDATA_GROUND))
        {
            if (pData->GetInt(MAGICDATA_CRIME))
            {
                return false;
            }
        }
        else
        {
            IRole* pTarget = RoleManager()->QueryRole(idTarget);
            if (pTarget && pTarget->QueryOwnerUser() && pData->GetInt(MAGICDATA_CRIME))
            {
                return false;
            }
        }
    }
    // resource
    if (!m_pOwner->GetMap()->IsTrainMap())
    {
        if (m_pOwner->GetMana() < pData->GetInt(MAGICDATA_USE_MP))
        {
            return false;
        }
        if (m_pOwner->GetEnergy() < pData->GetInt(MAGICDATA_USE_EP))
        {
            return false;
        }
        // check use_item
        if (pData->GetInt(MAGICDATA_USE_ITEM))
        {
            if (!m_pOwner->CheckWeaponSubType(pData->GetInt(MAGICDATA_USE_ITEM), pData->GetInt(MAGICDATA_ITEM_NUM)))
            {
                return false;
            }
        }
        if (m_pOwner->GetPotential() < pData->GetInt(MAGICDATA_USE_POTENTIAL))
        {
            m_pOwner->SendSysMsg(STR_NOT_ENOUGH_POTENTIAL);
            return false;
        }
    }
    // 使用XP技能需要XPFULL状态
    if (pData->GetInt(MAGICDATA_USE_XP) == TYPE_XPSKILL)
    {
        IStatus* pStatus = m_pOwner->QueryStatus(STATUS_XPFULL);
        if (!pStatus)
        {
            return false;
        }
    }
    // with weapon
    if (pData->GetInt(MAGICDATA_WEAPON_SUBTYPE))
    {
        if (!m_pOwner->CheckWeaponSubType(pData->GetInt(MAGICDATA_WEAPON_SUBTYPE)))
        {
            return false;
        }
    }
    // check map
    int nSort = pData->GetInt(MAGICDATA_SORT);
    if ((nSort == MAGICSORT_CALLTEAMMEMBER || nSort == MAGICSORT_RECORDTRANSSPELL) && m_pOwner->GetMap()->IsFlyToDisable())
    {
        return false;
    }
    if (nSort == MAGICSORT_RECORDTRANSSPELL && m_pOwner->GetMap()->QueryRegion(REGION_CITY, m_pOwner->GetPosX(), m_pOwner->GetPosY()))
    {
        return false;
    }
    if (m_pOwner->QueryTransformation() && nSort != MAGICSORT_TRANSFORM)
    {
        return false;
    }
    if (m_pOwner->IsWing() && nSort == MAGICSORT_TRANSFORM)
    {
        return false;
    }
    //	if (!m_pOwner->GetMap()->IsWingEnable() && nSort == MAGICSORT_ATTACHSTATUS && pData->GetInt(MAGICDATA_STATUS) == STATUS_WING)
    //	{
    //		return false;
    //	}
    // check distance
    IRole* pRole = NULL;
    if (!pData->GetInt(MAGICDATA_GROUND))
    {
        pRole = m_pOwner->FindAroundRole(idTarget);		// may be NULL
        if (!pRole)
        {
            return false;    // not find target
        }
        if (!pRole->IsAlive() && nSort != MAGICSORT_DETACHSTATUS && nSort != MAGICSORT_SORB_SOUL)
        {
            return false;
        }
        if (nSort == MAGICSORT_ESCAPE)
        {
            if (pRole->GetLife() * 100 / pRole->GetMaxLife() >= MAGIC_ESCAPE_LIFE_PERCENT)
            {
                return false;
            }
        }
        if (nSort == MAGICSORT_SORB_SOUL)
        {
            CMonster* pMonster = NULL;
            if (!pRole->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)))
            {
                return false;
            }
            if ((pMonster->GetMaskData() & MASK_DISABLE_SORB_SOUL) == MASK_DISABLE_SORB_SOUL)
            {
                return false;
            }
        }
        if (nSort == MAGICSORT_STEAL_MONEY)
        {
            CMonster* pMonster = NULL;
            if (pRole->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)))
            {
                if ((pMonster->GetMaskData() & MASK_DISABLE_STEAL_MONEY) == MASK_DISABLE_STEAL_MONEY)
                {
                    return false;
                }
            }
        }
        x = pRole->GetPosX();
        y = pRole->GetPosY();
    }
    if (m_pOwner->GetDistance(x, y) > pData->GetInt(MAGICDATA_DISTANCE))
    {
        return false;
    }
    // check goal
    CNpc* pNpc = NULL;
    if (pRole)
    {
        pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc));
    }
    if (pNpc)
    {
        if (!pNpc->IsBeAtkable())
        {
            return false;
        }
        if (pNpc->IsGoal() && m_pOwner->GetLev() < pNpc->GetLev()
                || pNpc->GetType() != _MAGICGOAL_NPC)
        {
            return false;
        }
    }
    // check recruit
    if (pRole && pData->GetInt(MAGICDATA_SORT) == MAGICSORT_RECRUIT)
    {
        CMonster* pMonster = NULL;
        pRole->QueryObj(OBJ_MONSTER, IPP_OF(pMonster));
        if ((pNpc && !pNpc->IsGoal()) || pMonster)			// && pMonster->IsSynPet()
        {
            return false;
        }
    }
    if (pRole && m_pOwner->IsBowman() && !m_pOwner->IsArrowPass(x, y))
    {
        return false;
    }
    // 结界魔法只有队长才可以发动
    if (pData->GetInt(MAGICDATA_SORT) == MAGICSORT_TEAM_MAGIC)
    {
        CTeam* pTeam = m_pOwner->GetTeam();
        if (!pTeam || pTeam->GetLeader() != m_pOwner->GetID()
                || pTeam->GetMemberAmount() < TEAM_STATUS_REQ_ROLES)	// 发动界结，队伍中必须至少有3人
        {
            return false;
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::CheckCrime(IRole* pRole)
{
    CHECKF(m_pMagic);
    CHECKF(pRole);
    if (!m_pMagic->GetInt(MAGICDATA_CRIME))
    {
        return false;
    }
    return m_pOwner->CheckCrime(pRole);
}

//////////////////////////////////////////////////////////////////////
bool CMagic::CheckCrime(const vector<IRole*>* pSet)
{
    CHECKF(m_pMagic);
    if (!m_pMagic->GetInt(MAGICDATA_CRIME))
    {
        return false;
    }
    for(int i = 0; i < pSet->size(); i++)
    {
        if (m_pOwner->CheckCrime(pSet->at(i)))
        {
            return true;
        }
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
void CMagic::OwnerUpLevel(int nOwnerLev)
{
    //m_nRawDelay = MAGIC_DELAY - MAGIC_DECDELAY_PER_LEVEL*m_pOwner->GetProfessionLevel();
    //ASSERT(m_nRawDelay >= 500);
    m_tDelay.SetInterval(m_nRawDelay);
    m_nDelay	= m_nRawDelay;//MulDiv(m_nRawDelay, AUTO_MAGIC_DELAY_PERCENT, 100);
    if (m_setData->GetAmount() == 0)
    {
        return;
    }
    DWORD	dwProf = m_pOwner->GetProfession();
    for(int i = 0; i < m_setData->GetAmount(); i++)
    {
        IMagicData* pData = m_setData->GetObjByIndex(i);
        if (pData && pData->GetInt(MAGICDATA_NEED_EXP) == AUTOLEVELUP_EXP
                && pData->GetInt(MAGICDATA_USE_XP) != TYPE_KONGFU
                && nOwnerLev >= pData->GetInt(MAGICDATA_NEED_LEVEL)
                && this->CheckProfession(dwProf, pData->GetInt(MAGICDATA_NEED_PROFMASK))
                && !IsWeaponMagic(pData->GetInt(MAGICDATA_TYPE))
          )
        {
            int nNewLevel = pData->GetInt(MAGICDATA_LEVEL) + 1;
            pData->SetInt(MAGICDATA_LEVEL, nNewLevel);
            pData->Update();
            //			ASSERT(pData->FindType());
            if ((pData->GetInt(MAGICDATA_AUTO_ACTIVE) & AUTOACTIVE_HIDDEN) == 0)
            {
                CMsgMagicInfo	msg;
                IF_OK(msg.Create(m_pOwner->GetID(), pData->GetInt(MAGICDATA_TYPE), nNewLevel, 0))
                m_pOwner->SendMsg(&msg);
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////
void CMagic::UserWeaponSkillUpLevel(int nWeaponSkillType, int nWeaponSkillLevel)
{
    if (m_setData->GetAmount() == 0)
    {
        return;
    }
    DWORD	dwProf = m_pOwner->GetProfession();
    for(int i = 0; i < m_setData->GetAmount(); i++)
    {
        IMagicData* pData = m_setData->GetObjByIndex(i);
        if (pData && pData->GetInt(MAGICDATA_NEED_EXP) == AUTOLEVELUP_EXP
                && pData->GetInt(MAGICDATA_USE_XP) == TYPE_KONGFU
                && pData->GetInt(MAGICDATA_WEAPON_SUBTYPE) == nWeaponSkillType
                && nWeaponSkillLevel >= pData->GetInt(MAGICDATA_NEED_LEVEL)
                && this->CheckProfession(dwProf, pData->GetInt(MAGICDATA_NEED_PROFMASK))
                && !IsWeaponMagic(pData->GetInt(MAGICDATA_TYPE))
          )
        {
            int nNewLevel = pData->GetInt(MAGICDATA_LEVEL) + 1;
            pData->SetInt(MAGICDATA_LEVEL, nNewLevel);
            pData->Update();
            //			ASSERT(pData->FindType());
            if ((pData->GetInt(MAGICDATA_AUTO_ACTIVE) & AUTOACTIVE_HIDDEN) == 0)
            {
                CMsgMagicInfo	msg;
                IF_OK(msg.Create(m_pOwner->GetID(), pData->GetInt(MAGICDATA_TYPE), nNewLevel, 0))
                m_pOwner->SendMsg(&msg);
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////
bool CMagic::AutoLearnWeaponSkill(int nWeaponSkillType, int nWeaponSkillLevel, int nWeaponSkillExp/*=0*/)
{
    CMagicTypeData* pLearnMagic = NULL;
    for(int i = 0; i < AutoMagicTypeSet()->GetAmount(); i++)		//??? 可优化
    {
        CMagicTypeData* pData = AutoMagicTypeSet()->GetObjByIndex(i);
        if (pData && pData->GetInt(MAGICTYPEDATA_AUTO_LEARN)
                && pData->GetInt(MAGICTYPEDATA_WEAPON_SUBTYPE) == nWeaponSkillType
                && nWeaponSkillLevel >= pData->GetInt(MAGICTYPEDATA_LEARN_LEVEL) % 100		// %100: min level of need, 1208->max=12,min=8
                && FindMagic(pData->GetInt(MAGICTYPEDATA_TYPE)) == NULL )
        {
            int nMinLevel = pData->GetInt(MAGICTYPEDATA_LEARN_LEVEL) % 100;
            int nMaxLevel = pData->GetInt(MAGICTYPEDATA_LEARN_LEVEL) / 100;
            if (nMaxLevel == 0 || nWeaponSkillLevel >= nMaxLevel)
            {
                pLearnMagic = pData;
                break;
            }
            else if (nWeaponSkillExp > 0)
            {
                ASSERT(nMinLevel < nMaxLevel);
                int nExp = 0;
                for(int i = nMinLevel; i < nMaxLevel; i++)
                {
                    CHECKF(i >= 1 && i < MAX_WEAPONSKILLLEVEL);
                    nExp += nWeaponSkillLeveUpNeedExp[i];
                }
                const int nMaxRand = 30000;
                int nRand = (nExp / nWeaponSkillExp) / 2;
                if (nRand <= nMaxRand && ::RandGet(nRand) == 0 || nRand > nMaxRand && ::RandGet(nMaxRand) == 0 && ::RandGet(nRand / nMaxRand) == 0)
                {
                    pLearnMagic = pData;
                    break;
                }
            }
        }
    }
    if (pLearnMagic)
    {
        LearnMagic(pLearnMagic->GetInt(MAGICTYPEDATA_TYPE), pLearnMagic->GetInt(MAGICTYPEDATA_LEVEL));
        return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::UserKillTarget(IRole* pTarget)
{
    for(int i = 0; i < m_setData->GetAmount(); i++)
    {
        IMagicData* pData = m_setData->GetObjByIndex(i);
        if ( pData && (pData->GetInt(MAGICDATA_AUTO_ACTIVE) & AUTOACTIVE_KILL)
                && !m_pOwner->QueryTransformation()
                && (!pData->GetInt(MAGICDATA_WEAPON_SUBTYPE) || m_pOwner->CheckWeaponSubType(pData->GetInt(MAGICDATA_WEAPON_SUBTYPE)))
                && ::RandGet(100) < pData->GetInt(MAGICDATA_PERCENT) )
        {
            // backup
            IMagicData*	pMagic		= m_pMagic;
            OBJID		idTarget	= m_idTarget;
            POINT		pos;
            pos.x			= m_pos.x;
            pos.y			= m_pos.y;
            switch(pData->GetInt(MAGICDATA_SORT))
            {
            case	MAGICSORT_RECRUIT:
                {
                    m_pMagic		= pData;
                    m_idTarget		= m_pOwner->GetID();
                    m_pos.x			= m_pOwner->GetPosX();
                    m_pos.y			= m_pOwner->GetPosY();
                    ProcessRecruit();
                }
                break;
            case	MAGICSORT_SERIALCUT:
                {
                    m_pMagic		= pData;
                    m_idTarget		= pTarget->GetID();
                    m_pos.x			= pTarget->GetPosX();
                    m_pos.y			= pTarget->GetPosY();
                    ProcessSerialCut();
                }
                break;
            }
            // restore
            m_pMagic		= pMagic;
            m_idTarget		= idTarget;
            m_pos.x			= pos.x;
            m_pos.y			= pos.y;
            return true;
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::AutoAttackTarget(IRole* pTarget)
{
    int nPercentAdjust = 100;
    // 幻兽发动技能的概率调整
    CMonster* pMonster = NULL;
    if (m_pOwner->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)) && pMonster->IsEudemon())
    {
        if (pMonster->GetFidelity() < 500)
        {
            nPercentAdjust = 0;
        }
        else
        {
            nPercentAdjust = 150 + 5 * ((pMonster->GetFidelity() - 500) / 50);
        }
    }
    for(int i = 0; i < m_setData->GetAmount(); i++)		//??? 可优化
    {
        IMagicData* pData = m_setData->GetObjByIndex(i);
        if ( pData && (pData->GetInt(MAGICDATA_AUTO_ACTIVE) & AUTOACTIVE_RANDOM)
                && !m_pOwner->QueryTransformation()
                && (!pData->GetInt(MAGICDATA_WEAPON_SUBTYPE)
                    || m_pOwner->CheckWeaponSubType(pData->GetInt(MAGICDATA_WEAPON_SUBTYPE))
                    || (pData->GetInt(MAGICDATA_SORT) == MAGICSORT_KO
                        && pTarget->GetLife() * 100 / pTarget->GetMaxLife() <= MAGIC_KO_LIFE_PERCENT))
                && ::RandGet(100) < (nPercentAdjust * pData->GetInt(MAGICDATA_PERCENT) / 100))
        {
            return MagicAttack(pData->GetInt(MAGICDATA_TYPE), pTarget->GetID(), pTarget->GetPosX(), pTarget->GetPosY(), AUTOACTIVE_RANDOM);
        }
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
// timer
//////////////////////////////////////////////////////////////////////
void CMagic::OnTimer(DWORD tCurr)
{
    if (!m_pMagic)
    {
        return;
    }
    switch (m_nMagicState)
    {
    case MAGICSTATE_INTONE:
        {
            if (!m_tIntone.IsActive() || m_tIntone.TimeOver())
            {
#ifdef ZLONG_DEBUG
                m_pOwner->SendSysMsg("DEBUG: 魔法吟唱结束");
#endif
                if (!Launch())	// 魔法执行失败就结束——这里包括目标死亡、目标消失等情况
                {
                    LockTarget(false);
                    m_tApply.Clear();
                    ResetDelay();
                    break;
                }
                m_nMagicState	= MAGICSTATE_LAUNCH;
                // 魔法作用的次数由轨迹数来决定
                // 为了允许没有轨迹的魔法执行，默认作用次数为1，无轨迹
                m_nApplyTimes	= __max(1, m_pMagic->GetSTrackAmount());
                m_tApply.Startup(m_pMagic->GetApplyMS());
            }
        }
        break;
    case MAGICSTATE_LAUNCH:
        {
            if (!m_tApply.IsActive() || m_tApply.IsTimeOut())
            {
#ifdef ZLONG_DEBUG
                m_pOwner->SendSysMsg("Debug: 魔法施展结束");
#endif
                // MAGICSORT_ATTACK类型的魔法留在攻击结束后Kill目标
                if (m_pMagic->GetInt(MAGICDATA_SORT) == MAGICSORT_ATTACK
                        && m_idTarget != ID_NONE)
                {
                    IRole* pTarget = RoleManager()->QueryRole(m_idTarget);
                    if (pTarget && !pTarget->IsAlive())
                    {
                        m_pOwner->Kill(pTarget, GetDieMode());
                    }
                }
                // 怪物死亡需要查询是否处于锁定状态，因此这里必须在Kill怪物之后解锁
                // 否则将不出现尸体消失状态
                LockTarget(false);
                ResetDelay();
                // OnTimer的调用频率为0.5秒，
                // 为了及时执行NextMagic，这里直接进入MAGICSTATE_DELAY
            }
            else
            {
                break;
            }
        }
        //		break;
    case MAGICSTATE_DELAY:
        {
            if (!m_tDelay.IsActive())
            {
                m_nMagicState	= MAGICSTATE_NONE;
                break;
            }
            if (m_bAutoAttack
                    && m_pMagic->GetInt(MAGICDATA_NEXT_MAGIC) != ID_NONE)
                //				&& m_pMagic->GetInt(MAGICDATA_NEXT_MAGIC) != m_pMagic->GetInt(MAGICDATA_TYPE))
            {
                if (m_tDelay.IsActive() && !m_tDelay.TimeOver())	// 必须等到魔法延迟结束才可以执行下一个魔法
                {
                    break;
                }
#ifdef ZLONG_DEBUG
                m_pOwner->SendSysMsg("Debug: 执行下一个魔法。");
#endif
                // may be no next magic
                if (!FindMagic(m_pMagic->GetInt(MAGICDATA_NEXT_MAGIC)))
                {
                    ASSERT(EquipWeaponMagic(SpawnMagicID(), m_pMagic->GetInt(MAGICDATA_NEXT_MAGIC), 0));
                    m_tDelay.Clear();
                    break;
                }
                m_nMagicState	= MAGICSTATE_NONE;
                //				m_pOwner->SetDelay(0);
                MagicAttack(m_pMagic->GetInt(MAGICDATA_NEXT_MAGIC), m_idTarget, m_pos.x, m_pos.y);
                break;
            }
            if (m_tDelay.IsActive() && m_tDelay.TimeOver())
            {
#ifdef ZLONG_DEBUG
                m_pOwner->SendSysMsg("Debug: 魔法延迟结束");
#endif
                m_nMagicState	= MAGICSTATE_NONE;
            }
        }
        break;
    case MAGICSTATE_NONE:
    default:
        break;
    }
}

//////////////////////////////////////////////////////////////////////
// task
//////////////////////////////////////////////////////////////////////
bool CMagic::CheckType(int nType)
{
    return FindMagic(nType) != NULL;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::CheckLevel(int nType, int nLevel)
{
    IMagicData* pData = FindMagic(nType);
    if (pData && pData->GetInt(MAGICDATA_LEVEL) == nLevel)
    {
        return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::UpLevelByTask(int nType)
{
    IMagicData* pData = FindMagic(nType);
    if (!pData)
    {
        return false;
    }
    CHECKF(!IsWeaponMagic(pData->GetInt(MAGICDATA_TYPE)));
    int	nNewLevel = pData->GetInt(MAGICDATA_LEVEL) + 1;
    CHECKF(CMagic::FindMagicType(nType, nNewLevel));
    pData->SetInt(MAGICDATA_EXP, 0);
    pData->SetInt(MAGICDATA_LEVEL, nNewLevel);
    pData->Update();
    //	if (bSynchro)
    if ((pData->GetInt(MAGICDATA_AUTO_ACTIVE) & AUTOACTIVE_HIDDEN) == 0)
    {
        CMsgMagicInfo msg;
        IF_OK(msg.Create(m_pOwner->GetID(), pData->GetInt(MAGICDATA_TYPE), nNewLevel, 0))
        m_pOwner->SendMsg(&msg);
    }
    //	CHECKF(pData->FindType());
    return true;
}

//////////////////////////////////////////////////////////////////////
OBJID CMagic::LearnMagic(int nType, int nLevel/*=0*/, bool bSaveMagic/*=true*/)
{
    CHECKF(CMagic::FindMagicType(nType, nLevel));
    if (FindMagic(nType))
    {
        return ID_NONE;
    }
    DWORD	dwExp	= 0;
    OBJID	idMagic	= ID_NONE;
    IMagicData* pData = NULL;
    if (bSaveMagic)
    {
        // check unlearn magic
        SQLBUF	szSQL;
        sprintf(szSQL, "SELECT id FROM %s WHERE ownerid=%u && type=%d", _TBL_MAGIC, m_pOwner->GetID(), nType);
        CAutoPtr<IRecordset> pRes = _Database(PID)->CreateNewRecordset(szSQL);
        if (pRes)
        {
            idMagic = pRes->GetInt(0);
        }
        pData = CMagicData::CreateNew();
        CHECKF(pData);
        IF_NOT(pData->Create(GameDataDefault()->GetMagicData(), idMagic))
        return ID_NONE;
    }
    else
    {
        // 临时学会魔法，仅仅保存在内存中
        pData = CMagicTempData::CreateNew();
        CHECKF (pData);
        ST_MAGIC_DATA data;
        memset(&data, 0L, sizeof(ST_MAGIC_DATA));
        data.idMagic	= CMagic::SpawnMagicID();
        IF_NOT (pData->Create(&data))
        return ID_NONE;
        idMagic	= data.idMagic;
    }
    pData->SetInt(MAGICDATA_OWNERID, m_pOwner->GetID());
    pData->SetInt(MAGICDATA_TYPE, nType);
    pData->SetInt(MAGICDATA_LEVEL, nLevel);
    pData->SetInt(MAGICDATA_EXP, dwExp);
    pData->SetInt(MAGICDATA_UNLEARN, false);
    if (idMagic != ID_NONE)
    {
        pData->Update();
    }
    else
    {
        pData->SetInt(MAGICDATA_OLD_LEVEL, 0);
        idMagic = pData->InsertRecord();
    }
    pData->FindType();
    pData->SetDelay();		// 设置延迟
    // synchro
    if ((pData->GetInt(MAGICDATA_AUTO_ACTIVE) & AUTOACTIVE_HIDDEN) == 0)
    {
        CMsgMagicInfo msg;
        IF_OK(msg.Create(m_pOwner->GetID(),
                         pData->GetInt(MAGICDATA_TYPE),
                         pData->GetInt(MAGICDATA_LEVEL),
                         pData->GetInt(MAGICDATA_EXP)))
        m_pOwner->SendMsg(&msg);
    }
    m_setData->AddObj(pData);
    return idMagic;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::UnlearnMagic(int nType, bool bDrop)
{
    IMagicData* pData = FindMagic(nType);
    if (!pData)
    {
        return false;
    }
    if (bDrop)
    {
        ASSERT(pData->DeleteRecord());
    }
    else
    {
        pData->SetInt(MAGICDATA_OLD_LEVEL, pData->GetInt(MAGICDATA_LEVEL));
        pData->SetInt(MAGICDATA_UNLEARN, true);
        pData->Update();
    }
    if (m_pMagic && m_pMagic->GetID() == pData->GetID())
    {
        m_pMagic = NULL;
    }
    m_setData->DelObj(pData->GetID());
    // synchro
    CMsgAction	msg;
    IF_OK(msg.Create(m_pOwner->GetID(), m_pOwner->GetPosX(), m_pOwner->GetPosY(), m_pOwner->GetDir(), actionDropMagic, nType))
    m_pOwner->SendMsg(&msg);
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::EquipWeaponMagic(OBJID idMagic, int nType, int nLevel)
{
    //	CHECKF(IsWeaponMagic(nType));
    CMagicTypeData* pType = CMagic::FindMagicType(nType, nLevel);
    CHECKF(pType);
    DWORD	dwExp = 0;
    IMagicData* pData = CMagicData::CreateNew();
    CHECKF(pData);
    IF_NOT(pData->Create(GameDataDefault()->GetMagicData(), idMagic))
    return false;
    pData->SetInt(MAGICDATA_ID_, idMagic);
    pData->SetInt(MAGICDATA_OWNERID, m_pOwner->GetID());
    pData->SetInt(MAGICDATA_TYPE, nType);
    pData->SetInt(MAGICDATA_LEVEL, nLevel);
    pData->SetInt(MAGICDATA_EXP, dwExp);
    pData->ClearUpdateFlags();
    pData->FindType();
    m_setData->DelObj(idMagic);		// 冲掉
    m_setData->AddObj(pData);
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::UnequipWeaponMagic(OBJID idMagic)
{
    if (m_pMagic && m_pMagic->GetID() == idMagic)
    {
        m_pMagic		= NULL;
        m_bAutoAttack	= false;
    }
    return m_setData->DelObj(idMagic);
}

//////////////////////////////////////////////////////////////////////
// calc magic fight
//////////////////////////////////////////////////////////////////////
int CMagic::CalcMagicPower(IRole* pAtker, IRole* pTarget, int* pAdjustAtk/*=0*/)
{
    // attack
    int nMaxRand = 50 + pAtker->GetLuck();
    int nAtk = 0;
    if (::RandGet(100) < nMaxRand)
    {
        nAtk = pAtker->GetMgcMaxAtk() - ::RandGet((pAtker->GetMgcMaxAtk() - pAtker->GetMgcMinAtk()) / 2);
    }
    else
    {
        nAtk = pAtker->GetMgcMinAtk() + ::RandGet((pAtker->GetMgcMaxAtk() - pAtker->GetMgcMinAtk()) / 2);
    }
    nAtk = pAtker->AdjustMagicAtk(nAtk);
    /*CUser* pUser;
    if (pAtker->QueryObj(OBJ_USER, IPP_OF(pUser)))
    {
    	nAtk = __max(0, nAtk + pUser->GetMagicAtk());
    	nAtk += nAtk*pUser->GetGemMgcAtkEffect()/100;
    }*/
    if (pAdjustAtk)
    {
        if (*pAdjustAtk)
        {
            nAtk = ::CutTrail(0, CRole::AdjustDataEx(nAtk, *pAdjustAtk));
        }
        *pAdjustAtk = nAtk;
    }
    int nDef = pTarget->GetMagicDef();
    nDef	= pTarget->AdjustMagicDef(nDef);
    // damage
    int	nDamage	= nAtk - nDef;
    if (pAtker->QueryOwnerUser() && pTarget->IsMonster())
    {
        CMonster* pMonster;
        if (pTarget->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)) && !pMonster->IsEquality())
        {
            nDamage	= CBattleSystem::CalcDamageUser2Monster(nAtk, nDef, pAtker->GetLev(), pTarget->GetLev());
        }
        nDamage = pTarget->AdjustMagicDamage(nDamage);
        nDamage	= CBattleSystem::AdjustMinDamageUser2Monster(nDamage, pAtker, pTarget);
    }
    else if (pAtker->IsMonster() && pTarget->QueryOwnerUser())
    {
        CMonster* pMonster;
        if (pAtker->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)) && !pMonster->IsEquality())
        {
            nDamage	= CBattleSystem::CalcDamageMonster2User(nAtk, nDef, pAtker->GetLev(), pTarget->GetLev());
        }
        nDamage = pTarget->AdjustMagicDamage(nDamage);
        nDamage	= CBattleSystem::AdjustMinDamageMonster2User(nDamage, pAtker, pTarget);
    }
    else
    {
        // adjust magic damage
        nDamage = pTarget->AdjustMagicDamage(nDamage);
    }
    // adjust synflag damage
    CNpc* pNpc = NULL;
    pTarget->QueryObj(OBJ_NPC, IPP_OF(pNpc));
    if (pNpc && pNpc->IsSynFlag() && pNpc->IsSynMoneyEmpty())
    {
        nDamage	*= SYNWAR_NOMONEY_DAMAGETIMES;
    }
    return ::CutTrail(1, nDamage);
}

//////////////////////////////////////////////////////////////////////
bool CMagic::CheckAwardExpEnable(DWORD dwProf)
{
    CHECKF(m_pMagic);
    return m_pOwner->GetLev() >= m_pMagic->GetInt(MAGICDATA_NEED_LEVEL)
           && m_pMagic->GetInt(MAGICDATA_NEED_EXP) != AUTOLEVELUP_EXP
           && m_pMagic->GetInt(MAGICDATA_NEED_EXP) != DISABLELEVELUP_EXP
           && this->CheckProfession(dwProf, m_pMagic->GetInt(MAGICDATA_NEED_PROFMASK))
           //&& m_pMagic->GetInt(MAGICDATA_USE_XP) != TYPE_KONGFU
           ;
}

//////////////////////////////////////////////////////////////////////
void CMagic::OtherMemberAwardExp(IRole* pTarget, int nRawExp)
{
    if (m_pOwner->GetMap()->IsTrainMap())
    {
        return ;
    }
    CTeam* pTeam	= m_pOwner->GetTeam();
    if (pTeam)
    {
        pTeam->AwardMemberExp(m_pOwner->GetID(), pTarget, nRawExp);
    }
    CUser* pUser = NULL;
    if (m_pOwner->QueryObj(OBJ_USER, IPP_OF(pUser)))
    {
        for (int i = 0; i < pUser->GetEudemonAmount(); i++)
        {
            CMonster* pEudemon = pUser->QueryEudemonByIndex(i);
            if (pEudemon && pEudemon->IsAlive() &&
                    (abs(pEudemon->GetPosX() - pTarget->GetPosX()) <= _RANGE_EXPSHARE
                     || abs(pEudemon->GetPosY() - pTarget->GetPosY()) <= _RANGE_EXPSHARE))
            {
                int nBattleExp = pEudemon->AdjustExp(pTarget, nRawExp);
                bool bIncludeOwner = false;
                pEudemon->AwardBattleExp(nBattleExp, true, bIncludeOwner);
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////
int	CMagic::GetExtendAtkRange(CItem* pWeaponR, CItem* pWeaponL)
{
    CHECKF(m_pOwner);
    int nRange = 0;
    for(int i = 0; i < m_setData->GetAmount(); i++)
    {
        IMagicData* pData = m_setData->GetObjByIndex(i);
        if (pData
                && (pData->GetInt(MAGICDATA_AUTO_ACTIVE) & AUTOACTIVE_FOREVER)
                && MAGICSORT_ATKRANGE == pData->GetInt(MAGICDATA_SORT))
        {
            int nWeaponRequired = pData->GetInt(MAGICDATA_WEAPON_SUBTYPE);
            if (0 == nWeaponRequired
                    || (pWeaponR && pWeaponR->GetItemSubType() == nWeaponRequired)
                    || (pWeaponL && pWeaponL->GetItemSubType() == nWeaponRequired))
            {
                nRange += GetPower();
            }
        }
    }
    return nRange;
}

//////////////////////////////////////////////////////////////////////
int CMagic::HitByMagic()
{
    CHECKF(m_pMagic);
    if (m_pMagic->GetInt(MAGICDATA_WEAPON_HIT) == HIT_BY_MAGIC)
    {
        if (m_pMagic->GetInt(MAGICDATA_USE_XP) == TYPE_XPSKILL)
        {
            return MAGICTYPE_XPSKILL;
        }
        else
        {
            return MAGICTYPE_NORMAL;
        }
    }
    CItem* pItem = m_pOwner->GetEquipItemByPos(ITEMPOSITION_WEAPONL);
    if (m_pMagic->GetInt(MAGICDATA_WEAPON_HIT) == HIT_BY_ITEM && pItem && pItem->GetMgcMinAtk() > 0) // ->GetInt(ITEMDATA_MAGICATK_MIN) > 0)
    {
        if (m_pMagic->GetInt(MAGICDATA_USE_XP) == TYPE_XPSKILL)
        {
            return MAGICTYPE_XPSKILL;
        }
        else
        {
            return MAGICTYPE_NORMAL;
        }
    }
    return MAGICTYPE_NONE;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::HitByWeapon()
{
    CHECKF(m_pMagic);
    if (m_pMagic->GetInt(MAGICDATA_WEAPON_HIT) == HIT_BY_WEAPON)
    {
        return true;
    }
    CItem* pItem = m_pOwner->GetEquipItemByPos(ITEMPOSITION_WEAPONL);
    if (m_pMagic->GetInt(MAGICDATA_WEAPON_HIT) == HIT_BY_ITEM && pItem && pItem->GetMgcMinAtk() <= 0) // ->GetInt(ITEMDATA_MAGICATK_MIN) <= 0)
    {
        return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
void CMagic::ResetDelay()
{
    CHECK(m_pMagic);
    m_nDelay	= m_nRawDelay;
    m_nMagicState	= MAGICSTATE_DELAY;
    m_tDelay.Update();			// 开始DELAY
    // 启动 每个魔法单独的延迟计时器
    if (m_pMagic)
    {
        m_pMagic->StartDelay();
    }
}

//////////////////////////////////////////////////////////////////////
// bAtkerPenetrable:  攻击者移动轨迹是否可以穿人
// bTargetPenetrable: 被攻击者移动轨迹是否可以穿人
bool CMagic::AdjustPosByTrack(int nStepIndex, bool bAtkerPenetrable/*=false*/, bool bTargetPenetrable/*=false*/)
{
    CHECKF(m_pOwner);
    CHECKF(m_pMagic);
    CMapPtr pMap = m_pOwner->GetMap();
    if (!pMap)
    {
        return false;
    }
    // record target pos
    int	nTargetPosX = m_pOwner->GetPosX();
    int nTargetPosY = m_pOwner->GetPosY();
    IRole*	pTarget	= RoleManager()->QueryRole(m_idTarget);
    if (pTarget)
    {
        nTargetPosX	= pTarget->GetPosX();
        nTargetPosY	= pTarget->GetPosY();
    }
    if (nStepIndex >= 0 && nStepIndex < m_pMagic->GetTTrackAmount())
    {
        // 取轨迹
        const ST_TRACK& track = m_pMagic->GetTTrackByIndex(nStepIndex);
        for (int i = 0; i < m_setTargetLocked.size(); i++)
        {
            DEBUG_TRY
            OBJID idTarget	= m_setTargetLocked[i];
            if (idTarget == ID_NONE)
            {
                continue;
            }
            IRole*	pTarget	= m_pOwner->FindAroundRole(idTarget);
            if (!pTarget)
            {
                continue;
            }
            int nNewX	= pTarget->GetPosX();
            int nNewY	= pTarget->GetPosY();
            // 取得攻击方向
            UCHAR ucAtkDir = ::GetDirByPos(m_pOwner->GetPosX(), m_pOwner->GetPosY(), pTarget->GetPosX(), pTarget->GetPosY());
            // 根据轨迹方向计算位移坐标
            int nMoveDir = (ucAtkDir + track.ucDir) % 8;
            for (int i = 0; i < track.ucStep; i++)
            {
                int nTmpX = (nNewX + _DELTA_X[nMoveDir]);
                int nTmpY = (nNewY + _DELTA_Y[nMoveDir]);
                if (pMap->IsMoveEnable(nNewX, nNewY, nTmpX, nTmpY, track.ucAlt)
                        || (bTargetPenetrable && pMap->IsStandEnable(nTmpX, nTmpY)))
                {
                    nNewX	= nTmpX;
                    nNewY	= nTmpY;
                }
            }
            //			if (nNewX != pTarget->GetPosX() || nNewY != pTarget->GetPosY())
            {
#ifdef _DEBUG
                CUser* pTargetUser = NULL;
                if (pTarget->QueryObj(OBJ_USER, IPP_OF(pTargetUser)))
                {
                    //					::MyLogSave("syslog\\magic", "Debug: track step [%d], move from [%d, %d] to [%d, %d]",
                    //						nStepIndex, pTargetUser->GetPosX(), pTargetUser->GetPosY(), nNewX, nNewY);
                    pTargetUser->SendSysMsg("Debug: track step [%d], move from [%d, %d] to [%d, %d]",
                                            nStepIndex, pTargetUser->GetPosX(), pTargetUser->GetPosY(), nNewX, nNewY);
                }
#endif
                pTarget->ProcessOnMove(MOVEMODE_TRACK);
                pTarget->SyncTrackTo(nNewX, nNewY, (nMoveDir + 4) % 8, track.idAction);
            }
            DEBUG_CATCH("Process adjust target track error.")
        }	// end of for
    }
    // attacker track
    if (nStepIndex >= 0 && nStepIndex < m_pMagic->GetSTrackAmount())
    {
        int nNewX	= m_pOwner->GetPosX();
        int nNewY	= m_pOwner->GetPosY();
        // 取得攻击方向
        UCHAR ucAtkDir = m_pOwner->GetDir();
        if (pTarget)
        {
            ucAtkDir = ::GetDirByPos(m_pOwner->GetPosX(), m_pOwner->GetPosY(), nTargetPosX, nTargetPosY);
        }
        // 根据轨迹方向计算位移坐标
        const ST_TRACK& track	= m_pMagic->GetSTrackByIndex(nStepIndex);
        int nMoveDir = (ucAtkDir + track.ucDir) % 8;
        for (int i = 0; i < track.ucStep; i++)
        {
            int nTmpX = (nNewX + _DELTA_X[nMoveDir]);
            int nTmpY = (nNewY + _DELTA_Y[nMoveDir]);
            if (pMap->IsMoveEnable(nNewX, nNewY, nTmpX, nTmpY, track.ucAlt)
                    || (bAtkerPenetrable && pMap->IsStandEnable(nTmpX, nTmpY)))
            {
                nNewX	= nTmpX;
                nNewY	= nTmpY;
            }
        }
        //		if (nNewX != m_pOwner->GetPosX() || nNewY != m_pOwner->GetPosY())
        {
#ifdef _DEBUG
            //			::MyLogSave("syslog\\magic", "Debug: track step [%d], move from [%d, %d] to [%d, %d]",
            //				nStepIndex, m_pOwner->GetPosX(), m_pOwner->GetPosY(), nNewX, nNewY);
            m_pOwner->SendSysMsg("Debug: track step [%d], move from [%d, %d] to [%d, %d]",
                                 nStepIndex, m_pOwner->GetPosX(), m_pOwner->GetPosY(), nNewX, nNewY);
#endif
            m_pOwner->ProcessOnMove(MOVEMODE_TRACK);
            m_pOwner->SyncTrackTo(nNewX, nNewY, nMoveDir, track.idAction);
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::LockTarget(bool bLock)
{
    if (m_bTargetLocked == bLock)
    {
        return true;
    }
    int nAction = actionLockUser;
    if (!bLock)
    {
        nAction	= actionUnlockUser;
    }
    int nLockSecs	= m_pMagic->GetLockSecs();
    // lock atk user
    if (bLock)
    {
        if (m_pOwner->IsAlive())
        {
            CMsgAction	msg;
            if (msg.Create(m_pOwner->GetID(), m_pOwner->GetPosX(), m_pOwner->GetPosY(), m_pOwner->GetDir(), nAction))
            {
                m_pOwner->BroadcastRoomMsg(&msg, true);
                CRole::AttachStatus(m_pOwner, STATUS_LOCK, 0, nLockSecs);
            }
        }
    }
    else
    {
        CRole::DetachStatus(m_pOwner, STATUS_LOCK);
    }
    // lock target
    vector<OBJID>::iterator	it = m_setTargetLocked.begin();
    for (; it != m_setTargetLocked.end(); it++)
    {
        IRole* pRole = RoleManager()->QueryRole(*it);
        if (pRole)
        {
            if (bLock)
            {
                if (pRole->IsAlive())
                {
                    CMsgAction msg;
                    if (msg.Create(pRole->GetID(), pRole->GetPosX(), pRole->GetPosY(), pRole->GetDir(), nAction))
                    {
                        pRole->BroadcastRoomMsg(&msg, true);
                    }
                    CRole::AttachStatus(pRole, STATUS_LOCK, 0, nLockSecs);
                }
            }
            else
            {
                CRole::DetachStatus(pRole, STATUS_LOCK);
            }
        }
    }
    m_bTargetLocked	= bLock;
    return true;
}

//////////////////////////////////////////////////////////////////////
int CMagic::GetPower(int nStepIndex /* = -1*/)
{
    CHECKF(m_pMagic);
    //	if (nStepIndex != -1)
    //		nStepIndex	= m_pMagic->GetSTrackAmount() - (m_nApplyTimes + 1);
    if (nStepIndex >= 0 && nStepIndex < m_pMagic->GetSTrackAmount())
    {
        const ST_TRACK&	track = m_pMagic->GetSTrackByIndex(nStepIndex);
        return track.nPower;
    }
    return m_pMagic->GetInt(MAGICDATA_POWER);
}

//////////////////////////////////////////////////////////////////////
/*
int CMagic::GetApplyMS(int nStepIndex / *= -1* /)
{
	CHECKF(m_pMagic);

	// 缺省取当前步的延迟时间
//	if (nStepIndex == -1)
//		nStepIndex	= m_pMagic->GetSTrackAmount() - (m_nApplyTimes + 1);

	if (nStepIndex >= 0 && nStepIndex < m_pMagic->GetSTrackAmount())
	{
		const ST_TRACK&	track = m_pMagic->GetSTrackByIndex(nStepIndex);
		return track.nApplyMS;
	}

	return m_pMagic->GetInt(MAGICDATA_APPLY_MS);
}
*/
//////////////////////////////////////////////////////////////////////
BOOL CMagic::HaveXpMagic()
{
    for(int i = 0; i < m_setData->GetAmount(); i++)
    {
        IMagicData* pData = m_setData->GetObjByIndex(i);
        if (pData && pData->GetInt(MAGICDATA_USE_XP) == TYPE_XPSKILL)
        {
            return true;
        }
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
// 目前设计仅用于幻兽自动学习魔法
// 如需要用于其它用途可能要做相应修改
bool CMagic::AutoLearnMagic()
{
    const int	MAX_AUTO_LEARN_RAND = 10000;
    bool bLearnMagic = false;
    DWORD	dwProf = m_pOwner->GetProfession();
    for(int i = 0; i < AutoMagicTypeSet()->GetAmount(); i++)		//??? 可优化
    {
        CMagicTypeData* pData = AutoMagicTypeSet()->GetObjByIndex(i);
        if (pData && pData->GetInt(MAGICTYPEDATA_AUTO_LEARN)
                && m_pOwner->GetLev() >= pData->GetInt(MAGICTYPEDATA_LEARN_LEVEL)
                && CheckProfession(dwProf, pData->GetInt(MAGICTYPEDATA_NEED_PROFMASK))
                && ::RandGet(MAX_AUTO_LEARN_RAND) < pData->GetInt(MAGICTYPEDATA_AUTO_LEARN_PROB)
                && FindMagic(pData->GetInt(MAGICTYPEDATA_TYPE)) == NULL)
        {
            LearnMagic(pData->GetInt(MAGICTYPEDATA_TYPE), pData->GetInt(MAGICTYPEDATA_LEVEL));
            bLearnMagic = true;
        }
    }
    return bLearnMagic;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::CheckProfession(DWORD dwProf, DWORD dwNeedProf)
{
    DWORD dwProfMask = Prof2Mask(dwProf);
    if (dwProfMask != 0)
    {
        const DWORD		PROF_MASK_USER		= 0x0000003F;
        if ((dwNeedProf & PROF_MASK_USER) == 0 || (dwProfMask & dwNeedProf) != 0)
        {
            return true;
        }
    }
    else
    {
        // 幻兽职业
        const DWORD		PROF_MASK_EVOLVE0	= 0x00003FC0;
        const DWORD		PROF_MASK_EVOLVE1	= 0x0000C000;
        const DWORD		PROF_MASK_EVOLVE2	= 0x00FF0000;
        dwProfMask = 0;
        DWORD dwProf1 = dwProf / 100;
        if (dwProf1 > 0)		// 0 次进化职业，1~8
        {
            dwProfMask = 1 << (dwProf1 - 1 + MAX_USER_PROFS);
        }
        if ((dwNeedProf & PROF_MASK_EVOLVE0) != 0 && (dwNeedProf & dwProfMask) == 0)
        {
            return false;
        }
        dwProfMask = 0;
        DWORD dwProf2 = dwProf / 10 % 10;
        if (dwProf2 > 0)		// 1 次进化职业，1~2
        {
            dwProfMask = 1 << (dwProf2 - 1 + MAX_USER_PROFS + 8);
        }
        if ((dwNeedProf & PROF_MASK_EVOLVE1) != 0 && (dwNeedProf & dwProfMask) == 0)
        {
            return false;
        }
        dwProfMask = 0;
        DWORD dwProf3 = dwProf % 10;
        if (dwProf3 > 0)		// 2 次进化职业，1~8
        {
            dwProfMask = 1 << (dwProf3 - 1 + MAX_USER_PROFS + 8 + 2);
        }
        if ((dwNeedProf & PROF_MASK_EVOLVE2) != 0 && (dwNeedProf & dwProfMask) == 0)
        {
            return false;
        }
        return true;
    }
    return false;
}
