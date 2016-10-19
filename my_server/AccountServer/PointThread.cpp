
//**********************************************************
// 代码编辑器
//**********************************************************

// PointThread.cpp: 计点线程类
// 仙剑修：2001.11.20
//////////////////////////////////////////////////////////////////////

#include "AllHeads.h"
#include "PointThread.h"
#include "MsgConnect.h"
#include "MsgConnect91U.h"
#include "MsgAccount.h"
#include "MsgAccount91U.h"
#include "MsgLogin.h"
#include "MsgFee.h"
#include "MsgQueryFee.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPointThread::CPointThread(u_short nPort, int nSndBuf /*= 0*/)
    : CThreadBase(), m_cListenSocket(nPort, nSndBuf)
{
    for(int i = 0; i < MAXGAMESERVERS; i++)
    {
        Clear(i, false);
    }

    // 读入帐号表
    GetServerAccount();		// 预读
    m_pBanIPs = new CBanIP[MAXBANIPS];
}

CPointThread::~CPointThread()
{
    delete m_pBanIPs;
}

/////////////////////////////////////////////////////////////////////////////
// 共享函数，注意互斥

//#define	LOCK	{LOCKTHREAD;
//#define	UNLOCK	}

// ★使用共享成员变量，必须先锁定。调用外部函数，必须先解锁。
void	CPointThread::OnInit()
{
    LOCKTHREAD;
    try
    {

        m_cRc5.Rc5InitKey(RC5PASSWORD_KEY);

        m_cListenSocket.Open();
        LOGMSG("计点线程正常启动");
    }
    catch(...) { LOGCATCH("计点线程初始化时异常退出");}
}

bool	CPointThread::OnProcess()
{
    LOCKTHREAD;

    try
    {
        time_t	tStart = clock();
        // select
        fd_set	readmask;
        FD_ZERO(&readmask);
        if (!m_cListenSocket.IsOpen())
        {
            m_cListenSocket.Rebuild();
        }
        if (m_cListenSocket.IsOpen())
        {
            FD_SET(m_cListenSocket.Socket(), &readmask);
        }
        for(int i = 1; i < MAXGAMESERVERS; i++)	// 1: 从1开始。
        {
            if (m_aServerSocket[i].IsOpen())
            {
                FD_SET(m_aServerSocket[i].Socket(), &readmask);
            }
        }

        struct timeval	timeout = {0, 0};
        int ret = select(FD_SETSIZE, &readmask, (fd_set*) 0, (fd_set*) 0, &timeout);

        // 检查是否接收到新连接
        if (ret == -1)	// error
        {
            m_cListenSocket.Close();
            LOGERROR("计点线程 select 错误[%d]", WSAGetLastError());
            PrintText("计点线程出错，SOCKET被关闭。%d 秒后将会自动重建", REBUILDLISTENDELAYSEC);
        }
        else if (ret > 0 && FD_ISSET(m_cListenSocket.Socket(), &readmask))
        {
            FD_CLR(m_cListenSocket.Socket(), &readmask);
            ret--;
            u_long	nClientIP;
            SOCKET	sockNew = m_cListenSocket.Accept(nClientIP);
            if (sockNew != INVALID_SOCKET)
            {
                // 找空位置
                bool bSuccess = false;
                int i = 1;
                for(; i < MAXGAMESERVERS; i++)	// 1: 从1开始。
                {
                    if (!m_aServerSocket[i].IsOpen())
                    {
                        bSuccess = true;
                        break;	// 成功
                    }
                }

                // 添加连接
                struct in_addr	in;
                in.S_un.S_addr = nClientIP;
                char* pAddr = inet_ntoa(in);
                if (bSuccess)
                {
                    m_aServerSocket[i].Open(sockNew, nClientIP);
                    m_aHeartbeat[i] = clock();
                    LOGMSG("一个新游戏服务器连接进来，服务器ID号分配为[%d], IP地址为[%s]", i, pAddr);
                    ::SetServerState(i, c_flagSocket);
                }
                else
                {
                    LOGWARNING("已接收到一个新连接[%s]，但连接表已满。请修改MAXGAMESERVERS，并重新编译程序。"
                               , pAddr);
                    //	修改SOCKET属性成立即关闭型
                    struct linger ling;
                    ling.l_onoff = 1;
                    ling.l_linger = 0;
                    setsockopt(sockNew, SOL_SOCKET, SO_LINGER, (char*)&ling, sizeof(ling));
                    closesocket(sockNew);
                    InterlockedDecrement(&s_nSocketCount);
                }
            }
        }

        // 接收到登录消息包。需要优化??
        for(int i = 1; i < MAXGAMESERVERS; i++)	// 1: 从1开始。
        {
            if (ret <= 0)
            {
                break;    // 没连接有数据了
            }

            if (m_aServerSocket[i].IsOpen())
            {
                if (FD_ISSET(m_aServerSocket[i].Socket(), &readmask))
                {
                    FD_CLR(m_aServerSocket[i].Socket(), &readmask);
                    ret--;
                    char	bufMsg[_MAX_MSGSIZE];
                    int		nMsgLen = 0;
                    if (m_aServerSocket[i].Recv(bufMsg, nMsgLen))
                    {
                        if (nMsgLen)
                        {
                            m_aHeartbeat[i] = clock();
                            m_aHeartbeatLast[i] = 0;
                            ProcessMsg(i, bufMsg, nMsgLen);
                        }
					}
                    else	// 错误或关闭
                    {
                        LOGWARNING("Recv错，服务器[%s]关闭。", m_aServerName[i]);
                        LOGSERVER("掉线，服务器[%s]关闭。", m_aServerName[i]);
                        PrintText("服务器“%s”关闭。", m_aServerName[i]);
                        Clear(i);
                    }
                }
            }
        }
        long	nUsed = clock() - tStart;
        InterlockedExchangeAdd(&s_nAllTimeSum, nUsed);
        InterlockedIncrement(&s_nPointCount);

        long	nRemain = POINTLOOPDELAY - nUsed;
        if (nRemain < 0 || nRemain > POINTLOOPDELAY)
        {
            nRemain = 0;
        }
        if (nRemain)		//? 不切换到登录线程。计费线程优先
        {
            Sleep(nRemain);
        }

        return true;

    }
    catch(...)
    {
        LOGCATCH("计点线程主循环异常，程序坚持运行中");
        PrintText("计点线程主循环出错，程序坚持运行中...");
        return true;
    }
}

void	CPointThread::OnDestroy()
{
    LOCKTHREAD;

//	LOCK	// VVVVVVVVV
    try
    {
        m_cListenSocket.Close();
        for(int i = 1; i < MAXGAMESERVERS; i++)	// 1: 从1开始。
        {
            if (m_aServerSocket[i].IsOpen())
            {
                m_aServerSocket[i].Close();
            }
        }
        LOGMSG("计点线程正常退出");

    }
    catch(...) { LOGCATCH("计点线程结束时异常退出"); }

//	UNLOCK	// AAAAAAAAA
}

// ★共享函数，需要互斥。由LOGIN线程调用，最好改为消息传递模式??
// return ERR_NONE: OK
int	CPointThread::NewLogin(OBJID idAccount, DWORD nAuthenID, const char* szClientIP, const char* pGameServer)
{
    LOCKTHREAD;

    CMsgConnect	cMsg;
    ASSERT(IPSTRSIZE == _MAX_NAMESIZE);
    cMsg.Create(idAccount, nAuthenID, (char*)szClientIP);
    OBJID		nIndex = GetServerIndex_0(pGameServer);		// 错误返回NULL
    if (nIndex && m_aServerSocket[nIndex].IsOpen())
    {
        if (m_aState[nIndex] == STATE_NORMAL || idAccount <= MAX_GM_ID)
        {
            m_aServerSocket[nIndex].Send(cMsg.GetBuf(), cMsg.GetSize());	// 连接失败时忽略错误，下次会删除游戏服务器名
//			LOGACCOUNT("发送给[%s]玩家[%d]的登录消息。认证ID[%08X]，客户端IP[%s]",
//						m_aServerName[nIndex], idAccount, nAuthenID, szClientIP);
            return ERR_NONE;
        }
        else if (m_aState[nIndex] == STATE_BUSY)
        {
            return ERR_BUSY;
        }
        else if (m_aState[nIndex] == STATE_FULL)
        {
            return ERR_FULL;
        }
    }

    return ERR_NO_SERVER;	// 错误的服务器名或服务器未启动。
}

