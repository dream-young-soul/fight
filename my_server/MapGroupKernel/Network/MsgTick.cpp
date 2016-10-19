
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgTick.cpp: implementation of the CMsgTick class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "mapgroup.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgTick::CMsgTick()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgTick::~CMsgTick()
{
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgTick::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_TICK != this->GetType())
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgTick::Create(OBJID idPlayer)
{
    // param check
    if (idPlayer == ID_NONE)
    {
        return false;
    }
    // init
    this->Init();
    // fill info now
    m_unMsgSize	= sizeof(MSG_Info);
    m_unMsgType	= _MSG_TICK;
    m_pInfo->idPlayer	= idPlayer;
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgTick::Process(void* pInfo)
{
    CUserPtr pUser = UserManager()->GetUser(this);
    if (pUser)
    {
        /*
        // cheat test!
        DWORD dwMsgCount = m_pInfo->dwChkData^pUser->GetID();
        if (pUser->m_dwMsgCount > dwMsgCount
        		|| pUser->m_dwMsgCount+16 < dwMsgCount)	// cheater found!
        {
        	if (::RandGet(8) == 1)
        	{
        		pUser->DoCheaterPunish();

        		::MyLogSave("gmlog/cheater", "Invalid msg Cheater found: %s[%d]",
        					pUser->GetName(),
        					pUser->GetID());
        	}
        }
        */
        // tick process
        DWORD dwTime = m_pInfo->dwData ^ (m_pInfo->idPlayer * m_pInfo->idPlayer + 9527);
        pUser->ProcessTick(dwTime);
    }
}

