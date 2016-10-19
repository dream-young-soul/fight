
//**********************************************************
// 代码编辑器
//**********************************************************

// 接口类。接口：基于消息通讯机制的网络间通讯接口。
// 仙剑修，2002.9.6

#ifndef	INTERNETPORT_H
#define INTERNETPORT_H

#pragma warning(disable:4786)

#define	_WINSOCKAPI_
#include <afxmt.h>
#ifndef FD_SETSIZE
#define	FD_SETSIZE					110	//? 可修改以增加节点数
#endif
#include "winsock2.h"
#include "I_MessagePort.h"
#include <list>
#include <vector>
using namespace std;

#include "MessagePort.h"		// 包含“通用定义”


/////////////////////////////////////////////////////////////////////////////////////////////////
// 外部定义
MSGPORT_BEGIN
#define	CONNECT_OVERTIMESECS		10					// 客户端OPEN等待超时秒数
#define	SOCKET_VERSION				0x0002				// 使用的WIN SOCKET版本号。0x0101、0x0002
#define	SNDBUF_SIZE					(32*1024)			// SOCKET系统内部SENDBUF尺寸

#define	IPSTR_SIZE	16
#define	MAX_KEYSIZE	256

#define	MASTER_PORT_ID				0					// 主节点号
#define	MAX_PORT_SIZE				FD_SETSIZE-2			// 最多多少节点

enum {	SYS_PACKET_BASE		= MAX_PACKET_ID + 1,	// 系统消息包
        SYS_PACKET_PORTID	= SYS_PACKET_BASE + 1,	// 向主节点发送自己的ID号(内容为INT)
        SYS_PACKET_PORTSIZE	= SYS_PACKET_BASE + 2,	// 向子节点发送SIZE(内容为INT)。子节点SIZE不同时，修正自己的SIZE
        SYS_PACKET_LOGINKEY	= SYS_PACKET_BASE + 3,	// 向主节点发送自己的LOGIN_KEY(内容为STRING)
     };
MSGPORT_END
/////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////
// 内部结构
MSGPORT_BEGIN
const int	MSG_HEAD_SIZE		= 6;					// 消息头的尺寸
const int	RECV_BUFFER_SIZE	= 16 * 1024;				// 应用程序内部BUFFER尺寸。
struct	CMessageHead							// 注意：结构尺寸必须与MSG_HEAD_SIZE相同
{
    unsigned short		m_nPort;				// 子->主：TARGET_PORT，主->子：SOURCE_PORT
    unsigned short		m_nPacket;
    unsigned short		m_nVarType;
    char				m_bufData[1];
};
MSGPORT_END
/////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////
class	CInternetPort : private IMessagePort
{
private: // 内部类型
    // 输入BUFFER
    struct	CRecvBuffer
    {
    public:
        CRecvBuffer()	{ Reset(); }
        void Reset()	{ m_nMsgLen = 0; }
        bool DelMsg(int nLen)
        {
            if (nLen <= m_nMsgLen)
            {
                if (nLen < m_nMsgLen)
                {
                    memcpy(m_bufMsg, m_bufMsg + nLen, m_nMsgLen - nLen);
                }
                m_nMsgLen -= nLen;
                return true;
            }
            m_nMsgLen	= 0;
            return false;
        }
        message_port::CMessageHead* GetHead()	{ return (message_port::CMessageHead*)m_bufMsg; }
        char* GetTail()	{ return m_bufMsg + m_nMsgLen; }
        int	  GetLen()	{ return m_nMsgLen; }
        void  AddLen(int nLen)	{ m_nMsgLen += nLen; }
    protected:
        char		m_bufMsg[message_port::RECV_BUFFER_SIZE];
        int			m_nMsgLen;
    };
public:
    CInternetPort			(int nPort);
    virtual ~CInternetPort	() { ClearPortSet(); }

    IMessagePort*	GetInterface() { return (IMessagePort*)this; }

protected: // Interface
    // 取本接口的ID号
    virtual int		GetSize	() { return m_nPortSize; }
    virtual int		GetID	() { return m_id; }

    // 初始化，设置接口ID号，开始接收消息。可重复调用(PORT_ID不能改变)。
    virtual bool	Open	();
    // 关闭接口，不再接收消息。可重复调用。客户端会等待CONNECT_OVERTIMESECS秒
    virtual bool	Close	();

