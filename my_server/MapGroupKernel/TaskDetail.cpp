
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// TaskDetail.cpp: implementation of the CTaskDetail class.
//
//////////////////////////////////////////////////////////////////////

#include "TaskDetail.h"
#include "I_Role.h"
#include "MapGroup.h"
#include "User.h"
#include "AllMsg.h"


MYHEAP_IMPLEMENTATION(CTaskDetail, s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction]
//////////////////////////////////////////////////////////////////////

CTaskDetail::CTaskDetail()
{
}

CTaskDetail::~CTaskDetail()
{
    TASKDETAILDATA_SET::iterator it = m_setData.begin();
    for (; it != m_setData.end(); it++)
    {
        CTaskDetailData* pData = *it;
        SAFE_RELEASE (pData);
    }
}

//////////////////////////////////////////////////////////////////////
bool CTaskDetail::Create(PROCESS_ID idProcess, CUser* pUser)
{
    CHECKF(pUser);
    m_idProcess	= idProcess;
    SQLBUF	szSQL;
    sprintf(szSQL, "SELECT * FROM %s WHERE userid=%d ORDER BY userid, taskid", _TBL_TASKDETAIL, pUser->GetID());
    IRecordset* pRes = Database()->CreateNewRecordset(szSQL);
    if (pRes)
    {
        for (int i = 0; i < pRes->RecordCount(); i++)
        {
            CTaskDetailData* pData = CTaskDetailData::CreateNew();
            if (pData && pData->Create(pRes, Database()))
            {
                m_setData.push_back(pData);
            }
            else
            {
                SAFE_RELEASE (pData);
                SAFE_RELEASE (pRes);
                return false;
            }
            pRes->MoveNext();
        }
        SAFE_RELEASE (pRes);
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CTaskDetail::CreateNewTaskDetailData(ST_TASKDETAILDATA* pInfo)
{
    CTaskDetailData* pData = CTaskDetailData::CreateNew();
    if (pData)
    {
        if (pData->CreateRecord(pInfo, Database()))
        {
            m_setData.push_back(pData);
            return true;
        }
        SAFE_RELEASE (pData);
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
CTaskDetailData* CTaskDetail::QueryData(OBJID idData)
{
    TASKDETAILDATA_SET::iterator it = m_setData.begin();
    for (; it != m_setData.end(); it++)
    {
        CTaskDetailData* pData = *it;
        if (pData && pData->GetID() == idData)
        {
            return pData;
        }
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////
CTaskDetailData* CTaskDetail::QueryData(OBJID idUser, OBJID idTask)
{
    TASKDETAILDATA_SET::iterator it = m_setData.begin();
    for (; it != m_setData.end(); it++)
    {
        CTaskDetailData* pData = *it;
        if (pData && pData->GetInt(TASKDETAILDATA_USERID) == idUser	&& pData->GetInt(TASKDETAILDATA_TASKID) == idTask)
        {
            return pData;
        }
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////
bool CTaskDetail::DeleteData(OBJID idUser, OBJID idTask)
{
    TASKDETAILDATA_SET::iterator it = m_setData.begin();
    for (; it != m_setData.end(); it++)
    {
        CTaskDetailData* pData = *it;
        if (pData && pData->GetInt(TASKDETAILDATA_USERID) == idUser	&& pData->GetInt(TASKDETAILDATA_TASKID) == idTask)
        {
            pData->DeleteRecord();
            SAFE_RELEASE (pData);
            m_setData.erase(it);
            return true;
        }
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
bool CTaskDetail::FindNextData(OBJID idUser, int& nIter)
{
    OBJID	idLast	= nIter;
    OBJID	idNext	= ID_NONE;
    bool	nBreak		= false;
    TASKDETAILDATA_SET::iterator it = m_setData.begin();
    for (; it != m_setData.end(); it++)
    {
        CTaskDetailData* pData = *it;
        if (pData && pData->GetInt(TASKDETAILDATA_USERID) == idUser)
        {
            if (idNext == ID_NONE)
            {
                idNext	= pData->GetID();
                if (nBreak)
                {
                    break;
                }
                else
                {
                    nBreak = true;
                }
            }
            if (pData->GetID() == idLast)
            {
                idNext	= ID_NONE;
                continue;
            }
        }
    }
    if (idNext)
    {
        nIter = idNext;
        return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
bool CTaskDetail::FindData(OBJID idTask, int nPhase, int nCompleteNum, int& nIter)
{
    TASKDETAILDATA_SET::iterator it = m_setData.begin();
    for (; it != m_setData.end(); it++)
    {
        CTaskDetailData* pData = *it;
        if (nCompleteNum == -1 && nPhase == -1)
        {
            if (pData && pData->GetInt(TASKDETAILDATA_TASKID) == idTask)
            {
                nIter = pData->GetID();
                return true;
            }
        }
        else
        {
            if (pData && pData->GetInt(TASKDETAILDATA_TASKID) == idTask	&& pData->GetInt(TASKDETAILDATA_TASKPHASE) == nPhase
                    && pData->GetInt(TASKDETAILDATA_TASKCOMPLETENUM) == nCompleteNum)
            {
                nIter = pData->GetID();
                return true;
            }
        }
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
bool CTaskDetail::FindNextData(int& nIter)
{
    OBJID	idLast	= nIter;
    OBJID	idNext	= ID_NONE;
    bool	nBreak		= false;
    TASKDETAILDATA_SET::iterator it = m_setData.begin();
    for (; it != m_setData.end(); it++)
    {
        CTaskDetailData* pData = *it;
        {
            if (idNext == ID_NONE)
            {
                idNext	= pData->GetID();
                if (nBreak)
                {
                    break;
                }
                else
                {
                    nBreak = true;
                }
            }
            if (pData->GetID() == idLast)
            {
                idNext	= ID_NONE;
                continue;
            }
        }
    }
    if (idNext)
    {
        nIter = idNext;
        return true;
    }
    return false;
}
