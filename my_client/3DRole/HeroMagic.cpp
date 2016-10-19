
//**********************************************************
// 代码编辑器
//**********************************************************

//------------------------------------------------------------------
// HeroMagic.cpp
//------------------------------------------------------------------

#include "Hero.h"
#include "MsgInteract.h"
#include "GamePlayerSet.h"
#include "GameMsg.h"
#include "GameDataSet.h"
#include "3dGameMap.h"
#include "AllMsg.h"
//------------------------------------------------------------------
static CMagic* test_magic;
void CHero::DestroyMagicSet()
{
	
    int nAmount = m_setMagic.size();
	for(int i = 0; i < nAmount; i ++)
	{
		CMagic* pMagic = m_setMagic[i];
		SAFE_DELETE(pMagic);
	}
    m_setMagic.clear();
}
//------------------------------------------------------------------
void CHero::AddMagic(OBJID idMagic, DWORD dwLevel, DWORD dwExp)
{
	
    int nAmount = m_setMagic.size();
    int nInsertIndexAt = 0;	//主动
    int nInsertIndexPs = 0; //被动
    int nInsertIndexXp = 0; //XP
    int nInsertIndexTm = 0; //TEAM
    for(int i = 0; i < nAmount; i ++)
    {
        CMagic* pMagic = m_setMagic[i];
        if (pMagic && (pMagic->m_infoMagicType.idMagicType == idMagic))
        {
            MagicTypeInfo infoMagicType;
            if (CMagic::s_objMagicData.GetMagicTypeInfo(idMagic, dwLevel, infoMagicType))
            {
                if (pMagic->m_infoMagicType.dwLevel < infoMagicType.dwLevel )
                {
                    char szMsg[128] = "";
                    sprintf(szMsg, g_objGameDataSet.GetStr(100203), infoMagicType.szName);
                    g_objGameMsg.AddMsg(szMsg);		//恭喜阁下***技能升级
                    m_objEffect.Add("magicup", false, 0, this->GetLook()); //技能升级！
                }
                pMagic->m_infoMagicType = infoMagicType;
                pMagic->m_dwExp = dwExp;
                pMagic->m_idOwner = this->GetID();
            }
            return;
        }
        else
        {
            if (pMagic->m_infoMagicType.dwActionSort == MAGICSORT_TEAM_MAGIC)
            {
                nInsertIndexTm++;
            }
            else if (pMagic->TestTarget(MAGIC_PASSIVE))
            {
                nInsertIndexPs++;
            }
            else if (pMagic->m_infoMagicType.dwXp == TYPE_XPSKILL)
            {
                nInsertIndexXp++;
            }
            else
            {
                nInsertIndexAt++;
            }
            //				nInsertIndex++;
        }
    }
    CMagic* pMagic = CMagic::CreateNew(idMagic, dwLevel, dwExp);
	
    // Magic 依照type排序
    if (pMagic)
    {
        pMagic->m_idOwner = this->GetID();
        if (pMagic->m_infoMagicType.dwActionSort == MAGICSORT_TEAM_MAGIC)
        {
            m_setMagic.insert(m_setMagic.begin() + nInsertIndexTm, pMagic);
        }
        else if (pMagic->TestTarget(MAGIC_PASSIVE))
        {
            m_setMagic.insert(m_setMagic.begin() + nInsertIndexTm + nInsertIndexAt + nInsertIndexPs, pMagic);
        }
        else if (pMagic->m_infoMagicType.dwXp == TYPE_XPSKILL)
        {
            if (!this->m_bXpSkillEnable)
            {
                m_bXpSkillEnable = TRUE;
            }
            m_setMagic.insert(m_setMagic.begin() + nInsertIndexTm + nInsertIndexAt + nInsertIndexPs + nInsertIndexXp, pMagic);
        }
        else
        {
            m_setMagic.insert(m_setMagic.begin() + nInsertIndexTm + nInsertIndexAt, pMagic);
        }

		
        //		m_setMagic.insert(m_setMagic.begin() + nInsertIndex, pMagic);
    }
    else
    {
        char szTemp[256];
        sprintf(szTemp, "Not found magic: id=%d, Lev=%d", idMagic, dwLevel);
        g_objGameMsg.AddMsg(szTemp);
    }
}
//------------------------------------------------------------------
int CHero::GetMagicAmount()
{
    int nMagicAmount = 0;
    int nAmount = m_setMagic.size();
    for(int i = 0; i < nAmount; i ++)
    {
        CMagic* pMagic = m_setMagic[i];
        if (pMagic
                //			&& pMagic->m_infoMagicType.dwXp == TYPE_MAGIC
                && pMagic->m_infoMagicType.dwActionSort != MAGICSORT_TEAM_MAGIC)
        {
            nMagicAmount ++;
        }
    }
    return nMagicAmount;
}
//------------------------------------------------------------------
int CHero::GetTeamMagicAmount()
{
    int nTMagicAmount = 0;
    int nAmount = m_setMagic.size();
    for(int i = 0; i < nAmount; i++)
    {
        CMagic* pMagic = m_setMagic[i];
        if (pMagic && pMagic->m_infoMagicType.dwXp == TYPE_MAGIC
                && pMagic->m_infoMagicType.dwActionSort == MAGICSORT_TEAM_MAGIC)
        {
            nTMagicAmount ++;
        }
    }
    return nTMagicAmount;
}
//------------------------------------------------------------------
CMagic*	CHero::GetTeamMagicByIndex(int nIndex)
{
    if (nIndex < 0)
    {
        return NULL;
    }
    int nMagicAmount = 0;
    int nAmount = m_setMagic.size();
    for(int i = 0; i < nAmount; i ++)
    {
        CMagic* pMagic = m_setMagic[i];
        if (pMagic && pMagic->m_infoMagicType.dwXp == TYPE_MAGIC
                && pMagic->m_infoMagicType.dwActionSort == MAGICSORT_TEAM_MAGIC)
        {
            if (nMagicAmount == nIndex)
            {
                return pMagic;
            }
            nMagicAmount ++;
        }
    }
    return NULL;
}
//------------------------------------------------------------------
CMagic*	CHero::GetMagicByIndex(int nIndex)
{
    if (nIndex < 0)
    {
        return NULL;
    }
    int nMagicAmount = 0;
    int nAmount = m_setMagic.size();
    for(int i = 0; i < nAmount; i ++)
    {
        CMagic* pMagic = m_setMagic[i];
        if (pMagic
                //		&& pMagic->m_infoMagicType.dwXp == TYPE_MAGIC
                && pMagic->m_infoMagicType.dwActionSort != MAGICSORT_TEAM_MAGIC)
        {
            if (nMagicAmount == nIndex)
            {
                return pMagic;
            }
            nMagicAmount ++;
        }
    }
    return NULL;
}

