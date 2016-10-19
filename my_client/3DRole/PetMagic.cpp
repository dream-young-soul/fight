
//**********************************************************
// 代码编辑器
//**********************************************************

// PetMagic.cpp:
//xueliang code -- 2005 7-12
//////////////////////////////////////////////////////////////////////
#include "Pet.h"
#include "Hero.h"
#include "AllMsg.h"
#include "GameMsg.h"
#include "3DGameMap.h"
#include "MsgInteract.h"
#include "GamePlayerSet.h"
//--------------------------------------------------------------------------------------

void CPet::AddMagic(OBJID idMagic, DWORD dwLevel, DWORD dwExp)
{
    int nAmount = m_setMagic.size();
    for(int i = 0; i < nAmount; i++)
    {
        CMagic* pMagic = m_setMagic[i];
        if (pMagic && pMagic->m_infoMagicType.idMagicType == idMagic)
        {
            MagicTypeInfo infoMagicType;
            if (CMagic::s_objMagicData.GetMagicTypeInfo(idMagic, dwLevel, infoMagicType))
            {
                pMagic->m_infoMagicType = infoMagicType;
                pMagic->m_dwExp = dwExp;
                pMagic->m_idOwner = this->GetID();
            }
            return;
        }
    }
    CMagic* pMagic = CMagic::CreateNew(idMagic, dwLevel, dwExp);
    if (pMagic)
    {
        pMagic->m_idOwner = this->GetID();
        m_setMagic.push_back(pMagic);
        ::PostCmd(CMD_FLASH_PETSTATUS);
    }
}

