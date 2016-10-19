
//**********************************************************
// 代码编辑器
//**********************************************************

// MapGroupKernel.h: interface for the CMapGroupKernel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPGROUPKERNEL_H__5901B393_10B2_410C_A305_C639044FF8B2__INCLUDED_)
#define AFX_MAPGROUPKERNEL_H__5901B393_10B2_410C_A305_C639044FF8B2__INCLUDED_

#include <time.h>
#include <winsock2.h>
#include "I_mydb.h"
#include "define.h"	// Added by ClassView
#include "usermanager.h"
#include "network\\NetMsg.h"
#include "I_Shell.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMapGroupKernel : public IMapGroup, ISocket
{
public:
    CMapGroupKernel() {}		// 建议留空
    virtual ~CMapGroupKernel() {}		// 建议留空

public:
    IMapGroup*	GetInterface() { return (IMapGroup*)this; }
    ISocket*	GetSocketInterface() { return (ISocket*)this; }

protected: // IMapGroup
    virtual bool	Create(IMessagePort* pPort);
    virtual void	SynchroData();
    virtual bool	ProcessMsg(OBJID idPacket, void* buf, int nType, int nFrom);
    virtual bool	OnTimer(time_t tCurr);
    virtual bool	Release();

protected: // ISocket
    bool SendMsg			(CNetMsg* pNetMsg);
    bool SendMsg			(SOCKET_ID idSocket, OBJID idMsg, const char* pBuf, int nMsgLen);
    bool SendNpcMsg			(OBJID idNpc, OBJID idMsg, const char* pBuf, int nMsgLen);
    bool CloseSocket		(SOCKET_ID idSocket);		// 直接关闭socket

protected:
    bool ProcessClientMsg	(SOCKET_ID idSocket, OBJID idMsg, const char* pBuf, int nMsgLen, int nTrans = 0);
    bool ProcessNpcMsg		(OBJID idNpc, OBJID idMsg, const char* pBuf, int nMsgLen, int nTrans = 0);
    bool BroadcastMapGroupMsg		(SOCKET_ID idSocket, OBJID idMsg, const char* pBuf, int nMsgLen);
    PROCESS_ID	GetProcessID() { return m_idProcess; }

protected:
    PROCESS_ID		m_idProcess;
    IDatabase*		m_pDb;
    IMessagePort*	m_pMsgPort;

public:
    MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_MAPGROUPKERNEL_H__5901B393_10B2_410C_A305_C639044FF8B2__INCLUDED_)
