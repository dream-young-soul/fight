
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// ItemPack.cpp: implementation of the CItemPack class.
//
//////////////////////////////////////////////////////////////////////

#include "windows.h"
#include "define.h"
#include "ItemPack.h"

MYHEAP_IMPLEMENTATION(CItemPack, s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItemPack::CItemPack()
{
    m_setItem	= ITEM_SET::CreateNew(true);
    for(int i = 0; i < MAX_EQUIPSIZE; i++)
    {
        m_setEquip.Push(NULL);
    }
}

CItemPack::~CItemPack()
{
    for(int i = 0; i < MAX_EQUIPSIZE; i++)
    {
        m_setEquip[i]->ReleaseByOwner();
    }
}

CItem* CItemPack::GetEquipItem(int nPos)
{
    int idx = nPos - ITEMPOSITION_EQUIPBEGIN;
    return m_setEquip[idx];
}

CItem** CItemPack::GetEquipItemPtr(int nPos)
{
    int idx = nPos - ITEMPOSITION_EQUIPBEGIN;
    return &m_setEquip[idx];
}

void CItemPack::EquipItem(OBJID idItem, int nPos)
{
    CItem* pItem = FindItem(idItem);
    IF_OK(pItem)
    {
        CItem** ppEquip = GetEquipItemPtr(nPos);
        IF_OK(ppEquip && !*ppEquip)
        {
            pItem->SetInt(ITEMDATA_POSITION, nPos);
            *ppEquip = pItem;
            m_setItem->PopObj(idItem);
        }
    }
}

void CItemPack::UnequipItem(OBJID idItem, int nPos)
{
    CItem** ppEquip = GetEquipItemPtr(nPos);
    IF_OK(ppEquip && *ppEquip)
    {
        IF_OK(!IsItemFull())
        {
            (*ppEquip)->SetInt(ITEMDATA_POSITION, ITEMPOSITION_BACKPACK);
            m_setItem->AddObj(*ppEquip);
            *ppEquip	= NULL;
        }
    }
}

void CItemPack::DropItem(OBJID idItem)
{
    CItem* pItem = FindItem(idItem);
    IF_OK(pItem)
    {
        m_setItem->DelObj(idItem);
    }
}

void CItemPack::DropEquipItem(OBJID idItem, int nPos)
{
    CItem** ppItem = GetEquipItemPtr(nPos);
    IF_OK(ppItem && *ppItem)
    {
        (*ppItem)->ReleaseByOwner();
        *ppItem	= NULL;
    }
}

void CItemPack::SetItemAmount(OBJID idItem, int nAmount)
{
    CItem* pItem = FindItem(idItem, idItem);
    IF_OK(pItem)
    {
        pItem->SetInt(ITEMDATA_AMOUNT, nAmount);
    }
}

CItem* CItemPack::FindItem(OBJID idItem, OBJID idEquip)
{
    if (idItem)
    {
        CItem* pItem = m_setItem->GetObj(idItem);
        if (pItem)
        {
            return pItem;
        }
    }
    if (idEquip)
    {
        EQUIP_SET::Iterator iter = m_setEquip.NewEnum();
        while(iter.Next())
        {
            CItem* pItem = *iter;
            if (pItem->GetID() == idEquip)
            {
                return pItem;
            }
        }
    }
    return NULL;
}

CItem* CItemPack::FindItemByType(OBJID idType)
{
    ITEM_SET::Iterator pItem = m_setItem->NewEnum();
    while(pItem.Next())
    {
        if (pItem && pItem->IsMedicine())
        {
            return pItem;
        }
    }
    return NULL;
}

bool CItemPack::AddItem(CItem* pItem)
{
    CHECKF(pItem && pItem->GetInt(ITEMDATA_POSITION) == ITEMPOSITION_BACKPACK && !IsItemFull());
    m_setItem->AddObj(pItem);
    return true;
}

int CItemPack::GetWeaponRange()
{
    CItem* pItem = GetEquipItem(ITEMPOSITION_WEAPONR);
    if (pItem)
    {
        return ::CutTrail(1, pItem->GetInt(ITEMDATA_ATKRANGE));
    }
    return 1;
}

bool CItemPack::AddEquip(CItem* pItem)
{
    int nPos = pItem->GetInt(ITEMDATA_POSITION);
    CHECKF(pItem && nPos >= ITEMPOSITION_EQUIPBEGIN && nPos < ITEMPOSITION_EQUIPEND
           && m_setEquip[nPos - ITEMPOSITION_EQUIPBEGIN] == NULL);
    m_setEquip[nPos - ITEMPOSITION_EQUIPBEGIN]	= pItem;
    return true;
}
