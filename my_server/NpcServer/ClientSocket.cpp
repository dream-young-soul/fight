
//**********************************************************
// 代码编辑器
//**********************************************************

// ClientSocket.cpp
// paled, 2002.6.16

// 该类用于SOCKET连接的客户端一方。即可以用于客户端程序，也可以用于小游戏服务器连接数据服务器。
// 使用非阻塞方式实现。

#include "ClientSocket.h"

#define	ENCRYPT					// 加密开关

/////////////////////////////////////////////////////////////////////////////////
// 全局函数
/////////////////////////////////////////////////////////////////////////////////
bool	ClientSocketInit()
{
    // 初始化网络
    WSADATA		wsaData;
    int ret = WSAStartup(0x0002, &wsaData);			// 0x0101
    if (ret != 0)
    {
        LOGERROR("ERROR: Init WSAStartup() failed.");
        return false;
    }
    // 检查版本
    if (wsaData.wVersion != 0x0002)				// 0x0101
    {
        LOGERROR("ERROR: WSAStartup Version not match 2.0");		// 1.1
        return false;
    }
    return true;
}

/////////////////////////////////////////////////////////////////////////////////
bool	ClientSocketFinal()
{
    WSACleanup();
    return true;
}

/////////////////////////////////////////////////////////////////////////////////
// CClientSocket
/////////////////////////////////////////////////////////////////////////////////
CClientSocket::CClientSocket()
{
    m_sock		= INVALID_SOCKET;
    m_nLen		= 0;
    m_nSendLen	= 0;
    m_nState	= STATE_CLOSED;
    m_szIP[0]	= 0;
}

/////////////////////////////////////////////////////////////////////////////////
bool CClientSocket::Open(const char* szName, int nPort, int nBlockSecs /*= 0*/, int nSndBuf /*= 0*/, int nRcvBuf /*= 0*/)
{
    ASSERT(m_nState == STATE_CLOSED);
    SafeCopy(m_szIP, szName, IPSTR_SIZE);
    m_nPort		= nPort;
    m_nSndSize	= nSndBuf;
    m_nRcvSize	= nRcvBuf;
    if (m_nState == STATE_OPEN)
    {
        return true;
    }
    if (m_nState == STATE_CLOSED)
    {
        ASSERT(m_sock == INVALID_SOCKET);
#ifdef	ENCRYPT
        m_cSendEncrypt.Init();
        m_cRecvEncrypt.Init();
#endif
        // 创建套接字
        m_sock = socket(PF_INET, SOCK_STREAM, 0);
        if (m_sock == INVALID_SOCKET)
        {
            LOGERROR("Init socket() failed.");
            return false;
        }
        // 初始化
        m_nLen		= 0;
        // 设置SOCKET为KEEPALIVE
        int		optval = 1;
        if (setsockopt(m_sock, SOL_SOCKET, SO_KEEPALIVE, (char*) &optval, sizeof(optval)))
        {
            closesocket(m_sock);
            LOGERROR("setsockopt() set SO_KEEPALIVE failed.");
            return false;
        }
        // 设置SENDBUG
        optval = m_nSndSize;
        if (m_nSndSize && setsockopt(m_sock, SOL_SOCKET, SO_SNDBUF, (char*) &optval, sizeof(optval)))
        {
            int err = WSAGetLastError();
            LOGERROR("setsockopt() set SO_SNDBUF failed[%d].", err);
            Close();
            return false;
        }
        // 设置RECVBUG
        optval = m_nRcvSize;
        if (m_nRcvSize && setsockopt(m_sock, SOL_SOCKET, SO_RCVBUF, (char*) &optval, sizeof(optval)))
        {
            int err = WSAGetLastError();
            LOGERROR("setsockopt() set SO_RCVBUF failed[%d].", err);
            Close();
            return false;
        }
        // 设置为非阻塞方式
        unsigned long	i = 1;
        if (ioctlsocket(m_sock, FIONBIO, &i))
        {
            LOGERROR("Init ioctlsocket() failed.");
            Close();
            return false;
        }
        // 域名->IP地址
        UINT	nAddr = inet_addr(m_szIP);			// 必须为 UINT, 以便与in_addr兼容
        if (nAddr == INADDR_NONE)
        {
            hostent*	 hp;
            hp = gethostbyname(m_szIP);
            if (hp == 0)
            {
                LOGERROR("帐号服务器的域名错.");
                Close();
                return false;
            }
            nAddr = *(UINT*)(hp->h_addr_list[0]);	// 或 h_addr
        }
        sockaddr_in	addr_in;
        memset((void*)&addr_in, sizeof(addr_in), 0);
        addr_in.sin_family = AF_INET;
        addr_in.sin_port = htons(m_nPort);
        addr_in.sin_addr.s_addr = nAddr;
        int ret = connect(m_sock, (sockaddr*)&addr_in, sizeof(addr_in));
        if (ret == -1)
        {
            int err = WSAGetLastError();
            if (err != WSAEWOULDBLOCK)
            {
                Close();
                return false;
            }
        }
        m_nState = STATE_CONNECT;
    }
    ASSERT(m_nState == STATE_CONNECT);
    // 重复打开
    timeval timeout;
    timeout.tv_sec	= nBlockSecs;
    timeout.tv_usec	= 0;
    fd_set writeset, exceptset;
    FD_ZERO(&writeset);
    FD_ZERO(&exceptset);
    FD_SET(m_sock, &writeset);
    FD_SET(m_sock, &exceptset);
    int ret = select(FD_SETSIZE, NULL, &writeset, &exceptset, &timeout);
    if (ret > 0 && FD_ISSET(m_sock, &exceptset))
    {
        Close();
        return false;
    }
    else if (ret > 0 && FD_ISSET(m_sock, &writeset))
    {
        m_nState	= STATE_OPEN;
        return true;
    }
    return false;
}

