
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgMagicInfo.cpp: interface for the CMsgMagicInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "Magic.h"
#pragma	warning(disable:4786)

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

//////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////
BOOL CMsgMagicInfo::Create(OBJID idOwner, int nType, int nLevel, int nExp)
{
    CHECKF(nType);
    // init
    this->Init();
    // fill info now
    m_pInfo->idOwner		= idOwner;
    m_pInfo->usType			= nType;
    m_pInfo->usLevel		= nLevel;
    m_pInfo->dwExp			= nExp;
    m_unMsgType	= _MSG_MAGICINFO;
    m_unMsgSize	= sizeof(MSG_Info);
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgMagicInfo::Process(void* pInfo)
{
    ASSERT(!"CMsgMagicInfo::Process()");
    return;
#ifdef _MSGDEBUG
    ::LogMsg("Process CMsgMagicInfo, id:%u", m_info.id);
#endif
}
