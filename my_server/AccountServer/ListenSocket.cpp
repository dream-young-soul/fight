
//**********************************************************
// 代码编辑器
//**********************************************************

// 监听线程部分
// 仙剑修, 2001.10.10

#include "allheads.h"
#include "ListenSocket.h"

CListenSocket::CListenSocket(u_short nPort, int nSndBuf /*= 0*/)	// 0: 缺省
    : m_nPort(nPort), m_nSndBuf(nSndBuf)
{
    m_sockListen	= INVALID_SOCKET;
    m_bState		= false;
//	m_nPort			= nPort;
    m_tCloseTime	= 0;
}

CListenSocket::~CListenSocket()
{
    Close();
}

//	创建服务端监听SOCKET，具体选项请看代码。失败时返回false。
bool	CListenSocket::Open()		// 可重复调用
{
    if (m_bState)
    {
        return true;
    }

    // 初始化网络
    WSADATA		wsaData;
    int	err;
    if ((err = WSAStartup(0x0002, &wsaData)) != 0)
    {
        LOGERROR("Init WSAStartup() failed[%d].", err);
        return FALSE;
    }

    // 设置异常中断信号
//	signal(SIGINT, ::CloseListenSock);
//	signal(SIGABRT, ::CloseListenSock);
//	signal(SIGTERM, ::CloseListenSock);

    // 检查版本
    if (wsaData.wVersion != 0x0002)
    {
//		WSACleanup();
        LOGERROR("WSAStartup Version[0x%04X] not match 2.0", wsaData.wVersion);
        return FALSE;
    }
    // 创建主套接字
    m_sockListen = socket(AF_INET, SOCK_STREAM, 0);
    if (m_sockListen == INVALID_SOCKET)
    {
        int err = WSAGetLastError();
//		WSACleanup();
        LOGERROR("Init socket() failed[%d].", err);
        return FALSE;
    }
    else
    {
        InterlockedIncrement(&s_nSocketCount);
        m_bState = true;
    }

    // 设置SOCKET的REUSEADDR
    int		optval = 1;
    if (setsockopt(m_sockListen, SOL_SOCKET, SO_REUSEADDR, (char*) &optval, sizeof(optval)))
    {
        int err = WSAGetLastError();
        Close();
        LOGERROR("Init setsockopt() SO_REUSEADDR failed[%d].", err);
        return FALSE;
    }

    // 设置SOCKET的KEEPALIVE
    optval = 1;
    if (setsockopt(m_sockListen, SOL_SOCKET, SO_KEEPALIVE, (char*) &optval, sizeof(optval)))
    {
        int err = WSAGetLastError();
        Close();
        LOGERROR("Init setsockopt() SO_KEEPALIVE failed[%d].", err);
        return FALSE;
    }

    // 设置SOCKET的SNDBUF
    if (m_nSndBuf)		// 非0：指针SNDBUG
    {
        optval = m_nSndBuf;
        if (setsockopt(m_sockListen, SOL_SOCKET, SO_SNDBUF, (char*) &optval, sizeof(optval)))
        {
            int err = WSAGetLastError();
            Close();
            LOGERROR("Init setsockopt() SO_SNDBUF failed[%d].", err);
            return FALSE;
        }

        // 读取SOCKET的SNDBUF
        int ret = sizeof(optval);
        if (getsockopt(m_sockListen, SOL_SOCKET, SO_SNDBUF, (char*) &optval, &ret) == 0)
        {
            LOGMSG("帐号SOCKET内部发送缓冲区尺寸为[%d]K.", optval / 1024);
        }
    }
    // 绑定
    SOCKADDR_IN		sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family		= AF_INET;
    sin.sin_addr.s_addr	= 0;
    sin.sin_port		= htons(m_nPort);
    if (bind(m_sockListen, (LPSOCKADDR)&sin, sizeof(sin)))
    {
        int err = WSAGetLastError();
        Close();
        LOGERROR("Init bind() failed[%d].", err);
        return FALSE;
    }

    // 设置为非阻塞方式
    //*
    unsigned long i = 1;
    if (ioctlsocket(m_sockListen, FIONBIO, &i))
    {
        int err = WSAGetLastError();
        Close();
        LOGERROR("Init ioctlsocket() failed[%d].", err);
        return FALSE;
    }
    //*/

    // 监听端口
    if (listen(m_sockListen, SOMAXCONN))		// SOMAXCONN: WIN的宏定义
    {
        int err = WSAGetLastError();
        Close();
        LOGERROR("Init listen() failed[%d].", err);
        return FALSE;
    }

    return TRUE;
}