//------------------------------------------------------------------
void CHero::MagicAttack(OBJID idMagicType, CMyPos posTarget)
{
    if (idMagicType >= 30000)
    {
        return;
    }
    if (this->IsMagicLock())
    {
        return;
    }
    if (this->TestStatus(USERSTATUS_CANNOTMOVE))
    {
        return;
    }
    this->Direction(posTarget.x, posTarget.y);
    CONST int MAGIC_CIRCLEATK = 1115;
    if (MAGIC_CIRCLEATK == idMagicType)
    {
        CItem* pWeaponR = this->GetEquipment(ITEMPOSITION_WEAPONR);
        CItem* pWeaponL = this->GetEquipment(ITEMPOSITION_WEAPONL);
        if (!(pWeaponR && pWeaponL
                && (pWeaponR->GetWeaponSkillType() == pWeaponL->GetWeaponSkillType())))
        {
            const OBJID idStr = 100117;
            g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idStr));//"此技能必须双手相同类型武器才能使用。"
            return;
        }
    }
    if (m_bMagicAttackBusy)
    {
        OBJID idMagicAttackToFast = 100055;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idMagicAttackToFast));
        return;
    }
    CMagic* pMagic = this->GetMagic(idMagicType);
    if (!pMagic)
    {
        const OBJID idMagicTypeError = 100056;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idMagicTypeError));
        return;
    }
    if (g_objGameMap.GetType() & MAPTYPE_BOOTH_ENABLE)
    {
        if (!pMagic->m_infoMagicType.dwCanBeusedInMarket)
        {
            return;
        }
    }
    if (pMagic->m_infoMagicType.dwActionSort == MAGICSORT_TEAM_MAGIC )
    {
        if (g_objHero.GetTeamMemberAmount() < 3)
        {
            g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr(100171)); //结界需要有3人以上的组队方可使用
            return;
        }
        if (!g_objHero.IsTeamLeader())
        {
            return;
        }
    }
    CMyPos posHero;
    this->GetPos(posHero);
    if (!g_objGameMap.IsPkRegion(posHero) && pMagic->m_infoMagicType.dwCrime)//禁止pk区域检测
    {
        const OBJID idStr = 100157;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(100157));
        return;
    }
    this->BeginChangeUp(pMagic->GetIDType(), pMagic->GetLevel());
    if (g_objGameMap.GetType() & MAPTYPE_BOOTH_ENABLE)
    {
        if (pMagic->m_infoMagicType.dwCrime)
        {
            return;
        }
    }
    DWORD nManaCost	= pMagic->m_infoMagicType.dwMpCost;
    if (this->GetData(CPlayer::_PLAYER_MANA) < nManaCost)
    {
        OBJID idLowPower = 100057;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idLowPower));
        return;
    }
    int nPotential	= pMagic->m_infoMagicType.dwPotential;
    if (this->GetPotential() < nPotential)
    {
        OBJID idLowPotential = 100166;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idLowPotential));
        return;
    }
    if (pMagic->m_infoMagicType.dwActionSort == MAGICSORT_TRANSFORM)
    {
        m_dwBeginUseTransformMagicTime = ::TimeGet();
    }
    DWORD nPhysicalForceCost = pMagic->m_infoMagicType.dwUsePP;
    if (this->GetPhysicalForce() < nPhysicalForceCost)
    {
        OBJID idLowPP = 100118;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idLowPP));
        return;
    }
    int	nRange	= pMagic->m_infoMagicType.dwDistance;
    //nRange += g_objHero.GetAddSize() + g_objHero.GetAtkRange();
    posHero = this->GetAlignPos();
    if (abs(posTarget.x - posHero.x) > nRange || abs(posTarget.y - posHero.y) > nRange)
    {
        OBJID idNoTarget = 100058;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idNoTarget));
        return;
    }
    if (!pMagic->IsEnable())
    {
        OBJID idMagicAttackToFast = 100055;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idMagicAttackToFast));
        return;
    }
    if (pMagic->GetWeaponRequired() != 0)
    {
        CItem* pWeaponR = this->GetEquipment(ITEMPOSITION_WEAPONR);
        CItem* pWeaponL = this->GetEquipment(ITEMPOSITION_WEAPONL);
        BOOL bCanUse = FALSE;
        if ((pWeaponR && pWeaponR->GetWeaponSkillType() == pMagic->GetWeaponRequired())
                || (pWeaponL && pWeaponL->GetWeaponSkillType() == pMagic->GetWeaponRequired()))
        {
            bCanUse = TRUE;
        }
        if (pMagic->GetWeaponRequired() == 400
                && ((pWeaponR &&  (pWeaponR->GetWeaponSkillType () == 410 || pWeaponR->GetWeaponSkillType () == 420) )
                    || ( pWeaponL &&  (pWeaponL->GetWeaponSkillType () == 410 || pWeaponL->GetWeaponSkillType () == 420))))
        {
            bCanUse = TRUE;
        }
        if (bCanUse == FALSE)
        {
            return;
        }
    }
    int nLevel	= pMagic->m_infoMagicType.dwLevel;
    CMsgInteract msg;
    OBJID idUser = this->GetID();
    unsigned short usMagicType = (unsigned short)idMagicType;
    OBJID idTarget = ID_NONE;
    int nPosX = posTarget.x;
    int nPosY = posTarget.y;
    ENCODE_MAGICATTACK((unsigned short)idUser, usMagicType, idTarget, nPosX, nPosY)
    if (msg.Create(INTERACT_MAGICATTACK, idUser, idTarget, nPosX, nPosY, usMagicType, (unsigned short)nLevel, ID_NONE))
    {
        msg.Send();
    }
    m_objEffect.Add(pMagic->m_infoMagicType.szIntoneEffect, false, 0, this->GetLook());
    this->StartMagicBusy(pMagic->m_infoMagicType.dwIntoneDuration + pMagic->m_infoMagicType.dwMagicBreak + 3000);
    this->SetDir(this->GetDir(posTarget));
    this->Stop();
    if (pMagic->m_infoMagicType.dwSenderAction  == _ACTION_INTONE_DURATION)
    {
        CCommand cmd;
        cmd.iType		= _COMMAND_EMOTION;
        cmd.iStatus		= _CMDSTATUS_BEGIN;
        cmd.nData		= _ACTION_INTONE;
        cmd.bHeroSendMsg = false;
        CMyPos posSender;
        this->GetPos(posSender);
        cmd.nDir = (g_objGameMap.GetDirection(posSender.x, posSender.y, posTarget.x, posTarget.y) + 1) % 7;
        g_objHero.SetCommand(&cmd);
    }
}

