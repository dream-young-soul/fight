
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgTaskList.h: interface for the CMsgTaskList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGTASKLIST_H__E8D5D347_C147_4213_849A_EA98365E23BF__INCLUDED_)
#define AFX_MSGTASKLIST_H__E8D5D347_C147_4213_849A_EA98365E23BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "NetMsg.h"
#include "Hero.h"

//////////////////////////////////////////////////////////////////////
class CMsgTaskList : public CNetMsg
{
public:
    CMsgTaskList();
    virtual ~CMsgTaskList();

    BOOL Create(char* pbufMsg, DWORD dwMsgSize);

    void Process (void* pInfo);

protected:
    typedef struct
    {
        USHORT	unMsgSize;
        USHORT	unMsgType;

        USHORT usType; // 类型
        USHORT usAmount; // 数量

        ST_TASK_TITLE setTask[1];
    } MSG_Info;

    MSG_Info* m_pInfo;
};

#endif // !defined(AFX_MSGTASKLIST_H__E8D5D347_C147_4213_849A_EA98365E23BF__INCLUDED_)
