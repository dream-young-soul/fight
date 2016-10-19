
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgMagicEffect.cpp: implementation of the CMsgMagicEffect class.
//
//////////////////////////////////////////////////////////////////////

#include "MsgMagicEffect.h"
#include "3DGameMap.h"
#include "Magic.h"
#include "Hero.h"
#include "GamePlayerSet.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgMagicEffect::CMsgMagicEffect()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgMagicEffect::~CMsgMagicEffect()
{
}

//-------------------------------------------------------------------

BOOL CMsgMagicEffect::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_MAGICEFFECT != this->GetType())
    {
        return false;
    }
    return true;
}

//-------------------------------------------------------------------
void CMsgMagicEffect::Process(void* pInfo)
{
    MagicTypeInfo infoMagicType;
    if (!CMagic::s_objMagicData.GetMagicTypeInfo(m_pInfo->usType, m_pInfo->usLevel, infoMagicType))
    {
        return;
    }
    if (m_pInfo->idUser == g_objHero.GetID())
    {
        CMagic* pMagic = g_objHero.GetMagic(m_pInfo->usType);
        if (pMagic)
        {
            pMagic->StartBusy();
        }
    }
    CPet* pPet = g_objHero.GetPetByID(m_pInfo->idUser);
    if (pPet)
    {
        CMagic* pMagic = pPet->GetMagic(m_pInfo->usType);
        if (pMagic)
        {
            pMagic->StartBusy();
        }
    }
    if (infoMagicType.bTrack)
    {
        CPlayer* pSender = g_objPlayerSet.GetPlayer(m_pInfo->idUser, true);
        if (pSender)
        {
            pSender->m_idLocker = m_pInfo->idUser;
        }
        for(int i = 0; i < m_pInfo->ucEffectNum; i ++)
        {
            CPlayer* pTarget;
            pTarget = g_objPlayerSet.GetPlayer(m_pInfo->setEffectRole[i].idRole, true);
            if (pTarget)
            {
                pTarget->m_setLife.push_back(m_pInfo->setEffectRole[i].dwData);
                pTarget->m_idLocker = m_pInfo->idUser;
            }
        }
        return;
    }
    if (m_pInfo->idUser == g_objHero.GetID())
    {
        g_objHero.EndMagicBusy();
        g_objHero.StartMagicBusy(g_objHero.GetMagicDelay());
        g_objHero.BeginTransForm(m_pInfo->usType, m_pInfo->usLevel);
        g_objHero.BeginFly(m_pInfo->usType, m_pInfo->usLevel);
    }
    CPlayer* pSender = g_objPlayerSet.GetPlayer(m_pInfo->idUser, true);
    if (pSender && pSender->IsMyPet())
    {
        CPet* pPet = g_objHero.GetPetByID(pSender->GetID());
        if (pPet)
        {
            pPet->SetAtkTarget(m_pInfo->setEffectRole[0].idRole);
        }
    }
    //	if ((infoMagicType.dwStatus & USERSTATUS_CHARGEUP) != 0)
    //		return;
    const int nMagic = 1501;
    if (m_pInfo->usType == 1501)
    {
        if (m_pInfo->idTarget == m_pInfo->idUser)
        {
            CPlayer* pSender = g_objPlayerSet.GetPlayer(m_pInfo->idUser, true);
            if (!pSender)
            {
                return;
            }
            pSender->FlashLastAttackTime();
            if (m_pInfo->ucEffectNum == 0)
            {
                return;
            }
            int nLife = (int)pSender->GetData(CPlayer::_PLAYER_LIFE) - (int)m_pInfo->setEffectRole[0].dwData;
            pSender->SetData(CPlayer::_PLAYER_LIFE, __max(0, nLife));
            DWORD dwColor = _COLOR_YELLOW;
            if (g_objHero.GetID() == pSender->GetID())
            {
                dwColor = _COLOR_RED;
            }
            pSender->AddZoomNum(m_pInfo->setEffectRole[0].dwData, dwColor, 300, 30);
            return;
        }
    }
    if (infoMagicType.dwCrime > 0)
    {
        if (m_pInfo->idUser == g_objHero.GetID())
        {
            if (m_pInfo->ucEffectNum >= 0)
            {
                g_objHero.SetIDMyTarget(m_pInfo->setEffectRole[0].idRole);
                g_objHero.FlashLastAttackTime();
            }
        }
        else
        {
            for(int i = 0; i < m_pInfo->ucEffectNum; i ++)
            {
                CPlayer* pPlayer = g_objPlayerSet.GetPlayer(m_pInfo->setEffectRole[i].idRole);
                if (pPlayer && pPlayer->IsMyPet() && !pSender->IsMyPet())
                {
                    pPlayer->SetIDWhoAttackMe(m_pInfo->idUser);
                    pPlayer->FlashLastAttackTime();
                }
                if (m_pInfo->setEffectRole[i].idRole == g_objHero.GetID() && !pSender->IsMyPet())
                {
                    g_objHero.SetIDWhoAttackMe(m_pInfo->idUser);
                    g_objHero.FlashLastAttackTime();
                }
            }
        }
    }
    //	if (infoMagicType.dwSenderAction == _ACTION_TM_FAST_SHOOT || infoMagicType.dwSenderAction == _ACTION_TM_DISPERSION_SHOOT)
    {
        CPlayer* pSender = g_objPlayerSet.GetPlayer(m_pInfo->idUser, true);
        CPlayer* pTarget = g_objPlayerSet.GetPlayer(m_pInfo->idTarget, true);
        if (!pSender)
        {
            return;
        }
        pSender->ResetStepDir();
        pSender->FlashLastAttackTime();
        if (!pSender->IsMoving())
        {
            pSender->ResetActionData();
        }
        int nDir = pSender->GetDir();
        if (pTarget)
        {
            nDir	= pSender->GetDir(pTarget);
        }
        if (pTarget)
        {
            pTarget->FlashLastAttackTime();
        }
        if (pSender->IsMonster())
        {
            pSender->SetDir(nDir);
        }
        switch(infoMagicType.dwSenderAction )
        {
        case _ACTION_TM_FAST_SHOOT:
            {
                pSender->SetActionCmd(_ACTION_FAST_SHOOT, nDir);
                int nAmount = infoMagicType.dwLevel + 2;
                if (m_pInfo->ucEffectNum > 0)
                {
                    int nLife = m_pInfo->setEffectRole[0].dwData;
                    int nLifeLost = nLife / nAmount;
                    int nLastLifeLost = nLife - (nAmount - 1) * nLifeLost;
                    DWORD dwDelay = 0;
                    for(int i = 0; i < nAmount; i ++)
                    {
                        CMagicEffect* pEffect = CMagicEffect::CreateNewArrow(pSender->GetID(), m_pInfo->setEffectRole[0].idRole, pSender->GetLWeapon(), dwDelay);
                        if (pEffect)
                        {
                            pEffect->m_nDir = m_pInfo->ucDir;
                            g_objGameMap.AddMagicEffect(pEffect);
                            if (i != nAmount - 1)
                                pEffect->AddRoleInfo(m_pInfo->setEffectRole[0].idRole,
                                                     nLifeLost);
                            else
                                pEffect->AddRoleInfo(m_pInfo->setEffectRole[0].idRole,
                                                     nLastLifeLost);
                        }
                        dwDelay += 150;
                    }
                }
            }
            return;
        case _ACTION_TM_DISPERSION_SHOOT:
            {
                pSender->SetActionCmd(_ACTION_ATTACK0 +::RandGet(3), nDir);
                // create arrow
                for(int i = 0; i < m_pInfo->ucEffectNum; i ++)
                {
                    CMagicEffect* pEffect = CMagicEffect::CreateNewArrow(pSender->GetID(), m_pInfo->setEffectRole[i].idRole, pSender->GetLWeapon());
                    if (pEffect)
                    {
                        pEffect->m_nDir = m_pInfo->ucDir;
                        g_objGameMap.AddMagicEffect(pEffect);
                        pEffect->AddRoleInfo(m_pInfo->setEffectRole[i].idRole,
                                             m_pInfo->setEffectRole[i].dwData);
                    }
                }
            }
            return;
        default:
            if (pSender->IsMonster())
            {
                pSender->SetActionCmd(_ACTION_ATTACK0 +::RandGet(3), nDir);
            }
            break;
        }
    }
    CMagicEffect* pEffect = NULL;
    if (infoMagicType.dwClientRepresent == MAGICEFFECT_BODY_TERRAIN_BODY)
    {
        CMyPos posTarget;
        posTarget.x = m_pInfo->usPosX;
        posTarget.y = m_pInfo->usPosY;
        pEffect =
            CMagicEffect::CreateNewTerrain(m_pInfo->idUser,
                                           posTarget, m_pInfo->usType, m_pInfo->usLevel);
    }
    else
    {
        pEffect =
            CMagicEffect::CreateNew(m_pInfo->idUser, m_pInfo->idTarget,
                                    m_pInfo->usType, m_pInfo->usLevel);
    }
    if (pEffect)
    {
        if (infoMagicType.dwActionSort != MAGICSORT_TEAM_MAGIC)
            for(int i = 0; i < m_pInfo->ucEffectNum; i ++)
                pEffect->AddRoleInfo(m_pInfo->setEffectRole[i].idRole,
                                     m_pInfo->setEffectRole[i].dwData);
        pEffect->m_nDir = m_pInfo->ucDir;
        g_objGameMap.AddMagicEffect(pEffect);
    }
}
//-------------------------------------------------------------------

