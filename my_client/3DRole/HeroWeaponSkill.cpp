
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

//------------------------------------------------------------------------
// HeroWeaponSkill.cpp
//------------------------------------------------------------------------
#include "Hero.h"

//-----------------------------------------------------------------
void CHero::DestroyWeaponSkill()
{
    int nAmount, i;
    nAmount  = m_setSingleHandWeaponSkill.size();
    for(i = 0; i < nAmount; i ++)
    {
        CWeaponSkill* pSkill = m_setSingleHandWeaponSkill[i];
        SAFE_DELETE(pSkill);
    }
    m_setSingleHandWeaponSkill.clear();
    nAmount  = m_setDoubleHandWeaponSkill.size();
    for(i = 0; i < nAmount; i ++)
    {
        CWeaponSkill* pSkill = m_setDoubleHandWeaponSkill[i];
        SAFE_DELETE(pSkill);
    }
    m_setDoubleHandWeaponSkill.clear();
    nAmount  = m_setOtherWeaponSkill.size();
    for(i = 0; i < nAmount; i ++)
    {
        CWeaponSkill* pSkill = m_setOtherWeaponSkill[i];
        SAFE_DELETE(pSkill);
    }
    m_setOtherWeaponSkill.clear();
}
//-----------------------------------------------------------------
CWeaponSkill* CHero::GetWeaponSkill(int nWeaponSkillType, int nIndex)
{
    switch(nWeaponSkillType)
    {
    case SINGLEHAND_WEAPON_SKILL:
        return this->GetWeaponSkill(m_setSingleHandWeaponSkill, nIndex);
    case DOUBLEHAND_WEAPON_SKILL:
        return this->GetWeaponSkill(m_setDoubleHandWeaponSkill, nIndex);
    case OTHER_WEAPON_SKILL:
        return this->GetWeaponSkill(m_setOtherWeaponSkill, nIndex);
    default:
        break;
    }
    return NULL;
}
//-----------------------------------------------------------------
int CHero::GetWeaponSkillAmount(int nWeaponSkillType)
{
    switch(nWeaponSkillType)
    {
    case SINGLEHAND_WEAPON_SKILL:
        return m_setSingleHandWeaponSkill.size();
    case DOUBLEHAND_WEAPON_SKILL:
        return m_setDoubleHandWeaponSkill.size();
    case OTHER_WEAPON_SKILL:
        return m_setOtherWeaponSkill.size();
    default:
        break;
    }
    return 0;
}
//-----------------------------------------------------------------
CWeaponSkill* CHero::GetWeaponSkill(DEQUE_WEAPONSKILL& setSkill, int nIndex)
{
    int nAmount = setSkill.size();
    if ((nIndex >= nAmount) || (nIndex < 0))
    {
        return NULL;
    }
    return setSkill[nIndex];
}
//-----------------------------------------------------------------
CWeaponSkill* CHero::GetWeaponSkillByType(DEQUE_WEAPONSKILL& setSkill, int nType)
{
    int nAmount = setSkill.size();
    for(int i = 0; i < nAmount; i ++)
    {
        CWeaponSkill* pSkill = setSkill[i];
        if (pSkill && nType == pSkill->GetType())
        {
            return pSkill;
        }
    }
    return NULL;
}
//-----------------------------------------------------------------
void CHero::AddNewWeaponSkill(int nSkillType, DWORD dwLevel, DWORD dwExp)
{
    if (nSkillType / 100 == 4)
    {
        this->AddNewWeaponSkill(m_setSingleHandWeaponSkill, nSkillType, dwLevel, dwExp);
    }
    else if (nSkillType / 100 == 5)
    {
        this->AddNewWeaponSkill(m_setDoubleHandWeaponSkill, nSkillType, dwLevel, dwExp);
    }
    else
    {
        this->AddNewWeaponSkill(m_setOtherWeaponSkill, nSkillType, dwLevel, dwExp);
    }
}
//-----------------------------------------------------------------
void CHero::AddNewWeaponSkill(DEQUE_WEAPONSKILL& setSkill, int nSkillType, DWORD dwLevel, DWORD dwExp)
{
    int nAmount = setSkill.size();
    for(int i = 0; i < nAmount; i ++)
    {
        CWeaponSkill* pSkill = setSkill[i];
        if (pSkill && (pSkill->GetType() == nSkillType))
        {
            pSkill->SetLevel(dwLevel);
            pSkill->SetExp(dwExp);
            return;
        }
    }
    CWeaponSkill* pSkill = new CWeaponSkill;
    MYASSERT(pSkill);
    pSkill->SetType(nSkillType);
    pSkill->SetLevel(dwLevel);
    pSkill->SetExp(dwExp);
    setSkill.push_back(pSkill);
}
//-----------------------------------------------------------------
CWeaponSkill*	CHero::GetWeaponSkill(int nWeaponSkillType)
{
    CWeaponSkill* pSkill = NULL;
    pSkill = this->GetWeaponSkillByType(m_setSingleHandWeaponSkill, nWeaponSkillType);
    if (pSkill)
    {
        return pSkill;
    }
    pSkill = this->GetWeaponSkillByType(m_setDoubleHandWeaponSkill, nWeaponSkillType);
    if (pSkill)
    {
        return pSkill;
    }
    pSkill = this->GetWeaponSkillByType(m_setOtherWeaponSkill, nWeaponSkillType);
    if (pSkill)
    {
        return pSkill;
    }
    return NULL;
}
