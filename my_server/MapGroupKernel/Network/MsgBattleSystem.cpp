
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgBattleSystem.cpp: implementation of the CMsgBattleSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgBattleSystem::CMsgBattleSystem()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgBattleSystem::~CMsgBattleSystem()
{
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgBattleSystem::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }
    //if (_MSG_BATTLESYSTEM != this->GetType())
    return false;
    return true;
}

///////////////////////////////////////////////////////////////////////////
BOOL CMsgBattleSystem::Create(OBJID idAtker, OBJID idDefer, int nLifeLost, int nDeferLife,
                              DWORD dwAtkerStatus, DWORD dwDeferStatus)
{
    m_pInfo->idAtker		= idAtker;
    m_pInfo->idDefer		= idDefer;
    m_pInfo->nDeferLifeLost	= nLifeLost;
    m_pInfo->nDeferLife		= nDeferLife;
    m_pInfo->dwAtkerStatus	= dwAtkerStatus;
    m_pInfo->dwDeferStatus	= dwDeferStatus;
    //m_unMsgType		= _MSG_BATTLESYSTEM;
    m_unMsgSize		= sizeof(MSG_Info);
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgBattleSystem::Process(void* pInfo)
{
}
