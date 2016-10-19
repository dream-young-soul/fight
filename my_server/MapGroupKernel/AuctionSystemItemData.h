
//**********************************************************
// 代码编辑器
//**********************************************************

// AuctionSystemItemData.h: interface for the CAuctionSystemItemData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUCTIONSYSTEMITEMDATA_H__43BA14C3_9A92_45BB_A422_C519C0F72BBD__INCLUDED_)
#define AFX_AUCTIONSYSTEMITEMDATA_H__43BA14C3_9A92_45BB_A422_C519C0F72BBD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "define.h"
#include "GameData.h"
#include "GameObj.h"
#include <deque>
#include "MyHeap.h"
//#include "MsgPlayerTask.h"
#include <vector>


enum AUCTIONSYSTEMDATA
{
    AUCTION_SYSTEMITEM_ID = 0,        // use for insert
    AUCTION_SYSTEMITEM_TYPE = 1,      // 物品的TYPE类型
    AUCTION_SYSTEMITEM_NPC,           //拍买NPC的ID
    AUCTION_SYSTEMITEM_AMOUNT,        //ITEMDATA_EXP
    AUCTION_SYSTEMITEM_AMOUNTLIMIT,	  //ITEMDATA_ATTRIB
    AUCTION_SYSTEMITEM_IDENT,

    AUCTION_SYSTEMITEM_GEM1,
    AUCTION_SYSTEMITEM_GEM2,
    AUCTION_SYSTEMITEM_MAGIC1,

    AUCTION_SYSTEMITEM_MAGIC2,
    AUCTION_SYSTEMITEM_MAGIC3,
    AUCTION_SYSTEMITEM_DATA,

    AUCTION_SYSTEMITEM_WARGHOSTEXP,
    AUCTION_SYSTEMITEM_GEMTYPE,
    AUCTION_SYSTEMITEM_AVAILABLETIME,
    AUCTION_SYSTEMITEM_DATE_TYPE,
    AUCTION_SYSTEMITEM_PARAM,        //拍买物品日期时间
    AUCTION_SYSTEMITEM_VALUE,        //拍买物品的底价
};

struct ST_AUCTION_SYSTEM_DATA
{
    int id;
    OBJID user_id;
    char name[16];
    int level;
    int teacherlevel;
    int profession;
    //UCHAR profession;
    char title[32];
    char content[128];
};

char	szAuctionsystemTable[];
typedef CGameData<AUCTIONSYSTEMDATA, szAuctionsystemTable, szID>		CGameAuctionSystemData;
typedef	deque<CGameAuctionSystemData*>	AUCTION_DATA_SET;

class IDatabase;
class CAuctionSystemItemData: public CGameObj
{
public:
    CAuctionSystemItemData();
    virtual ~CAuctionSystemItemData();
    static CAuctionSystemItemData* CreateNew()		{ return new CAuctionSystemItemData; }
    ULONG	Release()				{ delete this; return 0; }
    ULONG	ReleaseByOwner()		{ delete this; return 0; }
public:
    bool	Create		(IDatabase* pDb, OBJID idNpc, int nAmount, int nMaxType);
    bool	DeleteRecord(int id);
    CGameAuctionSystemData* GetItemDataByID(int id);
    CGameAuctionSystemData* GetItemDataByIndex(int nIndex);
    void GetItemDataByDateType(AUCTION_DATA_SET& Data_set, int nType, int nAmount);
    int GetAmount() {return m_SystemData_set.size();}
    friend class CAuction;
protected:
    void CleanDataSet();
    AUCTION_DATA_SET	m_SystemData_set;
    //	CGameAuctionSystemData*	m_pData;
    MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_AUCTIONSYSTEMITEMDATA_H__43BA14C3_9A92_45BB_A422_C519C0F72BBD__INCLUDED_)
