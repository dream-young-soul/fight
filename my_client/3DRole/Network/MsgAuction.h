
//**********************************************************
// 代码编辑器
//**********************************************************

// MsgAuction.h: interface for the CMsgAuction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGAUCTION_H__5FC288AD_505C_4132_A530_06AFD4FF7C1F__INCLUDED_)
#define AFX_MSGAUCTION_H__5FC288AD_505C_4132_A530_06AFD4FF7C1F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"
#include "Hero.h"

enum
{
    AUCTION_ADD = 0,//to server//加入拍买物品 参数：id(NPC) ,idItem dwValue
    AUCTION_BID ,//to server//玩家竞标出价 参数：id(NPC) idItem dwValue
    AUCTION_NEW ,//to client//开始新的拍买 参数：idUser idItem dwValue
    AUCTION_HINT_USER,//to client 广播玩家竞标叫价 参数：idUser idItem dwValue
    //		AUCTION_HINT, //to client（广播主持叫价dwData为第几次叫，前二次）参数：dwData idItem dwValue
    AUCTION_HAMMER,//to client（广播主持叫价dwData为第几次叫，三次成交，返回idUser,idItem,dwValue）
    AUCTION_ITEM_INFO,//to server/to client 参数：id(NPC),idItem 返回：idItem,idUser,dwValue
    //		AUCTION_SYS, //to client 广播现在是系统拍买，不允许玩家再加入拍买物品
};

class CMsgAuction : public CNetMsg
{
public:
    CMsgAuction();
    virtual ~CMsgAuction();

    BOOL Create (OBJID idUser , OBJID idItem, int dwValue, int nAction);
    BOOL Create (OBJID idUser , OBJID idItem, int nAction);

    BOOL Create (char* pbufMsg, DWORD dwMsgSize);
    void Process (void* pInfo);
protected:
    typedef struct
    {
        USHORT			unMsgSize;
        USHORT			unMsgType;

        OBJID id; //拍买NPC的id
        OBJID idItem;
        int nAuction;
        int dwValue;
        union
        {
            char name[1];//由发送ID改为名字
            OBJID dwData;
        };
    } MSG_Info;

    MSG_Info* m_pInfo;
private:
    CNetStringPacker	m_StrPacker;
};

#endif // !defined(AFX_MSGAUCTION_H__5FC288AD_505C_4132_A530_06AFD4FF7C1F__INCLUDED_)
