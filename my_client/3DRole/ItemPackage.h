
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

//--------------------------------------------------------------------------------------
// File Name: ItemPackage.h
// Create by: Huang Yuhang
// Create on: 2004/2/24 15:00
//--------------------------------------------------------------------------------------
#ifndef _ITEMPACKAGE_H
#define _ITEMPACKAGE_H
//--------------------------------------------------------------------------------------
#include <DEQUE>
#include "IItemPackage.h"
//--------------------------------------------------------------------------------------
class CItemPackage : public IItemPackage
{
public:
    CItemPackage();
    virtual ~CItemPackage();

public:
    unsigned long	Release();
    IItem*	QueryItemByID(OBJID idItem);
    IItem*	QueryItemByIndex(unsigned long uIndex);
    unsigned long GetItemAmount();
    BOOL	AddItem(IItem* pItem);
    BOOL	UpdateItem(IItem* pItem);
    BOOL	DeleteItemByID(OBJID idItem, BOOL bDel);
    BOOL	DeleteItemByIndex(unsigned long uIndex, BOOL bDel);
    void	Clear(BOOL bDel);
private:
    std::deque <IItem*> m_setItem;
};
//--------------------------------------------------------------------------------------
#endif
