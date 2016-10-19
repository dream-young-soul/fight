
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// ItemPack.h: interface for the CItemPack class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMPACK_H__EC4056F3_172C_453E_BE68_4D3279F05C9D__INCLUDED_)
#define AFX_ITEMPACK_H__EC4056F3_172C_453E_BE68_4D3279F05C9D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AutoPtr.h"
#include "Array.h"
#include "GameObjSet.h"
#include "myheap.h"

#include "Item.h"

//////////////////////////////////////////////////////////////////////
const int	MAX_EQUIPSIZE				= 8;
const int	MAX_USERITEMSIZE			= 40;
const int	MAX_ALLUSERITEMSIZE			= MAX_USERITEMSIZE + MAX_EQUIPSIZE;

class CItemPack
{
    COM_STYLE(CItemPack)
protected:
    CItemPack();
    virtual ~CItemPack();
    typedef	Array<CItem*>	EQUIP_SET;
    typedef	CGameObjSet<CItem>	ITEM_SET;

public: // const
    bool		IsItemFull()			{ return m_setItem->GetAmount() >= MAX_USERITEMSIZE; }
    int			GetWeaponRange();
    const ITEM_SET*	QueryItemSet()		{ return m_setItem; }

public: // application
    bool AddEquip(CItem* pItem);
    CItem* FindItemByType(OBJID idType);
    bool		AddItem(CItem* pItem);
    CItem*		FindItem(OBJID idItem, OBJID idEquip = ID_NONE);
    void		SetItemAmount(OBJID idItem, int nAmount);
    void		DropEquipItem(OBJID idItem, int nPos);
    void		DropItem(OBJID idItem);
    void		UnequipItem(OBJID idItem, int nPos);
    void		EquipItem(OBJID idItem, int nPos);
    CItem**		GetEquipItemPtr(int nPos);
    CItem*		GetEquipItem(int nPos);

protected:
    EQUIP_SET			m_setEquip;
    CAutoPtr<ITEM_SET>	m_setItem;

protected: // ctrl
    MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_ITEMPACK_H__EC4056F3_172C_453E_BE68_4D3279F05C9D__INCLUDED_)