/////////////////////////////////////////////////////////////////////////////////
bool CClientSocket::Open(int nBlockSecs /*= 0*/)
{
    ASSERT(m_szIP[0]);
    if (m_nState == STATE_OPEN)
    {
        return true;
    }
    if (m_nState == STATE_CLOSED)
    {
        ASSERT(m_sock == INVALID_SOCKET);
#ifdef	ENCRYPT
        m_cSendEncrypt.Init();
        m_cRecvEncrypt.Init();
#endif
        // 创建套接字
        m_sock = socket(PF_INET, SOCK_STREAM, 0);
        if (m_sock == INVALID_SOCKET)
        {
            LOGERROR("Init socket() failed.");
            return false;
        }
        // 初始化
        m_nLen		= 0;
        // 设置SOCKET为KEEPALIVE
        int		optval = 1;
        if (setsockopt(m_sock, SOL_SOCKET, SO_KEEPALIVE, (char*) &optval, sizeof(optval)))
        {
            closesocket(m_sock);
            LOGERROR("setsockopt() set SO_KEEPALIVE failed.");
            return false;
        }
        // 设置SENDBUG
        optval = m_nSndSize;
        if (m_nSndSize && setsockopt(m_sock, SOL_SOCKET, SO_SNDBUF, (char*) &optval, sizeof(optval)))
        {
            int err = WSAGetLastError();
            LOGERROR("setsockopt() set SO_SNDBUF failed[%d].", err);
            Close();
            return false;
        }
        // 设置RECVBUG
        optval = m_nRcvSize;
        if (m_nRcvSize && setsockopt(m_sock, SOL_SOCKET, SO_RCVBUF, (char*) &optval, sizeof(optval)))
        {
            int err = WSAGetLastError();
            LOGERROR("setsockopt() set SO_RCVBUF failed[%d].", err);
            Close();
            return false;
        }
        // 设置为非阻塞方式
        unsigned long	i = 1;
        if (ioctlsocket(m_sock, FIONBIO, &i))
        {
            LOGERROR("Init ioctlsocket() failed.");
            Close();
            return false;
        }
        // 域名->IP地址
        UINT	nAddr = inet_addr(m_szIP);			// 必须为 UINT, 以便与in_addr兼容
        if (nAddr == INADDR_NONE)
        {
            hostent*	 hp;
            hp = gethostbyname(m_szIP);
            if (hp == 0)
            {
                LOGERROR("帐号服务器的域名错.");
                Close();
                return false;
            }
            nAddr = *(UINT*)(hp->h_addr_list[0]);	// 或 h_addr
        }
        sockaddr_in	addr_in;
        memset((void*)&addr_in, sizeof(addr_in), 0);
        addr_in.sin_family = AF_INET;
        addr_in.sin_port = htons(m_nPort);
        addr_in.sin_addr.s_addr = nAddr;
        int ret = connect(m_sock, (sockaddr*)&addr_in, sizeof(addr_in));
        if (ret == -1)
        {
            int err = WSAGetLastError();
            if (err != WSAEWOULDBLOCK)
            {
                Close();
                return false;
            }
        }
        m_nState = STATE_CONNECT;
    }
    ASSERT(m_nState == STATE_CONNECT);
    // 重复打开
    timeval timeout;
    timeout.tv_sec	= nBlockSecs;
    timeout.tv_usec	= 0;
    fd_set writeset, exceptset;
    FD_ZERO(&writeset);
    FD_ZERO(&exceptset);
    FD_SET(m_sock, &writeset);
    FD_SET(m_sock, &exceptset);
    int ret = select(FD_SETSIZE, NULL, &writeset, &exceptset, &timeout);
    if (ret > 0 && FD_ISSET(m_sock, &exceptset))
    {
        Close();
        return false;
    }
    else if (ret > 0 && FD_ISSET(m_sock, &writeset))
    {
        m_nState	= STATE_OPEN;
        return true;
    }
    return false;
}

