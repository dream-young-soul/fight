
//**********************************************************\\
// 代码编辑器\\
//**********************************************************\\
\\
// UserTaskData.cpp: implementation of the CUserTaskData class.
//
//////////////////////////////////////////////////////////////////////

#include "UserTaskData.h"
#include "I_mydb.h"

char	szTaskDataTable[]	= _TBL_TASKDATA;

MYHEAP_IMPLEMENTATION(CUserTaskData, s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUserTaskData::CUserTaskData()
    : m_pData(NULL)
{
}

CUserTaskData::~CUserTaskData()
{
    SAFE_RELEASE (m_pData);
}

//////////////////////////////////////////////////////////////////////
bool CUserTaskData::Create(IRecordset* pRes)
{
    CHECKF (!m_pData);
    CHECKF (pRes);
    m_pData = CGameUserTaskData::CreateNew();
    IF_NOT (m_pData)
    return false;
    IF_NOT (m_pData->Create(pRes))
    return false;
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CUserTaskData::CreateRecord(OBJID idOwner, OBJID idTask, UCHAR ucStep, UCHAR ucTimes, IDatabase* pDb)
{
    CHECKF (!m_pData);
    CHECKF (pDb);
    CHECKF (idOwner != ID_NONE);
    CHECKF (idTask != ID_NONE);
    // 简单起见，这里直接使用sql语句插入记录
    SQLBUF szSQL;
    sprintf(szSQL, "INSERT INTO %s (id_owner, id_task, step, times) VALUES (%u, %u, %u, %u)",
            _TBL_TASKDATA, idOwner, idTask, ucStep, ucTimes);
    IF_NOT (pDb->ExecuteSQL(szSQL))
    return false;
    OBJID id = pDb->GetInsertId();
    m_pData = CGameUserTaskData::CreateNew();
    IF_NOT (m_pData)
    return false;
    IF_NOT (m_pData->Create(id, pDb))
    return false;
    return true;
}
//////////////////////////////////////////////////////////////////////
bool CUserTaskData::DeleteRecord()
{
    return m_pData->DeleteRecord();
}

//////////////////////////////////////////////////////////////////////
int CUserTaskData::GetInt(USERTASKDATA idx)
{
    return m_pData->GetInt(idx);
}

//////////////////////////////////////////////////////////////////////
void CUserTaskData::SetInt(USERTASKDATA idx, int nData, bool bUpdate /*= false*/)
{
    m_pData->SetInt(idx, nData);
    if (bUpdate)
    {
        m_pData->Update();
    }
}
