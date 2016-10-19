// 服务线程部分
// 仙剑修, 2001.10.10

//#include "allheads.h"


//	2、CServerSocket：该类用于服务线程中创建局域变量。
/*
CServerSocket<key1, key2>::CServerSocket()
{
	m_bufPeerIP[0]	= 0;
	m_sockServer = INVALID_SOCKET;

	m_bSendEnable	= false;
	m_bState		= false;
}
//else*/
template <unsigned long key1, unsigned long key2>
CServerSocket<key1, key2>::CServerSocket(/*unsigned long key1, unsigned long key2*/)	// 由于有监听SOCKET时初始化WSAStartup()，故该处不必调用
{
    m_sockServer	= INVALID_SOCKET;
    m_bufPeerIP[0]	= 0;
    m_tLastRecv		= 0;

//	m_bSendEnable	= false;
    m_bState		= c_stateNone;
}
//*/

template <unsigned long key1, unsigned long key2>
CServerSocket<key1, key2>::~CServerSocket()
{
    Close();
}

template <unsigned long key1, unsigned long key2>
bool CServerSocket<key1, key2>::Open(SOCKET sockServer, u_long nClientIP)
{
    if(m_bState)
    {
        LOGWARNING("CServerSocket<key1, key2>::Open()前没有CLOSE");
        Close(true);
    }

    if(sockServer == INVALID_SOCKET)
    {
        LOGERROR("CServerSocket<key1, key2>::Open()参数错误，SOCKET号无效");
        return	false;
    }

    if(m_sockServer != INVALID_SOCKET)
    {
        LOGERROR("CServerSocket<key1, key2>::Open()复盖了前一个SOCKET");
    }

    m_sockServer = sockServer;
#ifdef	ENCRYPT
    m_cEncryptRecv.Init();
    m_cEncryptSend.Init();
#endif
    m_bufPeerIP[0] = 0;
    struct in_addr	in;
    in.S_un.S_addr = nClientIP;
    char* pClientIP = inet_ntoa(in);
    if(pClientIP)
    {
        strcpy(m_bufPeerIP, pClientIP);
    }
    m_tLastRecv		= -clock();			// 负值表示连接时间

//	m_bSendEnable	= true;
    m_bState		= c_stateNormal;

//	IncConnectCount();

    return true;
}
/*
bool CServerSocket<key1, key2>::InitSocket(SOCKET sockServer)
{
	if(m_bState)
		return false;

	m_sockServer = sockServer;

	m_bSendEnable	= true;
	m_bState		= true;

	return true;
}
//*/
//*** 接收一个消息，写入pBuf所指的缓冲区中。返回false: 错误或关闭。返回true时nLen置0：没有数据
template <unsigned long key1, unsigned long key2>
bool CServerSocket<key1, key2>::Recv(char* pBuf, int& nLen)
{
    int		ret;
    u_short	nMessageSize = 0;
    nLen = 0;

    if(!m_bState)
    {
        return false;
    }

    if((ret = recv(m_sockServer, (char*)&nMessageSize, 2, MSG_PEEK)) == 2)	// 接收2字节长度
    {
#ifdef	ENCRYPT
        // 解密功能
//		SAVELOG("@@@: 未解密长度[%d].", nMessageSize);
        if(key1 || key2)
        {
            m_cEncryptRecv.Encrypt((unsigned char*)&nMessageSize, 2, false);    // false: 不移动加密指针
        }
//		SAVELOG("@@@: 已解密长度[%d].", nMessageSize);
#endif
        // 接收到2字节的消息长度
        u_long	nRcvBufLen;
        ioctlsocket(m_sockServer, FIONREAD, &nRcvBufLen);		// 检查SOCKET内部的INBUF尺寸

        // 检查消息长度
        if(!(nMessageSize > 0 && nMessageSize <= _MAX_MSGSIZE))
        {
            // 接收到一个错误消息
            LOGWARNING("接收到一个错误长度[%d]的消息。这时RCVBUF中有[%d]个字节的数据。客户端的IP地址为[%s]",
                       nMessageSize, nRcvBufLen, m_bufPeerIP);
/*
            recv(m_sockServer, pBuf, 52, 0);
            pBuf[19] = 0; pBuf[35] = 0; pBuf[51] = 0;
            LOGERROR("可能有人用明码登录服务器。登录名[%s]，口令[%s]，服务器[%s]", pBuf+4, pBuf+20, pBuf+36);
*/
            Close(true);
            return false;
        }

        if(nRcvBufLen >= nMessageSize)
        {
            // SOCKET内部有足够的数据
            int	nGotBytes	= 0;
            int	nCount		= 0;
            while(nGotBytes < nMessageSize)
            {
                ret = recv(m_sockServer, (char*)pBuf + nGotBytes, nMessageSize - nGotBytes, 0);		// 接收数据
                if(ret > 0)
                {
                    nGotBytes += ret;
                }
                else
                {
                    break;
                }

                // 防死循环
                if(++nCount > RECVREPEATTIMES)		//?? 调试变量
                {
                    LOGWARNING("系统效率严重下降，recv()函数无法正常读出数据。一个连接被强行断开。");
                    Close(true);
                    return false;
                }
            }

            // 接收到数据
            if(nGotBytes == nMessageSize)
            {
                // 检查 CPU 效率
                if(nCount >= 2)		//?? 2: 调试参数
                {
                    if(nCount >= 3)		//?? 3: 调试参数
                    {
                        LOGMSG("系统效率严重下降，recv()运行了[%d]次才取到足够的数据。", nCount);
                    }

#ifdef	LOCKACCEPT
                    // 锁定监听线程的accept()函数，5秒内不接受新玩家登录。
                    if(InterlockedExchange(&s_nLockAcceptTime, clock() + LOCKACCEPTSECOND * CLOCKS_PER_SEC) == 0)
                    {
                        LOGWARNING("系统太繁忙，已暂停接收新连接...");
                    }
#endif	// LOCKACCEPT
                }

                // 更新m_tLastRecv变量
                m_tLastRecv = clock();

                // 返回接收到的数据
                nLen = nMessageSize;
#ifdef	ENCRYPT
                // 解密功能
                if(key1 || key2)
                {
                    m_cEncryptRecv.Encrypt((unsigned char*)pBuf, nLen);
                }
//		SAVELOG("@@@: 已解密长度[%d][%d].", *(unsigned short *)pBuf, nLen);
#endif
#ifdef	DUMP
                char buf[4096] = "";
                char buf3[4];
                for(int i = 0; i < nLen; i++)
                {
                    sprintf(buf3, "%02X ", (unsigned char)pBuf[i]);
                    strcat(buf, buf3);
                    if(i % 16 == 15)
                    {
                        strcat(buf, "\n");
                    }
                }
                strcat(buf, "\n");
                LOGMSG("Send: \n%s", buf);
#endif
                return true;
            }
            else if(ret == 0)
            {
#ifdef	ALPHA_X
                LOGMSG("接收到客户端的关闭SOCKET消息。");
#endif
                Close();
                return false;	// 关闭
            }
            else if(ret < 0)
            {
                int err = WSAGetLastError();
                if(err == WSAEWOULDBLOCK)
                {
                    LOGWARNING("系统效率严重下降，recv()函数出现阻塞。一个连接被强行断开。系统有[%d]字节，", nRcvBufLen);
                    LOGWARNING("打算读入[%d]个字节，但只能读入[%d]个字节。", nMessageSize, nGotBytes);
//					nLen = 0;		// 无数据
                    return false;
                }
                else
                {
                    // 错误，或已关闭
#ifdef	ALPHA_X
                    LOGERROR("recv()错误[%d]", err);
#endif
                    Close(true);
                    return false;
                }
            }
            else
            {
                LOGERROR("CServerSocket<key1, key2>::Recv()函数异常");
                return false;
            }

            // 错误，或已关闭
//			LOGERROR("recv()错误");
//			Close(true);
//			return false;
        }
        else	// if(nRcvBufLen >= nMessageSize)
        {
            nLen = 0;		// 无数据
            return true;
        }
    }
    else if(ret > 0)
    {
        nLen = 0;		// 数据不足
        return true;
    }
    else if(ret == 0)
    {
#ifdef	ALPHA_X
        LOGMSG("接收到客户端的关闭SOCKET消息。");
#endif
        Close();
        return false;	// 关闭
    }
    else	// if(ret < 0)
    {
        int err = WSAGetLastError();
        if(err == WSAEWOULDBLOCK)
        {
            nLen = 0;		// 无数据
            return true;
        }
        else
        {
            // 错误
#ifdef	ALPHA_X
            LOGERROR("recv()错误[%d]", err);
#endif
            Close(true);
            return false;
        }
    }

    // 错误，或已关闭
//	LOGERROR("recv()错误");
//	return false;
}

