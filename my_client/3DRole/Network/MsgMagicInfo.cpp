
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgMagicInfo.cpp: implementation of the CMsgMagicInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "MsgMagicInfo.h"
#include "Hero.h"
#include "GamePlayerset.h"
#include "Pet.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgMagicInfo::CMsgMagicInfo()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgMagicInfo::~CMsgMagicInfo()
{
}

//-------------------------------------------------------------------

BOOL CMsgMagicInfo::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_MAGICINFO != this->GetType())
    {
        return false;
    }
    return true;
}

//-------------------------------------------------------------------
void CMsgMagicInfo::Process(void* pInfo)
{
    if (m_pInfo->idOwner == g_objHero.GetID())
    {
        g_objHero.AddMagic(m_pInfo->usType, m_pInfo->usLevel, m_pInfo->dwExp);
        //set helptip
        if (g_objHero.m_bCanSetHelpTips)
        {
            g_objHero.SetHelpTip(0, m_pInfo->usType, m_pInfo->usLevel);
        }
    }
    else
    {
        if (g_objHero.CheckPet(m_pInfo->idOwner))
        {
            CPlayer* pPlayer = (CPet*)g_objPlayerSet.GetPlayer(m_pInfo->idOwner);
            if (pPlayer)
            {
                CPet* pPet = (CPet*)pPlayer;
                pPet->AddMagic(m_pInfo->usType, m_pInfo->usLevel, m_pInfo->dwExp);
            }
        }
    }
    ::PostCmd(CMD_FLASH_MAGIC);
}
//-------------------------------------------------------------------
