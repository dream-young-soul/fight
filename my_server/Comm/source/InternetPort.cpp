
//**********************************************************
// 代码编辑器
//**********************************************************

// 接口类。接口：基于消息通讯机制的网络间通讯接口。
// 仙剑修，2002.9.6

#include "InternetPort.h"

using namespace message_port;

///////////////////////////////////////////////////////////////////////////////////////
// construct
///////////////////////////////////////////////////////////////////////////////////////
CInternetPort::CInternetPort(int nPort)
{
    m_nPortSize	= 0;
    m_id		= nPort;
    m_nState	= STATE_CLOSED;
    m_sockMain	= INVALID_SOCKET;
    m_szKey[0]	= 0;
    m_bWinStartup	= false;
}

///////////////////////////////////////////////////////////////////////////////////////
// interface
///////////////////////////////////////////////////////////////////////////////////////
// 初始化，设置接口ID号，开始接收消息。可重复调用(PORT_ID不能改变)。客户端会等待CONNECT_OVERTIMESECS秒
bool CInternetPort::Open	()
{
    if (IsOpen())
    {
        return true;
    }
    if (m_id == 0)		// 第一号接口为服务端，其它接口为客户端
    {
        m_nState = STATE_CLOSED;
        m_sockMain = CreateServerSocket();
        if (m_sockMain == INVALID_SOCKET)
        {
            return false;
        }
    }
    else
    {
        if (m_nState != STATE_OPENING)
        {
            m_nState = STATE_CLOSED;
            m_sockMain = CreateClientSocket();
            if (m_sockMain == INVALID_SOCKET)
            {
                return false;
            }
            m_nState = STATE_OPENING;
        }
        timeval timeout;
        timeout.tv_sec		= CONNECT_OVERTIMESECS / 1000;
        timeout.tv_usec		= (CONNECT_OVERTIMESECS % 1000) * 1000;
        fd_set readset, writeset, exceptset;
        FD_ZERO(&readset);
        FD_ZERO(&writeset);
        FD_ZERO(&exceptset);
        FD_SET(m_sockMain, &readset);
        FD_SET(m_sockMain, &writeset);
        FD_SET(m_sockMain, &exceptset);
        int ret = select(FD_SETSIZE, NULL, &writeset, &exceptset, &timeout);
        if (ret == 0)
        {
            return false;
        }
        else if (ret < 0 || FD_ISSET(m_sockMain, &exceptset))
        {
            Close();
            return false;
        }
        for(int i = 0; i < m_nPortSize; i++)
        {
            m_setSocket[i] = m_sockMain;
        }
        // 节点登录，通知主节点
        m_nState	= STATE_OK;			// 连接成功，以便下面SEND()
        Send(MASTER_PORT_ID, SYS_PACKET_LOGINKEY, STRING_TYPE(m_szKey), m_szKey);
        Send(MASTER_PORT_ID, SYS_PACKET_PORTID, VARTYPE_INT, &m_id);
    }
    m_nState	= STATE_OK;
    for(int i = 0; i < m_setRecvBuf.size(); i++)
    {
        m_setRecvBuf[i].Reset();
    }
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////
SOCKET CInternetPort::CreateServerSocket()
{
    // 创建主套接字
    SOCKET	sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
    {
        int err = WSAGetLastError();
        LOGERROR("socket() failed[%d].", err);
        return INVALID_SOCKET;
    }
    // 设置SOCKET的REUSEADDR
    int		optval = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*) &optval, sizeof(optval)))
    {
        int err = WSAGetLastError();
        closesocket(sock);
        LOGERROR("setsockopt(SO_REUSEADDR) failed[%d].", err);
        return INVALID_SOCKET;
    }
    // 设置SOCKET的KEEPALIVE
    optval = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (char*) &optval, sizeof(optval)))
    {
        int err = WSAGetLastError();
        closesocket(sock);
        LOGERROR("setsockopt(SO_KEEPALIVE) failed[%d].", err);
        return INVALID_SOCKET;
    }
    // 设置SOCKET的SNDBUF
    optval = SNDBUF_SIZE;
    if (setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (char*) &optval, sizeof(optval)))
    {
        int err = WSAGetLastError();
        closesocket(sock);
        LOGERROR("setsockopt(SO_SNDBUF) failed[%d].", err);
        return INVALID_SOCKET;
    }
    /*/ 读取SOCKET的SNDBUF
    int ret = sizeof(optval);
    if (getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (char *) &optval, &ret) == 0)
    {
    	LOGERROR("message: 服务SOCKET内部发送缓冲区尺寸为[%d]K.", optval/1024);
    }//*/
    // 绑定
    SOCKADDR_IN		sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family		= AF_INET;
    sin.sin_addr.s_addr	= 0;
    sin.sin_port		= htons(m_nMasterPort);
    if (bind(sock, (LPSOCKADDR) &sin, sizeof(sin)))
    {
        int err = WSAGetLastError();
        closesocket(sock);
        LOGERROR("bind() failed[%d].", err);
        return INVALID_SOCKET;
    }
    // 设置为非阻塞方式
    unsigned long	a = 1;
    if (ioctlsocket(sock, FIONBIO, &a))
    {
        int err = WSAGetLastError();
        closesocket(sock);
        LOGERROR("ioctlsocket() failed[%d].", err);
        return INVALID_SOCKET;
    }
    // 监听端口
    if (listen(sock, SOMAXCONN))		// SOMAXCONN: WIN的宏定义
    {
        int err = WSAGetLastError();
        closesocket(sock);
        LOGERROR("listen() failed[%d].", err);
        return INVALID_SOCKET;
    }
    return sock;
}

