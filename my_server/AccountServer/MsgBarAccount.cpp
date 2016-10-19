
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// Íø°ÉÕÊºÅµÇÂ¼ÏûÏ¢
// ÏÉ½£ÐÞ£¬2002.2.26
//////////////////////////////////////////////////////////////////////

#include "AllHeads.h"
#include "MsgBarAccount.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgBarAccount::CMsgBarAccount()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgBarAccount::~CMsgBarAccount()
{

}

//////////////////////////////////////////////////////////////////////
BOOL CMsgBarAccount::Create(LPCTSTR pszAccount, LPCTSTR pszPsw, LPCTSTR pszServer, LPCTSTR pszFeeAccount, LPCTSTR pszFeePsw)
{
    ASSERT(!"CMsgBarAccount::Create()");
    return false;
    /*
    	// param check
    	if (!pszAccount || strlen(pszAccount) >= _MAX_NAMESIZE)
    		return false;

    	if (!pszPsw || strlen(pszPsw) >= _MAX_NAMESIZE)
    		return false;

    	if (!pszServer || strlen(pszServer) >= _MAX_NAMESIZE)
    		return false;

    	// init
    	this->Init();

    	// fill info
    	strcpy(m_pInfo->szAccount, pszAccount);
    	strcpy(m_pInfo->szPassword, pszPsw);
    	strcpy(m_pInfo->szServer, pszServer);

    	m_pInfo->unMsgType	=_MSG_BARACCOUNT;
    	m_pInfo->unMsgSize	=sizeof(MSG_Info);

    	return true;
    */
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgBarAccount::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }

    if (_MSG_BARACCOUNT != this->GetType())
    {
        return false;
    }

    return true;
}

//////////////////////////////////////////////////////////////////////
/*void CMsgBarAccount::Process(CGameSocket* pSocket)
{
	assert(pSocket);

	CAccount* pAccount	=CAccount::CreateNew(m_pInfo->szAccount);
	if (!pAccount)
	{
		char szMsg[256]	="´íÎó:·Ç·¨ÕÊºÅ";

		CMsgConnect msg;
		if (msg.Create(ID_NONE, 0, szMsg))
			CNetMsg::SendMsg(pSocket, msg.GetBuf(), msg.GetSize());

		return;
	}

	if (!pAccount->CheckPassword(m_pInfo->szPassword))
	{
		char szMsg[256]	="´íÎó:ÃÜÂë²»·û";

		CMsgConnect msg;
		if (msg.Create(ID_NONE, 0, szMsg))
			CNetMsg::SendMsg(pSocket, msg.GetBuf(), msg.GetSize());
	}
	else
	{
		char szMsg[256]	="³É¹¦:Í¨¹ýÐ£Ñé";

		CMsgConnect msg;
		if (msg.Create(pAccount->GetID(), 0, szMsg))
			CNetMsg::SendMsg(pSocket, msg.GetBuf(), msg.GetSize());
	}

	delete pAccount;

#ifdef _MYDEBUG
	::LogSave("Process CMsgBarAccount, szAccount:%s, szPassword:%s",
				m_pInfo->szAccount,
				m_pInfo->szPassword);
#endif
}
//*/
