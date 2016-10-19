
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
#include "MapTrap.h"

/*/ 4超级的特殊状态，及超级回复
3500	3500	0002	回复术
3501	3501	0006	钢铁皮肤
3502	3502	0006	魔法护盾
3503	3503	0006	神圣祈祷
3504	3504	0006	魔力加持
*/
const int	SUPER_MAGIC_RECRUIT			= 3500;
const int	SUPER_MAGIC_DEFENCE			= 3501;
const int	SUPER_MAGIC_MGC_DEFENCE		= 3502;
const int	SUPER_MAGIC_ATTACK			= 3503;
const int	SUPER_MAGIC_MGC_ATTACK		= 3504;

const int	THROW_WEAPON_DROP_RANGE		= 1;							// 扔武器攻击可掉物品的范围

//////////////////////////////////////////////////////////////////////
void	MovePos(POINT* pPos, int nDir)
{
    nDir %= 8;
    pPos->x	+= _DELTA_X[nDir];
    pPos->y	+= _DELTA_X[nDir];
}

//////////////////////////////////////////////////////////////////////
void	DDALine(int x0, int y0, int x1, int y1, int nRange, vector<POINT>& vctPoint)
{
    if (x0 == x1 && y0 == y1)
    {
        return;
    }
    float scale = 1.0f * nRange / sqrt(float((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0)));
    x1 = int(0.5f + scale * (x1 - x0) + x0);
    y1 = int(0.5f + scale * (y1 - y0) + y0);
    ::DDALineEx(x0, y0, x1, y1, vctPoint);
}

//////////////////////////////////////////////////////////////////////
// launch
//////////////////////////////////////////////////////////////////////
int CMagic::Launch()
{
    if (!m_pMagic)
    {
        return 0;
    }
    if (!m_pOwner->IsAlive())
    {
        ShowMiss();
        return 0;
    }
    int ret = 0;
    switch(m_pMagic->GetInt(MAGICDATA_SORT))
    {
    case	MAGICSORT_ATTACK:
        ret = ProcessAttack();
        break;
    case	MAGICSORT_RECRUIT:
        ret = ProcessRecruit();
        break;
    case	MAGICSORT_CROSS:
        ret = ProcessCross();
        break;
    case	MAGICSORT_LINE:
    case	MAGICSORT_LINE_PENETRABLE:
        ret = ProcessLine();
        break;
    case	MAGICSORT_FAN:
        ret = ProcessFan();
        break;
    case	MAGICSORT_BOMB:
        ret = ProcessBomb();
        break;
    case	MAGICSORT_BOMB_LOCKALL:
        ret = ProcessBombLockAll();
        break;
    case	MAGICSORT_ATTACHSTATUS:
    case	MAGICSORT_TEAM_MAGIC:
        ret = ProcessAttach();
        break;
    case	MAGICSORT_DETACHSTATUS:
        ret = ProcessDetach();
        break;
    case	MAGICSORT_SQUARE:
        ret = ProcessSquare();
        break;
    case	MAGICSORT_JUMPATTACK:
        ret = ProcessJumpAtk();
        break;
    case	MAGICSORT_RANDOMTRANS:
        ret = ProcessRandomTrans();
        break;
    case	MAGICSORT_DISPATCHXP:
        ret = ProcessDispatchXP();
        break;
    case	MAGICSORT_COLLIDE:
        ret = ProcessCollide();
        break;
    case	MAGICSORT_ATKSTATUS:
        ret = ProcessAtkStatus();
        break;
    case	MAGICSORT_CALLTEAMMEMBER:
        ret = ProcessCallTeamMember();
        break;
    case	MAGICSORT_RECORDTRANSSPELL:
        ret = ProcessRecordTransSpell();
        break;
    case	MAGICSORT_TRANSFORM:
        ret = ProcessTransform();
        break;
    case	MAGICSORT_ADDMANA:
        ret = ProcessAddMana();
        break;
    case	MAGICSORT_LAYTRAP:
        ret = ProcessLayTrap();
        break;
    case	MAGICSORT_CALLPET:
        ret = ProcessCallPet();
        break;
    case	MAGICSORT_DECLIFE:
        ret = ProcessDecLife();
        break;
    case	MAGICSORT_GROUNDSTING:
        ret = ProcessGroundSting();
        break;
    case	MAGICSORT_REBORN:
        ret = ProcessReborn();
        break;
    case	MAGICSORT_SORB_SOUL:
        ret = ProcessSorbSoul();
        break;
    case	MAGICSORT_STEAL:
        ret = ProcessSteal();
        break;
        /////////////////////////////////////////////////
        // 新增幻兽魔法类型
    case	MAGICSORT_BLAST_THUNDER:
        ret = ProcessBlastThunder();
        break;
    case	MAGICSORT_MULTI_ATTACHSTATUS:
        ret = ProcessMultiAttach();
        break;
    case	MAGICSORT_MULTI_DETACHSTATUS:
        ret = ProcessMultiDetach();
        break;
    case	MAGICSORT_MULTI_CURE:
        ret = ProcessMultiCure();
        break;
    case	MAGICSORT_STEAL_MONEY:
        ret = ProcessStealMoney();
        break;
    case	MAGICSORT_KO:
        ret = ProcessKO();
        break;
    case	MAGICSORT_ESCAPE:
        ret = ProcessEscape();
        break;
        /////////////////////////////////////////////////
    default:
        ASSERT(!"switch magic sort");
    }
    return ret;
}

//////////////////////////////////////////////////////////////////////
// magic process
//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessAttack()
{
    CHECKF(m_pMagic);
    CHECKF(m_pOwner);
    /*
    // check syn of user
    if (!m_pOwner->SynPosition(m_pos.x, m_pos.y))
    {
    	UserManager()->KickOutSocket(m_pOwner->GetSocketID(), "SynPosition");
    	return false;
    }
    */
    m_setTargetLocked.clear();
    IRole* pTarget = m_pOwner->FindAroundRole(m_idTarget);
    if (!pTarget || !pTarget->IsAlive())
    {
        return false;
    }
    //	if (m_pOwner->GetDistance(pTarget->GetPosX(), pTarget->GetPosY()) > m_pMagic->GetInt(MAGICDATA_DISTANCE))
    //		return false;
    if (this->IsImmunity(pTarget))
    {
        return false;
    }
    if (m_pMagic->GetInt(MAGICDATA_FLOOR_ATTR))		// 0: need not check
    {
        int nAttr = m_pOwner->GetMap()->GetFloorAttr(pTarget->GetPosX(), pTarget->GetPosY());
        if (m_pMagic->GetInt(MAGICDATA_FLOOR_ATTR) != nAttr)
        {
            return false;
        }
    }
    m_setTargetLocked.push_back(pTarget->GetID());
    if (m_pMagic->GetSTrackAmount() > 0)
    {
        LockTarget(true);
    }
    int nTotalExp = 0;
    for (int k = 0; k < m_nApplyTimes; k++)
    {
        int	nPower = CBattleSystem::CalcPower(HitByMagic(), m_pOwner, pTarget, GetPower(k));
        // 广播法术效果
        CMsgMagicEffect	msg;
        IF_OK(msg.Create(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetInt(MAGICDATA_LEVEL), m_idTarget, nPower, m_pOwner->GetDir()))
        m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
        // set crime status
        CheckCrime(pTarget);
        if (nPower)
        {
            int nLifeLost = nPower;
            if (pTarget->TransferShield(HitByMagic(), m_pOwner, nPower))
            {
            }
            else
            {
                nLifeLost = __min(pTarget->GetLife(), nPower);
                pTarget->AddAttrib(_USERATTRIB_LIFE, -1 * nLifeLost, SYNCHRO_TRUE);
                pTarget->BeAttack(HitByMagic(), m_pOwner, nPower);
            }
            nTotalExp += nLifeLost;
            IStatus* pStatus = m_pOwner->QueryStatus(STATUS_DMG2LIFE);
            if (pStatus)
            {
                int nLifeGot = ::CutTrail(0, MulDiv(nPower, pStatus->GetPower(), 100));
                if (nLifeGot > 0)
                {
                    m_pOwner->AddAttrib(_USERATTRIB_LIFE, nLifeGot, SYNCHRO_TRUE);
                }
            }
        }
        if (m_pMagic->GetSTrackAmount() > 0)
        {
            AdjustPosByTrack(k);
        }
    }
    if (nTotalExp > 0)
    {
        AwardExpOfLife(pTarget, nTotalExp);
    }
    m_pos.x	= pTarget->GetPosX();
    m_pos.y = pTarget->GetPosY();
    //	if (!pTarget->IsAlive())
    //		m_pOwner->Kill(pTarget, GetDieMode());
    //? 未增加武器熟练度，也未损耗双方装备。
    // drop weapon in weapon skill
    if (m_pMagic->GetInt(MAGICDATA_USE_XP) == TYPE_KONGFU && m_pMagic->GetInt(MAGICDATA_DROPWEAPON) == THROW_WERPONR_TARGET)
    {
        CItem* pWeapon = m_pOwner->GetWeaponR();
        IF_OK(pWeapon)
        {
            POINT	pos;
            pos.x	= pTarget->GetPosX();
            pos.y	= pTarget->GetPosY();
            m_pOwner->GetMap()->FindDropItemCell(THROW_WEAPON_DROP_RANGE, &pos);
            m_pOwner->DropItem(pWeapon->GetID(), pos.x, pos.y);					// 没找到CELL也扔，但会重叠。
        }
    }
    return true;
}

