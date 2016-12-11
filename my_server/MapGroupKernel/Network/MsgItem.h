
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

enum {	ITEMACT_NONE			= 0,
        ITEMACT_BUY,					// to server, id: idNpc, data: idItemType
        ITEMACT_SELL,	 				// to server, id: idNpc, data: idItem
        ITEMACT_DROP,					// to server, x, y
        ITEMACT_USE,					// to server, data: position
        ITEMACT_EQUIP			= 5,		// to client，通知装备物品
        ITEMACT_UNEQUIP,				// to server, data: position
        ITEMACT_SPLITITEM,				// to server, data: num
        ITEMACT_COMBINEITEM,			// to server, data: id
        ITEMACT_QUERYMONEYSAVED,		// to server/client, id: idNpc, data: nMoney
        ITEMACT_SAVEMONEY		= 10,	// to server, id: idNpc, data: nMoney
        ITEMACT_DRAWMONEY,				// to server, id: idNpc, data: nMoney
        ITEMACT_DROPMONEY,	 			// to server, x, y
        ITEMACT_SPENDMONEY,				// 无意义，暂保留
        ITEMACT_REPAIR,					// to server, return MsgItemInfo
        ITEMACT_REPAIRALL		= 15,	// to server, return ITEMACT_REPAIRAll, or not return if no money
        ITEMACT_IDENT,					// to server, return MsgItemInfo
        ITEMACT_DURABILITY,				// to client, update durability
        ITEMACT_DROPEQUIPMENT,			// to client, delete equipment
        ITEMACT_IMPROVE,				// to server, improve equipment
        ITEMACT_UPLEV			= 20,	// to server, upleve equipment
        ITEMACT_BOOTH_QUERY,			// to server, open booth, data is npc id
        ITEMACT_BOOTH_ADD,				// to server/client(broadcast in table), id is idItem, data is money
        ITEMACT_BOOTH_DEL,				// to server/client(broadcast), id is idItem, data is npc id
        ITEMACT_BOOTH_BUY,				// to server, id is idItem, data is npc id
        ITEMACT_SYNCHRO_AMOUNT	= 25,	// to client(data is amount)
        ITEMACT_FIREWORKS,

        ITEMACT_COMPLETE_TASK,			// to server, 完成佣兵任务, id=任务ID, dwData=提交的物品，如果没有则为ID_NONE
        ITEMACT_EUDEMON_EVOLVE,			// to server, 幻兽进化, id=幻兽物品ID，dwData=进化成的类型，取值范围1~2
        ITEMACT_EUDEMON_REBORN,			// to server, 幻兽复活，id=幻兽物品ID, data=宝石ID
        ITEMACT_EUDEMON_ENHANCE	= 30,	// to server, 幻兽强化，id=幻兽物品ID, data=宝石ID
        ITEMACT_CALL_EUDEMON,			// to server, 召唤幻兽，id=幻兽物品ID
        ITEMACT_KILL_EUDEMON,			// to server, 收回幻兽，id=幻兽物品ID
        // to client, 收回幻兽的光效表现，id=幻兽物品ID, dwData=幻兽ID
        ITEMACT_EUDEMON_EVOLVE2,		// to server, 幻兽二次进化, id=幻兽物品ID，dwData=进化成的类型，取值范围1~8
        ITEMACT_EUDEMON_ATKMODE,		// to server, 设置幻兽攻击模式, id=幻兽物品ID，dwData=攻击模式，取值范围1~3
        ITEMACT_ATTACH_EUDEMON	= 35,	// 幻兽与人合体, id=幻兽物品ID
        ITEMACT_DETACH_EUDEMON,			// 解除幻兽与人合体状态, id=幻兽物品ID
     };

class CMsgItem : public CNetMsg
{
public:
    CMsgItem();
    virtual ~CMsgItem();

    BOOL	Create	(OBJID id, int nAction, int nPosition = ITEMPOSITION_NONE);
    //	BOOL	Create	(DWORD dwData, int nAction);

public:
    BOOL	Create		(char* pMsgBuf, DWORD dwSize);
    void	Process		(void* pInfo);

protected:
    typedef struct
    {
        MSGHEAD_DEFINE

        OBJID		id;
       // union
        //{
            DWORD		dwData;
            //struct
            //{
                USHORT	usPosY;
                USHORT	usPosX;
            //};
       // };
        USHORT		usAction;
		USHORT		flag;
        OBJID		idTarget;	// 目标对象ID，ID_NONE表示自己
    } MSG_Info;

    MSG_Info*	m_pInfo;
};

#endif // !defined(AFX_MSGITEM_H__9AB1BCEA_F6D1_4413_A39D_8AC35C9FF655__INCLUDED_)
