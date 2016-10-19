
//**********************************************************
// 代码编辑器
//**********************************************************

// SocketThread.h: interface for the CSocketThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOCKETTHREAD_H__FB24259F_D321_441C_A6C7_33970E96B6C3__INCLUDED_)
#define AFX_SOCKETTHREAD_H__FB24259F_D321_441C_A6C7_33970E96B6C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ThreadBase.h"
#include "TimeOut.h"

class IMessagePort;
class CSocketThread : public CThreadBase
{
public:
    CSocketThread(IMessagePort* pPort);
    virtual ~CSocketThread();

public: // overload
    virtual	bool	CreateThread(bool bRun = true);		// false: 暂不运行，用 ResumeThread() 运行

    /////////////////////////////////////////////////////////////////////
protected:	// 派生用
    //overrideable
    virtual	void	OnInit();
    virtual bool	OnProcess();		// 不需要返回DWORD
    virtual void	OnDestroy();

protected: // 核心对象及接口
    CTimeOutMS		m_tProcessSocket;
    clock_t m_tNextClock;
    ISocketKernel*	m_pServerSocket;
    IMessagePort*	m_pMsgPort;

protected: // ctrl
    HANDLE	m_hMutexThread;
};

#endif // !defined(AFX_SOCKETTHREAD_H__FB24259F_D321_441C_A6C7_33970E96B6C3__INCLUDED_)
