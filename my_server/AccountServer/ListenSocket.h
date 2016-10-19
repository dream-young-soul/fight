
//**********************************************************
// 代码编辑器
//**********************************************************

// 监听线程部分
// 仙剑修, 2001.10.10
// ★该类中没有包含WSASTARTUP()和WSACLEANUP()

#ifndef LISTENTHREAD_H
#define LISTENTHREAD_H


//	1、CListenSocket：该类用于监听线程中创建局域变量。
// 该类可重复使用。当调用 Close() 关闭后，可重新调用 Open() 新建监听端口。但该系统中没使用该功能。
class	CListenSocket
{
public:
    //	主要方法：
    //	每个对象只能BIND到一个端口。
    CListenSocket(u_short nPort, int nSndSize = 0);
    //	用于释放SOCKET
    ~CListenSocket();
    //	创建服务端监听SOCKET，具体选项请看代码。失败时返回false。
    bool	Open();		// 可重复调用
    //	错误时返回INVALID_SOCKET
    SOCKET	Accept(u_long& nClientIP);
    //	可重复调用。通常不用，可用于主动关闭SOCKET。
    //	关闭后可重新调用Open()重复使用该对象。
    void	Close();		// 可重复调用
    bool	IsOpen() { return m_bState; }
    bool	Rebuild();
public:
    SOCKET	Socket() { return m_sockListen; }
protected:
    //	主要成员变量：
    const u_short	m_nPort;
    const int		m_nSndBuf;
    SOCKET	m_sockListen;
    //	网络状态是否正常的标志。
    //	当该值为false时表示该对象不可用。可能未Open()，也可能网络错误。
    bool	m_bState;
    time_t	m_tCloseTime;		// 上次关闭SOCKET的时间，用于自动延时重建SOCKET
};



#endif	// LISTENTHREAD_H
