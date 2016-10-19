
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgAccount.cpp: implementation of the CMsgAccount class.
//
//////////////////////////////////////////////////////////////////////

//#include "AllMsg.h"
//#include "Account.h"
#include <assert.h>
#include "MsgAccount.h"

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

    if (!pszPsw || strlen(pszPsw) >= _MAX_NAMESIZE)
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
    strcpy(m_pInfo->szPassword, pszPsw);
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
/*void CMsgAccount::Process(CGameSocket* pSocket)
{
	assert(pSocket);

	CAccount* pAccount	=CAccount::CreateNew(m_pInfo->szAccount);
	if (!pAccount)
	{
		char szMsg[256]	="错误:非法帐号";

		CMsgConnect msg;
		if (msg.Create(ID_NONE, 0, szMsg))
			CNetMsg::SendMsg(pSocket, msg.GetBuf(), msg.GetSize());

		return;
	}

	if (!pAccount->CheckPassword(m_pInfo->szPassword))
	{
		char szMsg[256]	="错误:密码不符";

		CMsgConnect msg;
		if (msg.Create(ID_NONE, 0, szMsg))
			CNetMsg::SendMsg(pSocket, msg.GetBuf(), msg.GetSize());
	}
	else
	{
		char szMsg[256]	="成功:通过校验";

		CMsgConnect msg;
		if (msg.Create(pAccount->GetID(), 0, szMsg))
			CNetMsg::SendMsg(pSocket, msg.GetBuf(), msg.GetSize());
	}

	delete pAccount;

#ifdef _MYDEBUG
	::LogSave("Process CMsgAccount, szAccount:%s, szPassword:%s",
				m_pInfo->szAccount,
				m_pInfo->szPassword);
#endif
}
//*/
