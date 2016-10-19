
//**********************************************************
// 代码编辑器
//**********************************************************

// BattleSystem.cpp: implementation of the CBattleSystem class.
//
//////////////////////////////////////////////////////////////////////


#include "Allmsg.h"
#include "I_Role.h"
#include "BattleSystem.h"
#include "mapgroup.h"

const int	BOW_SHIELD_HITRATE_PERCENT			= 12;			// 盾牌对箭矢的遮挡率
MYHEAP_IMPLEMENTATION(CBattleSystem, s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

char	szZcxKey[]			= "level";
char	szLevupexpTable[]	= _TBL_LEVEXP;

char	szTutorKey[]		= "tutor_id";
char	szTutorexpTable[]	= _TBL_TUTOREXP;


//////////////////////////////////////////////////////////////////////
CBattleSystem::CBattleSystem(PROCESS_ID idProcess, IRole* pOwner)
{
    m_idProcess	= idProcess;
    m_pOwner	= pOwner;
    m_idTarget	= ID_NONE;
}

//////////////////////////////////////////////////////////////////////
CBattleSystem::~CBattleSystem()
{
}

//////////////////////////////////////////////////////////////////////
BOOL CBattleSystem::ProcAttack_Hand2Hand()
{
    //战斗消息也在本函数中发送给客户端
    DEBUG_TRY
    if (!m_pOwner || m_idTarget == ID_NONE)
    {
        return false;
    }
    if (!IsBattleMaintain())
    {
        return false;
    }
    DEBUG_CATCH("[初期调试信息]CBattleSystem::ProcAttack_Hand2Hand IsBattleMain()");
    IRole* pTarget	= m_pOwner->GetMap()->QueryRole(m_pOwner->GetPosX(), m_pOwner->GetPosY(), m_idTarget);
    if (!pTarget)
    {
        this->ResetBattle();
        return true;
    }
    CMonster* pMonster = NULL;
    if (pTarget->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)))
    {
        if (pMonster->IsEudemon())
        {
            int k = 0;
        }
    }
    if (m_pOwner->IsBowman() && !m_pOwner->GetMap()->IsTrainMap())
    {
        if (!m_pOwner->SpendArrow())
        {
            this->ResetBattle();
            return true;
        }
    }
    if (!m_pOwner->IsAtkable(pTarget))
    {
        this->ResetBattle();
        return true;
    }
    // 自动(被动)技能触发
    if (m_pOwner->AutoSkillAttack(pTarget))
    {
        return true;
    }
    /*DEBUG_TRY
    // 计算躲避率coding here...
    if (this->IsTargetDodged(m_pOwner, pTarget))
    {
    	// 传送结果消息。
    	m_pOwner->SendDamageMsg(pTarget->GetID(), 0);
    	m_pOwner->QueryStatusSet()->DelObj(STATUS_KEEPBOW);
    	return true;
    }
    DEBUG_CATCH("[初期调试信息]CBattleSystem::ProcAttack_Hand2Hand TryDodge!");*/
    CNpc* pNpc;
    pTarget->QueryObj(OBJ_NPC, IPP_OF(pNpc));
    CUser* pOwnerUser = m_pOwner->QueryOwnerUser();
    //---jinggy---2004-11-18--用攻击类宝石攻击----begin
    if (pOwnerUser)
    {
        if (pOwnerUser->UseSkillAtk(pTarget->GetID()))
        {
            return true ;
        }
    }
    //---jinggy---2004-11-18--用攻击类宝石攻击----end
    // 躲避失败，计算伤害值。
    int nTargetOrgLife	= pTarget->GetLife();
    int nDamage			= m_pOwner->Attack(pTarget);
    int nTargetLifeLost = __max(0, nTargetOrgLife - pTarget->GetLife());
    int nExp = nTargetLifeLost;
    // 传送结果消息。
    DEBUG_TRY
    m_pOwner->SendDamageMsg(pTarget->GetID(), nDamage);
    //	m_pOwner->QueryStatusSet()->DelObj(STATUS_KEEPBOW);
