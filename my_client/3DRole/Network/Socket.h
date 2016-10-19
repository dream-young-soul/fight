
//**********************************************************
// 代码编辑器
//**********************************************************

// Socket.h
// zengsong, 2001.9.25

#ifndef _SOCKET_H
#define _SOCKET_H

#include <windows.h>
#include <winsock.h>
#include <assert.h>
#include "EncryptClient.h"


////////////////////////////////////////////////////////////////////////////
//#define	LOGFILE
#define	ENCRYPT
////////////////////////////////////////////////////////////////////////////
//const int _MAX_MSGSIZE		=1024;		//??? 可能重复定义
#define ACCOUNT_KEY1			// A = 0xB2, B = 0x9B, C = 0x53, first = 0xCF, key = f2cfa8cc
#define ACCOUNT_KEY2			// A = 0xFC, B = 0xC9, C = 0xD9, first = 0xC6, key = 78c6fa82
#define LOGIN_KEY1				// A = 0x20, B = 0xFD, C = 0x07, first = 0x1F, key = a61fce5e
#define LOGIN_KEY2				// A = 0x7A, B = 0xCF, C = 0xE5, first = 0x3F, key = 443ffc04
////////////////////////////////////////////////////////////////////////////
#ifdef	LOGFILE
void LogSave (const char* fmt, ...);
#endif


#define BLOCKSECONDS	30		// INIT函数阻塞时间
#define INBUFSIZE	(64*1024)	//?	具体尺寸根据剖面报告调整
#define OUTBUFSIZE	(8*1024)		//? 具体尺寸根据剖面报告调整。当不超过8K时，FLUSH只需要SEND一次
#define	SOCKETLOGFILE	"Socket.h"

//void	SaveLog(const char * sFormat, ...);
//#define	LOGCATCH SaveLog


/*
#ifdef CLIENTSOCKET32_EXPORTS
#define DLLAPI extern "C" _declspec(dllexport)
#else
#define DLLAPI extern "C" _declspec(dllimport)
#endif

#define	DLLAPI

DLLAPI BOOL __stdcall ClientSocketInit		(char* pszServerIP, int nServerPort);
DLLAPI BOOL __stdcall ClientSocketSendMsg	(void* pBuf, int nSize);
DLLAPI BOOL __stdcall ClientSocketReceiveMsg(void* pBuf, int& nSize);
DLLAPI BOOL __stdcall ClientSocketCheck		(void);
DLLAPI void __stdcall ClientSocketDestroy	(void);
//*/



/*
#ifdef	_DEBUG

// 调试用的宏定义
#define LogSave(x)		SaveLog("*SOCKET* "x"\n")
#define LogSave(x,y)	SaveLog("*SOCKET* "x"\n",(y))
#define LogSave(x,y,z)	SaveLog("*SOCKET* "x"\n",(y),(z))
#define PF_INBUFCOUNT	1000		//? 调试INBUF尺寸的循环次数
#define PF_OUTBUFCOUNT	500			//? 调试OUTBUF尺寸的循环次数
#define SOCKETLOGFILE	"Socket.log"

void	SaveLog(const char * sFormat, ...);

#else	// _DEBUG

#define LogSave(x)
#define LogSave(x,y)
#define LogSave(x,y,z)

#endif	// _DEBUG
//*/
/////////////////////////////////////////////////////////////////////
template < unsigned char a1, unsigned char b1, unsigned char c1, unsigned char fst1,
         unsigned char a2, unsigned char b2, unsigned char c2, unsigned char fst2 >
