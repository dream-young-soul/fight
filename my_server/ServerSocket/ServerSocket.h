
//**********************************************************
// 代码编辑器
//**********************************************************

// ServerSocket.h: interface for the CSocketKernel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVERSOCKET_H__44A9ACE7_FE12_4CDC_9492_F39CD1423D77__INCLUDED_)
#define AFX_SERVERSOCKET_H__44A9ACE7_FE12_4CDC_9492_F39CD1423D77__INCLUDED_

#pragma warning(disable:4786)
#include <time.h>
#include "define.h"
#include <vector>
#include <map>
using namespace std;

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSocketKernel : public ISocketKernel
{
public:
    CSocketKernel() {}		// 建议留空
    virtual ~CSocketKernel() {}		// 建议留空

public:
    ISocketKernel*	GetInterface() { return (ISocketKernel*)this; }

public:
    virtual bool	Create(IMessagePort* pPort);
    virtual bool	ProcessMsg(OBJID idPacket, void* buf, int nType, int nFrom);
    virtual bool	ProcessSocket();
    virtual bool	OnTimer(time_t tCurr);
    virtual bool	Release();

public:
    bool	SendCloseNotify(SOCKET_ID idSocket);
    //? 暂时用于控制只通知核心关闭一次
    bool	IsNetBreakFlag(SOCKET_ID idSocket)			{ return (idSocket >= 0 && idSocket < m_setSocket.size() && m_setSocket[idSocket] && m_setNetBreakFlag[idSocket]); }
    void	SetNetBreakFlag(SOCKET_ID idSocket)			{ if (idSocket >= 0 && idSocket < m_setSocket.size() && m_setSocket[idSocket]) { m_setNetBreakFlag[idSocket] = true; } }

protected:
    void			AcceptNewSocket();

protected:
    IListenSocket*	m_pListen;
    CListenSocket	m_cListen;
    vector<IServerSocket*>	m_setSocket;				// SOCKET_ID对应索引,以下四个SET同步添加删除!
    vector<CServerSocket*>	m_setServerSocket;
    vector<PROCESS_ID>		m_setProcessID;				// PROCESS_ID对应MSGPORT号
    vector<bool>			m_setNetBreakFlag;
    IMessagePort*	m_pMsgPort;
    SOCKET_ID		m_idNpcSocket;
    typedef map<OBJID, PROCESS_ID>	NPCPROCESSID_MAP;
    NPCPROCESSID_MAP	m_setNpcProcessID;			// 每个NPC的PROCESS_ID，新NPC的消息发到WORLD
};

#endif // !defined(AFX_SERVERSOCKET_H__44A9ACE7_FE12_4CDC_9492_F39CD1423D77__INCLUDED_)
