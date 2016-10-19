
//**********************************************************
// 代码编辑器
//**********************************************************

// 服务线程部分
// 仙剑修, 2001.10.10

#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

#ifdef	ENCRYPT
	#include "EncryptServer.h"
#endif

	//	2、CServerSocket：该类用于服务线程中创建局域变量。
template <unsigned long key1, unsigned long key2>
class	CServerSocket
{
public:
	CServerSocket();
	~CServerSocket();
public:
//	主要方法：
	bool	Open(SOCKET sockServer, u_long nClientIP);

	//*** 接收一个消息，写入pBuf所指的缓冲区中。返回false: 错误或关闭。返回true时nLen置0：没有数据
	bool	Recv(char * pBuf, int &nLen);

	bool	Send(const char * pBuf, int nLen);	// 返回false: 网络错误

	//	关闭写操作。以便让对方主动关闭SOCKET。未使用
	bool	ShutDown();
	//	可重复调用。通常不用，用于主动关闭SOCKET。
	void	Close(bool bLinger0 = false);
	//	内联函数，直接返回m_sockServer成员变量。m_bState为false时返回INVALID_SOCKET。
	SOCKET	Socket() { return m_sockServer; }
	bool	IsOpen() { return m_bState != 0; }
	bool	IsShutDown() { return m_bState == c_stateShutDown; }
	char *	GetPeerIP();		// 返回NULL为错。空串：无数据
protected:
	time_t m_tLastRecv;
//	主要成员变量：
	//	本线程索引，用于找到本线程的全局中共享变量。
	SOCKET	m_sockServer;
	//	是否允许发送数据。调用ShutDown()之后将不可写。
//	bool	m_bSendEnable;
	//	网络状态是否正常的标志。为false时表示SOCKET已不可用。
	enum	{ c_stateNone = 0, c_stateNormal, c_stateShutDown }	m_bState;
//	次要成员变量：
	char 	m_bufPeerIP[IPSTRSIZE];		// 好象不必有该属性，因为 CConnect 对象中也有此属性。
#ifdef	ENCRYPT
	typedef		CEncryptServer<key1, key2>	POINTENCRYPT;
	POINTENCRYPT	m_cEncryptRecv;
	POINTENCRYPT	m_cEncryptSend;
#endif
};


#include "ServerSocket.hpp"


#endif	// SERVERSOCKET_H
