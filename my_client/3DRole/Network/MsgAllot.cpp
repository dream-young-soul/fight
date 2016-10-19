
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgAllot.cpp: implementation of the CMsgAllot class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgAllot::CMsgAllot()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgAllot::~CMsgAllot()
{
}

//-------------------------------------------------------------------

BOOL CMsgAllot::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_ALLOT != this->GetType())
    {
        return false;
    }
    return true;
}

//-------------------------------------------------------------------

BOOL CMsgAllot::Create(unsigned char ucForce, unsigned char ucSpeed, unsigned char ucHealth, unsigned char ucSoul)
{
    // init
    this->Init();
    if ((ucForce + ucSpeed + ucHealth + ucSoul) == 0)
    {
        return false;
    }
    m_pInfo->ucForce = ucForce;
    m_pInfo->ucHealth = ucHealth;
    m_pInfo->ucSoul = ucSoul;
    m_pInfo->ucSpeed = ucSpeed;
    m_unMsgType	= _MSG_ALLOT;
    m_unMsgSize	= sizeof(MSG_Info);
    return true;
}

//-------------------------------------------------------------------
void CMsgAllot::Process(void* pInfo)
{
    return;
}
//-------------------------------------------------------------------
