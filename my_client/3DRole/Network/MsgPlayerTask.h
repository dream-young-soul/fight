
//**********************************************************
// 代码编辑器
//**********************************************************

//--------------------------------------------------------------------------------------
// File Name: MsgPlayerTask.h
// Create by: Huang Yuhang
// Create on: 2004/10/9 10:36
//--------------------------------------------------------------------------------------
#ifndef _MSGPLAYERTASK_H
#define _MSGPLAYERTASK_H
//--------------------------------------------------------------------------------------

#include "NetMsg.h"
#include "NetStringPacker.h"
#include "Hero.h"


class CMsgPlayerTask : public CNetMsg
{
public:
    CMsgPlayerTask();
    virtual ~CMsgPlayerTask();

public:
    void			WantPk(const char* pszName, const char* pszTitle, const char* pszDesc,
                           DWORD dwMoney, UCHAR ucRankReq, OBJID idItem0,
                           OBJID idItem1, OBJID idItem2, OBJID idItem3,
                           OBJID idItem4);
    void			WantItem(const char* pszName,  const char* pszTitle, const char* pszDesc,
                             UCHAR ucType, DWORD dwDurability,
                             UCHAR ucAddition,
                             UCHAR ucDegree,
                             WORD dwMoney, UCHAR ucRankReq, OBJID idItem0,
                             OBJID idItem1, OBJID idItem2, OBJID idItem3,
                             OBJID idItem4);
public:
    BOOL			Create		(char* pMsgBuf, DWORD dwSize);
    void			Process		(void* pInfo);

private:
    typedef struct
    {
        USHORT	unMsgSize;
        USHORT	unMsgType;

        DWORD	dwMoney;

        OBJID idTask;

        // 如果是物品 需要物品的附加属性
        DWORD dwDurability;	// 耐久
        UCHAR ucAddition;	// 追加 // 幻兽成长
        UCHAR ucDegree;		// 等级 // 幻兽等级

        UCHAR ucType;		// 悬赏类型
        UCHAR ucRankReq; // 需要的佣兵等级

        ST_PLAYERTASK_ITEM infoItem[_MAX_PRIZE]; // 奖品信息 -- dwItemType = ID_NONE表示这项没有

        char	szBuf[1];//to client 下单人名称 单子详细内容 悬赏杀人或物品名称
        //to server 单子title 单子详细内容 悬赏杀人或物品名称
    } MSG_Info;

    MSG_Info*	m_pInfo;
private:
    CNetStringPacker	m_StrPacker;
};

//--------------------------------------------------------------------------------------
#endif