bool	CPointThread::GetServerIP(char* bufIP, const char* pServerName)
{
    LOCKTHREAD;

    int i = 1;
    for(; i < MAXGAMESERVERS; i++)	// 保留0，0为错误
    {
        if (strcmp(m_aServerName[i], pServerName) == 0)
        {
            break;
        }
    }

    if (bufIP && i < MAXGAMESERVERS)
    {
        if (m_aServerSocket[i].IsOpen())
        {
            strcpy(bufIP, m_aServerSocket[i].GetPeerIP());
            return true;
        }
        else
        {
            if (m_aServerName[i][0])
            {
                LOGWARNING("取IP时，游戏服务器[%s]已经关闭", m_aServerName[i]);
                LOGSERVER("服务器[%s]已经关闭了", m_aServerName[i]);
                PrintText("服务器“%s”已经关闭了", m_aServerName[i]);
            }
            Clear(i);
        }
    }

    bufIP[0] = 0;
    return false;
}

int	CPointThread::GetServerState(LPCTSTR szServerName)
{
    LOCKTHREAD;

    int	nIndex = GetServerIndex_0(szServerName);
    if (nIndex)
    {
        return m_aState[nIndex];
    }
    else
    {
        return STATE_OFFLINE;
    }
}

int	CPointThread::GetServerCount()
{
    LOCKTHREAD;

    bool	nCount = 0;
    for(int i = 1; i < MAXGAMESERVERS; i++)	// 1: 从1开始。
    {
        if (m_aState[i] != STATE_OFFLINE)
        {
            nCount++;
        }
    }

    return nCount;
}

bool CPointThread::CheckHeartbeatAll()
{
    LOCKTHREAD;

    bool	ret = false;
    for(int i = 1; i < MAXGAMESERVERS; i++)	// 1: 从1开始。
    {
        if (m_aServerSocket[i].IsOpen())
        {
            CheckHeartbeat(i);
            ret = true;
        }
    }
    return ret;
}

/*bool	CPointThread::GetServerName(OBJID idServer, char bufName[SERVERNAMESIZE])
{
	LOCKTHREAD;

	ASSERT(bufName);

	bufName[0] = 0;
	if (!idServer)
		return false;

	if (m_aServerSocket[idServer].IsOpen())
	{
		SafeCopy(bufName, m_aServerName[idServer], _MAX_NAMESIZE);
		return true;
	}
	else
	{
		if (m_aServerName[idServer][0])
		{
			LOGWARNING("取名时，游戏服务器[%s]已经关闭", m_aServerName[idServer]);
			LOGSERVER("服务器[%s]已经关闭。", m_aServerName[idServer]);
			PrintText("服务器“%s”已经关闭。", m_aServerName[idServer]);
		}
		Clear(idServer);		// SOCKET关闭后，删除游戏服务器名。被动同步
		return false;
	}
}*/

void	CPointThread::LogCount()
{
    LOCKTHREAD;

    char	bufTime[20];
    DateTime(bufTime, time(NULL));
    char	bufLine[4096] = "";			//?
    int		nAmount = g_pOnlineTable->GetPlayerCount();
    for(int i = 1; i < g_nServerAccount; i++)	// 1: 从1开始。
    {
        int	nCount = g_pOnlineTable->GetPlayerCount(g_aServerAccount[i].m_szServerName);
        int	nState = g_pPointThread->GetServerState(g_aServerAccount[i].m_szServerName);
        if (nState == CPointThread::STATE_OFFLINE)
        {
            nAmount	-= nCount;
            nCount	= 0;
        }
        char	buf[256];
        sprintf(buf, " | %12d", nCount);
        strcat(bufLine, buf);
    }
    strcat(bufLine, "\n");

    char	buf[4096];
    sprintf(buf, "%s | %12d", bufTime, nAmount);
    strcat(buf, bufLine);
    log_SaveFile("count", buf, g_szCountHeadLine);		//? LOGFILE中有自己的互斥量，双重互斥效率较低
}

void	CPointThread::LogSerialCount()
{
    LOCKTHREAD;

    char	bufTime[20];
    DateTime(bufTime, time(NULL));
    char	bufLine[4096] = "";			//?
    int		nAmount = 0;
    int		nCount = 0;
    for (COnlineTable::SERIAL_INDEX::Iter_t iter = g_pOnlineTable->GetSerialSet()->Begin();
            iter != g_pOnlineTable->GetSerialSet()->End();
            iter++, nCount++)
    {
        char	buf[256];
        sprintf(buf, " | %4d:%4d", iter->first, iter->second);
        strcat(bufLine, buf);
        nAmount	+= iter->second;
        if (strlen(bufLine) > 4000)
        {
            break;
        }
        if (nCount > 50000)
        {
            LOGERROR("★LogSerialCount()卡死★");
            break;
        }
    }
    strcat(bufLine, "\n");

    char	buf[4196];
    sprintf(buf, "%s | %8d", bufTime, nAmount);
    strcat(buf, bufLine);
    log_SaveFile("serial", buf, "时间                |   合计   | 序列号:人数\n");		//? LOGFILE中有自己的互斥量，双重互斥效率较低
}

void	CPointThread::LogServer()
{
    LOCKTHREAD;

    char	bufTime[20];
    DateTime(bufTime, time(NULL));
    char	bufLine[4096];			//?
    sprintf(bufLine, "%s | %12d", bufTime, GetServerCount());
    for(int i = 1; i < g_nServerAccount; i++)	// 1: 从1开始。
    {
        BOOL	bState = GetServerState(g_aServerAccount[i].m_szServerName);
        char	buf[256];
        if (bState != STATE_OFFLINE)
        {
            sprintf(buf, " | %12s", "√");
        }
        else
        {
            sprintf(buf, " | %12s", "╳");
        }
        strcat(bufLine, buf);
    }
    strcat(bufLine, "\n");

    log_SaveFile("server", bufLine, g_szCountHeadLine);		//? LOGFILE中有自己的互斥量，双重互斥效率较低
}

