
//**********************************************************\\
// ´úÂë±à¼­Æ÷\\
//**********************************************************\\
\\
// UserTaskData.h: interface for the CUserTaskData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERTASKDATA_H__1BC3B0A3_3F9C_4EF7_B9E0_93149676DADB__INCLUDED_)
#define AFX_USERTASKDATA_H__1BC3B0A3_3F9C_4EF7_B9E0_93149676DADB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameObj.h"
#include "GameData.h"
#include "MyHeap.h"

//////////////////////////////////////////////////////////////////////
enum USERTASKDATA
{
    USERTASKDATA_ID_ = 0,
    USERTASKDATA_ID_OWNER,
    USERTASKDATA_ID_TASK,
    USERTASKDATA_STEP,
    USERTASKDATA_TIMES,
};

//////////////////////////////////////////////////////////////////////
char	szTaskDataTable[];
typedef	CGameData<USERTASKDATA, szTaskDataTable, szID>	CGameUserTaskData;


//////////////////////////////////////////////////////////////////////
class IDatabase;
class IRecordset;
class CUserTaskData
{
public:
    CUserTaskData();
    virtual ~CUserTaskData();



public:
    bool	Create			(IRecordset* pRes);
    //	bool	Create			(OBJID id, IDatabase* pDb);
    bool	CreateRecord	(OBJID idOwner, OBJID idTask, UCHAR ucStep, UCHAR ucTimes, IDatabase* pDb);
    bool	DeleteRecord	();

public:
    OBJID	GetID()		{ return m_pData->GetKey(); }

    int		GetInt(USERTASKDATA idx);
    void	SetInt(USERTASKDATA idx, int nData, bool bUpdate = false);

protected:
    CGameUserTaskData*	m_pData;

public:
    MYHEAP_DECLARATION(s_heap)
};



#endif // !defined(AFX_USERTASKDATA_H__1BC3B0A3_3F9C_4EF7_B9E0_93149676DADB__INCLUDED_)
