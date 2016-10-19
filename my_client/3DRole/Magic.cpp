
//**********************************************************
// 代码编辑器
//**********************************************************

// Magic.cpp: implementation of the CMagic class.
//
//////////////////////////////////////////////////////////////////////

#include "Magic.h"
#include "Hero.h"
#include "3DGameMap.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMagicData CMagic::s_objMagicData;

CMagic::CMagic()
{
    m_bEnable = true;
    m_dwTimeBegin = 0;
}

CMagic::~CMagic()
{
}

//------------------------------------------------------------------
BOOL CMagic::Create(OBJID idType, DWORD dwLevel, DWORD dwExp)
{
    if (!s_objMagicData.GetMagicTypeInfo(idType, dwLevel, m_infoMagicType))
    {
        return false;
    }
    m_dwExp = dwExp;
    return true;
}
//------------------------------------------------------------------
CMagic* CMagic::CreateNew(OBJID idType, DWORD dwLevel, DWORD dwExp)
{
    CMagic* pMagic = new CMagic;
    MYASSERT(pMagic);
    if (pMagic->Create(idType, dwLevel, dwExp))
    {
        return pMagic;
    }
    SAFE_DELETE(pMagic);
    return NULL;
}
//------------------------------------------------------------------
char* CMagic::GetName()
{
    return m_infoMagicType.szName;
}
//------------------------------------------------------------------
char* CMagic::GetDescribe()
{
    return m_infoMagicType.szDisc;
}
//------------------------------------------------------------------
DWORD CMagic::GetExp()
{
    return m_dwExp;
}
//------------------------------------------------------------------
DWORD CMagic::GetNextLevelExp()
{
    return m_infoMagicType.dwExpRequired;
}
//------------------------------------------------------------------
DWORD CMagic::GetLevel()
{
    return m_infoMagicType.dwLevel;
}
//------------------------------------------------------------------
OBJID CMagic::GetIDType()
{
    return m_infoMagicType.idMagicType;
}
//------------------------------------------------------------------
BOOL CMagic::TestTarget(DWORD dwMask)
{
    return m_infoMagicType.dwTarget & dwMask;
}
//------------------------------------------------------------------
DWORD CMagic::GetBusyDuration()
{
    DWORD dwProfession = g_objHero.GetProfession();
    DWORD dwRequireProfLevel = dwProfession % 10;
    if (dwRequireProfLevel > 10)
    {
        return m_infoMagicType.dwIntoneDuration;
    }
    return (10 - dwRequireProfLevel) * 100 + m_infoMagicType.dwIntoneDuration;
}
//------------------------------------------------------------------
BOOL CMagic::IsTransformMagic()
{
    return m_infoMagicType.dwActionSort == MAGICSORT_TRANSFORM;
}
//------------------------------------------------------------------
BOOL CMagic::PetMagicProcess()
{
    return false;
}
//------------------------------------------------------------------
BOOL CMagic::HeroMagicProcess()
{
    if (m_infoMagicType.idMagicType >= 30000)
    {
        return false;
    }
    const int _SHIELD_SUBTYPE = 900;
    BOOL bEnable = true;
    // check the magic ...
    // step 1: transform
    if (g_objHero.IsTransform() && !this->IsTransformMagic())
    {
        bEnable = false;
    }
    // step 2: weapon ...
    CONST int MAGIC_CIRCLEATK = 1115;
    // 纵横四海的特殊判断
    if (MAGIC_CIRCLEATK == this->GetIDType())
    {
        CItem* pWeaponR = g_objHero.GetEquipment(ITEMPOSITION_WEAPONR);
        CItem* pWeaponL = g_objHero.GetEquipment(ITEMPOSITION_WEAPONL);
        if (!(pWeaponR && pWeaponL
                && pWeaponR->GetWeaponSkillType() == pWeaponL->GetWeaponSkillType()))
        {
            bEnable = true;
        }
    }
    // 盾牌技能的特殊判断
    else if (this->GetWeaponRequired() == _SHIELD_SUBTYPE)
    {
        CItem* pWeaponL = g_objHero.GetEquipment(ITEMPOSITION_WEAPONL);
        if (!pWeaponL)
        {
            bEnable = false;
        }
        else if (pWeaponL->GetWeaponSkillType() != _SHIELD_SUBTYPE)
        {
            bEnable = false;
        }
    }
    // 其他需要武器的技能判断
    else if (this->GetWeaponRequired() != 0)
    {
        CItem* pWeaponR = g_objHero.GetEquipment(ITEMPOSITION_WEAPONR);
        CItem* pWeaponL = g_objHero.GetEquipment(ITEMPOSITION_WEAPONL);
        BOOL bEnable = FALSE;
        if ((pWeaponR && pWeaponR->GetWeaponSkillType() == this->GetWeaponRequired())
                || (pWeaponL && pWeaponL->GetWeaponSkillType() == this->GetWeaponRequired()))
        {
            bEnable = TRUE;
        }
        if (this->GetWeaponRequired() == 400
                && ((pWeaponR &&  (pWeaponR->GetWeaponSkillType () == 410 || pWeaponR->GetWeaponSkillType () == 420) )
                    || ( pWeaponL &&  (pWeaponL->GetWeaponSkillType () == 410 || pWeaponL->GetWeaponSkillType () == 420))))
        {
            bEnable = TRUE;
        }
    }
    // step 3: dead ...
    if (g_objHero.IsDead())
    {
        bEnable = false;
    }
    // step 4: PASSIVE ...
    //		if (this->TestTarget(MAGIC_PASSIVE))
    //			bEnable = false;
    // step 5: market ...
    if (g_objGameMap.GetType() & MAPTYPE_BOOTH_ENABLE)
    {
        if (!this->m_infoMagicType.dwCanBeusedInMarket)
        {
            bEnable = false;
        }
        return false;
    }
    // step 6: xp ...
    if (m_infoMagicType.dwXp == TYPE_XPSKILL && !g_objHero.TestStatus(USERSTATUS_XPFULL))
    {
        bEnable = false;
    }
    // step fly magic ...
    //		if (m_infoMagicType.dwStatus == USERSTATUS_FLY && !(g_objGameMap.GetType()&MAPTYPE_FLY_DISABLE))
    //		{
    //			bEnable = false;
    //		}
    //		if (m_infoMagicType.dwStatus == USERSTATUS_FLY && !g_objHero.HaveBow())
    //		{
    //			bEnable = false;
    //		}
    // step 7: teammagic
    if (m_infoMagicType.dwActionSort == MAGICSORT_TEAM_MAGIC && !g_objHero.IsTeamLeader())
    {
        bEnable = false;
    }
    if (bEnable != this->IsEnable())
    {
        m_bEnable = bEnable;
        return true;
    }
    return false;
}
//------------------------------------------------------------------
BOOL CMagic::Process()
{
    if (::TimeGet() - m_dwTimeBegin < m_infoMagicType.dwDelay)
    {
        m_bEnable = false;
        return false;
    }
    if (m_idOwner == g_objHero.GetID())
    {
        return	this->HeroMagicProcess();
    }
    else
    {
        return  this->PetMagicProcess();
    }
}
