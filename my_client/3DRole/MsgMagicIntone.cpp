
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgMagicIntone.cpp: implementation of the MsgMagicIntone class.
//
//////////////////////////////////////////////////////////////////////
#include "gameplayerset.h"
#include "Hero.h"
#include "Pet.h"
#include "3dGameMap.h"
#include "MsgMagicIntone.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgMagicIntone::CMsgMagicIntone()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgMagicIntone::~CMsgMagicIntone()
{
}

BOOL	CMsgMagicIntone::Create(char* pMsgBuf, DWORD dwSize)
{
    if (!CNetMsg::Create(pMsgBuf, dwSize))
    {
        return false;
    }
    if (_MSG_MAGICINTONE != this->GetType())
    {
        return false;
    }
    return true;
}

void	CMsgMagicIntone::Process(void* pInfo)
{
    if (m_pInfo->ucEudemonNum == 0)
    {
        return ;
    }
    MagicTypeInfo infoMagicType;
    if (!CMagic::s_objMagicData.GetMagicTypeInfo(m_pInfo->usMagicType, m_pInfo->usMagicLevel, infoMagicType))
    {
        return;
    }
    CPet* pPet = (CPet*)g_objPlayerSet.GetPlayer(m_pInfo->setEudemonPos[0].idEudemon);
    CPlayer* pPlayer = NULL;
    if (pPet)
    {
        pPlayer = g_objPlayerSet.GetPlayer(pPet->GetPetOwnerId(), true);
        if (pPlayer && pPlayer->GetID() != g_objHero.GetID())
        {
            pPlayer->ResetActionData();
            pPlayer->FlashLastAttackTime();
            CCommand cmd;
            cmd.iType		= _COMMAND_EMOTION;
            cmd.iStatus		= _CMDSTATUS_BEGIN;
            cmd.nData		= _ACTION_INTONE;
            CMyPos posSender;
            pPlayer->GetPos(posSender);
            cmd.nDir = m_pInfo->ucDir;
            pPlayer->SetCommand(&cmd);
            pPlayer->m_objEffect.Add("other27");
        }
    }
    for(int i = 0; i < m_pInfo->ucEudemonNum; i++)
    {
        CPet* pPet = (CPet*)g_objPlayerSet.GetPlayer(m_pInfo->setEudemonPos[i].idEudemon);
        if (pPet && pPet->IsPet() && !pPet->IsGhost())
        {
            if (m_pInfo->ucIntoneType == MAGICGROUP_ATTACK_NOMOVE)
            {
                pPet->ResetActionData();
                pPet->FlashLastAttackTime();
                pPet->StartMagicBusy(infoMagicType.dwIntoneDuration + infoMagicType.dwMagicBreak + 3000);
                CCommand cmd;
                cmd.iType		= _COMMAND_EMOTION;
                cmd.iStatus		= _CMDSTATUS_BEGIN;
                cmd.nData		= _ACTION_INTONE;
                CMyPos posSender, posTarget;
                pPet->GetPos(posSender);
                if (pPlayer)
                {
                    pPlayer->GetPos(posTarget);
                }
                cmd.nDir = (g_objGameMap.GetDirection(posSender.x, posSender.y, posTarget.x, posTarget.y) + 1) % 7;
                pPet->SetCommand(&cmd);
                pPet->m_objEffect.Add("pet3000");
            }
            else if (m_pInfo->ucIntoneType == MAGICGROUP_ATTACK_MOVE)
            {
                return;
            }
        }
    }
}