class CGameSocket
{
protected:
    SOCKET	m_sockClient;
    BOOL	m_bState;
    char	m_bufInput[INBUFSIZE];
    int		m_nInbufLen;
    int		m_nInbufStart;				// INBUF使用循环式队列，该变量为队列起点，0 - (SIZE-1)
    char	m_bufOutput[OUTBUFSIZE];	//? 可优化为指针数组
    int		m_nOutbufLen;
#ifdef	ENCRYPT
    // 加密对象
    typedef	CEncryptClient<a1, b1, c1, fst1, a2, b2, c2, fst2>	ENCRYPTCLIENT;
    ENCRYPTCLIENT	m_cSendEncrypt;
    ENCRYPTCLIENT	m_cRecvEncrypt;
#endif	// ENCRYPT

#ifdef	_DEBUG_X
    // 调试用的变量
    int		pf_nInbufAverage;
    int		pf_nInbufMax;
    int		pf_nInbufCount;
    int		pf_nOutbufAverage;
    int		pf_nOutbufMax;
    int		pf_nOutbufCount;
#endif	// _DEBUG

public:
    CGameSocket(void);
    // TODO: add your methods here.
    BOOL	Create(char* pszServerIP, int nServerPort, BOOL bKeepAlive = FALSE);
    BOOL	SendMsg(void* pBuf, int nSize);
    BOOL	ReceiveMsg(void* pBuf, int& nSize);
    BOOL	Flush(void);
    BOOL	Check(void);
    void	Destroy(void);
#ifdef	ENCRYPT
    void	ChangeCode(DWORD dwData) { m_cSendEncrypt.ChangeCode(dwData); }

#endif
private:
    BOOL	RecvFromSock(void);		// 从网络中读取尽可能多的数据
};

/////////////////////////////////////////////////////////////////////////////

#ifdef	_DEBUG_X
// 调试用的变量
int		pf_nInbufAverage;
int		pf_nInbufMax;
int		pf_nInbufCount;
int		pf_nOutbufAverage;
int		pf_nOutbufMax;
int		pf_nOutbufCount;
#endif

//#define	TEST			//?????????????????????????????????????????????


template < unsigned char a1, unsigned char b1, unsigned char c1, unsigned char fst1,
         unsigned char a2, unsigned char b2, unsigned char c2, unsigned char fst2 >
CGameSocket<a1, b1, c1, fst1, a2, b2, c2, fst2>::CGameSocket()
{
    // 初始化
    m_bState	= false;
#ifndef	TEST
    DeleteFile(SOCKETLOGFILE);
#endif
}

template < unsigned char a1, unsigned char b1, unsigned char c1, unsigned char fst1,
         unsigned char a2, unsigned char b2, unsigned char c2, unsigned char fst2 >