//------------------------------------------------------------------
void CHero::StartMagicBusy(DWORD dwDuration)
{
    if (m_bMagicAttackBusy)
    {
        return;
    }
    m_dwMagicAttackBegin	= ::TimeGet();
    m_dwMagicAttackBusy		= dwDuration;
    m_bMagicAttackBusy		= true;
    ::PostCmd(CMD_START_MAGIC_BUSY);
    ::DebugMsg("magic begin at:%06u --- end at:%06u\n", m_dwMagicAttackBegin, m_dwMagicAttackBegin + m_dwMagicAttackBusy);
}
//------------------------------------------------------------------
void CHero::EndMagicBusy()
{
    if (m_bMagicAttackBusy && !this->IsMoving() && !this->IsMagicLock())
    {
        this->StandBy();
    }
    m_bMagicAttackBusy		= false;
    ::PostCmd(CMD_END_MAGIC_BUSY);
}

//------------------------------------------------------------------
void CHero::MagicAttack(OBJID idMagicType, OBJID idPlayer, BOOL bForce)
{
    if (idMagicType >= 30000)
    {
        return;
    }
    if (this->TestStatus(USERSTATUS_CANNOTMOVE))
    {
        return;
    }
    if (this->IsMagicLock())
    {
        return;
    }
    CONST int MAGIC_BUMP = 1051;
    if (MAGIC_BUMP == idMagicType)
    {
        // in bumping
        if (_COMMAND_BUMP == this->GetCommandType())
        {
            return;
        }
        CMagic* pMagic = this->GetMagic(idMagicType);
        if (!pMagic)
        {
            const OBJID idMagicTypeError = 100056;
            g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idMagicTypeError));
            return;
        }
        // interval check
        DWORD dwTimeNow = ::TimeGet();
        if (dwTimeNow - m_dwLastBump < 1000) //pMagic->m_infoMagicType.dwIntoneDuration)
        {
            return;
        }
        else
        {
            m_dwLastBump = dwTimeNow;
        }
        CPlayer* pTarget = g_objPlayerSet.GetPlayer(idPlayer);
        if (!pTarget)
        {
            return;
        }
        if (!this->CanPk(pTarget))
        {
            return;
        }
        const OBJID idGuard = 100028;
        if (pTarget->GetRoleType() == _ROLE_TASK_NPC)
        {
            return;
        }
        CMyPos posTarget;
        pTarget->GetPos(posTarget.x, posTarget.y);
        CMyPos posHero;	////禁止pk区域检测
        this->GetPos(posHero);
        if (!g_objGameMap.IsPkRegion(posHero) && pMagic->m_infoMagicType.dwCrime && pTarget->IsPlayer())
        {
            const OBJID idStr = 100157;
            g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(100157));
            return;
        }
        if (pTarget->IsNpc() && strstr(pTarget->GetName(), g_objGameDataSet.GetStr(idGuard)))
        {
            if (!(bForce && PKMODE_FREE == this->GetPkMode()))
            {
                const OBJID idCanNotPkGuard = 100029;
                g_objGameMsg.AddMsg(_TXTATR_SYSTEM, g_objGameDataSet.GetStr(idCanNotPkGuard));
                return;
            }
        }
        if (pMagic->GetWeaponRequired() != 0)
        {
            CItem* pWeaponR = this->GetEquipment(ITEMPOSITION_WEAPONR);
            CItem* pWeaponL = this->GetEquipment(ITEMPOSITION_WEAPONL);
            BOOL bCanUse = FALSE;
            if ((pWeaponR && pWeaponR->GetWeaponSkillType() == pMagic->GetWeaponRequired())
                    || (pWeaponL && pWeaponL->GetWeaponSkillType() == pMagic->GetWeaponRequired()))
            {
                bCanUse = TRUE;
            }
            if (pMagic->GetWeaponRequired() == 400
                    && ((pWeaponR &&  (pWeaponR->GetWeaponSkillType () == 410 || pWeaponR->GetWeaponSkillType () == 420) )
                        || ( pWeaponL &&  (pWeaponL->GetWeaponSkillType () == 410 || pWeaponL->GetWeaponSkillType () == 420))))
            {
                bCanUse = TRUE;
            }
            if (bCanUse == FALSE)
            {
                return;
            }
        }
        CCommand cmd;
        cmd.iType		= _COMMAND_BUMP;
        cmd.iStatus		= _CMDSTATUS_BEGIN;
        cmd.idTarget	= pTarget->GetID();
        this->SetCommand(&cmd);
        return;	// do not send msg here.
    }
    CONST int MAGIC_CIRCLEATK = 1115;
    if (MAGIC_CIRCLEATK == idMagicType)
    {
        CItem* pWeaponR = this->GetEquipment(ITEMPOSITION_WEAPONR);
        CItem* pWeaponL = this->GetEquipment(ITEMPOSITION_WEAPONL);
        if (!(pWeaponR && pWeaponL
                && pWeaponR->GetWeaponSkillType() == pWeaponL->GetWeaponSkillType()))
        {
            const OBJID idStr = 100117;
            g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idStr));//"此技能必须双手相同类型武器才能使用。"
            return;
        }
    }
    if (m_bMagicAttackBusy)
    {
        OBJID idMagicAttackToFast = 100055;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idMagicAttackToFast));
        return;
    }
    CMagic* pMagic = this->GetMagic(idMagicType);
    if (!pMagic)
    {
        const OBJID idMagicTypeError = 100056;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idMagicTypeError));
        return;
    }
    if (pMagic->m_infoMagicType.dwActionSort == MAGICSORT_TEAM_MAGIC )
    {
        if (g_objHero.GetTeamMemberAmount() < 3)
        {
            g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr(100171)); //结界需要有3人以上的组队方可使用
            return;
        }
        if (!g_objHero.IsTeamLeader())
        {
            return;
        }
    }
    if (g_objGameMap.GetType() & MAPTYPE_BOOTH_ENABLE)
    {
        if (!pMagic->m_infoMagicType.dwCanBeusedInMarket)
        {
            return;
        }
    }
    CMyPos posTarget;
    CPlayer* pTarget = g_objPlayerSet.GetPlayer(idPlayer);
    if (!pTarget && idPlayer == this->GetID())
    {
        pTarget = this;
    }
    if (!pTarget)
    {
        return;
    }
    if (pTarget->TestStatus(USERSTATUS_PK_PROTECT))
    {
        return;
    }
    if (pMagic->m_infoMagicType.dwActionSort == MAGIC_SORT_RECOVER_SINGLE_HP ||
            (pMagic->m_infoMagicType.dwActionSort == MAGIC_SORT_RECOVER_SINGLE_STATUS &&
             !pMagic->m_infoMagicType.dwCrime))
    {
        if (!pTarget->IsPlayer() && !(pTarget->GetID() == g_objHero.GetID()) )
        {
            return;
        }
    }
    pTarget->GetPos(posTarget.x, posTarget.y);
    this->Direction(posTarget.x, posTarget.y);
    CMyPos posHero;
    this->GetPos(posHero);
    if (!g_objGameMap.IsPkRegion(posHero) && pMagic->m_infoMagicType.dwCrime && pTarget->IsPlayer()) //禁止pk区域检测
    {
        const OBJID idStr = 100157;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(100157));
        return;
    }
    this->BeginChangeUp(pMagic->GetIDType(), pMagic->GetLevel());
    if (pMagic->m_infoMagicType.dwActionSort == MAGICSORT_DANCE)
    {
        const OBJID idDance = 100030;
        if (this->GetLWeapon() == ID_NONE && this->GetRWeapon() == ID_NONE)
        {
            this->Emotion(pMagic->m_infoMagicType.dwSenderAction);
        }
        else
        {
            g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(100030));
        }
        return;
    }
    if (pMagic->GetWeaponRequired() != 0)
    {
        CItem* pWeaponR = this->GetEquipment(ITEMPOSITION_WEAPONR);
        CItem* pWeaponL = this->GetEquipment(ITEMPOSITION_WEAPONL);
        BOOL bCanUse = FALSE;
        if ((pWeaponR && pWeaponR->GetWeaponSkillType() == pMagic->GetWeaponRequired())
                || (pWeaponL && pWeaponL->GetWeaponSkillType() == pMagic->GetWeaponRequired()))
        {
            bCanUse = TRUE;
        }
        if (pMagic->GetWeaponRequired() == 400
                && ((pWeaponR &&  (pWeaponR->GetWeaponSkillType () == 410 || pWeaponR->GetWeaponSkillType () == 420) )
                    || ( pWeaponL &&  (pWeaponL->GetWeaponSkillType () == 410 || pWeaponL->GetWeaponSkillType () == 420))))
        {
            bCanUse = TRUE;
        }
        if (bCanUse == FALSE)
        {
            return;
        }
    }
    if (pMagic->m_infoMagicType.dwActionSort == MAGICSORT_TRANSFORM)
    {
        m_dwBeginUseTransformMagicTime = ::TimeGet();
    }
    DWORD nManaCost	= pMagic->m_infoMagicType.dwMpCost;
    if (this->GetData(CPlayer::_PLAYER_MANA) < nManaCost)
    {
        OBJID idLowPower = 100057;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idLowPower));
        return;
    }
    int nPotential	= pMagic->m_infoMagicType.dwPotential;
    if (this->GetPotential() < nPotential)
    {
        OBJID idLowPotential = 100166;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idLowPotential));
        return;
    }
    DWORD nPhysicalForceCost = pMagic->m_infoMagicType.dwUsePP;
    if (this->GetPhysicalForce() < nPhysicalForceCost)
    {
        OBJID idLowPP = 100118;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idLowPP));
        return;
    }
    if (!pMagic->IsEnable())
    {
        OBJID idMagicAttackToFast = 100055;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idMagicAttackToFast));
        return;
    }
    CPlayer* pPlayer = g_objPlayerSet.GetPlayer(idPlayer);
    if (!pPlayer)
    {
        if (pMagic->TestTarget(MAGIC_TARGET_SELF)
                || pMagic->TestTarget(MAGIC_TARGET_NONE)
                || pMagic->m_infoMagicType.dwActionSort == MAGICSORT_TEAM_MAGIC)
        {
            if (this->GetID() == idPlayer)
            {
                pPlayer = this;
            }
        }
        if (!pPlayer)
        {
            OBJID idErrorTarget = 100059;
            g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idErrorTarget));
            return;
        }
    }
    const OBJID idGuard = 100028;
    if (pPlayer->IsNpc() && strstr(pPlayer->GetName(), g_objGameDataSet.GetStr(idGuard)))
    {
        if (!(bForce && PKMODE_FREE == this->GetPkMode()))
        {
            const OBJID idCanNotPkGuard = 100029;
            g_objGameMsg.AddMsg(_TXTATR_SYSTEM, g_objGameDataSet.GetStr(idCanNotPkGuard));
            return;
        }
    }
    // target check
    CONST int MAGIC_REBORN	= 2009;
    CONST int XP_GETSOUL  = 3012; //收魂技能
    if (pPlayer->IsDead() && XP_GETSOUL != idMagicType && MAGIC_REBORN != idMagicType &&
            !(pMagic->m_infoMagicType.dwTarget & PLAYER_INCLUDEDEADMONSTER))
    {
        OBJID idErrorTarget = 100060;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idErrorTarget));
        return;
    }
    if (pPlayer->IsNpc() &&  MAGIC_REBORN == idMagicType)
    {
        OBJID idErrorTarget = 100060;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idErrorTarget));
        return;
    }
    if (pPlayer->IsPlayer() && (pMagic->m_infoMagicType.dwTarget & PLAYER_INCLUDEDEADMONSTER))
    {
        OBJID idErrorTarget = 100060;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idErrorTarget));
        return;
    }
    if ((pPlayer->TestStatus(USERSTATUS_DISAPPEARING) || !pPlayer->TestStatus(USERSTATUS_DIE)) && (pMagic->m_infoMagicType.dwTarget & PLAYER_INCLUDEDEADMONSTER))
    {
        OBJID idErrorTarget = 100060;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idErrorTarget));
        return;
    }
    // ....
    if (pMagic->m_infoMagicType.dwCrime
            && !this->CanPk(pPlayer)
            && (!bForce || this->GetPkMode() != PKMODE_FREE) && pPlayer->GetID() != g_objHero.GetID())
    {
        OBJID idCanNotPk = 100061;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idCanNotPk));
        return;
    }
    posHero = this->GetAlignPos();
    pPlayer->GetPos(posTarget);
    int	nRange	= pMagic->m_infoMagicType.dwDistance;
    //nRange += g_objHero.GetAddSize() + g_objHero.GetAtkRange() + (pPlayer->GetAddSize() + 1)/2;
    if (abs(posTarget.x - posHero.x) > nRange || abs(posTarget.y - posHero.y) > nRange)
    {
        OBJID idNoTarget = 100058;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idNoTarget));
        return;
    }
    switch(idMagicType)
    {
    case 1015:
        {
            //const OBJID idMagicNote = 100063;
            //g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idMagicNote));
        }
        break;
    case 1020:
        {
            //const OBJID idMagicNote = 100064;
            //g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idMagicNote));
        }
        break;
    default:
        this->Stop();
        break;
    }
    int nPosX, nPosY;
    this->GetPos(nPosX, nPosY);
    /*
    if (this->IsMoving())
    	nPosX = nPosY = 0;
    */
    if (pMagic->m_infoMagicType.dwActionSort == MAGIC_SORT_ATTACK_SINGLE_HP ||
            pMagic->m_infoMagicType.dwActionSort == MAGIC_SORT_ATTACK_SINGLE_STATUS ||
            pMagic->m_infoMagicType.dwActionSort == MAGICSORT_LINE_PENETRABLE)
    {
        nPosX = posTarget.x;
        nPosY = posTarget.y;
    }
    int nLevel	= pMagic->m_infoMagicType.dwLevel;
    CMsgInteract msg;
    OBJID idUser = this->GetID();
    unsigned short usMagicType = (unsigned short)idMagicType;
    OBJID idTarget = idPlayer;
    int nMyPosX = nPosX;
    int nMyPosY = nPosY;
    ENCODE_MAGICATTACK((unsigned short)idUser, usMagicType, idTarget, nMyPosX, nMyPosY)
    if (msg.Create( INTERACT_MAGICATTACK,
                    idUser, idTarget,
                    nMyPosX, nMyPosY,
                    usMagicType, (unsigned short)nLevel, idPlayer))
    {
        msg.Send();
    }
    m_objEffect.Add(pMagic->m_infoMagicType.szIntoneEffect, false, 0, this->GetLook());
    this->StartMagicBusy(pMagic->m_infoMagicType.dwIntoneDuration + pMagic->m_infoMagicType.dwMagicBreak + 3000);
    if (pMagic->m_infoMagicType.dwSenderAction  == _ACTION_INTONE_DURATION )
    {
        CCommand cmd;
        cmd.iType		= _COMMAND_EMOTION;
        cmd.iStatus		= _CMDSTATUS_BEGIN;
        cmd.nData		= _ACTION_INTONE;
        cmd.bHeroSendMsg = false;
        if (pTarget)
        {
            CMyPos posSender, posTarget;
            g_objHero.GetPos(posSender);
            pTarget->GetPos(posTarget);
            cmd.nDir = (g_objGameMap.GetDirection(posSender.x, posSender.y, posTarget.x, posTarget.y) + 1) % 7;
        }
        g_objHero.SetCommand(&cmd);
    }
}

