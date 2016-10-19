
//**********************************************************
// 代码编辑器
//**********************************************************

// WorldKernel.cpp: implementation of the CWorldKernel class.
//
//////////////////////////////////////////////////////////////////////
#define	_WINSOCKAPI_		// 阻止加载winsock.h
#include "inifile.h"
#include "protocol.h"
#include "windows.h"
#include "Winuser.h"
#include "BaseFunc.h"
#include "I_MessagePort.h"
#include "AllMsg.h"
#include "NetMsg.h"
#include "MessageBoard.h"
#include "WorldKernel.h"
#include "ClientSocket.h"
#include "MsgAccount_A.h"
#include "MsgConnect_A.h"
#include "MsgLogin_A.h"
#include "MsgFee_A.h"
#include "MsgQueryFee_A.h"
#include "SynWorldManager.h"
#include "SupermanList.h"
#include "AutoPtr.h"
#include "ChatRoomManager.h"

using namespace world_kernel;

//////////////////////////////////////////////////////////////////////
CWorldKernel*	CWorldKernel::m_pWorld = NULL;

//extern CChatRoomManager g_objChatRoomManager;
//////////////////////////////////////////////////////////////////////
// interface
//////////////////////////////////////////////////////////////////////
bool CWorldKernel::Create(IMessagePort* pPort)
{
    m_pMsgPort		= pPort;
    m_idProcess		= m_pMsgPort->GetID();		// process id == msg_port_id
    m_pMsgPort->Open();
    ASSERT(m_idProcess == MSGPORT_WORLD);
    m_idNpcSocket	= SOCKET_NONE;
    // 初始化数据库
    char	DB_IP	[DBSTR_SIZE] = "";
    char	DB_USER	[DBSTR_SIZE] = "";
    char	DB_PW	[DBSTR_SIZE] = "";
    char	DB_DB	[DBSTR_SIZE] = "";
    CIniFile	ini(CONFIG_FILENAME, "Database");
    ini.GetString(DB_IP,	"DB_IP",	DBSTR_SIZE);
    ini.GetString(DB_USER,	"DB_USER",	DBSTR_SIZE);
    ini.GetString(DB_PW,	"DB_PW",	DBSTR_SIZE);
    ini.GetString(DB_DB,	"DB_DB",	DBSTR_SIZE);
    m_pDb	= ::CreateDatabase(DB_IP, DB_USER, DB_PW, DB_DB);
    if (!m_pDb)
    {
        LOGERROR("数据库连接失败!");
        return false;
    }
    // TODO: 请在此添加初始化代码
    m_pUserList	= CUserList::CreateNew();
    if (!m_pUserList)
    {
        return false;
    }
    m_pUserList->Create(GetSocketInterface(), m_pDb);
    m_pMapList	= CMapList::CreateNew();
    if (!m_pMapList)
    {
        return false;
    }
    m_pMapList->Create(m_pDb);
    m_pWorld	= this;
    m_pSynManager	= new CSynWorldManager;
    CHECKF(m_pSynManager);
    m_pSynManager->Create(m_pDb);
    m_setConnectClient		= CConnectSet::CreateNew(true);
    m_setConnectAccount		= CConnectSet::CreateNew(true);
    // account server
    //	CIniFile	ini(CONFIG_FILENAME, "AccountServer");
    ini.SetSection("AccountServer");
    NAMESTR		ACCOUNT_IP;
    int			ACCOUNT_PORT;
    ini.GetString(ACCOUNT_IP, "ACCOUNT_IP", _MAX_NAMESIZE);
    ACCOUNT_PORT	= ini.GetInt("ACCOUNT_PORT");
    ini.GetString(m_szServer, "SERVERNAME", _MAX_NAMESIZE);
    ini.GetString(m_szAccount, "LOGINNAME", _MAX_NAMESIZE);
    ini.GetString(m_szPassword, "PASSWORD", _MAX_NAMESIZE);
    if (ACCOUNT_PORT == 0)
    {
        LOGERROR("INI配置错误!");
        return false;
    }
    if (m_sock.Open(ACCOUNT_IP, ACCOUNT_PORT, 0, SOCKET_ACCOUNTBUFSIZE, SOCKET_ACCOUNTBUFSIZE))
    {
        m_nState = STATE_CONNECTOK;
    }
    m_ptrTradeMsgBd		= CMessageBoard::CreateNew(_TXTATR_MSG_TRADE);
    m_ptrFriendMsgBd	= CMessageBoard::CreateNew(_TXTATR_MSG_FRIEND);
    m_ptrTeamMsgBd		= CMessageBoard::CreateNew(_TXTATR_MSG_TEAM);
    m_ptrOtherMsgBd		= CMessageBoard::CreateNew(_TXTATR_MSG_OTHER);
    m_ptrSystemMsgBd	= CMessageBoard::CreateNew(_TXTATR_MSG_SYSTEM);
    LoadConfig(m_pDb);
    //	g_objChatRoomManager.Init();
    return true;		// return false : 创建失败，程序关闭。
}