bool CMagic::ProcessAtkStatus()
{
    CHECKF(m_pMagic);
    //	vector<OBJID>	setLastLocked;
    //	if (!m_setTargetLocked.empty())
    //		copy(m_setTargetLocked.begin(), m_setTargetLocked.end(), back_inserter(setLastLocked));
    m_setTargetLocked.clear();
    IRole* pTarget = m_pOwner->FindAroundRole(m_idTarget);
    if (!pTarget || !pTarget->IsAlive())
    {
        return false;
    }
    //	if (m_pOwner->GetDistance(pTarget->GetPosX(), pTarget->GetPosY()) > m_pMagic->GetInt(MAGICDATA_DISTANCE))
    //		return false;
    if (this->IsImmunity(pTarget))
    {
        return false;
    }
    //	m_setTargetLocked.push_back(pTarget->GetID());
    int	nPower	= 0;
    bool bAttachStatus = false;
    int nStatusPower = 0;
    int nSecs = 0;
    int nTimes = 0;
    DWORD nStatus = 0;
    if (HitByWeapon())	// && !CBattleSystem::IsTargetDodged(m_pOwner, pTarget))
    {
        int	nAdjust = 0;
        switch(m_pMagic->GetInt(MAGICDATA_STATUS))
        {
        case	STATUS_ATTACK:
            {
                int	nAdjustAtk	= GetPower();
                nPower = CBattleSystem::CalcAttackPower(m_pOwner, pTarget, &nAdjustAtk, 0);
            }
            break;
        case	STATUS_DEFENCE1:
        case	STATUS_DEFENCE2:
        case	STATUS_DEFENCE3:
            {
                int	nAdjustDef	= GetPower();
                nPower = CBattleSystem::CalcAttackPower(m_pOwner, pTarget, 0, &nAdjustDef);
            }
            break;
            //		case	STATUS_WEAPONDAMAGE:
            //			{
            //				int	nAdjustDmg	= GetPower();
            //				nPower = CBattleSystem::CalcAttackPower(m_pOwner, pTarget);
            //				nPower = ::CutRange(m_pOwner->AdjustData(nPower, nAdjustDmg, pTarget->GetLife()), 0, (int)pTarget->GetMaxLife()*9/10);
            //			}
            //			break;
        case	STATUS_ATKSPEED:
            {
                int	nPause	= GetPower();
                nPower = CBattleSystem::CalcAttackPower(m_pOwner, pTarget);
                pTarget->SetFightPause(nPause);
            }
            break;
            // 根据策划要求，取消这个状态以避免MAGICDATA_PERCENT的使用冲突
            //		case	STATUS_STOP:
            //			{
            //				int	nAdjustAtk	= GetPower();
            //				int	nMilliSecs	= m_pMagic->GetInt(MAGICDATA_STEP);
            //				nPower = CBattleSystem::CalcAttackPower(m_pOwner, pTarget, &nAdjustAtk);
            //				if (pTarget->IsMonster() && ::RandGet(100) < m_pMagic->GetInt(MAGICDATA_PERCENT))
            //				{
            //					CMsgAction	msg;
            //					IF_OK(msg.Create(pTarget->GetID(), pTarget->GetPosX(), pTarget->GetPosY(), pTarget->GetDir(), actionMoveStop, nMilliSecs))
            //						pTarget->SendMsg(&msg);
            //				}
            //			}
            //			break;
        case	STATUS_VAMPIRE:
            {
                int	nAdjustAtk		= GetPower();
                int	nAwardPercent	= m_pMagic->GetInt(MAGICDATA_WIDTH);
                CHECKF(nAwardPercent >= 0 && nAwardPercent <= 100);
                nPower = CBattleSystem::CalcAttackPower(m_pOwner, pTarget, &nAdjustAtk);
                int	nAddLife	= MulDiv(::CutTrail((int)pTarget->GetLife(), nPower), nAwardPercent, 100);
                if (nAddLife > 0 && m_pOwner->GetLife() < m_pOwner->GetMaxLife() && m_pOwner->IsAlive())
                {
                    m_pOwner->AddAttrib(_USERATTRIB_LIFE, nAddLife, UPDATE_TRUE);
                }
            }
            break;
        case	STATUS_NORMAL:
            break;
        default:	// other status
            {
                if (::RandGet(100) < m_pMagic->GetInt(MAGICDATA_STATUS_CHANCE))
                {
                    bAttachStatus = true;
                    nStatusPower	= GetPower();
                    nSecs	= m_pMagic->GetInt(MAGICDATA_STEP);
                    nTimes	= m_pMagic->GetInt(MAGICDATA_TIMES);
                    nStatus	= m_pMagic->GetInt(MAGICDATA_STATUS);
                    ASSERT(nStatusPower > 0);
                    //					CRole::AttachStatus(pTarget, nStatus, nStatusPower, nSecs, nTimes);
                }
                nPower = CBattleSystem::CalcAttackPower(m_pOwner, pTarget);
            }
            break;
        }
    }
    // 广播法术效果
    CMsgMagicEffect	msg;
    IF_OK(msg.Create(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetInt(MAGICDATA_LEVEL), m_idTarget, nPower, m_pOwner->GetDir()))
    m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
    // set crime status
    CheckCrime(pTarget);
    if (nPower)
    {
        CNpc* pNpc = NULL;
        int nLifeLost = nPower;
        if (pTarget->TransferShield(HitByMagic(), m_pOwner, nPower))
        {
        }
        else
        {
            nLifeLost = __min(pTarget->GetLife(), nPower);
            pTarget->AddAttrib(_USERATTRIB_LIFE, -1 * nLifeLost, SYNCHRO_TRUE);
            CUser* pTargetUser = NULL;
            if (pTarget->QueryObj(OBJ_USER, IPP_OF(pTargetUser)))
            {
                pTargetUser->BroadcastTeamLife();
            }
            if (pTarget->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && pNpc->IsAwardScore())
            {
                m_pOwner->AwardSynWarScore(pNpc, nLifeLost);
            }
            pTarget->BeAttack(HitByMagic(), m_pOwner, nPower);
        }
        int nExp = 0;
        if (pTarget->IsMonster() || pNpc && pNpc->IsGoal() && m_pOwner->GetLev() >= pNpc->GetLev())
        {
            nExp += m_pOwner->AdjustExp(pTarget, nLifeLost);
            if (!pTarget->IsAlive())
            {
                // 有组队，并且队伍中有其他队员，则奖励其他队员经验
                int	nBonusExp	= pTarget->GetMaxLife() * KILLBONUS_PERCENT / 100;
                OtherMemberAwardExp(pTarget, nBonusExp);
                nExp += m_pOwner->AdjustExp(pTarget, nBonusExp, true);
            }
        }
        // 必须在分配组队经验之后杀死
        if (!pTarget->IsAlive())
        {
            m_pOwner->Kill(pTarget, GetDieMode());
        }
        IStatus* pStatus = m_pOwner->QueryStatus(STATUS_DMG2LIFE);
        if (pStatus)
        {
            int nLifeGot = ::CutTrail(0, MulDiv(nPower, pStatus->GetPower(), 100));
            if (nLifeGot > 0)
            {
                m_pOwner->AddAttrib(_USERATTRIB_LIFE, nLifeGot, SYNCHRO_TRUE);
            }
        }
        this->AwardExp(nExp);
    }
    // 把原来的先加状态后算攻击修改为先算攻击后加状态
    // 原因是例如冰冻之类的魔法会在BeAttack的时候解除，因此必须在BeAttack之后调用加状态
    if (bAttachStatus)
    {
        if (pTarget->IsAlive())
        {
            CRole::AttachStatus(pTarget, nStatus, nStatusPower, nSecs, nTimes);
        }
    }
    //? 未增加武器熟练度，也未损耗双方装备。
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessRecruit()
{
    CHECKF(m_pMagic);
    m_setTargetLocked.clear();
    vector<IRole*>	setRole;
    vector<int>		setPower;
    CTeam* pTeam = m_pOwner->GetTeam();
    if (pTeam && m_pMagic->GetInt(MAGICDATA_MULTI))
    {
        for(int i = 0; i < pTeam->GetMemberAmount(); i++)
        {
            OBJID idTarget = pTeam->GetMemberByIndex(i);
            IRole* pRole = m_pOwner->FindAroundRole(idTarget);
            CNpc* pNpc;
            if (!pRole || !pRole->IsAlive() || pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc)))
            {
                continue;
            }
            setRole.push_back(pRole);
            if (m_pOwner->GetID() != pRole->GetID())
            {
                m_setTargetLocked.push_back(pRole->GetID());
            }
        }
    }
    else
    {
        IRole* pRole = m_pOwner->FindAroundRole(m_idTarget);
        if (!pRole || !pRole->IsAlive())
        {
            return false;
        }
        setRole.push_back(pRole);
        m_setTargetLocked.push_back(pRole->GetID());
    }
    if (m_pMagic->GetSTrackAmount() > 0)
    {
        LockTarget(true);
    }
    int	nExp = 0;
    for (int k = 0; k < m_nApplyTimes; k++)
    {
        // 广播法术效果
        CMsgMagicEffect	msg;
        IF_OK(msg.Create(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetInt(MAGICDATA_LEVEL), m_idTarget, m_pOwner->GetDir()))
        {
            for(int i = 0; i < setRole.size(); i++)
            {
                IRole* pRole = setRole[i];
                if (pRole && pRole->IsAlive())
                {
                    int nPower = GetPower(k);
                    if (nPower == ADJUST_FULL)
                    {
                        nPower = pRole->GetMaxLife() - pRole->GetLife();
                    }
                    // 超级回复
                    if (m_pMagic && m_pMagic->GetInt(MAGICDATA_TYPE) >= SUPER_MAGIC_RECRUIT)
                    {
                        nPower *= m_pOwner->GetSoulSum();
                    }
                    setPower.push_back(nPower);
                    msg.AppendRole(setRole[i]->GetID(), nPower);
                }
                else
                {
                    setPower.push_back(0);
                }
            }
            m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
        }
        for(int i = 0; i < setRole.size(); i++)
        {
            if (!setRole[i]->IsAlive())
            {
                continue;
            }
            int nAddLife = ::CutOverflow(setPower[i], ::CutTrail(0, (int)(setRole[i]->GetMaxLife() - setRole[i]->GetLife())));
            if (nAddLife > 0)
            {
                setRole[i]->AddAttrib(_USERATTRIB_LIFE, nAddLife, SYNCHRO_TRUE);
                CUser* pUser = NULL;
                if (setRole[i]->QueryObj(OBJ_USER, IPP_OF(pUser)))
                {
                    pUser->BroadcastTeamLife();
                }
            }
            CNpc* pNpc = NULL;
            if (setRole[i])
            {
                setRole[i]->QueryObj(OBJ_NPC, IPP_OF(pNpc));
                if (pNpc && pNpc->IsGoal())
                {
                    nAddLife = nAddLife * GOAL_EXP_PERCENT / 100;
                }
            }
            nExp += nAddLife;
        }
        if (m_pMagic->GetSTrackAmount() > 0)
        {
            AdjustPosByTrack(k);
        }
    }
    this->AwardExp(0);
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessCross()
{
    CHECKF(m_pMagic);
    int	nRange1	= m_pMagic->GetInt(MAGICDATA_RANGE) % 100;
    int	nRange2	= m_pMagic->GetInt(MAGICDATA_RANGE) / 100;
    int	nSize		= nRange1 * 2 + 1;
    int	nBufSize	= nSize * nSize;
    MAPTHING_SET	setTarget;
    vector<IRole*>	setRole;
    vector<int>		setPower;
    POINT	pos;
    int nExp = 0;
    int nPowerSum = 0;
    for (int k = 0; k < m_nApplyTimes; k++)
    {
        // 搜索目标集
        {
            m_setTargetLocked.clear();
            setTarget.clear();
            setRole.clear();
            setPower.clear();
            int nDir	= ::GetDirByPos(m_pOwner->GetPosX(), m_pOwner->GetPosY(), m_pos.x, m_pos.y);
            pos.x		= m_pOwner->GetPosX();
            pos.y		= m_pOwner->GetPosY();
            m_pOwner->GetMap()->CollectMapThing(setTarget, pos, nRange1, OBJ_USER | OBJ_MONSTER | OBJ_NPC);
            int nThisDir = nDir;
            for(int i = 0; i < 4; i++)
            {
                POINT	posThis = pos;
                for(int j = 0; j < (i % 2 ? nRange1 : nRange2); j++)
                {
                    MovePos(&posThis, nThisDir);
                    if (!m_pOwner->GetMap()->IsValidPoint(posThis))
                    {
                        continue;
                    }
                    if (!m_pOwner->GetMap()->IsAltEnable(pos, posThis, MAGICDAMAGE_ALT))
                    {
                        continue;
                    }
                    int idx = POS2INDEX(posThis.x - pos.x + nRange1, posThis.y - pos.y + nRange1, nSize, nSize);
                    ASSERT(idx >= 0 && idx < setTarget.size());
                    if (setTarget[idx])
                    {
                        IRole* pRole = NULL;
                        if (setTarget[idx]->QueryRole(IPP_OF(pRole)) && pRole->IsAlive() && !IsImmunity(pRole))
                        {
                            //if (nLoseLife)
                            {
                                setRole.push_back(pRole);
                                // 只锁定一个攻击目标
                                if (pRole->GetID() == m_idTarget)
                                {
                                    m_setTargetLocked.push_back(pRole->GetID());
                                }
                            }
                        }
                    }
                }
                nThisDir = (nThisDir + 2) % 8;
            }
        }
        // 锁定目标
        if (m_pMagic->GetSTrackAmount() > 0)
        {
            LockTarget(true);
        }
        // 广播法术效果
        CMsgMagicEffect	msg;
        IF_OK(msg.CreateByPos(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetInt(MAGICDATA_LEVEL), pos.x, pos.y, m_pOwner->GetDir()))
        {
            for(int i = 0; i < setRole.size(); i++)
            {
                IRole* pRole = setRole[i];
                if (pRole && pRole->IsAlive())
                {
                    int nPower = CBattleSystem::CalcPower(HitByMagic(), m_pOwner, pRole, GetPower(k));
                    setPower.push_back(nPower);
                    msg.AppendRole(pRole->GetID(), nPower);
                }
                else
                {
                    setPower.push_back(0);
                }
            }
            m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
        }
        // set crime status
        CheckCrime(&setRole);
        for (int i = 0; i < setRole.size(); i++)
        {
            if (!setRole[i]->IsAlive())
            {
                continue;
            }
            nPowerSum += setPower[i];
            CNpc* pNpc = NULL;
            int nLifeLost = setPower[i];
            if (setPower[i] && setRole[i]->TransferShield(HitByMagic(), m_pOwner, setPower[i]))
            {
            }
            else
            {
                nLifeLost = __min(setRole[i]->GetLife(), setPower[i]);
                setRole[i]->AddAttrib(_USERATTRIB_LIFE, -1 * nLifeLost, SYNCHRO_TRUE);
                if (setRole[i]->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && pNpc->IsAwardScore())
                {
                    m_pOwner->AwardSynWarScore(pNpc, nLifeLost);
                }
                setRole[i]->BeAttack(HitByMagic(), m_pOwner, setPower[i]);
            }
            if (setRole[i]->IsMonster() || pNpc && pNpc->IsGoal() && m_pOwner->GetLev() >= pNpc->GetLev())
            {
                nExp += m_pOwner->AdjustExp(setRole[i], nLifeLost);
                if (!setRole[i]->IsAlive())
                {
                    // 有组队，并且队伍中有其他队员，则奖励其他队员经验
                    int	nBonusExp	= setRole[i]->GetMaxLife() * KILLBONUS_PERCENT / 100;
                    OtherMemberAwardExp(setRole[i], nBonusExp);
                    nExp += m_pOwner->AdjustExp(setRole[i], nBonusExp, true);
                }
            }
        }
        if (m_pMagic->GetSTrackAmount() > 0)
        {
            AdjustPosByTrack(k);
        }
        // 必须先发轨迹后发kill消息
        for (int i = 0; i < setRole.size(); i++)
        {
            if (!setRole[i]->IsAlive())
            {
                m_pOwner->Kill(setRole[i], GetDieMode());
            }
        }
    }
    IStatus* pStatus = m_pOwner->QueryStatus(STATUS_DMG2LIFE);
    if (pStatus)
    {
        int nLifeGot = ::CutTrail(0, MulDiv(nPowerSum, pStatus->GetPower(), 100));
        if (nLifeGot > 0)
        {
            m_pOwner->AddAttrib(_USERATTRIB_LIFE, nLifeGot, SYNCHRO_TRUE);
        }
    }
    AwardExp(nExp);
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessLine()
{
    CHECKF(m_pMagic);
    vector<IRole*>	setTarget;
    vector<int>		setPower;
    vector<POINT>	setPoint;
    int	nExp = 0;
    int nPowerSum = 0;
    bool bAtkerPenetrable	= (m_pMagic->GetInt(MAGICDATA_SORT) == MAGICSORT_LINE_PENETRABLE);
    for (int k = 0; k < m_nApplyTimes; k++)
    {
        // 搜索目标集
        {
            m_setTargetLocked.clear();
            setTarget.clear();
            setPower.clear();
            setPoint.clear();
            POINT pos;
            pos.x	= m_pOwner->GetPosX();
            pos.y	= m_pOwner->GetPosY();
            DDALine(pos.x, pos.y, m_pos.x, m_pos.y, m_pMagic->GetInt(MAGICDATA_RANGE), setPoint);
            for(int i = 0; i < setPoint.size(); i++)
            {
                POINT posThis = setPoint[i];
                if (!m_pOwner->GetMap()->IsValidPoint(posThis))
                {
                    continue;
                }
                if (!m_pOwner->GetMap()->IsAltEnable(pos, posThis, MAGICDAMAGE_ALT))
                {
                    continue;
                }
                IRole* pTarget = m_pOwner->GetMap()->QueryRole(setPoint[i].x, setPoint[i].y, ID_NONE);
                if (!pTarget || !pTarget->IsAlive() || this->IsImmunity(pTarget))
                {
                    continue;
                }
                // attack now
                setTarget.push_back(pTarget);
                if (m_pOwner->GetID() != pTarget->GetID())
                {
                    m_setTargetLocked.push_back(pTarget->GetID());
                }
            }
        }
        // 锁定目标
        if (m_pMagic->GetSTrackAmount() > 0)
        {
            LockTarget(true);
        }
        // 广播法术效果
        CMsgMagicEffect	msg;
        IF_OK(msg.CreateByPos(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetInt(MAGICDATA_LEVEL), m_pos.x, m_pos.y, m_pOwner->GetDir()))
        {
            for(int i = 0; i < setTarget.size(); i++)
            {
                IRole* pRole = setTarget[i];
                if (pRole && pRole->IsAlive())
                {
                    int nPower = CBattleSystem::CalcPower(HitByMagic(), m_pOwner, pRole, GetPower(k));
                    setPower.push_back(nPower);
                    msg.AppendRole(pRole->GetID(), nPower);
                }
                else
                {
                    setPower.push_back(0);
                }
            }
            m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
        }
        // set crime status
        CheckCrime(&setTarget);
        for (int i = 0; i < setTarget.size(); i++)
        {
            if (!setTarget[i]->IsAlive())
            {
                continue;
            }
            nPowerSum += setPower[i];
            CNpc* pNpc = NULL;
            int nLifeLost = setPower[i];
            if (setPower[i] && setTarget[i]->TransferShield(HitByMagic(), m_pOwner, setPower[i]))
            {
            }
            else
            {
                nLifeLost = __min(setTarget[i]->GetLife(), setPower[i]);
                setTarget[i]->AddAttrib(_USERATTRIB_LIFE, -1 * nLifeLost, SYNCHRO_TRUE);
                if (setTarget[i]->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && pNpc->IsAwardScore())
                {
                    m_pOwner->AwardSynWarScore(pNpc, nLifeLost);
                }
                setTarget[i]->BeAttack(HitByMagic(), m_pOwner, setPower[i]);
            }
            if (setTarget[i]->IsMonster() || pNpc && pNpc->IsGoal() && m_pOwner->GetLev() >= pNpc->GetLev())
            {
                nExp += this->m_pOwner->AdjustExp(setTarget[i], nLifeLost);
                if (!setTarget[i]->IsAlive())
                {
                    // 有组队，并且队伍中有其他队员，则奖励其他队员经验
                    int	nBonusExp	= setTarget[i]->GetMaxLife() * KILLBONUS_PERCENT / 100;
                    OtherMemberAwardExp(setTarget[i], nBonusExp);
                    nExp += m_pOwner->AdjustExp(setTarget[i], nBonusExp, true);
                }
            }
        }
        if (m_pMagic->GetSTrackAmount() > 0)
        {
            AdjustPosByTrack(k, bAtkerPenetrable);
        }
        // 必须先发轨迹后发kill消息
        for (int i = 0; i < setTarget.size(); i++)
        {
            if (!setTarget[i]->IsAlive())
            {
                m_pOwner->Kill(setTarget[i], GetDieMode());
            }
        }
    }
    if (HitByWeapon() && !m_pOwner->GetMap()->IsTrainMap())
    {
        m_pOwner->AddEquipmentDurability(ITEMPOSITION_WEAPONR, -1 * LINE_WEAPON_DURABILITY);
        m_pOwner->AddEquipmentDurability(ITEMPOSITION_WEAPONL, -1 * LINE_WEAPON_DURABILITY);
    }
    IStatus* pStatus = m_pOwner->QueryStatus(STATUS_DMG2LIFE);
    if (pStatus)
    {
        int nLifeGot = ::CutTrail(0, MulDiv(nPowerSum, pStatus->GetPower(), 100));
        if (nLifeGot > 0)
        {
            m_pOwner->AddAttrib(_USERATTRIB_LIFE, nLifeGot, SYNCHRO_TRUE);
        }
    }
    AwardExp(nExp);
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessFan()
{
    CHECKF(m_pMagic);
    MAPTHING_SET	setTarget;
    vector<IRole*>	setRole;
    vector<int>		setPower;
    POINT	pos;	// source of fan
    int	nRange	= m_pMagic->GetInt(MAGICDATA_RANGE) + MAX_SIZEADD;
    int nWidth	= m_pMagic->GetInt(MAGICDATA_WIDTH);
    if (!nWidth)
    {
        nWidth = DEFAULT_MAGIC_FAN;
    }
    int	nSize		= nRange * 2 + 1;
    int	nBufSize	= nSize * nSize;
    int	nExp = 0;
    int nPowerSum = 0;
    for (int k = 0; k < m_nApplyTimes; k++)
    {
        // 搜索目标集
        {
            m_setTargetLocked.clear();
            setTarget.clear();
            setRole.clear();
            setPower.clear();
            if (m_pMagic->GetInt(MAGICDATA_GROUND))
            {
                pos.x		= m_pOwner->GetPosX();
                pos.y		= m_pOwner->GetPosY();
            }
            else
            {
                IRole* pTarget = m_pOwner->FindAroundRole(m_idTarget);
                if (!pTarget || !pTarget->IsAlive())
                {
                    return false;
                }
                pos.x		= pTarget->GetPosX();
                pos.y		= pTarget->GetPosY();
                // 只锁定一个攻击目标
                setRole.push_back(pTarget);
                m_setTargetLocked.push_back(pTarget->GetID());
            }
            m_pOwner->GetMap()->CollectMapThing(setTarget, pos, nRange, OBJ_USER | OBJ_MONSTER | OBJ_NPC);
            for(int i = CutTrail<int>(pos.x - nRange, 0); i <= pos.x + nRange && i < m_pOwner->GetMap()->GetWidth(); i++)
            {
                for(int j = CutTrail<int>(pos.y - nRange, 0); j <= pos.y + nRange && j < m_pOwner->GetMap()->GetHeight(); j++)
                {
                    POINT posThis;
                    posThis.x	= i;
                    posThis.y	= j;
                    if (!m_pOwner->IsInFan(posThis, pos, nRange, nWidth, m_pos))	// m_pos: target pos
                    {
                        continue;
                    }
                    int idx = POS2INDEX(posThis.x - pos.x + nRange, posThis.y - pos.y + nRange, nSize, nSize);
                    ASSERT(idx >= 0 && idx < setTarget.size());
                    if (setTarget[idx])
                    {
                        IRole* pRole = NULL;
                        if (setTarget[idx]->QueryRole(IPP_OF(pRole)) && pRole->IsAlive() && !IsImmunity(pRole)
                          )	//&& m_pOwner->GetDistance(pRole->QueryMapThing()) <= m_pOwner->GetAttackRange(pRole->GetSizeAdd()))
                        {
                            //if (nLoseLife)
                            {
                                if (pRole->GetID() != m_idTarget)	// m_idTarget如果存在前面已经push到setRole中了
                                {
                                    setRole.push_back(pRole);
                                }
                            }
                        }
                    }
                }
            }
        }
        // 锁定目标
        if (m_pMagic->GetSTrackAmount() > 0)
        {
            LockTarget(true);
        }
        // 广播法术效果
        CMsgMagicEffect	msg;
        IF_OK(msg.CreateByPos(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetInt(MAGICDATA_LEVEL), pos.x, pos.y, m_pOwner->GetDir()))
        {
            for(int i = 0; i < setRole.size(); i++)
            {
                IRole* pRole = setRole[i];
                if (pRole && pRole->IsAlive())
                {
                    int nPower = CBattleSystem::CalcPower(HitByMagic(), m_pOwner, pRole, GetPower(k));
                    setPower.push_back(nPower);
                    msg.AppendRole(pRole->GetID(), nPower);
                }
                else
                {
                    setPower.push_back(0);
                }
            }
            m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
        }
        // set crime status
        CheckCrime(&setRole);
        for (int i = 0; i < setRole.size(); i++)
        {
            if (!setRole[i]->IsAlive())
            {
                continue;
            }
            nPowerSum += setPower[i];
            CNpc* pNpc = NULL;
            int nLifeLost = setPower[i];
            if (setPower[i] && setRole[i]->TransferShield(HitByMagic(), m_pOwner, setPower[i]))
            {
            }
            else
            {
                nLifeLost = __min(setRole[i]->GetLife(), setPower[i]);
                setRole[i]->AddAttrib(_USERATTRIB_LIFE, -1 * nLifeLost, SYNCHRO_TRUE);
                if (setRole[i]->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && pNpc->IsAwardScore())
                {
                    m_pOwner->AwardSynWarScore(pNpc, nLifeLost);
                }
                setRole[i]->BeAttack(HitByMagic(), m_pOwner, setPower[i]);
            }
            if (setRole[i]->IsMonster() || pNpc && pNpc->IsGoal() && m_pOwner->GetLev() >= pNpc->GetLev())
            {
                nExp += this->m_pOwner->AdjustExp(setRole[i], nLifeLost);
                if (!setRole[i]->IsAlive())
                {
                    // 有组队，并且队伍中有其他队员，则奖励其他队员经验
                    int	nBonusExp	= setRole[i]->GetMaxLife() * KILLBONUS_PERCENT / 100;
                    OtherMemberAwardExp(setRole[i], nBonusExp);
                    nExp += m_pOwner->AdjustExp(setRole[i], nBonusExp, true);
                }
            }
        }
        if (m_pMagic->GetSTrackAmount() > 0)
        {
            AdjustPosByTrack(k);
        }
        // 必须先发轨迹后发kill消息
        for (int i = 0; i < setRole.size(); i++)
        {
            if (!setRole[i]->IsAlive())
            {
                m_pOwner->Kill(setRole[i], GetDieMode());
            }
        }
    }
    IStatus* pStatus = m_pOwner->QueryStatus(STATUS_DMG2LIFE);
    if (pStatus)
    {
        int nLifeGot = ::CutTrail(0, MulDiv(nPowerSum, pStatus->GetPower(), 100));
        if (nLifeGot > 0)
        {
            m_pOwner->AddAttrib(_USERATTRIB_LIFE, nLifeGot, SYNCHRO_TRUE);
        }
    }
    AwardExp(nExp);
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessBomb()
{
    CHECKF(m_pMagic);
    ROLE_SET		setRole;
    vector<int>		setPower;
    POINT	pos;
    int	nExp = 0;
    int nPowerSum = 0;
    for (int k = 0; k < m_nApplyTimes; k++)
    {
        // 搜索目标集
        {
            setRole.clear();
            setPower.clear();
            CollectTargetSet_Bomb(setRole, pos, LOCK_NONE);
        }
        // 锁定目标
        if (m_pMagic->GetSTrackAmount() > 0)
        {
            LockTarget(true);    // 可以重复调用
        }
        // 广播法术效果，计算伤害
        CMsgMagicEffect	msg;
        IF_OK(msg.CreateByPos(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetInt(MAGICDATA_LEVEL), pos.x, pos.y, m_pOwner->GetDir()))
        {
            for(int i = 0; i < setRole.size(); i++)
            {
                IRole* pRole = setRole[i];
                if (pRole && pRole->IsAlive())
                {
                    int nPower = CBattleSystem::CalcPower(HitByMagic(), m_pOwner, pRole, GetPower(k));
                    setPower.push_back(nPower);
                    msg.AppendRole(pRole->GetID(), nPower);
                }
                else
                {
                    setPower.push_back(0);
                }
            }
            m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
        }
        // set crime status
        CheckCrime(&setRole);
        // 伤害
        for (int i = 0; i < setRole.size(); i++)
        {
            if (!setRole[i]->IsAlive())
            {
                continue;
            }
            nPowerSum += setPower[i];
            CNpc* pNpc = NULL;
            int nLifeLost = setPower[i];
            if (setPower[i] && setRole[i]->TransferShield(HitByMagic(), m_pOwner, setPower[i]))
            {
            }
            else
            {
                nLifeLost = __min(setRole[i]->GetLife(), setPower[i]);
                setRole[i]->AddAttrib(_USERATTRIB_LIFE, -1 * nLifeLost, SYNCHRO_TRUE);
                if (setRole[i]->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && pNpc->IsAwardScore())
                {
                    m_pOwner->AwardSynWarScore(pNpc, nLifeLost);
                }
                setRole[i]->BeAttack(HitByMagic(), m_pOwner, setPower[i]);
            }
            if (setRole[i]->IsMonster() || pNpc && pNpc->IsGoal() && m_pOwner->GetLev() >= pNpc->GetLev())
            {
                nExp += this->m_pOwner->AdjustExp(setRole[i], nLifeLost);
                if (!setRole[i]->IsAlive())
                {
                    // 有组队，并且队伍中有其他队员，则奖励其他队员经验
                    int	nBonusExp	= setRole[i]->GetMaxLife() * KILLBONUS_PERCENT / 100;
                    OtherMemberAwardExp(setRole[i], nBonusExp);
                    nExp += m_pOwner->AdjustExp(setRole[i], nBonusExp, true);
                }
            }
        }
        // 调整移动轨迹
        if (m_pMagic->GetSTrackAmount() > 0)
        {
            AdjustPosByTrack(k);
        }
        // 必须先发轨迹后发kill消息
        for (int i = 0; i < setRole.size(); i++)
        {
            if (!setRole[i]->IsAlive())
            {
                m_pOwner->Kill(setRole[i], GetDieMode());
            }
        }
    }
    IStatus* pStatus = m_pOwner->QueryStatus(STATUS_DMG2LIFE);
    if (pStatus)
    {
        int nLifeGot = ::CutTrail(0, MulDiv(nPowerSum, pStatus->GetPower(), 100));
        if (nLifeGot > 0)
        {
            m_pOwner->AddAttrib(_USERATTRIB_LIFE, nLifeGot, SYNCHRO_TRUE);
        }
    }
    AwardExp(nExp);
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessBombLockAll()
{
    CHECKF(m_pMagic);
    POINT	pos;
    ROLE_SET		setRole;
    vector<int>		setPower;
    // 收集目标集
    CollectTargetSet_Bomb(setRole, pos, LOCK_ALL);
    if (m_pMagic->GetSTrackAmount() > 0)
    {
        LockTarget(true);
    }
    int	nExp = 0;
    int nPowerSum = 0;
    for (int k = 0; k < m_nApplyTimes; k++)
    {
        // 广播法术效果
        CMsgMagicEffect	msg;
        IF_OK(msg.CreateByPos(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetInt(MAGICDATA_LEVEL), pos.x, pos.y, m_pOwner->GetDir()))
        {
            for(int i = 0; i < setRole.size(); i++)
            {
                IRole* pRole = setRole[i];
                if (pRole && pRole->IsAlive())
                {
                    int nPower = CBattleSystem::CalcPower(HitByMagic(), m_pOwner, pRole, GetPower(k));
                    setPower.push_back(nPower);
                    msg.AppendRole(pRole->GetID(), nPower);
                }
                else
                {
                    setPower.push_back(0);
                }
            }
            m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
        }
        // set crime status
        CheckCrime(&setRole);
        for (int i = 0; i < setRole.size(); i++)
        {
            if (!setRole[i]->IsAlive())
            {
                continue;
            }
            nPowerSum += setPower[i];
            CNpc* pNpc = NULL;
            int nLifeLost = setPower[i];
            if (setPower[i] && setRole[i]->TransferShield(HitByMagic(), m_pOwner, setPower[i]))
            {
            }
            else
            {
                nLifeLost = __min(setRole[i]->GetLife(), setPower[i]);
                setRole[i]->AddAttrib(_USERATTRIB_LIFE, -1 * nLifeLost, SYNCHRO_TRUE);
                if (setRole[i]->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && pNpc->IsAwardScore())
                {
                    m_pOwner->AwardSynWarScore(pNpc, nLifeLost);
                }
                setRole[i]->BeAttack(HitByMagic(), m_pOwner, setPower[i]);
            }
            if (setRole[i]->IsMonster() || pNpc && pNpc->IsGoal() && m_pOwner->GetLev() >= pNpc->GetLev())
            {
                nExp += this->m_pOwner->AdjustExp(setRole[i], nLifeLost);
                if (!setRole[i]->IsAlive())
                {
                    // 有组队，并且队伍中有其他队员，则奖励其他队员经验
                    int	nBonusExp	= setRole[i]->GetMaxLife() * KILLBONUS_PERCENT / 100;
                    OtherMemberAwardExp(setRole[i], nBonusExp);
                    nExp += m_pOwner->AdjustExp(setRole[i], nBonusExp, true);
                }
            }
        }
        if (m_pMagic->GetSTrackAmount() > 0)
        {
            AdjustPosByTrack(k);
        }
        // 必须先发轨迹后发kill消息
        for (int i = 0; i < setRole.size(); i++)
        {
            if (!setRole[i]->IsAlive())
            {
                m_pOwner->Kill(setRole[i], GetDieMode());
            }
        }
    }
    IStatus* pStatus = m_pOwner->QueryStatus(STATUS_DMG2LIFE);
    if (pStatus)
    {
        int nLifeGot = ::CutTrail(0, MulDiv(nPowerSum, pStatus->GetPower(), 100));
        if (nLifeGot > 0)
        {
            m_pOwner->AddAttrib(_USERATTRIB_LIFE, nLifeGot, SYNCHRO_TRUE);
        }
    }
    AwardExp(nExp);
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessSquare()
{
    CHECKF(m_pMagic);
    MAPTHING_SET	setTarget;
    vector<IRole*>	setRole;
    vector<int>		setPower;
    POINT	pos;
    int	nRange	= m_pMagic->GetInt(MAGICDATA_RANGE);
    if (HitByWeapon())
    {
        nRange	= m_pOwner->GetAttackRange(MAX_SIZEADD);
    }
    int	nSize		= nRange * 2 + 1;
    int	nBufSize	= nSize * nSize;
    int	nExp = 0;
    int nPowerSum = 0;
    for (int k = 0; k < m_nApplyTimes; k++)
    {
        // 搜索目标集
        {
            m_setTargetLocked.clear();
            setTarget.clear();
            setRole.clear();
            setPower.clear();
            int nDir	= m_pOwner->GetDir();
            if (m_pMagic->GetInt(MAGICDATA_GROUND))
            {
                pos.x		= m_pos.x;
                pos.y		= m_pos.y;
            }
            else
            {
                IRole* pTarget = m_pOwner->FindAroundRole(m_idTarget);
                if (!pTarget || !pTarget->IsAlive())
                {
                    return false;
                }
                pos.x		= pTarget->GetPosX();
                pos.y		= pTarget->GetPosY();
            }
            m_pOwner->GetMap()->CollectMapThing(setTarget, pos, nRange, OBJ_USER | OBJ_MONSTER | OBJ_NPC);
            for(int i = ::CutTrail<int>(pos.x - nRange, 0); i <= pos.x + nRange && i < m_pOwner->GetMap()->GetWidth(); i++)
            {
                for(int j = ::CutTrail<int>(pos.y - nRange, 0); j <= pos.y + nRange && j < m_pOwner->GetMap()->GetHeight(); j++)
                {
                    POINT posThis;
                    posThis.x	= i;
                    posThis.y	= j;
                    int idx = POS2INDEX(posThis.x - pos.x + nRange, posThis.y - pos.y + nRange, nSize, nSize);
                    ASSERT(idx >= 0 && idx < setTarget.size());
                    if (setTarget[idx])
                    {
                        IRole* pRole = NULL;
                        if (setTarget[idx]->QueryRole(IPP_OF(pRole)) && pRole->IsAlive() && !IsImmunity(pRole)
                                && m_pOwner->GetDistance(pRole->QueryMapThing()) <= m_pOwner->GetAttackRange(pRole->GetSizeAdd()))
                        {
                            //if (nLoseLife)
                            {
                                setRole.push_back(pRole);
                                // 只锁定一个攻击目标
                                if (pRole->GetID() == m_idTarget)
                                {
                                    m_setTargetLocked.push_back(pRole->GetID());
                                }
                            }
                        }
                    }
                }
            }
        }
        // 锁定目标
        if (m_pMagic->GetSTrackAmount() > 0)
        {
            LockTarget(true);
        }
        // 广播法术效果
        CMsgMagicEffect	msg;
        IF_OK(msg.CreateByPos(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetInt(MAGICDATA_LEVEL), pos.x, pos.y, m_pOwner->GetDir()))
        {
            for(int i = 0; i < setRole.size(); i++)
            {
                IRole* pRole = setRole[i];
                if (pRole && pRole->IsAlive())
                {
                    int nPower = CBattleSystem::CalcPower(HitByMagic(), m_pOwner, pRole, GetPower(k));
                    setPower.push_back(nPower);
                    msg.AppendRole(pRole->GetID(), nPower);
                }
                else
                {
                    setPower.push_back(0);
                }
            }
            m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
        }
        // set crime status
        CheckCrime(&setRole);
        for (int i = 0; i < setRole.size(); i++)
        {
            if (!setRole[i]->IsAlive())
            {
                continue;
            }
            nPowerSum += setPower[i];
            CNpc* pNpc = NULL;
            int nLifeLost = setPower[i];
            if (setPower[i] && setRole[i]->TransferShield(HitByMagic(), m_pOwner, setPower[i]))
            {
            }
            else
            {
                nLifeLost = __min(setRole[i]->GetLife(), setPower[i]);
                setRole[i]->AddAttrib(_USERATTRIB_LIFE, -1 * nLifeLost, SYNCHRO_TRUE);
                if (setRole[i]->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && pNpc->IsAwardScore())
                {
                    m_pOwner->AwardSynWarScore(pNpc, nLifeLost);
                }
                setRole[i]->BeAttack(HitByMagic(), m_pOwner, setPower[i]);
            }
            if (setRole[i]->IsMonster() || pNpc && pNpc->IsGoal() && m_pOwner->GetLev() >= pNpc->GetLev())
            {
                nExp += this->m_pOwner->AdjustExp(setRole[i], nLifeLost);
                if (!setRole[i]->IsAlive())
                {
                    // 有组队，并且队伍中有其他队员，则奖励其他队员经验
                    int	nBonusExp	= setRole[i]->GetMaxLife() * KILLBONUS_PERCENT / 100;
                    OtherMemberAwardExp(setRole[i], nBonusExp);
                    nExp += m_pOwner->AdjustExp(setRole[i], nBonusExp, true);
                }
            }
        }
        if (m_pMagic->GetSTrackAmount() > 0)
        {
            AdjustPosByTrack(k);
        }
        // 必须先发轨迹后发kill消息
        for (int i = 0; i < setRole.size(); i++)
        {
            if (!setRole[i]->IsAlive())
            {
                m_pOwner->Kill(setRole[i], GetDieMode());
            }
        }
    }
    IStatus* pStatus = m_pOwner->QueryStatus(STATUS_DMG2LIFE);
    if (pStatus)
    {
        int nLifeGot = ::CutTrail(0, MulDiv(nPowerSum, pStatus->GetPower(), 100));
        if (nLifeGot > 0)
        {
            m_pOwner->AddAttrib(_USERATTRIB_LIFE, nLifeGot, SYNCHRO_TRUE);
        }
    }
    AwardExp(nExp);
    //? 未增加武器熟练度，也未损耗双方装备。
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessAttach()
{
    CHECKF(m_pMagic);
    m_setTargetLocked.clear();
    IRole* pRole = m_pOwner->FindAroundRole(m_idTarget);
    if (!pRole || !pRole->IsAlive())
    {
        return false;
    }
    //	if (m_pOwner->GetDistance(pRole->GetPosX(), pRole->GetPosY()) > m_pMagic->GetInt(MAGICDATA_DISTANCE))
    //		return false;
    int nPower	= GetPower();
    int nSecs	= m_pMagic->GetInt(MAGICDATA_STEP);
    int	nTimes	= m_pMagic->GetInt(MAGICDATA_TIMES);
    DWORD nStatus	= m_pMagic->GetInt(MAGICDATA_STATUS);
    ASSERT(nPower > 0);
    //	m_setTargetLocked.push_back(m_idTarget);
    // 广播法术效果
    CMsgMagicEffect	msg;
    IF_OK(msg.Create(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetInt(MAGICDATA_LEVEL), m_idTarget, 0, m_pOwner->GetDir()))
    m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
    // set crime status
    CheckCrime(pRole);
    // user status only
    void* pTemp;
    if (!pRole->QueryObj(OBJ_USER, IPP_OF(pTemp))
            && (nStatus == STATUS_LURKER /*|| nStatus == STATUS_ATKRANGE*/ || nStatus == STATUS_REFLECT || nStatus == STATUS_REFLECTMAGIC))
    {
        return false;
    }
    if (nStatus == STATUS_LURKER || nStatus == STATUS_ATKSPEED)	// 不可叠加
    {
        CRole::DetachWellStatus(pRole);
    }
    // superman list
    //	if (nStatus == STATUS_SUPERMAN || nStatus == STATUS_TORNADO)
    //	{
    ////		m_pOwner->CalcSupermanKills();
    //		nSecs	+= m_pOwner->GetSupermanAddSecs();
    //	}
    // 4超级的特殊状态，及超级回复
    if (m_pMagic && m_pMagic->GetInt(MAGICDATA_TYPE) >= SUPER_MAGIC_DEFENCE && m_pMagic->GetInt(MAGICDATA_TYPE) <= SUPER_MAGIC_MGC_ATTACK)
    {
        nPower *= m_pOwner->GetSoulSum() / 2;
    }
    switch(m_pMagic->GetInt(MAGICDATA_TYPE))
    {
    case	SUPER_MAGIC_DEFENCE:
        {
            CRole::DetachStatus(m_pOwner, STATUS_SUPER_ATK);
            //CRole::DetachStatus(m_pOwner, STATUS_SUPER_DEF);
            CRole::DetachStatus(m_pOwner, STATUS_SUPER_MATK);
            CRole::DetachStatus(m_pOwner, STATUS_SUPER_MDEF);
        }
        break;
    case	SUPER_MAGIC_MGC_DEFENCE:
        {
            CRole::DetachStatus(m_pOwner, STATUS_SUPER_ATK);
            CRole::DetachStatus(m_pOwner, STATUS_SUPER_DEF);
            CRole::DetachStatus(m_pOwner, STATUS_SUPER_MATK);
            //CRole::DetachStatus(m_pOwner, STATUS_SUPER_MDEF);
        }
        break;
    case	SUPER_MAGIC_ATTACK:
        {
            //CRole::DetachStatus(m_pOwner, STATUS_SUPER_ATK);
            CRole::DetachStatus(m_pOwner, STATUS_SUPER_DEF);
            CRole::DetachStatus(m_pOwner, STATUS_SUPER_MATK);
            CRole::DetachStatus(m_pOwner, STATUS_SUPER_MDEF);
        }
        break;
    case	SUPER_MAGIC_MGC_ATTACK:
        {
            CRole::DetachStatus(m_pOwner, STATUS_SUPER_ATK);
            CRole::DetachStatus(m_pOwner, STATUS_SUPER_DEF);
            //CRole::DetachStatus(m_pOwner, STATUS_SUPER_MATK);
            CRole::DetachStatus(m_pOwner, STATUS_SUPER_MDEF);
        }
        break;
    }
    // 疾行技能特殊处理
    if (nStatus == STATUS_PELT && pRole->QueryStatus(STATUS_PELT))
    {
        CRole::DetachStatus(pRole, nStatus);
    }
    else
    {
        CRole::AttachStatus(pRole, nStatus, nPower, nSecs, nTimes);
    }
    AwardExp(0);		// false: not award battle exp
    // hint
    if (nPower >= 30000)
    {
        int nPowerTimes = (nPower - 30000) / 100;
        MSGBUF	szMsg;
        switch(nStatus)
        {
            //		case	STATUS_HITRATE:
            //			{
            //				sprintf(szMsg, STR_HITRATE_ADD, nSecs, nPowerTimes);
            //				pRole->SendSysMsg(szMsg);
            //			}
            //			break;
        case	STATUS_DEFENCE1:
        case	STATUS_DEFENCE2:
        case	STATUS_DEFENCE3:
            {
                sprintf(szMsg, STR_DEFENCE_ADD, nSecs, nPowerTimes);
                pRole->SendSysMsg(szMsg);
            }
            break;
        case	STATUS_ATTACK:
            {
                sprintf(szMsg, STR_ATTACK_ADD, nSecs, nPowerTimes);
                pRole->SendSysMsg(szMsg);
            }
            break;
        }
    }
    else
    {
        int nPowerTimes = nPower;
        MSGBUF	szMsg;
        switch(nStatus)
        {
            //		case	STATUS_HITRATE:
            //			{
            //				sprintf(szMsg, STR_HITRATE_ADD2, nSecs, nPowerTimes);
            //				pRole->SendSysMsg(szMsg);
            //			}
            //			break;
        case	STATUS_DEFENCE1:
        case	STATUS_DEFENCE2:
        case	STATUS_DEFENCE3:
            {
                sprintf(szMsg, STR_DEFENCE_ADD2, nSecs, nPowerTimes);
                pRole->SendSysMsg(szMsg);
            }
            break;
        case	STATUS_ATTACK:
            {
                sprintf(szMsg, STR_ATTACK_ADD2, nSecs, nPowerTimes);
                pRole->SendSysMsg(szMsg);
            }
            break;
        case	STATUS_SUPER_ATK:
            {
                sprintf(szMsg, STR_SUPER_ATK_ADD, nSecs, nPowerTimes);
                pRole->SendSysMsg(szMsg);
            }
            break;
        case	STATUS_SUPER_DEF:
            {
                sprintf(szMsg, STR_SUPER_DEF_ADD, nSecs, nPowerTimes);
                pRole->SendSysMsg(szMsg);
            }
            break;
        case	STATUS_SUPER_MATK:
            {
                sprintf(szMsg, STR_SUPER_MATK_ADD, nSecs, nPowerTimes);
                pRole->SendSysMsg(szMsg);
            }
            break;
        case	STATUS_SUPER_MDEF:
            {
                sprintf(szMsg, STR_SUPER_MDEF_ADD, nSecs, nPowerTimes);
                pRole->SendSysMsg(szMsg);
            }
            break;
        }
    }
    // 组队界结提示
    if (m_pMagic->GetInt(MAGICDATA_SORT) == MAGICSORT_TEAM_MAGIC)
    {
        CUser* pUser = NULL;
        if (m_pOwner->QueryObj(OBJ_USER, IPP_OF(pUser)))
        {
            CTeam* pTeam = pUser->GetTeam();
            if (pTeam && pTeam->GetMemberAmount() >= TEAM_STATUS_REQ_ROLES)
            {
                char szMsg[256] = "";
                if (pTeam->GetMemberAmount() == TEAM_STATUS_REQ_ROLES)
                {
                    sprintf(szMsg, STR_TEAM_MAGIC, m_pMagic->GetName());
                }
                else
                {
                    sprintf(szMsg, STR_TEAM_MAGIC2, m_pMagic->GetName(), 10 * (pTeam->GetMemberAmount() - TEAM_STATUS_REQ_ROLES));
                }
                CMsgTalk msg;
                if (msg.Create(SYSTEM_NAME, ALLUSERS_NAME, szMsg, NULL, 0xff0000, _TXTATR_SYSTEM))
                {
                    pTeam->BroadcastTeamMsg(&msg);
                }
            }
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessDetach()
{
    CHECKF(m_pMagic);
    m_setTargetLocked.clear();
    IRole* pRole = m_pOwner->FindAroundRole(m_idTarget);
    if (!pRole)
    {
        return false;
    }
    int nPower	= 0;		//解状态 GetPower();
    int nSecs	= m_pMagic->GetInt(MAGICDATA_STEP);
    int	nTimes	= m_pMagic->GetInt(MAGICDATA_TIMES);
    int nStatus	= m_pMagic->GetInt(MAGICDATA_STATUS);
    ASSERT(nPower == 0);	// 0 : 减少
    if (!pRole->IsAlive())
    {
        if (STATUS_DIE != nStatus)
        {
            return false;
        }
        void* pTemp;
        if (!pRole->QueryObj(OBJ_USER, IPP_OF(pTemp)))
        {
            return false;
        }
    }
    // pk field will disable reborn magic
    if (STATUS_DIE == nStatus)
    {
        CGameMap* pMap = m_pOwner->GetMap();
        if (pMap && pMap->IsPkField())
        {
            return false;
        }
    }
    if (nStatus == STATUS_DETACH_ALL)
    {
        CRole::DetachAllStatus(pRole);
    }
    else if (nStatus == STATUS_DETACH_BADLY)
    {
        CRole::DetachBadlyStatus(pRole);
    }
    else
    {
        IStatus* pStatus = pRole->QueryStatus(nStatus);
        if (pStatus)
        {
            pStatus->ChangeData(nPower, nSecs, nTimes);
        }
    }
    //	m_setTargetLocked.push_back(m_idTarget);
    // 广播法术效果
    CMsgMagicEffect	msg;
    IF_OK(msg.Create(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetInt(MAGICDATA_LEVEL), m_idTarget, nStatus, m_pOwner->GetDir()))
    m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
    // set crime status
    CheckCrime(pRole);
    pRole->ClsStatus(nStatus, SYNCHRO_TRUE);
    if (STATUS_DIE == nStatus)
    {
        // reborn
        CUser* pUser = NULL;
        pRole->QueryObj(OBJ_USER, IPP_OF(pUser));
        if (pUser)
        {
            bool bChgPos = false;
            pUser->Reborn(bChgPos);
        }
    }
    AwardExp(0);
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessJumpAttackFaild(int x, int y)
{
    CHECKF(m_pMagic);
    m_setTargetLocked.clear();
    int		nPower		= 0;
    OBJID	idTarget	= ID_NONE;
    // 广播法术效果
    CMsgMagicEffect	msg;
    IF_OK(msg.CreateByPos(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetInt(MAGICDATA_LEVEL), x, y, idTarget, nPower, m_pOwner->GetDir()))
    m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
    m_pOwner->ProcessOnMove(MOVEMODE_JUMPMAGICATTCK);
    m_pOwner->JumpPos(x, y);
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessJumpAtk()
{
    CHECKF(m_pMagic);
    m_setTargetLocked.clear();
    //	if (m_pOwner->GetDistance(m_pos.x, m_pos.y) > m_pMagic->GetInt(MAGICDATA_DISTANCE))
    //	{
    //		::GmLogSave("玩家[%s]XP技跳砍的距离非法!", m_pOwner->GetName());
    //		return false;
    //	}
    POINT	pos;
    IRole* pTarget	= NULL;
    if (m_pMagic->GetInt(MAGICDATA_GROUND))
    {
        pos.x		= m_pos.x;
        pos.y		= m_pos.y;
    }
    else
    {
        pTarget = m_pOwner->FindAroundRole(m_idTarget);
        if (!pTarget || !pTarget->IsAlive())
        {
            return false;
        }
        if (pTarget->GetDistance(m_pos.x, m_pos.y) > m_pOwner->GetAttackRange(pTarget->GetSizeAdd()))
        {
            return false;
        }
        if (this->IsImmunity(pTarget))
        {
            return false;
        }
        pos.x		= pTarget->GetPosX();
        pos.y		= pTarget->GetPosY();
    }
    //	if (m_pOwner->GetDistance(pos.x, pos.y) > m_pMagic->GetInt(MAGICDATA_DISTANCE))
    //		return false;
    int	nRange	= m_pMagic->GetInt(MAGICDATA_RANGE);
    int	nPower	= 0;
    if (pTarget)
    {
        nPower = CBattleSystem::CalcPower(HitByMagic(), m_pOwner, pTarget, GetPower());
    }
    //	m_setTargetLocked.push_back(m_idTarget);
    // 广播法术效果
    CMsgMagicEffect	msg;
    IF_OK(msg.CreateByPos(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetInt(MAGICDATA_LEVEL), m_pos.x, m_pos.y, m_idTarget, nPower, m_pOwner->GetDir()))
    m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
    // set crime status
    if (pTarget)
    {
        CheckCrime(pTarget);
    }
    // move!!! 攻击方需要锁定坐标
    m_pOwner->ProcessOnMove(MOVEMODE_JUMPMAGICATTCK);
    m_pOwner->JumpPos(m_pos.x, m_pos.y);
    if (nPower && pTarget)
    {
        CNpc* pNpc = NULL;
        int nLifeLost = nPower;
        if (pTarget->TransferShield(HitByMagic(), m_pOwner, nPower))
        {
        }
        else
        {
            nLifeLost = __min(pTarget->GetLife(), nPower);
            pTarget->AddAttrib(_USERATTRIB_LIFE, -1 * nLifeLost, SYNCHRO_TRUE);
            if (pTarget->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && pNpc->IsAwardScore())
            {
                m_pOwner->AwardSynWarScore(pNpc, nLifeLost);
            }
            pTarget->BeAttack(HitByMagic(), m_pOwner, nPower);
        }
        if (pTarget->IsMonster() || pNpc && pNpc->IsGoal() && m_pOwner->GetLev() >= pNpc->GetLev())
        {
            // correct exp here
            int nExp = this->m_pOwner->AdjustExp(pTarget, nLifeLost);
            // kill bonus
            if (!pTarget->IsAlive()) // target was killed
            {
                // 有组队，并且队伍中有其他队员，则奖励其他队员经验
                int	nBonusExp	= pTarget->GetMaxLife() * KILLBONUS_PERCENT / 100;
                OtherMemberAwardExp(pTarget, nBonusExp);
                nExp += m_pOwner->AdjustExp(pTarget, nBonusExp, true);
            }
            this->AwardExp(nExp);
        }
        if (!pTarget->IsAlive())
        {
            m_pOwner->Kill(pTarget, GetDieMode());
        }
        IStatus* pStatus = m_pOwner->QueryStatus(STATUS_DMG2LIFE);
        if (pStatus)
        {
            int nLifeGot = ::CutTrail(0, MulDiv(nPower, pStatus->GetPower(), 100));
            if (nLifeGot > 0)
            {
                m_pOwner->AddAttrib(_USERATTRIB_LIFE, nLifeGot, SYNCHRO_TRUE);
            }
        }
    }
    //? 未增加武器熟练度，也未损耗双方装备。
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessRandomTrans()
{
    CHECKF(m_pMagic);
    m_setTargetLocked.clear();
    int nWidth = GetPower() * 2 + 1;
    AwardExp(0);
    for(int i = 0; i < RANDOMTRANS_TRY_TIMES; i++)
    {
        int x = (int)m_pOwner->GetPosX() + nWidth / 2 -::RandGet(nWidth);
        int y = (int)m_pOwner->GetPosY() + nWidth / 2 -::RandGet(nWidth);
        if (m_pOwner->GetMap()->IsStandEnable(x, y))
        {
            // 广播法术效果
            CMsgMagicEffect	msg;
            IF_OK(msg.CreateByPos(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetInt(MAGICDATA_LEVEL), x, y, m_pOwner->GetDir()))
            m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
            // move!!! 攻击方需要锁定坐标
            m_pOwner->ProcessOnMove(MOVEMODE_TRANS);
            m_pOwner->TransPos(x, y);		//? may be optimize to JumpPos(,)
            m_pOwner->Synchro();
            return true;
        }
    }
    m_pOwner->SendSysMsg(STR_MAGIC_FAILED);
    return false;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessDispatchXP()
{
    CHECKF(m_pMagic);
    m_setTargetLocked.clear();
    vector<CUser*>	setUser;
    vector<int>		setPower;
    CTeam* pTeam = m_pOwner->GetTeam();
    if (pTeam)
    {
        MSGBUF	szMsg;
        sprintf(szMsg, STR_DISPATCHXP_s, m_pOwner->GetName());
        for(int i = 0; i < pTeam->GetMemberAmount(); i++)
        {
            OBJID idUser = pTeam->GetMemberByIndex(i);
            IRole* pRole = m_pOwner->FindAroundRole(idUser);
            CUser* pUser = NULL;
            if (pRole)
            {
                pRole->QueryObj(OBJ_USER, IPP_OF(pUser));
            }
            if (idUser != ID_NONE && pUser && pUser->IsAlive() && pUser->GetID() != m_pOwner->GetID())
            {
                pUser->AddXp(DISPATCHXP_NUMBER);
                setUser.push_back(pUser);
                setPower.push_back(DISPATCHXP_NUMBER);
                //				m_setTargetLocked.push_back(pUser->GetID());
                pUser->SendSysMsg(szMsg);
            }
        }
    }
    // 广播法术效果
    CMsgMagicEffect	msg;
    IF_OK(msg.CreateByPos(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetInt(MAGICDATA_LEVEL), m_pos.x, m_pos.y, m_pOwner->GetDir()))
    {
        for(int i = 0; i < setUser.size(); i++)
        {
            msg.AppendRole(setUser[i]->GetID(), setPower[i]);
        }
        m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
    }
    AwardExp(0);
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessCollideFail(int x, int y, int nDir)
{
    m_setTargetLocked.clear();
    // target data is dir
    int nTargetX	= m_pos.x + _DELTA_X[nDir];
    int nTargetY	= m_pos.y + _DELTA_Y[nDir];
    int	nPower		= 0;
    OBJID	idTarget	= ID_NONE;
    IF_NOT(m_pOwner->GetMap()->IsStandEnable(nTargetX, nTargetY))
    {
        m_pOwner->SendSysMsg(STR_INVALID_MSG);
        CUser* pUser = NULL;
        if (m_pOwner->QueryObj(OBJ_USER, IPP_OF(pUser)))
        {
            UserManager()->KickOutSocket(pUser->GetSocketID(), "COLLIDE coord!");
        }
        return false;
    }
    CMsgInteract msg;
    IF_OK (msg.Create(INTERACT_BUMP, m_pOwner->GetID(), idTarget, nTargetX, nTargetY, nDir * 0x01000000 + nPower))
    m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
    // move now...
    // if (!(m_pOwner->GetPosX() == m_pos.x && m_pOwner->GetPosY() == m_pos.y))
    {
        m_pOwner->ProcessOnMove(MOVEMODE_COLLIDE);
        m_pOwner->MoveToward(nDir);
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessCollide()
{
    CHECKF(m_pMagic);
    CHECKF(m_pOwner);
    m_setTargetLocked.clear();
    // check syn of user
    IF_NOT(m_pOwner->SynPosition(m_pos.x, m_pos.y))
    {
        CUser* pUser = NULL;
        if (m_pOwner->QueryObj(OBJ_USER, IPP_OF(pUser)))
        {
            UserManager()->KickOutSocket(pUser->GetSocketID(), "ProcessCollide SynPosition");
        }
        return false;
    }
    // target data is dir
    int nDir		= m_nData;
    int nTargetX	= m_pos.x + _DELTA_X[nDir];
    int nTargetY	= m_pos.y + _DELTA_Y[nDir];
    IF_NOT(m_pOwner->GetMap()->IsStandEnable(nTargetX, nTargetY))
    {
        m_pOwner->SendSysMsg(STR_INVALID_MSG);
        CUser* pUser = NULL;
        if (m_pOwner->QueryObj(OBJ_USER, IPP_OF(pUser)))
        {
            UserManager()->KickOutSocket(pUser->GetSocketID(), "COLLIDE coord!");
        }
        return false;
    }
    // search the target
    bool bSuc = true;
    IRole* pTarget = m_pOwner->GetMap()->QueryRole(nTargetX, nTargetY, ID_NONE);
    if (!pTarget || !pTarget->IsAlive())
    {
        bSuc = false;
    }
    if (!pTarget || this->IsImmunity(pTarget))
    {
        bSuc = false;
    }
    //if (m_pOwner->GetDistance(nTargetX, nTargetY) > m_pMagic->GetInt(MAGICDATA_DISTANCE))
    //	bSuc = false;
    int	nPower	= 0;
    bool bBackEnable = false;
    OBJID idTarget = ID_NONE;
    CNpc* pNpc = NULL;
    if (pTarget)
    {
        pTarget->QueryObj(OBJ_NPC, IPP_OF(pNpc));
        //		int nHitRate = 100;
        //		if (::RandGet(100) >= nHitRate)
        //			bSuc = false;
        if (bSuc)
        {
            bBackEnable	= pTarget->GetMap()->IsStandEnable(nTargetX + _DELTA_X[nDir], nTargetY + _DELTA_Y[nDir]);
            if (pNpc)
            {
                bBackEnable = false;
            }
            if (!bBackEnable)
            {
                nDir = MAX_DIRSIZE;
            }
            if (HitByWeapon())	// && !CBattleSystem::IsTargetDodged(m_pOwner, pTarget))
            {
                nPower += CBattleSystem::CalcAttackPower(m_pOwner, pTarget);
                if (bBackEnable || pNpc)
                {
                    nPower	= MulDiv(nPower, COLLIDE_POWER_PERCENT, 100);
                }
                else
                {
                    nPower	= m_pOwner->AdjustData(nPower, GetPower());
                }
            }
            if (!m_pOwner->GetMap()->IsTrainMap())
            {
                m_pOwner->AddEquipmentDurability(ITEMPOSITION_WEAPONL, -1 * COLLIDE_SHIELD_DURABILITY);
            }
        }
        idTarget = pTarget->GetID();
        //		m_setTargetLocked.push_back(idTarget);
    }
    CMsgInteract msg;
    DWORD dwData = nDir * 0x01000000 + nPower;
    if (!bBackEnable)
    {
        dwData += 1 * 0x10000000;
    }
    IF_OK (msg.Create(INTERACT_BUMP, m_pOwner->GetID(), idTarget, nTargetX, nTargetY, dwData))
    m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
    if (m_pOwner->IsPM())
    {
        CUser* pUser = NULL;
        if (m_pOwner->QueryObj(OBJ_USER, IPP_OF(pUser)))
            pUser->SendSysMsg(_TXTATR_NORMAL, "bump move:(%u, %u)->(%u, %u)",
                              pUser->GetPosX(), pUser->GetPosY(),
                              nTargetX, nTargetY);
    }
    // move now...
    // if (!(m_pOwner->GetPosX() == m_pos.x && m_pOwner->GetPosY() == m_pos.y))
    if (bBackEnable)
    {
        m_pOwner->ProcessOnMove(MOVEMODE_COLLIDE);
        m_pOwner->MoveToward(nDir);
    }
    if (pTarget && bBackEnable && bSuc)
    {
        pTarget->ProcessOnMove(MOVEMODE_COLLIDE);
        pTarget->MoveToward(nDir);
        // paled - set not synchro yet
        CUser* pTargetUser;
        if (pTarget->QueryObj(OBJ_USER, IPP_OF(pTargetUser)))
        {
            // syn pos of client
            pTargetUser->SetSynchro(false);
        }
    }
    // set crime status
    if (pTarget)
    {
        CheckCrime(pTarget);
    }
    if (nPower && pTarget)
    {
        CNpc* pNpc = NULL;
        int nLifeLost = nPower;
        if (pTarget->TransferShield(HitByMagic(), m_pOwner, nPower))
        {
        }
        else
        {
            nLifeLost = __min(pTarget->GetLife(), nPower);
            pTarget->AddAttrib(_USERATTRIB_LIFE, -1 * nLifeLost, SYNCHRO_TRUE);
            if (pTarget->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && pNpc->IsAwardScore())
            {
                m_pOwner->AwardSynWarScore(pNpc, nLifeLost);
            }
            pTarget->BeAttack(HitByMagic(), m_pOwner, nPower);
        }
        if (pTarget->IsMonster() || pNpc && pNpc->IsGoal() && m_pOwner->GetLev() >= pNpc->GetLev())
        {
            // correct exp here
            int nExp = this->m_pOwner->AdjustExp(pTarget, nLifeLost);
            // kill bonus
            if (!pTarget->IsAlive()) // target was killed
            {
                // 有组队，并且队伍中有其他队员，则奖励其他队员经验
                int	nBonusExp	= pTarget->GetMaxLife() * KILLBONUS_PERCENT / 100;
                OtherMemberAwardExp(pTarget, nBonusExp);
                nExp += m_pOwner->AdjustExp(pTarget, nBonusExp, true);
            }
            this->AwardExp(nExp);
        }
        if (!pTarget->IsAlive())
        {
            m_pOwner->Kill(pTarget, GetDieMode());
        }
        IStatus* pStatus = m_pOwner->QueryStatus(STATUS_DMG2LIFE);
        if (pStatus)
        {
            int nLifeGot = ::CutTrail(0, MulDiv(nPower, pStatus->GetPower(), 100));
            if (nLifeGot > 0)
            {
                m_pOwner->AddAttrib(_USERATTRIB_LIFE, nLifeGot, SYNCHRO_TRUE);
            }
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessSerialCut()
{
    CHECKF(m_pMagic);
    m_setTargetLocked.clear();
    POINT pos;
    pos.x	= m_pos.x;
    pos.y	= m_pos.y;
    int	nRange	= 1;
    int	nSize		= nRange * 2 + 1;
    int	nBufSize	= nSize * nSize;
    vector<IRole*>	setRole;
    vector<int>		setPower;
    for(int i = 0; i < MAX_SERIALCUTSIZE; i++)
    {
        MAPTHING_SET	setTarget;
        m_pOwner->GetMap()->CollectMapThing(setTarget, pos, nRange, OBJ_USER | OBJ_MONSTER | OBJ_NPC);
        IRole* pTarget = NULL;
        for(int j = 0; j < setTarget.size(); j++)
        {
            int idx = j;
            ASSERT(idx >= 0 && idx < setTarget.size());
            if (setTarget[idx])
            {
                IRole* pRole = NULL;
                if (setTarget[idx]->QueryRole(IPP_OF(pRole)) && pRole->IsAlive() && !IsImmunity(pRole))
                {
                    int	nPower = CBattleSystem::CalcPower(HitByMagic(), m_pOwner, pRole, GetPower());
                    //if (nLoseLife)
                    {
                        setRole.push_back(pRole);
                        setPower.push_back(nPower);
                        // 只锁定一个攻击目标
                        //						if (pRole->GetID() == m_idTarget)
                        //							m_setTargetLocked.push_back(pRole->GetID());
                        if (nPower >= pRole->GetLife())
                        {
                            pTarget = pRole;    // continue next cut
                        }
                        break;
                    }
                }
            }
        }
        if (!pTarget)
        {
            break;
        }
        pos.x = pTarget->GetPosX();
        pos.y = pTarget->GetPosY();
    }
    // 广播法术效果
    CMsgMagicEffect	msg;
    IF_OK(msg.CreateByPos(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetInt(MAGICDATA_LEVEL), pos.x, pos.y, m_pOwner->GetDir()))
    {
        for(int i = 0; i < setRole.size(); i++)
        {
            msg.AppendRole(setRole[i]->GetID(), setPower[i]);
        }
        m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
    }
    int nPowerSum = 0;
    int	nExp = 0;
    for (int i = 0; i < setRole.size(); i++)
    {
        nPowerSum += setPower[i];
        CNpc* pNpc = NULL;
        int nLifeLost = setPower[i];
        if (setPower[i] && setRole[i]->TransferShield(HitByMagic(), m_pOwner, setPower[i]))
        {
        }
        else
        {
            nLifeLost = __min(setRole[i]->GetLife(), setPower[i]);
            setRole[i]->AddAttrib(_USERATTRIB_LIFE, -1 * nLifeLost, SYNCHRO_TRUE);
            if (setRole[i]->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && pNpc->IsAwardScore())
            {
                m_pOwner->AwardSynWarScore(pNpc, nLifeLost);
            }
            setRole[i]->BeAttack(HitByMagic(), m_pOwner, setPower[i]);
        }
        if (setRole[i]->IsMonster() || pNpc && pNpc->IsGoal() && m_pOwner->GetLev() >= pNpc->GetLev())
        {
            nExp += this->m_pOwner->AdjustExp(setRole[i], nLifeLost);
            if (!setRole[i]->IsAlive())
            {
                // 有组队，并且队伍中有其他队员，则奖励其他队员经验
                int	nBonusExp	= setRole[i]->GetMaxLife() * KILLBONUS_PERCENT / 100;
                OtherMemberAwardExp(setRole[i], nBonusExp);
                nExp += m_pOwner->AdjustExp(setRole[i], nBonusExp, true);
            }
        }
        if (!setRole[i]->IsAlive())
        {
            m_pOwner->Kill(setRole[i], GetDieMode());
        }
    }
    IStatus* pStatus = m_pOwner->QueryStatus(STATUS_DMG2LIFE);
    if (pStatus)
    {
        int nLifeGot = ::CutTrail(0, MulDiv(nPowerSum, pStatus->GetPower(), 100));
        if (nLifeGot > 0)
        {
            m_pOwner->AddAttrib(_USERATTRIB_LIFE, nLifeGot, SYNCHRO_TRUE);
        }
    }
    AwardExp(nExp);
    //? 未增加武器熟练度，也未损耗双方装备。
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessCallTeamMember()
{
    CHECKF(m_pMagic);
    m_setTargetLocked.clear();
    CTeam* pTeam = m_pOwner->GetTeam();
    if (!pTeam)
    {
        return false;
    }
    if (!m_pOwner->GetMap())
    {
        return false;
    }
    // 广播法术效果
    CMsgMagicEffect	msg;
    IF_OK(msg.Create(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetInt(MAGICDATA_LEVEL), m_pOwner->GetID(), 0, m_pOwner->GetDir()))
    m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
    for(int i = 0; i < pTeam->GetMemberAmount(); i++)
    {
        OBJID idTarget = pTeam->GetMemberByIndex(i);
        if (idTarget != ID_NONE && idTarget != m_pOwner->GetID())
        {
            CUser* pTarget = UserManager()->GetUser(idTarget);
            if (pTarget)
            {
                if (!pTarget->IsAlive())
                {
                    return false;
                }
                //				m_setTargetLocked.push_back(pTarget->GetID());
                pTarget->FlyMap(m_pOwner->GetMap()->GetID(), m_pOwner->GetPosX(), m_pOwner->GetPosY());
            }
            else
            {
                MapGroup(PID)->QueryIntraMsg()->ChangeTeam(CHANGETEAM_FLY, m_pOwner->GetMap()->GetID(), idTarget, MAKELONG(m_pOwner->GetPosX(), m_pOwner->GetPosY()));
            }
        }
    }
    AwardExp(0);
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessRecordTransSpell()
{
    CHECKF(m_pMagic);
    m_setTargetLocked.clear();
    // 广播法术效果
    CMsgMagicEffect	msg;
    IF_OK(msg.Create(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetInt(MAGICDATA_LEVEL), m_idTarget, 0, m_pOwner->GetDir()))
    m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
    CItem* pItem = m_pOwner->FindSpaceTransSpell();
    if (pItem)
    {
        OBJID idMap = ID_NONE;
        if (m_pOwner->GetMap())
        {
            idMap = m_pOwner->GetMap()->GetID();
        }
        pItem->SetInt(ITEMDATA_DATA, idMap, UPDATE_FALSE);
        pItem->SetInt(ITEMDATA_AMOUNT, m_pOwner->GetPosX(), UPDATE_FALSE);
        pItem->SetInt(ITEMDATA_AMOUNTLIMIT, m_pOwner->GetPosY(), UPDATE_TRUE);
        CMsgItemInfo	msg;
        msg.Create(pItem, ITEMINFO_UPDATE);
        m_pOwner->SendMsg(&msg);
    }
    AwardExp(0);
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessTransform()
{
    CHECKF(m_pMagic);
    m_setTargetLocked.clear();
    // 广播法术效果
    CMsgMagicEffect	msg;
    IF_OK(msg.Create(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetInt(MAGICDATA_LEVEL), m_idTarget, 0, m_pOwner->GetDir()))
    m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
    m_pOwner->Transform(GetPower(), GetPower());
    this->AwardExp(0);
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessAddMana()
{
    CHECKF(m_pMagic);
    m_setTargetLocked.clear();
    int	nAddMana = GetPower();
    nAddMana = ::CutOverflow(nAddMana, (int)m_pOwner->GetMaxMana() - (int)m_pOwner->GetMana());
    // 广播法术效果
    CMsgMagicEffect	msg;
    IF_OK(msg.Create(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetInt(MAGICDATA_LEVEL), m_pOwner->GetID(), nAddMana, m_pOwner->GetDir()))
    m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
    int nExp = m_pOwner->GetMaxMana() - m_pOwner->GetMana();
    m_pOwner->AddAttrib(_USERATTRIB_MANA, nAddMana, SYNCHRO_TRUE);
    this->AwardExp(0);
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessLayTrap()
{
    CHECKF(m_pMagic);
    m_setTargetLocked.clear();
    int	nTrapType = GetPower();
    // 广播法术效果
    CMsgMagicEffect	msg;
    IF_OK(msg.Create(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetInt(MAGICDATA_LEVEL), m_pOwner->GetID(), 0, m_pOwner->GetDir()))
    m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
    int nCostDir	= abs(m_pOwner->GetPosX() - m_pos.x) < abs(m_pOwner->GetPosY() - m_pos.y) ? 0 : 2;
    TRAP_INFO	info;
    info.id				= ID_NONE;		//? set id in CreateNew()
    info.idMap			= ID_NONE;
    if (m_pOwner->GetMap())
    {
        info.idMap	= m_pOwner->GetMap()->GetID();
    }
    info.idOwner		= m_pOwner->GetID();
    info.usLook			= (nCostDir + 4) % MAX_DIRSIZE;			// 4: rotate 90 degree
    info.usPosX			= m_pos.x;
    info.usPosY			= m_pos.y;
    info.usType			= nTrapType;
    // 暂时这么修改，等魔法系统修改完成后，还需要修改陷阱为使用IRole为入口参数
    CUser* pUser = NULL;
    if (m_pOwner->QueryObj(OBJ_USER, IPP_OF(pUser)))
    {
        CMapTrap* pTrap	= CMapTrap::CreateNew(m_idProcess, &info, pUser);
        if (pTrap)
        {
            RoleManager()->QuerySet()->AddObj(pTrap->QueryRole());
        }
    }
    this->AwardExp(0);
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessCallPet()
{
    CHECKF(m_pMagic);
    CUser* pUser = NULL;
    if (!m_pOwner->QueryObj(OBJ_USER, IPP_OF(pUser)))
    {
        return false;
    }
    m_setTargetLocked.clear();
    // 广播法术效果
    CMsgMagicEffect	msg;
    IF_OK(msg.CreateByPos(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetInt(MAGICDATA_LEVEL), m_pos.x, m_pos.y, m_pOwner->GetDir()))
    m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
    pUser->CallPet(GetPower(), m_pos.x, m_pos.y, m_pMagic->GetInt(MAGICDATA_STEP));
    this->AwardExp(0);
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessDecLife()
{
    CHECKF(m_pMagic);
    m_setTargetLocked.clear();
    IRole* pTarget = m_pOwner->FindAroundRole(m_idTarget);
    if (!pTarget || !pTarget->IsAlive())
    {
        return false;
    }
    //	if (m_pOwner->GetDistance(pTarget->GetPosX(), pTarget->GetPosY()) > m_pMagic->GetInt(MAGICDATA_DISTANCE))
    //		return false;
    if (this->IsImmunity(pTarget))
    {
        return false;
    }
    int	nPower	= ::CutTrail(0, CRole::AdjustDataEx(pTarget->GetLife(), GetPower()));
    //	m_setTargetLocked.push_back(m_idTarget);
    // 广播法术效果
    CMsgMagicEffect	msg;
    IF_OK(msg.Create(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetInt(MAGICDATA_LEVEL), m_idTarget, nPower, m_pOwner->GetDir()))
    m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
    // set crime status
    CheckCrime(pTarget);
    if (nPower)
    {
        int nLifeLost = nPower;
        if (pTarget->TransferShield(HitByMagic(), m_pOwner, nPower))
        {
        }
        else
        {
            nLifeLost = __min(pTarget->GetLife(), nPower);
            pTarget->AddAttrib(_USERATTRIB_LIFE, -1 * nLifeLost, SYNCHRO_TRUE);
            pTarget->BeAttack(HitByMagic(), m_pOwner, nPower);
        }
        AwardExpOfLife(pTarget, nLifeLost);
        if (!pTarget->IsAlive())
        {
            m_pOwner->Kill(pTarget, GetDieMode());
        }
        IStatus* pStatus = m_pOwner->QueryStatus(STATUS_DMG2LIFE);
        if (pStatus)
        {
            int nLifeGot = ::CutTrail(0, MulDiv(nPower, pStatus->GetPower(), 100));
            if (nLifeGot > 0)
            {
                m_pOwner->AddAttrib(_USERATTRIB_LIFE, nLifeGot, SYNCHRO_TRUE);
            }
        }
    }
    //? 未增加武器熟练度，也未损耗双方装备。
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessGroundSting()
{
    CHECKF(m_pMagic);
    vector<IRole*>	setRole;
    vector<int>		setPower;
    POINT	pos;
    int	nCount = ::CutOverflow(4, m_pMagic->GetInt(MAGICDATA_LEVEL) + 2);
    int	nExp = 0;
    int nPowerSum = 0;
    for (int k = 0; k < m_nApplyTimes; k++)
    {
        // 搜索目标集
        {
            m_setTargetLocked.clear();
            setRole.clear();
            setPower.clear();
            if (m_pMagic->GetInt(MAGICDATA_GROUND))
            {
                pos.x		= m_pos.x;
                pos.y		= m_pos.y;
            }
            else
            {
                IRole* pTarget = m_pOwner->FindAroundRole(m_idTarget);
                if (!pTarget || !pTarget->IsAlive())
                {
                    return false;
                }
                pos.x		= pTarget->GetPosX();
                pos.y		= pTarget->GetPosY();
            }
            IRole* pRole = NULL;
            switch(nCount)
            {
            case 2:
                if (::RandGet(2) == 0)
                {
                    pRole = m_pOwner->GetMap()->QueryRoleByPos(pos.x - 1, pos.y);
                    if (pRole && pRole->IsAlive() && !IsImmunity(pRole))
                    {
                        m_setTargetLocked.push_back(pRole->GetID());
                        setRole.push_back(pRole);
                    }
                    pRole = m_pOwner->GetMap()->QueryRoleByPos(pos.x + 1, pos.y);
                    if (pRole && pRole->IsAlive() && !IsImmunity(pRole))
                    {
                        m_setTargetLocked.push_back(pRole->GetID());
                        setRole.push_back(pRole);
                    }
                }
                else
                {
                    pRole = m_pOwner->GetMap()->QueryRoleByPos(pos.x, pos.y - 1);
                    if (pRole && pRole->IsAlive() && !IsImmunity(pRole))
                    {
                        m_setTargetLocked.push_back(pRole->GetID());
                        setRole.push_back(pRole);
                    }
                    pRole = m_pOwner->GetMap()->QueryRoleByPos(pos.x, pos.y + 1);
                    if (pRole && pRole->IsAlive() && !IsImmunity(pRole))
                    {
                        m_setTargetLocked.push_back(pRole->GetID());
                        setRole.push_back(pRole);
                    }
                }
                break;
            case 3:
                {
                    int nRand = ::RandGet(4);
                    if (nRand != 0)
                    {
                        pRole = m_pOwner->GetMap()->QueryRoleByPos(pos.x - 1, pos.y);
                        if (pRole && pRole->IsAlive() && !IsImmunity(pRole))
                        {
                            m_setTargetLocked.push_back(pRole->GetID());
                            setRole.push_back(pRole);
                        }
                    }
                    if (nRand != 1)
                    {
                        pRole = m_pOwner->GetMap()->QueryRoleByPos(pos.x + 1, pos.y);
                        if (pRole && pRole->IsAlive() && !IsImmunity(pRole))
                        {
                            m_setTargetLocked.push_back(pRole->GetID());
                            setRole.push_back(pRole);
                        }
                    }
                    if (nRand != 2)
                    {
                        pRole = m_pOwner->GetMap()->QueryRoleByPos(pos.x, pos.y - 1);
                        if (pRole && pRole->IsAlive() && !IsImmunity(pRole))
                        {
                            m_setTargetLocked.push_back(pRole->GetID());
                            setRole.push_back(pRole);
                        }
                    }
                    if (nRand != 3)
                    {
                        pRole = m_pOwner->GetMap()->QueryRoleByPos(pos.x, pos.y + 1);
                        if (pRole && pRole->IsAlive() && !IsImmunity(pRole))
                        {
                            m_setTargetLocked.push_back(pRole->GetID());
                            setRole.push_back(pRole);
                        }
                    }
                }
                break;
            case 4:
                {
                    pRole = m_pOwner->GetMap()->QueryRoleByPos(pos.x - 1, pos.y);
                    if (pRole && pRole->IsAlive() && !IsImmunity(pRole))
                    {
                        m_setTargetLocked.push_back(pRole->GetID());
                        setRole.push_back(pRole);
                    }
                    pRole = m_pOwner->GetMap()->QueryRoleByPos(pos.x + 1, pos.y);
                    if (pRole && pRole->IsAlive() && !IsImmunity(pRole))
                    {
                        m_setTargetLocked.push_back(pRole->GetID());
                        setRole.push_back(pRole);
                    }
                    pRole = m_pOwner->GetMap()->QueryRoleByPos(pos.x, pos.y - 1);
                    if (pRole && pRole->IsAlive() && !IsImmunity(pRole))
                    {
                        m_setTargetLocked.push_back(pRole->GetID());
                        setRole.push_back(pRole);
                    }
                    pRole = m_pOwner->GetMap()->QueryRoleByPos(pos.x, pos.y + 1);
                    if (pRole && pRole->IsAlive() && !IsImmunity(pRole))
                    {
                        m_setTargetLocked.push_back(pRole->GetID());
                        setRole.push_back(pRole);
                    }
                }
                break;
            }
        }
        // 锁定目标
        if (m_pMagic->GetSTrackAmount() > 0)
        {
            LockTarget(true);
        }
        // 广播法术效果
        CMsgMagicEffect	msg;
        IF_OK(msg.CreateByPos(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetInt(MAGICDATA_LEVEL), pos.x, pos.y, m_pOwner->GetDir()))
        {
            for(int i = 0; i < setRole.size(); i++)
            {
                IRole* pRole = setRole[i];
                if (pRole && pRole->IsAlive())
                {
                    int nPower = CBattleSystem::CalcPower(HitByMagic(), m_pOwner, pRole, GetPower(k));
                    setPower.push_back(nPower);
                    msg.AppendRole(pRole->GetID(), nPower);
                }
                else
                {
                    setPower.push_back(0);
                }
            }
            m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
        }
        // set crime status
        CheckCrime(&setRole);
        for (int i = 0; i < setRole.size(); i++)
        {
            if (!setRole[i]->IsAlive())
            {
                continue;
            }
            nPowerSum += setPower[i];
            CNpc* pNpc = NULL;
            int nLifeLost = setPower[i];
            if (setPower[i] && setRole[i]->TransferShield(HitByMagic(), m_pOwner, setPower[i]))
            {
            }
            else
            {
                nLifeLost = __min(setRole[i]->GetLife(), setPower[i]);
                setRole[i]->AddAttrib(_USERATTRIB_LIFE, -1 * nLifeLost, SYNCHRO_TRUE);
                if (setRole[i]->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && pNpc->IsAwardScore())
                {
                    m_pOwner->AwardSynWarScore(pNpc, nLifeLost);
                }
                setRole[i]->BeAttack(HitByMagic(), m_pOwner, setPower[i]);
            }
            if (setRole[i]->IsMonster() || pNpc && pNpc->IsGoal() && m_pOwner->GetLev() >= pNpc->GetLev())
            {
                nExp += m_pOwner->AdjustExp(setRole[i], nLifeLost);
                if (!setRole[i]->IsAlive())
                {
                    // 有组队，并且队伍中有其他队员，则奖励其他队员经验
                    int	nBonusExp	= setRole[i]->GetMaxLife() * KILLBONUS_PERCENT / 100;
                    OtherMemberAwardExp(setRole[i], nBonusExp);
                    nExp += m_pOwner->AdjustExp(setRole[i], nBonusExp, true);
                }
            }
        }
        if (m_pMagic->GetSTrackAmount() > 0)
        {
            AdjustPosByTrack(k);
        }
        // 必须先发轨迹后发kill消息
        for (int i = 0; i < setRole.size(); i++)
        {
            if (!setRole[i]->IsAlive())
            {
                m_pOwner->Kill(setRole[i], GetDieMode());
            }
        }
    }
    IStatus* pStatus = m_pOwner->QueryStatus(STATUS_DMG2LIFE);
    if (pStatus)
    {
        int nLifeGot = ::CutTrail(0, MulDiv(nPowerSum, pStatus->GetPower(), 100));
        if (nLifeGot > 0)
        {
            m_pOwner->AddAttrib(_USERATTRIB_LIFE, nLifeGot, SYNCHRO_TRUE);
        }
    }
    AwardExp(nExp);
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessReborn()
{
    CHECKF (m_pMagic);
    m_setTargetLocked.clear();
    IRole* pRole = m_pOwner->FindAroundRole(m_idTarget);
    if (!pRole || pRole->IsAlive())
    {
        return false;
    }
    CUser* pTarget = NULL;
    if (!pRole->QueryObj(OBJ_USER, IPP_OF(pTarget)))
    {
        return false;
    }
    //	if (m_pOwner->GetDistance(pTarget->GetPosX(), pTarget->GetPosY()) > m_pMagic->GetInt(MAGICDATA_DISTANCE))
    //		return false;
    //	if (this->IsImmunity(pRole))
    //		return false;
    int	nPower	= ::CutTrail(0, CRole::AdjustDataEx(pTarget->GetLife(), GetPower()));
    // 广播法术效果
    CMsgMagicEffect	msg;
    IF_OK(msg.Create(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetInt(MAGICDATA_LEVEL), m_idTarget, nPower, m_pOwner->GetDir()))
    m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
    // set crime status
    CheckCrime(pRole);
    bool bChgPos = false;
    pTarget->Reborn(bChgPos);
    if (nPower)
    {
        // 设置HP -- 复活以后不是满血更真实
    }
    //? 未增加武器熟练度，也未损耗双方装备。
    this->AwardExp(0);
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessSorbSoul()
{
    CHECKF (m_pMagic);
    m_setTargetLocked.clear();
    IRole* pRole = m_pOwner->FindAroundRole(m_idTarget);
    if (!pRole || pRole->IsAlive() || !pRole->IsMonster())
    {
        return false;
    }
    int nPower = GetPower();
    // 广播法术效果
    CMsgMagicEffect	msg;
    IF_OK(msg.Create(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetInt(MAGICDATA_LEVEL), m_idTarget, nPower, m_pOwner->GetDir()))
    m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
    // TODO: 吸收尸体的灵魂转换为魔力
    // 通过前面的CheckCondition判断，以下条件判断已经是冗余的
    CMonster* pMonster = NULL;
    if (pRole->QueryObj(OBJ_MONSTER, IPP_OF(pMonster))
            && (pMonster->GetMaskData() & MASK_DISABLE_SORB_SOUL) == 0)
    {
        pMonster->SetMaskData(MASK_DISABLE_SORB_SOUL);
        pMonster->DelMonster(true);		// 立刻删除怪物尸体
        pMonster->SetStatus(STATUS_DISAPPEARING);
        m_pOwner->AddAttrib(_USERATTRIB_MANA, nPower, SYNCHRO_TRUE);
    }
    this->AwardExp(0);
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessSteal()
{
    CHECKF(m_pMagic);
    CHECKF(m_pOwner);
    IRole* pTarget = m_pOwner->FindAroundRole(m_idTarget);
    if (!pTarget || !pTarget->IsAlive())
    {
        return false;
    }
    CUser* pTargetUser = NULL;
    if (!pTarget->QueryObj(OBJ_USER, IPP_OF(pTargetUser)))
    {
        return false;
    }
    m_setTargetLocked.clear();
    int	nPower = GetPower();
    // 广播法术效果
    CMsgMagicEffect	msg;
    IF_OK(msg.Create(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetInt(MAGICDATA_LEVEL), m_idTarget, nPower, m_pOwner->GetDir()))
    m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
    // set crime status
    CheckCrime(pTarget);
    if (nPower)
    {
        CItem* pItem = pTargetUser->GetItemByType(GHOST_GEM_AVOID_STEAL);
        if (pItem)
        {
            pTargetUser->SendSysMsg(STR_AUTO_USE_ANTISTEAL, pItem->GetStr(ITEMDATA_NAME));
            pTargetUser->SendSysMsg(STR_AVOID_STEAL, m_pOwner->GetName());
            pTargetUser->EraseItem(pItem->GetID(), true);
            return false;
        }
        else
        {
            IUserPackage* pPackage = pTargetUser->QueryPackage();
            if (pPackage)
            {
                int nStealNum = __min(nPower, pPackage->GetAmount());
                for (int i = 0; i < nStealNum; i++)
                {
                    OBJID idItem = pPackage->RandGetItem();
                    CItem* pItem = pTargetUser->PopItem(idItem, true, true);
                    if (pItem)
                    {
                        pTargetUser->SendSysMsg(STR_STOLEN, m_pOwner->GetName(), pItem->GetStr(ITEMDATA_NAME));
                        if (!m_pOwner->AddItem(pItem, true))
                        {
                            pItem->DeleteRecord();
                            pItem->ReleaseByOwner();
                        }
                    }
                }
            }
        }
    }
    this->AwardExp(0);
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessBlastThunder()
{
    CHECKF(m_pMagic);
    CHECKF(m_pOwner);
    vector<IRole*>		setRole;
    vector<int>			setPower;
    POINT	pos;
    m_setTargetLocked.clear();
    IRole* pTarget = m_pOwner->FindAroundRole(m_idTarget);
    if (!pTarget || !pTarget->IsAlive())
    {
        return false;
    }
    pos.x = m_pOwner->GetPosX();
    pos.y = m_pOwner->GetPosY();
    // 搜索目标集
    while (pTarget)
    {
        if (m_pOwner->GetDistance(pos.x, pos.y) > m_pMagic->GetInt(MAGICDATA_DISTANCE)
                || !pTarget->IsAlive()
                || IsImmunity(pTarget))
        {
            break;
        }
        setRole.push_back(pTarget);
        pTarget = NULL;
        int nDirX = ::RandGet(MAX_DIRSIZE);
        int nDirY = ::RandGet(MAX_DIRSIZE);
        POINT pt = {pos.x, pos.y};
        for (int i = 0; i < m_pMagic->GetInt(MAGICDATA_DISTANCE); i++)
        {
            pt.x += _DELTA_X[nDirX];
            pt.y += _DELTA_Y[nDirY];
            IRole* pRole = m_pOwner->GetMap()->QueryRoleByPos(pt.x, pt.y);
            if (pRole && pRole->IsAlive() && !IsImmunity(pRole)
                    && m_pOwner->FindAroundRole(pRole->GetID()))	// 必须在自己的广播区内，这里暂时这么实现，效率不够高 -- zlong
            {
                // 避免重复
                bool bInTargetSet = false;
                for (int k = 0; k < setRole.size(); k++)
                {
                    if (setRole[k] && setRole[k]->GetID() == pRole->GetID())
                    {
                        bInTargetSet = true;
                        break;
                    }
                }
                if (!bInTargetSet)
                {
                    pTarget = pRole;
                }
            }
        }
    }	// end of while
    // 广播法术效果，计算伤害
    CMsgMagicEffect	msg;
    IF_OK(msg.CreateByPos(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetInt(MAGICDATA_LEVEL), m_pOwner->GetPosX(), m_pOwner->GetPosY(), m_pOwner->GetDir()))
    {
        for(int i = 0; i < setRole.size(); i++)
        {
            IRole* pRole = setRole[i];
            if (pRole && pRole->IsAlive())
            {
                int nPower = CBattleSystem::CalcPower(HitByMagic(), m_pOwner, pRole, GetPower());
                setPower.push_back(nPower);
                msg.AppendRole(pRole->GetID(), nPower);
            }
            else
            {
                setPower.push_back(0);
            }
        }
        m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
    }
    CheckCrime(&setRole);
    int	nExp = 0;
    int nPowerSum = 0;
    for (int i = 0; i < setRole.size(); i++)
    {
        if (!setRole[i]->IsAlive())
        {
            continue;
        }
        nPowerSum += setPower[i];
        CNpc* pNpc = NULL;
        int nLifeLost = setPower[i];
        if (setPower[i] && setRole[i]->TransferShield(HitByMagic(), m_pOwner, setPower[i]))
        {
        }
        else
        {
            nLifeLost = __min(setRole[i]->GetLife(), setPower[i]);
            setRole[i]->AddAttrib(_USERATTRIB_LIFE, -1 * nLifeLost, SYNCHRO_TRUE);
            if (setRole[i]->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && pNpc->IsAwardScore())
            {
                m_pOwner->AwardSynWarScore(pNpc, nLifeLost);
            }
            setRole[i]->BeAttack(HitByMagic(), m_pOwner, setPower[i]);
        }
        if (setRole[i]->IsMonster() || pNpc && pNpc->IsGoal() && m_pOwner->GetLev() >= pNpc->GetLev())
        {
            nExp += this->m_pOwner->AdjustExp(setRole[i], nLifeLost);
            if (!setRole[i]->IsAlive())
            {
                // 有组队，并且队伍中有其他队员，则奖励其他队员经验
                int	nBonusExp	= setRole[i]->GetMaxLife() * KILLBONUS_PERCENT / 100;
                OtherMemberAwardExp(setRole[i], nBonusExp);
                nExp += m_pOwner->AdjustExp(setRole[i], nBonusExp, true);
            }
        }
        if (!setRole[i]->IsAlive())
        {
            m_pOwner->Kill(setRole[i], GetDieMode());
        }
    }
    IStatus* pStatus = m_pOwner->QueryStatus(STATUS_DMG2LIFE);
    if (pStatus)
    {
        int nLifeGot = ::CutTrail(0, MulDiv(nPowerSum, pStatus->GetPower(), 100));
        if (nLifeGot > 0)
        {
            m_pOwner->AddAttrib(_USERATTRIB_LIFE, nLifeGot, SYNCHRO_TRUE);
        }
    }
    AwardExp(nExp);
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::CollectTargetSet_Bomb(ROLE_SET& setRole, POINT& pos, int nLockType)
{
    CHECKF (m_pMagic);
    int	nRange	= m_pMagic->GetInt(MAGICDATA_RANGE);
    int	nSize		= nRange * 2 + 1;
    int	nBufSize	= nSize * nSize;
    MAPTHING_SET	setTarget;
    m_setTargetLocked.clear();
    setRole.clear();
    int nDir	= m_pOwner->GetDir();
    if (m_pMagic->GetInt(MAGICDATA_GROUND))
    {
        pos.x		= m_pOwner->GetPosX(); //m_pos.x;
        pos.y		= m_pOwner->GetPosY(); //m_pos.y;
    }
    else
    {
        IRole* pTarget = m_pOwner->FindAroundRole(m_idTarget);
        if (!pTarget || !pTarget->IsAlive())
        {
            return false;
        }
        pos.x		= pTarget->GetPosX();
        pos.y		= pTarget->GetPosY();
    }
    m_pOwner->GetMap()->CollectMapThing(setTarget, pos, nRange, OBJ_USER | OBJ_MONSTER | OBJ_NPC);
    for(int i = ::CutTrail<int>(pos.x - nRange, 0); i <= pos.x + nRange && i < m_pOwner->GetMap()->GetWidth(); i++)
    {
        for(int j = ::CutTrail<int>(pos.y - nRange, 0); j <= pos.y + nRange && j < m_pOwner->GetMap()->GetHeight(); j++)
        {
            POINT posThis;
            posThis.x	= i;
            posThis.y	= j;
            if (!CUser::IsInCircle(posThis, pos, nRange))
            {
                continue;
            }
            int idx = POS2INDEX(posThis.x - pos.x + nRange, posThis.y - pos.y + nRange, nSize, nSize);
            ASSERT(idx >= 0 && idx < setTarget.size());
            if (setTarget[idx])
            {
                IRole* pRole = NULL;
                if (setTarget[idx]->QueryRole(IPP_OF(pRole)) && pRole->IsAlive() && !IsImmunity(pRole))
                {
                    //if (nLoseLife)
                    {
                        setRole.push_back(pRole);
                        // 锁定攻击目标
                        if ((nLockType == LOCK_ONE && pRole->GetID() == m_idTarget)
                                || nLockType == LOCK_ALL)
                        {
                            m_setTargetLocked.push_back(pRole->GetID());
                        }
                    }
                }
            }
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::CollectTargetSet_Team(ROLE_SET& setRole)
{
    // 如果是幻兽则需要先获得主人
    CUser* pUser = NULL;
    CMonster* pMonster = NULL;
    if (m_pOwner->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)))
    {
        pUser = pMonster->QueryOwnerUser();
    }
    else
    {
        m_pOwner->QueryObj(OBJ_USER, IPP_OF(pUser));
    }
    if (!pUser)
    {
        return false;
    }
    int		nRange	= m_pMagic->GetInt(MAGICDATA_RANGE);
    POINT	pos = { m_pOwner->GetPosX(), m_pOwner->GetPosY() };
    // 先搜索玩家集合
    vector<CUser*>	setUser;
    setUser.push_back(pUser);
    CTeam* pTeam = pUser->GetTeam();
    if (pTeam)
    {
        for (int i = 0; i < pTeam->GetMemberAmount(); i++)
        {
            OBJID idMember = pTeam->GetMemberByIndex(i);
            IRole* pMember = m_pOwner->FindAroundRole(idMember);
            CUser* pMemberUser = NULL;
            if (pMember && pMember->GetID() != pUser->GetID()
                    && pMember->QueryObj(OBJ_USER, IPP_OF(pMemberUser)))
            {
                setUser.push_back(pMemberUser);
            }
        }
    }
    // 然后搜索目标集合——包括满足条件的玩家和幻兽
    POINT	posThis;
    for (int i = 0; i < setUser.size(); i++)
    {
        CUser* pTarget = setUser[i];
        posThis.x = pTarget->GetPosX();
        posThis.y = pTarget->GetPosY();
        if (pTarget->IsAlive() && CUser::IsInCircle(posThis, pos, nRange))
        {
            setRole.push_back(pTarget->QueryRole());
        }
        for (int k = 0; k < pTarget->GetEudemonAmount(); k++)
        {
            CMonster* pEudemon = pTarget->QueryEudemonByIndex(i);
            if (pEudemon)
            {
                posThis.x = pEudemon->GetPosX();
                posThis.y = pEudemon->GetPosY();
                if (pEudemon->IsAlive() && CUser::IsInCircle(posThis, pos, nRange))
                {
                    setRole.push_back(pEudemon->QueryRole());
                }
            }
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessMultiAttach()
{
    CHECKF (m_pMagic);
    CHECKF (m_pOwner);
    ROLE_SET	setRole;
    POINT		pos;
    // 搜索目标集
    if (m_pMagic->GetInt(MAGICDATA_CRIME))
    {
        if (!CollectTargetSet_Bomb(setRole, pos, LOCK_NONE))
        {
            return false;
        }
    }
    else
    {
        pos.x = m_pOwner->GetPosX();
        pos.y = m_pOwner->GetPosY();
        if (!CollectTargetSet_Team(setRole))
        {
            return false;
        }
    }
    int nPower	= GetPower();
    int nSecs	= m_pMagic->GetInt(MAGICDATA_STEP);
    int nTimes	= m_pMagic->GetInt(MAGICDATA_TIMES);
    DWORD nStatus = m_pMagic->GetInt(MAGICDATA_STATUS);
    ASSERT(nPower > 0);
    CMsgMagicEffect	msg;
    IF_OK(msg.Create(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetInt(MAGICDATA_LEVEL), pos.x, pos.y, m_pOwner->GetDir()))
    {
        for (int i = 0; i < setRole.size(); i++)
        {
            IRole* pRole = setRole[i];
            if (pRole && pRole->IsAlive())
            {
                msg.AppendRole(pRole->GetID(), 0);
            }
        }
        m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
    }
    CheckCrime(&setRole);
    for (int i = 0; i < setRole.size(); i++)
    {
        IRole* pRole = setRole[i];
        if (!pRole || !pRole->IsAlive())
        {
            continue;
        }
        void* pTemp = NULL;
        if (!pRole->QueryObj(OBJ_USER, IPP_OF(pTemp))
                && (nStatus == STATUS_LURKER /*|| nStatus == STATUS_ATKRANGE*/ || nStatus == STATUS_REFLECT || nStatus == STATUS_REFLECTMAGIC))
        {
            continue;
        }
        if (nStatus == STATUS_LURKER || nStatus == STATUS_ATKSPEED)	// 不可叠加
        {
            CRole::DetachWellStatus(pRole);
        }
        // 暂时不做超级状态，有要求再说
        // 疾行技能特殊处理
        if (nStatus == STATUS_PELT && pRole->QueryStatus(STATUS_PELT))
        {
            CRole::DetachStatus(pRole, nStatus);
        }
        else
        {
            CRole::AttachStatus(pRole, nStatus, nPower, nSecs, nTimes);
        }
        // hint
        if (nPower >= 30000)
        {
            int nPowerTimes = (nPower - 30000) / 100;
            MSGBUF	szMsg;
            switch (nStatus)
            {
            case	STATUS_DEFENCE1:
            case	STATUS_DEFENCE2:
            case	STATUS_DEFENCE3:
                {
                    sprintf(szMsg, STR_DEFENCE_ADD, nSecs, nPowerTimes);
                    pRole->SendSysMsg(szMsg);
                }
                break;
            case	STATUS_ATTACK:
                {
                    sprintf(szMsg, STR_ATTACK_ADD, nSecs, nPowerTimes);
                    pRole->SendSysMsg(szMsg);
                }
                break;
            }
        }
        else
        {
            int nPowerTimes = nPower;
            MSGBUF	szMsg;
            switch(nStatus)
            {
            case	STATUS_DEFENCE1:
            case	STATUS_DEFENCE2:
            case	STATUS_DEFENCE3:
                {
                    sprintf(szMsg, STR_DEFENCE_ADD2, nSecs, nPowerTimes);
                    pRole->SendSysMsg(szMsg);
                }
                break;
            case	STATUS_ATTACK:
                {
                    sprintf(szMsg, STR_ATTACK_ADD2, nSecs, nPowerTimes);
                    pRole->SendSysMsg(szMsg);
                }
                break;
            case	STATUS_SUPER_ATK:
                {
                    sprintf(szMsg, STR_SUPER_ATK_ADD, nSecs, nPowerTimes);
                    pRole->SendSysMsg(szMsg);
                }
                break;
            case	STATUS_SUPER_DEF:
                {
                    sprintf(szMsg, STR_SUPER_DEF_ADD, nSecs, nPowerTimes);
                    pRole->SendSysMsg(szMsg);
                }
                break;
            case	STATUS_SUPER_MATK:
                {
                    sprintf(szMsg, STR_SUPER_MATK_ADD, nSecs, nPowerTimes);
                    pRole->SendSysMsg(szMsg);
                }
                break;
            case	STATUS_SUPER_MDEF:
                {
                    sprintf(szMsg, STR_SUPER_MDEF_ADD, nSecs, nPowerTimes);
                    pRole->SendSysMsg(szMsg);
                }
                break;
            }
        }
    }
    AwardExp(0);		// false: not award battle exp
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessMultiDetach()
{
    CHECKF (m_pMagic);
    CHECKF (m_pOwner);
    int nPower	= 0;
    int nSecs	= m_pMagic->GetInt(MAGICDATA_STEP);
    int nTimes	= m_pMagic->GetInt(MAGICDATA_TIMES);
    DWORD nStatus = m_pMagic->GetInt(MAGICDATA_STATUS);
    // PK场不能使用复活魔法
    if (STATUS_DIE == nStatus)
    {
        if (m_pOwner->GetMap() && m_pOwner->GetMap()->IsPkField())
        {
            return false;
        }
    }
    ROLE_SET	setRole;
    POINT		pos;
    // 搜索目标集
    if (m_pMagic->GetInt(MAGICDATA_CRIME))
    {
        if (!CollectTargetSet_Bomb(setRole, pos, LOCK_NONE))
        {
            return false;
        }
    }
    else
    {
        pos.x = m_pOwner->GetPosX();
        pos.y = m_pOwner->GetPosY();
        if (!CollectTargetSet_Team(setRole))
        {
            return false;
        }
    }
    CMsgMagicEffect	msg;
    IF_OK(msg.Create(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetInt(MAGICDATA_LEVEL), pos.x, pos.y, m_pOwner->GetDir()))
    {
        for (int i = 0; i < setRole.size(); i++)
        {
            IRole* pRole = setRole[i];
            if (!pRole)
            {
                continue;
            }
            if (!pRole->IsAlive())
            {
                // 对于死亡的对象，只能使用复活魔法，并且只能对玩家使用
                if (STATUS_DIE != nStatus)
                {
                    continue;
                }
                void* pTemp;
                if (!pRole->QueryObj(OBJ_USER, IPP_OF(pTemp)))
                {
                    continue;
                }
            }
            msg.AppendRole(pRole->GetID(), 0);
        }
        m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
    }
    CheckCrime(&setRole);
    for (int i = 0; i < setRole.size(); i++)
    {
        IRole* pRole = setRole[i];
        if (!pRole || !pRole->IsAlive())
        {
            continue;
        }
        if (STATUS_DETACH_ALL == nStatus)
        {
            CRole::DetachAllStatus(pRole);
        }
        else if (STATUS_DETACH_BADLY == nStatus)
        {
            CRole::DetachBadlyStatus(pRole);
        }
        else
        {
            IStatus* pStatus = pRole->QueryStatus(nStatus);
            if (pStatus)
            {
                pStatus->ChangeData(nPower, nSecs, nTimes);
            }
        }
        pRole->ClsStatus(nStatus, SYNCHRO_TRUE);
        if (STATUS_DIE == nStatus)
        {
            CUser* pUser = NULL;
            if (pRole->QueryObj(OBJ_USER, IPP_OF(pUser)))
            {
                bool bChgPos = false;
                pUser->Reborn(bChgPos);
            }
        }
    }
    AwardExp(0);
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessMultiCure()
{
    CHECKF(m_pMagic);
    ROLE_SET	setRole;
    vector<int>	setPower;
    POINT		pos;
    // 搜索目标集
    if (m_pMagic->GetInt(MAGICDATA_CRIME))
    {
        if (!CollectTargetSet_Bomb(setRole, pos, LOCK_NONE))
        {
            return false;
        }
    }
    else
    {
        pos.x = m_pOwner->GetPosX();
        pos.y = m_pOwner->GetPosY();
        if (!CollectTargetSet_Team(setRole))
        {
            return false;
        }
    }
    //	if (m_pMagic->GetSTrackAmount() > 0)
    //		LockTarget(true);
    int	nExp = 0;
    //	for (int k=0; k<m_nApplyTimes; k++)
    {
        // 广播法术效果
        CMsgMagicEffect	msg;
        IF_OK(msg.Create(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetInt(MAGICDATA_LEVEL), m_idTarget, m_pOwner->GetDir()))
        {
            for(int i = 0; i < setRole.size(); i++)
            {
                IRole* pRole = setRole[i];
                if (pRole && pRole->IsAlive())
                {
                    int nPower = GetPower();
                    if (nPower == ADJUST_FULL)
                    {
                        nPower = pRole->GetMaxLife() - pRole->GetLife();
                    }
                    // 超级回复
                    //					if (m_pMagic && m_pMagic->GetInt(MAGICDATA_TYPE)>=SUPER_MAGIC_RECRUIT)
                    //						nPower *= m_pOwner->GetSoulSum();
                    setPower.push_back(nPower);
                    msg.AppendRole(setRole[i]->GetID(), nPower);
                }
                else
                {
                    setPower.push_back(0);
                }
            }
            m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
        }
        for(int i = 0; i < setRole.size(); i++)
        {
            if (!setRole[i]->IsAlive())
            {
                continue;
            }
            int nAddLife = ::CutOverflow(setPower[i], ::CutTrail(0, (int)(setRole[i]->GetMaxLife() - setRole[i]->GetLife())));
            if (nAddLife > 0)
            {
                setRole[i]->AddAttrib(_USERATTRIB_LIFE, nAddLife, SYNCHRO_TRUE);
                CUser* pUser = NULL;
                if (setRole[i]->QueryObj(OBJ_USER, IPP_OF(pUser)))
                {
                    pUser->BroadcastTeamLife();
                }
            }
            CNpc* pNpc = NULL;
            if (setRole[i])
            {
                setRole[i]->QueryObj(OBJ_NPC, IPP_OF(pNpc));
                if (pNpc && pNpc->IsGoal())
                {
                    nAddLife = nAddLife * GOAL_EXP_PERCENT / 100;
                }
            }
            nExp += nAddLife;
        }
        //		if (m_pMagic->GetSTrackAmount() > 0)
        //			AdjustPosByTrack(k);
    }
    this->AwardExp(0);
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessStealMoney()
{
    CHECKF (m_pMagic);
    CHECKF (m_pOwner);
    m_setTargetLocked.clear();
    IRole* pRole = m_pOwner->FindAroundRole(m_idTarget);
    if (!(pRole && pRole->IsAlive()))
    {
        return false;
    }
    // 广播法术效果
    CMsgMagicEffect	msg;
    IF_OK(msg.Create(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetInt(MAGICDATA_LEVEL), m_idTarget, 0, m_pOwner->GetDir()))
    m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);
    int nMoneyStolen = 0;
    CMonster* pMonster = NULL;
    CUser* pUser = NULL;
    if (pRole->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)))
    {
        if ((pMonster->GetMaskData() & MASK_DISABLE_STEAL_MONEY) == 0)
        {
            pMonster->SetMaskData(MASK_DISABLE_STEAL_MONEY);
            nMoneyStolen = pMonster->GetDropMoneyAvg() * (::RandGet(50) + 1) / 100;
        }
    }
    else if (pRole->QueryObj(OBJ_USER, IPP_OF(pUser)))
    {
        nMoneyStolen = pUser->GetMoney() * (::RandGet(10) + 1) / 100;
        pUser->SpendMoney(nMoneyStolen, true);
    }
    if (nMoneyStolen > 0)
    {
        if (!m_pOwner->QueryObj(OBJ_USER, IPP_OF(pUser)))
        {
            pUser = m_pOwner->QueryOwnerUser();
        }
        if (pUser)
        {
            pUser->AddMoney(nMoneyStolen);
        }
    }
    this->AwardExp(0);
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessKO()
{
    CHECKF (m_pMagic);
    CHECKF (m_pOwner);
    IRole* pRole = m_pOwner->FindAroundRole(m_idTarget);
    if (!(pRole && pRole->IsAlive()))
    {
        return false;
    }
    if (pRole->GetLife() * 100 / pRole->GetMaxLife() > MAGIC_KO_LIFE_PERCENT)
    {
        return false;
    }
    int nPower = pRole->GetLife();
    int nExp = nPower;
    if (nPower)
    {
        if (pRole->TransferShield(HitByMagic(), m_pOwner, nPower))
        {
        }
        else
        {
            pRole->AddAttrib(_USERATTRIB_LIFE, -1 * nPower, SYNCHRO_TRUE);
            pRole->BeAttack(HitByMagic(), m_pOwner, nPower);
            if (!pRole->IsAlive())
            {
                m_pOwner->Kill(pRole, GetDieMode());
            }
        }
        IStatus* pStatus = m_pOwner->QueryStatus(STATUS_DMG2LIFE);
        if (pStatus)
        {
            int nLifeGot = ::CutTrail(0, MulDiv(nPower, pStatus->GetPower(), 100));
            if (nLifeGot > 0)
            {
                m_pOwner->AddAttrib(_USERATTRIB_LIFE, nLifeGot, SYNCHRO_TRUE);
            }
        }
    }
    AwardExpOfLife(pRole, nExp);
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CMagic::ProcessEscape()
{
    CHECKF (m_pMagic);
    CHECKF (m_pOwner);
    IRole* pRole = m_pOwner->FindAroundRole(m_idTarget);
    if (!pRole || !pRole->IsAlive())
    {
        return false;
    }
    const int	MIN_TRANS_DISTANCE	= 5;
    const int	MAX_TRANS_DISTANCE	= 20;
    const int	DIV_DIR[]	= { -1, 1};
    const int	MAX_TRY_TIMES	= 20;
    // find random pos
    int nPosX = 0;
    int nPosY = 0;
    int i = 0;
    for (int i = 0; i < MAX_TRY_TIMES; i++)
    {
        int nDivX = ::RandGet(MAX_TRANS_DISTANCE - MIN_TRANS_DISTANCE + 1) + MIN_TRANS_DISTANCE;
        int nDivY = ::RandGet(MAX_TRANS_DISTANCE - MIN_TRANS_DISTANCE + 1) + MIN_TRANS_DISTANCE;
        nPosX = pRole->GetPosX() + DIV_DIR[::RandGet(2)] * nDivX;
        nPosY = pRole->GetPosY() + DIV_DIR[::RandGet(2)] * nDivY;
        if (pRole->GetMap() && pRole->GetMap()->IsStandEnable(nPosX, nPosY))
        {
            break;
        }
    }
    if (i >= MAX_TRY_TIMES)
    {
        return false;
    }
    pRole->ActiveTransPos(nPosX, nPosY);
    AwardExp(0);
    return true;
}

//////////////////////////////////////////////////////////////////////
/*
bool CMagic::ProcessFlashAttack()
{
	CHECKF(m_pMagic);
	CHECKF(m_pOwner);

	m_setTargetLocked.clear();
	IRole* pTarget = m_pOwner->FindAroundRole(m_idTarget);
	if (!pTarget || !pTarget->IsAlive())
		return false;

	if (this->IsImmunity(pTarget))
		return false;

	if (m_pMagic->GetInt(MAGICDATA_FLOOR_ATTR))		// 0: need not check
	{
		int nAttr = m_pOwner->GetMap()->GetFloorAttr(pTarget->GetPosX(), pTarget->GetPosY());
		if (m_pMagic->GetInt(MAGICDATA_FLOOR_ATTR) != nAttr)
			return false;
	}

	m_setTargetLocked.push_back(pTarget->GetID());

	LockTarget(true);

	// 记录目标周围可以站立的坐标点
	vector<POINT>	setPosition;
	int nSize = 0;
	for (int i=-1; i<=1; i++)
	{
		for (int j=-1; j<=1; j++)
		{
			if (i!=0 || j!=0)
			{
				if (m_pOwner->GetMap()->IsStandEnable(pTarget->GetPosX()+i, pTarget->GetPosY()+j))
				{
					setPosition.resize(nSize+1);
					setPosition[nSize].x = pTarget->GetPosX()+i;
					setPosition[nSize].y = pTarget->GetPosY()+j;
					++nSize;
				}
			}
		}
	}

	int nTotalExp = 0;
	const int	FLASH_ATTACK_STEPS	= 5;	// 5次攻击
	for (int i=0; i<FLASH_ATTACK_STEPS; i++)
	{
		int	nPower = CBattleSystem::CalcPower(HitByMagic(), m_pOwner, pTarget, GetPower());

		// 广播法术效果
		CMsgMagicEffect	msg;
		IF_OK(msg.Create(m_pOwner->GetID(), m_pMagic->GetInt(MAGICDATA_TYPE), m_pMagic->GetInt(MAGICDATA_LEVEL), m_idTarget, nPower, m_pOwner->GetDir()))
			m_pOwner->BroadcastRoomMsg(&msg, INCLUDE_SELF);

		// set crime status
		CheckCrime(pTarget);

		if (nPower)
		{
			int nLifeLost = __min(pTarget->GetLife(), nPower);
			pTarget->AddAttrib(_USERATTRIB_LIFE, -1*nLifeLost, SYNCHRO_TRUE);

			nTotalExp += nLifeLost;

			pTarget->BeAttack(HitByMagic(), m_pOwner, nPower);

			IStatus* pStatus = m_pOwner->QueryStatus(STATUS_DMG2LIFE);
			if (pStatus)
			{
				int nLifeGot = ::CutTrail(0, MulDiv(nPower, pStatus->GetPower(), 100));
				if (nLifeGot > 0)
					m_pOwner->AddAttrib(_USERATTRIB_LIFE, nLifeGot, SYNCHRO_TRUE);
			}
		}

		// TODO:
	}
	if (nTotalExp > 0)
		AwardExpOfLife(pTarget, nTotalExp);

	m_pos.x	= pTarget->GetPosX();
	m_pos.y = pTarget->GetPosY();


	//? 未增加武器熟练度，也未损耗双方装备。

	// drop weapon in weapon skill
	if (m_pMagic->GetInt(MAGICDATA_USE_XP) == TYPE_KONGFU && m_pMagic->GetInt(MAGICDATA_DROPWEAPON) == THROW_WERPONR_TARGET)
	{
		CItem* pWeapon = m_pOwner->GetWeaponR();
		IF_OK(pWeapon)
		{
			POINT	pos;
			pos.x	= pTarget->GetPosX();
			pos.y	= pTarget->GetPosY();
			m_pOwner->GetMap()->FindDropItemCell(THROW_WEAPON_DROP_RANGE, &pos);
			m_pOwner->DropItem(pWeapon->GetID(), pos.x, pos.y);					// 没找到CELL也扔，但会重叠。
		}
	}

	return true;
}*/