//------------------------------------------------------------------
void CHero::ProcessMagicBusy()
{
    if (!m_bMagicAttackBusy)
    {
        return;
    }
    if (::TimeGet() >= m_dwMagicAttackBegin + m_dwMagicAttackBusy)
    {
        this->EndMagicBusy();
    }
}
//------------------------------------------------------------------
CMagic* CHero::GetMagic(OBJID idType)
{
    int nAmount = m_setMagic.size();
    for(int i = 0; i < nAmount; i ++)
    {
        CMagic* pMagic = m_setMagic[i];
        if (pMagic && (pMagic->m_infoMagicType.idMagicType == idType))
        {
            return pMagic;
        }
    }
    return NULL;
}
//------------------------------------------------------------------
int CHero::GetXpSkillAmount()
{
    int nMagicAmount = 0;
    int nAmount = m_setMagic.size();
    for(int i = 0; i < nAmount; i ++)
    {
        CMagic* pMagic = m_setMagic[i];
        if (pMagic && pMagic->m_infoMagicType.dwXp == TYPE_XPSKILL)
        {
            nMagicAmount ++;
        }
    }
    return nMagicAmount;
}
//------------------------------------------------------------------
CMagic* CHero::GetXpSkillByIndex(int nIndex)
{
    if (nIndex < 0)
    {
        return NULL;
    }
    int nXpAmount = 0;
    int nAmount = m_setMagic.size();
    for(int i = 0; i < nAmount; i ++)
    {
        CMagic* pMagic = m_setMagic[i];
        if (pMagic && pMagic->m_infoMagicType.dwXp == TYPE_XPSKILL)
        {
            if (nXpAmount == nIndex)
            {
                return pMagic;
            }
            nXpAmount ++;
        }
    }
    return NULL;
}
//------------------------------------------------------------------
int CHero::GetKongfuAmount()
{
    int nMagicAmount = 0;
    int nAmount = m_setMagic.size();
    for(int i = 0; i < nAmount; i ++)
    {
        CMagic* pMagic = m_setMagic[i];
        if (pMagic && pMagic->m_infoMagicType.dwXp == TYPE_KONGFU)
        {
            nMagicAmount ++;
        }
    }
    return nMagicAmount;
}
//------------------------------------------------------------------
CMagic* CHero::GetKongfuByIndex(int nIndex)
{
    if (nIndex < 0)
    {
        return NULL;
    }
    int nKongfuAmount = 0;
    int nAmount = m_setMagic.size();
    for(int i = 0; i < nAmount; i ++)
    {
        CMagic* pMagic = m_setMagic[i];
        if (pMagic && pMagic->m_infoMagicType.dwXp == TYPE_KONGFU)
        {
            if (nKongfuAmount == nIndex)
            {
                return pMagic;
            }
            nKongfuAmount++;
        }
    }
    return NULL;
}