template <unsigned long key1, unsigned long key2>
bool	CServerSocket<key1, key2>::Send(const char* pBuf, int nLen)	// 返回false: 网络错误
{
//		SAVELOG("@@@: 已解密长度Send[%d][%d].", *(unsigned short *)pBuf, nLen);
    if(m_bState == c_stateNone)
    {
        return false;
    }
    if(m_bState == c_stateShutDown)
    {
        return true;    //?? 数据丢弃
    }

    // 检查参数
    if(!(pBuf && nLen > 0 && nLen <= _MAX_MSGSIZE))
    {
        LOGERROR("CServerSocket<key1, key2>::Send([0x%04X], [%d])参数异常。", pBuf, nLen);
        Close(true);
        return false;
    }
    int	nMsgLen;
    try { nMsgLen = *(unsigned short*)pBuf;	}
    catch(...)
    {
        LOGCATCH("CServerSocket<key1, key2>::Send()指针参数无效。一个连接被断开。");
        Close(true);
        return false;
    }
    if(!(nMsgLen > 0 && nMsgLen <= _MAX_MSGSIZE && nMsgLen == nLen))		// 检查通讯消息完整性
    {
        LOGERROR("内核SEND的一个消息的长度值[%d]错误，或与消息长度[%d]不符。一个连接被断开，", nMsgLen, nLen);
        LOGERROR("该消息ID为[%d]", *(const unsigned short*)(pBuf + 2));
        Close(true);
        return false;
    }

#ifdef	PROFILE_X
    InterlockedExchangeAdd(&s_nPackSum, nLen);
    InterlockedIncrement(&s_nPackCount);
#endif	// PROFILE

#ifdef	DUMP
    char buf[4096] = "";
    char buf3[4];
    for(int i = 0; i < nLen; i++)
    {
        sprintf(buf3, "%02X ", (unsigned char)pBuf[i]);
        strcat(buf, buf3);
        if(i % 16 == 15)
        {
            strcat(buf, "\n");
        }
    }
    strcat(buf, "\n");
    LOGMSG("Send: \n%s", buf);
#endif
#ifdef	ENCRYPT
    // 加密功能
    char	buf[_MAX_MSGSIZE];			//??? 可优化
    memcpy(buf, pBuf, nLen);
    pBuf = buf;		//? 替换掉参数的值
    if(key1 || key2)
    {
        m_cEncryptSend.Encrypt((unsigned char*)pBuf, nLen);
    }
#endif

    // 重复写入RECVREPEATTIMES次
    int		nPutBytes = 0;		// 已经发送了的长度
    int		nCount = 0;
    int		ret = 0;
    while(nPutBytes < nLen)
    {
        ret = send(m_sockServer, (char*)pBuf + nPutBytes, nLen - nPutBytes, 0);		// 发送数据
        if(ret > 0)
        {
            nPutBytes += ret;
        }
        else	// ret < 0
        {
            int err = WSAGetLastError();
            if(err != WSAEWOULDBLOCK)
            {
#ifdef	ALPHA_X
                LOGWARNING("服务SOCKET的send()错误[%d]，可能客户端关闭了SOCKET", err);
#endif
                Close(true);
                return false;
            }

            // 再给它一次机会。无意义
            ret = send(m_sockServer, (char*)pBuf + nPutBytes, nLen - nPutBytes, 0);		// 发送数据
            if(ret > 0)
            {
                nPutBytes += ret;
                LOGERROR("首次SEND不成功，再次SEND成功，原因未知。IP地址为[%s]", m_bufPeerIP);
                // continue;
            }
            else	// ret < 0
            {
                int err = WSAGetLastError();
                if(err == WSAEWOULDBLOCK)
                {
                    if(m_tLastRecv > 0)
                    {
                        if((clock() - m_tLastRecv) / CLOCKS_PER_SEC >= 25)		//? 25秒以上给出警告
                        {
                            LOGERROR("send()溢出，可能服务器已断线。IP地址为[%s]", m_bufPeerIP);
                        }
                        else
                        {
                            LOGERROR("send()溢出，玩家网速太慢。IP地址为[%s]", m_bufPeerIP);
                        }
                    }
                    else if(m_tLastRecv < 0)			// 负值表示连接时间
                    {
                        if((clock() + m_tLastRecv) / CLOCKS_PER_SEC >= 25)		//? 25秒以上给出警告
                        {
                            LOGERROR("send()溢出，可能服务器已断线。IP地址为[%s]。", m_bufPeerIP);
                        }
                        else
                        {
                            LOGERROR("send()溢出，玩家网速太慢。IP地址为[%s]。", m_bufPeerIP);
                        }
                    }
                    else
                    {
                        LOGERROR("内部异常，m_tLastRecv 为 0");
                    }
                }	// WSAEWOULDBLOCK
#ifdef	ALPHA_X
                else
                {
                    LOGWARNING("服务SOCKET的send()错误[%d]，可能客户端关闭了SOCKET.", err);
                }
#endif

                // 再次ERR, 断开
                Close(true);
                return false;
            }	// ret < 0
        }	// ret < 0

        // 防死循环
        if(++nCount > RECVREPEATTIMES)		//?? 调试变量
        {
            LOGWARNING("系统效率严重下降，send()函数无法正常写出数据。一个连接被强行断开。");
            Close(true);
            return false;
        }
    }

    ASSERT(ret == nLen);

    if(nCount >= 2)		//?? 2: 调试参数
    {
        LOGMSG("系统效率严重下降，send()运行了[%d]次才发送完数据。", nCount);
    }

#ifdef	PROFILE_XXXXX
    // OUTBUF溢出警告
    if(g_aServerShare[nIndex].m_queOutput.Len() > OUTBUFSIZE * 4 / 5)
    {
        LOGWARNING("连接[%d]OUTBUF使用太多[%d]，即将溢出", nIndex,
                   g_aServerShare[nIndex].m_queOutput.Len());
    }

    // 统计OUTBUF使用情况
#endif

    return true;
}