/////////////////////////////////////////////////////////////////////////////////
const char* CClientSocket::GetPacket(int* pBufSize, bool bFromNet /*= true*/)
{
    CHECKF(pBufSize);
    *pBufSize = 0;
    if (m_nState != STATE_OPEN)
    {
        return NULL;
    }
    if (bFromNet && m_sock != INVALID_SOCKET && m_nLen < RECV_BUFFER_SIZE)
    {
        int ret = recv(m_sock, m_bufMsg + m_nLen, RECV_BUFFER_SIZE - m_nLen, 0);
        if (ret > 0)
        {
#ifdef	ENCRYPT
            // 消息解密
            //			if (a1 || b1 || c1 || fst1 || a2 || b2 || c2 || fst2)
            m_cRecvEncrypt.Encrypt((unsigned char*)m_bufMsg + m_nLen, ret);
#endif
            m_nLen += ret;
        }
        else if (ret == 0)
        {
            Close();
        }
        else if (ret < 0)
        {
            int err = WSAGetLastError();
            if (err != WSAEWOULDBLOCK)
            {
                Close();
            }
        }
    }
    *pBufSize	= m_nLen;
    return m_bufMsg;
}

/////////////////////////////////////////////////////////////////////////////////////////
bool CClientSocket::ClearPacket	(int nLen)
{
    ASSERT(nLen <= m_nLen);
    IF_NOT (m_nState == STATE_OPEN)
    return false;
    if (m_nLen - nLen > 0)
    {
        memcpy(m_bufMsg, m_bufMsg + nLen, m_nLen - nLen);
    }
    m_nLen	-= nLen;
    IF_NOT (m_nLen >= 0)
    {
        m_nLen = 0;
        return false;
    }
    return true;
}

/////////////////////////////////////////////////////////////////////////////////
void CClientSocket::Close()
{
    if (m_nState != STATE_CLOSED)
    {
        if (m_sock != INVALID_SOCKET)
        {
            closesocket(m_sock);
        }
        m_sock		= INVALID_SOCKET;
        m_nState	= STATE_CLOSED;
    }
}

/////////////////////////////////////////////////////////////////////////////////
bool CClientSocket::SendPacket(const char* buf, int nLen, bool bFlush /*= false*/)
{
    if (m_nState != STATE_OPEN)
    {
        return false;
    }
    if (nLen + m_nSendLen > SEND_BUFFER_SIZE)
    {
        Flush();
    }
    if (nLen + m_nSendLen > SEND_BUFFER_SIZE)
    {
        LOGERROR("SOCKET溢出!");
        Close();
        return false;
    }
    memcpy(m_bufSendMsg + m_nSendLen, buf, nLen);
#ifdef	ENCRYPT
    // 消息加密
    //		if (a1 || b1 || c1 || fst1 || a2 || b2 || c2 || fst2)
    m_cSendEncrypt.Encrypt((unsigned char*)m_bufSendMsg + m_nSendLen, nLen);
#endif
    m_nSendLen	+= nLen;
    if (bFlush)
    {
        Flush();
    }
    return true;
}

/////////////////////////////////////////////////////////////////////////////////
bool CClientSocket::Flush()				// return false : no empty
{
    if (m_nState != STATE_OPEN)
    {
        return false;
    }
    if (!m_nSendLen)
    {
        return true;
    }
    int ret = send(m_sock, m_bufSendMsg, m_nSendLen, 0);
    if (ret > 0)
    {
        if (m_nSendLen - ret > 0)
        {
            memcpy(m_bufSendMsg, m_bufSendMsg + ret, m_nSendLen - ret);
        }
        m_nSendLen	-= ret;
        ASSERT(m_nSendLen >= 0);
        return !m_nSendLen;
    }
    else // ret < 0
    {
        LOGERROR("SOCKET BUF 溢出!");
        Close();
        return false;
    }
}

/////////////////////////////////////////////////////////////////////////////////
bool CClientSocket::IsOpen()
{
    //	if (m_sock == INVALID_SOCKET)
    //		return false;
    return (m_nState == STATE_OPEN);
}