//////////////////////////////////////////////////////////////////////
bool CWorldKernel::ProcessMsg(OBJID idPacket, void* buf, int nType, int nFrom)
{
    switch(idPacket)
    {
    case	KERNEL_CLIENTMSG:
        {
            CLIENTMSG_PACKET0*	pMsg = (CLIENTMSG_PACKET0*)buf;
            SOCKET_ID	idSocket = pMsg->idSocket;
            OBJID	idMsg	= pMsg->idPacket;
            char*	pBuf	= pMsg->buf;
            int		nMsgLen	= pMsg->nSize;
            ProcessClientMsg(idSocket, idMsg, pBuf, nMsgLen);
        }
        break;
    case	WORLD_TRANSMITMSG:
        {
            TRANSMSG_PACKET0*	pMsg = (TRANSMSG_PACKET0*)buf;
            SOCKET_ID	idSocket = pMsg->idSocket;
            OBJID		idNpc	= pMsg->idNpc;
            OBJID	idMsg	= pMsg->idPacket;
            char*	pBuf	= pMsg->buf;
            int		nMsgLen	= pMsg->nSize;
            int		nTrans	= pMsg->nTrans;
            if (idSocket == SOCKET_NONE)
            {
                ProcessNpcMsg(idNpc, idMsg, pBuf, nMsgLen, nTrans);
            }
            else
            {
                ProcessClientMsg(idSocket, idMsg, pBuf, nMsgLen, nTrans);
            }
        }
        break;
    case	KERNEL_NPCMSG:
        {
            NPCMSG_PACKET0*	pMsg = (NPCMSG_PACKET0*)buf;
            OBJID	idNpc	= pMsg->idNpc;
            OBJID	idMsg	= pMsg->idPacket;
            char*	pBuf	= pMsg->buf;
            int		nMsgLen	= pMsg->nSize;
            ProcessNpcMsg(idNpc, idMsg, pBuf, nMsgLen);
        }
        break;
    case	WORLD_SHELLTALK:
        {
            char* pText = (char*)buf;
            // TODO: 请在此添加外壳输入文本的处理代码
            m_pMsgPort->Send(MSGPORT_SHELL, SHELL_PRINTTEXT, STRING_TYPE(pText), pText);
            char szCmd[256] = "";
            if (sscanf(pText, "%s", szCmd) == 1)
            {
                if (0 == stricmp(szCmd, "setmaxplayer"))
                {
                    int nMaxPlayers = 0;
                    if (2 == sscanf(pText, "%s %d", szCmd, &nMaxPlayers))
                    {
                        extern DWORD g_dwMaxPlayer;
                        InterlockedExchange((long*)&g_dwMaxPlayer, nMaxPlayers);
                        {
                            char szMsg[256] = "";
                            sprintf(szMsg, "Set max player limit to %d", g_dwMaxPlayer);
                            m_pMsgPort->Send(MSGPORT_SHELL, SHELL_PRINTTEXT, STRING_TYPE(szMsg), szMsg);
                        }
                    }
                }
                else if (0 == stricmp(szCmd, "kickoutcheat"))
                {
                    int	nData = 0;
                    if (2 == sscanf(pText, "%s %d", szCmd, &nData))
                    {
                        extern long	g_sKickoutCheat;
                        long nOld = InterlockedExchange(&g_sKickoutCheat, nData);
                    }
                }
                else if (0 == stricmp(szCmd, "kickoutall"))
                {
                    OBJID idGmSocket = -1;		// -1: for console
                    m_pMsgPort->Send(MSGPORT_SOCKET, SOCKET_BREAKALLCONNECT, VARTYPE_INT, &idGmSocket);
                }
                else if (0 == stricmp(szCmd, "broadcast"))
                {
                    char* pCmd = pText + strlen(szCmd);
                    if (*pCmd == ' ')
                    {
                        pCmd++;
                    }
                    GameWorld()->QueryUserList()->BroadcastMsg(pCmd, NULL, NULL, 0x00FF0000, _TXTATR_GM);
                }
                else if (0 == stricmp(szCmd, "shutdown"))
                {
                    GameWorld()->QueryUserList()->BroadcastMsg(STR_SHUTDOWN_NOTIFY, NULL, NULL, 0x00FF0000, _TXTATR_GM);
                    char* pMsg = "Send shut down broadcast message.";
                    m_pMsgPort->Send(MSGPORT_SHELL, SHELL_PRINTTEXT, STRING_TYPE(pMsg), pMsg);
                }
                else if (0 == stricmp(szCmd, "restartserver"))
                {
                    extern long g_nServerClosed;
                    if (g_nServerClosed)
                    {
                        extern long g_nRestart;
                        g_nRestart = true;
                    }
                    else
                    {
                        LPCTSTR pMsg = "Please <kickoutall> before <restartserver>!";
                        m_pMsgPort->Send(MSGPORT_SHELL, SHELL_PRINTTEXT, STRING_TYPE(pMsg), pMsg);
                    }
                }
                else if (0 == stricmp(szCmd, "restartwindows"))
                {
                    extern long g_nServerClosed;
                    if (g_nServerClosed)
                    {
                        HANDLE hToken;
                        TOKEN_PRIVILEGES tkp;
                        // Get a token for this process.
                        if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
                        {
                            LOGERROR("OpenProcessToken");
                            break;
                        }
                        // Get the LUID for the shutdown privilege.
                        LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
                        tkp.PrivilegeCount = 1; // one privilege to set
                        tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
                        // Get the shutdown privilege for this process.
                        AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
                        // Cannot test the return value of AdjustTokenPrivileges.
                        if (GetLastError() != ERROR_SUCCESS)
                        {
                            LOGERROR("AdjustTokenPrivileges");
                            break;
                        }
                        // Shut down the system and force all applications to close.
#define EWX_FORCEIFHUNG      0x00000010
                        if (!ExitWindowsEx(EWX_REBOOT | EWX_FORCEIFHUNG, 0))
                        {
                            LOGERROR("ExitWindowsEx");
                            break;
                        }
                        //						::ExitWindowsEx(EWX_REBOOT, 0);		// | EWX_FORCEIFHUNG
                    }
                    else
                    {
                        LPCTSTR pMsg = "Please <kickoutall> before <restartserver>!";
                        m_pMsgPort->Send(MSGPORT_SHELL, SHELL_PRINTTEXT, STRING_TYPE(pMsg), pMsg);
                    }
                }
            }
        }
        break;
        // TODO: 请在此添加其它内部消息处理代码
    case	KERNEL_CLOSEKERNEL:
        {
            SOCKET_ID	idSocket = *(SOCKET_ID*)buf;
            LOGDEBUG("DEBUG：世界核心收到关闭核心消息，SOCKET_ID[%u]", idSocket);
#ifdef	NEW_LOGOUT
            g_UserList.LogoutUser(idSocket);
            m_pMsgPort->Send(MSGPORT_SOCKET, SOCKET_CLOSESOCKET, VARTYPE_INT, &idSocket);		// readme.txt(2-1)
#else
            if (g_UserList.IsLoginMapGroup(idSocket))
            {
                for(int i = MSGPORT_MAPGROUP_FIRST; i < m_pMsgPort->GetSize(); i++)
                {
                    m_pMsgPort->Send(i, KERNEL_CLOSEKERNEL, VARTYPE_INT, &idSocket);    // readme.txt(2-2)
                }
            }
            else
            {
                m_pMsgPort->Send(MSGPORT_SOCKET, SOCKET_CLOSESOCKET, VARTYPE_INT, &idSocket);    // readme.txt(2-1)
            }
#endif
        }
        break;
    case	WORLD_CLOSESOCKET:		// 由MAPGROUP发过来
        {
#ifdef	NEW_LOGOUT
            ASSERT(!"WORLD_CLOSESOCKET");
#else
            SOCKET_ID	idSocket = *(SOCKET_ID*)buf;
            LOGDEBUG("DEBUG：世界核心收到关闭网络消息，SOCKET_ID[%u]", idSocket);
            g_UserList.LogoutUser(idSocket);
            m_pMsgPort->Send(MSGPORT_SOCKET, SOCKET_CLOSESOCKET, VARTYPE_INT, &idSocket);		// readme.txt(2-6)
#endif
        }
        break;
    case	WORLD_SETPROCESSID:
        {
            CHANGE_USERDATA* pPakcet = (CHANGE_USERDATA*)buf;
            g_UserList.ChangeProcessID(pPakcet->idSocket, pPakcet->nData);
        }
        break;
    case	WORLD_USERNAMEMSG:
        {
            USERNAMEMSG_PACKET0*	pMsg = (USERNAMEMSG_PACKET0*)buf;
            SOCKET_ID	idSocket = pMsg->idSocket;
            OBJID	idMsg	= pMsg->idPacket;
            char*	pBuf	= pMsg->buf;
            int		nMsgLen	= pMsg->nSize;
            //			if (idMsg == _MSG_TALK)
            //			{
            //				// 服务频道判断
            //				typedef struct {
            //					DWORD	dwWordsColor;
            //					USHORT	unTxtAttribute;
            //					// ... 其它定义没必要写了
            //				}MSG_Info;
            //				MSG_Info* pInfo = (MSG_Info*)pBuf;
            //				if (pInfo->unTxtAttribute == _TXTATR_SERVE)
            //				{
            //					CPlayer* pTarget = UserList()->GetGM();
            //					if (pTarget)
            //					{
            //						SendMsg(pTarget->idSocket, idMsg, pBuf, nMsgLen);
            //						break;
            //					}
            //				}
            //			}
            CPlayer* pPlayer = UserList()->GetPlayer(pMsg->szName);
            if (pPlayer)
            {
                if (!pPlayer->IsAgent())
                {
                    SendMsg(pPlayer->idSocket, idMsg, pBuf, nMsgLen);
                }
                else
                {
                    SendNpcMsg(pPlayer->GetID(), idMsg, pBuf, nMsgLen);
                }
            }
            else if (idMsg == _MSG_TALK && idSocket != SOCKET_NONE)		// SOCKET_NONE : agent talk to user (BUG :)
            {
                // 私聊
                CPlayer* pSender = UserList()->GetPlayerBySocket(idSocket);
                if (pSender)
                {
                    CMsgTalk	msg;
                    IF_OK(msg.Create(SYSTEM_NAME, pSender->szName, STR_TARGET_OFFLINE))
                    pSender->SendMsg(&msg);
                }
            }
        }
        break;
    case	WORLD_USERIDMSG:
        {
            USERIDMSG_PACKET0*	pMsg = (USERIDMSG_PACKET0*)buf;
            SOCKET_ID	idSocket = pMsg->idSocket;
            OBJID	idMsg	= pMsg->idPacket;
            char*	pBuf	= pMsg->buf;
            int		nMsgLen	= pMsg->nSize;
            CPlayer* pPlayer = UserList()->GetPlayer(pMsg->idUser);
            if (pPlayer)
            {
                if (!pPlayer->IsAgent())
                {
                    SendMsg(pPlayer->idSocket, idMsg, pBuf, nMsgLen);
                }
                else
                {
                    SendNpcMsg(pPlayer->GetID(), idMsg, pBuf, nMsgLen);
                }
            }
        }
        break;
        // syndicate -------------------------------------------------------------------------------
    case	KERNEL_CREATESYNDICATE:
        {
            CreateSyndicateInfo* pPacket = (CreateSyndicateInfo*)buf;
            GameWorld()->QuerySynManager()->QueryModify()->CreateSyndicate(pPacket);
        }
        break;
    case	KERNEL_DESTROYSYNDICATE:
        {
            OBJID* pId	= (OBJID*)buf;
            OBJID idSyn		= pId[0];
            OBJID idTarget	= pId[1];
            GameWorld()->QuerySynManager()->QueryModify()->DestroySyndicate(idSyn, idTarget);
        }
        break;
    case	KERNEL_COMBINESYNDICATE:
        {
            OBJID* pId	= (OBJID*)buf;
            OBJID idSyn		= pId[0];
            OBJID idTarget	= pId[1];
            GameWorld()->QuerySynManager()->QueryModify()->CombineSyndicate(idSyn, idTarget);
        }
        break;
    case	KERNEL_CHANGESYNDICATE:
        {
            SynFuncInfo0* pPacket = (SynFuncInfo0*)buf;
            CSyndicateWorld* pSyn = GameWorld()->QuerySynManager()->QuerySyndicate(pPacket->idSyn);
            IF_OK(pSyn)
            {
                pSyn->ChangeSyndicate(pPacket);
            }
        }
        break;
    case	WORLD_LEVELUP:
        {
            OBJID idUser = *(OBJID*)buf;
            CPlayer* pPlayer = GameWorld()->QueryUserList()->GetPlayer(idUser);
            IF_OK(pPlayer)
            pPlayer->m_nLevel	= LOGIN_FREE_LEVEL;
        }
        break;
    case	WORLD_QUERYFEE:
        {
            OBJID idAccount = *(OBJID*)buf;
            if (IsAccountLoginOK())
            {
                SendQueryFee(idAccount, 0, 0, 0);
            }
        }
        break;
    case	WORLD_SENDTIME:
        {
            SOCKET_ID idSocket = *(SOCKET_ID*)buf;
            IF_OK(idSocket != SOCKET_NONE)
            UserList()->SendTimeToSocket(idSocket);
        }
        break;
    case	KERNEL_SUPERMANLIST:
        {
            VARIANT_SET0* pInfo  = (VARIANT_SET0*)buf;
            OBJID	idUser	= pInfo->IntParam(0);
            int		nNumber	= pInfo->IntParam(1);
            if (nNumber)
            {
                UserList()->QuerySupermanList()->AddMember(idUser, nNumber);
            }
            else
            {
                UserList()->QuerySupermanList()->QueryMember(idUser);
            }
        }
        break;
    case	KERNEL_QUERYSUPERMANLIST:
        {
            VARIANT_SET0* pInfo  = (VARIANT_SET0*)buf;
            OBJID	idUser	= pInfo->IntParam(0);
            int		nIndex	= pInfo->IntParam(1);
            OBJID	idAction	= pInfo->IntParam(2);
            int		nNumber	= pInfo->IntParam(3);
            UserList()->QuerySupermanList()->SendList(idUser, nIndex, nNumber, idAction);
        }
        break;
    default:
        ASSERT(!"CWorldKernel::ProcessMsg()");
    }
    return true;		// return false : 消息处理异常，程序关闭。
}

