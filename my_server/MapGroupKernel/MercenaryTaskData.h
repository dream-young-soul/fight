
//**********************************************************
// 代码编辑器
//**********************************************************

// MercenaryTaskData.h: interface for the CMercenaryTaskData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MERCENARYTASKDATA_H__10522F05_A91D_4979_9240_BE2EDE9DFEBB__INCLUDED_)
#define AFX_MERCENARYTASKDATA_H__10522F05_A91D_4979_9240_BE2EDE9DFEBB__INCLUDED_

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

// 佣兵任务数据

enum MTASKDATA
{
    MTASKDATA_ID_	= 0,				// 任务ID
    MTASKDATA_TYPE,						// 任务类型
    MTASKDATA_USER_ID,					// 发布任务人ID
    MTASKDATA_USER_NAME,				// 发布任务人名字
    MTASKDATA_TITLE,					// 任务标题
    MTASKDATA_DETAIL,					// 任务详细说明
    MTASKDATA_TARGET_NAME,				// 目标名字：目标物品名字、幻兽名字(其实也是物品)、目标人物名字
    MTASKDATA_DURA,						// 需要耐久度
    MTASKDATA_ADDITION,					// 需要追加 / 幻兽成长
    MTASKDATA_DEGREE,					// 需要等级 / 幻兽等级
    MTASKDATA_PRIZE_MONEY,				// 奖金
    MTASKDATA_PRIZE_ITEMID0,			// 奖品0 ~ 4
    MTASKDATA_PRIZE_ITEMID1,
    MTASKDATA_PRIZE_ITEMID2,
    MTASKDATA_PRIZE_ITEMID3,
    MTASKDATA_PRIZE_ITEMID4,
    MTASKDATA_RANK_REQ,					// 任务需要的佣兵等级
    MTASKDATA_ACCEPT_USERID,			// 接任务人ID
    MTASKDATA_ACCEPT_TIME,				// 接任务时间
    MTASKDATA_RESULT_ITEMID,			// 任务完成物品
    MTASKDATA_STATE,					// 任务状态
};

enum MTASKSTATE
{
    MTASKSTATE_NONE	= 0,				//
    MTASKSTATE_ACCEPTED,				// 任务已经被接
    MTASKSTATE_FINISHED,				// 任务已完成，但还没领取奖品——主要是用于杀人的任务
    MTASKSTATE_COMPLETED,				// 任务已经完全结束
    MTASKSTATE_CANCELED,				// 任务已经被取消
    MTASKSTATE_CLOSED,					// 任务已经关闭——当任务被完成并且发布任务人也取走所要的物品后
};

struct ST_MTASK_DATA
{
    OBJID		idTask;					// 任务ID
    OBJID		idUser;					// 发布任务人ID
    NAMESTR		szUserName;				// 发布任务人名字
    char		szTitle[32];			// 任务标题
    char		szDetail[128];			// 任务详细内容
    NAMESTR		szTargetName;			// 任务目标名字(可能是人物、物品、幻兽名字)

    UCHAR		ucType;					// 任务类型

    UCHAR		ucAddition;				// 需要追加 / 幻兽成长
    UCHAR		ucDegree;				// 需要等级 / 幻兽等级
    UCHAR		ucRankReq;				// 需要佣兵等级
    DWORD		dwDura;					// 需要耐久度

    OBJID		idAcceptUser;			// 接任务人ID
    DWORD		dwAcceptTime;			// 接任务时间
    OBJID		idResultItem;			// 任务完成物品ID
    UCHAR		ucTaskState;			// 任务状态

    DWORD		dwPrizeMoney;				// 奖金
    OBJID		idPrizeItems[_MAX_PRIZE];	// 奖品
};
//////////////////////////////////////////////////////////////////////
char	szMercenaryTaskTable[];
typedef CGameData<MTASKDATA, szMercenaryTaskTable, szID>		CGameMTaskData;

//////////////////////////////////////////////////////////////////////
class IDatabase;
class CItem;
class CMercenaryTaskData  : public CGameObj
{
public:
    CMercenaryTaskData();
    virtual ~CMercenaryTaskData();

    static CMercenaryTaskData* CreateNew()		{ return new CMercenaryTaskData; }
    ULONG	Release()				{ delete this; return 0; }
    ULONG	ReleaseByOwner()		{ delete this; return 0; }

public:
    bool	Create		(OBJID idTask, IDatabase* pDb);
    bool	Create		(IRecordset* pRes, IDatabase* pDb);

    bool	CreateRecord(const ST_MTASK_DATA* pInfo, IDatabase* pDb);
    bool	DeleteRecord();

public:
    //	bool	IsFinished	()				{ return (GetInt(MTASKDATA_RESULT_ITEMID) != ID_NONE); }
    //	bool	IsAccepted	()				{ return (GetInt(MTASKDATA_ACCEPT_USERID) != ID_NONE); }

public:	// get&set
    OBJID	GetID()			{ return m_pData->GetKey(); }

    int		GetInt(MTASKDATA idx)		{ return m_pData->GetInt(idx); }
    LPCTSTR	GetStr(MTASKDATA idx)		{ return m_pData->GetStr(idx); }
    void	SetInt(MTASKDATA idx, int nData, bool bUpdate = false)					{ m_pData->SetInt(idx, nData); if (bUpdate) { m_pData->Update(); } }
    void	SetStr(MTASKDATA idx, LPCTSTR szData, int nSize, bool bUpdate = false)	{ m_pData->SetStr(idx, szData, nSize); if (bUpdate) { m_pData->Update(); } }

    CItem*	GetItem(OBJID idItem);
    CItem*	PopItem(OBJID idItem);
    CItem*	GetItemByIndex(int nIndex);

    bool	AddResultItem(CItem* pItem, bool bUpdate = true);
    CItem*	GetResultItem()		{ return m_pResultItem; }
    CItem*	PopResultItem()		{ CItem* pItem = m_pResultItem; m_pResultItem = NULL; return pItem; }

protected:
    bool	LoadItems(IDatabase* pDb);

protected:
    CGameMTaskData*	m_pData;

    CItem*		m_pResultItem;
    typedef std::vector<CItem*>		ITEM_SET;
    ITEM_SET	m_setPrizeItem;

    MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_MERCENARYTASKDATA_H__10522F05_A91D_4979_9240_BE2EDE9DFEBB__INCLUDED_)