BOOL CGameSocket<a1, b1, c1, fst1, a2, b2, c2, fst2>::Create(char* pszServerIP, int nServerPort, BOOL bKeepAlive /*= FALSE*/)
{
    try
    {
        if (m_bState)		// 用于重新初始化
        {
            Destroy();
        }
        // 检查参数
        try
        {
            if (pszServerIP == 0 || strlen(pszServerIP) > 15)
            {
#ifdef	LOGFILE
                LogSave("ERROR: IP addr too long.");
#endif
                exit(5);
            }
        }
        catch(...)
        {
#ifdef	LOGFILE
            LogSave("CATCH ERROR: Init() parameter [pszServerIP] is invalid point.");
#endif
            exit(5);
        }
        WSADATA		cData;
        if (WSAStartup(0x0101, &cData) != 0)
        {
#ifdef	LOGFILE
            LogSave("ERROR: WSAStartup() failed.");
#endif
            return false;
        }
        if (cData.wVersion != 0x0101)
        {
#ifdef	LOGFILE
            LogSave("ERROR: unmatched Socket version[0x%04X].", cData.wVersion);
#endif
            WSACleanup();
            return false;
        }
        // 创建主套接字
        //		sockaddr_in		sin;
        m_sockClient = socket(PF_INET, SOCK_STREAM, 0);
        if (m_sockClient == INVALID_SOCKET)
        {
            int err = WSAGetLastError();
#ifdef	LOGFILE
            LogSave("ERROR %d: Init socket() failed.", err);
#endif
            WSACleanup();
            return FALSE;
        }
        // 设置SOCKET为KEEPALIVE
        if (bKeepAlive)
        {
            int		optval = 1;
            if (setsockopt(m_sockClient, SOL_SOCKET, SO_KEEPALIVE, (char*) &optval, sizeof(optval)))
            {
                int err = WSAGetLastError();
#ifdef	LOGFILE
                LogSave("ERROR %d: Init setsockopt() failed.\n", err);
#endif
                closesocket(m_sockClient);
                WSACleanup();
                return FALSE;
            }
        }
        // 设置为非阻塞方式
        //*
        unsigned long	i = 1;
        if (ioctlsocket(m_sockClient, FIONBIO, &i))
        {
            int err = WSAGetLastError();
#ifdef	LOGFILE
            LogSave("ERROR %d: Init ioctlsocket() failed.", err);
#endif
            closesocket(m_sockClient);
            WSACleanup();
            return FALSE;
        }
        //*/
        unsigned long serveraddr = inet_addr(pszServerIP);
        if (serveraddr == INADDR_NONE)	// 检查IP地址格式错误
        {
#ifdef	LOGFILE
            LogSave("ERROR: IP地址[%s]格式错误.", pszServerIP);
#endif
            closesocket(m_sockClient);
            WSACleanup();
            return false;
        }
        sockaddr_in	addr_in;
        memset((void*)&addr_in, sizeof(addr_in), 0);
        addr_in.sin_family = AF_INET;
        addr_in.sin_port = htons(nServerPort);
        addr_in.sin_addr.s_addr = serveraddr;
        if (connect(m_sockClient, (sockaddr*)&addr_in, sizeof(addr_in)) == SOCKET_ERROR)
        {
            int err = WSAGetLastError();
            if (err != WSAEWOULDBLOCK)
            {
#ifdef	LOGFILE
                LogSave("ERROR %d: Connect() failed.", err);
#endif
                closesocket(m_sockClient);
                WSACleanup();
                return false;
            }
            else	// WSAWOLDBLOCK
            {
                timeval timeout;
                timeout.tv_sec	= BLOCKSECONDS;
                timeout.tv_usec	= 0;
                fd_set writeset, exceptset;
                FD_ZERO(&writeset);
                FD_ZERO(&exceptset);
                FD_SET(m_sockClient, &writeset);
                FD_SET(m_sockClient, &exceptset);
                int ret = select(FD_SETSIZE, NULL, &writeset, &exceptset, &timeout);
                if (ret == 0)
                {
#ifdef	LOGFILE
                    LogSave("ERROR : select连接超时。");
#endif
                    closesocket(m_sockClient);
                    WSACleanup();
                    return false;
                }
                else if (ret < 0)
                {
                    int	err = WSAGetLastError();
#ifdef	LOGFILE
                    LogSave("ERROR %d: select连接错误。", err);
#endif
                    closesocket(m_sockClient);
                    WSACleanup();
                    return false;
                }
                else	// ret > 0
                {
                    if (FD_ISSET(m_sockClient, &exceptset))		// or (!FD_ISSET(m_sockClient, &writeset)
                    {
#ifdef	LOGFILE
                        LogSave("ERROR : select连接例外。");
#endif
                        closesocket(m_sockClient);
                        WSACleanup();
                        return false;
                    }
                }
            }
        }
        m_nInbufLen		= 0;
        m_nInbufStart	= 0;
        m_nOutbufLen	= 0;
        m_bState	= true;
#ifdef	ENCRYPT
        m_cSendEncrypt.Init();
        m_cRecvEncrypt.Init();
#endif
#ifdef	_DEBUG_X
        pf_nInbufAverage	= 0;
        pf_nInbufMax		= 0;
        pf_nInbufCount		= 0;
        pf_nOutbufAverage	= 0;
        pf_nOutbufMax		= 0;
        pf_nOutbufCount		= 0;
#endif
        return true;
    }
    catch(...)
    {
#ifdef	LOGFILE
        LogSave("CATCH ERROR: Init() failed.");
#endif
        exit(5);
    }
}

template < unsigned char a1, unsigned char b1, unsigned char c1, unsigned char fst1,
         unsigned char a2, unsigned char b2, unsigned char c2, unsigned char fst2 >
