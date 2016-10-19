
//**********************************************************
// 代码编辑器
//**********************************************************

// WorldThread.h: interface for the CWorldThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORLDTHREAD_H__F8D42B39_4635_4C70_8EE1_36B8B497546F__INCLUDED_)
#define AFX_WORLDTHREAD_H__F8D42B39_4635_4C70_8EE1_36B8B497546F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ThreadBase.h"
//#include "WorldKernel.h"

class IMessagePort;
class CWorldThread : public CThreadBase
{
public:
    CWorldThread(IMessagePort* pPort);
    virtual ~CWorldThread();

public: // overload
    virtual	bool	CreateThread(bool bRun = true);		// false: 暂不运行，用 ResumeThread() 运行

    /////////////////////////////////////////////////////////////////////
protected:	// 派生用
    //overrideable
    virtual	void	OnInit();
    virtual bool	OnProcess();		// 不需要返回DWORD
    virtual void	OnDestroy();

protected: // 核心对象及接口
    clock_t m_tNextClock;
    IWorld*			m_pWorld;
    IMessagePort*	m_pMsgPort;

    HANDLE	m_hMutexThread;
};

#endif // !defined(AFX_WORLDTHREAD_H__F8D42B39_4635_4C70_8EE1_36B8B497546F__INCLUDED_)