#ifdef	PALED_DEBUG
    m_pOwner->SendSysMsg("lose life: %d - %d = %d", nTargetOrgLife, nTargetLifeLost, pTarget->GetLife());
#endif
    if (nDamage == 0)
    {
        return true;
    }
    // syn war score
    if (pOwnerUser && pNpc && pNpc->IsAwardScore())
    {
        pOwnerUser->AwardSynWarScore(pNpc, nTargetLifeLost);
    }
    DEBUG_CATCH("[初期调试信息]CBattleSystem:: attack & syn score!");
    // award exp
    DEBUG_TRY
    if (pTarget->IsMonster() || (pNpc && pNpc->IsGoal()))	// target is monster
    {
        // correct exp
        nExp = m_pOwner->AdjustExp(pTarget, nExp);
        int nAdditionExp = 0;
        if (!pTarget->IsAlive()) // target was killed
        {
            nAdditionExp = pTarget->GetMaxLife() * KILLBONUS_PERCENT / 100;
            nExp += nAdditionExp;
            // 有组队，并且队伍中有其他队员，则奖励其他队员经验
            if (pOwnerUser)
            {
                CTeam* pTeam	= pOwnerUser->GetTeam();
                if (pTeam && !(pNpc && pNpc->IsGoal()))
                {
                    pTeam->AwardMemberExp(pOwnerUser->GetID(), pTarget, nAdditionExp);
                }
            }
            // award exp for eudemon
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
                        int nBattleExp = pEudemon->AdjustExp(pTarget, nAdditionExp);
                        bool bIncludeOwner = false;
                        pEudemon->AwardBattleExp(nBattleExp, true, bIncludeOwner);
                    }
                }
            }
        }
        m_pOwner->AwardBattleExp(nExp);
        if (!pTarget->IsAlive() && nAdditionExp > 0) // target was killed
        {
            if (!m_pOwner->GetMap()->IsTrainMap())
            {
                m_pOwner->SendSysMsg(STR_KILLING_EXPERIENCE, nAdditionExp);
            }
        }
    }
    DEBUG_CATCH("[初期调试信息]CBattleSystem:: Add Exp!");
    // addition magic
    DEBUG_TRY
    m_pOwner->AdditionMagic(nTargetLifeLost, nDamage);
    DEBUG_CATCH("[初期调试信息]CBattleSystem:: AdditionMagic!");
    // kill?
    DEBUG_TRY
    if (!pTarget->IsAlive())
    {
        DWORD dwDieWay = m_pOwner->IsSimpleMagicAtk() ? DIE_MAGIC : DIE_NORMAL;
        if (nDamage > pTarget->GetMaxLife() / 3)
        {
            dwDieWay = DIE_CRITICALHIT;
        }
        m_pOwner->Kill(pTarget, m_pOwner->IsBowman() ? DIE_ARROWSHOOT : dwDieWay);
    }
    DEBUG_CATCH("[初期调试信息]CBattleSystem::ProcAttack_Hand2Hand Kill!");
    // 计算武器损耗coding here.
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CBattleSystem::IsBattleMaintain()
{
    if (m_idTarget == ID_NONE)
    {
        return false;
    }
    IRole* pTarget	= RoleManager()->QueryRole(m_idTarget);
    if (!pTarget)
    {
        this->ResetBattle();
        return false;
    }
    if (!m_pOwner->IsAtkable(pTarget))
    {
        this->ResetBattle();
        return false;
    }
    if (pTarget->IsWing() && !m_pOwner->IsWing() && !(m_pOwner->IsBowman() || m_pOwner->IsSimpleMagicAtk()))
    {
        this->ResetBattle();
        return false;
    }
    int nDistance = 0;
    if (m_pOwner->IsMonster())
    {
        nDistance = m_pOwner->GetDistance(pTarget->QueryMapThing());
    }
    else
    {
        int nPosX = 0, nPosY = 0;
        pTarget->GetFootPrint(nPosX, nPosY);
        nDistance = m_pOwner->GetDistance(nPosX, nPosY);
    }
    if (nDistance > m_pOwner->GetAttackRange(pTarget->GetSizeAdd()))
    {
        this->ResetBattle();
        return false;
    }
    /*
    	if (!pTarget->IsAlive())
    	{
    		this->ResetBattle();
    		return false;
    	}

    	if (!m_pOwner->IsAlive())
    	{
    		this->ResetBattle();
    		return false;
    	}
    */
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CBattleSystem::CreateBattle(OBJID idTarget)
{
    if (idTarget == ID_NONE)
    {
        return false;
    }
    m_idTarget	= idTarget;
    return true;
}

//////////////////////////////////////////////////////////////////////
void CBattleSystem::ResetBattle()
{
    m_idTarget	= ID_NONE;
}

//////////////////////////////////////////////////////////////////////
// 根据魔域修改案，这个函数修改后仅用于计算物理攻击的躲避
BOOL CBattleSystem::IsTargetDodged(IRole* pAtker, IRole* pTarget)
{
    if (!pAtker || !pTarget)
    {
        return true;
    }
    // 如果目标处于魔法施展中，百分百躲避 -- zlong 2004.5.12
    if (pTarget->QueryMagic() && pTarget->QueryMagic()->IsInLaunch())
    {
        return true;
    }
    int nDodge	= 0;
    CUser* pTargetUser = NULL;
    if (pTarget->QueryObj(OBJ_USER, IPP_OF(pTargetUser)))
    {
        nDodge = 40;	// 被攻击者是人，初始闪避为40
    }
    int nHitRate = __min(100, __max(0, 100 + (int)pAtker->GetAtkHitRate() - nDodge - (int)pTarget->GetDdg()));
#ifdef _DEBUG
    CUser* pAtkerUser = NULL;
    pAtker->QueryObj(OBJ_USER, IPP_OF(pAtkerUser));
    if (pAtkerUser)
    {
        pAtkerUser->SendSysMsg(_TXTATR_NORMAL, "HitRate: %d", nHitRate);
    }
    if (pTargetUser)
    {
        pTargetUser->SendSysMsg(_TXTATR_NORMAL, "Monster HitRate: %d", nHitRate);
    }
#endif
    return  (::RandGet(100) >= nHitRate);
}

//////////////////////////////////////////////////////////////////////
int CBattleSystem::CalcPower(int nMagic, IRole* pAtker, IRole* pTarget, int* pAdjustAtk/*=0*/, bool bCanDodge/*=false*/)
{
    int	nPower	= 0;
    // bCanDodge==false则不考虑躲避 —— 用于魔法攻击的时候计算伤害，因为策划要求魔法攻击仅通过percent计算命中率
    // bCanDodge==true则计算躲避 —— 用于普通攻击时计算伤害
    if (!(bCanDodge && CBattleSystem::IsTargetDodged(pAtker, pTarget)))
    {
        if (nMagic == MAGICTYPE_NONE)
        {
            nPower += CBattleSystem::CalcAttackPower(pAtker, pTarget, pAdjustAtk);
        }
        else
        {
            nPower += CMagic::CalcMagicPower(pAtker, pTarget, pAdjustAtk);
            // XP技能对玩家的伤害为 2%
            if (nMagic == MAGICTYPE_XPSKILL)
            {
                CUser* pUser = NULL;
                if (pTarget->QueryObj(OBJ_USER, IPP_OF(pUser)))
                {
                    nPower /= 50;
                }
            }
        }
    }
    if (nPower > 0)
    {
        CMonster* pMonster = NULL;
        if (pAtker->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)))
        {
            nPower = pMonster->AdjustFrenzy2Dmg(nPower);
        }
        IStatus* pStatus = NULL;
        // 攻击结界，增加伤害
        if (pStatus = pAtker->QueryStatus(STATUS_TEAMATTACK))
        {
            nPower	+= nPower * pStatus->GetPower() / 100;
        }
        // 狂暴状态增加伤害
        if (pStatus = pAtker->QueryStatus(STATUS_FRENZY))
        {
            if (pAtker->GetLife() * 100 / pAtker->GetMaxLife() < 50)
            {
                nPower = ::MulDiv(nPower, 100 + pStatus->GetPower(), 100);
            }
        }
        // 神力状态
        if (pStatus = pAtker->QueryStatus(STATUS_EXTRA_POWER))
        {
            int nMultiple	= 2;
            if (pTarget->IsMonster())
            {
                nMultiple	= 10;
            }
            nPower *= nMultiple;
        }
        // 调整攻击造成的伤害
        if (pStatus = pAtker->QueryStatus(STATUS_ATKER_DAMAGE))
        {
            nPower = ::CutTrail(0, CRole::AdjustDataEx(nPower, pStatus->GetPower()));
        }
        // 调整被击造成的伤害
        if (pStatus = pTarget->QueryStatus(STATUS_DAMAGE))
        {
            nPower = ::CutTrail(0, CRole::AdjustDataEx(nPower, pStatus->GetPower()));
        }
        // 潜力值的影响
        if (pAtker->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)))
        {
            // 幻兽每增/减1点潜力值，对敌人伤害增/减1%
            int nOffset = pMonster->GetPotential() - DEFAULT_EUDEMON_POTENTIAL;
            if (nOffset != 0)
            {
                nPower = ::MulDiv(nPower, 100 + nOffset, 100);
            }
        }
        if (pTarget->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)))
        {
            // 幻兽每增/减1点潜力值，受到敌人的伤害减/增1% —— 刚好与上面相反
            int nOffset = pMonster->GetPotential() - DEFAULT_EUDEMON_POTENTIAL;
            if (nOffset != 0)
            {
                nPower = ::MulDiv(nPower, 100 - nOffset, 100);
            }
        }
        // 狂暴状态减少伤害
        if (pStatus = pTarget->QueryStatus(STATUS_FRENZY))
        {
            if (pTarget->GetLife() * 100 / pTarget->GetMaxLife() < 50)
            {
                nPower -= __min(nPower, nPower * pStatus->GetPower() / 100);
            }
        }
        // 护体结界，减少伤害
        if (pStatus = pTarget->QueryStatus(STATUS_TEAMDEFENCE))
        {
            nPower	= nPower * __max(0, 100 - (int)pStatus->GetPower()) / 100;
        }
        // 狂暴状态会改变伤害
        if (pStatus = pAtker->QueryStatus(STATUS_TRUCULENCE))
        {
            int nPercent = 150;
            nPercent -= (pAtker->GetMaxLife() - pAtker->GetLife()) * 100 / pAtker->GetMaxLife();
            nPower	= MulDiv(nPower, nPercent, 100);
        }
        // 结界系统的伤害转移
        if (pStatus = pTarget->QueryStatus(STATUS_TEAMSPIRIT))
        {
            CUser* pUser = NULL;
            if (!pTarget->QueryObj(OBJ_USER, IPP_OF(pUser)))
            {
                CMonster* pMonster = NULL;
                if (pTarget->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)))
                {
                    if (pMonster->IsEudemon())
                    {
                        pUser	= pTarget->QueryOwnerUser();
                    }
                }
            }
            if (pUser)
            {
                CTeam* pTeam = pUser->GetTeam();
                if (pTeam)
                {
                    nPower	= pTeam->ShareDamage(nPower, pTarget->GetID());
                }
            }
        }
    }
    return nPower;
}

