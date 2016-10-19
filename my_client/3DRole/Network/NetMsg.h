
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// NetMsg.h: interface for the CNetMsg class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _NETMSG_H
#define _NETMSG_H

#include "basefunc.h"
#include "NetworkDef.h"
#include "socket.h"

class CNetwork
{
public:
    CNetwork();
    virtual ~CNetwork()	{Destroy();}

    BOOL	Init		(const char* pszServerIP, int nServerPort);
    void	Destroy		(void);

    BOOL	IsReady	()	{return m_bReady;}
    BOOL	SendMsg	(char* pbufMsg, DWORD dwSize);

    DWORD	GetMsgCount	(void);
    void	AddMsgCount	(int nAddValue = 1);

    DWORD	m_dwMsgCount;

#ifdef	ENCRYPT
    void	ChangeCode(DWORD dwCode)	{if (m_pSocket) { m_pSocket->ChangeCode(dwCode); }}
#endif
    void	ProcessNetMsg(void);

    DWORD	m_dwBytesReceived;
    DWORD	m_dwBytesSend;

private:

    CLoginSocket*	m_pSocket;
    BOOL			m_bReady;
};

class CNetMsg
{
public:
    CNetMsg();
    virtual ~CNetMsg();

    void	Init(void);
    void	Reset(void)	{Init();}

    const unsigned short	GetType	(void)		{return m_unMsgType;}
    const unsigned short	GetSize	(void)		{return m_unMsgSize;}

    char*	GetBuf(void)	{return m_bufMsg;}

public:
    virtual BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    virtual BOOL			IsValid		(void);
    virtual void			Send		(void);
    virtual void			Process		(void* pInfo);

public:
    static int	GetMaxSize	(void)		{return _MAX_MSGSIZE;}
    static unsigned short	GetType		(char* pbufMsg, DWORD dwMsgSize);
    static unsigned short	GetSize		(char* pbufMsg, DWORD dwMsgSize);

    static CNetMsg*	CreateMsg	(char* pbufMsg, DWORD dwMsgSize);

protected:
    union
    {
        char	m_bufMsg[_MAX_MSGSIZE];
        struct
        {
            unsigned short	m_unMsgSize;
            unsigned short	m_unMsgType;
        };
    };
};

//--------------------------------------------------------------
extern CNetwork	g_objNetwork;
extern CHAR		g_szServerName[256];
#endif