BOOL CGameSocket<a1, b1, c1, fst1, a2, b2, c2, fst2>::SendMsg(void* pBuf, int nSize)
{
    try
    {
        if (pBuf == 0 || nSize <= 0 || nSize > _MAX_MSGSIZE)
        {
#ifdef	LOGFILE
            LogSave("ERROR: SendMsg()参数错误.");
#endif
            exit(5);
        }
        if (!m_bState)
        {
#ifdef	LOGFILE
            LogSave("ERROR: 连接已经断开，无法SEND.");
#endif
            return false;		// 状态已错
        }
        // 检查通讯消息包长度
        try
        {
            int packsize = *(unsigned short*)pBuf;
            if (packsize <= 0 || packsize > _MAX_MSGSIZE)			//? 检测消息包尺寸错误
            {
#ifdef	LOGFILE
                LogSave("ERROR：想发送一个消息包尺寸超长[%d]的消息!!!", packsize);
#endif
                Destroy();
                exit(5);
            }
            if (packsize != nSize)
            {
#ifdef	LOGFILE
                LogSave("ERROR：CGameSocket::SendMsg()长度参数[%d]和消息头长度[%d]不相同!!!", nSize, packsize);
#endif
                Destroy();
                exit(5);
            }
        }
        catch(...)
        {
#ifdef	LOGFILE
            LogSave("CATCH ERROR: SendMsg() parameter pBuf is invalid point.");
#endif
            exit(5);
        }
        // 检测BUF溢出
        if (m_nOutbufLen + nSize > OUTBUFSIZE)
        {
            // 立即发送INBUF中的数据，以清空INBUF。
            Flush();
            if (m_nOutbufLen + nSize > OUTBUFSIZE)
            {
#ifdef	LOGFILE
                LogSave("ERROR: OUTBUF溢出!!! 可能是 send() 阻塞");
#endif
                Destroy();
                return FALSE;
            }
        }
        // 数据添加到BUF尾
        memcpy(m_bufOutput + m_nOutbufLen, pBuf, nSize);
#ifdef	ENCRYPT
        // 消息加密
        if (a1 || b1 || c1 || fst1 || a2 || b2 || c2 || fst2)
        {
            m_cSendEncrypt.Encrypt((unsigned char*)m_bufOutput + m_nOutbufLen, nSize);
        }
#endif
        m_nOutbufLen += nSize;
        assert(m_nOutbufLen <= OUTBUFSIZE);
#ifdef	_DEBUG_X
        if (pf_nOutbufCount >= PF_OUTBUFCOUNT)
        {
#ifdef	LOGFILE
            LogSave("Message: outbuf len average is %d.", pf_nOutbufAverage / PF_OUTBUFCOUNT);
            LogSave("Message: outbuf len max is %d.", pf_nOutbufMax);
#endif
            pf_nOutbufAverage	= 0;
            pf_nOutbufMax		= 0;
            pf_nOutbufCount		= 0;
        }
        else
        {
            pf_nOutbufAverage = pf_nOutbufAverage + m_nOutbufLen;
            if (pf_nOutbufMax < m_nOutbufLen)
            {
                pf_nOutbufMax = m_nOutbufLen;
            }
            pf_nOutbufCount++;
        }
#endif
        return true;
    }
    catch(...)
    {
#ifdef	LOGFILE
        LogSave("CATCH ERROR: SendMsg().");
#endif
        exit(5);
    }
}

template < unsigned char a1, unsigned char b1, unsigned char c1, unsigned char fst1,
         unsigned char a2, unsigned char b2, unsigned char c2, unsigned char fst2 >