///////////////////////////////////////////////////////////////////////////////////////
SOCKET CInternetPort::CreateClientSocket()
{
    // 创建套接字
    SOCKET	sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
    {
        int err = WSAGetLastError();
        LOGERROR("socket() failed[%d].", err);
        return INVALID_SOCKET;
    }
    // 设置SOCKET为KEEPALIVE
    int		optval = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (char*) &optval, sizeof(optval)))
    {
        int err = WSAGetLastError();
        LOGERROR("setsockopt(SO_KEEPALIVE) failed[%d].", err);
        closesocket(sock);
        return INVALID_SOCKET;
    }
    // 设置SENDBUF
    optval = SNDBUF_SIZE;
    if (setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (char*) &optval, sizeof(optval)))
    {
        int err = WSAGetLastError();
        LOGERROR("setsockopt(SO_SNDBUF) failed[%d].", err);
        closesocket(sock);
        return INVALID_SOCKET;
    }
    /*/ 读取SOCKET的SNDBUF
    int ret = sizeof(optval);
    if (getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (char *) &optval, &ret) == 0)
    {
    	if (!m_tOpen)	// 首次
    		LOGMSG("帐号SOCKET内部发送缓冲区尺寸为[%d]K.", optval/1024);
    }//*/
    // 设置为非阻塞方式
    unsigned long	a = 1;
    if (ioctlsocket(sock, FIONBIO, &a))
    {
        int err = WSAGetLastError();
        LOGERROR("ioctlsocket() failed[%d].", err);
        closesocket(sock);
        return INVALID_SOCKET;
    }
    // 域名->IP地址
    UINT	nAddr = inet_addr(m_szMasterIP);			// 必须为 UINT, 以便与in_addr兼容
    if (nAddr == INADDR_NONE)
    {
        hostent*	 hp;
        hp = gethostbyname(m_szMasterIP);
        if (hp == 0)
        {
            int	err = WSAGetLastError();
            LOGERROR("Invalid ip addr[%s], err[%d].", m_szMasterIP, err);
            closesocket(sock);
            return INVALID_SOCKET;
        }
        nAddr = inet_addr(hp->h_addr_list[0]);	// 或 h_addr。自动取第一网卡的IP。
    }
    sockaddr_in	addr_in;
    memset((void*)&addr_in, sizeof(addr_in), 0);
    addr_in.sin_family = AF_INET;
    addr_in.sin_port = htons(m_nMasterPort);
    addr_in.sin_addr.s_addr = nAddr;
    if (connect(sock, (sockaddr*)&addr_in, sizeof(addr_in)) == SOCKET_ERROR)
    {
        int err = WSAGetLastError();
        if (err != WSAEWOULDBLOCK)
        {
            LOGERROR("Connect() failed[%d].", err);
            closesocket(sock);
            return INVALID_SOCKET;
        }
        else // if (err == WSAEWOULDBLOCK)
        {
            return sock;
        }
    } // connect
    return sock;
}

