
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgConnect.cpp: implementation of the CMsgConnect class.
//
//////////////////////////////////////////////////////////////////////
#include "AllMsg.h"
#include "UserList.h"
#include "logfile.h"
using namespace world_kernel;
#include "WorldKernel.h"

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
    m_unMsgSize	= sizeof(MSG_Info);
    m_unMsgType	= _MSG_CONNECT;
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
void CMsgConnect::Process(void* pSocket)
{
#ifdef _MYDEBUG_X
    ::LogSave("Process CMsgConnect, idAccount:%u, Data:%d, Info:%s",
              m_pInfo->idAccount,
              m_pInfo->dwData,
              m_pInfo->szInfo);
#endif
    if (!pSocket || m_pInfo->idAccount == ID_NONE)
    {
        return;
    }
    int nVersion = 0;
    if (1 != sscanf(m_pInfo->szInfo, "%d", &nVersion))
    {
        CMsgTalk msg;
        if (msg.Create(SYSTEM_NAME, ALLUSERS_NAME, STR_ERROR_VERSION, NULL, _COLOR_WHITE, _TXTATR_ENTRANCE))
        {
            SendMsg(&msg);
        }
        return;
    }
#ifdef	PALED_DEBUG
#else
    if (nVersion < GAME_VERSION)
    {
        CMsgTalk msg;
        if (msg.Create(SYSTEM_NAME, ALLUSERS_NAME, STR_ERROR_LOWER_VERSION, NULL, _COLOR_WHITE, _TXTATR_ENTRANCE))
        {
            SendMsg(&msg);
        }
        return;
    }
#endif
    extern DWORD g_dwMaxPlayer;
    if (GameWorld()->QueryUserList()->GetUserAmount() > g_dwMaxPlayer && m_pInfo->idAccount > MAX_GM_ID)
    {
        CMsgTalk msg;
        if (msg.Create(SYSTEM_NAME, ALLUSERS_NAME, STR_ERROR_SERVER_FULL, NULL, _COLOR_WHITE, _TXTATR_ENTRANCE))
        {
            SendMsg(&msg);
        }
        return;
    }
    // 接收完了MSG_CONNECT后，用认证号重置密码。
    DWORD dwEncyptCode = (m_pInfo->idAccount + m_pInfo->dwData) ^ 0x4321;
    GameWorld()->ChangeCode(GetSocketID(), m_pInfo->dwData ^ dwEncyptCode);
    if (GameWorld()->ClientConnect(m_pInfo->idAccount, m_pInfo->dwData, m_pInfo->szInfo, this->GetSocketID()))
    {
        CPlayer* pPlayer	= g_UserList.GetPlayerByAccountID(m_pInfo->idAccount);
        ASSERT(pPlayer);
        if (pPlayer)
        {
            UserList()->LoginToMapGroup(pPlayer);
        }
    }
}