BOOL CGameSocket<a1, b1, c1, fst1, a2, b2, c2, fst2>::ReceiveMsg(void* pBuf, int& nSize)
{
    try
    {
        if (pBuf == NULL)
        {
#ifdef	LOGFILE
            LogSave("ERROR: ReceiveMsg()参数错误.");
#endif
            exit(5);
        }
        //检查参数
        if (nSize <= 0 || nSize > _MAX_MSGSIZE)
        {
#ifdef	ALPHA_X
#ifdef	LOGFILE
            LogSave("警告: ClientSocketReceiveMsg(void* pBuf, int& nSize[%d])参数错误, 忽略", nSize);
#endif
#endif
        }
        try
        {
            char temp = *((char*)pBuf);
        }
        catch(...)
        {
#ifdef	LOGFILE
            LogSave("CATCH ERROR: ReceiveMsg() parameter [pBuf] is invalid point.");
#endif
            exit(5);
        }
        //失效也可取数据		if (!m_bState)	return false;
        // 检查是否有一个消息
        if (m_nInbufLen < 2)
        {
            if (!RecvFromSock() || m_nInbufLen < 2)		// 这个m_nInbufLen更新了
            {
                return false;
            }
        }
        int packsize = (u_char)m_bufInput[m_nInbufStart] +
                       (u_char)m_bufInput[(m_nInbufStart + 1) % INBUFSIZE] * 256;	//? 因为有回卷，必须分开算
        if (packsize <= 0 || packsize > _MAX_MSGSIZE)			//? 检测消息包尺寸错误
        {
#ifdef	LOGFILE
            LogSave("ERROR：接收到一个消息包尺寸超长[%d]的消息!!!", packsize);
#endif
            m_nInbufLen = 0;		// 直接清空INBUF
            m_nInbufStart = 0;
            Destroy();
            exit(5);
        }
        // 检查消息是否完整
        if (packsize > m_nInbufLen)
        {
            if (!RecvFromSock() || packsize > m_nInbufLen)	//? 这个m_nInbufLen已更新
            {
                return false;
            }
        }
        // 复制出一个消息
        if (m_nInbufStart + packsize > INBUFSIZE)		// 数据回卷
        {
            int copylen = INBUFSIZE - m_nInbufStart;
            assert(copylen < packsize);
            assert(m_nInbufStart + copylen == INBUFSIZE);
            try
            {
                memcpy(pBuf, m_bufInput + m_nInbufStart, copylen);
            }
            catch(...)
            {
#ifdef	LOGFILE
                LogSave("CATCH ERROR：memcpy(pBuf, m_bufInput + m_nInbufStart[%d], copylen[%d]);", m_nInbufStart, copylen);
#endif
                exit(5);
            }
            //assert(copylen < packsize);
            assert(packsize - copylen < INBUFSIZE);
            try
            {
                memcpy((unsigned char*)pBuf + copylen, m_bufInput, packsize - copylen);
            }
            catch(...)
            {
#ifdef	LOGFILE
                LogSave("CATCH ERROR：memcpy(pBuf + copylen[%d], m_bufInput, packsize[%d] - copylen);", copylen, packsize);
#endif
                exit(5);
            }
            nSize = packsize;
        }
        else
        {
            assert(m_nInbufStart + packsize <= INBUFSIZE);
            try
            {
                memcpy(pBuf, m_bufInput + m_nInbufStart, packsize);
            }
            catch(...)
            {
#ifdef	LOGFILE
                LogSave("CATCH ERROR：memcpy(pBuf, m_bufInput + m_nInbufStart[%d], packsize[%d]);", m_nInbufStart, packsize);
#endif
                exit(5);
            }
            nSize = packsize;
        }
        /*
        		if (nSize != 9 || memcmp(pBuf, "\x09\x00\x99\x99\x00\x01\x02\x03\x04", 9) != 0)
        		{
        #ifdef	LOGFILE
        			LogSave("TEMP: 消息长度[%d]", nSize);
        #endif
        			char buf[10];
        			sprintf(buf, "%02X %02X %02X %02X %02X %02X %02X %02X %02X",
        							((char*)pBuf)[0],
        							((char*)pBuf)[1],
        							((char*)pBuf)[2],
        							((char*)pBuf)[3],
        							((char*)pBuf)[4],
        							((char*)pBuf)[5],
        							((char*)pBuf)[6],
        							((char*)pBuf)[7],
        							((char*)pBuf)[8] );
        #ifdef	LOGFILE
        			LogSave("TEMP: 消息内容[%s]", buf);
        			LogSave("TEMP：START[%d]", m_nInbufStart);
        			LogSave("TEMP: LEN[%d]", m_nInbufLen);
        #endif
        		}
        */
        // 从BUF中删除该消息
        //if (m_nInbufLen - packsize > 0)
        //	memcpy(m_bufInput, m_bufInput + packsize, m_nInbufLen - packsize);
        m_nInbufStart = (m_nInbufStart + packsize) % INBUFSIZE;
        m_nInbufLen -= packsize;
        assert(m_nInbufStart >= 0 && m_nInbufStart < INBUFSIZE);
        assert(m_nInbufLen >= 0);
        return	true;
    }
    catch(...)
    {
#ifdef	LOGFILE
        LogSave("CATCH ERROR: ReceiveMsg().");
#endif
        exit(5);
    }
}

