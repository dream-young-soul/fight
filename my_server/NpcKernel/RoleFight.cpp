
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// RoleFight.cpp: implementation of the CRoleFight class.
//
//////////////////////////////////////////////////////////////////////

#include "common.h"
#include "define.h"
#include "RoleFight.h"
#include "Agent.h"
#include "AllMsg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRoleFight::CRoleFight()
{
}

CRoleFight::~CRoleFight()
{
}

bool CRoleFight::Create(CAgent* pOwner)
{
    m_pOwner	= pOwner;
    m_tAttackDelay.SetInterval(ATTACK_DELAY_MS);
    return true;
}

void CRoleFight::OnTimer()
{
    if (m_pTarget && QueryTime()->IsTimeOut())
    {
        if (m_pOwner->GetDistance(m_pTarget->GetPosX(), m_pTarget->GetPosY()) <= m_pOwner->GetAttackRange(m_pTarget->GetSizeAdd()))
        {
            if (m_tAttackDelay.ToNextTime())
            {
                CMsgInteract	msg;
                IF_OK(msg.Create(INTERACT_ATTACK, m_pOwner->GetID(), m_pTarget->GetID(), m_pOwner->GetPosX(), m_pOwner->GetPosY()))
                m_pOwner->SendMsg(&msg);
            }
        }
        else
        {
            m_pTarget.Clear();
        }
    }
}

bool CRoleFight::Attack(IRole* pRole)
{
    CHECKF(pRole);
    CHECKF(m_pOwner->GetDistance(pRole->GetPosX(), pRole->GetPosY()) <= m_pOwner->GetAttackRange(pRole->GetSizeAdd()));
    m_pTarget	= pRole;
    // msg
    CMsgInteract	msg;
    IF_OK(msg.Create(INTERACT_ATTACK, m_pOwner->GetID(), m_pTarget->GetID(), m_pOwner->GetPosX(), m_pOwner->GetPosY()))
    m_pOwner->SendMsg(&msg);
    m_tAttackDelay.Update();
    return true;
}
