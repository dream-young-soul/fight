
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgFlushExp.cpp: implementation of the CMsgFlushExp class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "mapgroup.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgFlushExp::CMsgFlushExp()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgFlushExp::~CMsgFlushExp()
{
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgFlushExp::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_FLUSHEXP != this->GetType())
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgFlushExp::Create(int nAction, int nType, DWORD dwExp)
{
    // param check
    // init
    this->Init();
    // fill info now
    m_unMsgSize	= sizeof(MSG_Info);
    m_unMsgType	= _MSG_FLUSHEXP;
    m_pInfo->ucAction		= nAction;
    m_pInfo->usType			= nType;
    m_pInfo->dwExp			= dwExp;
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgFlushExp::Process(void* pInfo)
{
    ASSERT(!"CMsgFlushExp");
}

