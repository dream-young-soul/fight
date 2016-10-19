
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgItem.h: interface for the CMsgItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGITEM_H__9AB1BCEA_F6D1_4413_A39D_8AC35C9FF655__INCLUDED_)
#define AFX_MSGITEM_H__9AB1BCEA_F6D1_4413_A39D_8AC35C9FF655__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

enum
{
    ITEMACT_NONE			= 0,
    ITEMACT_BUY				= 1,				// to server, id: idNpc, data: idItemType
    ITEMACT_SELL			= 2,				// to server, id: idNpc, data: idItem
    ITEMACT_DROP			= 3,				// to server, HIWORD(data): x, LOWORD(data): y
    ITEMACT_USE				= 4,				// to server, data: position
    ITEMACT_EQUIP			= 5,				// to client，通知装备物品
    ITEMACT_UNEQUIP			= 6,				// to server, data: position
    ITEMACT_SPLITITEM		= 7,				// to server, data: num
    ITEMACT_COMBINEITEM		= 8,				// to server, data: id
    ITEMACT_QUERYMONEYSAVED	= 9,				// to server/client, id: idNpc, data: nMoney
    ITEMACT_SAVEMONEY		= 10,			// to server, id: idNpc, data: nMoney
    ITEMACT_DRAWMONEY		= 11,			// to server, id: idNpc, data: nMoney
    ITEMACT_DROPMONEY		= 12, 			// to server, HIWORD(data): x, LOWORD(data): y
    ITEMACT_SPENDMONEY		= 13,			// 无意义，暂保留
    ITEMACT_REPAIR			= 14,			// to server, return MsgItemInfo
    ITEMACT_REPAIRALL		= 15,			// to server, return ITEMACT_REPAIRAll, or not return if no money
    ITEMACT_IDENT			= 16,			// to server, return MsgItemInfo
    ITEMACT_DURABILITY		= 17,			// to client, update durability
    ITEMACT_DROPEQUIPMENT	= 18,			// to client, delete equipment
    ITEMACT_IMPROVE			= 19,			// to server, improve equipment
    ITEMACT_UPLEV			= 20,			// to server, upleve equipment
    ITEMACT_BOOTH_QUERY		= 21,			// to server, open booth, data is npc id
    ITEMACT_BOOTH_ADD		= 22,			// to server/client, id is idItem, data is money
    ITEMACT_BOOTH_DEL		= 23,			// to server/client, id is idItem, data is npc id
    ITEMACT_BOOTH_BUY		= 24,			// to server, id is idItem, data is npc id
    ITEMACT_SYNCHRO_AMOUNT	= 25,			// to client(data is amount)
    ITEMACT_FIREWORKS		= 26,

    ITEMACT_COMPLETE_TASK   = 27,			// to server, 完成佣兵任务, id=任务ID, dwData=提交的物品，如果没有则为ID_NONE
    ITEMACT_EUDEMON_EVOLVE	= 28,			// to server, 幻兽进化, id=幻兽物品ID，其它参数无意义
    ITEMACT_EUDEMON_REBORN	= 29,			// to server, 幻兽复活，id=幻兽ID, data=宝石ID
    ITEMACT_EUDEMON_ENHANCE	= 30,			// to server, 幻兽强化，id=幻兽ID, data=宝石ID
    ITEMACT_CALL_EUDEMON	= 31,			// to server, 召唤幻兽，id=幻兽物品ID
    ITEMACT_KILL_EUDEMON	= 32,			// to server, 收回幻兽，id=幻兽物品ID
    ITEMACT_EUDEMON_EVOLVE2 = 33,			// to server, 幻兽二次进化, id=幻兽物品ID，dwData=进化成的类型，取值范围1~8
    ITEMACT_EUDEMON_ATKMODE	= 34,			// to server, 设置幻兽攻击模式, id=幻兽物品ID，dwData=攻击模式，取值范围1~3
    ITEMACT_ATTACH_EUDEMON  = 35,			// to server, 幻兽与人合体, id=幻兽物品ID
    ITEMACT_DETACH_EUDEMON  = 36,			// to server, 解除幻兽与人合体状态, id=幻兽物品ID
};

enum EATK_MODE
{
    EATK_MODE_NONE = 0, // 无类型
    EATK_MODE_ATK, // 攻击
    EATK_MODE_ATKDEF, // 攻防
    EATK_MODE_DEF, // 防御

    EATK_MODE_LIMIT,
};

class CMsgItem : public CNetMsg
{
public:
    CMsgItem();
    virtual ~CMsgItem();

    BOOL	CreateForUseItem (OBJID id, DWORD data, int nAction, OBJID idTarget);
    BOOL	Create	(OBJID id, DWORD data, int nAction);
    BOOL	Create	(OBJID id, int nAction);
    BOOL	Create	(OBJID id, USHORT usPosX, USHORT usPosY, int nAction);

public:
    BOOL	Create		(char* pMsgBuf, DWORD dwSize);
    void	Process		(void* pInfo);

protected:
    typedef struct
    {
        unsigned short	unMsgSize;
        unsigned short	unMsgType;

        OBJID		id;
        union
        {
            DWORD		dwData;
            struct
            {
                USHORT	usPosY;
                USHORT	usPosX;
            };
        };
        USHORT		usAction;
        OBJID		idTarget; // 目标对象ID，ID_NONE表示自己
    } MSG_Info;

    MSG_Info*	m_pInfo;
};

#endif // !defined(AFX_MSGITEM_H__9AB1BCEA_F6D1_4413_A39D_8AC35C9FF655__INCLUDED_)