//////////////////////////////////////////////////////////////////////
int CBattleSystem::CalcAttackPower(IRole* pAtker, IRole* pTarget, int* pAdjustAtk/*=0*/, int* pAdjustDef/*=0*/)
{
    CHECKF(pAtker && pTarget);
    // super man status
    //	IStatus* pSuperMan = pAtker->QueryStatus(STATUS_SUPERMAN);
    // get user
    CUser* pUser = NULL;
    pAtker->QueryObj(OBJ_USER, IPP_OF(pUser));
    // attack
    int nMaxRand = 50 + pAtker->GetLuck();
    int nAtk = 0;
    if (::RandGet(100) < nMaxRand)
    {
        nAtk = pAtker->GetMaxAtk() - ::RandGet((pAtker->GetMaxAtk() - pAtker->GetMinAtk()) / 2);
    }
    else
    {
        nAtk = pAtker->GetMinAtk() + ::RandGet((pAtker->GetMaxAtk() - pAtker->GetMinAtk()) / 2);
    }
    if (pUser)
    {
#ifdef _DEBUG
        pUser->SendSysMsg(_TXTATR_NORMAL, "atk org value: %d", nAtk);
#endif
    }
    nAtk	= pAtker->AdjustAttack(nAtk);
    if (pAdjustAtk)
    {
        if (*pAdjustAtk)
        {
            nAtk = ::CutTrail(0, CRole::AdjustDataEx(nAtk, *pAdjustAtk));
        }
        *pAdjustAtk = nAtk;
    }
    // defence
    int nRawDef	= pTarget->GetDef();
#ifdef _DEBUG
    if (pUser)
    {
        pUser->SendSysMsg(_TXTATR_NORMAL, "target row def: +%d", nRawDef);
    }
#endif
    int nDef = pTarget->AdjustDefence(nRawDef);
    if (pAtker->IsBowman())
    {
        nDef = pTarget->AdjustBowDefence(nDef);
    }
#ifdef	PALED_DEBUG
    //	if (pSuperMan && pAtker->IsPM())
    //	{
    //		nAtk = pTarget->GetLife();
    //		pAtker->SendSysMsg("==========必杀===========");
    //	}
#endif
    if (pAdjustDef)
    {
        if (*pAdjustDef)
        {
            nDef = ::CutTrail(0, CRole::AdjustDataEx(nDef, *pAdjustDef));
        }
        *pAdjustDef	= nDef;
    }
    //	if (pSuperMan)
    //	{
    //		nDef /= 2;
    //#ifdef _DEBUG
    //		if (pUser)
    //			pUser->SendSysMsg(_TXTATR_NORMAL, "target def decrease to 1/2 for superman status, def=%d", nDef);
    //#endif
    //	}
    // damage
    int	nDamage	= nAtk - nDef;
    if (pAtker->QueryOwnerUser() && pTarget->IsMonster())
    {
        CMonster* pMonster;
        if (pTarget->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)) && !pMonster->IsEquality())
        {
            nDamage	= CBattleSystem::CalcDamageUser2Monster(nAtk, nDef, pAtker->GetLev(), pTarget->GetLev());
        }
        nDamage = pTarget->AdjustWeaponDamage(nDamage);
        nDamage	= CBattleSystem::AdjustMinDamageUser2Monster(nDamage, pAtker, pTarget);
    }
    else if (pAtker->IsMonster() && pTarget->QueryOwnerUser())
    {
        CMonster* pMonster;
        if (pAtker->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)) && !pMonster->IsEquality())
        {
            nDamage	= CBattleSystem::CalcDamageMonster2User(nAtk, nDef, pAtker->GetLev(), pTarget->GetLev());
        }
        nDamage = pTarget->AdjustWeaponDamage(nDamage);
        nDamage	= CBattleSystem::AdjustMinDamageMonster2User(nDamage, pAtker, pTarget);
    }
    else
    {
        nDamage	= pTarget->AdjustWeaponDamage(nDamage);
    }
    return ::CutTrail(0, nDamage);
}

