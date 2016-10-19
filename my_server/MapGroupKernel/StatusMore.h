
//**********************************************************
// 代码编辑器
//**********************************************************

// StatusOnce.h: interface for the CStatusMore class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATUSONCE_H__D1AD5A1E_58D9_49C0_ACE9_B9A0C4B20336__INCLUDED_2)
#define AFX_STATUSONCE_H__D1AD5A1E_58D9_49C0_ACE9_B9A0C4B20336__INCLUDED_2

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "I_Role.h"

const int	STATUS_FLASH_REMAIN_MS		= 5000;			// 提前5秒闪烁

struct StatusInfoStruct
{
    int	nStatus;
    int nPower;
    int	nSecs;
    int	nTimes;				// =0 : 一次性状态CStatusMore
};

struct StatusInfoStruct;
class CStatusMore : IStatus
{
protected:
    CStatusMore();
    virtual ~CStatusMore();
public:
    static CStatusMore* CreateNew()			{ return new CStatusMore; }
    ULONG	ReleaseByOwner()				{ delete this; return 0; }
    OBJID	GetID()							{ return m_nStatus; }
    bool	Create(IRole* pRole, int nStatus, int nPower, int nSecs, int nTimes)
    { m_pOwner = pRole; m_nStatus = nStatus; m_nData = nPower; m_tKeep.Startup(nSecs); m_nTimes = nTimes; return true; }
    IStatus*	QueryInterface()			{ return (IStatus*)this; }

public: // application
    bool		IsValid()					{ return m_nTimes > 0; }
    int			GetPower();	//					{ return m_nData; }
    bool		GetInfo(StatusInfoStruct* pInfo);
    void		OnTimer(DWORD tCurr);
    bool		ChangeData(int nPower, int nSecs, int nTimes = 0);		// =0 : status once
    bool		IncTime(int nMilliSecs, int nLimit)			{ return false; }
    bool		ToFlash();

protected:
    IRole*		m_pOwner;
    int			m_nData;
    int			m_nStatus;
    CTimeOut	m_tKeep;
    int			m_nTimes;
    auto_long	m_bFlash;

    MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_STATUSONCE_H__D1AD5A1E_58D9_49C0_ACE9_B9A0C4B20336__INCLUDED_2)