//	关闭写操作。以便让对方主动关闭SOCKET。
template <unsigned long key1, unsigned long key2>
bool	CServerSocket<key1, key2>::ShutDown()
{
    if(!m_bState)
    {
        return false;
    }

//	m_bSendEnable = false;
    m_bState = c_stateShutDown;
    if(shutdown(m_sockServer, 1))		// 1: SD_SEND
    {
        int err = WSAGetLastError();
        LOGWARNING("服务SOCKET的shutdown()错误[%d]，可能客户端关闭了SOCKET", err);
        Close(true);
        return false;
    }
    else
    {
        return true;	// 正常关闭
    }
}

//	可重复调用。通常不用，用于主动关闭SOCKET。
template <unsigned long key1, unsigned long key2>
void	CServerSocket<key1, key2>::Close(bool bLinger0 /*= false*/)
{
    if(m_bState)
    {
        if(bLinger0)
        {
            //	修改SOCKET属性成立即关闭型
            struct linger ling = { 1, 0 };
            setsockopt(m_sockServer, SOL_SOCKET, SO_LINGER, (char*)&ling, sizeof(ling));
        }

        m_bState = c_stateNone;
//		m_bSendEnable = false;
        closesocket(m_sockServer);
        InterlockedDecrement(&s_nSocketCount);
        m_sockServer	= INVALID_SOCKET;
        m_bufPeerIP[0]	= 0;

//		DecConnectCount();
    }
}

template <unsigned long key1, unsigned long key2>
char* 	CServerSocket<key1, key2>::GetPeerIP()	// 返回NULL为错。可改为NEW时从ACCEPT接收nClientIP???
{
    if(!m_bState)
    {
        return NULL;
    }

    if(strlen(m_bufPeerIP))
    {
        return m_bufPeerIP;
    }
    else
    {
        // 取对方IP。无意义
        LOGERROR("调用了GetPeerIP()中的无效代码。并无错误：)");
        sockaddr_in	inAddr;
        memset(&inAddr, 0, sizeof(inAddr));
        int		nLen = sizeof(inAddr);
        if(getpeername(m_sockServer, (sockaddr*)&inAddr, &nLen))
        {
            int err = WSAGetLastError();
            LOGERROR("服务端SOCKET的getpeername()错误[%d].", err);
            Close(true);
            return NULL;
        }
        char* 	pAddr = inet_ntoa(inAddr.sin_addr);
        if(pAddr)
        {
            strcpy(m_bufPeerIP, pAddr);
            return m_bufPeerIP;
        }
        else
        {
            LOGERROR("服务端SOCKET的inet_ntoa()错误.");
            Close(true);
            return NULL;
        }
    }
}
