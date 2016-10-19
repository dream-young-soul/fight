
//**********************************************************
// 代码编辑器
//**********************************************************

// UserPackage.h: interface for the CUserPackage class.
// 背包类，用于管理玩家身上背包物品
// zlong 2004-02-05
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERPACKAGE_H__726A0F38_4530_4036_98E0_F00DD64DE3D7__INCLUDED_)
#define AFX_USERPACKAGE_H__726A0F38_4530_4036_98E0_F00DD64DE3D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "define.h"
#include "GameObjSet.h"
#include "Myheap.h"
//#include "User.h"
#include "Item.h"


////////////////////////////////////////////////////////////////
// 背包容量上限
const int	_MAX_EUDEMONSIZE				= 3;				// 幻兽最大数量
const int	_MAX_EUDEMONEGGSIZE				= 6;				// 幻兽蛋最大数量
const int	_MAX_GHOSTGEMSIZE				= 40;				// 魔魂宝石最大数量
const int	_MAX_USERITEMSIZE				= 40;				// 普通物品背包最大数量
const int	_MAX_ALLUSERITEMSIZE			= _MAX_EUDEMONSIZE + _MAX_EUDEMONEGGSIZE + _MAX_GHOSTGEMSIZE + _MAX_USERITEMSIZE + (ITEMPOSITION_EQUIPEND - ITEMPOSITION_EQUIPBEGIN);		// + 装备数量

const int	_MAX_PACK_TYPE	= ITEMPOSITION_PACK_END - ITEMPOSITION_PACK_BEGIN;
/*
enum PACK_TYPE {
	_PACK_NORMAL			= ITEMPOSITION_BACKPACK,			// 普通物品背包
	_PACK_GHOST_GEM			= ITEMPOSITION_GHOSTGEM_PACK,		// 魔魂宝石背包
	_PACK_EUDEMON_EGG		= ITEMPOSITION_EUDEMONEGG_PACK,		// 幻兽蛋背包
	_PACK_EUDEMON			= ITEMPOSITION_EUDEMON_PACK,		// 幻兽背包
};
*/
const int	_MAX_PACK_SIZES[_MAX_PACK_TYPE] =
{
    _MAX_USERITEMSIZE,				// 普通物品背包容量
    _MAX_GHOSTGEMSIZE,				// 魔魂宝石最大数量
    _MAX_EUDEMONEGGSIZE,			// 幻兽蛋最大数量
    _MAX_EUDEMONSIZE,				// 幻兽最大数量

};


enum DROP_MAP_TYPE
{
    _MAP_NONE	= 0,
    _MAP_PKFIELD,
    _MAP_SYN,
    _MAP_PRISON,
};

typedef	IGameObjSet<CItem>	IItemSet;
typedef	CGameObjSet<CItem>	CItemSet;

class CUser;
class CMapGroup;
//////////////////////////////////////////////////////////////////////
class IUserPackage
{
public:
    static IUserPackage* CreateNew();
    virtual void	Release()													PURE_VIRTUAL_FUNCTION

public:
    virtual bool	Create		(CUser* pUser, IDatabase* pDb)					PURE_VIRTUAL_FUNCTION_0
    virtual bool	AddItem		(CItem* pItem, bool bUpdate = false)			PURE_VIRTUAL_FUNCTION_0
    virtual bool	DelItem		(OBJID idItem)									PURE_VIRTUAL_FUNCTION_0
    virtual CItem*	PopItem		(OBJID idItem)									PURE_VIRTUAL_FUNCTION_0
    virtual int		MultiGetItem(OBJID idTypeFirst, OBJID idTypeLast, int nNum, OBJID* pId, int nSize)		PURE_VIRTUAL_FUNCTION_0
    virtual bool	MultiCheckItem(OBJID idTypeFirst, OBJID idTypeLast, int nNum)							PURE_VIRTUAL_FUNCTION_0

    virtual int		RandDropItem(int nMapType, int nChance)						PURE_VIRTUAL_FUNCTION_0
    virtual int		RandDropItem(int nDropNum)									PURE_VIRTUAL_FUNCTION_0
    virtual OBJID	RandGetItem()												PURE_VIRTUAL_FUNCTION_0

    virtual CItem*	GetItem			(OBJID idItem)								PURE_VIRTUAL_FUNCTION_0
    virtual CItem*	GetItemByType	(OBJID idType)								PURE_VIRTUAL_FUNCTION_0
    virtual CItem*	GetTaskItem		(LPCTSTR szItemName)						PURE_VIRTUAL_FUNCTION_0
    virtual CItem*	GetTaskItemByType	(OBJID idType, int nAmount)				PURE_VIRTUAL_FUNCTION_0

