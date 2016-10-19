
//**********************************************************
// 代码编辑器
//**********************************************************

// 计费消息类
// 仙剑修，2001.11.22
//////////////////////////////////////////////////////////////////////

#include <assert.h>
#include "MsgFee.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgFee::CMsgFee()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgFee::~CMsgFee()
{

}

//////////////////////////////////////////////////////////////////////
BOOL CMsgFee::Create(OBJID idAccount, unsigned char ucType)
{
    // init
    this->Init();

    // fill info
    m_pInfo->idAccount	= idAccount;
    m_pInfo->ucType		= ucType;

    m_pInfo->unMsgType	= _MSG_FEE;
    m_pInfo->unMsgSize	= sizeof(MSG_Info);

    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgFee::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }

    if (_MSG_FEE != this->GetType())
    {
        return false;
    }

    return true;
}

//////////////////////////////////////////////////////////////////////
/*void CMsgFee::Process(CGameSocket* pSocket)
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
	::LogSave("Process CMsgFee, szAccount:%s, szPassword:%s",
				m_pInfo->szAccount,
				m_pInfo->szPassword);
#endif
}
//*/
