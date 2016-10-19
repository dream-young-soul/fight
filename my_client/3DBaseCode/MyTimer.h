
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MyTimer.h: interface for the CMyTimer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYTIMER_H__81BEF98D_8521_4566_9A8E_06BD8510FD73__INCLUDED_)
#define AFX_MYTIMER_H__81BEF98D_8521_4566_9A8E_06BD8510FD73__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "basefunc.h"

class CMyTimer
{
public:
    CMyTimer();
    virtual ~CMyTimer();

public:
    virtual void	Update	()							{ m_tUpdateTime = ::TimeGet(); }
    virtual bool	IsTimeOut	()						{ return (IsActive() && ::TimeGet() >= m_tUpdateTime + m_dwInterval); }
    virtual bool	ToNextTime	()						{ if (IsTimeOut()) { return Update(), true; } else { return false; } }
    virtual void	SetInterval	(DWORD dwMilliSecs)		{ m_dwInterval = dwMilliSecs; }

    virtual void	Startup		(DWORD dwMilliSecs)		{ m_dwInterval = dwMilliSecs; Update(); }
    virtual bool	TimeOver	()						{ if (IsActive() && IsTimeOut()) { return Clear(), true; } return false; }
    virtual void	Clear		()						{ m_dwInterval = m_tUpdateTime = 0; }
    virtual bool	IsActive	()						{ return m_tUpdateTime != 0; }

    DWORD	GetInterval	()						{ return m_dwInterval; }
    DWORD	GetUpdateTime()						{ return m_tUpdateTime; }

protected:
    DWORD	m_dwInterval;
    DWORD	m_tUpdateTime;
};

class CMyCounter : public CMyTimer
{
public:
    CMyCounter(DWORD dwTicks = 1) { m_dwTicks = dwTicks; }
    virtual ~CMyCounter() {}

    bool	ToNextTime	()	{ if (CMyTimer::ToNextTime()) { IncTicks(-1); return true; } else { return false; } }
    bool	TimeOver	()	{ return ( m_dwTicks <= 0 ? Clear(), true : false ); }

    DWORD	GetTicks	()				{ return m_dwTicks; }
    void	SetTicks	(DWORD dwTicks)	{ m_dwTicks = dwTicks; }
    DWORD	IncTicks	(int nTicks)	{ m_dwTicks = __max(0, (int)m_dwTicks + nTicks); return m_dwTicks; }

private:
    DWORD	m_dwTicks;
};

#endif // !defined(AFX_MYTIMER_H__81BEF98D_8521_4566_9A8E_06BD8510FD73__INCLUDED_)
