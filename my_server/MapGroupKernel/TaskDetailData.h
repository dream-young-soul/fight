
//**********************************************************
// 代码编辑器
//**********************************************************

// TaskDetailData.h: interface for the CTaskDetailData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TASKDETAILDATA_H__87F05AAE_F2F6_422E_83D0_6D911519C5A2__INCLUDED_)
#define AFX_TASKDETAILDATA_H__87F05AAE_F2F6_422E_83D0_6D911519C5A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma warning(disable:4786)
#include "define.h"
#include "GameData.h"
#include "GameObj.h"
#include "MyHeap.h"
#include "MsgPlayerTask.h"
#include <vector>

#define COMPLETENUM_LIMIT 65535
#define PHASE_LIMIT 255
//////////////////////////////////////////////////////////////////////
enum TASKDETAILDATA
{
    TASKDETAILDATA_ID = 0,			// 任务ID
    TASKDETAILDATA_USERID = 1,		//玩家ID
    TASKDETAILDATA_TASKID = 2,		//任务编号
    TASKDETAILDATA_TASKPHASE = 3,	//任务阶段
    TASKDETAILDATA_TASKCOMPLETENUM = 4, //任务完成次数
    TASKDETAILDATA_TASKBEGINTIME  = 5,  //任务开始时间
};
//////////////////////////////////////////////////////////////////////
struct ST_TASKDETAILDATA
{
    OBJID idUser;
    OBJID idTask;
    int	  nTaskPhase;
    int	  nCompleteNum;
};
//////////////////////////////////////////////////////////////////////
char	szTaskDetailTable[];
typedef CGameData<TASKDETAILDATA, szTaskDetailTable, szID>		CGameTaskDetailData;

//////////////////////////////////////////////////////////////////////
class IDatabase;
class CItem;

class CTaskDetailData  : public CGameObj
{
public:
    bool Create(IRecordset* pRes, IDatabase* pDb);
    bool DeleteRecord()	{ return m_pData->DeleteRecord(); }
public:
    OBJID	GetID()		{return m_pData->GetKey();}
    int		GetInt(TASKDETAILDATA idx)	{ return m_pData->GetInt(idx); }
    LPCTSTR	GetStr(TASKDETAILDATA idx)	{ return m_pData->GetStr(idx); }
    void	SetInt(TASKDETAILDATA idx, int nData, bool bUpdate = false)	{ m_pData->SetInt(idx, nData); if (bUpdate) { m_pData->Update(); } }
    void	SetStr(TASKDETAILDATA idx, LPCTSTR szData, int nSize, bool bUpdate = false)	{ m_pData->SetStr(idx, szData, nSize); if (bUpdate) { m_pData->Update(); } }
    bool	Release()				{ delete this; return true; }

public:
    bool CreateRecord(ST_TASKDETAILDATA* pInfo, IDatabase* pDb);
    CTaskDetailData();
    virtual ~CTaskDetailData();
    static CTaskDetailData* CreateNew() {return new CTaskDetailData;}
protected:
    CGameTaskDetailData*	m_pData;
    MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_TASKDETAILDATA_H__87F05AAE_F2F6_422E_83D0_6D911519C5A2__INCLUDED_)
