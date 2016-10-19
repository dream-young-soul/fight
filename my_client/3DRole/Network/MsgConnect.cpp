
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgConnect.cpp: implementation of the CMsgConnect class.
//
//////////////////////////////////////////////////////////////////////
#include "AllMsg.h"
#include "Hero.h"
//#include "AllChanel.h"
#include "GameDataSet.h"
#include "Register.h"
#include "Entrance.h"
#include "Shop.h"

// external...
extern UINT g_uStatus;

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
BOOL CMsgConnect::Create(OBJID idAccount, DWORD dwData, const char* pszInfo)
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
void CMsgConnect::Process(void* pInfo)
{
    // check if the account is valid
    if (m_pInfo->idAccount == ID_NONE)	// invalid id
    {
        g_uStatus	= _STATUS_EXIT;
        char szMsg[256];
        const OBJID idLoginInError = 100099;
        sprintf(szMsg, g_objGameDataSet.GetStr(idLoginInError), m_pInfo->szInfo);
        g_objEntrance.AddMsg(szMsg);
        return;
    }
    else
    {
        g_objHero.m_idAccount	= m_pInfo->idAccount;
    }
    DWORD dwEncyptCode = (m_pInfo->idAccount + m_pInfo->dwData) ^ 0x4321;
    // get res.dat
    CMsgConnect msg;
    const char szVersionFile[] = "res.dat";
    FILE* fp = fopen(szVersionFile, "r");
    DWORD dwRes = 0;
    if (fp)
    {
        if (fscanf(fp, "%d\n", &dwRes))
        {
            fclose(fp);
        }
    }
    if (msg.Create(m_pInfo->idAccount, dwRes, "res.dat"))
    {
        msg.Send();
    }
    // reconnect to game server
    {
        g_objNetwork.Destroy();
        // change connection to game server
        if (!g_objNetwork.Init(m_pInfo->szInfo, _PORT_GAMESERVER))
        {
            g_uStatus	= _STATUS_EXIT;
            const OBJID idConnectError = 100100;
            g_objEntrance.AddMsg(g_objGameDataSet.GetStr(idConnectError));
            return;
        }
    }
    // connect game server now
    char szVersion[256] = "";
    sprintf(szVersion, "%u", _VERSION);
    if (msg.Create(m_pInfo->idAccount, m_pInfo->dwData, szVersion))
    {
        msg.Send();
    }
    // change encrpt code
    g_objNetwork.ChangeCode(m_pInfo->dwData ^ dwEncyptCode);
    // change status
    g_uStatus = _STATUS_LOGIN;
}