//////////////////////////////////////////////////////////////////////
int	CBattleSystem::AdjustMinDamageUser2Monster(int nDamage, IRole* pAtker, IRole* pTarget)
{
    int nMinDamage = 1;
    nMinDamage += pAtker->GetLev() / 10;
    CUser* pUser;
    if (!pAtker->QueryObj(OBJ_USER, IPP_OF(pUser)))
    {
        return ::CutTrail(nMinDamage, nDamage);
    }
    CItem* pItem = pUser->GetWeaponR();
    if (pItem)
    {
        nMinDamage += pItem->GetQuality();
    }
    return ::CutTrail(nMinDamage, nDamage);
}

//////////////////////////////////////////////////////////////////////
int	CBattleSystem::AdjustMinDamageMonster2User(int nDamage, IRole* pAtker, IRole* pTarget)
{
    int nMinDamage = 7;
    if (nDamage >= nMinDamage || pTarget->GetLev() <= 15)
    {
        return nDamage;
    }
    CUser* pUser;
    if (!pTarget->QueryObj(OBJ_USER, IPP_OF(pUser)))
    {
        return ::CutTrail(nMinDamage, nDamage);
    }
    for(int i = ITEMPOSITION_EQUIPBEGIN; i < ITEMPOSITION_EQUIPEND; i++)
    {
        switch(i)
        {
        case	ITEMPOSITION_HELMET:
        case	ITEMPOSITION_NECKLACE:
        case	ITEMPOSITION_ARMOR:
            {
                CItem* pItem = pUser->GetEquipItemByPos(i);
                if (pItem)
                {
                    nMinDamage -= pItem->GetQuality() / 5;
                }
            }
            break;
        }
    }
    nMinDamage = ::CutTrail(1, nMinDamage);
    return ::CutTrail(nMinDamage, nDamage);
}

