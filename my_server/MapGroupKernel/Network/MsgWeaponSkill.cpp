
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgWeaponSkill.cpp: implementation of the CMsgWeaponSkill class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"

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

//////////////////////////////////////////////////////////////////////
BOOL CMsgWeaponSkill::Create(char* pMsgBuf, DWORD dwSize)
{
    if (!CNetMsg::Create(pMsgBuf, dwSize))
    {
        return false;
    }
    if (_MSG_WEAPONSKILL != this->GetType())
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgWeaponSkill::Create(int nType, int nLevel, int nExp)
{
    // init
    this->Init();
    // fill info now
    m_unMsgSize	= sizeof(MSG_Info);
    m_unMsgType	= _MSG_WEAPONSKILL;
    m_pInfo->unType		= nType;
    m_pInfo->usLevel	= nLevel;
    m_pInfo->unExp		= nExp;
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgWeaponSkill::Process(void* pInfo)
{
}
