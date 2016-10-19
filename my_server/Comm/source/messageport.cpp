
//**********************************************************
// 代码编辑器
//**********************************************************

// 接口类。接口：基于消息通讯机制的线程间通讯接口。
// 仙剑修，2002.8.28

#include "MessagePort.h"
#include "BaseFunc.h"
#include <stdlib.h>

using namespace message_port;


long	g_nMessagePortErrorCount;
///////////////////////////////////////////////////////////////////////////////////////
// interface
///////////////////////////////////////////////////////////////////////////////////////
// 初始化，设置接口ID号，开始接收消息。可重复调用(PORT_ID不能改变)。
bool CMessagePort::Open	()
{
    LOCKOBJ;
    m_nState = STATE_OK;
    for(MSG_SET::iterator iter = m_setMsg.begin(); iter != m_setMsg.end(); iter++)
    {
        m_setRecycle.push(*iter);
    }
    m_setMsg.clear();
    ::ResetEvent(m_hHaveMsg);
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////
// 发送消息到指定接口。包含消息ID、数据类型、数据。
bool CMessagePort::Send	(int nPort, int nPacket, VAR_TYPE nVarType, const void* buf)
{
    LOCKOBJ;		//VVVVV
    ASSERT(buf);
    if (!IsOpen())
    {
        return false;
    }
    if (nPort == INVALID_PORT)
    {
        return true;
    }
    if (nPort < 0 || nPort >= m_setPort.size())
    {
        ASSERT(!"nPort");
        return false;
    }
    UNLOCKOBJ;		//AAAAA ★调用外部接口，释放互斥。必须释放，否则死锁★
    return m_setPort[nPort]->PushMsg(m_id, nPacket, nVarType, buf);
}

///////////////////////////////////////////////////////////////////////////////////////
// 接收指定接口(或所有接口)发来的消息。可指定消息ID，也可不指定。
bool CMessagePort::Recv	(int nPort, int nPacket, VAR_TYPE nVarType, void* buf, CMessageStatus* pStatus /*= NULL*/)	// return false: 没数据
{
    LOCKOBJ;
    ASSERT(buf);
    if (m_nState == STATE_CLOSED)
    {
        if (pStatus)
        {
            pStatus->m_nError	= STATUS_FLAG_CLOSE;
        }
        return false;
    }
    int	nRcvPort = nPort, nRcvPacket = nPacket;
    VAR_TYPE	nRcvVarType;
    if (PopMsg(&nRcvPort, &nRcvPacket, &nRcvVarType, buf))		// 内部函数，不用打开互斥锁
    {
        // 检查类型
        if (SIZE_OF_TYPE(nRcvVarType) > SIZE_OF_TYPE(nVarType))
        {
            ASSERT(!"VarType");
            if (pStatus)
            {
                pStatus->m_nError		= STATUS_FLAG_ERROR;    //? 以后可支持自动转换类型
            }
            return false;
        }
        if (pStatus)
        {
            pStatus->m_nPortFrom	= nRcvPort;
            pStatus->m_nPacket		= nRcvPacket;
            pStatus->m_nVarType		= nRcvVarType;
            pStatus->m_nError		= STATUS_FLAG_OK;
        }
        return true;
    }
    if (pStatus)
    {
        pStatus->m_nError	= STATUS_FLAG_OK;
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////////////
// 接收指定接口(或所有接口)发来的消息。可指定消息ID，也可不指定。return false: 超时或错误。
bool CMessagePort::WaitMsg(int nMilliSec)
{
    LOCKOBJ;
    if (m_nState == STATE_CLOSED)
    {
        return false;
    }
    // 原子操作
    if (m_setMsg.size())
    {
        return true;
    }
    ::ResetEvent(m_hHaveMsg);		// 因为PushMsg()的SetEvent是在LOCK状态调用的，size()和Event有原子性，无共享冲突。
    UNLOCKOBJ;		//AAAAA ★等待其它线程的SEND()调用，释放互斥。必须释放，否则死锁★
    DWORD	ret = ::WaitForSingleObject(m_hHaveMsg, nMilliSec);
    if (ret == WAIT_OBJECT_0)
    {
        return true;
    }
    if (ret == WAIT_ABANDONED)
    {
        LOCKOBJ;		//? 内部调用，锁定
        Close();		// 本地函数，不用打开互斥锁
        return false;
    }
    else // if (ret == WAIT_TIMEOUT)
    {
        return false;
    }
}

///////////////////////////////////////////////////////////////////////////////////////
// 关闭接口，不再接收消息。可重复调用。
bool CMessagePort::Close()
{
    LOCKOBJ;
    m_nState = STATE_CLOSED;
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////
// CMessagePort
///////////////////////////////////////////////////////////////////////////////////////
int CMessagePort::SIZE_OF_TYPE		(int type)
{
    const int	fixlen_type_size = 10;
    const int	size_of[fixlen_type_size] = {1, 1, 2, 2, 4, 4, 4, 4, 4, 8};
    if (type < VARTYPE_NONE)
    {
        return type;
    }
    else if (type == VARTYPE_NONE)
    {
        ASSERT(!"SIZE_OF_TYPE");
        return 0;
    }
    else if (type - (VARTYPE_NONE + 1) < fixlen_type_size)
    {
        return size_of[type - (VARTYPE_NONE+1)];
    }
    ASSERT(!"SIZE_OF_TYPE.");
    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////
bool CMessagePort::PushMsg(int nPort, int nPacket, VAR_TYPE nVarType, const void* buf)	// nData中的串和结构都会被COPY
{
    LOCKOBJ;
    ASSERT(buf);
    if (m_nState != STATE_OK)		// Close()后，不接收消息
    {
        return false;
    }
    if (nPort < 0 || nPort >= m_setPort.size())
    {
        ASSERT(!"PushMsg(nPort)");
        return false;
    }
    int	nSize = SIZE_OF_TYPE(nVarType);
    if (nSize == 0 || nSize > MAX_MSGPACKSIZE)
    {
        ASSERT(!"PushMsg(nSize)");
        return false;
    }
    CMessagePacket*	pMsg = m_setRecycle.pop();
    if (!pMsg)
    {
        pMsg = new CMessagePacket;    // VVVVVVVVVVVVVVVVVVV
    }
    if (!pMsg)
    {
        ASSERT(!"new");
        return false;
    }
    pMsg->m_nPortFrom		= nPort;
    pMsg->m_nPacket			= nPacket;
    pMsg->m_nVarType		= nVarType;
    memcpy(pMsg->m_bufData, buf, nSize);
    // 原子操作
    m_setMsg.push_back(pMsg);		// AAAAAAAAAAAAAAAAAAAAAAAA
    ::SetEvent(m_hHaveMsg);					// 有消息了
    // 检查溢出
    int nSetSize = m_setMsg.size();
    if (nSetSize >= 5000 && (nSetSize % 1000) == 0 && nSetSize != m_debugPackets)		//? 100报警
    {
        m_debugPackets = nSetSize;
        LOGERROR("CMessagePacket[%d]节点的数量达到[%u]。[%d][%d][%s]", m_id, nSetSize, nPort, nPacket, (LPCTSTR)DumpBuffer((char*)buf, __min(16, nSize)));
        InterlockedExchange(&g_nMessagePortErrorCount, nSetSize);
    }
    // 随机取此全局变量
    if (::RandGet(300) == 123)
    {
        InterlockedExchange(&g_nMessagePortErrorCount, nSetSize);
    }
    if (m_setMsg.size() > 100000)
    {
        LOGERROR("ERROR: 消息堆溢出，MessagePort[%d]关闭!", m_id);
        Close();
    }
    return m_setMsg.size() <= 1000;		//? 1000出错
}

///////////////////////////////////////////////////////////////////////////////////////
bool CMessagePort::PopMsg(int* pPort, int* pPacket, VAR_TYPE* pVarType, void* buf)
{
    // 内部函数	LOCKOBJ;
    ASSERT(pPort && pPacket && pVarType && buf);
    if (m_nState != STATE_OK)		// Close()后，不处理消息
    {
        return false;
    }
    if (m_setMsg.size() == 0)
    {
        return false;
    }
    for(MSG_SET::iterator iter = m_setMsg.begin(); iter != m_setMsg.end(); iter++)
    {
        CMessagePacket* pMsg = *iter;
        if ((*pPort == PORT_ANY || *pPort == pMsg->m_nPortFrom) && (*pPacket == PACKET_ANY || *pPacket == pMsg->m_nPacket))
        {
            *pPort		= pMsg->m_nPortFrom;
            *pPacket	= pMsg->m_nPacket;
            *pVarType	= (VAR_TYPE)pMsg->m_nVarType;
            int	nSize = SIZE_OF_TYPE(*pVarType);
            if (nSize)
            {
                memcpy(buf, pMsg->m_bufData, nSize);
            }
            m_setRecycle.push(pMsg);
            m_setMsg.erase(iter);
            return true;
        }
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////////////
bool CMessagePort::Init()
{
    if ((m_hHaveMsg	= ::CreateEvent(NULL, false, false, NULL)) == NULL)
    {
        LOGERROR("无法创建CMessagePort句柄!");
        return false;
    }
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////
void CMessagePort::Clear()
{
    for(MSG_SET::iterator iter = m_setMsg.begin(); iter != m_setMsg.end(); iter++)
    {
        delete(*iter);
    }
    m_setMsg.clear();
    while(m_setRecycle.size())
    {
        delete(m_setRecycle.pop());
    }
    if (m_hHaveMsg != NULL)
    {
        ::CloseHandle(m_hHaveMsg);
    }
    m_hHaveMsg = NULL;
}

///////////////////////////////////////////////////////////////////////////////////////
// static
///////////////////////////////////////////////////////////////////////////////////////
bool CMessagePort::InitPortSet(int nPortNum)
{
    ASSERT(nPortNum >= 1);
    for(int i = 0; i < nPortNum; i++)
    {
        CMessagePort*	pPort = new CMessagePort(i);		// VVVVVVVVVVVVVVVV
        if (!pPort)
        {
            return false;
        }
        m_setPort.push_back(pPort);			// AAAAAAAAAAAAAAAAAAAAAAAAAA
        if (!pPort->Init())
        {
            return false;
        }
    }
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////
void CMessagePort::ClearPortSet()
{
    for(int i = 0; i < m_setPort.size(); i++)
    {
        //m_setPort[i]->Clear();
        delete(m_setPort[i]);
    }
    m_setPort.clear();
}

///////////////////////////////////////////////////////////////////////////////////////
CMessagePort::PORT_SET CMessagePort::m_setPort;
CRITSECT CMessagePort::m_xSetCtrl;			// 用于m_setPort互斥












