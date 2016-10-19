
//**********************************************************
// 代码编辑器
//**********************************************************

// Auction.h: interface for the CAuction class.
// 拍买系统    －LW-   2004-11-26
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUCTION_H__983FD325_E569_4AC7_BB51_2C11F613DDBF__INCLUDED_)
#define AFX_AUCTION_H__983FD325_E569_4AC7_BB51_2C11F613DDBF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "AuctionStorage.h"
#include "AuctionSystemItemData.h"
#include "TimeOut.h"

//#include "T_MyQueue.h"
class	CUser;
class   IDatabase;
class   IAuction
{
public:
    virtual bool    JoinAuction(OBJID idUser,  OBJID idItem, DWORD dwValue, IDatabase* pDb) PURE_VIRTUAL_FUNCTION_0 //参数type为了以后可以拍买任意东东而使用
    virtual bool	ShoutPrice(OBJID idItem, OBJID idUser, int nMoney)                      PURE_VIRTUAL_FUNCTION_0 //叫价
    virtual bool    SendAuctionItemInfo(OBJID idUser, OBJID idItem)                         PURE_VIRTUAL_FUNCTION_0
    virtual void	OnTimer(DWORD nCurr)                                                   PURE_VIRTUAL_FUNCTION
    virtual bool    Create(PROCESS_ID idProcess, OBJID idNpc)                              PURE_VIRTUAL_FUNCTION_0
    virtual void	Release()								                               PURE_VIRTUAL_FUNCTION
    virtual CPackage* QueryPackage()                                                       PURE_VIRTUAL_FUNCTION_0
    virtual bool    SysAuctionStart(char* szInfo)                                          PURE_VIRTUAL_FUNCTION_0
    virtual bool    CheckAuctionState(int nType)                                           PURE_VIRTUAL_FUNCTION_0
};
class   CPackage;
class   CAuctionStorage;
class CAuction: public IAuction //: public CAuctionStorage
{
public:
    CAuction(PROCESS_ID idProcess);
    virtual ~CAuction();
public:
    static IAuction*	CreateNew(PROCESS_ID idProcess)		{ return new CAuction(idProcess); }
public:
    //将物品放到拍买处等待拍买
    virtual bool    JoinAuction(OBJID idUser,  OBJID idItem, DWORD dwValue, IDatabase* pDb);
    virtual bool	ShoutPrice(OBJID idItem, OBJID idUser, int nMoney); //叫价
    virtual bool    SendAuctionItemInfo(OBJID idUser, OBJID idItem);
    virtual void	OnTimer(DWORD nCurr);
    virtual bool    Create(PROCESS_ID idProcess, OBJID idNpc);
    virtual void	Release()	{ delete this; }
    virtual bool    SysAuctionStart(char* szInfo);
    virtual bool    CheckAuctionState(int nType);
protected:
    enum	{ 	auction_none = 0, auction_bating , auction_bated, }	m_nState, m_nSysState; //记录拍买的当前状态
    CPackage* QueryPackage() { ASSERT(m_pPackage); return m_pPackage; }

    int		m_nCountUp;                           //当前系统叫价次数（每三十秒一次）
    bool    m_isSysBid;                           //记录是否为系统拍买
    OBJID   m_idAuction;                          //拍买Ｎｐｃ的ＩＤ
    UCHAR   m_nCue;                               //判断是否开始系统拍卖前提示
    int     m_tDay;                               //记录当天时间
    char    m_szInfo[1024];                       //记录提示内容
private:
    void BroadcastListMsg(CNetMsg* pMsg, CUser* pExcludeUser = NULL);  //广播消息
    void BroadcastSysMsg(LPCTSTR szMsg, CUser* pSendSelf = NULL);
protected: // ctrl
    CAuctionSystemItemData* m_SystemItem;
    CAuctionStorage* m_pAuctionInfo;              //用于和数据库的作用
    CPackage*  m_pPackage;                        //存贮拍买物品的仓库
    pAUCTION_STORAGE_INFO m_pProprietor;	      // 用于保存当前最高价
    AUCTION_STORAGE_INFO m_Current;               // 用于保存当前拍买物品的原始信息
    ///////////////////////////////////////////
private:
    time_t	m_tHammer;                            //每三十钞锤一下。
    time_t  m_tBroadcast;                         //每三秒统计一次当前最高出价，并广播
    CTimeOut	m_tClock;                         //用于控制OnTime()每秒循环一次
protected:
    PROCESS_ID	m_idProcess;
    MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_AUCTION_H__983FD325_E569_4AC7_BB51_2C11F613DDBF__INCLUDED_)
