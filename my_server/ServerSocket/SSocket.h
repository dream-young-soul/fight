
//**********************************************************
// 代码编辑器
//**********************************************************

// ServerSocket.h
// paled, 2002.6.2

// 该类用于SOCKET连接的服务端一方。可以用于小游戏服务器LISTEN客户端登录，也可用于连接数据服务器。
// 使用非阻塞方式实现。


#ifndef	SSOCKET_H
#define	SSOCKET_H


////////////////////////////////////////////////////////////////////////////////////////////
#include "Common.h"				// 公用函数库

#define	PURE_VIRTUAL_DECLARE		PURE_VIRTUAL_FUNCTION		//???
#define	PURE_VIRTUAL_DECLARE_0		PURE_VIRTUAL_FUNCTION_0		//???

#define LOGIN_KEY1				0xa61fce5e	// A = 0x20, B = 0xFD, C = 0x07, first = 0x1F, key = a61fce5e
#define LOGIN_KEY2				0x443ffc04	// A = 0x7A, B = 0xCF, C = 0xE5, first = 0x3F, key = 443ffc04

////////////////////////////////////////////////////////////////////////////////////////////

const int		RECV_BUFFER_SIZE		= 4096;		// 接收缓冲区尺寸

// SOCKET的初始化和终结(注：服务端使用。)
// 实现方式为：初始化和清除WINSOCK2.0版的DLL。
bool	ServerSocketInit();			// 相当于WSAStartup()
bool	ServerSocketFinal();		// 相当于WSACleanup()

typedef	char* CPacket;

class	IListenSocket
{
public:
    // 初始化监听SOCKET。可重复调用，以监听另一个端口。
    // nPort:	监听的端口
    // return:	成功返回ture
    virtual bool Listen(int nPort, int nSndBuf = 0, int nRcvBuf = 0)	PURE_VIRTUAL_DECLARE_0

    // 接收一个新连接的SOCKET。
    // SOCKET:		新连接的SOCKET，如无连接则返回INVALID_SOCKET常量。该函数立即返回。
    virtual SOCKET		Accept()	PURE_VIRTUAL_DECLARE_0

    // 关闭监听SOCKET。可重复调用。
    virtual void		Close()		PURE_VIRTUAL_DECLARE

    // 返回SOCKET，以便于SELECT()
    virtual SOCKET		Socket()	PURE_VIRTUAL_DECLARE_0
};

class	CListenSocket : IListenSocket
{
public:
    IListenSocket*	GetInterface() { return (IListenSocket*)this; }
public:
    // nSndBuf:	系统发送BUFFER的长度。(注：不是指应用程序的BUFFER)
    // nRcvBuf:	系统接收BUFFER的长度。(注：不是指应用程序的BUFFER)
    CListenSocket();
    ~CListenSocket() { Close(); }
protected: // interface
    virtual bool		Listen(int nPort, int nSndBuf = 0, int nRcvBuf = 0);		// 0 : 不修改系统的缺省值。
    virtual SOCKET		Accept();
    virtual void		Close();
    virtual SOCKET		Socket() { return m_sock; }

    ////////////////////////////////////////////////
    // 内部属性
protected:
    SOCKET		m_sock;
};


class	IServerSocket
{
public:
    // 取出一个消息包，如果关闭或错误，要使用IsOpen()检查。
    virtual const char*	GetPacket	(int* nLen, BOOL bFromNet = true)				PURE_VIRTUAL_DECLARE_0
    // 取出一个消息包后，必须调用此函数清除消息包。
    virtual bool		ClearPacket	(int nLen)				PURE_VIRTUAL_DECLARE_0
    // 发送一个消息包到网络上，立即发送，不在应用程序中缓冲。
    // return false: 网络错误、溢出或关闭。
    virtual bool		SendPacket	(const char* pack, int nLen)	PURE_VIRTUAL_DECLARE_0
    // 看SOCKET是否已关闭
    virtual bool		IsOpen		()						PURE_VIRTUAL_DECLARE_0
    // 关闭数据SOCKET。可重复调用
    // bNoLinger: 立即回收SOCKET资源。(通常用于SOCKET异常关闭)
    virtual void		Close		(BOOL bNoLinger = false)	PURE_VIRTUAL_DECLARE
    // 取得对方的SOCKET。
    // return: 以网络字节顺序表示。
    virtual DWORD		GetPeerIP	()						PURE_VIRTUAL_DECLARE_0

    // 返回SOCKET，以便于SELECT()
    virtual SOCKET		Socket()							PURE_VIRTUAL_DECLARE_0

    // 修改一个数据SOCKET的， 0: 不修改
    virtual int 		SetBufSize(int nSendBuf, int nRecvBuf = 0)			PURE_VIRTUAL_DECLARE_0

    // 修改动态加密
    virtual void		ChangeCode(DWORD dwData)			PURE_VIRTUAL_DECLARE
};
class	CServerSocket : IServerSocket
{
public:
    IServerSocket*	GetInterface() { return (IServerSocket*)this; }
public: // static
    // 从堆中分配一对象，并初始化。
    // sock:	数据套接字
    // GetPacketLen:	函数指针，可取得消息包的长度。
    static CServerSocket*	CreateNew(SOCKET sock);

    // 催毁由CreateNew创建的对象。
    static void				Destroy(CServerSocket* pSocket);

protected: // interface
    virtual const char*	GetPacket	(int* pLen, BOOL bFromNet /*= true*/);
    virtual bool		ClearPacket	(int nLen);
    virtual bool		SendPacket	(const char* pack, int nLen);
    virtual bool		IsOpen		();
    virtual	void		Close		(BOOL bNoLinger = false);
    virtual DWORD		GetPeerIP	();
    virtual SOCKET		Socket() { return m_sock; }
    virtual int			SetBufSize	(int nSendBuf, int nRecvBuf);

    // 内部属性 ////////////////////////////////////////////////
public:
    CServerSocket(SOCKET sock) { m_sock = sock; m_nLen = 0; }
    ~CServerSocket() { Close(); }
protected:
    SOCKET		m_sock;
    char		m_bufMsg[RECV_BUFFER_SIZE];
    int			m_nLen;

    //#ifdef	ENCRYPT
public:
    void	ChangeCode(DWORD dwData) { m_cEncryptRecv.ChangeCode(dwData); }
protected:
    typedef	CEncryptServer<LOGIN_KEY1, LOGIN_KEY2>	EncryptServer;
    EncryptServer	m_cEncryptRecv, m_cEncryptSend;
    //#endif
};


#endif // SSOCKET_H