//////////////////////////////////////////////////////////////////////
bool CWorldKernel::OnTimer(time_t tCurr)
{
    // TODO: 请在此添加时钟处理代码
    g_UserList.OnTimer(tCurr);
#ifdef	ACCOUNT_ENABLE
    // account server
    if (!m_sock.IsOpen())
    {
        m_nState = STATE_NONE;
        GameWorld()->PrintText("Connect to account server again...");
        if (m_sock.Open(0))
        {
            m_nState = STATE_CONNECTOK;
        }
    }
#endif
    if (m_sock.IsOpen())
    {
        if (m_nState == STATE_CONNECTOK)
        {
            SendAccount(m_szAccount, m_szPassword, m_szServer);
            m_nState = STATE_ACCOUNTOK;
        }
        ProcessAccountNetMsg();
    }
    //	g_objChatRoomManager.OnTimer(tCurr);
    return true;		// return false : 消息处理异常，程序关闭。
}

//////////////////////////////////////////////////////////////////////
bool CWorldKernel::Release()
{
    // TODO: 请在此添加代码
    g_UserList.LogoutAllUser();
    SAFE_RELEASE (m_pUserList);
    SAFE_RELEASE (m_pMapList);
    SAFE_RELEASE (m_pSynManager);
    m_pMsgPort->Close();
    GameWorld()->SendFee(ID_NONE, CMsgFee_A::SERVER_CLOSE);
    m_sock.Close();
    SAFE_RELEASE (m_setConnectClient);
    SAFE_RELEASE (m_setConnectAccount);
    SAFE_RELEASE (m_pDb);
    delete this;
    return true;		// return false : 无意义。
}

