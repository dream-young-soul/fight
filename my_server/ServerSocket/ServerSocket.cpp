
//**********************************************************
// 代码编辑器
//**********************************************************

// ServerSocket.cpp: implementation of the CSocketKernel class.
//
//////////////////////////////////////////////////////////////////////

#define _WINSOCKAPI_   /* Prevent inclusion of winsock.h in windows.h */
#define FD_SETSIZE      2048				// 限制最多2000个客户端登录
#include <winsock2.h>
#include <windows.h>
#pragma	warning(disable:4786)
#include "define.h"
#include "protocol.h"
#include "EncryptServer.h"
#include "SSocket.h"
#include "I_Shell.h"
#include "ServerSocket.h"
#include "I_MessagePort.h"
#include "basefunc.h"


#define		MAX_USERSIZE	(FD_SETSIZE - 48)

//////////////////////////////////////////////////////////////////////
// interface
//////////////////////////////////////////////////////////////////////
bool CSocketKernel::Create(IMessagePort* pPort)
{
    m_idNpcSocket = SOCKET_NONE;
    m_pMsgPort = pPort;
    m_pMsgPort->Open();
    // TODO: 请在此添加初始化代码
    m_pListen = m_cListen.GetInterface();
    if (!ServerSocketInit())
    {
        return false;
    }
    if (!m_pListen->Listen(SERVER_LISTEN_PORT, SOCKET_SENDBUFSIZE))
    {
        return false;
    }
    return true;		// return false : 创建失败，程序关闭。
}