//////////////////////////////////////////////////////////////////////
int CBattleSystem::CalcDamageUser2Monster(int nAtk, int nDef, int nAtkLev, int nDefLev)
{
    if (nAtkLev > 120)
    {
        nAtkLev = 120;
    }
    int nDamage = nAtk - nDef;
    if (CMonster::GetNameType(nAtkLev, nDefLev) != NAME_GREEN)
    {
        return ::CutTrail(0, nDamage);
    }
    int nDeltaLev = nAtkLev - nDefLev;
    if (nDeltaLev >= 3 && nDeltaLev <= 5)
    {
        nAtk *= 1.5;
    }
    else if (nDeltaLev > 5 && nDeltaLev <= 10)
    {
        nAtk *= 2;
    }
    else if (nDeltaLev > 10 && nDeltaLev <= 20)
    {
        nAtk *= 2.5;
    }
    else if (nDeltaLev > 20)
    {
        nAtk *= 3;
    }
    return ::CutTrail(0, nAtk - nDef);
    //	if (nAtkLev - nDefLev > 0)
    //	{
    //		int nDeltaLev = nAtkLev - nDefLev;
    //		if (1<nAtkLev && nAtkLev<=19)
    //		{
    //			if (nDeltaLev<3)
    //				nDamage=nAtk-nDef;
    //			else if (3 <= nDeltaLev && nDeltaLev < 6)
    //				nDamage=nAtk*1.2-nDef;
    //			else
    //				nDamage=nAtk*1.5-nDef;
    //		}
    //		else if (19<nAtkLev && nAtkLev<=49)
    //		{
    //			if (nDeltaLev<5)
    //				nDamage=nAtk-nDef;
    //			else if (5 <= nDeltaLev && nDeltaLev < 10)
    //				nDamage=nAtk*1.2-nDef;
    //			else
    //				nDamage=nAtk*1.5-nDef;
    //		}
    //		else if (49<nAtkLev && nAtkLev<=85)
    //		{
    //			if (nDeltaLev<4)
    //				nDamage=nAtk-nDef;
    //			else if (4 <= nDeltaLev && nDeltaLev < 8)
    //				nDamage=nAtk*1.2-nDef;
    //			else
    //				nDamage=nAtk*1.5-nDef;
    //		}
    //		else if (85<nAtkLev && nAtkLev<=112)
    //		{
    //			if (nDeltaLev<3)
    //				nDamage=nAtk-nDef;
    //			else if (3 <= nDeltaLev && nDeltaLev < 6)
    //				nDamage=nAtk*1.2-nDef;
    //			else
    //				nDamage=nAtk*1.5-nDef;
    //		}
    //		else if (112<nAtkLev && nAtkLev<=120)
    //		{
    //			if (nDeltaLev<2)
    //				nDamage=nAtk-nDef;
    //			else if (2 <= nDeltaLev && nDeltaLev < 4)
    //				nDamage=nAtk*1.2-nDef;
    //			else
    //				nDamage=nAtk*1.5-nDef;
    //		}
    //	}
    //	else
    //	{
    //		nDamage=nAtk-nDef;
    //	}
    //
    //	return ::CutTrail(0, nDamage);
}
//////////////////////////////////////////////////////////////////////
int CBattleSystem::CalcDamageMonster2User(int nAtk, int nDef, int nAtkLev, int nDefLev)
{
    if (nAtkLev > 120)
    {
        nAtkLev = 120;
    }
    int nDamage = nAtk - nDef;
    int nNameType = CMonster::GetNameType(nDefLev, nAtkLev);
    if (nNameType == NAME_RED)
    {
        nDamage = nAtk * 1.5 - nDef;
    }
    else if (nNameType == NAME_BLACK)
    {
        int nDeltaLev = nDefLev - nAtkLev;
        if (nDeltaLev >= -10 && nDeltaLev <= -5)
        {
            nAtk *= 2;
        }
        else if (nDeltaLev >= -20 && nDeltaLev < -10)
        {
            nAtk *= 3.5;
        }
        else if (nDeltaLev < -20)
        {
            nAtk *= 5;
        }
        nDamage = nAtk - nDef;
    }
    //	if (nAtkLev - nDefLev > 0)
    //	{
    //		int nDeltaLev = nDefLev-nAtkLev;
    //		if (1<nAtkLev && nAtkLev<=19)
    //		{
    //			if (nDeltaLev<3)
    //				nDamage=nAtk-nDef;
    //			else if (3 <= nDeltaLev && nDeltaLev < 6)
    //				nDamage=nAtk+14-nDef;
    //			else
    //				nDamage=nAtk+14*2-nDef;
    //		}
    //		else if (19<nAtkLev && nAtkLev<=49)
    //		{
    //			if (nDeltaLev<5)
    //				nDamage=nAtk-nDef;
    //			else if (5 <= nDeltaLev && nDeltaLev < 10)
    //				nDamage=nAtk+50-nDef;
    //			else
    //				nDamage=nAtk+50*2-nDef;
    //		}
    //		else if (49<nAtkLev && nAtkLev<=85)
    //		{
    //			if (nDeltaLev<4)
    //				nDamage=nAtk-nDef;
    //			else if (4 <= nDeltaLev && nDeltaLev < 8)
    //				nDamage=nAtk+96-nDef;
    //			else
    //				nDamage=nAtk+96*2-nDef;
    //		}
    //		else if (85<nAtkLev && nAtkLev<=112)
    //		{
    //			if (nDeltaLev<3)
    //				nDamage=nAtk-nDef;
    //			else if (3 <= nDeltaLev && nDeltaLev < 6)
    //				nDamage=nAtk+140-nDef;
    //			else
    //				nDamage=nAtk+140*2-nDef;
    //		}
    //		else if (112<nAtkLev && nAtkLev<=120)
    //		{
    //			if (nDeltaLev<2)
    //				nDamage=nAtk-nDef;
    //			else if (2 <= nDeltaLev && nDeltaLev < 4)
    //				nDamage=nAtk+165-nDef;
    //			else
    //				nDamage=nAtk+165*2-nDef;
    //		}
    //	}
    //	else
    //	{
    //		nDamage=nAtk-nDef;
    //	}
    return ::CutTrail(0, nDamage);
}
//////////////////////////////////////////////////////////////////////
int CBattleSystem::AdjustExp(int nDamage, int nAtkLev, int nDefLev)
{
    if (nAtkLev > 120)
    {
        nAtkLev = 120;
    }
    int nExp = nDamage;
    int nNameType = CMonster::GetNameType(nAtkLev, nDefLev);
    int nDeltaLev = nAtkLev - nDefLev;
    if (nNameType == NAME_GREEN)
    {
        if (nDeltaLev >= 3 && nDeltaLev <= 5)
        {
            nExp = nExp * 70 / 100;
        }
        else if (nDeltaLev > 5 && nDeltaLev <= 10)
        {
            nExp = nExp * 20 / 100;
        }
        else if (nDeltaLev > 10 && nDeltaLev <= 20)
        {
            nExp = nExp * 10 / 100;
        }
        else if (nDeltaLev > 20)
        {
            nExp = nExp * 5 / 100;
        }
    }
    else if (nNameType == NAME_RED)
    {
        nExp *= 1.3;
    }
    else if (nNameType == NAME_BLACK)
    {
        if (nDeltaLev >= -10 && nDeltaLev < -5)
        {
            nExp *= 1.5;
        }
        else if (nDeltaLev >= -20 && nDeltaLev < -10)
        {
            nExp *= 1.8;
        }
        else if (nDeltaLev < -20)
        {
            nExp *= 2.3;
        }
    }
    return ::CutTrail(0, nExp);
}

