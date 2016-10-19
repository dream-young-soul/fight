
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgLogin.cpp: implementation of the CMsgLogin class.
//
//////////////////////////////////////////////////////////////////////
//#include "AllMsg.h"
//#include "MyRoleSet.h"
#include "MsgLogin.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgLogin::CMsgLogin()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgLogin::~CMsgLogin()
{

}

//////////////////////////////////////////////////////////////////////
BOOL CMsgLogin::Create(OBJID idAccount, DWORD dwCheckData, char* pszName, USHORT usVersion)
{
    // check param
    if (!pszName || strlen(pszName) >= _MAX_NAMESIZE)
    {
        return false;
    }

    // init
    this->Init();

    // fill structure
    m_pInfo->unMsgType	= _MSG_LOGIN;
    m_pInfo->unMsgSize	= GetStdSize();

    m_pInfo->idAccount	= idAccount;
    m_pInfo->dwCheckData = dwCheckData;
    m_pInfo->usVersion	= usVersion;
    strcpy(m_pInfo->szUserName, pszName);
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgLogin::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!pbufMsg)
    {
        return false;
    }

    if ((unsigned short)dwMsgSize != GetSize(pbufMsg, dwMsgSize))
    {
        return false;
    }

    if (_MSG_LOGIN != GetType(pbufMsg, dwMsgSize))
    {
        return false;
    }

    memcpy(this->m_bufMsg, pbufMsg, dwMsgSize);
    return true;
}

//////////////////////////////////////////////////////////////////////
/*void CMsgLogin::Process(void* pInfo)
{
	// in client, this just a msg inform client to login game server.
	int nMyRoleAmount	=g_MyRoleSet.GetMyRoleAmount();
	if (nMyRoleAmount > 0)
	{
		extern int wsMaintainRole(void);
		wsMaintainRole();
	}
	else	// no role created
	{
		extern int wsCreateRole(void);
		wsCreateRole();
	}

	//--------------------
#ifdef _MYDEBUG
	::LogSave("Process MsgLogin, Account:%d, Name:%s, Version:%d",
					m_pInfo->idAccount,
					m_pInfo->szUserName,
					m_pInfo->usVersion);
#endif
}
//*/
