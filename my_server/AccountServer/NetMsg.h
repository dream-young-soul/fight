
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// NetMsg.h: interface for the CNetMsg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NETMSG_H__B85F37CC_0971_11D5_8D37_0010B13A12DB__INCLUDED_)
#define AFX_NETMSG_H__B85F37CC_0971_11D5_8D37_0010B13A12DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "define.h"
#include <windows.h>

class CGameSocket;

class CNetMsg
{
public:
    CNetMsg();
    virtual ~CNetMsg();

    //------------ static ----------------
    static HANDLE	s_hHeap;
    static UINT		s_uNumAllocsInHeap;

    static BOOL		IsValidPt(void* p);

    void*	operator new		(size_t size);
    void	operator delete		(void* p);

    void operator = (CNetMsg& msg);

    //------------ common ----------------
    void	Init(void);
    void	Reset(void)	{Init();}

    const	USHORT	GetType	(void)		{return m_unMsgType;}
    const	USHORT	GetSize	(void)		{return m_unMsgSize;}

    char*	GetBuf(void)	{return m_bufMsg;}

    //------------ virtual ----------------
    virtual BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    virtual void			Process		(CGameSocket* pSocket);

public:
    static int		GetMaxSize	(void)	{return _MAX_MSGSIZE;}
    static USHORT	GetType		(char* pbufMsg, DWORD dwMsgSize);
    static USHORT	GetSize		(char* pbufMsg, DWORD dwMsgSize);

//	static CNetMsg*	CreateMsg	(char* pbufMsg, DWORD dwMsgSize);

//	static void		DumpMsg		(CNetMsg* pMsg);
//	static void		DumpMsg		(void* pBuf, DWORD dwSize);

    static BOOL		SendMsg		(CGameSocket* pSocket, void* pbuf, DWORD dwSize);
    static BOOL		SendSysMsg	(CGameSocket* pSocket, char* pszMsg, unsigned short unTxtAtr = _TXTATR_NORMAL);


protected:
    union
    {
        char	m_bufMsg[_MAX_MSGSIZE];
        struct
        {
            USHORT	m_unMsgSize;
            USHORT	m_unMsgType;
        };
    };

public:
    DWORD		m_dwAnyData;
};

#include <deque>
using namespace std;
typedef deque<CNetMsg* >  DEQUE_NETMSG;

#endif // !defined(AFX_NETMSG_H__B85F37CC_0971_11D5_8D37_0010B13A12DB__INCLUDED_)
