
//**********************************************************
// 代码编辑器
//**********************************************************

// KernelThread.h: interface for the CKernelThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KERNELTHREAD_H__983039DE_C3AB_4C39_9427_7474641D35E7__INCLUDED_)
#define AFX_KERNELTHREAD_H__983039DE_C3AB_4C39_9427_7474641D35E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ThreadBase.h"
#include "ClientSocket.h"
#include "I_Shell.h"

class IShell;
class CKernelThread : public CThreadBase, IShell
{
public:
    CKernelThread();
    virtual ~CKernelThread();

protected: // IShell
    virtual void	PrintText(LPCTSTR szMsg)			{ m_pDialog->PrintText(szMsg); }
    virtual void	CloseAll()							{ m_pDialog->CloseAll(); }
    virtual void	SetState(LPCTSTR szState)			{ m_pDialog->SetState(szState); }
    virtual	void	ChangeEncrypt(DWORD nKey)			{ m_pDialog->ChangeEncrypt(nKey); }
protected:
    virtual bool	SendPacket(const char* pack, int nLen, bool bFlush = false)
    { return m_sockClient.SendPacket(pack, nLen, bFlush); }
    virtual bool	IsConnectOK()
    { return m_sockClient.IsOpen(); }

public: // dynamic link
    virtual	void	OnInit();
    virtual bool	OnProcess();
    virtual	void	OnDestroy();

public: // static link
    bool	CreateThread(IDialog* pDialog, bool bRun = true);		// false: 暂不运行，用 ResumeThread() 运行

protected:
    IDialog*		m_pDialog;
    CClientSocket	m_sockClient;
    IKernel*		m_pKernel;

    clock_t			m_tNextClock;
};

#endif // !defined(AFX_KERNELTHREAD_H__983039DE_C3AB_4C39_9427_7474641D35E7__INCLUDED_)