int CPet::GetPetMagicSort(OBJID idMagicType)
{
    if (idMagicType / 1000 != 6)
    {
        return 0;    //不是幻兽技能
    }
    DWORD idType = (idMagicType % 1000) / 100;
    int nSort = 0;
    switch(idType)
    {
    case 0:
        nSort = _NORMAL_MAGIC;
        break;
    case 1:
        nSort = _ALTITUTE_MAGIC;
        break;
    case 2:
        nSort = _UITIMATE_MAGIC;
        break;
    case 3:
        nSort = _COMBO_MAGIC;
        break;
    default:
        break;
    }
    return nSort;
}
//--------------------------------------------------------------------------------------
int CPet::GetMagicAmount(int nSort)
{
    int nMagicAmount = 0;
    int nAmount = m_setMagic.size();
    for(int i = 0; i < nAmount; i++)
    {
        CMagic* pMagic = m_setMagic[i];
        if (pMagic
                && (nSort == 0 || (nSort != 0 && this->GetPetMagicSort(pMagic->GetIDType()))))
        {
            nMagicAmount++;
        }
    }
    return nMagicAmount;
}
//--------------------------------------------------------------------------------------
CMagic* CPet::GetMagicByIndex(int nIndex, int nSort)
{
    int nMagicIndex = 0;
    int nAmount = m_setMagic.size();
    for(int i = 0; i < nAmount; i++)
    {
        CMagic* pMagic = m_setMagic[i];
        if (pMagic && nMagicIndex == nIndex
                && (nSort == 0 || (nSort != 0 && this->GetPetMagicSort(pMagic->GetIDType()) == nSort)))
        {
            return pMagic;
        }
        nMagicIndex++;
    }
    return NULL;
}
//--------------------------------------------------------------------------------------
CMagic* CPet::GetMagic(OBJID idMagicType)
{
    int nAmount = m_setMagic.size();
    for(int i = 0; i < nAmount; i ++)
    {
        CMagic* pMagic = m_setMagic[i];
        if (pMagic && (pMagic->m_infoMagicType.idMagicType == idMagicType))
        {
            return pMagic;
        }
    }
    return NULL;
}
//--------------------------------------------------------------------------------------
void CPet::DestroyMagicSet()
{
    int nAmount = m_setMagic.size();
    for(int i = 0; i < nAmount; i ++)
    {
        CMagic* pMagic = m_setMagic[i];
        SAFE_DELETE(pMagic);
    }
    m_setMagic.clear();
}
//--------------------------------------------------------------------------------------
void CPet::ProcessMagic()
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
    //	if (bFlash)
    //		::PostCmd(CMD_FLASH_MAGIC_STATUS);
}
//--------------------------------------------------------------------------------------
void CPet::ProcessMagicBusy()
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
//--------------------------------------------------------------------------------------
void CPet::StartMagicBusy(DWORD dwDuration)
{
    if (m_bMagicAttackBusy)
    {
        return;
    }
    m_dwMagicAttackBegin	= ::TimeGet();
    m_dwMagicAttackBusy		= dwDuration;
    m_bMagicAttackBusy		= true;
    ::DebugMsg("Pet magic begin at:%06u --- end at:%06u\n", m_dwMagicAttackBegin, m_dwMagicAttackBegin + m_dwMagicAttackBusy);
}
//--------------------------------------------------------------------------------------
void CPet::EndMagicBusy()
{
    if (m_bMagicAttackBusy && !this->IsMoving() && !this->IsMagicLock())
    {
        this->StandBy();
    }
    m_bMagicAttackBusy		= false;
}
//--------------------------------------------------------------------------------------
BOOL   CPet::ProcessAI_MagicAtk()
{
    return this->MagicAttack(m_idMagicType, m_idTarget);
}
//--------------------------------------------------------------------------------------
BOOL	CPet::MagicAttack(OBJID idMagicType, OBJID idPlayer, BOOL bForce)
{
    if (idMagicType >= 30000)
    {
        return FALSE;
    }
    if (this->TestStatus(USERSTATUS_CANNOTMOVE))
    {
        return FALSE;
    }
    if (this->IsMagicLock())
    {
        return FALSE;
    }
    if (m_bMagicAttackBusy)
    {
        OBJID idMagicAttackToFast = 100055;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idMagicAttackToFast));
        return FALSE;
    }
    CMagic* pMagic = this->GetMagic(idMagicType);
    if (!pMagic)
    {
        const OBJID idMagicTypeError = 100056;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idMagicTypeError));
        return FALSE;
    }
    if (g_objGameMap.GetType() & MAPTYPE_BOOTH_ENABLE)
    {
        if (!pMagic->m_infoMagicType.dwCanBeusedInMarket)
        {
            return FALSE;
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
        return FALSE;
    }
    if (pTarget->TestStatus(USERSTATUS_PK_PROTECT))
    {
        return FALSE;
    }
    if (pMagic->m_infoMagicType.dwActionSort == MAGIC_SORT_RECOVER_SINGLE_HP ||
            (pMagic->m_infoMagicType.dwActionSort == MAGIC_SORT_RECOVER_SINGLE_STATUS &&
             !pMagic->m_infoMagicType.dwCrime))
    {
        if (!pTarget->IsPlayer() && pTarget->GetID() != g_objHero.GetID() && pTarget->GetID() != this->GetID() )
        {
            return FALSE;
        }
    }
    pTarget->GetPos(posTarget.x, posTarget.y);
    //	this->Direction(posTarget.x, posTarget.y);
    CMyPos posHero;
    this->GetPos(posHero);
    //	this->BeginChangeUp(pMagic->GetIDType(), pMagic->GetLevel());
    if (!pMagic->IsEnable())
    {
        OBJID idMagicAttackToFast = 100055;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idMagicAttackToFast));
        return FALSE;
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
            return FALSE;
        }
    }
    // target check
    if (pPlayer->IsPlayer() && (pMagic->m_infoMagicType.dwTarget & PLAYER_INCLUDEDEADMONSTER))
    {
        OBJID idErrorTarget = 100060;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idErrorTarget));
        return FALSE;
    }
    if ((pPlayer->TestStatus(USERSTATUS_DISAPPEARING) || !pPlayer->TestStatus(USERSTATUS_DIE)) && (pMagic->m_infoMagicType.dwTarget & PLAYER_INCLUDEDEADMONSTER))
    {
        OBJID idErrorTarget = 100060;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idErrorTarget));
        return FALSE;
    }
    // ....
    CMyPos posPet;
    this->GetPos(posPet);
    pPlayer->GetPos(posTarget);
    int	nRange	= pMagic->m_infoMagicType.dwDistance;
    //nRange += g_objHero.GetAddSize() + g_objHero.GetAtkRange() + (pPlayer->GetAddSize() + 1)/2;
    /*
    	if (abs(posTarget.x - posPet.x) > nRange || abs(posTarget.y - posPet.y) > nRange)
    	{
    		OBJID idNoTarget = 100058;
    		g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idNoTarget));
    		return FALSE;
    	}*/
    //	this->Stop();
    int nPosX, nPosY;
    this->GetPos(nPosX, nPosY);
    if (pMagic->m_infoMagicType.dwActionSort == MAGIC_SORT_ATTACK_SINGLE_HP ||
            pMagic->m_infoMagicType.dwActionSort == MAGIC_SORT_ATTACK_SINGLE_STATUS)
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
    if (pMagic->m_infoMagicType.dwSenderAction  == _ACTION_INTONE_DURATION)
    {
        CCommand cmd;
        cmd.iType		= _COMMAND_EMOTION;
        cmd.iStatus		= _CMDSTATUS_BEGIN;
        cmd.nData		= _ACTION_INTONE;
        cmd.bHeroSendMsg = false;
        if (pTarget)
        {
            CMyPos posSender, posTarget;
            this->GetPos(posSender);
            pTarget->GetPos(posTarget);
            cmd.nDir = (g_objGameMap.GetDirection(posSender.x, posSender.y, posTarget.x, posTarget.y) + 1) % 7;
        }
        this->SetCommand(&cmd);
    }
    return TRUE;
}
//--------------------------------------------------------------------------------------
void	CPet::MagicAttack(OBJID idMagicType, CMyPos posTarget)
{
}
//--------------------------------------------------------------------------------------
BOOL    CPet::CheckMagicAI(OBJID idMagicType)
{
    CMagic* pMagic = this->GetMagic(idMagicType);
    if (pMagic)
    {
        CPlayer* pPlayer = g_objPlayerSet.GetPlayer(m_idTarget, true);
        if (pPlayer && !pPlayer->IsDead())
        {
            m_idMagicType = idMagicType;
            return true;
        }
        //		PetMagicInfo  PetMagicInfo = g_objGameDataSet.GetPetMagicInfo(idMagicType);
    }
    return false;
}
//--------------------------------------------------------------------------------------
BOOL    CPet::CanMagicAtk()
{
    int nAmount = this->GetMagicAmount();
    if (nAmount <= 0)
    {
        return false;
    }
    nAmount = this->GetMagicAmount(_UITIMATE_MAGIC);
    int i = 0;
    for(; i < nAmount; i++)
    {
        CMagic* pMagic = this->GetMagicByIndex(i, _UITIMATE_MAGIC);
        if (pMagic && this->CheckMagicAI(pMagic->GetIDType()) && pMagic->IsEnable())
        {
            return true;
        }
    }
    nAmount = this->GetMagicAmount(_NORMAL_MAGIC);
    for(int i = 0; i < nAmount; i++)
    {
        CMagic* pMagic = this->GetMagicByIndex(i, _NORMAL_MAGIC);
        if (pMagic && this->CheckMagicAI(pMagic->GetIDType()) && pMagic->IsEnable())
        {
            return true;
        }
    }
    return false;
}
