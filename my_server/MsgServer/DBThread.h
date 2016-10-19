
//**********************************************************\\
// 代码编辑器\\
//**********************************************************\\
\\
// DBThread.h: interface for the CDBThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBTHREAD_H__C4AB0D1D_1DF4_465E_AE6B_66FA03A0A160__INCLUDED_)
#define AFX_DBTHREAD_H__C4AB0D1D_1DF4_465E_AE6B_66FA03A0A160__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ThreadBase.h"

class IMessagePort;
class IDBService;
class CDBThread : public CThreadBase
{
public:
    CDBThread(IMessagePort* pPort);
    virtual ~CDBThread();

public: // overload
    virtual	bool	CreateThread(bool bRun = true);		// false: 暂不运行，用 ResumeThread() 运行

    /////////////////////////////////////////////////////////////////////
protected:	// 派生用
    //overrideable
    virtual	void	OnInit();
    virtual bool	OnProcess();		// 不需要返回DWORD
    virtual void	OnDestroy();

protected:
    clock_t m_tNextClock;
    IMessagePort*	m_pMsgPort;

    IDBService*		m_pDBService;
};

#endif // !defined(AFX_DBTHREAD_H__C4AB0D1D_1DF4_465E_AE6B_66FA03A0A160__INCLUDED_)
