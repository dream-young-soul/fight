
//**********************************************************
// 代码编辑器
//**********************************************************

// NetMsg.h: interface for the CNetMsg class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _NETMSG_H
#define _NETMSG_H

#pragma	warning(disable:4786)

#include "basefunc.h"
#include "NetworkDef.h"
#include "define.h"
#include "myheap.h"



class CNetMsg
{
public:
    CNetMsg();
    virtual ~CNetMsg();

    void	Init(void);
    void	Reset(void)	{Init();}

    bool	IsNpcMsg()			{return m_idSocket == SOCKET_NONE;}
    SOCKET_ID	GetSocketID()	{return m_idSocket;}
    OBJID		GetNpcID()		{return m_idNpc;}
    OBJID	GetType	(void)		{return m_unMsgType;}
    int		GetSize	(void)		{return m_unMsgSize;}

    char*	GetBuf(void)	{return m_bufMsg;}

public:
    virtual BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    virtual BOOL			IsValid		(void);
    //	virtual void			Send		(ISocket* pSocket);
    virtual void			Process		(void* pInfo);
    virtual bool			SendMsg		(CNetMsg* pMsg);

public:
    static int	GetMaxSize	(void)		{return _MAX_MSGSIZE;}

    static CNetMsg*	CreateClientMsg	(PROCESS_ID idProcess, SOCKET_ID idSocket, OBJID idMsg,
                                     const char* pbufMsg, DWORD dwMsgSize, int nTrans);
    static CNetMsg*	CreateNpcMsg	(PROCESS_ID idProcess, OBJID idNpc, OBJID idMsg,
                                     const char* pbufMsg, DWORD dwMsgSize, int nTrans);
    static CNetMsg*	CreateMsg	(OBJID idMsg, const char* pbufMsg, DWORD dwMsgSize);

public:
    void	AppendInfo(PROCESS_ID idProcess, SOCKET_ID idSocket, OBJID idNpc)	// 仅由CUser,CUserManager中的SendMsg调用
    { m_idProcess = idProcess; m_idSocket = idSocket; m_idNpc = idNpc; }

protected:
    OBJID		m_unMsgType;
    int			m_unMsgSize;
    char		m_bufMsg[_MAX_MSGSIZE];

    PROCESS_ID	m_idProcess;
    SOCKET_ID	m_idSocket;
    OBJID		m_idNpc;

public: // trans intra message
    void		SetTransData(int nData)				{ m_nTransData = nData; }
    int			GetTransData()						{ return m_nTransData; }
protected:
    int			m_nTransData;

    //@暂屏蔽	MYHEAP_DECLARATION(s_heap)
};

#endif
