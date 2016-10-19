
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// TaskDetailData.cpp: implementation of the CTaskDetailData class.
//
//////////////////////////////////////////////////////////////////////

#include "TaskDetailData.h"

char	szTaskDetailTable[] = _TBL_TASKDETAIL;

MYHEAP_IMPLEMENTATION(CTaskDetailData, s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTaskDetailData::CTaskDetailData()
{
    m_pData = NULL;
}

CTaskDetailData::~CTaskDetailData()
{
    SAFE_RELEASE (m_pData);
}

bool CTaskDetailData::Create(IRecordset* pRes, IDatabase* pDb)
{
    CHECKF (!m_pData);
    CHECKF (pRes);
    m_pData	= CGameTaskDetailData::CreateNew();
    IF_NOT (m_pData)
    return false;
    IF_NOT (m_pData->Create(pRes))
    return false;
    return true;
}

bool CTaskDetailData::CreateRecord(ST_TASKDETAILDATA* pInfo, IDatabase* pDb)
{
    CHECKF (!m_pData);
    CHECKF (pInfo);
    CHECKF (pDb);
    m_pData = CGameTaskDetailData::CreateNew();
    IF_NOT (m_pData)
    return false;
    IF_NOT (m_pData->Create(pDb, ID_NONE))
    return false;
    m_pData->SetInt(TASKDETAILDATA_USERID, pInfo->idUser);
    m_pData->SetInt(TASKDETAILDATA_TASKID, pInfo->idTask);
    m_pData->SetInt(TASKDETAILDATA_TASKPHASE, pInfo->nTaskPhase);
    m_pData->SetInt(TASKDETAILDATA_TASKCOMPLETENUM, pInfo->nCompleteNum);
    m_pData->SetInt(TASKDETAILDATA_TASKBEGINTIME, TimeGet(TIME_SECOND));
    if (ID_NONE == m_pData->InsertRecord())
    {
        return false;
    }
    return true;
}
