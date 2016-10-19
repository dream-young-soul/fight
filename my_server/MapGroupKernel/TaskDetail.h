
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// TaskDetail.h: interface for the CTaskDetail class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TASKDETAIL_H__0223CFB9_198C_474D_B3EF_39531B04F1CD__INCLUDED_)
#define AFX_TASKDETAIL_H__0223CFB9_198C_474D_B3EF_39531B04F1CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma warning(disable:4786)
#include "define.h"
#include <vector>
#include "TaskDetailData.h"
class IDatabase;
class IRole;

class CTaskDetail
{
public:
    CTaskDetail();
    virtual ~CTaskDetail();

    static CTaskDetail* CreateNew() {return new CTaskDetail;}
    bool	CreateNewTaskDetailData(ST_TASKDETAILDATA* pInfo);
public:
    bool	Create(PROCESS_ID idProcess, CUser* pUser);
    bool	Release()	{delete this; return true;}
public:
    CTaskDetailData* QueryData(OBJID idData);
    CTaskDetailData* QueryData(OBJID idUser, OBJID idTask);
    bool	FindNextData(int& nIter);
    bool	FindData(OBJID  idTask, int nPhase, int nCompleteNum, int& nIter);
    bool	FindNextData(OBJID idUser, int& nIter);
    bool	DeleteData(OBJID idUser, OBJID idTask);

protected:
    typedef std::vector<CTaskDetailData*>	TASKDETAILDATA_SET;
    TASKDETAILDATA_SET	m_setData;
    PROCESS_ID		m_idProcess;

    MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_TASKDETAIL_H__0223CFB9_198C_474D_B3EF_39531B04F1CD__INCLUDED_)
