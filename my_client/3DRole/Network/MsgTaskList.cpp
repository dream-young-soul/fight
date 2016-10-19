
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

void CMsgTaskList::Process(void* pInfo)
{
    int nAmount = m_pInfo->usAmount;
    if (0 == nAmount || TASKLIST_NONE == m_pInfo->usType)
    {
        return;
    }
    else
    {
        g_objHero.ClearPlayerTaskList();
        g_objHero.SetPlayerTaskListType(m_pInfo->usType);
        for(int i = 0; i < nAmount; i++)
        {
            g_objHero.AddPlayerTaskList(m_pInfo->setTask[i].idTask,
                                        m_pInfo->setTask[i].szTitle,
                                        m_pInfo->setTask[i].ucReqLev);
        }
        ::PostCmd(CMD_FLASH_PLAYERTASKLIST);
    }
}