//////////////////////////////////////////////////////////////////////
bool CSocketKernel::ProcessMsg(OBJID idPacket, void* buf, int nType, int nFrom)
{
    // TODO: 请在此添加消息处理代码
    switch(idPacket)
    {
    case	SOCKET_CLOSESOCKET:
        {
            SOCKET_ID	idSocket	= *(int*)buf;
            if (idSocket >= m_setSocket.size())
            {
                break;
            }
            LOGDEBUG("DEBUG：通讯核心收到关闭网络消息，SOCKET_ID[%u]", idSocket);
            if (m_setSocket[idSocket])			// readme.txt (1-7)
            {
                m_setSocket[idSocket]->Close(true);
                m_setSocket[idSocket]		= NULL;
                CServerSocket::Destroy(m_setServerSocket[idSocket]);
                m_setProcessID[idSocket]	= PROCESS_NONE;
                m_setNetBreakFlag[idSocket]	= false;
                if (m_idNpcSocket == idSocket)
                {
                    LOGDUMP("m_idNpcSocket(%d) = SOCKET_NONE", m_idNpcSocket);
                    m_idNpcSocket = SOCKET_NONE;
                    char*	pText = "NPC server broken connect.";
                    m_pMsgPort->Send(MSGPORT_SHELL, SHELL_PRINTTEXT, STRING_TYPE(pText), pText);
                }
            }
        }
        break;
    case	SOCKET_SETPROCESSID:
        {
            const CHANGE_USERDATA*	pPack = (CHANGE_USERDATA*)buf;
            if (pPack->idSocket >= 0 && pPack->idSocket < m_setSocket.size()
                    && m_setSocket[pPack->idSocket]
                    && !IsNetBreakFlag(pPack->idSocket) )		// 网络正常
            {
                m_setProcessID[pPack->idSocket] = pPack->nData;
            }
            else
            {
                m_pMsgPort->Send(nFrom, KERNEL_CLOSEKERNEL, VARTYPE_INT, &pPack->idSocket);    // readme.txt (1-6)
            }
        }
        break;
    case	SOCKET_SENDCLIENTMSG:
        {
            const SENDCLIENTMSG_PACKET0* pPack = (SENDCLIENTMSG_PACKET0*)buf;
            if (pPack->idSocket >= m_setSocket.size())
            {
                break;
            }
            if (m_setSocket[pPack->idSocket])
            {
                char buf[MAX_PACKETSIZE];
                int	nNetPacketSize = UnitePacket(buf, MAX_PACKETSIZE, pPack->idPacket, pPack->buf, pPack->nSize);
                ASSERT(nNetPacketSize);
                if (nNetPacketSize)
                {
#ifdef	PALED_DEBUG
                    OBJID	idPacket = *((unsigned short*)buf + 1);
                    char	szText[256];
                    sprintf(szText, "VVV 发送了消息%u", idPacket);
                    m_pMsgPort->Send(MSGPORT_SHELL, SHELL_PRINTTEXT, STRING_TYPE(szText), szText);
#endif
                    clock_t	clkDebug = clock();		// debug
                    m_setSocket[pPack->idSocket]->SendPacket(buf, nNetPacketSize);
                    {
                        extern struct STAT_STRUCT	g_stat;
                        //						InterlockedExchangeAdd(&g_stat.setDebug[0], clock() - clkDebug);		// debug
                        // stat
                        InterlockedExchangeAdd(&g_stat.nSocketBytes, nNetPacketSize);
                        //InterlockedIncrement(&g_stat.nSocketPackets);
                    }
                }
            }
        }
        break;
    case	SOCKET_SENDNPCMSG:
        {
            const SENDNPCMSG_PACKET0* pPack = (SENDNPCMSG_PACKET0*)buf;
            OBJID	idNpc = pPack->idNpc;
            //			if (idNpc == BCAST_NPCID)
            //				idNpc = nFrom;						// idNpc转换为MAPGROUP_ID
            if (m_idNpcSocket != SOCKET_NONE)
            {
                char buf[MAX_PACKETSIZE];
                int	nLen = UniteNpcPacket(buf, MAX_PACKETSIZE, pPack->idPacket, pPack->buf, pPack->nSize, idNpc);
                ASSERT(nLen);
                if (nLen)
                {
#ifdef	DUMP_NPC_MSG
                    OBJID	idPacket = *((unsigned short*)buf + 1);
                    char	szText[256];
                    sprintf(szText, "VVV 发送了NPC消息%u", idPacket);
                    m_pMsgPort->Send(MSGPORT_SHELL, SHELL_PRINTTEXT, STRING_TYPE(szText), szText);
#endif
                    clock_t	clkDebug = clock();		// debug
                    m_setSocket[m_idNpcSocket]->SendPacket(buf, nLen);
                    {
                        extern struct STAT_STRUCT	g_stat;
                        //						InterlockedExchangeAdd(&g_stat.setDebug[0], clock() - clkDebug);		// debug
                        // stat
                        InterlockedExchangeAdd(&g_stat.nNpcSocketBytes, nLen);
                        //InterlockedIncrement(&g_stat.nNpcSocketPackets);
                    }
                }
                break;		// break for
            }
        }
        break;
    case	SOCKET_SETNPCSOCKETID:
        {
            SOCKET_ID	idSocket = *(SOCKET_ID*)buf;
            // repeat set npc socket id
            if (idSocket != m_idNpcSocket && m_idNpcSocket != SOCKET_NONE && m_setSocket[m_idNpcSocket])
            {
                LOGDUMP("m_idNpcSocket(%d)->Close()", m_idNpcSocket);
                m_setSocket[m_idNpcSocket]->Close();		// 断开SOCKET，其余事件由SOCKET中断引发。可能有问题???
                LOGWARNING("NPC服务器重复登录，前一个SOCKET已断开!");
            }
            LOGDUMP("m_idNpcSocket(%d) = idSocket(%d)", m_idNpcSocket, idSocket);
            m_idNpcSocket = idSocket;
            LOGMSG("NPC服务器的SOCKET索引号为%u。", idSocket);
            if (m_idNpcSocket >= 0 && m_idNpcSocket < m_setSocket.size() && m_setSocket[m_idNpcSocket])
            {
                m_setSocket[m_idNpcSocket]->SetBufSize(SOCKET_NPCBUFSIZE, SOCKET_NPCBUFSIZE);
            }
        }
        break;
    case	SOCKET_SETNPCPROCESSID:
        {
            const CHANGE_NPCPROCESSID*	pPack = (CHANGE_NPCPROCESSID*)buf;
            if (pPack->idProcess == PROCESS_NONE)
            {
                m_setNpcProcessID.erase(pPack->idNpc);
            }
            else
            {
                m_setNpcProcessID[pPack->idNpc] = pPack->idProcess;
            }
        }
        break;
    case	MAPGROUP_DELALLMONSTER:
        {
            m_setNpcProcessID.clear();
            LOGDUMP("Del all monster in socket thread.");
        }
        break;
    case	SOCKET_CHANGECODE:
        {
            const CHANGE_USERDATA*	pPack = (CHANGE_USERDATA*)buf;
            SOCKET_ID idSocket	= pPack->idSocket;
            DWORD	dwData		= pPack->nData;
            CHECKF(idSocket >= 0 && idSocket < m_setSocket.size());
            m_setSocket[idSocket]->ChangeCode(dwData);
        }
        break;
    case	SOCKET_BREAKCONNECT:
        {
            SOCKET_ID	idSocket = *(SOCKET_ID*)buf;
            CHECKF(idSocket >= 0 && idSocket < m_setSocket.size());
            LOGDEBUG("DEBUG：网络核心应核心要求关闭SOCKET，SOCKET_ID[%d]", idSocket);
            if (m_setSocket[idSocket] && !IsNetBreakFlag(idSocket))
            {
                m_setSocket[idSocket]->Close(true);    // call SendCloseNotify() in Close()
            }
            else
            {
                SendCloseNotify(idSocket);
            }
        }
        break;
    case	SOCKET_BREAKALLCONNECT:
        {
            SOCKET_ID	idGmSocket = *(SOCKET_ID*)buf;
            //			CHECKF(idGmSocket>=0 && idGmSocket < m_setSocket.size());
            LOGDEBUG("DEBUG：网络核心应GM要求关闭所有SOCKET，SOCKET_ID[%d]", idGmSocket);
            extern long g_nServerClosed;
            g_nServerClosed = true;
            for(int i = m_setSocket.size() - 1; i >= 0; i--)
            {
                if (i != idGmSocket && i != m_idNpcSocket && m_setSocket[i])
                {
                    LOGDUMP("m_setSocket[%d]->Close(true);", i);
                    m_setSocket[i]->Close(true);
                }
            }
        }
        break;
    default:
        LOGERROR("CSocketKernel::ProcessMsg()");
        return false;
    }
    return true;		// return false : 消息处理异常，程序关闭。
}

