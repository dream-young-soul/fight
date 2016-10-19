
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgAuction.h: interface for the CMsgAuction class.
// 拍买系统信息相关类    －LW-   2004-11-26
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGAUCTION_H__1F99F42A_5E1E_4147_93DF_221F682C9E26__INCLUDED_)
#define AFX_MSGAUCTION_H__1F99F42A_5E1E_4147_93DF_221F682C9E26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"
#include "NetStringPacker.h"
enum
{
    AUCTION_ADD = 0,//to server//加入拍买物品 参数：idItem
    AUCTION_BID ,//to server//玩家竞标出价 参数：idItem  dwValue
    AUCTION_NEW ,//to client//开始新的拍买 参数：idUser idItem  dwValue
    AUCTION_HINT_USER,//to client 广播玩家竞标叫价 参数：idUser dwValue
    //	AUCTION_HINT, //to client（广播主持叫价dwData为第几次叫，前二次）参数：dwData idItem dwValue
    AUCTION_HAMMER,//to client（广播主持叫价dwData为第几次叫，三次成交，返回idUser,idItem,dwValue）
    AUCTION_ITEM_INFO,//to server idItem返回时通过CMsgItemInfo信息返回
    //	AUCTION_SYS,      //to client 广播现在是系统拍买，不允许玩家再放入拍买物品
};
class CMsgAuction  : public CNetMsg
{
public:
    CMsgAuction();
    virtual ~CMsgAuction();
    BOOL	Create	(/*OBJID idUser*/char* szName , OBJID idItem, DWORD dwValue, int nAction);
    BOOL	Create	(OBJID dwData, int dwValue, int nAction);
public:
    BOOL	Create		(char* pbufMsg, DWORD dwMsgSize);
    void	Process		(void* pInfo);

    //protected:
    typedef struct
    {
        MSGHEAD_DEFINE
        OBJID       idNpc;       //拍买NPC的id
        OBJID		idItem;
        int         nAuction;
        int         dwValue;
        union
        {
            char        szName[1]; //变长
            OBJID       dwData;
        };
    } MSG_Info;
    CNetStringPacker m_StrPacker;
    MSG_Info*	m_pInfo;
};

#endif // !defined(AFX_MSGAUCTION_H__1F99F42A_5E1E_4147_93DF_221F682C9E26__INCLUDED_)
