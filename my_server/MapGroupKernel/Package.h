
//**********************************************************
// 代码编辑器
//**********************************************************

// Package.h: interface for the CPackage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PACKAGE_H__DBC395BA_5C4A_40B5_944A_329D547BFBE5__INCLUDED_)
#define AFX_PACKAGE_H__DBC395BA_5C4A_40B5_944A_329D547BFBE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Item.h"
#include "GameObjSet.h"
#include "Myheap.h"
#include <vector>
//////////////////////////////////////////////////////////////////////
enum
{
    _STORE_NONE				= 0,
    _STORE_NPCITEM			= ITEMPOSITION_STORAGE,
    _STORE_TRUNKITEM		= ITEMPOSITION_TRUNK,
    _STORE_USERPACK_ITEM	= ITEMPOSITION_CHEST,				//	_ITEM_PACKAGE 百宝囊（放物品）
    _STORE_EUDEMONBROODER	= ITEMPOSITION_EUDEMON_BROODER,
    _STORE_EUDEMONITEM		= ITEMPOSITION_EUDEMON_STORAGE,
    _STORE_AUCTION          = ITEMPOSITION_AUCTION_STORAGE,
};
enum
{
    GOODS,
    MONEY,
};
const int	PACKAGE_LIMIT			= 50;			// 一个package最多能放多少个物品，注意：太大时CMsgPacket可能超过1024。
const int   PACKAGE_AUCTION_LIMIT   = 5;          //拍买到的物品仓库的数量限制LW
//////////////////////////////////////////////////////////////////////
typedef	IGameObjSet<CItem>	IItemSet;
typedef	CGameObjSet<CItem>	CItemSet;

class CPackage
{
protected:
    CPackage();
    virtual ~CPackage();
public:
    static CPackage* CreateNew()							{ return new CPackage; }
    ULONG	Release()										{ delete this; return 0; }

public:
    int GetItemType(int index);
    bool	Create(int nType, OBJID idOwner, OBJID idPlayer, IDatabase* pDb);// 可重复调用
    CItem*	AuctionGoodsBackOff(OBJID idPlayer, OBJID idItem, bool bUpdate = true);
    virtual bool	CheckIn(CUser* pUser, CItem* pItem, bool bUpdate = true);// 自动同步客户端
    virtual bool    CheckIn(CItem* pItem) {CHECKF(pItem); if (m_setItem->AddObj(pItem)) { return true; } else { return false; }}
    virtual CItem*	CheckOut(OBJID idItem);
    virtual CItem*	CheckOut(CUser* pUser, OBJID idItem, bool bUpdate);					// 自动同步客户端
    bool	SendInfo(CUser* pUser);									// 自动同步客户端
    bool	IsPlayerPackage()
    {
        return m_nType == ITEMPOSITION_STORAGE || m_nType == ITEMPOSITION_CHEST
               || m_nType == ITEMPOSITION_EUDEMON_BROODER || m_nType == ITEMPOSITION_EUDEMON_STORAGE
               /*LW*/ || m_nType == ITEMPOSITION_AUCTION_STORAGE;
    }
    int		GetAmount()										{ return m_setItem->GetAmount(); }
    int		GetWeight(OBJID idItem);
    OBJID	GetItemTypeByID(OBJID idItem);
    DWORD	GetItemAmountLimitByID(OBJID idItem);
    //LW拍买相关----------------------------------------
    CItem*	GetItemByIndex(int index) {CHECKF(index >= 0 && index < GetAmount()); return(m_setItem->GetObjByIndex(index));}
    CItem*  GetItemByID(OBJID idItem) {CHECKF(idItem > 0); CHECKF(m_nType != _STORE_NONE); return(m_setItem->GetObj(idItem));}
    bool    CAuctionChip(/*OBJID idNpc,*/OBJID idUser, DWORD value, IRecordset* pRecordset, IDatabase* pDb); //创建一个物品ＬＷ
    //LW------------------------------------------------
protected:
    IItemSet*	m_setItem;
    int			m_nType;
    OBJID		m_idOwner;
    OBJID		m_idPlayer;
    UCHAR		m_ucType;
protected:
    MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_PACKAGE_H__DBC395BA_5C4A_40B5_944A_329D547BFBE5__INCLUDED_)