//////////////////////////////////////////////////////////////////////
bool CSocketKernel::ProcessSocket()
{
    // TODO: 请在此添加SOCKET处理代码
    fd_set	readmask;
    FD_ZERO(&readmask);
    FD_SET(m_pListen->Socket(), &readmask);
    for(int i = 0; i < m_setSocket.size(); i++)
    {
        if (!m_setSocket[i])
        {
            continue;
        }
        FD_SET(m_setSocket[i]->Socket(), &readmask);
    }
    int	nBlockMilli = 0;
    struct timeval	timeout;
    timeout.tv_sec	= (nBlockMilli * 1000) / 1000000;
    timeout.tv_usec = (nBlockMilli * 1000) % 1000000;
    int ret = select(FD_SETSIZE, &readmask, (fd_set*) 0, (fd_set*) 0, &timeout);
    // 处理NEW_SOCKET
    if (FD_ISSET(m_pListen->Socket(), &readmask))
    {
        AcceptNewSocket();
    }
    // 处理USER_SOCKET
    for(int i = m_setSocket.size() - 1; i >= 0; i--)
    {
        if (!m_setSocket[i])
        {
            continue;
        }
        int		nBufLen = 0;
        BOOL	bHaveNetData = FD_ISSET(m_setSocket[i]->Socket(), &readmask);
        const char* pBuf = m_setSocket[i]->GetPacket(&nBufLen, bHaveNetData);
        // 处理SOCKET关闭
        if (!m_setSocket[i]->IsOpen())		// bHaveNetData &&
        {
            if (!IsNetBreakFlag(i))
            {
                SendCloseNotify(i);
                SetNetBreakFlag(i);
                if (m_idNpcSocket == i)
                {
                    LOGDUMP("m_setSocket(%d) = SOCKET_NONE", m_idNpcSocket);
                    m_idNpcSocket = SOCKET_NONE;
                    char*	pText = "NPC server broken connect!";
                    m_pMsgPort->Send(MSGPORT_SHELL, SHELL_PRINTTEXT, STRING_TYPE(pText), pText);
                }
            }
            pBuf	= NULL;
            nBufLen = 0;		// 阻止继续处理消息
        }
        if (pBuf && nBufLen > 0)
        {
#ifdef	DUMP_DEBUG_MSG
            OBJID	idPacket = *(((short*)pBuf) + 1);
            if (idPacket == 1002)
            {
                LOGDUMP("LOGIN pocket(%u), idSocket(%d), m_idNpcSocket(%d)", idPacket, i, m_idNpcSocket);
            }
#endif
            if (i != m_idNpcSocket)		// 客户端消息
            {
                OBJID	idPacket;
                char*	pMsg;
                int		nMsgSize;
                int		nNetPacketSize;
                if (( nNetPacketSize = SplitPacket(pBuf, nBufLen, &idPacket, &pMsg, &nMsgSize) ))
                {
                    char	bufPacket[MAX_MESSAGESIZE];
                    CLIENTMSG_PACKET0* pPacket = (CLIENTMSG_PACKET0*)bufPacket;
                    pPacket->idSocket	= i;
                    pPacket->idPacket	= idPacket;
                    pPacket->nSize		= nMsgSize;
                    memcpy(pPacket->buf, pMsg, nMsgSize);
                    m_pMsgPort->Send(m_setProcessID[i], KERNEL_CLIENTMSG, STRUCT_TYPE(bufPacket), bufPacket);
                    /*
                    // for encrypt socket receive buf
                    if (1012 == idPacket)
                    {
                    	typedef struct {
                    		OBJID			idPlayer;
                    		DWORD			dwData;
                    	}MsgInfo;

                    	MsgInfo* pInfo = (MsgInfo*)pMsg;
                    	m_setSocket[i]->ChangeCode(pInfo->dwData);
                    	::LogSave("the code is :%u", pInfo->dwData);
                    }
                    */
#ifdef	_DEBUGx
                    char	szText[256];
                    sprintf(szText, "AAA 接收了消息%u", idPacket);
                    m_pMsgPort->Send(MSGPORT_SHELL, SHELL_PRINTTEXT, STRING_TYPE(szText), szText);
#endif
                    m_setSocket[i]->ClearPacket(nNetPacketSize);
                    // stat
                    extern struct STAT_STRUCT	g_stat;
                    InterlockedIncrement(&g_stat.nSocketPackets);
                }
            }
            else // NPC服务器消息
            {
                do
                {
                    OBJID	idNpc;
                    OBJID	idPacket;
                    char*	pMsg;
                    int		nMsgSize;
                    if (SplitNpcPacket(pBuf, nBufLen, &idPacket, &pMsg, &nMsgSize, &idNpc))
                    {
                        PROCESS_ID	idProcess = MSGPORT_WORLD;
                        NPCPROCESSID_MAP::iterator iter = m_setNpcProcessID.find(idNpc);
                        if (iter != m_setNpcProcessID.end())
                        {
                            idProcess = iter->second;
                        }
#ifdef	DUMP_DEBUG_MSG
                        if (idPacket == 1002)
                        {
                            LOGDUMP("LOGIN AGENT %u in process id %d", idNpc, idProcess);
                        }
#endif
                        char	bufPacket[MAX_MESSAGESIZE];
                        NPCMSG_PACKET0* pPacket = (NPCMSG_PACKET0*)bufPacket;
                        pPacket->idNpc		= idNpc;
                        pPacket->idPacket	= idPacket;
                        pPacket->nSize		= nMsgSize;
                        memcpy(pPacket->buf, pMsg, nMsgSize);
                        m_pMsgPort->Send(idProcess, KERNEL_NPCMSG, STRUCT_TYPE(bufPacket), bufPacket);
                        int	nCurrMsgSize = PROTOCOL_NPCMSG_HEADSIZE + nMsgSize;
#ifdef	DUMP_NPC_MSG
                        char	szText[256];
                        sprintf(szText, "AAA 接收了NPC消息%u", idPacket);
                        m_pMsgPort->Send(MSGPORT_SHELL, SHELL_PRINTTEXT, STRING_TYPE(szText), szText);
#endif
                        IF_NOT(m_setSocket[i]->ClearPacket(nCurrMsgSize))
                        break;
                        // stat
                        extern struct STAT_STRUCT	g_stat;
                        InterlockedIncrement(&g_stat.nNpcSocketPackets);
                    }
                    else
                    {
                        break;
                    }
                    bHaveNetData = false;		// 不从SOCKET读数据
                    pBuf = m_setSocket[i]->GetPacket(&nBufLen, bHaveNetData);
                }
                while(pBuf && nBufLen > 0);
            }
        } // if have msg
    } // for every socket
    return true;		// return false : 消息处理异常，程序关闭。
}

