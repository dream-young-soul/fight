
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgPlayerTask.h: interface for the CMsgPlayerTask class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGPLAYERTASK_H__AAB8131E_45ED_4198_A946_E8716C9CF6B1__INCLUDED_)
#define AFX_MSGPLAYERTASK_H__AAB8131E_45ED_4198_A946_E8716C9CF6B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"
#include "NetStringPacker.h"

enum
{
    _WANT_PK		= 0,	// 悬赏PK
    _WANT_ITEM		= 1,	// 悬赏物品
    _WANT_EUDEMON	= 2,	// 悬赏幻兽
};

const int _MAX_PRIZE = 5;

struct ST_PLAYERTASK_ITEM
{
    union
    {
        OBJID	idItem;				// 物品ID		// to server的时候
        struct  									// to client的时候
        {
            OBJID	idItemType;		// 物品类型
            USHORT	usDurability;	// 耐久
            UCHAR	ucAddition;		// 追加 // 幻兽成长
            UCHAR	ucDegree;		// 等级 // 幻兽等级
        };
    };
};

class CMercenaryTaskData;
class CMsgPlayerTask : public CNetMsg
{
public:
    CMsgPlayerTask();
    virtual ~CMsgPlayerTask();

    BOOL	Create(char* pbufMsg, DWORD dwMsgSize);

    BOOL	Create	(CMercenaryTaskData* pData);
    void	Process	(void* pInfo);

protected:
    typedef struct
    {
        MSGHEAD_DEFINE;

        DWORD dwMoney;			// 奖金

        OBJID idTask;
        // 如果是物品 需要物品的附加属性
        DWORD dwDurability;		// 耐久
        UCHAR ucAddition;		// 追加 // 幻兽成长
        UCHAR ucDegree;			// 等级 // 幻兽等级

        UCHAR ucType;			// 悬赏类型
        UCHAR ucRankReq;		// 需要的佣兵等级

        ST_PLAYERTASK_ITEM	infoItem[_MAX_PRIZE];	// 奖品信息 -- dwItemType = ID_NONE表示这项没有

        char szBuf[1];	//to client 单子title 单子详细内容 悬赏杀人或物品名称
        //to server 单子title 单子详细内容 悬赏杀人或物品名称
    } MSG_Info;

    MSG_Info*	m_pInfo;
    CNetStringPacker	m_StrPacker;
};

#endif // !defined(AFX_MSGPLAYERTASK_H__AAB8131E_45ED_4198_A946_E8716C9CF6B1__INCLUDED_)
