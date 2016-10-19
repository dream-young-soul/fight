
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgRegister.cpp: implementation of the CMsgRegister class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgRegister::CMsgRegister()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgRegister::~CMsgRegister()
{
}


//////////////////////////////////////////////////////////////////////
BOOL CMsgRegister::Create(const char* pszAccount,
                          const char* pszName,
                          const char* pszPassword,
                          DWORD unLook,
                          USHORT unData,
                          OBJID idAccount,
                          char	cLength,
                          char  cFat)
{
    // check param
    if (!pszAccount || _MAX_NAMESIZE <= strlen(pszAccount))
    {
        return false;
    }
    if (!pszName || _MAX_NAMESIZE <= strlen(pszName))
    {
        return false;
    }
    if (!pszPassword || _MAX_NAMESIZE <= strlen(pszPassword))
    {
        return false;
    }
    // init
    this->Init();
    // fill info now
    m_pInfo->unMsgSize	= sizeof(MSG_Info);
    m_pInfo->unMsgType	= _MSG_REGISTER;
    strcpy(m_pInfo->szAccount, pszAccount);
    strcpy(m_pInfo->szName, pszName);
    strcpy(m_pInfo->szPassword, pszPassword);
    m_pInfo->unLook			= unLook;
    m_pInfo->unData			= unData;
    m_pInfo->idAccount		= idAccount;
    m_pInfo->cLength		= cLength;
    m_pInfo->cFat			= cFat;
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgRegister::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_REGISTER != this->GetType())
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgRegister::Process(void* pInfo)
{
    ::LogMsg("Invalid msg CMsgRegister in client.");
}
