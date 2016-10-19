
//**********************************************************
// 代码编辑器
//**********************************************************

//--------------------------------------------------------------------------------------
// File Name: MsgLoginReplyEx.cpp
// Create by: Huang Yuhang
// Create on: 2004/11/12 15:52
//--------------------------------------------------------------------------------------
#include "AllMsg.h"
#include "Hero.h"
#include "GameDataSet.h"
#include "Register.h"
#include "Entrance.h"
#include "Shop.h"

// external...
extern UINT g_uStatus;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgLoginReplyEx::CMsgLoginReplyEx()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgLoginReplyEx::~CMsgLoginReplyEx()
{
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgLoginReplyEx::Create(OBJID idAccount, DWORD dwData, const char* pszInfo)
{
    // param check
    if (!pszInfo || strlen(pszInfo) >= _MAX_NAMESIZE_CLI)
    {
        return false;
    }
    // init
    this->Init();
    // fill info now
    m_pInfo->unMsgSize	= sizeof(MSG_Info);
    m_pInfo->unMsgType	= _MSG_LOGINREPLYEX;
    m_pInfo->idAccount		= idAccount;
    m_pInfo->dwData			= dwData;
    strcpy(m_pInfo->szInfo, pszInfo);
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgLoginReplyEx::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_LOGINREPLYEX != this->GetType())
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgLoginReplyEx::Process(void* pInfo)
{
    // check if the account is valid
    if (m_pInfo->idAccount == ID_NONE)	// invalid id
    {
        g_uStatus	= _STATUS_EXIT;
        char szInfo[256] = "";
        if ( strstr( m_pInfo->szInfo, "服务器未启动1" ) != NULL )
        {
            strcpy( szInfo, g_objGameDataSet.GetStr( 100400 ) );
        }
        else if ( strstr( m_pInfo->szInfo, "帐号名或口令错1" ) != NULL )
        {
            strcpy( szInfo, g_objGameDataSet.GetStr( 100401 ) );
        }
        else if ( strstr( m_pInfo->szInfo, "请稍后重新登录1" ) != NULL )
        {
            strcpy( szInfo, g_objGameDataSet.GetStr( 100402 ) );
        }
        else if ( strstr( m_pInfo->szInfo, "该帐号被封号1" ) != NULL )
        {
            strcpy( szInfo, g_objGameDataSet.GetStr( 100403 ) );
        }
        else if ( strstr( m_pInfo->szInfo, "该帐号被锁定1" ) != NULL )
        {
            strcpy( szInfo, g_objGameDataSet.GetStr( 100404) );
        }
        else if ( strstr( m_pInfo->szInfo, "服务器人数满1" ) != NULL )
        {
            strcpy( szInfo, g_objGameDataSet.GetStr( 100405 ) );
        }
        else if ( strstr( m_pInfo->szInfo, "服务器维护或网络堵塞1" ) != NULL )
        {
            strcpy( szInfo, g_objGameDataSet.GetStr( 100406 ) );
        }
        else
        {
            strcpy( szInfo, g_objGameDataSet.GetStr( 100100 ) );
        }
        g_objEntrance.AddMsg(m_pInfo->szInfo);
        return;
    }
    else
    {
        g_objHero.m_idAccount	= m_pInfo->idAccount;
    }
    DWORD dwEncyptCode = (m_pInfo->idAccount + m_pInfo->dwData) ^ 0x4321;
    // get res.dat
    CMsgConnect msg;
    /*	const char szVersionFile[]="res.dat";
    	FILE* fp = fopen(szVersionFile, "r");
    	char szRes[_MAX_NAMESIZE] = "ERROR";
    	DWORD dwRes = 0;
    	if (fp)
    	{
    		if (fscanf(fp, "%s\n", szRes))
    		fclose(fp);
    	}
    	if (msg.Create(m_pInfo->idAccount, 0, szRes))
    		msg.Send();*/
    // reconnect to game server
    {
        g_objNetwork.Destroy();
        // change connection to game server
        if (!g_objNetwork.Init(m_pInfo->szInfo, m_pInfo->nServePort))
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
    g_objHero.Set91ID(m_pInfo->dwUin);
    // change status
    g_uStatus = _STATUS_LOGIN;
}
