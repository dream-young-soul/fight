
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgConnect.cpp: implementation of the CMsgConnect class.
//
//////////////////////////////////////////////////////////////////////
//#include "AllMsg.h"
//#include "Account.h"
//#include "UserSet.h"
#include <assert.h>
#include "MsgConnect.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgConnect::CMsgConnect()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;

}

CMsgConnect::~CMsgConnect()
{

}

//////////////////////////////////////////////////////////////////////
BOOL CMsgConnect::Create(OBJID idAccount, DWORD dwData, char* pszInfo)
{
    // param check
    if (!pszInfo || strlen(pszInfo) >= _MAX_NAMESIZE)
    {
        return false;
    }

    // init
    this->Init();

    // fill info now
    m_pInfo->unMsgSize	= sizeof(MSG_Info);
    m_pInfo->unMsgType	= _MSG_CONNECT;

    m_pInfo->idAccount		= idAccount;
    m_pInfo->dwData			= dwData;
    strcpy(m_pInfo->szInfo, pszInfo);

    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgConnect::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }

    if (_MSG_CONNECT != this->GetType())
    {
        return false;
    }

    return true;
}

//////////////////////////////////////////////////////////////////////
/*void CMsgConnect::Process(CGameSocket* pSocket)
{
	assert(pSocket);

	// check it account and check data....default it's true

	// keep account id in CGameSocket
	pSocket->m_idAccount	=m_pInfo->idAccount;

	// so...we should create user now...
	// get role created by this account
	CUserSet* pUserSet	=CUser::GetUserSetByAccount(m_pInfo->idAccount);
	if (pUserSet)
	{
		int nUser	=pUserSet->GetUserAmount();
		for (int i=0; i<nUser; i++)
		{
			CUser* pUser	=pUserSet->GetUserByIndex(i);
			if (!pUser)
				continue;

			CMsgRole msg;
			if (msg.Create(pUser))
				CNetMsg::SendMsg(pSocket, msg.GetBuf(), msg.GetSize());

			delete pUser;
		}

		delete pUserSet;
	}

	// send msg info client login
	CMsgLogin msgLogin;
	char szString[256]	="login";
	if (msgLogin.Create(ID_NONE, 0, szString, 0))
		CNetMsg::SendMsg(pSocket, msgLogin.GetBuf(), msgLogin.GetSize());

#ifdef _MYDEBUG
	::LogSave("Process CMsgConnect, idAccount:%u, Data:%d, Info:%s",
				m_pInfo->idAccount,
				m_pInfo->dwData,
				m_pInfo->szInfo);
#endif
}
//*/
