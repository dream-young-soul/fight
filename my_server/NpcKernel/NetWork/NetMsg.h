
//**********************************************************
// 代码编辑器
//**********************************************************

// NetMsg.h: interface for the CNetMsg class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _NETMSG_H
#define _NETMSG_H

#pragma	warning(disable:4786)
#include "NetworkDef.h"
#include "define.h"



class CNetMsg
{
public:
    CNetMsg();
    virtual ~CNetMsg();

    void	Init(void);
    void	Reset(void)	{Init();}

    SOCKET_ID	GetNpcID()	{return m_idNpc;}
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

    static CNetMsg*	CreateMsg	(OBJID idNpc, OBJID idMsg, const char* pbufMsg, DWORD dwMsgSize);

public:
    //	void	AppendInfo(PROCESS_ID idProcess, SOCKET_ID idSocket)	// 仅由CUser,CUserManager中的SendMsg调用
    //			{ m_idProcess = idProcess; m_idSocket = idSocket; }

protected:
    OBJID		m_unMsgType;
    int			m_unMsgSize;
    char		m_bufMsg[_MAX_MSGSIZE];

    OBJID		m_idNpc;
};

#endif