bool	CPointThread::Kickout(OBJID idAccount)
{
    LOCKTHREAD;

    char	bufServer[256];
    if (g_pOnlineTable->GetServerName(idAccount, bufServer))
    {
        return Kickout_0(idAccount, bufServer);
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////
// ★非共享函数，由子线程独占调用。不能调用外部函数。

bool	CPointThread::Kickout_0(OBJID idAccount, LPCTSTR szServerName)
{
    int nIndex = GetServerIndex_0(szServerName);

    if (nIndex != 0 )
    {
        CMsgFee	cMsg;
        cMsg.Create(idAccount, CMsgFee::FEE_KICKOUT);
        m_aServerSocket[nIndex].Send(cMsg.GetBuf(), cMsg.GetSize());
        return true;
    }


    return false;
}

bool	CPointThread::ProcessMsg(int nServerIndex,  char* pBuf, int nLen)
{
    unsigned short unMsgType = CNetMsg::GetType(pBuf, nLen);
    switch(unMsgType)
    {
    case _MSG_ACCOUNT:
        {
            CMsgAccount	cMsg;
            cMsg.Create(pBuf, nLen);
            FormatStr(cMsg.m_pInfo->szAccount, _MAX_NAMESIZE);		// 添尾0，删尾空
            FormatStr(cMsg.m_pInfo->szPassword, _MAX_NAMESIZE);		// 添尾0，删尾空
            FormatStr(cMsg.m_pInfo->szServer, _MAX_NAMESIZE);		// 添尾0，删尾空
            LOGMSG("接收到[%s]游戏服务器ACCOUNT消息。帐号[%s],口令[***]。"
                   , cMsg.m_pInfo->szServer
                   , cMsg.m_pInfo->szAccount
                   //, cMsg.m_pInfo->szPassword
                  );

            // 消息检查
            if (strlen(cMsg.m_pInfo->szAccount) == 0
                    || strlen(cMsg.m_pInfo->szPassword) == 0
                    || strlen(cMsg.m_pInfo->szServer) == 0)
            {
                LOGERROR("错误的登录消息[%s][%s][%s]",
                         cMsg.m_pInfo->szAccount, cMsg.m_pInfo->szPassword, cMsg.m_pInfo->szServer);
                return false;
            }

            GetServerAccount();		// 即时读入

            // 检查帐号
            int		nAccount = 0;		// 在帐号表中的序号，非游戏服务器ID号。
            for(int i = 1; i < g_nServerAccount; i++)	// 1: 从1开始。
            {
                if (!g_aServerAccount[i].m_b91U
                        && strcmp(g_aServerAccount[i].m_szServerName, cMsg.m_pInfo->szServer) == 0)		// 以服务器名匹配为准
                {
                    nAccount = i;
                    break;
                }
            }

            CMsgLogin	cRetMsg;
            if (nAccount					// 0：无此帐号
                    && strcmp(g_aServerAccount[nAccount].m_szLoginName, cMsg.m_pInfo->szAccount) == 0
                    && strcmp(g_aServerAccount[nAccount].m_szPassword, cMsg.m_pInfo->szPassword) == 0)	// 找到
            {
                for(int i = 1; i < MAXGAMESERVERS; i++)
                {
                    if (strcmp(g_aServerAccount[nAccount].m_szServerName, m_aServerName[i]) == 0)
                    {
                        // 重复登录
                        if (m_aServerSocket[i].IsOpen())
                        {
                            LOGMSG("游戏服务器[%s]重复ACCOUNT登录，前一个连接[%d]已断开。", m_aServerName[i], i);
                            m_aServerSocket[i].Close();		// 关闭前一个登录的连接
                            Clear(i);
                        }
                        m_aServerName[i][0] = 0;		// 删除SERVER名字
                        //表小，可全搜一遍	break;
                    }
                }

                // 保存游戏服务器域名/IP
                strcpy(m_aServerName[nServerIndex], g_aServerAccount[nAccount].m_szServerName);	// cMsg.m_pInfo->szServer);
                //?? BUG：提前允许登录了，应该接收到_MSG_LOGIN后开始允许登录
                PrintText("第%d号游戏服务器“%s”已登录", nServerIndex, m_aServerName[nServerIndex]);
                LOGSERVER("第%d号游戏服务器“%s”已登录", nServerIndex, m_aServerName[nServerIndex]);
                //?? BUG：提前允许登录了，应该接收到_MSG_LOGIN后开始允许登录
                cRetMsg.Create(nServerIndex, 0, "", ACCOUNTVERSION);
                m_aServerSocket[nServerIndex].Send(cRetMsg.GetBuf(), cRetMsg.GetSize());
                LOGMSG("新的游戏服务器[%s]ACCOUNT帐号认可。返回服务器ID[%d],游戏版本[%04X]",
                       m_aServerName[nServerIndex], nServerIndex, ACCOUNTVERSION);
                LOGMSG("新的游戏服务器[%s]ACCOUNT登录中......", m_aServerName[nServerIndex]);
                ::SetServerState(nServerIndex, c_flagAccount);

                // 添加索引
//				m_idxID.Add(nServerIndex, nAccount);
//				g_aServerAccount[nAccount].m_nIndex = nServerIndex;
            }
            else
            {
                cRetMsg.Create(0, 0, "", ACCOUNTVERSION);
                m_aServerSocket[nServerIndex].Send(cRetMsg.GetBuf(), cRetMsg.GetSize());
                LOGMSG("新的游戏服务器[%s]登录失败", cMsg.m_pInfo->szServer);
            }
        }
        break;
    case _MSG_ACCOUNT91U:
        {
            CMsgAccount91U	cMsg;
            cMsg.Create(pBuf, nLen);
            FormatStr(cMsg.m_pInfo->szAccount, _MAX_NAMESIZE);		// 添尾0，删尾空
            FormatStr(cMsg.m_pInfo->szPassword, _MAX_NAMESIZE);		// 添尾0，删尾空
            FormatStr(cMsg.m_pInfo->szServer, _MAX_NAMESIZE);		// 添尾0，删尾空
            FormatStr(cMsg.m_pInfo->szIPAddr, _MAX_NAMESIZE);		// 添尾0，删尾空

            if (cMsg.m_pInfo->id91U == 0)	// 91U服务器登录
            {
                LOGMSG("接收到[%s]91U服务器ACCOUNT消息。帐号[%s],口令[***]。"
                       , cMsg.m_pInfo->szServer
                       , cMsg.m_pInfo->szAccount
                       //, cMsg.m_pInfo->szPassword
                      );

                // 消息检查
                if (strlen(cMsg.m_pInfo->szAccount) == 0
                        || strlen(cMsg.m_pInfo->szPassword) == 0
                        || strlen(cMsg.m_pInfo->szServer) == 0)
                {
                    LOGERROR("错误的91U服务器登录消息[%s][%s][%s]",
                             cMsg.m_pInfo->szAccount, cMsg.m_pInfo->szPassword, cMsg.m_pInfo->szServer);
                    return false;
                }

                GetServerAccount();		// 即时读入

                // 检查帐号
                int		nAccount = 0;		// 在帐号表中的序号，非游戏服务器ID号。无0号帐号
                for(int i = 1; i < g_nServerAccount; i++)	// 1: 从1开始。
                {
                    if (g_aServerAccount[i].m_b91U
                            && strcmp(g_aServerAccount[i].m_szServerName, cMsg.m_pInfo->szServer) == 0)		// 以服务器名匹配为准
                    {
                        nAccount = i;
                        break;
                    }
                }

                CMsgLogin	cRetMsg;
                if (nAccount					// 0：无此帐号
                        && strcmp(g_aServerAccount[nAccount].m_szLoginName, cMsg.m_pInfo->szAccount) == 0
                        && strcmp(g_aServerAccount[nAccount].m_szPassword, cMsg.m_pInfo->szPassword) == 0)	// 找到
                {
                    for(int i = 1; i < MAXGAMESERVERS; i++)
                    {
                        if (strcmp(g_aServerAccount[nAccount].m_szServerName, m_aServerName[i]) == 0)
                        {
                            // 重复登录
                            if (m_aServerSocket[i].IsOpen())
                            {
                                LOGMSG("91U服务器[%s]重复ACCOUNT登录，前一个连接[%d]已断开。", m_aServerName[i], i);
                                m_aServerSocket[i].Close();		// 关闭前一个登录的连接
                                Clear(i);
                            }
                            m_aServerName[i][0] = 0;		// 删除SERVER名字
                            //表小，可全搜一遍	break;
                        }
                    }

                    // 保存91U服务器域名/IP
                    strcpy(m_aServerName[nServerIndex], g_aServerAccount[nAccount].m_szServerName);	// cMsg.m_pInfo->szServer);
                    //?? BUG：提前允许登录了，应该接收到_MSG_LOGIN后开始允许登录
                    PrintText("第%d号91U服务器“%s”已登录", nServerIndex, m_aServerName[nServerIndex]);
                    LOGSERVER("第%d号91U服务器“%s”已登录", nServerIndex, m_aServerName[nServerIndex]);
                    //?? BUG：提前允许登录了，应该接收到_MSG_LOGIN后开始允许登录
                    cRetMsg.Create(nServerIndex, 0, "", ACCOUNTVERSION);
                    m_aServerSocket[nServerIndex].Send(cRetMsg.GetBuf(), cRetMsg.GetSize());
                    LOGMSG("新的91U服务器[%s]ACCOUNT帐号认可。返回服务器ID[%d],游戏版本[%04X]",
                           m_aServerName[nServerIndex], nServerIndex, ACCOUNTVERSION);
                    LOGMSG("新的91U服务器[%s]ACCOUNT登录成功。", m_aServerName[nServerIndex]);
                    ::SetServerState(nServerIndex, c_flagNormal91U);
                    m_aState[nServerIndex] = STATE_NORMAL91U;		// 登录完成

                    // 添加索引
                    //				m_idxID.Add(nServerIndex, nAccount);
                    //				g_aServerAccount[nAccount].m_nIndex = nServerIndex;
                }
                else
                {
                    cRetMsg.Create(0, 0, "", ACCOUNTVERSION);
                    m_aServerSocket[nServerIndex].Send(cRetMsg.GetBuf(), cRetMsg.GetSize());
                    LOGMSG("新的游戏服务器[%s]登录失败", cMsg.m_pInfo->szServer);
                }
            }
            else	// 玩家通过91U服务器登录
            {
                if (m_aState[nServerIndex] != STATE_NORMAL91U)
                {
                    LOGERROR("★错误的收到_MSG_ACCOUNT91U消息★");
                    return false;
                }
                ProcessMsgAccount(nServerIndex, pBuf, nLen);	// szPassword 可能是空串
            }
        }
        break;
    case _MSG_LOGIN:
        {
            if (g_aServerAccount[nServerIndex].m_b91U)
            {
                LOGERROR("★错误的收到_MSG_LOGIN消息★");
                return false;
            }
            // 消息检查(不需要)

//			CMsgConnect	cMsg;
//			cMsg.Create(nServerIndex, 0, "登录成功");

            // 仅简单回传
//			m_aServerSocket[nServerIndex].Send(cMsg.GetBuf(), cMsg.GetSize());
            m_aState[nServerIndex] = STATE_NORMAL;		// 登录完成

            LOGMSG("......[%s]游戏服务器LOGIN登录成功", m_aServerName[nServerIndex]);
            LOGMSG("接收到登录消息。新的游戏服务器[%s]LOGIN登录成功", m_aServerName[nServerIndex]);
            ::SetServerState(nServerIndex, c_flagNormal);
        }
        break;
    case _MSG_FEE:
        {
            CMsgFee		cMsg;
            cMsg.Create(pBuf, nLen);
            OBJID	idAccount = cMsg.m_pInfo->idAccount;

            if (g_aServerAccount[nServerIndex].m_b91U && cMsg.m_pInfo->ucType != CMsgFee::HEARTBEAT)
            {
                LOGERROR("★错误的收到_MSG_FEE消息★");
                return false;
            }

            switch(cMsg.m_pInfo->ucType)
            {
            case CMsgFee::FEE_BEGIN:
                {
                    // 消息检查
                    if (idAccount == ID_NONE)
                    {
                        LOGERROR("错误的FEE_BEGIN消息[%d]★", idAccount);
                        return false;
                    }

                    // 添加在线表。仅用于重新同步
                    if (!g_pOnlineTable->IsOnline(idAccount))		//? 可能引起APPENDNEW()的非原子性操作
                    {
                        DWORD	nAuthenID = 0xCCCCCCCC;		// 代替认证号
                        ::WaitForSingleObject(g_xDatabase, INFINITE);	//+++++++++++++++++++++++++
                        if (g_cDatabase.Create(idAccount))	// VVVVVVVVVVVVVVV
                        {
                            char* pName = g_cDatabase.GetName();
                            if (!g_pOnlineTable->AppendNew(idAccount, nAuthenID, "StartFee", pName, "",
//											g_cDatabase.GetType(), m_aServerName[nServerIndex], idAccount, pName);
                                                           c_typeNone, m_aServerName[nServerIndex], idAccount, pName))
                            {
                                Kickout_0(idAccount, m_aServerName[nServerIndex]);
                            }
                            else
                                //?if (g_bEnableLogin)
                                LOGERROR("补登FEE_BEGIN, 服务器[%s]帐号[%s][%d]。如为ISP模式或计点模式将不再扣点★",
                                         m_aServerName[nServerIndex], pName, idAccount);
                            g_cDatabase.Destroy();			// AAAAAAAAAAAAAAA
                        }
                        else
                        {
                            LOGERROR("游戏服务器[%s]上传了一个未知帐号[%d]的FEE_BEGIN消息，未补登",
                                     m_aServerName[nServerIndex], idAccount);
                        }
                        ::ReleaseMutex(g_xDatabase);	//------------------------------------------
                    }

//#ifdef	SERVER_X
//					LOGPOINT("[%s]上传[%d]的START_FEE", m_aServerName[nServerIndex], idAccount);
//#endif	// SERVER_X
                    int nRet = g_pOnlineTable->StartFee(idAccount, m_aServerName[nServerIndex]);		// return -n: 服务器不匹配
                    // 双重登录，踢人
                    if (nRet < 0)		// <0 : error
                    {
                        LOGERROR("...双重登录，多余的START_FEE。帐号[%d]被踢下线。", idAccount);
                        Kickout_0(idAccount, m_aServerName[nServerIndex]);
                    }

                    if (m_aState[nServerIndex] == STATE_NORMAL)
                    {
                        ::SetServerState(nServerIndex, c_flagBegin);
                    }
                    else
                    {
                        ::SetServerState(nServerIndex, c_flagBegin2);
                    }
                }
                break;
            case CMsgFee::FEE_END:
            case CMsgFee::FEE_OFFLINE:
                {
                    // 消息检查
                    if (idAccount == ID_NONE)
                    {
                        LOGERROR("错误的FEE_END消息[%d]★", idAccount);
                        return false;
                    }

                    g_pOnlineTable->EndFee(idAccount, m_aServerName[nServerIndex],
                                           cMsg.m_pInfo->ucType == CMsgFee::FEE_OFFLINE);

                    // 删除在线表
//由ENDFEE删除					g_pOnlineTable->Remove(idAccount);				// 不能删除空帐号
                    if (m_aState[nServerIndex] == STATE_NORMAL)
                    {
                        ::SetServerState(nServerIndex, c_flagEnd);
                    }
                    else
                    {
                        ::SetServerState(nServerIndex, c_flagEnd2);
                    }
                }
                break;
            case CMsgFee::FEE_POINT:
            case CMsgFee::FEE_TICK:
                {
                    // 消息检查
                    if (idAccount == ID_NONE)
                    {
                        LOGERROR("错误的FEE_POINT消息[%d]★", idAccount);
                        return false;
                    }

                    // 添加在线表。仅用于重新同步
                    if (!g_pOnlineTable->IsOnline(idAccount))		//? 可能引起APPENDNEW()的非原子性操作
                    {
                        DWORD	nAuthenID = 0xCCCCCCCC;		// 代替认证号
                        ::WaitForSingleObject(g_xDatabase, INFINITE);	//+++++++++++++++++++++++++
                        if (g_cDatabase.Create(idAccount))	// VVVVVVVVVVVVVVV
                        {
                            char* pName = g_cDatabase.GetName();
                            if (!g_pOnlineTable->AppendNew(idAccount, nAuthenID, "PointFee", pName, "",
//											g_cDatabase.GetType(), m_aServerName[nServerIndex], idAccount, pName);
                                                           c_typeNone, m_aServerName[nServerIndex], idAccount, pName))
                            {
                                Kickout_0(idAccount, m_aServerName[nServerIndex]);
                            }
                            else
                                //?if (g_bEnableLogin)
                                LOGERROR("补登FEE_POINT，服务器[%s]帐号[%s][%d]。如为ISP模式或计点模式将不再扣点★",
                                         m_aServerName[nServerIndex], pName, idAccount);
                            g_cDatabase.Destroy();			// AAAAAAAAAAAAAAA
                        }
                        else
                        {
                            LOGERROR("游戏服务器[%s]上传了一个未知帐号[%d]的FEE_POINT消息，未补登",
                                     m_aServerName[nServerIndex], idAccount);
                        }
                        ::ReleaseMutex(g_xDatabase);	//------------------------------------------
                    }

                    int		nFeeType		= c_typeNone;
                    char	szServerName[_MAX_NAMESIZE] = "(未知)";
                    OBJID	idFeeAccount	= ID_NONE;
                    char	szFeeAccount[_MAX_NAMESIZE] = "(未知)";
                    int		nPoint			= 0;
                    if (g_pOnlineTable->GetAttr2(idAccount, nFeeType, szServerName, idFeeAccount, szFeeAccount))	//? 可能引起APPENDNEW()的非原子性操作
                    {
                        if ((nFeeType == c_typePoint || nFeeType == c_typeNetBarPoint
                                || nFeeType == c_typeISP) && cMsg.m_pInfo->ucType != CMsgFee::FEE_TICK)		//? 非计点类型不减点
                        {
                            if (strcmp(szServerName, m_aServerName[nServerIndex]) == 0)
                            {
                                bool	bOk		= false;
                                ::WaitForSingleObject(g_xDatabase, INFINITE);	//+++++++++++++++++++++++++
                                if (g_cDatabase.Create(idFeeAccount))	// VVVVVVVVVVVVVVV
                                {
                                    bOk		= true;
                                    nPoint	= g_cDatabase.GetPoint();
                                    g_cDatabase.Destroy();			// AAAAAAAAAAAAAAA
                                }
                                ::ReleaseMutex(g_xDatabase);	//------------------------------------------

                                if (bOk)
                                {
                                    if (nPoint > 0)
                                    {
                                        if (g_cDatabase.DecPoint(idFeeAccount))		//?? 不需要OPEN，直接使用CDATABASE的查询语句。
                                        {
                                            InterlockedIncrement(&s_nPointFee);
                                            InterlockedIncrement(&s_nPointSum);
                                        }
                                        else
                                        {
                                            LOGERROR("数据库错误，帐号ID[%d]无法扣点。★", idFeeAccount);
                                            LOGPOINT("数据库错误，帐号ID[%d]无法扣点。", idFeeAccount);
                                        }
                                    }
                                    else if (KICKOUT_USER_WHEN_NO_POINT)
                                    {
                                        LOGPOINT("服务器[%s]的玩家[%d]因帐号[%s][%d]点数耗尽被踢。",
                                                 m_aServerName[nServerIndex], idAccount, szFeeAccount, idFeeAccount);
                                        Kickout_0(idAccount, m_aServerName[nServerIndex]);
                                    }
                                }
                                else
                                {
                                    LOGERROR("游戏服务器[%s]上传了一个未知帐号[%d]的计点消息。点数未登记★",
                                             m_aServerName[nServerIndex], idFeeAccount);
                                    LOGPOINT("游戏服务器[%s]上传了一个未知帐号[%d]的计点消息。点数未登记",
                                             m_aServerName[nServerIndex], idFeeAccount);
                                }
                            }
                            else
                            {
                                LOGERROR("双重登录。玩家ID[%d]使用帐号[%s][%d]登录了[%s]，但收到[%s]上传了的的计点消息。玩家被踢下线★",
                                         idAccount, szFeeAccount, idFeeAccount, szServerName, m_aServerName[nServerIndex]);
                                Kickout_0(idAccount, m_aServerName[nServerIndex]);
                            }
                        }

                        int	nPointCount = g_pOnlineTable->PointFee(idAccount, m_aServerName[nServerIndex]);		// return -n: 服务器不匹配
//							LOGPOINT("[%s]上传[%d]的POINT_FEE。用了[%d]，剩余[%d]",
//								m_aServerName[nServerIndex], idAccount, nPointCount, nPoint-1);
                        // 双重登录，踢人
//						if (nPointCount < 0)		// <0 : error
                        {
//							LOGERROR("...双重登录，多余的POINT_FEE。帐号[%d]被踢下线。", idAccount);
//							Kickout_0(idAccount, m_aServerName[nServerIndex]);
                        }

                        if (m_aState[nServerIndex] == STATE_NORMAL)
                        {
                            ::SetServerState(nServerIndex, c_flagPoint);
                        }
                        else
                        {
                            ::SetServerState(nServerIndex, c_flagPoint2);
                        }
                    } // GetAttr()
                    else
                    {
                        LOGERROR("共享冲突，FEE_POINT补登录帐号不成功");
                    }
                }
                break;
            case CMsgFee::HEARTBEAT:
                {
                    /*// 消息检查
                    if (idAccount == ID_NONE)
                    {
                    	LOGERROR("错误的HEARTBEAT消息[%d]★", idAccount);
                    	return false;
                    }*/

                    LOGMSG("接收到[%s]游戏服务器上传的心跳消息.", m_aServerName[nServerIndex]);
                    // 无操作，ONPROCESS中会自动刷新心跳时间。
                    ::SetServerState(nServerIndex, c_flagHeartbeat);
                }
                break;
            case CMsgFee::SERVER_BUSY:
                {
                    SetServerBusy(nServerIndex);
                }
                break;
            case CMsgFee::SERVER_FULL:
                {
                    SetServerFull(nServerIndex);
                }
                break;
            case CMsgFee::SERVER_CLOSE:
                {
                    PrintText("接收到服务器[%s]的关闭消息。", m_aServerName[nServerIndex]);
                    //??? SetServerClose(nServerIndex);
                    //??? ::SetServerState(nIndex, c_flagOffline);
                }
                break;
            default:
                LOGERROR("游戏服务器[%s]计费消息子类型[%d]错误", m_aServerName[nServerIndex], cMsg.m_pInfo->ucType);
            }
        }
        break;
    case _MSG_QUERYFEE:
        {
            if (g_aServerAccount[nServerIndex].m_b91U)
            {
                LOGERROR("★错误的收到_MSG_QUERYFEE消息★");
                return false;
            }

            CMsgQueryFee	cMsg;
            cMsg.Create(pBuf, nLen);

            // 消息检查
            if (cMsg.m_pInfo->idAccount == ID_NONE)
            {
                LOGERROR("错误的_MSG_QUERYFEE消息[%d]★", cMsg.m_pInfo->idAccount);
                return false;
            }

            int		nFeeType	= c_typeNone;
            DWORD	dwData		= 0;
            int		nTime		= 0;
            char	szServerName[_MAX_NAMESIZE];
            OBJID	idFeeAccount;
            char	szFeeAccount[_MAX_NAMESIZE];
            if (g_pOnlineTable->GetAttr2(cMsg.m_pInfo->idAccount, nFeeType, szServerName, idFeeAccount, szFeeAccount))
            {
                switch(nFeeType)
                {
                case	c_typePoint:
                    ::WaitForSingleObject(g_xDatabase, INFINITE);	//+++++++++++++++++++++++++
                    if (g_cDatabase.Create(cMsg.m_pInfo->idAccount))	// VVVVVVVVVVVVVVVVVVVVVVVVVVVVV
                    {
                        dwData	= g_cDatabase.GetPoint();
                        g_cDatabase.Destroy();			// AAAAAAAAAAAAAAAAAAAAAAAAAAAAA
                    }
                    else
                    {
                        LOGERROR("游戏服务器[%s]请求查询一个未知帐号[%d]计费数据", m_aServerName[nServerIndex], cMsg.m_pInfo->idAccount);
                    }
                    ::ReleaseMutex(g_xDatabase);	//------------------------------------------
                    break;
                case	c_typeTime:
                    ::WaitForSingleObject(g_xDatabase, INFINITE);	//+++++++++++++++++++++++++
                    if (g_cDatabase.Create(cMsg.m_pInfo->idAccount))	// VVVVVVVVVVVVVVVVVVVVVVVVVVVVV
                    {
                        nTime	= g_cDatabase.GetPointTime();
                        g_cDatabase.Destroy();			// AAAAAAAAAAAAAAAAAAAAAAAAAAAAA
                    }
                    else
                    {
                        LOGERROR("游戏服务器[%s]请求查询一个未知帐号[%d]计费数据", m_aServerName[nServerIndex], cMsg.m_pInfo->idAccount);
                    }
                    ::ReleaseMutex(g_xDatabase);	//------------------------------------------
                    break;
                case	c_typeNetBarTime:
                    /* 暂不开放
                    ::WaitForSingleObject(g_xDatabase, INFINITE);	//+++++++++++++++++++++++++
                    if (g_cDatabase.Create(cMsg.m_pInfo->idAccount))	// VVVVVVVVVVVVVVVVVVVVVVVVVVVVV
                    {
                    	dwData	= g_cDatabase.GetLicence();
                    	g_cDatabase.Destroy();			// AAAAAAAAAAAAAAAAAAAAAAAAAAAAA
                    }
                    else
                    {
                    	LOGERROR("游戏服务器[%s]请求查询一个未知帐号[%d]计费数据", m_aServerName[nServerIndex], cMsg.m_pInfo->idAccount);
                    }
                    ::ReleaseMutex(g_xDatabase);	//------------------------------------------
                    break;
                    //*/
                case	c_typeNetBarPoint:
                case	c_typeISP:
                case	c_typeFree:
                    break;
                }
            }
            /*
            			::WaitForSingleObject(g_xDatabase, INFINITE);	//+++++++++++++++++++++++++
            			if (g_cDatabase.Create(cMsg.m_pInfo->idAccount))	// VVVVVVVVVVVVVVVVVVVVVVVVVVVVV
            			{
            				nType	= g_cDatabase.GetType();
            				dwData	= g_cDatabase.GetPoint();
            				nTime	= g_cDatabase.GetPointTime();
            				g_cDatabase.Destroy();			// AAAAAAAAAAAAAAAAAAAAAAAAAAAAA
            			}
            			else
            			{
            				LOGERROR("游戏服务器[%s]请求查询一个未知玩家[%d]计费数据", m_aServerName[nServerIndex], cMsg.m_pInfo->idAccount);
            			}
            			::ReleaseMutex(g_xDatabase);	//------------------------------------------
            //*/
            // 返回消息
            cMsg.m_pInfo->ucType	= nFeeType;
            cMsg.m_pInfo->dwData	= dwData;
            cMsg.m_pInfo->nTime		= nTime;
            m_aServerSocket[nServerIndex].Send(cMsg.GetBuf(), cMsg.GetSize());
//			LOGPOINT("发送给[%s]游戏服务器[%d]帐号的计费查询消息。类型[%d]，点数[%d]，时限[%04d-%02d-%02d]",
//						m_aServerName[nServerIndex], cMsg.m_pInfo->idAccount,
//						nFeeType, dwData, nTime/10000, (nTime/100)%100, nTime%100);
        }
        break;
    default:
        LOGERROR("游戏服务器上传了未知的消息类型[%d]", unMsgType);
        LOGPOINT("接收到[%s]游戏服务器上传的未知消息.", m_aServerName[nServerIndex], unMsgType);
    }

    return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////
bool	CPointThread::ProcessMsgAccount(int nIndex, char* pBuf, int nLen)
{
    CMsgAccount91U	cMsg;
    cMsg.Create(pBuf, nLen);
#ifdef	RC5ENCRYPT
    m_cRc5.Rc5Decrypt(cMsg.m_pInfo->szPassword, _MAX_NAMESIZE);
#endif
    FormatStr(cMsg.m_pInfo->szAccount, _MAX_NAMESIZE);		// 添尾0，删尾空
    FormatStr(cMsg.m_pInfo->szPassword, _MAX_NAMESIZE);		// 添尾0，删尾空
    FormatStr(cMsg.m_pInfo->szServer, _MAX_NAMESIZE);		// 添尾0，删尾空
    FormatStr(cMsg.m_pInfo->szIPAddr, _MAX_NAMESIZE);		// 添尾0，删尾空

    if (cMsg.m_pInfo->szAccount[0] == 0 || cMsg.m_pInfo->szIPAddr[0] == 0 || cMsg.m_pInfo->szServer[0] == 0)
    {
        return false;
    }

    OBJID	id91U							= cMsg.m_pInfo->id91U;
    char* 	pClientIP						= cMsg.m_pInfo->szIPAddr;
    unsigned long nClientIP					= inet_addr(pClientIP);
    DWORD	nAuthenID						= NewAuthenID(nClientIP);
    char* 	pErrMsg							= "登录失败";
    char	szNotifyMsg[_MAX_NAMESIZE]		= "";
    OBJID	idAccount						= ID_NONE;
    OBJID	idFeeAccount					= ID_NONE;
    char	szFeeAccount[_MAX_NAMESIZE]		= "";
    int		nFeeType						= c_typeNone;
    int		nPoint							= 0;
    DWORD	nPointTime						= 0;
    int		nBlock							= false;
    bool	bRejoin							= false;
    char	szOldServerName[SERVERNAMESIZE]	= "";
    char	szOldFeeAccount[_MAX_NAMESIZE]	= "";
    char 	szPassword[_MAX_NAMESIZE]		= "";		// 无用变量
    OBJID	idOldFeeAccount					= ID_NONE;	// 无用变量
    char	szOldIP[_MAX_NAMESIZE]			= "";		// 无用变量

    if (g_pOnlineTable->GetAttr(cMsg.m_pInfo->szAccount, idAccount, szPassword, nAuthenID,			// 仅nAuthenID、szOldServerName、szOldFeeAccount 有意义
                                szOldServerName, szOldFeeAccount, idOldFeeAccount, szOldIP))		//??? 在线。该函数可能引起ADDNEW的非原子性操作
    {
        bRejoin	= true;
    }

    if (!::GetAccount(cMsg.m_pInfo->szAccount, cMsg.m_pInfo->szPassword,			// cMsg.m_pInfo->szPassword: 可能为""
                      idAccount, nFeeType, nPoint, nPointTime, nBlock))			// 返回值
    {
        idAccount	= ID_NONE;
        if (/*ENABLE_91U_CREATE_ACCOUNT*/1 && cMsg.m_pInfo->szPassword[0] == 0)
        {
            char szSQL[1024];
            sprintf("INSERT account SET name='%s',type=6,point=0", cMsg.m_pInfo->szAccount);
            if (g_db.ExecuteSQL(szSQL))
            {
                idAccount	= g_db.GetInsertId();
                nFeeType	= c_typePoint;
                nPoint		= 0;
                nPointTime	= 0;
                nBlock		= false;
            }
        }

        if (idAccount == ID_NONE)
        {
            AddBan(nClientIP, pClientIP, cMsg.m_pInfo->szAccount);
            RefuseLogin(nIndex, id91U, cMsg.m_pInfo->szAccount, c_errPassword, "帐号名或口令错");
            LOGACCOUNT("玩家[%s]91U[%u]登录。帐号名或口令错, IP地址[%s]",
                       cMsg.m_pInfo->szAccount, id91U, pClientIP);
            return false;
        }
    }

    // 查封号
    if (nBlock)
    {
        RefuseLogin(nIndex, id91U, cMsg.m_pInfo->szAccount, c_errBan, "该帐号被封号");
        return false;
    }

    // 检查非法登录
    if (nFeeType == c_typeNetBarPoint || nFeeType == c_typeNetBarTime || nFeeType == c_typeISP)
    {
        RefuseLogin(nIndex, id91U, cMsg.m_pInfo->szAccount, c_errBarPassword, "该帐号不能登录");
        return false;
    }

    if (IsFreeServer(cMsg.m_pInfo->szServer))
    {
        // 免费服务器
        nFeeType		= c_typeFree;
//		SafeCopy(szNotifyMsg, "欢迎参加测试", _MAX_NAMESIZE);
    }
    else if (CheckISP(idAccount, nClientIP, pClientIP,
                      idFeeAccount, szFeeAccount, nPoint, nPointTime, szNotifyMsg))		// 返回值
    {
        // ISP类型
        nFeeType		= c_typeISP;
    }
    else
    {
        idFeeAccount	= idAccount;
        SafeCopy(szFeeAccount, cMsg.m_pInfo->szAccount, _MAX_NAMESIZE);
    }

    if (bRejoin)
    {
        // 不允许同时登录到另一台服务器。查服务器名和计费帐号名是否相同
        if (strcmp(cMsg.m_pInfo->szServer, szOldServerName) != 0
                || strcmp(szFeeAccount, szOldFeeAccount) != 0)	// 上次是用网吧帐号或ISP上的
        {
            if (!g_pOnlineTable->IsTimeWait(idAccount))
            {
                g_pPointThread->Kickout(idAccount);    // 通知游戏服务器踢人
            }

            RefuseLogin(nIndex, id91U, cMsg.m_pInfo->szAccount, c_errOnline, "请稍后重新登录");
            return false;
        }
    }

    // 分类检查过期
    switch(nFeeType)
    {
    case c_typePoint:	// 没有处理计点卡的时限(点卡也应该有时限)
    case c_typeISP:
        if (nPoint <= 0)
        {
            if (ENABLE_LOGIN_NO_POINT)
            {
                SafeCopy(szNotifyMsg, FLAG_NO_POINT, _MAX_NAMESIZE);
            }
            else
            {
                RefuseLogin(nIndex, id91U, cMsg.m_pInfo->szAccount, c_errNotPoint, "小时数已用尽");
                return false;
            }
        }
        break;
    case c_typeTime:
        {
            time_t	tCurr = time(NULL);
            tm* 	pTm = localtime(&tCurr);
            DWORD	nCurrTime = (pTm->tm_year + 1900) * 10000 + (pTm->tm_mon + 1) * 100 + pTm->tm_mday;
            if (nCurrTime > nPointTime)
            {
                if (MONTH_CARD_TO_POINT_CARD && nPoint > 0)
                {
                    nFeeType	= c_typePoint;
                }
                else if (MONTH_CARD_TO_POINT_CARD && ENABLE_LOGIN_NO_POINT)
                {
                    nFeeType	= c_typePoint;
                    SafeCopy(szNotifyMsg, FLAG_NO_POINT, _MAX_NAMESIZE);
                }
                else
                {
                    RefuseLogin(nIndex, id91U, cMsg.m_pInfo->szAccount, c_errNotTime, "帐号已过期");
                    return false;
                }
            }
            else
            {
                // 检查是否还剩7天
                if (szNotifyMsg[0] == 0)		// 没有通知消息时
                {
                    Check7DaysNodify(nPointTime, szNotifyMsg);		// return szNotifyMsg
                }
            }
        }
        break;
    case c_typeFree:
        break;
    default:
        RefuseLogin(nIndex, id91U, cMsg.m_pInfo->szAccount, c_errUnknown, "数据库错误");
        LOGERROR("玩家[%s](91U:%d)计费类型错误[%d]。★★★★★★★★★", cMsg.m_pInfo->szAccount, id91U, nFeeType);
        return false;
    }

    // 登录
    ASSERT(idAccount);

    // 通知游戏服务器
    int nRet = g_pPointThread->NewLogin(idAccount, nAuthenID, szNotifyMsg, cMsg.m_pInfo->szServer);
    switch(nRet)
    {
    case CPointThread::ERR_NO_SERVER:
        RefuseLogin(nIndex, id91U, cMsg.m_pInfo->szAccount, c_errUnknowServer, "服务器未启动");
        return false;
    case CPointThread::ERR_BUSY:
        RefuseLogin(nIndex, id91U, cMsg.m_pInfo->szAccount, c_errServerBusy, "服务器忙请稍候");
        return false;
    case CPointThread::ERR_FULL:
        RefuseLogin(nIndex, id91U, cMsg.m_pInfo->szAccount, c_errServerFull, "服务器人数已满");
        return false;
    case CPointThread::ERR_NONE:
        {
            if (bRejoin)
            {
                // 重新进入游戏，修改一些属性
                if (!g_pOnlineTable->Rejoin(idAccount, nAuthenID, pClientIP,
                                            cMsg.m_pInfo->szAccount, cMsg.m_pInfo->szPassword,
                                            nFeeType, cMsg.m_pInfo->szServer, idFeeAccount, szFeeAccount))
                {
                    if (!g_pOnlineTable->IsTimeWait(idAccount))
                    {
                        g_pPointThread->Kickout(idAccount);    // 通知游戏服务器踢人
                    }

                    RefuseLogin(nIndex, id91U, cMsg.m_pInfo->szAccount, c_errOnline, "请稍后重新登录");
                    return false;
                }

                AllowLogin(nIndex, id91U, idAccount, nAuthenID, cMsg.m_pInfo->szServer);
                LOGACCOUNT("同意91U[%d]玩家[%s][%d]按[%d]类型重新登录[%s]。认证ID[%08X]，IP[%s]，计费帐号[%s]，通知[%s]",
                           id91U, cMsg.m_pInfo->szAccount, idAccount, nFeeType, cMsg.m_pInfo->szServer,
                           nAuthenID, pClientIP, szFeeAccount, szNotifyMsg);
            }
            else
            {
                // 添加在线表
                if (!g_pOnlineTable->AddNew(idAccount, nAuthenID, pClientIP,
                                            cMsg.m_pInfo->szAccount, cMsg.m_pInfo->szPassword,
                                            nFeeType, cMsg.m_pInfo->szServer, idFeeAccount, szFeeAccount))
                {
                    if (!g_pOnlineTable->IsTimeWait(idAccount))
                    {
                        g_pPointThread->Kickout(idAccount);    // 通知游戏服务器踢人
                    }

                    RefuseLogin(nIndex, id91U, cMsg.m_pInfo->szAccount, c_errOnline, "请稍后重新登录");
                    return false;
                }

                AllowLogin(nIndex, id91U, idAccount, nAuthenID, cMsg.m_pInfo->szServer);
                LOGACCOUNT("同意91U[%d]玩家[%s][%d]按[%d]类型登录[%s]。认证ID[%08X]，IP[%s]，计费帐号[%s]，通知[%s]",
                           id91U, cMsg.m_pInfo->szAccount, idAccount, nFeeType, cMsg.m_pInfo->szServer,
                           nAuthenID, pClientIP, szFeeAccount, szNotifyMsg);
            }

            // dump to database
            char	szStamp[255];
            time_t	tCurr = time(NULL);
            tm*	pTm = localtime(&tCurr);
            sprintf(szStamp, "%04d%02d%02d%02d%02d%02d", pTm->tm_year + 1900, pTm->tm_mon + 1, pTm->tm_mday, pTm->tm_hour, pTm->tm_min, pTm->tm_sec);
            char szSQL[1024];
            sprintf(szSQL, "INSERT DELAYED logon VALUES ('%s', '%s',%d,%d,'%s',   %u,'%s','%s','%s',%d);",
                    szStamp, cMsg.m_pInfo->szAccount, idAccount, nFeeType, cMsg.m_pInfo->szServer,
                    nAuthenID, pClientIP, szFeeAccount, szNotifyMsg, bRejoin + 2);		// +2 for 91U
            g_db.ExecuteSQL(szSQL);
            return true;
        }
        break;
    default:
        ASSERT(!"NewLogin");
        return false;
    }
}

void CPointThread::AddBan(DWORD id91U, LPCTSTR szClientIP, LPCTSTR szAccount)
{
    // 增加BAN表
    bool	bFoundBan = false;
    int		nFreeSlot = -1;
    int		nBanCount = 0;
    // 找个空位置
    for(int i = 0; i < MAXBANIPS; i++)
    {
        if (m_pBanIPs[i].ClientIP() == 0)
        {
            if (nFreeSlot == -1)
            {
                nFreeSlot = i;
            }
        }
        else
        {
            nBanCount++;
        }

        if (m_pBanIPs[i].ClientIP() == id91U)	// 已记录
        {
            bFoundBan = true;
            m_pBanIPs[i].IncError();				//? 不立即踢，超过再踢
            if (m_pBanIPs[i].IsBan())
            {
                LOGERROR("某客户端已非法登录[%d]次，可能有人攻击帐号服务器。91Uid[%u], IP地址[%s]", BANERRORS, id91U, szClientIP);
                LOGACCOUNT("某玩家[%s]登录到帐号服务器连续出错[%d]次。91U[%u](IP地址[%s])被禁止[%d]秒钟",
                           szAccount, BANERRORS, id91U, szClientIP, BANSECS);
                PrintText("IP地址“%s”(91U:%u)连续出错“%d”次, “%d”秒内将无法登录",
                          szClientIP, id91U, BANERRORS, BANSECS);
            }
        }
    }
    if (!bFoundBan)
    {
        // 添加新BAN
        if (nFreeSlot != -1)
        {
            m_pBanIPs[nFreeSlot].Create(id91U);
            if (nBanCount * 100 / MAXBANIPS > 78)		// 快满了
            {
                LOGWARNING("添加BAN表[%d/%d], 91U[%u]，IP为[%s]", nBanCount + 1, MAXBANIPS, id91U, szClientIP);
            }
        }
        else
        {
            LOGERROR("BAN 表太小，有个91U[%u](IP[%s])没填进去。请适当增大 MAXBANIPS 参数", id91U, szClientIP);
        }
    }
}

void CPointThread::AllowLogin(int nServerIndex, DWORD id91U, OBJID idAccount, DWORD nAuthenID, LPCTSTR szServer)
{
    CMsgConnect91U cRetMsg;
    char	szServerIP[IPSTRSIZE] = "";
    g_pPointThread->GetServerIP(szServerIP, szServer);
    cRetMsg.Create(id91U, idAccount, nAuthenID, szServerIP);		//? 下传游戏服务器的IP地址
    m_aServerSocket[nServerIndex].Send(cRetMsg.GetBuf(), cRetMsg.GetSize());
//!	if (!STAT_SERIAL)
//!		m_aServerSocket[nServerIndex].ShutDown();		//?? 关闭，让对方先关闭(注意：如果没收到CMsgConnect消息则不会关闭)
}

void CPointThread::RefuseLogin(int nServerIndex, DWORD id91U, LPCTSTR szLoginName, int nType, LPCTSTR szText)
{
    CMsgConnect91U cRetMsg;
    cRetMsg.Create(id91U, ID_NONE, nType, (char*)szText);
    m_aServerSocket[nServerIndex].Send(cRetMsg.GetBuf(), cRetMsg.GetSize());
//!	m_aServerSocket[nServerIndex].ShutDown();		//?? 关闭，让对方先关闭
    LOGACCOUNT("通过91U[%d]登录不成功[%s]。原因是[%s][%d]", id91U, szLoginName, szText, nType);
}

//////////////////////////////////////////////////////////////////////
// 返回0：错误
OBJID	CPointThread::GetServerIndex_0(const char* pServerName)
{
    int i = 1;
    for(; i < MAXGAMESERVERS; i++)	// 保留0，0为错误
    {
        if (strcmp(m_aServerName[i], pServerName) == 0)
        {
            break;
        }
    }

    if (i < MAXGAMESERVERS)
    {
        if (m_aServerSocket[i].IsOpen())
        {
            return i;
        }
        else
        {
            if (m_aServerName[i][0])
            {
                LOGWARNING("取服务器ID时，游戏服务器[%s]已经关闭", m_aServerName[i]);
                LOGSERVER("服务器[%s]已经关闭了。", m_aServerName[i]);
                PrintText("服务器“%s”已经关闭了。", m_aServerName[i]);
            }
            Clear(i);		// SOCKET关闭后，删除游戏服务器名。被动同步
        }
    }

    return 0;
}

// 检查服务器是否在线，不在线时转换显示状态，并不让玩家登录。
bool	CPointThread::CheckHeartbeat(int nIndex)
{
    // 恢复BUSY、FULL
    if ((m_aState[nIndex] == STATE_BUSY || m_aState[nIndex] == STATE_FULL) && clock() > m_aServerDelay[nIndex])
    {
        m_aState[nIndex] = STATE_NORMAL;
        ::SetServerState(nIndex, c_flagNormal);
    }

    bool	ret = false;
    // 检查心跳
    if (m_aHeartbeatLast[nIndex] && (clock() - m_aHeartbeat[nIndex]) / CLOCKS_PER_SEC > HEARTBEATKICKSECS)
    {
        LOGWARNING("游戏服务器[%s]因为响应超时[%d]秒，连接被强行断开",
                   m_aServerName[nIndex], (clock() - m_aHeartbeat[nIndex]) / CLOCKS_PER_SEC);
        LOGSERVER("响应超时，服务器[%s]已断开连接!", m_aServerName[nIndex]);
        PrintText("响应超时，服务器“%s”已断开连接!", m_aServerName[nIndex]);
        m_aServerSocket[nIndex].Close();
        Clear(nIndex);
        ret = true;
    }
    else if ((clock() - m_aHeartbeat[nIndex]) / CLOCKS_PER_SEC > HEARTBEATINTERVALSECS
             && (clock() - m_aHeartbeatLast[nIndex]) / CLOCKS_PER_SEC > HEARTBEATINTERVALSECS)
    {
        CMsgFee	cMsg;
        cMsg.Create(666666, CMsgFee::HEARTBEAT);		// 666666: 玩家帐号ID，无意义
        if (m_aServerSocket[nIndex].Send(cMsg.GetBuf(), cMsg.GetSize()))
        {
            LOGERROR("游戏服务器[%s]响应超时[%d]秒，已发送一heartbeat消息",
                     m_aServerName[nIndex], (clock() - m_aHeartbeat[nIndex]) / CLOCKS_PER_SEC);
        }
        else
        {
            LOGWARNING("游戏服务器[%s]连接已关闭，无法SEND心跳消息", m_aServerName[nIndex]);
            LOGSERVER("发送心跳失败，服务器[%s]已断开连接!", m_aServerName[nIndex]);
            PrintText("发送心跳失败，服务器“%s”已断开连接!", m_aServerName[nIndex]);
            Clear(nIndex);
        }
        m_aHeartbeatLast[nIndex] = clock();
    }

    return ret;
}

void CPointThread::Clear(int nIndex, bool flag /*= true*/)
{
    m_aState[nIndex] = STATE_OFFLINE;
    m_aServerName[nIndex][0] = m_aHeartbeat[nIndex] = m_aHeartbeatLast[nIndex] = m_aServerDelay[nIndex] = 0;
    if (flag)
    {
        ::SetServerState(nIndex, c_flagOffline);
    }
}

void CPointThread::SetServerBusy(int nServerIndex)
{
    m_aServerDelay[nServerIndex] = clock() + SERVER_BUSY_DELAY_SECS * CLOCKS_PER_SEC;
    m_aState[nServerIndex] = STATE_BUSY;
    ::SetServerState(nServerIndex, c_flagStop);
}

void CPointThread::SetServerFull(int nServerIndex)
{
    m_aServerDelay[nServerIndex] = clock() + SERVER_FULL_DELAY_SECS * CLOCKS_PER_SEC;
    m_aState[nServerIndex] = STATE_FULL;
    ::SetServerState(nServerIndex, c_flagStop);
}

