
//**********************************************************
// 代码编辑器
//**********************************************************

// UserPackage.cpp: implementation of the CUserPackage class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "UserPackage.h"
#include "User.h"
#include "MapGroup.h"


MYHEAP_IMPLEMENTATION(CUserPackage, s_heap)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUserPackage::CUserPackage()
{
    m_pOwner	= NULL;
    for (int i = 0; i < _MAX_PACK_TYPE; i++)
    {
        m_setItem[i] = CItemSet::CreateNew();
        if (!m_setItem[i])
        {
            ::LogSave("error create UserPackage, CItemSet::CreateNew failed.");
        }
    }
}

CUserPackage::~CUserPackage()
{
    for (int i = 0; i < _MAX_PACK_TYPE; i++)
    {
        if (m_setItem[i])
        {
            SAFE_RELEASE (m_setItem[i]);
        }
    }
}

//////////////////////////////////////////////////////////////////////
bool CUserPackage::Create(CUser* pUser, IDatabase* pDb)
{
    CHECKF(pUser);
    CHECKF(pDb);
    ClearAll();
    m_pOwner = pUser;
    SQLBUF		szSQL;
    sprintf(szSQL, "SELECT * FROM %s WHERE owner_id=%u && position>=%u && position<%u", _TBL_ITEM, m_pOwner->GetID(), ITEMPOSITION_PACK_BEGIN, ITEMPOSITION_PACK_END);
    IRecordset*	pRes = pDb->CreateNewRecordset(szSQL);
    if (pRes)
    {
        for(int i = 0; i < pRes->RecordCount(); i++)
        {
            CItemPtr	pItem = CItem::CreateNew();
            if (pItem)
            {
                if (pItem->Create(pRes, pDb))
                {
                    AddItem(pItem);
                }
                else
                {
                    LOGERROR("玩家[%s]无法加载物品[%u]", m_pOwner->GetName(), pItem->GetID());
                    pItem->ReleaseByOwner();
                }
            }
            pRes->MoveNext();
        }
        pRes->Release();
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CUserPackage::AddItem(CItem* pItem, bool bUpdate /* = false */)
{
    CHECKF(pItem);
    CHECKF(m_pOwner);
    int nPosition = ITEMPOSITION_BACKPACK;
    if (pItem->IsGhostGem())
    {
        nPosition = ITEMPOSITION_GHOSTGEM_PACK;
    }
    else if (pItem->IsEudemon())
    {
        nPosition = ITEMPOSITION_EUDEMON_PACK;
    }
    else if (pItem->IsEudemonEgg())
    {
        nPosition = ITEMPOSITION_EUDEMONEGG_PACK;
    }
    if (IsPackFull(nPosition))
    {
        return false;
    }
    int nPackType = GetPackType(nPosition);
    if (nPackType < 0)
    {
        return false;
    }
    if (pItem->GetInt(ITEMDATA_POSITION) != nPosition)
    {
        pItem->SetInt(ITEMDATA_POSITION, nPosition, true);
    }
    if (m_setItem[nPackType]->AddObj(pItem))
    {
        if (bUpdate)
        {
            pItem->SaveInfo();
        }
        return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
bool CUserPackage::DelItem(OBJID idItem)
{
    for (int i = 0; i < _MAX_PACK_TYPE; i++)
    {
        if (m_setItem[i]->DelObj(idItem))
        {
            return true;
        }
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
CItem* CUserPackage::GetItem(OBJID idItem)
{
    for (int i = 0; i < _MAX_PACK_TYPE; i++)
    {
        CItemPtr pItem = m_setItem[i]->GetObj(idItem);
        if (pItem)
        {
            return pItem;
        }
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////
CItem* CUserPackage::GetItemByType(OBJID idType)
{
    for (int i = 0; i < _MAX_PACK_TYPE; i++)
    {
        int nSize = m_setItem[i]->GetAmount();
        for (int j = nSize - 1; j >= 0; j--)
        {
            CItemPtr pItem = m_setItem[i]->GetObjByIndex(j);
            if (pItem && pItem->GetInt(ITEMDATA_TYPE) == idType)
            {
                return pItem;
            }
        }
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////
bool CUserPackage::IsPackFull(int nPosition)
{
    return (!IsPackSpare(1, nPosition));
}

//////////////////////////////////////////////////////////////////////
void CUserPackage::ClearAll()
{
    for (int i = 0; i < _MAX_PACK_TYPE; i++)
    {
        DEBUG_TRY
        int nSize = m_setItem[i]->GetAmount();
        for (int j = nSize - 1; j >= 0; j--)
        {
            CItemPtr pItem = m_setItem[i]->GetObjByIndex(j);
            if (pItem)
            {
                m_setItem[i]->DelObj(pItem->GetID());
            }
            else
            {
                ::LogSave("CGameObjSet::GetObjByIndex return NULL in CUserPackage::ClearAll");
            }
        }
        DEBUG_CATCH("catch error in CUserPackage::ClearAll")
    }
}

//////////////////////////////////////////////////////////////////////
void CUserPackage::SaveAllInfo()
{
    for (int i = 0; i < _MAX_PACK_TYPE; i++)
    {
        int nSize = m_setItem[i]->GetAmount();
        for (int j = nSize - 1; j >= 0; j--)
        {
            CItemPtr pItem = m_setItem[i]->GetObjByIndex(j);
            if (pItem)
            {
                pItem->SaveInfo();
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////
void CUserPackage::SendAllItemInfo()
{
    if (!m_pOwner)
    {
        return;
    }
    for (int i = 0; i < _MAX_PACK_TYPE; i++)
    {
        int nSize = m_setItem[i]->GetAmount();
        for (int j = 0; j < nSize; j++)
        {
            CItemPtr pItem = m_setItem[i]->GetObjByIndex(j);
            if (pItem)
            {
                CMsgItemInfo	msg;
                if (msg.Create(pItem, ITEMINFO_ADDITEM))
                {
                    m_pOwner->SendMsg(&msg);
                }
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////
CItem* CUserPackage::GetTaskItem(LPCTSTR szItemName)
{
    for (int i = 0; i < _MAX_PACK_TYPE; i++)
    {
        int nSize = m_setItem[i]->GetAmount();
        for (int j = nSize - 1; j >= 0; j--)
        {
            CItemPtr pItem = m_setItem[i]->GetObjByIndex(j);
            if (pItem && strcmp(pItem->GetStr(ITEMDATA_NAME), szItemName) == 0)
            {
                return pItem;
            }
        }
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////
CItem* CUserPackage::GetTaskItemByType(OBJID idType, int nAmount)
{
    for (int i = 0; i < _MAX_PACK_TYPE; i++)
    {
        int nSize = m_setItem[i]->GetAmount();
        for (int j = nSize - 1; j >= 0; j--)
        {
            CItemPtr pItem = m_setItem[i]->GetObjByIndex(j);
            if (pItem && pItem->GetInt(ITEMDATA_TYPE) == idType && (!nAmount || pItem->GetInt(ITEMDATA_AMOUNT) >= nAmount))
            {
                return pItem;
            }
        }
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////
CItem* CUserPackage::FindSpaceTransSpell()
{
    for (int i = 0; i < _MAX_PACK_TYPE; i++)
    {
        int nSize = m_setItem[i]->GetAmount();
        for (int j = nSize - 1; j >= 0; j--)
        {
            CItemPtr pItem = m_setItem[i]->GetObjByIndex(j);
            if (pItem && pItem->IsTransSpell() && pItem->GetInt(ITEMDATA_DATA) == 0)
            {
                return pItem;
            }
        }
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////
CItem* CUserPackage::PopItem(OBJID idItem)
{
    if (ID_NONE == idItem)
    {
        return NULL;
    }
    for (int i = 0; i < _MAX_PACK_TYPE; i++)
    {
        CItemPtr pItem = m_setItem[i]->PopObj(idItem);
        if (pItem)
        {
            return pItem;
        }
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////
bool CUserPackage::IsPackFull(CItem* pItem)
{
    CHECKF(pItem);
    return IsPackFull(GetItemPosition(pItem));
}

//////////////////////////////////////////////////////////////////////
bool CUserPackage::IsPackFull(OBJID idItemType, int nWeight, DWORD dwData)
{
    int nItemPosition = ITEMPOSITION_BACKPACK;
    if (idItemType == ID_NONE)
    {
        // 如果不指定itemtype，则dwData表示要检查的背包
        nItemPosition = dwData;
    }
    else
    {
        if (CItem::IsGhostGem(idItemType))
        {
            nItemPosition = ITEMPOSITION_GHOSTGEM_PACK;
        }
        else if (CItem::IsEudemon(idItemType))
        {
            nItemPosition = ITEMPOSITION_EUDEMON_PACK;
        }
        else if (CItem::IsEudemonEgg(idItemType))
        {
            nItemPosition = ITEMPOSITION_EUDEMONEGG_PACK;
        }
    }
    return IsPackFull(nItemPosition);
}

//////////////////////////////////////////////////////////////////////
bool CUserPackage::IsPackSpare(int nNum, OBJID idItemType, DWORD dwData)
{
    int nItemPosition = ITEMPOSITION_BACKPACK;
    if (idItemType == ID_NONE)
    {
        // 如果不指定itemtype，则dwData表示要检查的背包
        nItemPosition = dwData;
    }
    else
    {
        if (CItem::IsGhostGem(idItemType))
        {
            nItemPosition = ITEMPOSITION_GHOSTGEM_PACK;
        }
        else if (CItem::IsEudemon(idItemType))
        {
            nItemPosition = ITEMPOSITION_EUDEMON_PACK;
        }
        else if (CItem::IsEudemonEgg(idItemType))
        {
            nItemPosition = ITEMPOSITION_EUDEMONEGG_PACK;
        }
    }
    return IsPackSpare(nNum, nItemPosition);
}

//////////////////////////////////////////////////////////////////////
bool CUserPackage::IsPackSpare(int nNum, int nPosition)
{
    CHECKF(nPosition >= ITEMPOSITION_PACK_BEGIN && nPosition < ITEMPOSITION_PACK_END);
    int nPackType = GetPackType(nPosition);
    if (nPackType == -1)
    {
        return false;
    }
    if (m_setItem[nPackType]->GetAmount() + nNum > _MAX_PACK_SIZES[nPackType])
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
inline int CUserPackage::GetItemPosition(CItem* pItem)
{
    CHECKF(pItem);
    return GetItemPositionByType(pItem->GetInt(ITEMDATA_TYPE));
}

//////////////////////////////////////////////////////////////////////
inline int CUserPackage::GetItemPositionByType(OBJID idItemType)
{
    CHECKF (idItemType != ID_NONE);
    if (CItem::IsGhostGem(idItemType))
    {
        return ITEMPOSITION_GHOSTGEM_PACK;
    }
    else if (CItem::IsEudemon(idItemType))
    {
        return ITEMPOSITION_EUDEMON_PACK;
    }
    else if (CItem::IsEudemonEgg(idItemType))
    {
        return ITEMPOSITION_EUDEMONEGG_PACK;
    }
    return ITEMPOSITION_BACKPACK;
}

//////////////////////////////////////////////////////////////////////
CItem* CUserPackage::FindCombineItem(OBJID idType)
{
    for (int i = 0; i < _MAX_PACK_TYPE; i++)
    {
        int nSize = m_setItem[i]->GetAmount();
        for (int j = nSize - 1; j >= 0; j--)
        {
            CItemPtr pItem = m_setItem[i]->GetObjByIndex(j);
            if (pItem && (pItem->GetInt(ITEMDATA_TYPE) == idType && pItem->GetInt(ITEMDATA_AMOUNT) < pItem->GetInt(ITEMDATA_AMOUNTLIMIT)))
            {
                return pItem;
            }
        }
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////
int CUserPackage::GetWeight()
{
    int nAllWeight = 0;
    for (int i = 0; i < _MAX_PACK_TYPE; i++)
    {
        int nSize = m_setItem[i]->GetAmount();
        for (int j = nSize - 1; j >= 0; j--)
        {
            CItemPtr pItem = m_setItem[i]->GetObjByIndex(j);
            if (pItem)
            {
                nAllWeight += pItem->GetWeight();
            }
        }
    }
    return nAllWeight;
}

//////////////////////////////////////////////////////////////////////
int CUserPackage::GetAmount()
{
    int nCount = 0;
    for (int i = 0; i < _MAX_PACK_TYPE; i++)
    {
        int nSize = m_setItem[i]->GetAmount();
        for (int j = nSize - 1; j >= 0; j--)
        {
            CItemPtr pItem = m_setItem[i]->GetObjByIndex(j);
            if (pItem)
            {
                ++nCount;
            }
        }
    }
    return nCount;
}

//////////////////////////////////////////////////////////////////////
int CUserPackage::MultiGetItem(OBJID idTypeFirst, OBJID idTypeLast, int nNum, OBJID* pId, int nSize)
{
    CHECKF(pId);
    int nCount = 0;
    for (int i = 0; i < _MAX_PACK_TYPE; i++)
    {
        int nAmount = m_setItem[i]->GetAmount();
        for (int j = nAmount - 1; j >= 0; j--)
        {
            CItemPtr pItem = m_setItem[i]->GetObjByIndex(j);
            if (pItem
                    && pItem->GetInt(ITEMDATA_TYPE) >= idTypeFirst
                    && pItem->GetInt(ITEMDATA_TYPE) <= idTypeLast)
            {
                ASSERT(nCount + 1 < nSize);
                pId[nCount++] = pItem->GetID();
                if (nCount >= nNum)
                {
                    return nCount;
                }
            }
        }
    }
    return nCount;
}

//////////////////////////////////////////////////////////////////////
bool CUserPackage::MultiCheckItem(OBJID idTypeFirst, OBJID idTypeLast, int nNum)
{
    int nCount = 0;
    for (int i = 0; i < _MAX_PACK_TYPE; i++)
    {
        int nAmount = m_setItem[i]->GetAmount();
        for (int j = nAmount - 1; j >= 0; j--)
        {
            CItemPtr pItem = m_setItem[i]->GetObjByIndex(j);
            if (pItem
                    && pItem->GetInt(ITEMDATA_TYPE) >= idTypeFirst
                    && pItem->GetInt(ITEMDATA_TYPE) <= idTypeLast)
            {
                if (++nCount >= nNum)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
int CUserPackage::RandDropItem(int nMapType, int nChance)
{
    CHECKF(m_pOwner);
    const int	DROP_ITEM_PACK_TYPES	= ITEMPOSITION_GHOSTGEM_PACK - ITEMPOSITION_PACK_BEGIN + 1;
    int nDropNum = 0;
    for (int i = 0; i < DROP_ITEM_PACK_TYPES; i++)
    {
        int nSize = m_setItem[i]->GetAmount();
        for (int j = nSize - 1; j >= 0; j--)
        {
            CItemPtr pItem = m_setItem[i]->GetObjByIndex(j);
            if (pItem && ::RandGet(100) < nChance)
            {
                switch (nMapType)
                {
                case _MAP_NONE:
                    break;
                case _MAP_PKFIELD:
                case _MAP_SYN:
                    if (!pItem->IsEatEnable() && !pItem->IsArmor())
                    {
                        continue;
                    }
                    break;
                case _MAP_PRISON:
                    break;
                }
                POINT	pos;
                pos.x	= m_pOwner->GetPosX();
                pos.y	= m_pOwner->GetPosY();
                if (m_pOwner->GetMap()->FindDropItemCell(USERDROPITEM_RANGE, &pos))
                {
                    if (m_pOwner->DropItem(pItem->GetID(), pos.x, pos.y))
                    {
                        ++nDropNum;
                    }
                }
            }
        }
    }
    return nDropNum;
}

//////////////////////////////////////////////////////////////////////
int CUserPackage::RandDropItem(int nDropNum)
{
    CHECKF(m_pOwner);
    const int	DROP_ITEM_PACK_TYPES	= ITEMPOSITION_GHOSTGEM_PACK - ITEMPOSITION_PACK_BEGIN + 1;
    int nRealDropNum = 0;
    std::vector<OBJID>	setItems;
    for (int i = 0; i < DROP_ITEM_PACK_TYPES; i++)
    {
        for (int j = 0; j < m_setItem[i]->GetAmount(); j++)
        {
            CItem* pItem = m_setItem[i]->GetObjByIndex(j);
            if (pItem && !pItem->IsNeverDropWhenDead())		// 有些物品是死亡也永不掉落的
            {
                setItems.push_back(pItem->GetID());
            }
        }
    }
    int nTotalItemCount = setItems.size();
    {
        // 随机选择掉__min(nDropNum, nTotalItemCount)个物品
        for (int i = 0; i < __min(nDropNum, nTotalItemCount); i++)
        {
            // 冗余代码
            IF_NOT (!setItems.empty())
            break;
            int nIdx = ::RandGet(setItems.size());
            OBJID idItem = setItems[nIdx];
            setItems.erase(setItems.begin() + nIdx);
            CItem* pItem = this->GetItem(idItem);
            if (pItem)
            {
                POINT	pos;
                pos.x	= m_pOwner->GetPosX();
                pos.y	= m_pOwner->GetPosY();
                if (m_pOwner->GetMap()->FindDropItemCell(USERDROPITEM_RANGE, &pos))
                {
                    if (m_pOwner->DropItem(pItem->GetID(), pos.x, pos.y))
                    {
                        ++nRealDropNum;
                    }
                }
                else
                {
                    break;
                }
            }
        }	// end of for loop
    }
    return nRealDropNum;
}

//////////////////////////////////////////////////////////////////////
void CUserPackage::SendAllObjInfo(CMapGroup* pMapGroup, PROCESS_ID idProcess)
{
    CHECK(pMapGroup);
    CHECK(m_pOwner);
    for (int i = 0; i < _MAX_PACK_TYPE; i++)
    {
        int nSize = m_setItem[i]->GetAmount();
        for (int j = nSize - 1; j >= 0; j--)
        {
            CItemPtr pItem = m_setItem[i]->GetObjByIndex(j);
            if (pItem)
            {
                ItemInfoStruct info;
                pItem->GetInfo(&info);
                pMapGroup->QueryIntraMsg()->SendObjInfo(idProcess, m_pOwner->GetID(), INFO_ITEM, &info, sizeof(info));
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////
// 给定多个物品，判断是否可以装下
bool CUserPackage::IsPackFull(int nAmount, CItem* setItems[])
{
    int	setPositionCount[_MAX_PACK_TYPE];
    memset(&setPositionCount, 0L, sizeof(setPositionCount));
    for (int i = 0; i < nAmount; i++)
    {
        DEBUG_TRY
        CItem* pItem = setItems[i];
        IF_NOT (pItem)
        continue;
        ++setPositionCount[GetItemPosition(pItem)];
        DEBUG_CATCH("CUserPackage::IsPackFull(nAmount, setItems)")
    }
    for (int i = 0; i < _MAX_PACK_TYPE; i++)
    {
        if (!IsPackFull(setPositionCount[i], 0, i + ITEMPOSITION_PACK_BEGIN))
        {
            return false;
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
CItem* CUserPackage::GetWPGBadge()
{
    for (int i = 0; i < _MAX_PACK_TYPE; i++)
    {
        int nSize = m_setItem[i]->GetAmount();
        for (int j = nSize - 1; j >= 0; j--)
        {
            CItemPtr pItem = m_setItem[i]->GetObjByIndex(j);
            if (pItem && CItem::IsWPGBadge(pItem->GetInt(ITEMDATA_TYPE)))
            {
                return pItem;
            }
        }
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////
bool CUserPackage::CheckWPGBadge(OBJID idItemType)
{
    for (int i = 0; i < _MAX_PACK_TYPE; i++)
    {
        int nSize = m_setItem[i]->GetAmount();
        for (int j = nSize - 1; j >= 0; j--)
        {
            CItemPtr pItem = m_setItem[i]->GetObjByIndex(j);
            if (pItem && CItem::IsWPGBadge(pItem->GetInt(ITEMDATA_TYPE)))
            {
                if (idItemType && pItem->GetInt(ITEMDATA_TYPE) == idItemType && pItem->GetInt(ITEMDATA_AMOUNT) == 1)
                {
                    idItemType	= ID_NONE;
                    continue;
                }
                return false;
            }
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
OBJID CUserPackage::RandGetItem()
{
    CHECKF(m_pOwner);
    int nRealDropNum = 0;
    int nItemCount = this->GetAmount();
    if (nItemCount <= 0)
    {
        return ID_NONE;
    }
    typedef std::vector<int>	PACKTYPE_SET;
    PACKTYPE_SET	setPackType;
    for (int i = 0; i < _MAX_PACK_TYPE; i++)
    {
        if (m_setItem[i]->GetAmount() > 0)
        {
            setPackType.push_back(i);
        }
    }
    int nPackType = setPackType[::RandGet(setPackType.size())];
    int nAmount = m_setItem[nPackType]->GetAmount();
    if (nAmount > 0)
    {
        int nIndex = ::RandGet(nAmount);
        CItemPtr pItem = m_setItem[nPackType]->GetObjByIndex(nIndex);
        if (pItem)
        {
            return pItem->GetID();
        }
    }
    return ID_NONE;
}
