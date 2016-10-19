
//**********************************************************
// 代码编辑器
//**********************************************************

// MapGroupThread.h: interface for the CMapGroupThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPGROUPTHREAD_H__946D8B82_F18C_4C03_9985_C5EFD89B9969__INCLUDED_)
#define AFX_MAPGROUPTHREAD_H__946D8B82_F18C_4C03_9985_C5EFD89B9969__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ThreadBase.h"
//#include "MapGroupKernel.h"

class IMessagePort;
class CMapGroupThread : public CThreadBase
{
public:
    CMapGroupThread(IMessagePort* pPort);
    virtual ~CMapGroupThread();

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
    //	CMapGroupKernel	m_cMapGroupKernel;
    IMapGroup*		m_pMapGroup;
    IMessagePort*	m_pMsgPort;

    HANDLE	m_hMutexThread;
};

#endif // !defined(AFX_MAPGROUPTHREAD_H__946D8B82_F18C_4C03_9985_C5EFD89B9969__INCLUDED_)