bool	CListenSocket::Rebuild()
{
    if (!m_bState)
    {
        if (clock() < m_tCloseTime + REBUILDLISTENDELAYSEC * CLOCKS_PER_SEC)
        {
            return false;
        }
        else
        {
            LOGMSG("防崩溃机制启动，监听SOCKET[%d]重新建中 . . .", m_nPort);
            if (Open())
            {
                LOGMSG(". . . 监听SOCKET重建成功。");
                PrintText("监听SOCKET %d 失败，重建SOCKET成功。服务端继续运行中...", m_nPort);
            }
            else
            {
                LOGERROR(". . . 监听SOCKET重建失败。服务端将无法接收新连接");
                PrintText("监听SOCKET %d 错误，%d 秒后将重建SOCKET。服务端继续运行中...", m_nPort, REBUILDLISTENDELAYSEC);		// nDelaySec);
                return false;
            }
            m_tCloseTime = clock();
        }
    }

    return true;
}

//	错误时返回INVALID_SOCKET
SOCKET	CListenSocket::Accept(u_long& nClientIP)
{
    /*
    	// 重建SOCKET
    	if (!m_bState)
    	{
    		if (clock() < m_tCloseTime + REBUILDLISTENDELAYSEC*CLOCKS_PER_SEC)
    			return INVALID_SOCKET;
    		else
    		{
    			LOGMSG("防崩溃机制启动，监听SOCKET[%d]重新建中 . . .", m_nPort);
    			if (Open())
    			{
    				LOGMSG(". . . 监听SOCKET重建成功。");
    				PrintText("监听SOCKET[%d]失败，重建SOCKET成功。服务端继续运行中...", m_nPort);
    			}
    			else
    			{
    				LOGERROR(". . . 监听SOCKET重建失败。服务端将无法接收新连接");
    				PrintText("监听SOCKET[%d]错误，[%d]秒后将重建SOCKET。服务端继续运行中...", m_nPort, REBUILDLISTENDELAYSEC);		// nDelaySec);
    			}
    			m_tCloseTime = clock();
    		}
    	}
    //*/
    if (!m_bState)
    {
        LOGERROR("ACCEPT内部异常a1");
        return INVALID_SOCKET;
    }

    // ACCEPT
    struct sockaddr_in	addr;
    memset(&addr, 0, sizeof(addr));
    int	   len = sizeof(addr);
    SOCKET newsock = accept(m_sockListen, (sockaddr*)&addr, (int*)&len);		// 接收到对方的地图，可使用
#ifdef	PROFILE_X
    // 分析Accept速度(循环速度)
    const int	nTimes2 = ACCEPTPROFILESEC;		// 每30秒统计一次ACCEPT速度
    static	clock_t tNextTime2 = clock() + nTimes2 * CLOCKS_PER_SEC;	//? 仅有一个监听线程，无共享冲突
    static long	nCount2	= 0;								//? 仅有一个监听线程，无共享冲突
    if (clock() >= tNextTime2)
    {
        LOGPROFILE("每[%d]秒执行了[%d]次Accept()", nTimes2, InterlockedExchange(&nCount2, 0));
        tNextTime2 = clock() + nTimes2 * CLOCKS_PER_SEC;
    }
    else
    {
        InterlockedIncrement(&nCount2);
    }
#endif	// PROFILE
    if (newsock == INVALID_SOCKET)
    {
        // 网络错误
        int err = WSAGetLastError();
        if (err != WSAEWOULDBLOCK)
        {
            PrintText("监听SOCKET %d 失败，%s 秒后将重建SOCKET.", m_nPort, REBUILDLISTENDELAYSEC);
            LOGERROR("监听SOCKET[%d]失败[%d]，[%s]秒后将重建SOCKET.", m_nPort, err, REBUILDLISTENDELAYSEC);
            Close();
        }
        else
        {
            LOGERROR("ACCEPT内部异常a2");
        }
        return INVALID_SOCKET;
    }
    else
    {
        nClientIP = addr.sin_addr.S_un.S_addr;
        InterlockedIncrement(&s_nSocketCount);
    }

    // 检查SOCKET是否已关闭
    fd_set	readmask;
    FD_ZERO(&readmask);
    FD_SET(newsock, &readmask);
    struct timeval	timeout = {0, 0};
    /*
    	char nTemp;
    	if (select(FD_SETSIZE, &readmask, (fd_set *) 0, (fd_set *) 0, &timeout)
    			&& recv(newsock, &nTemp, 1, MSG_PEEK) == 0)
    	{
    #ifdef	ALPHA_X
    		LOGMSG("ACCEPT一个新SOCKET是无效SOCKET。");
    #endif
    		closesocket(newsock);
    		InterlockedDecrement(&s_nSocketCount);
    		return INVALID_SOCKET;
    	}
    //else*/
//*
    fd_set	exceptmask;
    FD_ZERO(&exceptmask);
    FD_SET(newsock, &exceptmask);
    int ret = select(FD_SETSIZE, &readmask, (fd_set*) 0, (fd_set*) &exceptmask, &timeout);
    if (ret < 0)
    {
        LOGERROR("message: ACCEPT一个新SOCKET是无效SOCKET。can't read");		// 不会触发
        closesocket(newsock);
        InterlockedDecrement(&s_nSocketCount);
        return INVALID_SOCKET;
    }
    else if (ret > 0)
    {
        if (FD_ISSET(newsock, &exceptmask))
        {
            LOGMSG("ACCEPT一个新SOCKET是无效SOCKET。except");		// 不会触发
            closesocket(newsock);
            InterlockedDecrement(&s_nSocketCount);
            return INVALID_SOCKET;
        }
        else if (FD_ISSET(newsock, &readmask))
        {
            char nTemp;
            if (recv(newsock, &nTemp, 1, MSG_PEEK) == 0)
            {
#ifdef	ALPHA_X
                LOGMSG("ACCEPT一个新SOCKET是无效SOCKET。recv==0");		// 已经关闭了
#endif
                closesocket(newsock);
                InterlockedDecrement(&s_nSocketCount);
                return INVALID_SOCKET;
            }
        }
    }
//*/
#ifdef	PROFILE_X
    // 分析Accept速度(接收到有效SOCKET)
    const int	nTimes = ACCEPTPROFILESEC;		// 每10秒统计一次ACCEPT速度
    static	clock_t tNextTime = clock() + nTimes * CLOCKS_PER_SEC;		//? 仅有一个监听线程，无共享冲突
    static long	nCount	= 0;							//? 仅有一个监听线程，无共享冲突
    if (clock() >= tNextTime)
    {
        LOGPROFILE("Port[%d]每[%d]秒成功执行了[%d]次Accept()",
                   m_nPort, nTimes, InterlockedExchange(&nCount, 0));
        tNextTime = clock() + nTimes * CLOCKS_PER_SEC;
    }
    else
    {
        InterlockedIncrement(&nCount);
    }
#endif	// PROFILE

    return newsock;
}

//	可重复调用。通常不用，可用于主动关闭SOCKET。
//	关闭后可重新调用Open()重复使用该对象。
void	CListenSocket::Close()		// 可重复调用
{
    if (m_bState)
    {
//		if (bLinger0)
        {
            //	修改SOCKET属性成立即关闭型
            struct linger ling;
            ling.l_onoff = 1;
            ling.l_linger = 0;
            setsockopt(m_sockListen, SOL_SOCKET, SO_LINGER, (char*)&ling, sizeof(ling));
        }
        m_bState = false;
        closesocket(m_sockListen);
        InterlockedDecrement(&s_nSocketCount);
//		WSACleanup();
        m_sockListen = INVALID_SOCKET;
        m_tCloseTime = clock();
    }
}