    // 发送消息到指定接口。包含消息ID、数据类型、数据。return false: 发送不成功，或已经关闭
    virtual bool	Send	(int nPort, int nPacket, VAR_TYPE nVarType, const void* buf);
    // 接收指定接口(或所有接口)发来的消息。可指定消息ID，也可不指定。VARTYPE无意义。return false: 没有收到数据
    virtual bool	Recv	(int nPort, int nPacket, VAR_TYPE nVarType, void* buf, CMessageStatus* pStatus = NULL);

    // 检查消息栈中有没有消息，如没有消息则等待。return false: 超时或错误。
    virtual bool	WaitMsg	(int nMilliSec);						// ★可添加接口号和包类型参数，以等待某一消息。
    //	virtual bool	Broadcast	(int nGroup, int nPacket, int nVarType, DWORD nData);
    virtual bool	IsOpen	() { return m_nState == STATE_OK; }

protected:
    bool	PushMsg		(int nPort, int nPacket, VAR_TYPE nVarType, const void* buf);
    bool	PopMsg		(int* nPort, int* nPacket, VAR_TYPE* nVarType, void* buf);
    bool	SendFrom	(int nPortFrom, int nPort, int nPacket, VAR_TYPE nVarType, const void* buf);
    bool	RecvMsg		(int nMilliSec);
    bool	RecvToBuf	(CRecvBuffer* pBuf, SOCKET sock);					// return false: error
    bool	BufferToList	(int idx);								// return false: 无消息
    static int	SIZE_OF_TYPE		(int type);
    SOCKET	CreateServerSocket		();
    SOCKET	CreateClientSocket		();

    // 由SET的函数调用
    bool	Init();			// WSAStartup
    void	Clear();		// WSACleanup

protected:
    int			m_id;
    enum { STATE_OK, STATE_CLOSED, STATE_OPENING };
    int			m_nState;
    typedef	list<message_port::CMessagePacket*>	MSG_SET;
    MSG_SET		m_setMsg;

    ////////////////////////////////////////////////////////////////////////////////////////
    // 共用静态对象
    // 所有接口的添加必须在首次通讯前完成。通讯期间接口集不会有变化。完成所有通讯后，才能清空接口。
    // 注意：目前只支持“静态”接口集。
public: //static
    bool	InitPortSet(int nPortSize, LPCTSTR pszMasterIP, int nMasterPort);
    void	ClearPortSet();
    /*/ 向 1 ~ N 广播一个消息。
    void	BROADCAST1(int nPortFrom, int nPacket, int nVarType, const void* buf)
    {
    	for(int i = 1; i < m_nPortSize i++)
    	{
    		if (i != nPortFrom)
    			m_setSocket[i].Send(nPort, nPacket, nVarType, buf);
    	}
    }//*/

    ////////////////////////////////////////////////////////////////////////////////////////
    // 新接口
public:
    // release by: delete pMessagePort;
    // (szKey==""): need not key
    static IMessagePort* CreateNew(int nPortID, int nPortSize, LPCTSTR szMasterIP, int nMasterPort, LPCTSTR szKey);

    ////////////////////////////////////////////////////////////////////////////////////////
protected:
    // 服务端(主节点)的IP地址和端口号
    int				m_nPortSize;
    char			m_szMasterIP[IPSTR_SIZE];
    int				m_nMasterPort;
    char			m_szKey[MAX_KEYSIZE];

    typedef	vector<SOCKET> PORT_SET;
    PORT_SET		m_setSocket;				// 0~(size-1)为发送SOCKET表，自己为INVALID_SOCKET。服务端：后面的为刚accept()的SOCKET，还不知道ID的接口。(用于服务端)
    SOCKET			m_sockMain;					// listend()的SOCKET、或者connect()的SOCKET。

    typedef	vector<CRecvBuffer> BUFFER_SET;
    BUFFER_SET		m_setRecvBuf;				// 服务端每个子节点一个BUF，客户端只有1个。

    // 仅用于控制windows Startup & Cleanup
    bool			m_bWinStartup;
};



#endif // INTERNETPORT_H














