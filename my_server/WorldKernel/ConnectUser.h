
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// ConnectUser.h: interface for the CConnectUser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONNECTUSER_H__9394419B_3694_4679_A978_091F18D965C6__INCLUDED_)
#define AFX_CONNECTUSER_H__9394419B_3694_4679_A978_091F18D965C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Timeout.h"

const int	DELETECONNECTUSER_SECS		= 20;		// 20Ãë³¬Ê±£¬É¾³ýÁ¬½Ó±í

class CConnectUser
{
protected:
    CConnectUser();
    virtual ~CConnectUser();
public:
    static CConnectUser* CreateNew()	{ return new CConnectUser; }
    ULONG	ReleaseByOwner()			{ delete this; return 0; }

    OBJID	GetID()						{ return m_idAccount; }
    bool	Create(OBJID id, DWORD dw, LPCTSTR szInfo, SOCKET_ID idSocket = SOCKET_NONE);

public:
    bool	CheckData(DWORD dw)			{ return m_dwCheckData == dw; }
    SOCKET_ID	GetSocketID()			{ return m_idSocket; }
    LPCTSTR	GetInfo()					{ return m_szInfo; }
    bool	IsValid()					{ return !m_tDelete.IsTimeOut(); }

protected:
    OBJID		m_idAccount;
    DWORD		m_dwCheckData;
    NAMESTR		m_szInfo;
    SOCKET_ID	m_idSocket;

    CTimeOut	m_tDelete;

protected: // ctrl
    MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_CONNECTUSER_H__9394419B_3694_4679_A978_091F18D965C6__INCLUDED_)
