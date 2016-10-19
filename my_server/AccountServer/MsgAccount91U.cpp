
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgAccount.cpp: implementation of the CMsgAccount91U class.
//
//////////////////////////////////////////////////////////////////////

//#include "AllMsg.h"
//#include "Account.h"
#include <assert.h>
#include "MsgAccount91U.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgAccount91U::CMsgAccount91U()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgAccount91U::~CMsgAccount91U()
{

}

//////////////////////////////////////////////////////////////////////
BOOL CMsgAccount91U::Create(OBJID id91U, const char* pszAccount, const char* pszPsw, const char* pszServer, const char* szIPAddr)
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
    m_pInfo->id91U	= id91U;
    strcpy(m_pInfo->szAccount, pszAccount);
    strcpy(m_pInfo->szPassword, pszPsw);
    strcpy(m_pInfo->szServer, pszServer);
    strcpy(m_pInfo->szIPAddr, szIPAddr);

    m_pInfo->unMsgType	= _MSG_ACCOUNT91U;
    m_pInfo->unMsgSize	= sizeof(MSG_Info);

    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgAccount91U::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }

    if (_MSG_ACCOUNT91U != this->GetType())
    {
        return false;
    }

    return true;
}

//////////////////////////////////////////////////////////////////////
/*void CMsgAccount91U::Process(CGameSocket* pSocket)
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
	::LogSave("Process CMsgAccount91U, szAccount:%s, szPassword:%s",
				m_pInfo->szAccount,
				m_pInfo->szPassword);
#endif
}
//*/