//////////////////////////////////////////////////////////////////////
bool CWorldKernel::ProcessClientMsg(SOCKET_ID idSocket, OBJID idMsg, const char* pbufMsg, int nSize, int nTrans)
{
    // TODO: 请在此添加客户端上传消息的处理代码
    if (idMsg == _MSG_NONE || !pbufMsg || nSize <= 0 || nSize > _MAX_MSGSIZE)
    {
        return false;
    }
    CNetMsg* pMsg	= CNetMsg::CreateClientMsg(m_idProcess, idSocket, idMsg, pbufMsg, nSize, nTrans);
    if (pMsg)
    {
        try
        {
            pMsg->Process(this);
        }
        catch(...)
        {
            //			CNetMsg::DumpMsg(pMsg);
            ASSERT(!"pMsg->Process(this);");
            ::LogSave("exception catch at CGameSocket::ProcessMsg()! MsgType:%d, SocketID:%u", idMsg, idSocket);
        }
        delete pMsg;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CWorldKernel::SendMsg(CNetMsg* pNetMsg)
{
    ASSERT(pNetMsg);
    if (!pNetMsg)
    {
        return false;
    }
    if (!pNetMsg->IsNpcMsg())
    {
        return SendMsg(pNetMsg->GetSocketID(), pNetMsg->GetType(), pNetMsg->GetBuf(), pNetMsg->GetSize());
    }
    else
    {
        return SendNpcMsg(pNetMsg->GetNpcID(), pNetMsg->GetType(), pNetMsg->GetBuf(), pNetMsg->GetSize());
    }
}

//////////////////////////////////////////////////////////////////////
bool CWorldKernel::SendMsg(SOCKET_ID idSocket, OBJID idMsg, const char* pBuf, int nMsgLen)
{
    // TODO: 请在此添加NPC服务器下传消息的代码
    if (!pBuf)
    {
        ::LogSave("Error: null msg point found in CGameSocket::SendMsg.");
        return false;
    }
    if (_MSG_NONE == idMsg)
    {
        ::LogSave("Error: invalid msg type in CGameSocket::SendMsg().");
        return false;
    }
    try
    {
        MESSAGESTR	bufPacket;
        SENDCLIENTMSG_PACKET0* pPacket = (SENDCLIENTMSG_PACKET0*)bufPacket;
        pPacket->idSocket	= idSocket;
        pPacket->idPacket	= idMsg;
        pPacket->nSize		= nMsgLen;
        memcpy(pPacket->buf, pBuf, nMsgLen);
        m_pMsgPort->Send(MSGPORT_SOCKET, SOCKET_SENDCLIENTMSG, STRUCT_TYPE(bufPacket), &bufPacket);
        return true;
    }
    catch(...)
    {
        ASSERT(!"catch");
        ::LogSave("Error: exception catched in CGameSocket::SendMsg().");
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CWorldKernel::ProcessNpcMsg(OBJID idNpc, OBJID idMsg, const char* pbufMsg, int nSize, int nTrans)
{
    // TODO: 请在此添加NPC服务器上传消息的处理代码
    if (idMsg == _MSG_NONE || !pbufMsg || nSize <= 0 || nSize > _MAX_MSGSIZE)
    {
        return false;
    }
    CNetMsg* pMsg	= CNetMsg::CreateNpcMsg(m_idProcess, idNpc, idMsg, pbufMsg, nSize, nTrans);
    if (pMsg)
    {
        try
        {
            pMsg->Process(this);
        }
        catch(...)
        {
            //			CNetMsg::DumpMsg(pMsg);
            ASSERT(!"pMsg->Process(this);");
            ::LogSave("exception catch at CGameSocket::ProcessMsg()! MsgType:%d, NPCID:%u", idMsg, idNpc);
        }
        delete pMsg;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CWorldKernel::SendNpcMsg(CNetMsg* pNetMsg)
{
    ASSERT(pNetMsg);
    if (!pNetMsg)
    {
        return false;
    }
    return SendNpcMsg(pNetMsg->GetNpcID(), pNetMsg->GetType(), pNetMsg->GetBuf(), pNetMsg->GetSize());
}

//////////////////////////////////////////////////////////////////////
bool CWorldKernel::SendNpcMsg(OBJID idNpc, OBJID idMsg, const char* pBuf, int nMsgLen)
{
    // TODO: 请在此添加NPC服务器下传消息的代码
    if (!pBuf)
    {
        ::LogSave("Error: null msg point found in CGameSocket::SendMsg.");
        return false;
    }
    if (_MSG_NONE == idMsg)
    {
        ::LogSave("Error: invalid msg type in CGameSocket::SendMsg().");
        return false;
    }
    try
    {
        MESSAGESTR	bufPacket;
        SENDNPCMSG_PACKET0* pPacket = (SENDNPCMSG_PACKET0*)bufPacket;
        pPacket->idNpc		= idNpc;
        pPacket->idPacket	= idMsg;
        pPacket->nSize		= nMsgLen;
        memcpy(pPacket->buf, pBuf, nMsgLen);
        m_pMsgPort->Send(MSGPORT_SOCKET, SOCKET_SENDNPCMSG, STRUCT_TYPE(bufPacket), &bufPacket);
        return true;
    }
    catch(...)
    {
        ASSERT(!"catch");
        ::LogSave("Error: exception catched in CGameSocket::SendMsg().");
        return false;
    }
    return true;
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CWorldKernel::CloseSocket(SOCKET_ID idSocket)			// 直接关闭socket
{
    // TODO: 请在此添加关闭客户端SOCKET的代码
    m_pMsgPort->Send(MSGPORT_SOCKET, SOCKET_BREAKCONNECT, VARTYPE_INT, &idSocket);
    if (idSocket == m_idNpcSocket)
    {
        LOGMSG("CWorldKernel::CloseSocket break npc connect!", idSocket);
        m_idNpcSocket	= SOCKET_NONE;		//@???
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CWorldKernel::SendClientMsg(SOCKET_ID idSocket, CNetMsg* pMsg)
{
#ifdef _MYDEBUG
    ::LogMsg("SendMsg:%d, size:%d", m_unMsgType, m_unMsgSize);
#endif
    pMsg->AppendInfo(MSGPORT_WORLD, idSocket, ID_NONE);
    return SendMsg(pMsg);
}

//////////////////////////////////////////////////////////////////////
bool CWorldKernel::SendNpcMsg(OBJID idNpc, CNetMsg* pMsg)
{
#ifdef _MYDEBUG
    ::LogMsg("SendMsg:%d, size:%d", m_unMsgType, m_unMsgSize);
#endif
    pMsg->AppendInfo(MSGPORT_WORLD, SOCKET_NONE, idNpc);
    return SendNpcMsg(pMsg);
}

//////////////////////////////////////////////////////////////////////
bool CWorldKernel::PrintText			(LPCTSTR szText)
{
    return m_pMsgPort->Send(MSGPORT_SHELL, SHELL_PRINTTEXT, STRING_TYPE(szText), szText);
}

//////////////////////////////////////////////////////////////////////
bool CWorldKernel::ChangeCode(SOCKET_ID idSocket, DWORD dwData)
{
    CHANGE_USERDATA	st;
    st.idSocket		= idSocket;
    st.nData		= dwData;
    return m_pMsgPort->Send(MSGPORT_SOCKET, SOCKET_CHANGECODE, STRUCT_TYPE(CHANGE_USERDATA), &st);
}

///////////////////////////////////////////////////////////////////////////////////////
// account server
///////////////////////////////////////////////////////////////////////////////////////
bool CWorldKernel::SendAccount(LPCTSTR szAccount, LPCTSTR szPassword, LPCTSTR szServer)
{
    CMsgAccount_A::MSG_Info	msg;
    msg.unMsgSize	= sizeof(msg);
    msg.unMsgType	= _MSG_ACCOUNT_A;
    SafeCopy(msg.szAccount	, szAccount	, _MAX_NAMESIZE);
    SafeCopy(msg.szPassword	, szPassword, _MAX_NAMESIZE);
    SafeCopy(msg.szServer	, szServer	, _MAX_NAMESIZE);
    return m_sock.SendPacket((LPCTSTR)&msg, msg.unMsgSize, true);
}

///////////////////////////////////////////////////////////////////////////////////////
bool CWorldKernel::SendLogin(OBJID idAccount, DWORD dwCheckData, int usVersion, LPCTSTR szUserName)
{
    CMsgLogin_A::MSG_Info	msg;
    msg.unMsgSize	= sizeof(msg);
    msg.unMsgType	= _MSG_LOGIN_A;
    msg.idAccount	= idAccount;
    msg.dwCheckData	= dwCheckData;
    msg.usVersion	= usVersion;
    SafeCopy(msg.szUserName, szUserName);
    return m_sock.SendPacket((LPCTSTR)&msg, msg.unMsgSize, true);
}

///////////////////////////////////////////////////////////////////////////////////////
bool CWorldKernel::SendFee(OBJID idAccount, int nType)
{
    CMsgFee_A::MSG_Info	msg;
    msg.unMsgSize	= sizeof(msg);
    msg.unMsgType	= _MSG_FEE_A;
    msg.idAccount	= idAccount;
    msg.ucType		= nType;
    return m_sock.SendPacket((LPCTSTR)&msg, msg.unMsgSize, true);
}

///////////////////////////////////////////////////////////////////////////////////////
bool CWorldKernel::SendQueryFee(OBJID idAccount, DWORD dwData, int nTime, int ucType)
{
    CMsgQueryFee_A::MSG_Info	msg;
    msg.unMsgSize	= sizeof(msg);
    msg.unMsgType	= _MSG_QUERYFEE_A;
    msg.idAccount	= idAccount;
    msg.dwData		= dwData;
    msg.nTime		= nTime;
    msg.ucType		= ucType;
    return m_sock.SendPacket((LPCTSTR)&msg, msg.unMsgSize, true);
}

///////////////////////////////////////////////////////////////////////////////////////
void CWorldKernel::ProcessAccountNetMsg()
{
    bool	bGetData = true;
    while(true)
    {
        int	nNetSize = 0;
        LPCTSTR pMsg = m_sock.GetPacket(&nNetSize, bGetData);
        if (!pMsg || nNetSize == 0)
        {
            break;
        }
        int nMsgSize = *((unsigned short*)pMsg);
        if (nNetSize < nMsgSize)
        {
            break;
        }
        // 服务器登录 ////////////////////////////////////////
        CMsgLogin_A::MSG_Info*	pMsgLogin = (CMsgLogin_A::MSG_Info*)pMsg;
        if (pMsgLogin->unMsgType == _MSG_LOGIN_A)
        {
            CHECK(pMsgLogin->unMsgSize == sizeof(CMsgLogin_A::MSG_Info));
            ProcessLogin(pMsgLogin->idAccount, pMsgLogin->dwCheckData, pMsgLogin->usVersion, pMsgLogin->szUserName);
            m_sock.ClearPacket(nMsgSize);
            continue;
        }
        // 玩家部分 ////////////////////////////////////////
        CMsgConnect_A::MSG_Info*	pMsgConnect = (CMsgConnect_A::MSG_Info*)pMsg;
        if (pMsgConnect->unMsgType == _MSG_CONNECT)
        {
            CHECK(pMsgConnect->unMsgSize == sizeof(CMsgConnect_A::MSG_Info));
            if (ProcessConnect(pMsgConnect->idAccount, pMsgConnect->dwData, pMsgConnect->szInfo))
            {
                CPlayer* pPlayer	= g_UserList.GetPlayerByAccountID(pMsgConnect->idAccount);
                ASSERT(pPlayer);
                if (pPlayer)
                {
                    UserList()->LoginToMapGroup(pPlayer);
                }
            }
            m_sock.ClearPacket(nMsgSize);
            continue;
        }
        CMsgFee_A::MSG_Info*	pMsgFee = (CMsgFee_A::MSG_Info*)pMsg;
        if (pMsgFee->unMsgType == _MSG_FEE_A)
        {
            CHECK(pMsgFee->unMsgSize == sizeof(CMsgFee_A::MSG_Info));
            ProcessFee(pMsgFee->idAccount, pMsgFee->ucType);
            m_sock.ClearPacket(nMsgSize);
            continue;
        }
        CMsgQueryFee_A::MSG_Info*	pMsgQFee = (CMsgQueryFee_A::MSG_Info*)pMsg;
        if (pMsgQFee->unMsgType == _MSG_QUERYFEE_A)
        {
            CHECK(pMsgQFee->unMsgSize == sizeof(CMsgQueryFee_A::MSG_Info));
            ProcessQueryFee(pMsgQFee->idAccount, pMsgQFee->dwData, pMsgQFee->nTime, pMsgQFee->ucType);
            m_sock.ClearPacket(nMsgSize);
            continue;
        }
        bGetData = false;
    } // for
}

///////////////////////////////////////////////////////////////////////////////////////
void CWorldKernel::ProcessLogin(OBJID idAccount, DWORD dwCheckData, int usVersion, LPCTSTR szUserName)
{
    if (idAccount == ID_NONE)
    {
        PrintText("Account server login failed!");
        return;
    }
    //??? 未检查版本号
    m_idServer	= idAccount;
    PrintText("Account server login OK。");
    SendLogin(idAccount, dwCheckData, usVersion, szUserName);
    m_nState = STATE_NORMAL;
}

///////////////////////////////////////////////////////////////////////////////////////
bool CWorldKernel::ProcessConnect(OBJID idAccount, DWORD dwData, LPCTSTR szInfo)			// return true: pass barrier
{
    extern DWORD g_dwMaxPlayer;
    if (GameWorld()->QueryUserList()->GetUserAmount() > g_dwMaxPlayer && idAccount > MAX_GM_ID)
    {
        GameWorld()->SendFee(idAccount, CMsgFee_A::SERVER_FULL);		// 未上传SERVER_BUSY
        return false;
    }
    CConnectUser* pConnect = m_setConnectClient->GetObj(idAccount);
    if (pConnect)
    {
        if (pConnect->IsValid() && pConnect->CheckData(dwData))	// same id, same chkdata
        {
            SOCKET_ID	idSocket = pConnect->GetSocketID();
            m_setConnectClient->DelObj(idAccount);
            // check repeat user_id
            if (UserList()->GetPlayerByAccountID(idAccount))
            {
                LOGMSG("DEBUG：帐号[%u]重复登录，踢前一人下线 !", idAccount);
                UserList()->KickOutAccount(idAccount, STR_LOGIN_AGAIN);
                return false;
            }
            // OK
            int ret = UserList()->LoginUser(idAccount, idSocket, szInfo);		// enum { LOGIN_OK, LOGIN_NEW, LOGIN_BAN };
            if (ret == LOGIN_OK)
            {
                return true;
            }
            else if (ret == LOGIN_BAN)
            {
                CMsgTalk msg;
                if (msg.Create(SYSTEM_NAME, ALLUSERS_NAME, "错误：余额不足，请及时充值!", NULL, _COLOR_WHITE, _TXTATR_ENTRANCE))
                {
                    SendClientMsg(idSocket, &msg);
                }
                return false;
            }
            else	// LOGIN_NEW new account, no player created
            {
                CMsgTalk msg;
                if (msg.Create(SYSTEM_NAME, ALLUSERS_NAME, NEW_ROLE_STR, NULL, _COLOR_WHITE, _TXTATR_ENTRANCE))
                {
                    SendClientMsg(idSocket, &msg);
                }
            }
        }
        else	// same id, but wrong chkdata
        {
            m_setConnectClient->DelObj(idAccount);
        }
    }
    pConnect = CConnectUser::CreateNew();
    CHECKF(pConnect->Create(idAccount, dwData, szInfo));
    m_setConnectAccount->DelObj(idAccount);					// 冲掉，防止WARNING输出
    m_setConnectAccount->AddObj(pConnect);
    return false;
}

///////////////////////////////////////////////////////////////////////////////////////
bool CWorldKernel::ClientConnect(OBJID idAccount, DWORD dwData, LPCTSTR szInfo, SOCKET_ID idSocket)			// return true: pass barrier
{
    CHECKF(idSocket != SOCKET_NONE);
    CConnectUser* pConnect = m_setConnectAccount->GetObj(idAccount);
    if (pConnect)
    {
        NAMESTR	info;
        SafeCopy(info, pConnect->GetInfo(), _MAX_NAMESIZE);
        if (pConnect->IsValid() && pConnect->CheckData(dwData))	// same id, same chkdata
        {
            m_setConnectAccount->DelObj(idAccount);
            // check repeat user_id
            if (UserList()->GetPlayerByAccountID(idAccount))
            {
                LOGMSG("DEBUG：帐号[%u]重复登录，踢前一人下线!", idAccount);
                UserList()->KickOutAccount(idAccount, STR_LOGIN_AGAIN);
                return false;
            }
            // OK
            int ret = UserList()->LoginUser(idAccount, idSocket, info);		// enum { LOGIN_OK, LOGIN_NEW, LOGIN_BAN };
            if (ret == LOGIN_OK)
            {
                return true;
            }
            else if (ret == LOGIN_BAN)
            {
                CMsgTalk msg;
                if (msg.Create(SYSTEM_NAME, ALLUSERS_NAME, "错误：余额不足，请及时充值!", NULL, _COLOR_WHITE, _TXTATR_ENTRANCE))
                {
                    SendClientMsg(idSocket, &msg);
                }
                return false;
            }
            else	// LOGIN_NEW new account, no player created
            {
                CMsgTalk msg;
                if (msg.Create(SYSTEM_NAME, ALLUSERS_NAME, NEW_ROLE_STR, NULL, _COLOR_WHITE, _TXTATR_ENTRANCE))
                {
                    SendClientMsg(idSocket, &msg);
                }
            }
        }
        else
        {
            m_setConnectAccount->DelObj(idAccount);
        }
    }
    pConnect = CConnectUser::CreateNew();
    CHECKF(pConnect->Create(idAccount, dwData, szInfo, idSocket));
    m_setConnectClient->DelObj(idAccount);			// 防止WARNING
    m_setConnectClient->AddObj(pConnect);
    return false;
}

///////////////////////////////////////////////////////////////////////////////////////
void CWorldKernel::ProcessFee(OBJID idAccount, int nType)
{
    switch(nType)
    {
    case	CMsgFee_A::HEARTBEAT:
        SendFee(idAccount, CMsgFee_A::HEARTBEAT);
        break;
    case	CMsgFee_A::FEE_KICKOUT:
        LOGMSG("DEBUG：帐号服务器踢出帐号[%u]", idAccount);
        if (!UserList()->KickOutAccount(idAccount, STR_LOGIN_ANOTHER))		// 不在线
        {
            SendFee(idAccount, CMsgFee_A::FEE_OFFLINE);
        }
        break;
    default:
        break;
    }
}

///////////////////////////////////////////////////////////////////////////////////////
enum { c_typeNone = 0, c_typePoint, c_typeTime, c_typeNetBarPoint, c_typeNetBarTime, c_typeISP, c_typeFree, c_typeAll };
void CWorldKernel::ProcessQueryFee(OBJID idAccount, DWORD dwData, int nTime, int ucType)
{
    CPlayer*  pUser	= UserList()->GetPlayerByAccountID(idAccount);
    if (pUser)
    {
        char szMsg[1024] = STR_FEETYPE_KNOWN;
        switch(ucType)
        {
        case	c_typePoint:
            sprintf(szMsg, STR_FEETYPE_HOUR, dwData / 20, (dwData / 2) % 10);
            break;
        case	c_typeTime:
            sprintf(szMsg, STR_FEETYPE_MONTH, nTime / 10000, (nTime / 100) % 100, nTime % 100);
            break;
        case	c_typeNetBarPoint:
            sprintf(szMsg, STR_FEETYPE_BARHOUR, dwData);
            break;
        case	c_typeNetBarTime:
            sprintf(szMsg, STR_FEETYPE_BARMONTH, dwData);
            break;
        case	c_typeISP:
            sprintf(szMsg, STR_FEETYPE_ISP, dwData);
            break;
        case	c_typeFree:
            sprintf(szMsg, STR_FEETYPE_FREE, dwData);
            break;
        }
        CMsgTalk	msg;
        IF_OK(msg.Create(SYSTEM_NAME, pUser->szName, szMsg))
        pUser->SendMsg(&msg);
        szMsg[0]	= 0;
        if (strcmp(pUser->szNotify, FLAG_ISP_TO_NORMAL) == 0)			// ★注意：该字符串必须与帐号服务器同步
        {
            SafeCopy(szMsg, STR_LOGIN_ISP_FAIL);
        }
        else if (strcmp(pUser->szNotify, FLAG_NO_POINT) == 0)
        {
            SafeCopy(szMsg, STR_ACCOUNT_NOFEE);
        }
        else if (pUser->szNotify[0] >= '0' && pUser->szNotify[0] <= '9')
        {
            sprintf(szMsg, STR_FEW_FEE_NOTIFY, pUser->szNotify);
        }
        else
        {
            SafeCopy(szMsg, pUser->szNotify, _MAX_NAMESIZE);
        }
        if (strlen(szMsg) > 0)
        {
            CMsgTalk	msg;
            IF_OK(msg.Create(SYSTEM_NAME, pUser->szName, szMsg))
            pUser->SendMsg(&msg);
        }
    }
    else
    {
        LOGERROR("帐号服务器返回了未登录玩家的查询计费消息");
    }
}

///////////////////////////////////////////////////////////////////////////////////////
// global entry
///////////////////////////////////////////////////////////////////////////////////////
IWorld* IWorld::CreateNew()
{
    return new CWorldKernel();
}

//////////////////////////////////////////////////////////////////////
// syndicate
//////////////////////////////////////////////////////////////////////
bool CWorldKernel::CreateSyndicate		(const CreateSyndicateInfo* pInfo)
{
    for(int i = MSGPORT_MAPGROUP_FIRST; i < GameWorld()->GetMsgPort()->GetSize(); i++)
    {
        m_pMsgPort->Send(i, KERNEL_CREATESYNDICATE, STRUCT_TYPE(CreateSyndicateInfo), pInfo);
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CWorldKernel::DestroySyndicate	(OBJID idSyn, OBJID idTarget)
{
    OBJID	setID[2];
    setID[0]	= idSyn;
    setID[1]	= idTarget;
    for(int i = MSGPORT_MAPGROUP_FIRST; i < GameWorld()->GetMsgPort()->GetSize(); i++)
    {
        m_pMsgPort->Send(i, KERNEL_DESTROYSYNDICATE, STRUCT_TYPE(setID), &setID);
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CWorldKernel::CombineSyndicate(OBJID idSyn, OBJID idTarget)
{
    OBJID	setID[2];
    setID[0]	= idSyn;
    setID[1]	= idTarget;
    for(int i = MSGPORT_MAPGROUP_FIRST; i < GameWorld()->GetMsgPort()->GetSize(); i++)
    {
        m_pMsgPort->Send(i, KERNEL_COMBINESYNDICATE, STRUCT_TYPE(setID), &setID);
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CWorldKernel::ChangeSyndicate		(const SynFuncInfo0* pFuncInfo)
{
    for(int i = MSGPORT_MAPGROUP_FIRST; i < GameWorld()->GetMsgPort()->GetSize(); i++)
    {
        m_pMsgPort->Send(i, KERNEL_CHANGESYNDICATE, BUFFER_TYPE(pFuncInfo->nSize), pFuncInfo);
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
enum { CONFIGDATA_NAME = 1, CONFIGDATA_DATA, CONFIGDATA_STRING };
bool CWorldKernel::LoadConfig(IDatabase* pDb)
{
    extern long	g_sKickoutCheat;
    char	szSQL[1024];
    sprintf(szSQL, "SELECT * FROM %s", _TBL_CONFIG);
    CAutoPtr<IRecordset> pRes = pDb->CreateNewRecordset(szSQL, false);
    CHECKF(pRes);
    for(int i = 0; i < pRes->RecordCount(); i++, pRes->MoveNext())
    {
        if (stricmp(pRes->GetStr(CONFIGDATA_NAME), "kickoutcheat") == 0)
        {
            InterlockedExchange(&g_sKickoutCheat, pRes->GetInt(CONFIGDATA_DATA));
        }
    }
    return true;
}

bool CWorldKernel::SendMapGroupMsgForChatRoom(int nMapGroup, SOCKET_ID idSocket, CNetMsg* pMsg)
{
    ASSERT(idSocket != SOCKET_NONE);
    MESSAGESTR	buf;
    TRANSMSG_PACKET0* pPacket = (TRANSMSG_PACKET0*)buf;
    pPacket->idSocket	= idSocket;
    pPacket->idNpc		= ID_NONE;
    pPacket->idPacket	= pMsg->GetType();
    memcpy(pPacket->buf, pMsg->GetBuf(), pMsg->GetSize());
    pPacket->nSize		= pMsg->GetSize();
    pPacket->nTrans		= pMsg->GetTransData();
    CHECKF( nMapGroup >= MSGPORT_MAPGROUP_FIRST && nMapGroup < m_pMsgPort->GetSize());
    m_pMsgPort->Send(nMapGroup, MAPGROUP_TRANSMITMSG_FORCHATROOM, STRUCT_TYPE(buf), pPacket);
    return true;
}