    virtual CItem*	GetWPGBadge		()											PURE_VIRTUAL_FUNCTION_0
    virtual bool	CheckWPGBadge	(OBJID idItemType)							PURE_VIRTUAL_FUNCTION_0

    virtual void	ClearAll		()											PURE_VIRTUAL_FUNCTION
    virtual void	SaveAllInfo		()											PURE_VIRTUAL_FUNCTION
    virtual void	SendAllItemInfo	()											PURE_VIRTUAL_FUNCTION
    virtual void	SendAllObjInfo	(CMapGroup* pMapGroup, PROCESS_ID idProcess)		PURE_VIRTUAL_FUNCTION

public:
    virtual CItem*	FindSpaceTransSpell	()										PURE_VIRTUAL_FUNCTION_0
    virtual CItem*	FindCombineItem		(OBJID idType)							PURE_VIRTUAL_FUNCTION_0
    virtual bool	IsPackFull			(CItem* pItem)							PURE_VIRTUAL_FUNCTION_0
    virtual bool	IsPackFull			(int nPosition)							PURE_VIRTUAL_FUNCTION_0
    virtual int		GetWeight			()										PURE_VIRTUAL_FUNCTION_0
    virtual int		GetAmount			()										PURE_VIRTUAL_FUNCTION_0
    virtual bool	IsPackFull			(int nAmount, CItem* setItems[])		PURE_VIRTUAL_FUNCTION_0

    virtual bool	IsPackFull			(OBJID idItemType, int nWeight, DWORD dwData)		PURE_VIRTUAL_FUNCTION_0
    virtual bool	IsPackSpare			(int nNum, OBJID idItemType, DWORD dwData)			PURE_VIRTUAL_FUNCTION_0
    virtual int		GetItemPosition		(CItem* pItem)							PURE_VIRTUAL_FUNCTION_0
    virtual int		GetItemPositionByType(OBJID idItemType)						PURE_VIRTUAL_FUNCTION_0
};


//////////////////////////////////////////////////////////////////////
class CUserPackage  :	public IUserPackage
{
public:
    CUserPackage();
    virtual ~CUserPackage();
public:
    static IUserPackage* CreateNew()		{ return new CUserPackage; }
    void	Release()						{ delete this; }

public: // IUserPackage
    bool	Create		(CUser* pUser, IDatabase* pDb);
    bool	AddItem		(CItem* pItem, bool bUpdate = false);
    bool	DelItem		(OBJID idItem);
    CItem*	PopItem		(OBJID idItem);
    int		MultiGetItem(OBJID idTypeFirst, OBJID idTypeLast, int nNum, OBJID* pId, int nSize);
    bool	MultiCheckItem(OBJID idTypeFirst, OBJID idTypeLast, int nNum);

    int		RandDropItem(int nMapType, int nChance);
    int		RandDropItem(int nDropNum);
    OBJID	RandGetItem();

    CItem*	GetItem				(OBJID idItem);
    CItem*	GetItemByType		(OBJID idType);
    CItem*	GetTaskItem			(LPCTSTR szItemName);
    CItem*	GetTaskItemByType	(OBJID idType, int nAmount);

    CItem*	GetWPGBadge		();
    bool	CheckWPGBadge	(OBJID idItemType);

    void	ClearAll		();
    void	SaveAllInfo		();
    void	SendAllItemInfo	();
    void	SendAllObjInfo	(CMapGroup* pMapGroup, PROCESS_ID idProcess);	// 临时这么处理CMapGroup

public:
    CItem*	FindSpaceTransSpell	();
    CItem*	FindCombineItem		(OBJID idType);
    bool	IsPackFull			(CItem* pItem);
    bool	IsPackFull			(int nPosition);
    int		GetWeight			();
    int		GetAmount			();

    bool	IsPackFull			(int nAmount, CItem* setItems[]);

    bool	IsPackFull			(OBJID idItemType, int nWeight, DWORD dwData);
    bool	IsPackSpare			(int nNum, OBJID idItemType, DWORD dwData);
    int		GetItemPosition	(CItem* pItem);
    int		GetItemPositionByType(OBJID idItemType);
protected:
    bool	IsPackSpare			(int nNum, int nPosition);
    int		GetPackType		(int nPosition)
    {
        if (nPosition < ITEMPOSITION_PACK_BEGIN || nPosition >= ITEMPOSITION_PACK_END)
        {
            return -1;
        }
        return nPosition - ITEMPOSITION_PACK_BEGIN;
    }

protected:
    IItemSet*	m_setItem[_MAX_PACK_TYPE];
    CUser*		m_pOwner;

    MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_USERPACKAGE_H__726A0F38_4530_4036_98E0_F00DD64DE3D7__INCLUDED_)