template < unsigned char a1, unsigned char b1, unsigned char c1, unsigned char fst1,
         unsigned char a2, unsigned char b2, unsigned char c2, unsigned char fst2 >
BOOL CGameSocket<a1, b1, c1, fst1, a2, b2, c2, fst2>::RecvFromSock(void)		// 从网络中读取尽可能多的数据
{
    //内部函数	try{
    // 接收一段数据
    if (m_bState && m_nInbufLen < INBUFSIZE)
    {
        // 接收第一段数据
        int	savelen, savepos;			// 数据要保存的长度和位置
        if (m_nInbufStart + m_nInbufLen < INBUFSIZE)		// INBUF中的剩余空间有回绕
        {
            savelen = INBUFSIZE - (m_nInbufStart + m_nInbufLen);    // 后部空间长度
        }
        else
        {
            savelen = INBUFSIZE - m_nInbufLen;
        }
        savepos = (m_nInbufStart + m_nInbufLen) % INBUFSIZE;
        assert(savepos + savelen <= INBUFSIZE);
        int inlen = recv(m_sockClient, m_bufInput + savepos, savelen, 0);
        if (inlen > 0)
        {
            //			LogSave("接收到 %d 个字节.", inlen);			//???
#ifdef	ENCRYPT
            // 消息解密
            if (a1 || b1 || c1 || fst1 || a2 || b2 || c2 || fst2)
            {
                m_cRecvEncrypt.Encrypt((unsigned char*)m_bufInput + savepos, inlen);
            }
#endif
            m_nInbufLen += inlen;
            assert(m_nInbufLen <= INBUFSIZE);
            // 接收第二段数据
            if (inlen == savelen && m_nInbufLen < INBUFSIZE)
            {
                int savelen = INBUFSIZE - m_nInbufLen;
                int savepos = (m_nInbufStart + m_nInbufLen) % INBUFSIZE;
                assert(savepos + savelen <= INBUFSIZE);
                int inlen = recv(m_sockClient, m_bufInput + savepos, savelen, 0);
                if (inlen > 0)
                {
#ifdef	ENCRYPT
                    // 消息解密
                    if (a1 || b1 || c1 || fst1 || a2 || b2 || c2 || fst2)
                    {
                        m_cRecvEncrypt.Encrypt((unsigned char*)m_bufInput + savepos, inlen);
                    }
#endif
                    m_nInbufLen += inlen;
                    assert(m_nInbufLen <= INBUFSIZE);
                }
                else if (inlen == 0)
                {
#ifdef	LOGFILE
                    LogSave("ERROR: 服务器已经关闭!");
#endif
                    Destroy();
                    return false;
                }
                else
                {
                    // 连接已断开或者错误（包括阻塞）
                    int err = WSAGetLastError();
                    if (err != WSAEWOULDBLOCK)
                    {
#ifdef	LOGFILE
                        LogSave("ERROR %d: recv failed.", err);
#endif
                        Destroy();
                        return false;
                    }
                }
            }	// 第二段
            /*#ifdef	_DEBUG_X
            			if (pf_nInbufCount >= PF_INBUFCOUNT)
            			{
            				LogSave("Message: inbuf len average is %d.", pf_nInbufAverage / PF_INBUFCOUNT);
            				LogSave("Message: inbuf len max is %d.", pf_nInbufMax);
            				pf_nInbufAverage	= 0;
            				pf_nInbufMax		= 0;
            				pf_nInbufCount		= 0;
            			}
            			else
            			{
            				pf_nInbufAverage = pf_nInbufAverage + m_nInbufLen;
            				if (pf_nInbufMax < m_nInbufLen)
            					pf_nInbufMax = m_nInbufLen;
            				pf_nInbufCount++;
            			}
            #endif
            */
        }
        else if (inlen == 0)
        {
#ifdef	LOGFILE
            LogSave("ERROR: 服务器已经关闭!");
#endif
            Destroy();
            return false;
        }
        else	// len < 0
        {
            // 连接已断开或者错误（包括阻塞）
            int err = WSAGetLastError();
            if (err != WSAEWOULDBLOCK)
            {
#ifdef	LOGFILE
                LogSave("ERROR %d: recv failed.", err);
#endif
                Destroy();
                return false;
            }
        }
    }			// if (m_bState && m_nInbufLen < INBUFLEN)
    return true;
    //内部函数	}catch(...){ LogSave("CATCH ERROR: RecvFromSock()."); exit(5); }
}

