
//**********************************************************
// 代码编辑器
//**********************************************************

// NpcTask.cpp: implementation of the CNpcTask class.
//
//////////////////////////////////////////////////////////////////////

#include <windows.h>
#include "define.h"
#include "NpcTask.h"
#include "MapGroup.h"


MYHEAP_IMPLEMENTATION(CNpcTask, s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CNpcTask::CNpcTask()
{
    m_setTask	= NULL;
}

//////////////////////////////////////////////////////////////////////
CNpcTask::~CNpcTask()
{
    if (m_setTask)
    {
        m_setTask->Release();
    }
}

//////////////////////////////////////////////////////////////////////
bool CNpcTask::Create(PROCESS_ID idProcess, CNpc* pOwner, INpcData* pData)
{
    m_idProcess	= idProcess;
    m_pOwner	= pOwner;
    m_setTask	= CTaskSet::CreateNew(false);
    CHECKF(m_setTask);
    for(int i = 0; i < _MAX_NPCTASK; i++)
    {
        OBJID idTask = pData->GetInt((NPCDATA)(NPCDATA_TASK0 + i));
        if (idTask != ID_NONE)
        {
            CTaskData* pTask = TaskSet()->GetObj(idTask);
            if (!pTask)
            {
                LOGWARNING("Can't find task[%u] of npc[%d].", idTask, m_pOwner->GetRecordID());
                continue;
            }
            if (m_setTask->GetObj(idTask))
            {
                LOGWARNING("Duplicate task[%u] of npc[%d].", idTask, m_pOwner->GetRecordID());
                continue;
            }
            m_setTask->AddObj(pTask);
        }
        else
        {
            break;
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CNpcTask::ActivateTask(CUser* pUser)
{
    CHECKF(pUser);
    // 冲掉旧任务
    pUser->ClearTaskID();
    pUser->SetTaskNpcID(m_pOwner->GetID());
    for (int i = 0; i < m_setTask->GetAmount(); i++)
    {
        CTaskData* pTask	= m_setTask->GetObjByIndex(i);
        if (pTask)
        {
            if (pUser->TestTask(pTask))
            {
                GameAction()->ProcessAction(pTask->GetInt(TASKDATA_IDNEXT), pUser, m_pOwner->QueryRole());
                return true;
            }
        }
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
void CNpcTask::OnTimer(DWORD tCurr)
{
    if ((m_pOwner->GetSort()&NPCSORT_EVENT) != 0)
    {
        if (m_bActiveTime)
        {
            if (!::IsActiveTime(tCurr, (m_pOwner->GetData(TIMERNPC_TIME) / 1000) * 1000))
            {
                m_bActiveTime	= false;
            }
        }
        else
        {
            if (::IsActiveTime(tCurr, (m_pOwner->GetData(TIMERNPC_TIME) / 1000) * 1000))
            {
                m_bActiveTime	= true;
                // active
                clock_t	tStart = clock();
                GameAction()->ProcessAction(m_pOwner->GetInt(NPCDATA_LINKID), NULL, m_pOwner->QueryRole());		//? NPC是延时删除的，不需要是最后代码。
                if (clock() - tStart > 1000)
                {
                    ASSERT(!"任务NPC定时任务超时!");
                }
            }
        }
    }
}
