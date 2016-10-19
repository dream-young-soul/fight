
//**********************************************************
// 代码编辑器
//**********************************************************

//--------------------------------------------------------------------------------------
// File Name: ItemPackage.cpp
// Create by: Huang Yuhang
// Create on: 2004/2/24 15:08
//--------------------------------------------------------------------------------------
#pragma warning(disable:4786)
#include "ItemPackage.h"
//--------------------------------------------------------------------------------------
CItemPackage::CItemPackage()
{
    m_setItem.clear();
}
//--------------------------------------------------------------------------------------
CItemPackage::~CItemPackage()
{
    this->Clear(true);
}
//--------------------------------------------------------------------------------------
unsigned long	CItemPackage::Release()
{
    delete(this);
    return 0;
}
//--------------------------------------------------------------------------------------
IItem*	CItemPackage::QueryItemByID(OBJID idItem)
{
    unsigned long uAmount = m_setItem.size();
    for(unsigned long uAccount = 0; uAccount < uAmount; uAccount ++)
    {
        IItem* pItem = m_setItem[uAccount];
        if (pItem->GetID() == idItem)
        {
            return pItem;
        }
    }
    return NULL;
}
//--------------------------------------------------------------------------------------
IItem*	CItemPackage::QueryItemByIndex(unsigned long uIndex)
{
    unsigned long uAmount = m_setItem.size();
    if (uIndex >= uAmount)
    {
        return NULL;
    }
    return m_setItem[uIndex];
}
//--------------------------------------------------------------------------------------
unsigned long CItemPackage::GetItemAmount()
{
    return m_setItem.size();
}
//--------------------------------------------------------------------------------------
BOOL	CItemPackage::AddItem(IItem* pItem)
{
    //不添加空指针 ...
    if (!pItem)
    {
        return false;
    }
    //禁止重复添加 ...
    //包括指针重复和ID重复 ...
    unsigned long uAmount = m_setItem.size();
    for(unsigned long uAccount = 0; uAccount < uAmount; uAccount ++)
    {
        IItem* pOldItem = m_setItem[uAccount];
        if (pOldItem == pItem)
        {
            return false;
        }
        if (pItem->GetID() == pOldItem->GetID())
        {
            return false;
        }
    }
    m_setItem.push_back(pItem);
    return true;
}
//--------------------------------------------------------------------------------------
BOOL	CItemPackage::DeleteItemByID(OBJID idItem, BOOL bDel)
{
    if (idItem == ID_NONE)
    {
        return false;
    }
    unsigned long uAmount = m_setItem.size();
    for(unsigned long uAccount = 0; uAccount < uAmount; uAccount ++)
    {
        IItem* pItem = m_setItem[uAccount];
        if (pItem->GetID() == idItem)
        {
            m_setItem.erase(m_setItem.begin() + uAccount);
            if (bDel)
            {
                pItem->Release();
            }
            return true;
        }
    }
    return false;
}
//--------------------------------------------------------------------------------------
BOOL	CItemPackage::DeleteItemByIndex(unsigned long uIndex, BOOL bDel)
{
    unsigned long uAmount = m_setItem.size();
    if (uIndex > uAmount)
    {
        return false;
    }
    IItem* pItem = m_setItem[uIndex];
    m_setItem.erase(m_setItem.begin() + uIndex);
    if (bDel)
    {
        pItem->Release();
    }
    return true;
}
//--------------------------------------------------------------------------------------
void	CItemPackage::Clear(BOOL bDel)
{
    unsigned long uAmount = m_setItem.size();
    for(unsigned long uAccount = 0; uAccount < uAmount; uAccount ++)
    {
        IItem* pItem = m_setItem[uAccount];
        if (bDel)
        {
            pItem->Release();
        }
    }
    m_setItem.clear();
}
//--------------------------------------------------------------------------------------
IItemPackage*	IItemPackage::CreateNew()
{
    CItemPackage* pItemPackage = new CItemPackage;
    return pItemPackage;
}
//--------------------------------------------------------------------------------------
BOOL	CItemPackage::UpdateItem(IItem* pItem)
{
    //不添加空指针 ...
    if (!pItem)
    {
        return false;
    }
    //禁止指针重复添加 ...
    //ID重复替换
    unsigned long uAmount = m_setItem.size();
    for(unsigned long uAccount = 0; uAccount < uAmount; uAccount ++)
    {
        IItem* pOldItem = m_setItem[uAccount];
        if (pOldItem == pItem)
        {
            return false;
        }
        if (pOldItem->GetID() == pItem->GetID())
        {
            pOldItem->Release();
            m_setItem[uAccount] = pItem;
            return true;
        }
    }
    m_setItem.push_back(pItem);
    return true;
}
//--------------------------------------------------------------------------------------
