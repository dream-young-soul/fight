
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgTaskList.h: interface for the CMsgTaskList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGTASKLIST_H__1F8760A6_D7F9_4EF4_8FC8_034652BBB696__INCLUDED_)
#define AFX_MSGTASKLIST_H__1F8760A6_D7F9_4EF4_8FC8_034652BBB696__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"


//////////////////////////////////////////////////////////////////////
const int	_MAX_TASK_PER_PAGE	= 10;	// 每页发送10条任务标题

struct ST_TASK_TITLE
{
    OBJID	idTask;			// 任务ID
    UCHAR	ucReqLev;		// 需要佣兵等级
    char	szTitle[32];	// 任务标题
};

enum
{
    TASKLIST_NONE		= 0,
    TASKLIST_UNACCEPTED,	// 未接任务
    TASKLIST_ACCEPTED,		// 已接任务
    //	TASKLIST_COMPLETED,		// 已完成任务
    TASKLIST_MYTASK,		// 我的任务
};

//////////////////////////////////////////////////////////////////////
class CMsgTaskList : public CNetMsg
{
public:
    CMsgTaskList();
    virtual ~CMsgTaskList();

    BOOL	Create(char* pbufMsg, DWORD dwMsgSize);

    BOOL	Create(USHORT usType, USHORT usAmount = 0, ST_TASK_TITLE* setTask = NULL);
    BOOL	Append(OBJID idTask, UCHAR ucReqLev, const char* pszTitle);
    void	Process	(void* pInfo);

protected:
    typedef struct
    {
        MSGHEAD_DEFINE;

        USHORT	usType;				// 类型
        USHORT	usAmount;			// 数量

        ST_TASK_TITLE	setTask[1];
    } MSG_Info;

    MSG_Info*	m_pInfo;
};

#endif // !defined(AFX_MSGTASKLIST_H__1F8760A6_D7F9_4EF4_8FC8_034652BBB696__INCLUDED_)
