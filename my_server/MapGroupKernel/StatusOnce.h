
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// StatusOnce.h: interface for the CStatusOnce class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATUSONCE_H__D1AD5A1E_58D9_49C0_ACE9_B9A0C4B20336__INCLUDED_)
#define AFX_STATUSONCE_H__D1AD5A1E_58D9_49C0_ACE9_B9A0C4B20336__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StatusMore.h"

class CStatusOnce : IStatus
{
protected:
    CStatusOnce();
    virtual ~CStatusOnce();
public:
    static CStatusOnce* CreateNew()			{ return new CStatusOnce; }
    ULONG	ReleaseByOwner()				{ delete this; return 0; }
    OBJID	GetID()							{ return m_nStatus; }
    bool	Create(IRole* pRole, int nStatus, int nPower, int nSecs)
    {
        m_pOwner = pRole;
        m_nStatus = nStatus;
        m_nData = nPower;
        m_tKeep.Startup(nSecs * 1000);
        m_tInterval.Startup(1000);
        return true;
    }
    IStatus*	QueryInterface()			{ return (IStatus*)this; }

public: // application
    bool		IsValid()					{ return (m_tKeep.IsActive() && !m_tKeep.IsTimeOut()); }
    int			GetPower();	//					{ return m_nData; }
    bool		GetInfo(StatusInfoStruct* pInfo);
    void		OnTimer(DWORD tCurr);
    bool		ChangeData(int nPower, int nSecs, int nTimes = 0);		// =0 : status once
    bool		IncTime(int nMilliSecs, int nLimit);
    bool		ToFlash();

protected:
    IRole*		m_pOwner;
    int			m_nData;
    int			m_nStatus;
    CTimeOutMS	m_tKeep;
    auto_long	m_bFlash;
    CTimeOutMS	m_tInterval;

    MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_STATUSONCE_H__D1AD5A1E_58D9_49C0_ACE9_B9A0C4B20336__INCLUDED_)