///////////////////////////////////////////////////////////////////////////////////////
// 发送消息到指定接口。包含消息ID、数据类型、数据。
bool CInternetPort::Send	(int nPort, int nPacket, VAR_TYPE nVarType, const void* buf)
{
    if (!IsOpen())
    {
        return false;
    }
    if (nPort == INVALID_PORT)
    {
        return true;
    }
    return SendFrom(m_id, nPort, nPacket, nVarType, buf);
}

///////////////////////////////////////////////////////////////////////////////////////
// 发送消息到指定接口。包含消息ID、数据类型、数据。
bool CInternetPort::SendFrom(int nPortFrom, int nPort, int nPacket, VAR_TYPE nVarType, const void* buf)
{
    CHECKF(buf);
    if (nPort < 0 || nPort >= m_nPortSize)
    {
        ASSERT(!"nPort");
        return false;
    }
    if (nPort == m_id)
    {
        PushMsg(m_id, nPacket, nVarType, buf);
        return true;
    }
    if (m_setSocket[nPort] == INVALID_SOCKET)
    {
        LOGERROR("port[%d] send(%d, %d, %d, %s) failed.", m_id, nPort, nPacket, nVarType, buf);
        return false;
    }
    int		nLen = MSG_HEAD_SIZE;
    CMessageHead	cMsgHead;
    char*	pBuf = (char*)&cMsgHead;
    if (m_id == MASTER_PORT_ID)
    {
        cMsgHead.m_nPort	= nPortFrom;    //? m_nPort: 主节点上传到子节点时，表示SOURCE_PORT
    }
    else
    {
        cMsgHead.m_nPort	= nPort;    //? m_nPort: 子节点上传到主节点时，表示TARGET_PORT
    }
    cMsgHead.m_nPacket		= nPacket;
    cMsgHead.m_nVarType		= nVarType;
    int ret = send(m_setSocket[nPort], pBuf, nLen, 0);		// 发送HEAD数据
    if (ret == nLen)
    {
        nLen = SIZE_OF_TYPE(nVarType);
        pBuf = (char*)buf;
        ret = send(m_setSocket[nPort], pBuf, nLen, 0);		// 发送DATA数据
        if (ret == nLen)
        {
            return true;
        }
    }
    int err = WSAGetLastError();
    if (ret > 0 || err == WSAEWOULDBLOCK)
    {
        LOGERROR("port[%d] overflow.", m_id);
    }
    else
    {
        LOGERROR("port[%d] error[%d].", m_id, err);
    }
    if (m_id == 0)
    {
        closesocket(m_setSocket[nPort]);
        m_setSocket[nPort]	= INVALID_SOCKET;
    }
    else
    {
        Close();
        Open();			// 尽快重新打开
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////////////
// 接收指定接口(或所有接口)发来的消息。可指定消息ID，也可不指定。
bool CInternetPort::Recv	(int nPort, int nPacket, VAR_TYPE nVarType, void* buf, CMessageStatus* pStatus /*= NULL*/)	// return false: 没数据
{
    CHECKF(buf);
    if (!IsOpen())
    {
        if (pStatus)
        {
            pStatus->m_nError	= STATUS_FLAG_CLOSE;
        }
        return false;
    }
    RecvMsg(0);
    // 处理内部消息
    int		nRcvPort = nPort, nRcvPacket = nPacket;
    VAR_TYPE	nRcvVarType;
    if (PopMsg(&nRcvPort, &nRcvPacket, &nRcvVarType, buf))		// 内部函数，不用打开互斥锁
    {
        // 检查类型
        if (SIZE_OF_TYPE(nRcvVarType) > SIZE_OF_TYPE(nVarType))
        {
            ASSERT(!"VarType");
            if (pStatus)
            {
                pStatus->m_nError		= STATUS_FLAG_ERROR;    //? 以后可支持自动转换类型
            }
            return false;
        }
        if (pStatus)
        {
            pStatus->m_nPortFrom	= nRcvPort;
            pStatus->m_nPacket		= nRcvPacket;
            pStatus->m_nVarType		= nRcvVarType;
            pStatus->m_nError		= STATUS_FLAG_OK;
        }
        return true;
    }
    if (pStatus)
    {
        if (IsOpen())
        {
            pStatus->m_nError	= STATUS_FLAG_OK;
        }
        else
        {
            pStatus->m_nError	= STATUS_FLAG_ERROR;
        }
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////////////
bool CInternetPort::RecvMsg(int nMilliSec)
{
    if (m_nState == STATE_CLOSED)
    {
        return false;
    }
    // 接收SOCKET消息
    timeval timeout;
    timeout.tv_sec		= nMilliSec / 1000;
    timeout.tv_usec		= (nMilliSec % 1000) * 1000;
    fd_set readset;
    FD_ZERO(&readset);
    if (m_sockMain != INVALID_SOCKET)
    {
        FD_SET(m_sockMain, &readset);
    }
    if (m_id == 0)
    {
        for(int i = 0; i < m_setSocket.size(); i++)
        {
            if (m_setSocket[i] != INVALID_SOCKET)
            {
                FD_SET(m_setSocket[i], &readset);
            }
        }
    }
    int ret = select(FD_SETSIZE, &readset, NULL, NULL, &timeout);
    if (ret > 0)
    {
        if (m_id == 0)
        {
            // 服务端。处理ACCEPT
            if (FD_ISSET(m_sockMain, &readset))
            {
                struct sockaddr_in	addr;
                memset(&addr, 0, sizeof(addr));
                int	   len = sizeof(addr);
                SOCKET newsock = accept(m_sockMain, (sockaddr*) &addr, (int*) &len);
                if (newsock != INVALID_SOCKET)
                {
                    m_setSocket.push_back(newsock);
                    m_setRecvBuf.push_back(CRecvBuffer());
                }
            }
            for(int i = m_setSocket.size() - 1; i >= 0; i--)		// 要删除，倒排
            {
                if (m_setSocket[i] == INVALID_SOCKET || !FD_ISSET(m_setSocket[i], &readset))
                {
                    continue;
                }
                if (RecvToBuf(&m_setRecvBuf[i], m_setSocket[i]))
                {
                    BufferToList(i);				//? 对于登录消息，SOCKET可能前移
                }
                else // error
                {
                    closesocket(m_setSocket[i]);
                    m_setSocket[i]	= INVALID_SOCKET;
                    if (i >= m_nPortSize)
                    {
                        m_setSocket.erase(m_setSocket.begin() + i);		// 要删除，倒排
                        m_setRecvBuf.erase(m_setRecvBuf.begin() + i);		// 要删除，倒排
                    }
                }
            } // for all socket
        } // 服务端
        else
        {
            // 客户端
            if (RecvToBuf(&m_setRecvBuf[0], m_sockMain))
            {
                BufferToList(0);
            }
            else
            {
                Close();
                Open();			// 尽快重新打开
            }
        }
    } // select
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////
bool CInternetPort::RecvToBuf(CRecvBuffer* pRcvBuf, SOCKET sock)		// return false: error
{
    char*	pBuf = pRcvBuf->GetTail();
    int		nLen = message_port::RECV_BUFFER_SIZE - pRcvBuf->GetLen();
    int	ret = recv(sock, pBuf, nLen, 0);		// 接收数据
    if (ret > 0)
    {
        pRcvBuf->AddLen(ret);
        return true;
    }
    if (ret == 0)
    {
        LOGERROR("message: socket close.");
    }
    else if (ret < 0)
    {
        int err = WSAGetLastError();
        if (err == WSAEWOULDBLOCK)
        {
            return true;
        }
        LOGERROR("message: socket error[%d], recv() failed.", err);
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////////////
bool CInternetPort::BufferToList(int idx)		// return false: 无消息
{
    if (m_id == 0)
    {
        while(true)
        {
            // 处理消息
            if (m_setRecvBuf[idx].GetLen() < MSG_HEAD_SIZE)
            {
                break;
            }
            CMessageHead*	pHead = m_setRecvBuf[idx].GetHead();
            int nLen = SIZE_OF_TYPE(pHead->m_nVarType);
            if (m_setRecvBuf[idx].GetLen() < MSG_HEAD_SIZE + nLen)
            {
                break;
            }
            if (pHead->m_nPacket >= SYS_PACKET_BASE)
            {
                if (pHead->m_nPacket == SYS_PACKET_LOGINKEY)			// 处理节点验证
                {
                    LPCTSTR	pszKey = (LPCTSTR)pHead->m_bufData;
                    IF_NOT(strlen(m_szKey) && strcmp(pszKey, m_szKey) == 0)
                    {
                        closesocket(m_setSocket[idx]);
                        //??? 未强制检查KEY
                    }
                }
                else if (pHead->m_nPacket == SYS_PACKET_PORTID)			// 处理节点登录
                {
                    IF_OK(pHead->m_nVarType == VARTYPE_INT)
                    {
                        int	nLoginPort = *((int*)pHead->m_bufData);
                        IF_OK(nLoginPort > MASTER_PORT_ID && nLoginPort < m_nPortSize		// >MASTER_PORT_ID : 不能为主节点号
                              && idx >= m_nPortSize)
                        {
                            if (m_setSocket[nLoginPort] != INVALID_SOCKET)
                            {
                                // 双重登录，关闭旧SOCKET
                                closesocket(m_setSocket[nLoginPort]);
                            }
                            m_setSocket[nLoginPort]		= m_setSocket[idx];
                            m_setRecvBuf[nLoginPort]	= m_setRecvBuf[idx];
                            m_setSocket.erase(m_setSocket.begin() + idx);		// 要删除，倒排
                            m_setRecvBuf.erase(m_setRecvBuf.begin() + idx);
                            // 移动idx
                            idx		= nLoginPort;
                        }
                    }
                    SendFrom(m_id, idx, SYS_PACKET_PORTSIZE, VARTYPE_INT, &m_nPortSize);
                }
                else
                {
                    ASSERT(!"Unknown system packet!");
                }
            }
            else // not system packet
            {
                if (pHead->m_nPort != MASTER_PORT_ID)		//? m_nPort: 子节点上传到主节点时，表示TARGET_PORT
                {
                    // 处理转发消息
                    IF_OK(idx < m_nPortSize)
                    SendFrom(idx, pHead->m_nPort, pHead->m_nPacket, (VAR_TYPE)pHead->m_nVarType, pHead->m_bufData);		// idx == sub_port_id
                }
                else
                {
                    // 存消息
                    IF_OK(idx < m_nPortSize)
                    PushMsg(idx, pHead->m_nPacket, (VAR_TYPE)pHead->m_nVarType, pHead->m_bufData);		// idx == sub_port_id
                }
            }
            // 清除BUFFER中的消息
            m_setRecvBuf[idx].DelMsg(MSG_HEAD_SIZE + nLen);
        } // while
    }
    else // 子节点
    {
        while(true)
        {
            // 处理消息
            if (m_setRecvBuf[0].GetLen() < MSG_HEAD_SIZE)
            {
                break;
            }
            CMessageHead*	pHead = m_setRecvBuf[0].GetHead();
            int nLen = SIZE_OF_TYPE(pHead->m_nVarType);
            if (m_setRecvBuf[0].GetLen() < MSG_HEAD_SIZE + nLen)
            {
                break;
            }
            if (pHead->m_nPacket == SYS_PACKET_PORTSIZE)	// 处理节点尺寸
            {
                IF_OK(pHead->m_nVarType == VARTYPE_INT)
                {
                    int	nPortSize = *((int*)pHead->m_bufData);
                    IF_OK(idx == MASTER_PORT_ID)
                    {
                        if (nPortSize != m_nPortSize)
                        {
                            m_nPortSize = nPortSize;		//? reset size!!!
                            m_setSocket.resize(m_nPortSize, m_sockMain);
                        }
                    }
                }
            }
            else
            {
                // 存消息
                PushMsg(pHead->m_nPort, pHead->m_nPacket, (VAR_TYPE)pHead->m_nVarType, pHead->m_bufData);
            }
            // 清除BUFFER中的消息
            m_setRecvBuf[0].DelMsg(MSG_HEAD_SIZE + nLen);
        }
    }
    return (m_setMsg.size() != 0);
}

///////////////////////////////////////////////////////////////////////////////////////
bool CInternetPort::WaitMsg(int nMilliSec)
{
    if (!IsOpen())
    {
        return false;
    }
    if (m_setMsg.size())
    {
        return true;
    }
    RecvMsg(nMilliSec);
    return	(m_setMsg.size() != 0);
}

///////////////////////////////////////////////////////////////////////////////////////
// 关闭接口，不再接收消息。可重复调用。
bool CInternetPort::Close()
{
    if (m_nState == STATE_CLOSED)
    {
        return true;
    }
    m_nState = STATE_CLOSED;
    if (m_id == 0)		// 第一号接口为服务端，其它接口为客户端
    {
        // 清空消息串
        for(MSG_SET::iterator iter = m_setMsg.begin(); iter != m_setMsg.end(); iter++)
        {
            delete(*iter);
        }
        m_setMsg.clear();
        // 清空BUFFER
        m_setRecvBuf.resize(m_nPortSize);
        for(int i = 0; i < m_setRecvBuf.size(); i++)
        {
            m_setRecvBuf[i].Reset();
        }
        // 关闭主SOCKET
        if (m_sockMain != INVALID_SOCKET)
        {
            closesocket(m_sockMain);
            m_sockMain	= INVALID_SOCKET;
        }
        // 关闭子SOCKET
        m_setSocket.resize(m_nPortSize);
        for(int i = 0; i < m_setSocket.size(); i++)
        {
            if (m_setSocket[i] != INVALID_SOCKET)
            {
                closesocket(m_setSocket[i]);
                m_setSocket[i]	= INVALID_SOCKET;
            }
        }
    }
    else
    {
        // 清空消息串
        for(MSG_SET::iterator iter = m_setMsg.begin(); iter != m_setMsg.end(); iter++)
        {
            delete(*iter);
        }
        m_setMsg.clear();
        // 清空BUFFER
        m_setRecvBuf[0].Reset();
        // 关闭主SOCKET
        if (m_sockMain != INVALID_SOCKET)
        {
            closesocket(m_sockMain);
            m_sockMain	= INVALID_SOCKET;
        }
        // 清空SOCKET表
        for(int i = 0; i < m_setSocket.size(); i++)
        {
            m_setSocket[i] = INVALID_SOCKET;
        }
    }
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////
// CInternetPort
///////////////////////////////////////////////////////////////////////////////////////
int CInternetPort::SIZE_OF_TYPE		(int type)
{
    const int	fixlen_type_size = 10;
    const int	size_of[fixlen_type_size] = {1, 1, 2, 2, 4, 4, 4, 4, 4, 8};
    if (type < VARTYPE_NONE)
    {
        return type;
    }
    else if (type == VARTYPE_NONE)
    {
        ASSERT(!"SIZE_OF_TYPE");
        return 0;
    }
    else if (type - (VARTYPE_NONE + 1) < fixlen_type_size)
    {
        return size_of[type - (VARTYPE_NONE+1)];
    }
    ASSERT(!"SIZE_OF_TYPE.");
    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////
bool CInternetPort::PushMsg(int nPort, int nPacket, VAR_TYPE nVarType, const void* buf)	// nData中的串和结构都会被COPY
{
    CHECKF(buf);
    if (m_nState != STATE_OK)		// Close()后，不接收消息
    {
        return false;
    }
    if (nPort < 0 || nPort >= m_nPortSize)
    {
        ASSERT(!"PushMsg(nPort)");
        return false;
    }
    int	nSize = SIZE_OF_TYPE(nVarType);
    if (nSize == 0 || nSize > MAX_MSGPACKSIZE)
    {
        ASSERT(!"PushMsg(nSize)");
        return false;
    }
    CMessagePacket*	pMsg = new CMessagePacket;		// VVVVVVVVVVVVVVVVVVV
    if (!pMsg)
    {
        ASSERT(!"new");
        return false;
    }
    pMsg->m_nPortFrom		= nPort;
    pMsg->m_nPacket			= nPacket;
    pMsg->m_nVarType		= nVarType;
    memcpy(pMsg->m_bufData, buf, nSize);
    m_setMsg.push_back(pMsg);		// AAAAAAAAAAAAAAAAAAAAAAAA
    // 检查溢出
    if ((m_setMsg.size() % 100) == 0)		//? 100报警
    {
        LOGERROR("CInternetPacket[%d]接口的数量达到[%u]", m_id, m_setMsg.size());
    }
    if (m_setMsg.size() > 10000)
    {
        LOGERROR("ERROR: 消息堆溢出，InternetPort[%d]关闭!", m_id);
        Close();
    }
    return m_setMsg.size() <= 1000;		//? 1000出错
}

///////////////////////////////////////////////////////////////////////////////////////
bool CInternetPort::PopMsg(int* pPort, int* pPacket, VAR_TYPE* pVarType, void* buf)
{
    CHECKF(pPort && pPacket && pVarType && buf);
    if (m_nState != STATE_OK)		// Close()后，不处理消息
    {
        return false;
    }
    if (m_setMsg.size() == 0)
    {
        return false;
    }
    for(MSG_SET::iterator iter = m_setMsg.begin(); iter != m_setMsg.end(); iter++)
    {
        CMessagePacket* pMsg = *iter;
        if ((*pPort == PORT_ANY || *pPort == pMsg->m_nPortFrom) && (*pPacket == PACKET_ANY || *pPacket == pMsg->m_nPacket))
        {
            *pPort		= pMsg->m_nPortFrom;
            *pPacket	= pMsg->m_nPacket;
            *pVarType	= (VAR_TYPE)pMsg->m_nVarType;
            int	nSize = SIZE_OF_TYPE(*pVarType);
            if (nSize)
            {
                memcpy(buf, pMsg->m_bufData, nSize);
            }
            delete(pMsg);
            m_setMsg.erase(iter);
            return true;
        }
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////////////
bool CInternetPort::Init()
{
    m_setSocket.resize(m_nPortSize, INVALID_SOCKET);
    if (m_id == MASTER_PORT_ID)
    {
        for(int i = 0; i < m_nPortSize; i++)
        {
            m_setRecvBuf.push_back(CRecvBuffer());
        }
    }
    else
    {
        m_setRecvBuf.push_back(CRecvBuffer());
    }
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////
void CInternetPort::Clear()
{
    Close();
    for(MSG_SET::iterator iter = m_setMsg.begin(); iter != m_setMsg.end(); iter++)
    {
        delete(*iter);
    }
    m_setMsg.clear();
}

///////////////////////////////////////////////////////////////////////////////////////
// static
///////////////////////////////////////////////////////////////////////////////////////
bool CInternetPort::InitPortSet(int nPortSize, LPCTSTR pszMasterIP, int nMasterPort)
{
    if (!pszMasterIP || pszMasterIP[0] == 0 || strlen(pszMasterIP) >= IPSTR_SIZE)
    {
        return false;
    }
    if (nPortSize <= m_id)
    {
        nPortSize = m_id + 1;    // 容错
    }
    if (nPortSize < 1 || nPortSize > MAX_PORT_SIZE)
    {
        ASSERT(!"网络节点数错误! 请修改FD_SETSIZE值并重新编译程序。");
        return false;
    }
    m_nPortSize	= nPortSize;
    strcpy(m_szMasterIP, pszMasterIP);
    m_nMasterPort	= nMasterPort;
    // 初始化网络
    WSADATA		wsaData;
    int	err;
    if ((err = WSAStartup(SOCKET_VERSION, &wsaData)) != 0)
    {
        LOGERROR("WSAStartup failed[%d].", err);
        return false;
    }
    m_bWinStartup = true;
    // 检查版本
    if (wsaData.wVersion != 0x0002)
    {
        WSACleanup();
        LOGERROR("wsaData version unmatch[0x%04X].", wsaData.wVersion);
        return false;
    }
    Init();
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////
void CInternetPort::ClearPortSet()
{
    Clear();
    if (m_bWinStartup)
    {
        WSACleanup();
    }
}

///////////////////////////////////////////////////////////////////////////////////////
IMessagePort* CInternetPort::CreateNew(int nPortID, int nPortSize, LPCTSTR szMasterIP, int nMasterPort, LPCTSTR szKey)
{
    CHECKF(szMasterIP && szKey);
    CHECKF(nPortID >= 0 && nPortSize >= 0 && nMasterPort >= 0);
    if (nPortID == 0 && nPortSize == 0 || nPortID != 0 && strlen(szMasterIP) == 0)
    {
        return NULL;
    }
    CInternetPort* pPort = new CInternetPort(nPortID);
    if (pPort)
    {
        if (pPort->InitPortSet(nPortSize, szMasterIP, nMasterPort))
        {
            SafeCopy(pPort->m_szKey, szKey, MAX_KEYSIZE);
            return (IMessagePort*)pPort;
        }
        delete pPort;
    }
    return NULL;
}














