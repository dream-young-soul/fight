
//**********************************************************
// 代码编辑器
//**********************************************************

// MercenaryTask.h: interface for the CMercenaryTask class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MERCENARYTASK_H__D03D9691_EAAD_4725_AFCD_834ED8150B01__INCLUDED_)
#define AFX_MERCENARYTASK_H__D03D9691_EAAD_4725_AFCD_834ED8150B01__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)
#include "define.h"
//#include "ConstGameData.h"
//#include "GameData.h"
//#include "GameObj.h"
//#include "Myheap.h"
#include "TimeOut.h"
#include "MercenaryTaskData.h"
#include <vector>


//////////////////////////////////////////////////////////////////////

//typedef ISingleMap2<CMercenaryTaskData>		IMTaskDataSet;
//typedef CSingleMap2<CMercenaryTaskData>		CMTaskDataSet;




//////////////////////////////////////////////////////////////////////
// 目前不考虑开多个MapGroup，为了加快工作进度，佣兵任务暂时不做成世界同步
// 如果开了多个MapGroup，就必须修改这部分，象帮派系统一样做世界同步处理
//      —— zlong 2004.11.12
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
class IDatabase;
class IRole;
class CMercenaryTask	//: public CMercenaryTaskData
{
public:
    CMercenaryTask();
    virtual ~CMercenaryTask();

public:
    static CMercenaryTask*	CreateNew()		{ return new CMercenaryTask; }
    ULONG	Release		()					{ delete this; return 0; }
    bool	Create		(PROCESS_ID idProcess);

    bool	CreateNewTask(const ST_MTASK_DATA* pInfo);
    bool	DeleteTask	(OBJID idTask);

    int		GetTaskAmount()					{ return m_setData.size(); }
    CMercenaryTaskData*	QueryTaskDataByIndex	(int nIdx);
    CMercenaryTaskData*	QueryTaskDataByID		(OBJID idTask);
    CMercenaryTaskData*	QueryTaskDataByOwner	(OBJID idUser);
    CMercenaryTaskData*	QueryTaskDataAccepted	(OBJID idAccepter);

    void	OnTimer(time_t tCurr);

public:
    bool	SendTaskList(IRole* pRole, UCHAR usType, OBJID idLast);
    bool	SendTaskInfo(IRole* pRole, OBJID idTask);

protected:
    //	IMTaskDataSet*	m_setData;
    typedef std::vector<CMercenaryTaskData*>	TASKDATA_SET;
    TASKDATA_SET	m_setData;

    CTimeOut		m_tCheckInterval;

    PROCESS_ID		m_idProcess;
    //	MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_MERCENARYTASK_H__D03D9691_EAAD_4725_AFCD_834ED8150B01__INCLUDED_)