//////////////////////////////////////////////////////////////////////
bool CSocketKernel::OnTimer(time_t tCurr)
{
    // TODO: 请在此添加时钟处理代码
    return true;		// return false : 消息处理异常，程序关闭。
}

//////////////////////////////////////////////////////////////////////
bool CSocketKernel::Release()
{
    // TODO: 请在此添加代码
    m_pListen->Close();
    for(int i = m_setSocket.size() - 1; i >= 0; i--)
    {
        if (m_setSocket[i])
        {
            m_setSocket[i]->Close();
            CServerSocket::Destroy(m_setServerSocket[i]);
        }
    }
    ServerSocketFinal();
    m_pMsgPort->Close();
    delete this;
    return true;		// return false : 无意义。
}

//////////////////////////////////////////////////////////////////////
// inside
//////////////////////////////////////////////////////////////////////
void CSocketKernel::AcceptNewSocket()
{
    SOCKET socket = m_pListen->Accept();
    extern long g_nServerClosed;
    if (g_nServerClosed || m_setSocket.size() >= MAX_USERSIZE)
    {
        return ;
    }
    if (socket != INVALID_SOCKET)
    {
        CServerSocket* pSocket = CServerSocket::CreateNew(socket);
        if (pSocket)
        {
            BOOL	bInsertOk = false;
            for(int i = 0; i < m_setSocket.size(); i++)
            {
                if (!m_setSocket[i])
                {
                    m_setServerSocket[i]	= pSocket;
                    m_setSocket[i]			= pSocket->GetInterface();
                    m_setProcessID[i]		= MSGPORT_WORLD;		// 初始化时指向世界核心
                    m_setNetBreakFlag[i]	= false;
                    bInsertOk	= true;
                    break;
                }
            }
            if (!bInsertOk)
            {
                m_setServerSocket.push_back(pSocket);
                m_setSocket.push_back(pSocket->GetInterface());
                m_setProcessID.push_back(MSGPORT_WORLD);		// 初始化时指向世界核心
                m_setNetBreakFlag.push_back(false);
                if (m_setSocket.size() >= MAX_USERSIZE)
                {
                    LOGERROR("玩家SOCKET数量已达到[%d]★", MAX_USERSIZE);
                }
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////
bool CSocketKernel::SendCloseNotify(SOCKET_ID idSocket)
{
    LOGDEBUG("DEBUG：网络核心SOCKET关闭，踢出SOCKET_ID[%d]", idSocket);
#ifdef	NEW_LOGOUT
    m_pMsgPort->Send(m_pMsgPort->GetSize() - 1, KERNEL_CLOSEKERNEL, VARTYPE_INT, &idSocket);			// readme.txt (1-1)
#else
    if (m_setProcessID[idSocket] == MSGPORT_WORLD)
    {
        m_pMsgPort->Send(MSGPORT_WORLD, KERNEL_CLOSEKERNEL, VARTYPE_INT, &idSocket);    // readme.txt (1-1)
    }
    else
    {
        for(int j = MSGPORT_MAPGROUP_FIRST; j < m_pMsgPort->GetSize(); j++)			// readme.txt (1-2)
        {
            m_pMsgPort->Send(j, KERNEL_CLOSEKERNEL, VARTYPE_INT, &idSocket);
        }
    }
#endif
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////
// global entry
///////////////////////////////////////////////////////////////////////////////////////
ISocketKernel* ISocketKernel::CreateNew()
{
    return new CSocketKernel();
}

