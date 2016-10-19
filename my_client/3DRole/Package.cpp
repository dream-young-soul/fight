
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// Package.cpp: implementation of the CPackage class.
//
//////////////////////////////////////////////////////////////////////

#include "Package.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPackage::CPackage()
{
    m_setItem.clear();
}

CPackage::~CPackage()
{
    this->Reset();
}
//----------------------------------------------------------------------

void CPackage::Reset()
{
    int nAmount = m_setItem.size();
    for(int i = 0; i < nAmount; i ++)
    {
        CItem* pItem = m_setItem[i];
        SAFE_DELETE(pItem);
    }
    m_setItem.clear();
}

//----------------------------------------------------------------------
int CPackage::GetItemAmount()
{
    return m_setItem.size();
}

//----------------------------------------------------------------------
CItem* CPackage::GetItemByIndex(int nIndex)
{
    int nAmount = m_setItem.size();
    if ((nIndex < 0) || (nIndex >= nAmount))
    {
        return NULL;
    }
    return m_setItem[nIndex];
}

//----------------------------------------------------------------------
CItem* CPackage::GetItem(OBJID id)
{
    int nAmount = m_setItem.size();
    for (int i = nAmount - 1; i >= 0; i--)
    {
        CItem* pItem = m_setItem[i];
        if (pItem && pItem->GetID() == id)
        {
            return pItem;
        }
    }
    return NULL;
}

//----------------------------------------------------------------------

void CPackage::AddItem(CItem* pItem)
{
    if (pItem)
    {
        m_setItem.push_back(pItem);
    }
}

//----------------------------------------------------------------------
void CPackage::DelItem(OBJID idItem)
{
    int nAmount = m_setItem.size();
    for(int i = 0; i < nAmount; i ++)
    {
        CItem* pItem = m_setItem[i];
        if (pItem && (pItem->GetID() == idItem))
        {
            SAFE_DELETE(pItem);
            m_setItem.erase(m_setItem.begin() + i);
            return;
        }
    }
}

//----------------------------------------------------------------------

void CPackage::SetId(OBJID idPackage)
{
    m_idPackage = idPackage;
}

//----------------------------------------------------------------------

OBJID CPackage::GetId()
{
    return m_idPackage;
}

//----------------------------------------------------------------------

int CPackage::GetType()
{
    return m_nType;
}

//----------------------------------------------------------------------

void CPackage::SetType(int nType)
{
    m_nType = nType;
}

//----------------------------------------------------------------------
