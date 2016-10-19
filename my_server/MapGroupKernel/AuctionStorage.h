
//**********************************************************
// 代码编辑器
//**********************************************************

// AuctionStorage.h: interface for the CAuctionStorage class.
// 拍买系统仓库，只由Auction 调用   －LW-   2004-11-26
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUCTIONSTORAGE_H__FE6ABBEB_EFF7_4E89_9782_E1D93D9E95D8__INCLUDED_)
#define AFX_AUCTIONSTORAGE_H__FE6ABBEB_EFF7_4E89_9782_E1D93D9E95D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "define.h"
#include "I_mydb.h"
#include "T_MyQueue.h"
#include "Package.h"
typedef struct /*AuctionStorageInfo*/
{
    int id;              //拍买物品表的id
    int idAuction;       //拍买Ｎｐｃ的id
    NAMESTR szPlayer;    //拍买物品的属主名字
    int idPlayer;        //拍买物品的属主id
    int idItem;          //拍买物品的id
    DWORD dwValue;       //拍买物品的价格
    int iState;          //保存拍买的状态0表示是系统物品，1表示是玩家物品
    //	int itype;
} AUCTION_STORAGE_INFO, *pAUCTION_STORAGE_INFO;
enum
{
    SYSTEM_ITEM,  //系统放入的物品
    PLAYER_ITEM,  //玩家加入的物品
    COMPLETE_ITEM,//完成
};
const int   AUCTION_PACKAGE_MAX     = 50 ;    //拍卖仓库可放的最大数、
const int   AUCTION_INTERVAL = 30 ;           //每次叫价的间隔
const int   AUCTION_DEGREE_MAX  = 3  ;        //叫价多少次成交
const int   AUCTION_SYSTEM_MAX  = 10 ;        //系统每次放入的拍卖物品数量
const int   AUCTION_REVENUE =  3;             //税收
const int   AUCTION_BROADCAST_BID = 3;        //间隔多长时间广播最高出价信息(以秒算)
class CAuctionStorage
{
public:
    CAuctionStorage();
    virtual ~CAuctionStorage();
    // interface
public:
    virtual bool	Create			(IRecordset* pRes);
    virtual bool	Create			(OBJID idNpc, int iType, IDatabase* pDb);
    bool	LoadInfo		(IRecord* pRes, pAUCTION_STORAGE_INFO pInfo);
public:	// add del
    OBJID	CreateRecord(OBJID idAuction, const char* szPlayer, OBJID idItem, DWORD dwValue, UCHAR nType, IDatabase* pDb);
    bool	DeleteRecord(OBJID id, IDatabase* pDb);

public:
    pAUCTION_STORAGE_INFO m_pInfo;
    CMyQueue<pAUCTION_STORAGE_INFO> m_SetAuctionQueue;//保存玩家加入的拍买物品
    CMyQueue<pAUCTION_STORAGE_INFO> m_SetSysAuction;  //保存系统加入的拍买物品
};

#endif // !defined(AFX_AUCTIONSTORAGE_H__FE6ABBEB_EFF7_4E89_9782_E1D93D9E95D8__INCLUDED_)
