
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

//--------------------------------------------------------------------------------------
// File Name: IItemPackage.h
// Create by: Huang Yuhang
// Create on: 2004/2/24 13:55
//--------------------------------------------------------------------------------------
#ifndef _IITEMPACKAGE_H
#define _IITEMPACKAGE_H
//--------------------------------------------------------------------------------------
#include "BaseFunc.h"
//--------------------------------------------------------------------------------------
class IItem
{
public:
    virtual unsigned long Release() = 0;
    virtual OBJID GetID() = 0;
};
class IItemPackage
{
public:
    static	IItemPackage*	CreateNew();
    virtual	unsigned long	Release() = 0;

public:
    virtual IItem*	QueryItemByID(OBJID idItem) = 0;
    virtual IItem*	QueryItemByIndex(unsigned long uIndex) = 0;
    virtual unsigned long GetItemAmount() = 0;
    virtual BOOL	AddItem(IItem* pItem) = 0;
    virtual BOOL	UpdateItem(IItem* pItem) = 0;
    virtual BOOL	DeleteItemByID(OBJID idItem, BOOL bDel = true) = 0;
    virtual BOOL	DeleteItemByIndex(unsigned long uIndex, BOOL bDel = true) = 0;
    virtual void	Clear(BOOL bDel = true) = 0;
};
//--------------------------------------------------------------------------------------
#endif