//------------------------------------------------------------------
void CHero::ProcessMagic()
{
    BOOL bFlash = false;
    const int _SHIELD_SUBTYPE = 900;
    int nAmount, i;
    nAmount = this->GetMagicAmount();
    for(i = 0; i < nAmount; i ++)
    {
        CMagic* pMagic = this->GetMagicByIndex(i);
        if (!pMagic)
        {
            continue;
        }
        if (pMagic->Process())
        {
            bFlash = true;
        }
    }
    nAmount = this->GetXpSkillAmount();
    for(i = 0; i < nAmount; i ++)
    {
        CMagic* pMagic = this->GetXpSkillByIndex(i);
        if (!pMagic)
        {
            continue;
        }
        if (pMagic->Process())
        {
            bFlash = true;
        }
    }
    nAmount = this->GetKongfuAmount();
    for(i = 0; i < nAmount; i ++)
    {
        CMagic* pMagic = this->GetKongfuByIndex(i);
        if (!pMagic)
        {
            continue;
        }
        if (pMagic->Process())
        {
            bFlash = true;
        }
    }
    if (bFlash)
    {
        ::PostCmd(CMD_FLASH_MAGIC_STATUS);
    }
}
//------------------------------------------------------------------
void CHero::AbortTransform()
{
    CMsgAction msg;
    if (msg.Create(this->GetID(), 0, 0, 0, actionAbortTransform))
    {
        msg.Send();
    }
}
//------------------------------------------------------------------
void CHero::BeginChangeUp(OBJID idMagicType, DWORD dwDelay)
{
    // todo ...
    //	MagicTypeInfo info;
    //	if (!CMagic::s_objMagicData.GetMagicTypeInfo(idMagicType, dwDelay, info))
    //	{
    //		m_bChargeUp = false;
    //		return;
    //	}
    //	if (!(info.dwStatus & USERSTATUS_CHARGEUP))
    ////	{
    ////		m_bChargeUp = false;
    ////		return;
    ////	}
    //	m_bChargeUp = true;
    //	m_dwTimeChargeUp = ::TimeGet();
    //	const DWORD dwNatworkDelay = 500;
    //	m_dwChargeUpTotalTime = info.dwIntoneDuration+dwNatworkDelay;
}
//------------------------------------------------------------------
void CHero::AbortChargeUp()
{
    CMsgAction  msg;
    if (msg.Create(g_objHero.GetID(), 0, 0, this->GetDir(), actionDischarge))
    {
        msg.Send();
    }
}
//------------------------------------------------------------------
void CHero::ShowChargeUp()
{
    const int nBgSize		= 64;
    const int nBarLength	= 49;
    const int nBarHeight	= 9;
    const int nBarOffsetX	= 7;
    const int nBarOffsetY	= 27;
    const DWORD dwBarColor	= 0xff31A2FF;
    if (m_bChargeUp)
    {
        // get ani
        CAni* pAni = g_objGameDataSet.GetDataAni("ani/common.ani", "ChargeUp");
        if (!pAni)
        {
            return;
        }
        // get the current length of bar
        if (m_dwChargeUpTotalTime == 0)
        {
            m_dwChargeUpTotalTime = 1;
        }
        int nCurrentLength = nBarLength * (::TimeGet() - m_dwTimeChargeUp) / m_dwChargeUpTotalTime;
        if (nCurrentLength > nBarLength)
        {
            nCurrentLength = nBarLength;
        }
        // get ani show pos
        CMyPos posShow;
        int nNameShowOffset = (OFFSET_NAMESHOW + 80) * g_objGameMap.GetScale() / _NORMAL_SCALE;
        int nShowX	= m_Info.posScr.x - nBgSize / 2;
        int nShowY	= m_Info.posScr.y - nNameShowOffset;
        posShow.x = nShowX;
        posShow.y = nShowY - this->GetOffsetY();
        // set frame
        int nFrame = 0;
        //		if (this->TestStatus(USERSTATUS_CHARGEUP))
        //		{
        //			nFrame = 1;
        //			nCurrentLength = nBarLength;
        //		}
        pAni->Show(nFrame, posShow.x, posShow.y);
        CMyBitmap::ShowBlock(posShow.x + nBarOffsetX, posShow.y + nBarOffsetY,
                             posShow.x + nBarOffsetX + nCurrentLength, posShow.y + nBarOffsetY + nBarHeight, dwBarColor);
    }
}
//------------------------------------------------------------------
void  CHero::StopChargeUp()
{
    m_bChargeUp = false;
}
//--------------------------------------------------------------------------------------
