
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgTime.cpp: implementation of the CMsgTime class.
//
//////////////////////////////////////////////////////////////////////
#include "AllMsg.h"
#include "UserList.h"
#include "logfile.h"
using namespace world_kernel;
#include "WorldKernel.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgTime::CMsgTime()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgTime::~CMsgTime()
{
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgTime::Create(int nHour)
{
    // param check
    CHECKF(nHour >= 0 || nHour < 24);
    // init
    this->Init();
    // fill info now
    m_unMsgSize	= sizeof(MSG_Info);
    m_unMsgType	= _MSG_TIME;
    m_pInfo->ucHour			= nHour;
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgTime::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_TIME != this->GetType())
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgTime::Process(void* pSocket)
{
    ASSERT(!"CMsgTime::Process");
}
