
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MsgTaskList.cpp: implementation of the CMsgTaskList class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgTaskList::CMsgTaskList()
{
    Init();
    m_pInfo	= (MSG_Info*)m_bufMsg;
}

CMsgTaskList::~CMsgTaskList()
{
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgTaskList::Create(char* pbufMsg, DWORD dwMsgSize)
{
    if (!CNetMsg::Create(pbufMsg, dwMsgSize))
    {
        return false;
    }
    if (_MSG_TASK_LIST != this->GetType())
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgTaskList::Create(USHORT usType, USHORT usAmount, ST_TASK_TITLE* setTask)
{
    this->Init();
    m_unMsgType	= _MSG_TASK_LIST;
    m_unMsgSize	= sizeof(MSG_Info) - sizeof(ST_TASK_TITLE) + usAmount * sizeof(ST_TASK_TITLE);
    ASSERT (m_unMsgSize <= MAX_PACKETSIZE);
    m_pInfo->usType			= usType;
    m_pInfo->usAmount		= usAmount;
    if (usAmount > 0)
    {
        CHECKF (setTask);
    }
    for (int i = 0; i < usAmount; i++)
    {
        m_pInfo->setTask[i].idTask		= setTask[i].idTask;
        m_pInfo->setTask[i].ucReqLev	= setTask[i].ucReqLev;
        ::SafeCopy(m_pInfo->setTask[i].szTitle, setTask[i].szTitle, 32);
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
BOOL CMsgTaskList::Append(OBJID idTask, UCHAR ucReqLev, const char* pszTitle)
{
    CHECKF (pszTitle);
    m_pInfo->usAmount++;
    m_unMsgSize	= sizeof(MSG_Info) - sizeof(ST_TASK_TITLE) + m_pInfo->usAmount * sizeof(ST_TASK_TITLE);
    ASSERT (m_unMsgSize <= MAX_PACKETSIZE);
    m_pInfo->setTask[m_pInfo->usAmount-1].idTask	= idTask;
    m_pInfo->setTask[m_pInfo->usAmount-1].ucReqLev	= ucReqLev;
    ::SafeCopy(m_pInfo->setTask[m_pInfo->usAmount-1].szTitle, pszTitle, 32);
    return true;
}

//////////////////////////////////////////////////////////////////////
void CMsgTaskList::Process(void* pInfo)
{
#ifdef _MYDEBUG
    ::LogSave("Process CMsgTaskList: type=%u, amount=%u, index=%u",
              m_pInfo->usType, m_pInfo->usAmount, m_pInfo->dwIndexFirst);
#endif
    ASSERT(!"CMsgTaskList::Process");
}