template < unsigned char a1, unsigned char b1, unsigned char c1, unsigned char fst1,
         unsigned char a2, unsigned char b2, unsigned char c2, unsigned char fst2 >
BOOL CGameSocket<a1, b1, c1, fst1, a2, b2, c2, fst2>::Flush(void)		//? 如果 OUTBUF > SENDBUF 则需要多次SEND（）
{
    try
    {
        // 发送一段数据
        if (m_nOutbufLen > 0)
        {
            int	outsize;
            outsize = send(m_sockClient, m_bufOutput, m_nOutbufLen, 0);
            if (outsize > 0)
            {
                //				LogSave("发送了 %d 个字节", outsize);		//???
                // 删除已发送的部分
                if (m_nOutbufLen - outsize > 0)
                {
                    memcpy(m_bufOutput, m_bufOutput + outsize, m_nOutbufLen - outsize);
                }
                m_nOutbufLen -= outsize;
                assert(m_nOutbufLen >= 0);
            }
            else	// 错误
            {
                int err = WSAGetLastError();
                if (err != WSAEWOULDBLOCK)
                {
#ifdef	LOGFILE
                    LogSave("ERROR: 网络错误[%d]，无法发送", err);
#endif
                    Destroy();
                    return false;
                }
            }
        }
        return true;
    }
    catch(...)
    {
#ifdef	LOGFILE
        LogSave("CATCH ERROR: Flush().");
#endif
        exit(5);
    }
}

template < unsigned char a1, unsigned char b1, unsigned char c1, unsigned char fst1,
         unsigned char a2, unsigned char b2, unsigned char c2, unsigned char fst2 >
BOOL CGameSocket<a1, b1, c1, fst1, a2, b2, c2, fst2>::Check(void)
{
    try
    {
        // 检查状态
        if (m_bState)
        {
            char	buf[1];
            int	ret = recv(m_sockClient, buf, 1, MSG_PEEK);
            if (ret == 0)
            {
#ifdef	LOGFILE
                LogSave("ERROR: 服务器已经关闭.");
#endif
                Destroy();
                return false;
            }
            else if (ret < 0)
            {
                int err = WSAGetLastError();
                if (err != WSAEWOULDBLOCK)
                {
#ifdef	LOGFILE
                    LogSave("ERROR %d: 网络错误.", err);
#endif
                    Destroy();
                    return false;
                }
                else	// 阻塞
                {
                    return true;
                }
            }
            else	// 有数据
            {
                return true;
            }
        }
        else
        {
            return false;
        }
    }
    catch(...)
    {
#ifdef	LOGFILE
        LogSave("CATCH ERROR: Check().");
#endif
        exit(5);
    }
}

template < unsigned char a1, unsigned char b1, unsigned char c1, unsigned char fst1,
         unsigned char a2, unsigned char b2, unsigned char c2, unsigned char fst2 >
void CGameSocket<a1, b1, c1, fst1, a2, b2, c2, fst2>::Destroy(void)
{
    try
    {
        // 关闭
        if (m_bState)
        {
            closesocket(m_sockClient);
            m_sockClient = INVALID_SOCKET;
            WSACleanup();
            //!!! 剩余消息可继续处理		m_nInbufLen = 0;
            //!!! 剩余消息可继续处理		m_nInbufStart = 0;
            m_nOutbufLen = 0;
            m_bState	= false;
        }
    }
    catch(...)
    {
#ifdef	LOGFILE
        LogSave("CATCH ERROR: Destroy().");
#endif
        exit(5);
    }
}


///////////////////////////////////////////////////////////////////////////
//typedef	CGameSocket<0xB2, 0x9B, 0x53, 0xCF, 0xFC, 0xC9, 0xD9, 0xC6>	CAccountSocket;
//typedef	CGameSocket<0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00>	CAccountSocket;
typedef	CGameSocket<0x20, 0xFD, 0x07, 0x1F, 0x7A, 0xCF, 0xE5, 0x3F>	CLoginSocket;


#endif
