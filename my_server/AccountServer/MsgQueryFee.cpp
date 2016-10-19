
//**********************************************************
// 代码编辑器
//**********************************************************

// 查询费用类
// 仙剑修，2001.11.22
//////////////////////////////////////////////////////////////////////

#include <assert.h>
#include "AllHeads.h"
#include "MsgQueryFee.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgQueryFee::CMsgQueryFee()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgQueryFee::~CMsgQueryFee()
{

}

//////////////////////////////////////////////////////////////////////
BOOL CMsgQueryFee::Create(OBJID idAccount, unsigned char ucType, DWORD dwData, int nTime)
{
    // init
    this->Init();

    // fill info
    m_pInfo->idAccount	= idAccount;
    m_pInfo->ucType		= ucType;
    m_pInfo->dwData		= dwData;
    m_pInfo->nTime		= nTime;

    m_pInfo->unMsgType	= _MSG_QUERYFEE;
    m_pInfo->unMsgSize	= sizeof(MSG_Info);

    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgQueryFee::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }

    if (_MSG_QUERYFEE != this->GetType())
    {
        return false;
    }

    return true;
}

//////////////////////////////////////////////////////////////////////
/*void CMsgQueryFee::Process(CGameSocket* pSocket)
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
	::LogSave("Process CMsgQueryFee, szAccount:%s, szPassword:%s",
				m_pInfo->szAccount,
				m_pInfo->szPassword);
#endif
}
//*/
