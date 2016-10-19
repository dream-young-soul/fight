
//**********************************************************
// 代码编辑器
//**********************************************************

// ClientSocket.h
// paled, 2002.6.2

// 该类用于SOCKET连接的客户端一方。即可以用于客户端程序，也可以用于小游戏服务器连接数据服务器。
// 使用非阻塞方式实现。

#ifndef	CLIENTSOCKET_H
#define	CLIENTSOCKET_H

#include <winsock2.h>
#include "typedef.h"
#include "EncryptClient.h"

const int	RECV_BUFFER_SIZE	= 2048;			// 应用程序自己的BUF
const int	SEND_BUFFER_SIZE	= 4048;			// 应用程序自己的BUF

////////////////////////////////////////////////////////////////////////////////////////////
#include "common.h"				// 公用函数库


// SOCKET的初始化和终结(注：只有客户端程序才需要这两个函数。服务端用ServerSocket.h中的另两个函数。)
// 如果已经使用了MFC的WINSOCK初始化代码，则不要再使用这两个函数。
// 实现方式为：初始化和清除WINSOCKET2.0版的DLL。					// WINSOCK1.1
bool	ClientSocketInit();			// 相当于WSAStartup()
bool	ClientSocketFinal();		// 相当于WSACleanup()

class	CClientSocket
{
public:
    // GetPacketLen:	函数指针，可取得消息包的长度。
    // nSndBuf:	系统发送BUFFER的长度。(注：不是指应用程序的BUFFER)
    // nRcvBuf:	系统接收BUFFER的长度。(注：不是指应用程序的BUFFER)
    CClientSocket();		// =0 : 使用系统的缺省值。
public:
    // 连接到CServerSocket
    // szName:		服务器域名或IP地址
    // nPort:		服务器端口号
    // nBlockSecs:	阻塞调用的秒数
    // return:		连接成功时返回true，错误或正在连接时返回false。第二个可重复调用以检查是否连接成功。
    bool		Open(const char* szName, int nPort, int nBlockSecs = 0, int nSndBuf = 0, int nRcvBuf = 0);
    bool		Open(int nBlockSecs = 0);		// 不能用于首次调用

    // 取一个消息包。如无消息则返回NULL，pBufSize返回数据长度。
    LPCTSTR		GetPacket(int* pBufSize, bool bFromNet = true);

    // 处理消息后，清空BUF中的数据。
    bool		ClearPacket(int nPacketSize);

    // 关闭SOCKET。可重复调用。
    void		Close();

public:
    // 存一个消息包到应用程序的BUFFER(非系统BUFFER)
    // return false: SOCKET已断开, 或Flush()调用未清空(bFlush为true时)。
    bool		SendPacket(const char* pack, int nLen, bool bFlush = false);

    // 发送BUFFER中的所有消息。
    // 应用核心应保证定时调用此接口，以发送BUFFER中的数据。
    bool		Flush();				// return false : no empty

public:
    // 返回连接是否正常(是否可发送数据)。通常用于检测连接是否完成。
    bool		IsOpen();

    // 内部属性 /////////////////////////////////////////////////////////////////////
protected:
    IPSTR		m_szIP;
    int			m_nPort;
    int			m_nSndSize;
    int			m_nRcvSize;

    SOCKET		m_sock;
    char		m_bufMsg[RECV_BUFFER_SIZE];
    int			m_nLen;
    char		m_bufSendMsg[SEND_BUFFER_SIZE];
    int			m_nSendLen;

    enum { STATE_CLOSED = 0, STATE_CONNECT, STATE_OPEN, };
    int			m_nState;
    //#ifdef	ENCRYPT
public:
    void	ChangeCode(DWORD dwData) { m_cSendEncrypt.ChangeCode(dwData); }
protected:
    // 加密对象
    typedef	CEncryptClient<0x20, 0xFD, 0x07, 0x1F, 0x7A, 0xCF, 0xE5, 0x3F>	ENCRYPTCLIENT;
    ENCRYPTCLIENT	m_cSendEncrypt, m_cRecvEncrypt;
    //#endif	// ENCRYPT
};


#endif // CLIENTSOCKET_H




