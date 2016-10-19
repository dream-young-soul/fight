
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgAccount.cpp: implementation of the CMsgAccount class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgAccount::CMsgAccount()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgAccount::~CMsgAccount()
{
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgAccount::Create(const char* pszAccount, const char* pszPsw, const char* pszServer)
{
    // param check
    if (!pszAccount || strlen(pszAccount) >= _MAX_NAMESIZE)
    {
        return false;
    }
    if (!pszPsw) // maybe binary data
        //|| strlen(pszPsw) >= _MAX_NAMESIZE)
    {
        return false;
    }
    if (!pszServer || strlen(pszServer) >= _MAX_NAMESIZE)
    {
        return false;
    }
    // init
    this->Init();
    // fill info
    strcpy(m_pInfo->szAccount, pszAccount);
    memcpy(m_pInfo->szPassword, pszPsw, _MAX_NAMESIZE);	// maybe binary data
    strcpy(m_pInfo->szServer, pszServer);
    m_pInfo->unMsgType	= _MSG_ACCOUNT;
    m_pInfo->unMsgSize	= sizeof(MSG_Info);
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgAccount::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_ACCOUNT != this->GetType())
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgAccount::Process(void* pInfo)
{
}
