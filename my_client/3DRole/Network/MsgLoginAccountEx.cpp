
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

//--------------------------------------------------------------------------------------
// File Name: MsgLoginAccountEx.cpp
// Create by: Huang Yuhang
// Create on: 2004/11/12 15:55
//--------------------------------------------------------------------------------------


#include "AllMsg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgLoginAccountEx::CMsgLoginAccountEx()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgLoginAccountEx::~CMsgLoginAccountEx()
{
}

//////////////////////////////////////////////////////////////////////
BOOL	CMsgLoginAccountEx::Create	(unsigned long uType, const char* pszAccount, const char* pszPsw, const char* pszServer, const char* szInfo)
{
    // param check
    if (!pszAccount || strlen(pszAccount) >= _MAX_NAMESIZE_CLI)
    {
        return false;
    }
    if (!pszPsw) // maybe binary data
        //|| strlen(pszPsw) >= _MAX_NAMESIZE)
    {
        return false;
    }
    if (!pszServer || strlen(pszServer) >= _MAX_NAMESIZE_CLI)
    {
        return false;
    }
    // init
    this->Init();
    // fill info
    strcpy(m_pInfo->szAccount, pszAccount);
    memcpy(m_pInfo->szPassword, pszPsw, _MAX_NAMESIZE_CLI);	// maybe binary data
    strcpy(m_pInfo->szServer, pszServer);
    m_pInfo->unType		= uType;
    strcpy(m_pInfo->szInfo, szInfo);
    m_pInfo->unMsgType	= _MSG_LOGINACCOUNTEX;
    m_pInfo->unMsgSize	= sizeof(MSG_Info);
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgLoginAccountEx::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_LOGINACCOUNTEX != this->GetType())
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgLoginAccountEx::Process(void* pInfo)
{
}