//////////////////////////////////////////////////////////////////////
int CBattleSystem::AdjustDrop(int nDrop, int nAtkLev, int nDefLev)
{
    if (nAtkLev > 120)
    {
        nAtkLev = 120;
    }
    if (nAtkLev - nDefLev > 0)
    {
        int nDeltaLev = nAtkLev - nDefLev;
        if (1 < nAtkLev && nAtkLev <= 19)
        {
            if (nDeltaLev < 3)
                ;
            else if (3 <= nDeltaLev && nDeltaLev < 6)
            {
                nDrop = nDrop / 5;
            }
            else
            {
                nDrop = nDrop / 10;
            }
        }
        else if (19 < nAtkLev && nAtkLev <= 49)
        {
            if (nDeltaLev < 5)
                ;
            else if (5 <= nDeltaLev && nDeltaLev < 10)
            {
                nDrop = nDrop / 5;
            }
            else
            {
                nDrop = nDrop / 10;
            }
        }
        else if (49 < nAtkLev && nAtkLev <= 85)
        {
            if (nDeltaLev < 4)
                ;
            else if (4 <= nDeltaLev && nDeltaLev < 8)
            {
                nDrop = nDrop / 5;
            }
            else
            {
                nDrop = nDrop / 10;
            }
        }
        else if (85 < nAtkLev && nAtkLev <= 112)
        {
            if (nDeltaLev < 3)
                ;
            else if (3 <= nDeltaLev && nDeltaLev < 6)
            {
                nDrop = nDrop / 5;
            }
            else
            {
                nDrop = nDrop / 10;
            }
        }
        else if (112 < nAtkLev && nAtkLev <= 120)
        {
            if (nDeltaLev < 2)
                ;
            else if (2 <= nDeltaLev && nDeltaLev < 4)
            {
                nDrop = nDrop / 5;
            }
            else
            {
                nDrop = nDrop / 10;
            }
        }
    }
    return ::CutTrail(0, nDrop);
}
