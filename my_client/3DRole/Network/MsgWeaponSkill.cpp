
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgWeaponSkill.cpp: implementation of the CMsgWeaponSkill class.
//
//////////////////////////////////////////////////////////////////////

#include "MsgWeaponSkill.h"
#include "Hero.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgWeaponSkill::CMsgWeaponSkill()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgWeaponSkill::~CMsgWeaponSkill()
{
}
//---------------------------------------------------------------------
BOOL CMsgWeaponSkill::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_WEAPONSKILL != this->GetType())
    {
        return false;
    }
    return true;
}
//---------------------------------------------------------------------
void CMsgWeaponSkill::Process(void* pInfo)
{
#ifdef _MSGDEBUG
    ::LogMsg(Process CMsgWeaponSkill);
#endif
    g_objHero.AddNewWeaponSkill(m_pInfo->unType, m_pInfo->usLevel, m_pInfo->unExp);
    ::PostCmd(CMD_FLASHWEAPONSKILL);
}
//---------------------------------------------------------------------
