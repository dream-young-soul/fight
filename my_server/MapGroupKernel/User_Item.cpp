
//**********************************************************
// 代码编辑器
//**********************************************************

// User.cpp: implementation of the CUser class.
//
//////////////////////////////////////////////////////////////////////

#include "AllMsg.h"
#include "User.h"
#include "MapGroup.h"
#include "netmsg.h"
#include "ItemType.h"
#include "MapItem.h"
#include "Package.h"

const int	MINI_ADDLIFE_DRAG				= 100;
//const int	ADDLIFE_FIRST_PERCENT			= 40;
//const int	ADDLIFE_NEXT_PERCENT			= 20;
const int	STORAGE_NPC_ID					= 1005;		//? 信仰仓库互通
const int	_MINE_RANGE						= 1;		// 挖矿距离

//////////////////////////////////////////////////////////////////////
// item
//////////////////////////////////////////////////////////////////////
bool CUser::CreateAllItem(IDatabase* pDb)
{
    ASSERT(m_pPackage);
    // 载入背包物品
    if (!m_pPackage->Create(this, pDb))
    {
        return false;
    }
    // 载入装备物品
    SQLBUF		szSQL;
    //sprintf(szSQL, "SELECT * FROM %s WHERE owner_id=%u && position<=%u LIMIT %d", _TBL_ITEM, GetID(), ITEMPOSITION_USER_LIMIT, _MAX_ALLUSERITEMSIZE);
    sprintf(szSQL, "SELECT * FROM %s WHERE owner_id=%u && position >= %u && position<%u LIMIT %d", _TBL_ITEM, GetID(), ITEMPOSITION_EQUIPBEGIN, ITEMPOSITION_EQUIPEND, ITEMPOSITION_EQUIPEND - ITEMPOSITION_EQUIPBEGIN);
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
                    int nPosition = pItem->GetInt(ITEMDATA_POSITION);
                    CItemPtr& refpEquip = GetEquipItemRef(nPosition);
                    if (!refpEquip)				//? 防止两个物品的POSITION相同 paled
                    {
                        refpEquip = pItem;
                        EquipMagicItem(pItem, nPosition);
                    }
                    else
                    {
                        char err[256];
                        sprintf(err, "Found same position of user equip.The Item ID: %d", pItem->GetID());
                        LOGWARNING(err);
                        pItem->ReleaseByOwner();
                    }
                }
                else
                {
                    LOGERROR("玩家[%s]无法加载物品[%u]", GetName(), pItem->GetID());
                    pItem->ReleaseByOwner();
                }
            }
            pRes->MoveNext();
        }
        pRes->Release();
    }
    UpdateWeight();
    return true;
}

//////////////////////////////////////////////////////////////////////
void CUser::SaveItemInfo()
{
    if (m_pHelmet)
    {
        m_pHelmet->SaveInfo();
    }
    if (m_pNecklace)
    {
        m_pNecklace->SaveInfo();
    }
    if (m_pArmor)
    {
        m_pArmor->SaveInfo();
    }
    if (m_pWeaponR)
    {
        m_pWeaponR->SaveInfo();
    }
    if (m_pWeaponL)
    {
        m_pWeaponL->SaveInfo();
    }
    if (m_pRingR)
    {
        m_pRingR->SaveInfo();
    }
    if (m_pRingL)
    {
        m_pRingL->SaveInfo();
    }
    if (m_pShoes)
    {
        m_pShoes->SaveInfo();
    }
    if (m_pMount)
    {
        m_pMount->SaveInfo();
    }
    if (m_pSprite)
    {
        m_pSprite->SaveInfo();
    }
    if (m_pMantle)
    {
        m_pMantle->SaveInfo();
    }
	if(m_pFashion)
	{
		m_pFashion->SaveInfo();
	}
    m_pPackage->SaveAllInfo();
}

//////////////////////////////////////////////////////////////////////
void CUser::DeleteAllItem()
{
    SAFE_RELEASEBYOWNER(m_pHelmet);
    SAFE_RELEASEBYOWNER(m_pNecklace);
    SAFE_RELEASEBYOWNER(m_pArmor);
    SAFE_RELEASEBYOWNER(m_pWeaponR);
    SAFE_RELEASEBYOWNER(m_pWeaponL);
    SAFE_RELEASEBYOWNER(m_pRingR);
    SAFE_RELEASEBYOWNER(m_pRingL);
    SAFE_RELEASEBYOWNER(m_pShoes);
    SAFE_RELEASEBYOWNER(m_pMount);
    SAFE_RELEASEBYOWNER(m_pSprite);
    SAFE_RELEASEBYOWNER(m_pMantle);
	SAFE_RELEASEBYOWNER(m_pFashion);
    m_pPackage->ClearAll();
    UpdateWeight();
}

//////////////////////////////////////////////////////////////////////
void CUser::SendAllItemInfo()
{
    if (m_pHelmet)
    {
        CMsgItemInfo	msg;
        if (msg.Create(m_pHelmet, ITEMINFO_ADDITEM))
        {
            this->SendMsg(&msg);
        }
    }
    if (m_pNecklace)
    {
        CMsgItemInfo	msg;
        if (msg.Create(m_pNecklace, ITEMINFO_ADDITEM))
        {
            this->SendMsg(&msg);
        }
    }
    if (m_pArmor)
    {
        CMsgItemInfo	msg;
        if (msg.Create(m_pArmor, ITEMINFO_ADDITEM))
        {
            this->SendMsg(&msg);
        }
    }
    if (m_pWeaponR)
    {
        CMsgItemInfo	msg;
        if (msg.Create(m_pWeaponR, ITEMINFO_ADDITEM))
        {
            this->SendMsg(&msg);
        }
    }
    if (m_pWeaponL)
    {
        // auto correct (enable since arrow)
        //		if (m_pWeaponR && m_pWeaponR->GetItemSort() == ITEMSORT_WEAPON2)
        {
            //			ASSERT(!"auto unquiponly when error weapon, may be not backpack room.");
            //			this->UnEquipOnly(ITEMPOSITION_WEAPONL);
        }
        //		else
        {
            CMsgItemInfo	msg;
            if (msg.Create(m_pWeaponL, ITEMINFO_ADDITEM))
            {
                this->SendMsg(&msg);
            }
        }
    }
    if (m_pRingR)
    {
        CMsgItemInfo	msg;
        if (msg.Create(m_pRingR, ITEMINFO_ADDITEM))
        {
            this->SendMsg(&msg);
        }
    }
    if (m_pRingL)
    {
        CMsgItemInfo	msg;
        if (msg.Create(m_pRingL, ITEMINFO_ADDITEM))
        {
            this->SendMsg(&msg);
        }
    }
    if (m_pShoes)
    {
        CMsgItemInfo	msg;
        if (msg.Create(m_pShoes, ITEMINFO_ADDITEM))
        {
            this->SendMsg(&msg);
        }
    }
    if (m_pMount)
    {
        CMsgItemInfo	msg;
        if (msg.Create(m_pMount, ITEMINFO_ADDITEM))
        {
            this->SendMsg(&msg);
        }
    }
    if (m_pSprite)
    {
        CMsgItemInfo	msg;
        if (msg.Create(m_pSprite, ITEMINFO_ADDITEM))
        {
            this->SendMsg(&msg);
        }
    }
    if (m_pMantle)
    {
        CMsgItemInfo	msg;
        if (msg.Create(m_pMantle, ITEMINFO_ADDITEM))
        {
            this->SendMsg(&msg);
        }
    }
	if(m_pFashion)
	{
		CMsgItemInfo msg;
		if(msg.Create(m_pFashion,ITEMINFO_ADDITEM))
		{

			this->SendMsg(&msg);
		}
	}
    m_pPackage->SendAllItemInfo();
}

//////////////////////////////////////////////////////////////////////
void CUser::SendAllEquipInfoTo(CUser* pRecv)
{
    CHECK(pRecv);
    if (m_pHelmet)
    {
        CMsgItemInfo	msg;
        if (msg.Create(m_pHelmet, ITEMINFO_OTHERPLAYER_EQUIPMENT, GetID()))
        {
            pRecv->SendMsg(&msg);
        }
    }
    if (m_pNecklace)
    {
        CMsgItemInfo	msg;
        if (msg.Create(m_pNecklace, ITEMINFO_OTHERPLAYER_EQUIPMENT, GetID()))
        {
            pRecv->SendMsg(&msg);
        }
    }
    if (m_pArmor)
    {
        CMsgItemInfo	msg;
        if (msg.Create(m_pArmor, ITEMINFO_OTHERPLAYER_EQUIPMENT, GetID()))
        {
            pRecv->SendMsg(&msg);
        }
    }
    if (m_pWeaponR)
    {
        CMsgItemInfo	msg;
        if (msg.Create(m_pWeaponR, ITEMINFO_OTHERPLAYER_EQUIPMENT, GetID()))
        {
            pRecv->SendMsg(&msg);
        }
    }
    if (m_pWeaponL)
    {
        CMsgItemInfo	msg;
        if (msg.Create(m_pWeaponL, ITEMINFO_OTHERPLAYER_EQUIPMENT, GetID()))
        {
            pRecv->SendMsg(&msg);
        }
    }
    if (m_pRingR)
    {
        CMsgItemInfo	msg;
        if (msg.Create(m_pRingR, ITEMINFO_OTHERPLAYER_EQUIPMENT, GetID()))
        {
            pRecv->SendMsg(&msg);
        }
    }
    if (m_pRingL)
    {
        CMsgItemInfo	msg;
        if (msg.Create(m_pRingL, ITEMINFO_OTHERPLAYER_EQUIPMENT, GetID()))
        {
            pRecv->SendMsg(&msg);
        }
    }
    if (m_pShoes)
    {
        CMsgItemInfo	msg;
        if (msg.Create(m_pShoes, ITEMINFO_OTHERPLAYER_EQUIPMENT, GetID()))
        {
            pRecv->SendMsg(&msg);
        }
    }
    if (m_pMount)
    {
        CMsgItemInfo	msg;
        if (msg.Create(m_pMount, ITEMINFO_OTHERPLAYER_EQUIPMENT, GetID()))
        {
            pRecv->SendMsg(&msg);
        }
    }
    if (m_pSprite)
    {
        CMsgItemInfo	msg;
        if (msg.Create(m_pSprite, ITEMINFO_OTHERPLAYER_EQUIPMENT, GetID()))
        {
            pRecv->SendMsg(&msg);
        }
    }
    if (m_pMantle)
    {
        CMsgItemInfo	msg;
        if (msg.Create(m_pMantle, ITEMINFO_OTHERPLAYER_EQUIPMENT, GetID()))
        {
            pRecv->SendMsg(&msg);
        }
    }

	if(m_pFashion)
	{

		CMsgItemInfo msg;
		if(msg.Create(m_pFashion,ITEMINFO_OTHERPLAYER_EQUIPMENT, GetID()))
		{
			 pRecv->SendMsg(&msg);

		}
	}
}

//////////////////////////////////////////////////////////////////////
CItem* CUser::GetItem(OBJID idItem)
{
    return m_pPackage->GetItem(idItem);
}

//////////////////////////////////////////////////////////////////////
CItem* CUser::GetItemByType(OBJID idType)
{
    return m_pPackage->GetItemByType(idType);
}

//////////////////////////////////////////////////////////////////////
CItem* CUser::GetTaskItem(LPCTSTR szItemName)
{
    return m_pPackage->GetTaskItem(szItemName);
}

//////////////////////////////////////////////////////////////////////
CItem* CUser::GetTaskItemByType(OBJID idType, int nAmount/*=0*/)
{
    return m_pPackage->GetTaskItemByType(idType, nAmount);
}

/////////////////////////////////////////////////////////////////////////////
CItem* CUser::FindSpaceTransSpell()
{
    return m_pPackage->FindSpaceTransSpell();
}

//////////////////////////////////////////////////////////////////////
CItem* CUser::GetEquipItem(OBJID idItem)
{
    if (m_pHelmet && m_pHelmet->GetID() == idItem)
    {
        return m_pHelmet;
    }
    else if (m_pNecklace && m_pNecklace->GetID() == idItem)
    {
        return m_pNecklace;
    }
    else if (m_pArmor && m_pArmor->GetID() == idItem)
    {
        return m_pArmor;
    }
    else if (m_pWeaponR && m_pWeaponR->GetID() == idItem)
    {
        return m_pWeaponR;
    }
    else if (m_pWeaponL && m_pWeaponL->GetID() == idItem)
    {
        return m_pWeaponL;
    }
    else if (m_pRingR && m_pRingR->GetID() == idItem)
    {
        return m_pRingR;
    }
    else if (m_pRingL && m_pRingL->GetID() == idItem)
    {
        return m_pRingL;
    }
    else if (m_pShoes && m_pShoes->GetID() == idItem)
    {
        return m_pShoes;
    }
    else if (m_pMount && m_pMount->GetID() == idItem)
    {
        return m_pMount;
    }
    else if (m_pSprite && m_pSprite->GetID() == idItem)
    {
        return m_pSprite;
    }
    else if (m_pMantle && m_pMantle->GetID() == idItem)
    {
        return m_pMantle;
    }
	else if(m_pFashion && m_pFashion->GetID() == idItem)
	{
		return m_pFashion;
	}
    return NULL;
}

//////////////////////////////////////////////////////////////////////
CItem* CUser::GetEquipItemByPos(int nPosition)
{
    CItemPtr* ppEquip = this->GetEquipItemPtr(nPosition);
    if (!ppEquip)
    {
        return NULL;
    }
    return *ppEquip;
}

//////////////////////////////////////////////////////////////////////
CItemPtr* CUser::GetEquipItemPtr(int nPosition)
{
    switch(nPosition)
    {
    case	ITEMPOSITION_HELMET:
        return &m_pHelmet;
    case	ITEMPOSITION_NECKLACE:
        return &m_pNecklace;
    case	ITEMPOSITION_ARMOR:
        return &m_pArmor;
    case	ITEMPOSITION_WEAPONR:
        return &m_pWeaponR;
    case	ITEMPOSITION_WEAPONL:
        return &m_pWeaponL;
    case	ITEMPOSITION_RINGR:
        return &m_pRingR;
    case	ITEMPOSITION_RINGL:
        return &m_pRingL;
    case	ITEMPOSITION_SHOES:
        return &m_pShoes;
    case	ITEMPOSITION_MOUNT:
        return &m_pMount;
    case	ITEMPOSITION_SPRITE:
        return &m_pSprite;
    case	ITEMPOSITION_MANTLE:
        return &m_pMantle;
	case ITEMPOSITION_FASHION:
		{
			return &m_pFashion;
		}
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////
bool CUser::DelItem	(OBJID idItem, bool bSynchro)
{
    if (m_pPackage->DelItem(idItem))
    {
        if (bSynchro)
        {
            CMsgItem msg;
            if (msg.Create(idItem, ITEMACT_DROP))
            {
                this->SendMsg(&msg);
            }
        }
        return true;
    }
    else
    {
        return false;
    }
}

//////////////////////////////////////////////////////////////////////
bool CUser::AddItem (CItem* pItem, bool bSynchro, bool bUpdate /*= true*/)
{
    CHECKF(pItem);
    if (pItem->GetInt(ITEMDATA_OWNERID) != GetID())
    {
        pItem->SetInt(ITEMDATA_OWNERID, GetID());
    }
    if (pItem->GetInt(ITEMDATA_PLAYERID) != GetID())
    {
        pItem->SetInt(ITEMDATA_PLAYERID, GetID());
    }
    if (!m_pPackage->AddItem(pItem, bUpdate))
    {
        return false;
    }
    if (bSynchro)
    {
        CMsgItemInfo msg;
        if (msg.Create(pItem, ITEMINFO_ADDITEM))
        {
            this->SendMsg(&msg);
        }
    }
    UpdateWeight();
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::DelItemPt (OBJID idItem)
{
    if (m_pPackage->PopItem(idItem))
    {
        UpdateWeight();
        return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
CItem* CUser::PopItem (OBJID idItem, bool bSynchro, bool bUpdate)			// 立即清除OWNER
{
    CItemPtr pItem = m_pPackage->PopItem(idItem);
    if (pItem)
    {
        pItem->SetInt(ITEMDATA_PLAYERID, ID_NONE, UPDATE_FALSE);
        pItem->SetInt(ITEMDATA_OWNERID, ID_NONE, bUpdate);			// 立即清除OWNER
        if (bSynchro)
        {
            CMsgItem msg;
            if (msg.Create(idItem, ITEMACT_DROP))
            {
                this->SendMsg(&msg);
            }
        }
        UpdateWeight();
        return pItem;
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////
CItem* CUser::AwardItem(int nItemType, bool bSynchro, bool bIdentOK/*=false*/, bool bAutoCombine/*=true*/)		// 同时操作数据库
{
    ItemInfoStruct info;
    ASSERT(ItemType()->GetInfo(nItemType, &info));
    info.idOwner	= GetID();
    info.idPlayer	= GetID();
    info.nPosition	= this->QueryPackage()->GetItemPositionByType(nItemType);
    if (bIdentOK)
    {
        info.nIdent	&= ~_ITEM_STATUS_NOT_IDENT;
    }
    return AwardItem(&info, bSynchro, bAutoCombine);
}

//////////////////////////////////////////////////////////////////////
CItem* CUser::AwardItem(ItemInfoStruct* pInfo, bool bSynchro, bool bAutoCombine/*=true*/)		// 同时操作数据库
{
    CHECKF (pInfo);
    // check weight
    int nItemType = pInfo->idType;
    if (GetWeight() + CItem::GetWeight(nItemType) > GetWeightLimit())
    {
        this->SendSysMsg(STR_HEAVEY_BAG);
        return NULL;
    }
    if (bAutoCombine && CItem::IsCountable(nItemType))
    {
        //CItemTypeData* pType = ItemType()->QueryItemType(pInfo->idType);
        //CHECKF(pType);
        if (!CItem::IsNeedIdent(pInfo->nIdent) && pInfo->nAmount < pInfo->nAmountLimit)
        {
            CItem* pItem = FindCombineItem(nItemType);
            if (pItem)
            {
                // check size
                if (pItem->GetInt(ITEMDATA_AMOUNT) + pInfo->nAmount > pInfo->nAmountLimit
                        && m_pPackage->IsPackFull(pItem))
                {
                    if (pItem->IsGhostGem())
                    {
                        this->SendSysMsg(STR_GEMBAG_FULL);
                    }
                    else
                    {
                        this->SendSysMsg(STR_ITEMBAG_FULL);
                    }
                    return NULL;
                }
                return CombineNewItem(pItem, pInfo, bSynchro);
            }
        }
    }
    if (m_pPackage->IsPackFull(m_pPackage->GetItemPositionByType(nItemType)))
    {
        if (CItem::IsGhostGem(nItemType))
        {
            this->SendSysMsg(STR_GEMBAG_FULL);
        }
        else
        {
            this->SendSysMsg(STR_ITEMBAG_FULL);
        }
        return NULL;
    }
    CItemPtr pItem = CItem::CreateNew();
    CHECKF(pItem);
    bool INSERT_TRUE = true;
    if (!pItem->Create(GameDataDefault()->GetGameItemData(), pInfo, GameDataDefault()->GetEudemonData(), INSERT_TRUE, pInfo->id))
    {
        pItem->ReleaseByOwner();
        ASSERT(!"Create");
        this->SendSysMsg(STR_FAILED_GENERATE_ITEM);
        return NULL;
    }
    if (!AddItem(pItem, bSynchro))
    {
        //如果添加物品失败！
        ::LogSave("AddItem failed in CUser::AwardItem.");
        pItem->DeleteRecord();
        pItem->ReleaseByOwner();
        return NULL;
    }
    return pItem;
}

//////////////////////////////////////////////////////////////////////
bool CUser::EraseItem(OBJID idItem, bool bSynchro)		// 同时操作数据库
{
    CItemPtr pItem = m_pPackage->PopItem(idItem);
    if (pItem)
    {
        pItem->DeleteRecord();
        pItem->ReleaseByOwner();
        if (bSynchro)
        {
            CMsgItem msg;
            if (msg.Create(idItem, ITEMACT_DROP))
            {
                this->SendMsg(&msg);
            }
        }
        UpdateWeight();
        return true;
    }
    else
    {
        return false;
    }
}

//////////////////////////////////////////////////////////////////////
bool CUser::EraseEquip(int nPosition, bool bSynchro)		// 同时操作数据库
{
    CItemPtr& refpEquip = GetEquipItemRef(nPosition);
    CHECKF(refpEquip);
    OBJID idItem = refpEquip->GetID();
    refpEquip->DeleteRecord();
    refpEquip->ReleaseByOwner();
    refpEquip = NULL;
    if (bSynchro)
    {
        CMsgItem msg;
        if (msg.Create(idItem, ITEMACT_DROPEQUIPMENT, nPosition))
        {
            this->SendMsg(&msg);
        }
        if (nPosition == ITEMPOSITION_HELMET
                || nPosition == ITEMPOSITION_ARMOR
                || nPosition == ITEMPOSITION_WEAPONR
                || nPosition == ITEMPOSITION_WEAPONL
                || nPosition == ITEMPOSITION_MANTLE )
        {
            CMsgPlayer msgPlayer;
            if (msgPlayer.Create(this->QueryRole()))
            {
                this->BroadcastRoomMsg(&msgPlayer, EXCLUDE_SELF);
            }
        }
    }
    //	UnequipMagicItem(nPosition);				// 在下次 SetAtkTarget()时删除
    UpdateWeight();
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::EquipItem(CItem* pItem, int nPosition, bool bSynchro)
{
    CHECKF(pItem);
    if (QueryMagic())
    {
        QueryMagic()->AbortMagic();
    }
    OBJID idItem = pItem->GetID();
    if (!this->TryItem(idItem, nPosition))
    {
        return false;
    }
    // 双击，自动安排
    if (nPosition >= ITEMPOSITION_PACK_BEGIN && nPosition < ITEMPOSITION_PACK_END)
    {
        if (this->IsWing())
        {
            return false;
        }
        if (pItem->IsHelmet())
        {
            nPosition	= ITEMPOSITION_HELMET;
        }
        else if (pItem->IsNecklace())
        {
            nPosition	= ITEMPOSITION_NECKLACE;
        }
        else if (pItem->IsArmor())
        {
            nPosition	= ITEMPOSITION_ARMOR;
        }
        else if (pItem->IsHoldEnable())
        {
            if (pItem->IsArrowSort())
            {
                nPosition	= ITEMPOSITION_WEAPONL;
            }
            else if (pItem->IsWeapon2())
            {
                nPosition	= ITEMPOSITION_WEAPONR;
            }
            else if (pItem->IsWeapon1() && (!m_pWeaponR || m_pWeaponL))
            {
                nPosition	= ITEMPOSITION_WEAPONR;
            }
            else // if (pItem->IsWeapon1() && !m_pWeaponL || pItem->IsShield())
            {
                nPosition	= ITEMPOSITION_WEAPONL;
            }
        }
        else if (pItem->IsRing())
        {
            nPosition	= ITEMPOSITION_RINGR;
        }
        else if (pItem->IsBangle())
        {
            nPosition	= ITEMPOSITION_RINGL;
        }
        else if (pItem->IsShoes())
        {
            nPosition	= ITEMPOSITION_SHOES;
        }
        else if (pItem->IsMount())
        {
            nPosition	= ITEMPOSITION_MOUNT;
        }
        else if (pItem->IsSprite())
        {
            nPosition	= ITEMPOSITION_SPRITE;
        }
        else if (pItem->IsMantle())
        {
            nPosition	= ITEMPOSITION_MANTLE;
        }
    }
    // 冗错
    //if (m_setItem.size() > _MAX_USERITEMSIZE)
    /*
    	if (m_pPackage->IsPackFull(pItem))
    	{
    		CItem* pEquip = GetEquipItemByPos(nPosition);
    		if (pEquip)
    			return false;
    	}
    */
    // 先取出，以空出背包。
    CHECKF(DelItemPt(idItem));				// VVVVVVVVVVVVVVVVVVVVVVVVVVVV
    bool	bRet	= false;
    switch(nPosition)
    {
    case	ITEMPOSITION_HELMET:
        {
            if (pItem->IsHelmet())
            {
                UnEquipOnly(nPosition);
                m_pHelmet = pItem;
                pItem->SetInt(ITEMDATA_POSITION, ITEMPOSITION_HELMET);
                bRet	= true;
            }
        }
        break;
    case	ITEMPOSITION_NECKLACE:
        {
            if (pItem->IsNecklace())
            {
                UnEquipOnly(nPosition);
                m_pNecklace = pItem;
                pItem->SetInt(ITEMDATA_POSITION, ITEMPOSITION_NECKLACE);
                bRet	= true;
            }
        }
        break;
    case	ITEMPOSITION_ARMOR:
        {
            if (pItem->IsArmor())
            {
                UnEquipOnly(nPosition);
                m_pArmor = pItem;
                pItem->SetInt(ITEMDATA_POSITION, ITEMPOSITION_ARMOR);
                bRet	= true;
            }
        }
        break;
    case	ITEMPOSITION_WEAPONR:
        {
            if (pItem->IsHoldEnable())
            {
                if (pItem->IsWeapon2())
                {
                    if (!( m_pWeaponR && m_pWeaponL && !IsBackPackSpare(2, 0, ID_NONE, ITEMPOSITION_BACKPACK) ))
                    {
                        UnEquipOnly(ITEMPOSITION_WEAPONR);
                        if (m_pWeaponL && !(m_pWeaponL->IsArrow() && pItem->IsBow() || m_pWeaponL->IsDart() && pItem->IsCrossBow()))
                        {
                            UnEquipOnly(ITEMPOSITION_WEAPONL);
                        }
                        m_pWeaponR = pItem;
                        pItem->SetInt(ITEMDATA_POSITION, ITEMPOSITION_WEAPONR);
                        bRet	= true;
                    }
                }
                else if (pItem->IsWeapon1())
                {
                    if (!(m_pWeaponL && (m_pWeaponL->IsArrow() || m_pWeaponL->IsDart()) && !IsBackPackSpare(2, 0, ID_NONE, ITEMPOSITION_BACKPACK) ))
                    {
                        UnEquipOnly(ITEMPOSITION_WEAPONR);
                        if (m_pWeaponL && m_pWeaponL->IsArrowSort())
                        {
                            UnEquipOnly(ITEMPOSITION_WEAPONL);
                        }
                        m_pWeaponR = pItem;
                        pItem->SetInt(ITEMDATA_POSITION, ITEMPOSITION_WEAPONR);
                        bRet	= true;
                    }
                }
            }
        }
        break;
    case	ITEMPOSITION_WEAPONL:
        {
            if (pItem->IsHoldEnable())
            {
                if (m_pWeaponR)
                {
                    if ( m_pWeaponR->IsWeapon1() && (pItem->IsWeapon1() || pItem->IsShield())
                            || m_pWeaponR->IsBow() && pItem->IsArrow() || m_pWeaponR->IsCrossBow() && pItem->IsDart() )
                    {
                        UnEquipOnly(nPosition);
                        m_pWeaponL = pItem;
                        pItem->SetInt(ITEMDATA_POSITION, ITEMPOSITION_WEAPONL);
                        UnequipMagicItem(ITEMPOSITION_WEAPONL);		//paled: need not
                        bRet	= true;
                    }
                }
            }
        }
        break;
    case	ITEMPOSITION_RINGR:
        {
            if (pItem->IsRing())
            {
                UnEquipOnly(nPosition);
                m_pRingR = pItem;
                pItem->SetInt(ITEMDATA_POSITION, ITEMPOSITION_RINGR);
                bRet	= true;
            }
        }
        break;
    case	ITEMPOSITION_RINGL:
        {
            if (pItem->IsBangle())
            {
                UnEquipOnly(nPosition);
                m_pRingL = pItem;
                pItem->SetInt(ITEMDATA_POSITION, ITEMPOSITION_RINGL);
                bRet	= true;
            }
        }
        break;
    case	ITEMPOSITION_SHOES:
        {
            if (pItem->IsShoes())
            {
                UnEquipOnly(nPosition);
                m_pShoes = pItem;
                pItem->SetInt(ITEMDATA_POSITION, ITEMPOSITION_SHOES);
                bRet	= true;
            }
        }
        break;
    case	ITEMPOSITION_MOUNT:
        {
            if (pItem->IsMount())
            {
                UnEquipOnly(nPosition);
                m_pMount = pItem;
                pItem->SetInt(ITEMDATA_POSITION, ITEMPOSITION_MOUNT);
                bRet	= true;
            }
        }
        break;
    case	ITEMPOSITION_SPRITE:	// add by zlong 2003-11-27
        {
            if (pItem->IsSprite())
            {
                UnEquipOnly(nPosition);
                m_pSprite = pItem;
                pItem->SetInt(ITEMDATA_POSITION, ITEMPOSITION_SPRITE);
                bRet	= true;
            }
        }
        break;
    case	ITEMPOSITION_MANTLE:
        {
            if (pItem->IsMantle())
            {
                UnEquipOnly(nPosition);
                m_pMantle = pItem;
                pItem->SetInt(ITEMDATA_POSITION, ITEMPOSITION_MANTLE);
                bRet	= true;
            }
        }
	case ITEMPOSITION_FASHION:
		{
			if (pItem->IsFashion())
			{
				UnEquipOnly(nPosition);
				m_pFashion = pItem;
				pItem->SetInt(ITEMDATA_POSITION, ITEMPOSITION_FASHION);
				bRet	= true;
			}
			break;
		}
        break;
    default:
        ASSERT(!"switch");
    } // switch
    if (!bRet)
    {
        AddItem(pItem, SYNCHRO_FALSE);    // 恢复物品 AAAAAAAAAAAAAAAAAAAAAAA
    }
    if (bRet)
    {
        //---jinggy---2004-11-19--启动设备经验值记数器---begin
        if (pItem->GetWarGhostLevel() == MAX_LEVEL_WARLEVEL - 1)
        {
            UpdateEquipmentExp_Startup(nPosition, EQUIPMENT_EXP_ADD_SECS_AFTER9);
        }
        else if (pItem->GetWarGhostLevel() < MAX_LEVEL_WARLEVEL - 1)
        {
            UpdateEquipmentExp_Startup(nPosition, EQUIPMENT_EXP_ADD_SECS);
        }
        //---jinggy---2004-11-19--启动设备经验值记数器---end
        if (bSynchro)
        {
            CMsgItem msg;
            if (msg.Create(idItem, ITEMACT_EQUIP, nPosition))
            {
                this->SendMsg(&msg);
            }
            if (nPosition == ITEMPOSITION_HELMET
                    || nPosition == ITEMPOSITION_ARMOR
                    || nPosition == ITEMPOSITION_WEAPONR
                    || nPosition == ITEMPOSITION_WEAPONL
                    || nPosition == ITEMPOSITION_MANTLE
                    || nPosition == ITEMPOSITION_MOUNT
					|| nPosition == ITEMPOSITION_FASHION)
            {
                CMsgPlayer msgPlayer;
                if (msgPlayer.Create(this->QueryRole()))
                {
                    this->BroadcastRoomMsg(&msgPlayer, EXCLUDE_SELF);
                }
            }
        }
        //		UpdateWeight();
        CalcFightRate();
        EquipMagicItem(pItem, nPosition);
        return true;
    }
    else
    {
        return false;
    }
}

//////////////////////////////////////////////////////////////////////
bool CUser::TryItem(OBJID idItem, int nPosition)
{
    CItemPtr pItem = GetItem(idItem);
    if (!pItem)
    {
        return false;
    }
    if (pItem->IsTaskItem())
    {
        return false;
    }
    if (pItem->GetInt(ITEMDATA_REQ_LEVEL) && pItem->GetInt(ITEMDATA_REQ_LEVEL) > GetLev())		// 综合等级
    {
        return false;
    }
    if (pItem->IsNeedIdent())
    {
        return false;
    }
    if (pItem->GetInt(ITEMDATA_REQ_SEX) &&
           // (pItem->GetInt(ITEMDATA_REQ_SEX) & (1 << GetSex())) == 0)
			   pItem->GetInt(ITEMDATA_REQ_SEX) != GetSex())
    {
        return false;
    }
    if (pItem->IsMount() && pItem->GetInt(ITEMDATA_AMOUNT) == 0)
    {
        return false;
    }
    // rebirth ------------------------------------------------
    if (this->GetMetempsychosis() && GetLev() >= 70)
    {
        return true;    //!
    }
    if (pItem->GetInt(ITEMDATA_REQ_PROF) != 0)
    {
        int nRequireProfSort	= (pItem->GetInt(ITEMDATA_REQ_PROF) % 1000) / 10;
        int nRequireProfLevel	= pItem->GetInt(ITEMDATA_REQ_PROF) % 10;
        int nProfSort  = this->GetProfessionSort();
        int nProfLevel = this->GetProfessionLevel();
        if (nRequireProfSort == 19)	// 道士通用
        {
            if (nProfSort < 10)	// 不是道士
            {
                return false;
            }
            else
            {
                if (ITEMPOSITION_WEAPONL == nPosition)
                {
                    return false;
                }
            }
        }
        else
        {
            if (nRequireProfSort != nProfSort)
            {
                return false;
            }
        }
        if (nProfLevel < nRequireProfLevel)
        {
            return false;
        }
    }
    //	if (pItem->GetInt(ITEMDATA_REQ_SKILL) && pItem->IsWeapon() &&
    //				pItem->GetInt(ITEMDATA_REQ_SKILL) > GetWeaponSkillLev(pItem->GetInt(ITEMDATA_TYPE)))
    //		return false;
    if ((pItem->GetInt(ITEMDATA_REQ_FORCE) && pItem->GetInt(ITEMDATA_REQ_FORCE) > GetForce())
            || (pItem->GetInt(ITEMDATA_REQ_DEX) && pItem->GetInt(ITEMDATA_REQ_DEX) > GetDex())
            || (pItem->GetInt(ITEMDATA_REQ_HEALTH) && pItem->GetInt(ITEMDATA_REQ_HEALTH) > GetHealth())
            || (pItem->GetInt(ITEMDATA_REQ_SOUL) && pItem->GetInt(ITEMDATA_REQ_SOUL) > GetSoul()))
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::ChkUseItem(CItem* pItem, IRole* pTarget)
{
    CHECKF (pItem);
    CHECKF (pTarget);
    USHORT	usTarget = pItem->GetInt(ITEMDATA_TARGET);
    if (usTarget == TARGET_NONE)
    {
        if (pTarget->GetID() == this->GetID())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    if (usTarget & TARGET_FORBIDDEN)
    {
        return false;
    }
    // PK模式由判断提示由客户端完成
    CUser* pUser = NULL;
    if (pTarget->QueryObj(OBJ_USER, IPP_OF(pUser)))
    {
        // 限制使用对象
        if ((usTarget & TARGET_SELF) && pUser->GetID() != this->GetID()
                || (usTarget & TARGET_OTHERS) && pUser->GetID() == this->GetID())
        {
            return false;
        }
        // 是否对尸体使用的限制
        if (((usTarget & TARGET_BODY) && pUser->IsAlive())
                || (!(usTarget & TARGET_BODY) && !pUser->IsAlive()))
        {
            return false;
        }
    }
    else
    {
        CMonster* pMonster = NULL;
        if (!pTarget->QueryObj(OBJ_MONSTER, IPP_OF(pMonster)))
        {
            return false;
        }
        // 判断目标类型
        if (!((usTarget & TARGET_MONSTER) && pMonster->IsMonster())
                && !((usTarget & TARGET_EUDEMON) && pMonster->IsEudemon()))
        {
            return false;
        }
        // 判断限制条件
        CUser* pOwner = pMonster->QueryOwnerUser();
        if (pOwner)
        {
            if ((usTarget & TARGET_SELF) && pOwner->GetID() != this->GetID()
                    || (usTarget & TARGET_OTHERS) && pOwner->GetID() == this->GetID())
            {
                return false;
            }
        }
        if (((usTarget & TARGET_BODY) && pMonster->IsAlive())
                || (!(usTarget & TARGET_BODY) && !pMonster->IsAlive()))
        {
            return false;
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::UseItem(OBJID idItem, int nPosition, bool bSynchro)
{
    if (!this->TryItem(idItem, nPosition))
    {
        return false;
    }
    CItemPtr pItem = GetItem(idItem);
    if (!pItem)
    {
        return false;
    }
    if (!ChkUseItem(pItem, this->QueryRole()))
    {
        return false;
    }
    if (pItem->IsActionItem() || pItem->IsGhostGem())
    {
        /*
        		if (pItem->IsGhostGem())
        		{
        			if (!(pItem->GetInt(ITEMDATA_TYPE)%100000/1000*1000 == ITEMTYPE_GHOSTGEM_PASSIVE_ATK		// 状态被动类
        				|| pItem->GetInt(ITEMDATA_TYPE)%100000/1000*1000 == ITEMTYPE_GHOSTGEM_RELEASE			// 解除类
        				|| pItem->GetInt(ITEMDATA_TYPE)%100000/1000*1000 == ITEMTYPE_GHOSTGEM_PROTECTIVE))		// 护身类
        			{
        #ifdef _DEBUG
        				::LogSave("Can not use item %s [%u] to self.", pItem->GetStr(ITEMDATA_NAME), idItem);
        #endif
        				return false;
        			}
        		}
        */
        CHECKF(pItem->GetInt(ITEMDATA_ACTION));
        SetTaskItemID(pItem->GetID());
        GameAction()->ProcessAction(pItem->GetInt(ITEMDATA_ACTION), this, NULL, pItem);	//??? mast last code, may be user chgmap to another mapgroup
        return true;
    }
    /*else if (pItem->IsSpell())
    {
    	int nAmount = pItem->GetInt(ITEMDATA_AMOUNT);
    	// check it first
    	if (nAmount <= 0)
    	{
    		IF_NOT (EraseItem(pItem->GetID(), SYNCHRO_TRUE))
    			return false;
    	}

    	// process action
    	OBJID idAction = pItem->GetInt(ITEMDATA_ACTION);

    	pItem->SetInt(ITEMDATA_AMOUNT, --nAmount);
    	if (nAmount <= 0)
    	{
    		ASSERT (EraseItem(pItem->GetID(), SYNCHRO_TRUE));
    		pItem	= NULL;
    	}
    	else
    	{
    		pItem->SaveInfo();

    		CMsgItemInfo msg;
    		IF_OK (msg.Create(pItem, ITEMINFO_ADDITEM))
    			this->SendMsg(&msg);
    	}

    	if (idAction != ID_NONE)
    	{
    		SetTaskItemID(ID_NONE);		// no more spell item operator in action
    		GameAction()->ProcessAction(idAction, this, NULL, NULL);	//??? mast last code, may be user chgmap to another mapgroup
    	}
    	return true;
    }*/
    else if (pItem->IsEquipEnable())
    {
        return EquipItem(pItem, nPosition, bSynchro);
    }
    else if (pItem->IsEatEnable())
    {
        int	nAddLife	= pItem->GetInt(ITEMDATA_LIFE);
        int	nAddMana	= pItem->GetInt(ITEMDATA_MANA);
        //		if (nAddLife >= MINI_ADDLIFE_DRAG && m_setSlowHealUp2Life.Size() > 1)		// 限制吃药速度
        //			return false;
        DEBUG_TRY	// VVVVVVVVVVVVVV
        int nTimes = pItem->GetInt(ITEMDATA_ATKSPEED);	// 补血的次数
        IF_NOT(SpendItem(pItem))		//??? 能吃的一定能删除, 先删除，防作弊
        return false;
        // 2003.2.8 修改加血变成慢慢加。
        if (nAddLife > 0)
        {
            int nFirstAdd	= (nTimes < 2) ? nAddLife : 2 * nAddLife / (nTimes + 1);
            int nNextAdd	= (nTimes >= 2) ? (nAddLife - nFirstAdd) / (nTimes - 1) : 0;
            int nLastAdd	= nAddLife - nFirstAdd;
            if (nTimes > 2)
            {
                nLastAdd -= nNextAdd * (nTimes - 2);
            }
            typedef std::vector<int>	ADDLIFE_LIST;
            ADDLIFE_LIST	setAddLife;
            // first
            if (m_setSlowHealUp2Life.Size() == 0)
            {
                AddAttrib(_USERATTRIB_LIFE, nFirstAdd, SYNCHRO_TRUE);
                this->BroadcastTeamLife();
            }
            else
            {
                setAddLife.push_back(nFirstAdd);
            }
            if (nNextAdd > 0)
            {
                for (int i = 1; i < nTimes - 1; i++)
                {
                    setAddLife.push_back(nNextAdd);
                }
            }
            if (nLastAdd > 0)
            {
                setAddLife.push_back(nLastAdd);
            }
            int nMinNum = __min(setAddLife.size(), m_setSlowHealUp2Life.Size());
            for (int i = 0; i < nMinNum; i++)
            {
                if (setAddLife[i] > m_setSlowHealUp2Life[i])
                {
                    m_setSlowHealUp2Life[i] = setAddLife[i];
                }
            }
            for (int i = nMinNum; i < setAddLife.size(); i++)
            {
                m_setSlowHealUp2Life.Push(setAddLife[i]);
            }
            m_tSlowHealLife.Update();
            // cheat check
            {
                if ((IsCheater(_TYPE_WS) || IsCheater(_TYPE_FY)) && IsCheater(_TYPE_USE_LIFE) && ::RandGet(3) == 0)
                {
                    KickoutCheat(_TYPE_USE_LIFE);
                }
            }
        }
        else if (nAddLife < 0)
        {
            AddAttrib(_USERATTRIB_LIFE, nAddLife, SYNCHRO_TRUE);
            this->BroadcastTeamLife();
        }
        if (nAddMana > 0)
        {
            int nFirstAdd	= (nTimes < 2) ? nAddMana : 2 * nAddMana / (nTimes + 1);
            int nNextAdd	= (nTimes >= 2) ? (nAddMana - nFirstAdd) / (nTimes - 1) : 0;
            int nLastAdd	= nAddMana - nFirstAdd;
            if (nTimes > 2)
            {
                nLastAdd -= nNextAdd * (nTimes - 2);
            }
            typedef std::vector<int>	ADDMANA_LIST;
            ADDMANA_LIST	setAddMana;
            // first
            if (m_setSlowHealUp2Mana.Size() == 0)
            {
                AddAttrib(_USERATTRIB_MANA, nFirstAdd, SYNCHRO_TRUE);
            }
            else
            {
                setAddMana.push_back(nFirstAdd);
            }
            if (nNextAdd > 0)
            {
                for (int i = 1; i < nTimes - 1; i++)
                {
                    setAddMana.push_back(nNextAdd);
                }
            }
            if (nLastAdd > 0)
            {
                setAddMana.push_back(nLastAdd);
            }
            int nMinNum = __min(setAddMana.size(), m_setSlowHealUp2Mana.Size());
            for (int i = 0; i < nMinNum; i++)
            {
                if (setAddMana[i] > m_setSlowHealUp2Mana[i])
                {
                    m_setSlowHealUp2Mana[i] = setAddMana[i];
                }
            }
            for (int i = nMinNum; i < setAddMana.size(); i++)
            {
                m_setSlowHealUp2Mana.Push(setAddMana[i]);
            }
            m_tSlowHealMana.Update();
            // cheat check
            {
                if ((IsCheater(_TYPE_WS) || IsCheater(_TYPE_FY)) && IsCheater(_TYPE_USE_MANA) && ::RandGet(3) == 0)
                {
                    KickoutCheat(_TYPE_USE_MANA);
                }
            }
        }
        else if (nAddMana < 0)
        {
            AddAttrib(_USERATTRIB_MANA, nAddMana, SYNCHRO_TRUE);
        }
        DEBUG_CATCH("UseItem") // AAAAAAAAAAAAAAAAA
        UpdateWeight();
        return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
bool CUser::UseItemTo(OBJID idTarget, OBJID idItem)
{
    if (idTarget == ID_NONE)
    {
        return false;
    }
    if (!this->TryItem(idItem, ITEMPOSITION_GHOSTGEM_PACK))
    {
        return false;
    }
    CItemPtr pItem = GetItem(idItem);
    if (!pItem)
    {
        return false;
    }
    // 判断是否是可以对其他角色使用的魔魂宝石
    /*
    	if (!(pItem->IsGhostGem()		// 魔魂宝石
    			&& ((pItem->GetInt(ITEMDATA_TYPE)%100000/1000*1000 == ITEMTYPE_GHOSTGEM_ACTIVE_ATK) ||	// 状态攻击类
    				(pItem->GetInt(ITEMDATA_TYPE)%100000/1000*1000 == ITEMTYPE_GHOSTGEM_TRACE)))			// 追杀类
    			)
    	{
    #ifdef _DEBUG
    	::LogSave("Can not use item %s [%u] to target.", pItem->GetStr(ITEMDATA_NAME), idItem);
    #endif
    		return false;
    	}
    */
    IRole* pTarget = RoleManager()->QueryRole(idTarget);
    if (!pTarget)
    {
        return false;
    }
    if (!ChkUseItem(pItem, pTarget))
    {
        return false;
    }
    if (pItem->IsActionItem() || pItem->IsGhostGem())
    {
        CHECKF(pItem->GetInt(ITEMDATA_ACTION));
        SetTaskItemID(pItem->GetID());
        GameAction()->ProcessAction(pItem->GetInt(ITEMDATA_ACTION), this, pTarget, pItem);	//??? mast last code, may be user chgmap to another mapgroup
        return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
/*
bool CUser::UserItemToByType(OBJID idTarget, OBJID idType)
{
	if (idTarget == ID_NONE)
		return false;

	// 判断是否是可以对别人使用的魔魂宝石
	if (!(CItem::IsGhostGem(idType)
		&& ((idType%100000/1000*1000 == ITEMTYPE_GHOSTGEM_ACTIVE_ATK)	// 状态攻击类
			|| (idType%100000/1000*1000 == ITEMTYPE_GHOSTGEM_TRACE))			// 追杀类
		))
	{
#ifdef _DEBUG
		::LogSave("Can not use itemtype [%u] to target.", idType);
#endif
		return false;
	}

	IRole* pTarget = RoleManager()->QueryRole(idTarget);
	if (!pTarget)
		return false;

	CItemTypeData* pItemType = ItemType()->QueryItemType(idType);
	if (pItemType && pItemType->GetInt(ITEMTYPEDATA_ACTION))
	{
//		SetTaskItemID(pItem->GetID());
		GameAction()->ProcessAction(pItemType->GetInt(ITEMTYPEDATA_ACTION), this, pTarget, NULL);	//??? mast last code, may be user chgmap to another mapgroup
		return true;
	}

	return false;
}
*/

//////////////////////////////////////////////////////////////////////
bool CUser::UnEquipItem(int nPosition, bool bSynchro)
{
    if (!( nPosition >= ITEMPOSITION_EQUIPBEGIN && nPosition < ITEMPOSITION_EQUIPEND ))
    {
        return false;
    }
    if (QueryMagic())
    {
        QueryMagic()->AbortMagic();
    }
    CItem* pEquip = GetEquipItemByPos(nPosition);
    if (!pEquip)
    {
        return false;
    }
    if (nPosition == ITEMPOSITION_WEAPONR && m_pWeaponL && (m_pWeaponL->IsShield() || m_pWeaponL->IsArrowSort()))
    {
        if (!IsBackPackSpare(2, 0, ID_NONE, ITEMPOSITION_BACKPACK))
        {
            return false;
        }
    }
    else
    {
        if (!IsBackPackSpare(1, 0, ID_NONE, ITEMPOSITION_BACKPACK))
        {
            return false;
        }
    }
    CItemPtr pItem = UnEquipOnly(nPosition);
    CHECKF(pItem);
    //---jinggy---2004-11-19--启动设备经验值记数器---begin
    UpdateEquipmentExp_Stop(nPosition);
    //---jinggy---2004-11-19--启动设备经验值记数器---end
    // 无右手武器
    if (!m_pWeaponR && m_pWeaponL)
    {
        if (m_pWeaponL->IsWeapon1())
        {
            m_pWeaponR = m_pWeaponL;
            m_pWeaponL = NULL;
        }
        else if (m_pWeaponL->IsShield() || m_pWeaponL->IsArrowSort())
        {
            UnEquipOnly(ITEMPOSITION_WEAPONL);
        }
    }
    if (bSynchro)
    {
        CMsgItem msg;
        if (msg.Create(pItem->GetID(), ITEMACT_UNEQUIP, nPosition))
        {
            this->SendMsg(&msg);
        }
        if (nPosition == ITEMPOSITION_HELMET
                || nPosition == ITEMPOSITION_ARMOR
                || nPosition == ITEMPOSITION_WEAPONR
                || nPosition == ITEMPOSITION_WEAPONL
                || nPosition == ITEMPOSITION_MANTLE 
				|| nPosition == ITEMPOSITION_FASHION)
        {
            CMsgPlayer msgPlayer;
            if (msgPlayer.Create(this->QueryRole()))
            {
                this->BroadcastRoomMsg(&msgPlayer, EXCLUDE_SELF);
            }
        }
    }
    if (GetMana() > GetMaxMana())
    {
        SetAttrib(_USERATTRIB_MANA, GetMaxMana(), SYNCHRO_TRUE);
    }
    //	UpdateWeight();
    CalcFightRate();
    return true;
}

//////////////////////////////////////////////////////////////////////
CItem* CUser::UnEquipOnly(int nPosition)
{
    //CHECKF(m_setItem.size() < _MAX_USERITEMSIZE);
    CItem* pItem = NULL;
    CItemPtr& refpEquip = GetEquipItemRef(nPosition);
    if (refpEquip == NULL)
    {
        return NULL;
    }
    CHECKF(!m_pPackage->IsPackFull(refpEquip));
    //	if (nPosition == ITEMPOSITION_WEAPONR)
    //		QueryStatusSet()->DelObj(STATUS_KEEPBOW);
    if (refpEquip)
    {
        pItem = refpEquip;
        //pItem->SetInt(ITEMDATA_POSITION, ITEMPOSITION_BACKPACK);
        //m_setItem.push_back(pItem);
        m_pPackage->AddItem(pItem);
        refpEquip = NULL;
    }
    UnequipMagicItem(nPosition);
    return pItem;
}

//////////////////////////////////////////////////////////////////////
// msgitem
//////////////////////////////////////////////////////////////////////
void CUser::BuyItem (OBJID idNpc, OBJID idType)
{
    CNpc* pNpc;
    if (!GetMap()->QueryObj(GetPosX(), GetPosY(), OBJ_NPC, idNpc, IPP_OF(pNpc)))
    {
        return ;
    }
    if (!pNpc->IsShopNpc())
    {
        return;
    }
    if (!pNpc->QueryShop()->IsOpen())
    {
        return;
    }
    CItemTypeData* pType = pNpc->QueryShop()->QueryItemType(idType);
    IF_NOT (pType)
    return;
    int nPrice = pType->GetInt(ITEMTYPEDATA_PRICE);
    nPrice = pNpc->QueryShop()->Rebate(nPrice, GetSynID(), GetSynRankShow());
    //---jinggy---过是向帮派NPC买东西，帮派成员有优惠---begin
    /*
    	CGameMap * pSynMap = this->GetMap();
        //1%给帮派基金
    	int nSynGainMoney = MulDiv(nPrice,1, 100);
    	CSyndicate * pSyn = SynManager()->QuerySyndicate(pSynMap->GetSynID());
    	if (pSyn)
    	{
    		pSyn->QueryModify()->AddData(SYNDATA_MONEY,nSynGainMoney,true);
    	}

    */
    //---jinggy---过是向帮派NPC买东西，帮派成员有优惠---end
    if (this->GetMoney() < nPrice)
    {
        this->SendSysMsg(STR_NOT_SO_MUCH_MONEY);
        return;
    }
    if (!AwardItem(idType, SYNCHRO_TRUE, CUser::IDENT_OK))
    {
        //		this->SendSysMsg("您身上带不了这么多东西!");
        return;
    }
    ASSERT(this->SpendMoney(nPrice, SYNCHRO_TRUE));
    UpdateWeight();
}

//////////////////////////////////////////////////////////////////////
void CUser::SellItem(OBJID idNpc, OBJID idItem)
{
    CNpc* pNpc;
    if (!GetMap()->QueryObj(GetPosX(), GetPosY(), OBJ_NPC, idNpc, IPP_OF(pNpc)))
    {
        return ;
    }
    if (!pNpc->IsShopNpc())
    {
        return;
    }
    CItemPtr pItem = this->GetItem(idItem);
    if (!pItem)
    {
        this->SendSysMsg(STR_ITEM_NOT_FOUND);
        return;
    }
    if (!pItem->IsSellEnable())
    {
        this->SendSysMsg(STR_NOT_SELL_ENABLE);
        return;
    }
    if (pItem->IsNonsuchItem())
    {
        ::MyLogSave("gmlog/sell_item", "%s(%u) sell item:[id=%u, type=%u], dur=%d, max_dur=%d",
                    this->GetName(),
                    this->GetID(),
                    pItem->GetID(),
                    pItem->GetInt(ITEMDATA_TYPE),
                    pItem->GetInt(ITEMDATA_AMOUNT),
                    pItem->GetInt(ITEMDATA_AMOUNTLIMIT));
    }
    DEBUG_TRY	// VVVVVVVVVVVVVV
    DWORD	dwMoney = pItem->GetSellPrice();
    if (!this->EraseItem(idItem, SYNCHRO_TRUE))			//??? 先删除物品，以防作弊
    {
        this->SendSysMsg(STR_ITEM_INEXIST);
        return;
    }
    if (!this->GainMoney(dwMoney, SYNCHRO_TRUE))
    {
        this->SendSysMsg(STR_MONEYBAG_FULL);
    }
    DEBUG_CATCH("SellItem") // AAAAAAAAAAAAAAAAA
    UpdateWeight();
}

//////////////////////////////////////////////////////////////////////
bool CUser::DropItem(OBJID idItem, int x, int y)
{
    CHECKF(GetMap()->IsLayItemEnable(x, y));
    ItemInfoStruct info;
    CItemPtr pItem = NULL;
    bool	bDropItem = false;
    int i;
    for(i = ITEMPOSITION_EQUIPBEGIN; i < ITEMPOSITION_EQUIPEND; i++)
    {
        CItemPtr& refpEquip = GetEquipItemRef(i);
        if (refpEquip && refpEquip->GetID() == idItem)
        {
            // inform client the equipment is droped.
            CMsgItem msg;
            if (msg.Create(refpEquip->GetID(), ITEMACT_DROPEQUIPMENT, i))
            {
                this->SendMsg(&msg);
            }
            char szPrompt[255];
            sprintf(szPrompt, STR_DROPITEM_PROMPT, refpEquip->GetStr(ITEMDATA_NAME));
            this->SendSysMsg(szPrompt);
            pItem	= refpEquip;
            refpEquip	= NULL;
            // 丢弃幻兽要扣亲密度
            if (pItem->IsEudemon())
            {
                CallBackEudemon(pItem->GetID());
                DetachEudemon(pItem);
                int nFidelity = __max(0, pItem->GetInt(ITEMDATA_FIDELITY) - EUDEMON_DEC_FIDELITY_WHEN_DEAL);
                pItem->SetInt(ITEMDATA_FIDELITY, nFidelity);
            }
            //丢弃物品重新设置战魂经验值
            pItem->WarGhostLevelReset();
            pItem->GetInfo(&info);
            bDropItem	= pItem->IsExchangeEnable();
            if (pItem->IsNonsuchItem())
            {
                ::MyLogSave("gmlog/drop_item", "%s(%u) drop equip:[id=%u, type=%u], dur=%d, max_dur=%d",
                            this->GetName(),
                            this->GetID(),
                            pItem->GetID(),
                            pItem->GetInt(ITEMDATA_TYPE),
                            pItem->GetInt(ITEMDATA_AMOUNT),
                            pItem->GetInt(ITEMDATA_AMOUNTLIMIT));
            }
            pItem->DeleteRecord();				//??? 防作弊，提前删除
            pItem->ReleaseByOwner();
            pItem = NULL;
            break;
        }
    }
    if (i >= ITEMPOSITION_EQUIPEND)
    {
        pItem = GetItem(idItem);
        if (pItem)
        {
            if (pItem->IsNonsuchItem())
            {
                ::MyLogSave("gmlog/drop_item", "%s(%u) drop item:[id=%u, type=%u], dur=%d, max_dur=%d",
                            this->GetName(),
                            this->GetID(),
                            pItem->GetID(),
                            pItem->GetInt(ITEMDATA_TYPE),
                            pItem->GetInt(ITEMDATA_AMOUNT),
                            pItem->GetInt(ITEMDATA_AMOUNTLIMIT));
            }
            if (pItem->IsDiscardable())
            {
                // 丢弃幻兽要扣亲密度
                if (pItem->IsEudemon())
                {
                    CallBackEudemon(pItem->GetID());
                    DetachEudemon(pItem);
                    int nFidelity = __max(0, pItem->GetInt(ITEMDATA_FIDELITY) - EUDEMON_DEC_FIDELITY_WHEN_DEAL);
                    pItem->SetInt(ITEMDATA_FIDELITY, nFidelity);
                }
                //丢弃物品重新设置战魂经验值
                pItem->WarGhostLevelReset();
                pItem->GetInfo(&info);
                bDropItem	= pItem->IsExchangeEnable();
                char szPrompt[255];
                sprintf(szPrompt, STR_DROPITEM_PROMPT, pItem->GetStr(ITEMDATA_NAME));
                this->SendSysMsg(szPrompt);
                EraseItem(idItem, SYNCHRO_TRUE);			//??? 防作弊，提前删除
                pItem = NULL;
            }
            else
            {
                this->SendSysMsg(STR_NOT_DISCARDABLE);
                return false;
            }
        }
    }
    if (bDropItem)
    {
        POINT	pos;
        pos.x	= x;
        pos.y	= y;
        CMapItem* pMapItem = CMapItem::CreateNew();
        if (pMapItem)
        {
            IF_OK(pMapItem->Create(MapManager()->SpawnMapItemID(), GetMap(), pos, &info))
            {
                MapManager()->QueryMapItemSet()->AddObj(pMapItem);
            }
            else
            {
                this->SendSysMsg(STR_MAKE_ITEM_FAILED, pMapItem->GetInfo()->idType);
                pMapItem->ReleaseByOwner();
            }
        }
    }
    UpdateWeight();
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::DropMoney (DWORD dwMoney, int x, int y)
{
    //	CHECKF(nMoney > 0);
    CHECKF(GetMap()->IsLayItemEnable(x, y));
    if (this->GetMoney() < dwMoney)
    {
        return false;
    }
    POINT	pos;
    pos.x	= x;
    pos.y	= y;
    CMapItem* pMapItem = CMapItem::CreateNew();
    if (pMapItem)
    {
        if (pMapItem->CreateMoney(MapManager()->SpawnMapItemID(), GetMap(), pos, dwMoney))
        {
            this->SpendMoney(dwMoney, SYNCHRO_TRUE);				// 先删除，防止作弊
            MapManager()->QueryMapItemSet()->AddObj(pMapItem);
            if (this->IsAlive())
            {
                this->SendSysMsg(STR_DROP_MONEY_SUCC, dwMoney);
            }
            else
            {
                this->SendSysMsg(STR_DIE_DROP_MONEY, dwMoney);
            }
        }
        else
        {
            this->SendSysMsg(STR_MAKE_MONEY_FAILED, dwMoney);
            pMapItem->ReleaseByOwner();
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::SplitItem		(OBJID idItem, int nNum)
{
    //	if (IsItemFull(0))
    //		return false;
    CItemPtr pItem = GetItem(idItem);
    if (!pItem || !pItem->IsPileEnable() || nNum <= 0 || nNum >= pItem->GetInt(ITEMDATA_AMOUNT))
    {
        return false;
    }
    // 检查背包是否满
    if (this->IsItemFull(0, pItem->GetInt(ITEMDATA_TYPE), pItem->GetInt(ITEMDATA_AMOUNTLIMIT)))
    {
        return false;
    }
    CItemPtr pOther = pItem->Split(nNum);
    CHECKF(pOther);
    AddItem(pOther, SYNCHRO_TRUE);
    {
        CMsgItem	msg;
        IF_OK(msg.Create(pItem->GetID(), ITEMACT_SYNCHRO_AMOUNT, pItem->GetInt(ITEMDATA_AMOUNT)))
        SendMsg(&msg);
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::CombineItem		(OBJID idItem, OBJID idOther)
{
    CItemPtr pItem = GetItem(idItem);
    CItemPtr pOther = GetItem(idOther);
    if (!pItem || !pOther || !pItem->IsPileEnable() || pItem->GetInt(ITEMDATA_TYPE) != pOther->GetInt(ITEMDATA_TYPE))
    {
        return false;
    }
    int nNewNum		= pItem->GetInt(ITEMDATA_AMOUNT) + pOther->GetInt(ITEMDATA_AMOUNT);
    if (nNewNum > pItem->GetInt(ITEMDATA_AMOUNTLIMIT))
    {
        pOther->SetInt(ITEMDATA_AMOUNT, nNewNum - pItem->GetInt(ITEMDATA_AMOUNTLIMIT));
        {
            CMsgItem	msg;
            IF_OK(msg.Create(pOther->GetID(), ITEMACT_SYNCHRO_AMOUNT, pOther->GetInt(ITEMDATA_AMOUNT)))
            SendMsg(&msg);
        }
        pItem->SetInt(ITEMDATA_AMOUNT, pItem->GetInt(ITEMDATA_AMOUNTLIMIT));
        {
            CMsgItem	msg;
            IF_OK(msg.Create(pItem->GetID(), ITEMACT_SYNCHRO_AMOUNT, pItem->GetInt(ITEMDATA_AMOUNT)))
            SendMsg(&msg);
        }
    }
    else
    {
        EraseItem(idOther, SYNCHRO_TRUE);
        pItem->SetInt(ITEMDATA_AMOUNT, nNewNum);
        {
            CMsgItem	msg;
            IF_OK(msg.Create(pItem->GetID(), ITEMACT_SYNCHRO_AMOUNT, pItem->GetInt(ITEMDATA_AMOUNT)))
            SendMsg(&msg);
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
CItem* CUser::CombineNewItem	(CItem* pItem, ItemInfoStruct* pInfo, bool bSynchro)
{
    CHECKF(CItem::IsCountable(pInfo->idType));
    CHECKF(pItem->GetInt(ITEMDATA_TYPE) == pInfo->idType);
    int nNewNum		= pItem->GetInt(ITEMDATA_AMOUNT) + pInfo->nAmount;
    if (nNewNum > pItem->GetInt(ITEMDATA_AMOUNTLIMIT))
    {
        // other
        pInfo->nAmount	= nNewNum - pItem->GetInt(ITEMDATA_AMOUNTLIMIT);
        CItemPtr pOther = CItem::CreateNew();
        CHECKF(pOther);
        if (!pOther->Create(GameDataDefault()->GetGameItemData(), pInfo, GameDataDefault()->GetEudemonData()))
        {
            pOther->ReleaseByOwner();
            return NULL;
        }
        this->AddItem(pOther, bSynchro);
        // item
        pItem->SetInt(ITEMDATA_AMOUNT, pItem->GetInt(ITEMDATA_AMOUNTLIMIT));
        if (bSynchro)
        {
            CMsgItem	msg;
            IF_OK(msg.Create(pItem->GetID(), ITEMACT_SYNCHRO_AMOUNT, pItem->GetInt(ITEMDATA_AMOUNT)))
            SendMsg(&msg);
        }
        return pOther;
    }
    else
    {
        pItem->SetInt(ITEMDATA_AMOUNT, nNewNum);
        if (bSynchro)
        {
            CMsgItem	msg;
            IF_OK(msg.Create(pItem->GetID(), ITEMACT_SYNCHRO_AMOUNT, pItem->GetInt(ITEMDATA_AMOUNT)))
            SendMsg(&msg);
        }
        return pItem;
    }
}

//////////////////////////////////////////////////////////////////////
CItem* CUser::FindCombineItem	(OBJID idType)
{
    return m_pPackage->FindCombineItem(idType);
}

//////////////////////////////////////////////////////////////////////
bool CUser::SpendArrow()
{
    CItem* pArrow = GetEquipItemByPos(ITEMPOSITION_WEAPONL);
    if (!pArrow)
    {
        return false;
    }
    CHECKF(pArrow->IsArrowSort());
    // spend one
    return SpendItem(pArrow, 1, ITEMPOSITION_WEAPONL);
}

//////////////////////////////////////////////////////////////////////
bool CUser::SpendItem(CItem* pItem, int nNum/*=1*/, int nPosition/*=ITEMPOSITION_BACKPACK*/, bool bSynchro/*=true*/)	// may be erase
{
    if (pItem->IsPileEnable() && pItem->GetInt(ITEMDATA_AMOUNT) > nNum)
    {
        pItem->SetInt(ITEMDATA_AMOUNT, pItem->GetInt(ITEMDATA_AMOUNT) - nNum);
        if (bSynchro)
        {
            CMsgItem	msg;
            IF_OK(msg.Create(pItem->GetID(), ITEMACT_SYNCHRO_AMOUNT, pItem->GetInt(ITEMDATA_AMOUNT)))
            SendMsg(&msg);
        }
    }
    else
    {
        if (pItem->IsPileEnable())
        {
            ASSERT(pItem->GetInt(ITEMDATA_AMOUNT) == nNum);
        }
        if (nPosition >= ITEMPOSITION_PACK_BEGIN && nPosition < ITEMPOSITION_PACK_END)
        {
            EraseItem(pItem->GetID(), bSynchro);
        }
        else
        {
            ASSERT(nPosition >= ITEMPOSITION_EQUIPBEGIN && nPosition < ITEMPOSITION_EQUIPEND);
            EraseEquip(nPosition, bSynchro);
        }
    }
    UpdateWeight();
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::SpendItemByType(OBJID idType, bool bSynchro)
{
    CHECKF(idType);
    CItem* pItem = GetTaskItemByType(idType);
    if (!pItem)
    {
        return false;
    }
    if (pItem->IsExpend() && pItem->GetInt(ITEMDATA_AMOUNT) > 1)
    {
        pItem->SetInt(ITEMDATA_AMOUNT, pItem->GetInt(ITEMDATA_AMOUNT) - 1);
        if (bSynchro)
        {
            CMsgItem	msg;
            IF_OK(msg.Create(pItem->GetID(), ITEMACT_SYNCHRO_AMOUNT, pItem->GetInt(ITEMDATA_AMOUNT)))
            SendMsg(&msg);
        }
    }
    else
    {
        if (pItem->IsNonsuchItem())
        {
            ::MyLogSave("gmlog/SpendItemByType", "%s(%u) Spend item:[id=%u, type=%u], dur=%d, max_dur=%d",
                        this->GetName(),
                        this->GetID(),
                        pItem->GetID(),
                        pItem->GetInt(ITEMDATA_TYPE),
                        pItem->GetInt(ITEMDATA_AMOUNT),
                        pItem->GetInt(ITEMDATA_AMOUNTLIMIT));
        }
        EraseItem(pItem->GetID(), bSynchro);
    }
    UpdateWeight();
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::SpendEquipItem(int nSubType, int nNum, bool bSynchro)
{
    CHECKF(nNum > 0);
    CItem* pItem = NULL;
    if (m_pWeaponR && m_pWeaponR->GetItemSubType() == nSubType && m_pWeaponR->GetInt(ITEMDATA_AMOUNT) >= nNum)
    {
        pItem = m_pWeaponR;
    }
    else if (m_pWeaponL && m_pWeaponL->GetItemSubType() == nSubType && m_pWeaponL->GetInt(ITEMDATA_AMOUNT) >= nNum)
    {
        pItem = m_pWeaponL;
    }
    IF_NOT(pItem)
    return false;
    //ASSERT(!pItem->IsExpend() || pItem->GetInt(ITEMDATA_AMOUNT) >= nNum);
    ASSERT(pItem->IsExpend() && pItem->GetInt(ITEMDATA_AMOUNT) >= nNum);
    if (pItem->IsExpend() && pItem->GetInt(ITEMDATA_AMOUNT) > nNum)
    {
        pItem->SetInt(ITEMDATA_AMOUNT, pItem->GetInt(ITEMDATA_AMOUNT) - nNum);
        if (bSynchro)
        {
            CMsgItem	msg;
            IF_OK(msg.Create(pItem->GetID(), ITEMACT_SYNCHRO_AMOUNT, pItem->GetInt(ITEMDATA_AMOUNT)))
            SendMsg(&msg);
        }
    }
    else
    {
        if (pItem->IsNonsuchItem())
        {
            ::MyLogSave("gmlog/SpendEquipItem", "%s(%u) Spend item:[id=%u, type=%u], dur=%d, max_dur=%d",
                        this->GetName(),
                        this->GetID(),
                        pItem->GetID(),
                        pItem->GetInt(ITEMDATA_TYPE),
                        pItem->GetInt(ITEMDATA_AMOUNT),
                        pItem->GetInt(ITEMDATA_AMOUNTLIMIT));
        }
        this->EraseEquip(pItem->GetInt(ITEMDATA_POSITION), bSynchro);
    }
    UpdateWeight();
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::RepairItem		(OBJID idItem, bool bSynchro)
{
    CHECKF(idItem);
    CItemPtr pItem = GetEquipItem(idItem);
    if (!pItem)
    {
        pItem = GetItem(idItem);
    }
    if (!( pItem && pItem->IsRepairEnable() && pItem->GetInt(ITEMDATA_AMOUNTLIMIT) > pItem->GetInt(ITEMDATA_AMOUNT) ))
    {
        this->SendSysMsg(STR_REPAIR_FAILED);
        return false;
    }
    int nMoney		= pItem->CalcRepairMoney();
    int	nRepair		= pItem->GetInt(ITEMDATA_AMOUNTLIMIT) - pItem->GetInt(ITEMDATA_AMOUNT);
    if (nMoney <= 0 || nRepair <= 0)
    {
        return true;
    }
    //---jinggy如果玩家的武器或装备的耐久<=10%才修理，则战魂经验减3点---begin
    if (pItem->IsEquipment() || pItem->IsWeapon())
    {
        int nScale = pItem->GetInt(ITEMDATA_AMOUNT) * 100 / pItem->GetInt(ITEMDATA_AMOUNTLIMIT);
        if (nScale <= 10)
        {
            pItem->WarGhostLevelDec(DEC_WARGHOSTEXP_FORREPAIR);
            //通知客户端 设备属性更新
            CMsgItemInfo msg;
            IF_OK (msg.Create(pItem))
            SendMsg(&msg);
        }
    }
    //---jinggy如果玩家的武器或装备的耐久<=10%才修理，则战魂经验减3点---end
    //---jinggy---过是向帮派NPC买,修理东西，帮派成员有优惠---begin
    /*CGameMap * pSynMap = this->GetMap();
    if (pSynMap->IsSynMap())
    {
    	if (pSynMap->GetSynID() == this->GetSynID())
    	{
    		int nSynMemLevel = this->QuerySynAttr()->GetSynMemberLevel();
    		nPrice = MulDiv(nPrice, __max(0, (100-5*nSynMemLevel)), 100);
    	}
    }*/
    //---jinggy---过是向帮派NPC买,修理东西，帮派成员有优惠---end
    if (!this->SpendMoney(nMoney, SYNCHRO_TRUE))
    {
        this->SendSysMsg(STR_REPAIR_NO_MONEY_d, nMoney);
        return false;	// not enough money
    }
    /*	// 圣战要求修理装备不降低最大耐久度
    	// max durability changed
    	CHECKF(pItem->IsEquipment());

    	if (pItem->GetInt(ITEMDATA_AMOUNT) < pItem->GetInt(ITEMDATA_AMOUNTLIMIT) / 2)
    	{
    		if (::RandGet(100) < 5)
    			pItem->SetInt(ITEMDATA_AMOUNTLIMIT, ::CutTrail(1, pItem->GetInt(ITEMDATA_AMOUNTLIMIT) - 1));
    	}
    	else if (pItem->GetInt(ITEMDATA_AMOUNT) < pItem->GetInt(ITEMDATA_AMOUNTLIMIT) / 10)
    	{
    		if (::RandGet(100) < 10)
    			pItem->SetInt(ITEMDATA_AMOUNTLIMIT, ::CutTrail(1, pItem->GetInt(ITEMDATA_AMOUNTLIMIT) - 1));
    	}
    	else
    	{
    		if (::RandGet(100) < 80)
    			pItem->SetInt(ITEMDATA_AMOUNTLIMIT, ::CutTrail(1, pItem->GetInt(ITEMDATA_AMOUNTLIMIT) - 1));
    	}
    */
    pItem->SetInt(ITEMDATA_AMOUNT, pItem->GetInt(ITEMDATA_AMOUNTLIMIT), true);
    if (bSynchro)
    {
        CMsgItemInfo msg;
        CHECKF(msg.Create(pItem, ITEMINFO_UPDATE));
        SendMsg(&msg);
    }
    return true;
}


//////////////////////////////////////////////////////////////////////
bool CUser::RepairAll		(bool bSynchro)
{
    /*
    int nMoney = 0;
    for(int i = ITEMPOSITION_EQUIPBEGIN; i < ITEMPOSITION_EQUIPEND; i++)
    {
    	CItem* pEquip = GetEquipItemByPos(i);
    	if (pEquip)
    		nMoney	+= pEquip->CalcRepairMoney();
    }

    if (GetMoney() < nMoney)
    	return false;

    for(int i = ITEMPOSITION_EQUIPBEGIN; i < ITEMPOSITION_EQUIPEND; i++)
    {
    	CItem* pEquip = GetEquipItemByPos(i);
    	if (pEquip && pEquip->CalcRepairMoney() > 0)
    		pEquip->SetInt(ITEMDATA_AMOUNT, pEquip->GetInt(ITEMDATA_AMOUNTLIMIT), UPDATE_FALSE);
    }

    if (bSynchro)
    {
    	CMsgItem	msg;
    	CHECKF(msg.Create(ID_NONE, ITEMACT_REPAIRALL));
    	SendMsg(&msg);
    }

    return true;
    */
    // 暂时去掉
    return false;
}

//////////////////////////////////////////////////////////////////////
bool CUser::IdentItem		(OBJID idItem, bool bSynchro)
{
    CHECKF(idItem);
    CItemPtr pItem = GetEquipItem(idItem);
    if (!pItem)
    {
        pItem = GetItem(idItem);
    }
    if (!( pItem && pItem->IsNeedIdent() ))
    {
        return false;
    }
    //根据品质计算转换为鉴定的成功概率
    int  nQuality = pItem->GetQuality();
    int  nRate = 0;
    if (nQuality == QUALITY_ZERO)
    {
        nRate = 80;
    }
    else if (nQuality == QUALITY_ONE || nQuality == QUALITY_TWO)
    {
        nRate = 70;
    }
    else if (nQuality == QUALITY_THREE || nQuality == QUALITY_FOUR)
    {
        nRate = 50;
    }
    int	nMoney	= IDENT_FEE;		// MulDiv(pItem->GetInt(ITEMDATA_PRICE), ITEMIDENT_PERCENT, 100);
    // 钱不够
    if (nMoney && GetMoney() < nMoney)
    {
        return false;
    }
    if (nMoney)
    {
        this->SpendMoney(nMoney, SYNCHRO_TRUE);
    }
    if (::RandGet(100) < nRate)
    {
        pItem->SetIdent(0, UPDATE_FALSE);
        char szBuff[255];
        if (nQuality == QUALITY_ZERO)
        {
            sprintf(szBuff, STR_IDENT_NORMAL, pItem->GetStr(ITEMDATA_NAME));
        }
        else if (nQuality == QUALITY_ONE)
        {
            sprintf(szBuff, STR_IDENT_QUALITY_ONE, pItem->GetStr(ITEMDATA_NAME));
        }
        else if (nQuality == QUALITY_TWO)
        {
            sprintf(szBuff, STR_IDENT_QUALITY_TWO, pItem->GetStr(ITEMDATA_NAME));
        }
        else if (nQuality == QUALITY_THREE)
        {
            sprintf(szBuff, STR_IDENT_QUALITY_THREE, pItem->GetStr(ITEMDATA_NAME));
        }
        else if (nQuality == QUALITY_FOUR)
        {
            sprintf(szBuff, STR_IDENT_QUALITY_FOUR, pItem->GetStr(ITEMDATA_NAME));
        }
        this->SendSysMsg(szBuff);
        if (bSynchro)
        {
            CMsgItemInfo	msg;
            CHECKF(msg.Create(pItem));
            SendMsg(&msg);
        }
        return true;
    }
    else
    {
        //鉴定失败
        if (nQuality == QUALITY_ZERO)
        {
            nRate = 70;
        }
        else if (nQuality == QUALITY_ONE || nQuality == QUALITY_TWO)
        {
            nRate = 50;
        }
        else if (nQuality == QUALITY_THREE || nQuality == QUALITY_FOUR)
        {
            nRate = 30;
        }
        if (::RandGet(100) < nRate)
        {
            this->EraseItem(idItem, true);
            SendSysMsg(STR_ACTION_IDENT_FAIL_DISAPPEAR);
        }
        else
        {
            SendSysMsg(STR_ACTION_IDENT_FAIL);
        }
        return false;
    }
}


//////////////////////////////////////////////////////////////////////
void CUser::EmbedGem (OBJID idItem, OBJID idGem, int nPos)
{
    CItem* pItem = this->GetItem(idItem);
    IF_NOT (pItem)
    return;
    CItem* pGem	= this->GetItem(idGem);
    IF_NOT (pGem)
    return;
    nPos = __min(2, __max(1, nPos));
    ItemInfoStruct info;
    pItem->GetInfo(&info);
    switch(nPos)
    {
    case 1:
        {
            IF_OK (GEM_HOLE == info.nGem1)
            {
                pItem->SetInt(ITEMDATA_GEM1, pGem->GetInt(ITEMDATA_TYPE) % 1000);
                int nDurEffect = pItem->GetGemDurEffect(pGem->GetInt(ITEMDATA_TYPE));
                if (nDurEffect != 0)
                {
                    int nDur = pItem->GetInt(ITEMDATA_AMOUNTLIMIT);
                    pItem->SetInt(ITEMDATA_AMOUNTLIMIT, nDur*(100 + nDurEffect) / 100);
                }
                DEBUG_TRY		// VVVVVVVVVV
                pItem->SaveInfo();
                ASSERT(EraseItem(idGem, SYNCHRO_TRUE));
                DEBUG_CATCH("EmbedGem2")		// AAAAAAAAAAA
            }
        }
        break;
    case 2:
        {
            IF_OK (GEM_HOLE == info.nGem2)
            {
                pItem->SetInt(ITEMDATA_GEM2, pGem->GetInt(ITEMDATA_TYPE) % 1000);
                int nDurEffect = pItem->GetGemDurEffect(pGem->GetInt(ITEMDATA_TYPE));
                if (nDurEffect != 0)
                {
                    int nDur = pItem->GetInt(ITEMDATA_AMOUNTLIMIT);
                    pItem->SetInt(ITEMDATA_AMOUNTLIMIT, nDur*(100 + nDurEffect) / 100);
                }
                DEBUG_TRY		// VVVVVVVVVV
                pItem->SaveInfo();
                ASSERT(EraseItem(idGem, SYNCHRO_TRUE));
                DEBUG_CATCH("EmbedGem2")		// AAAAAAAAAAA
            }
        }
        break;
    default:
        {
            ::LogSave("invalid pos:%u in gem embed.", nPos);
            return;
        }
        break;
    }
    // update item info at client
    CMsgItemInfo msg;
    IF_OK (msg.Create(pItem, ITEMINFO_UPDATE))
    this->SendMsg(&msg);
}

//////////////////////////////////////////////////////////////////////
void CUser::TakeOutGem (OBJID idItem, int nPos)
{
    CItem* pItem = this->GetItem(idItem);
    IF_NOT (pItem)
    return;
    nPos = __min(2, __max(1, nPos));
    ItemInfoStruct info;
    pItem->GetInfo(&info);
    switch(nPos)
    {
    case 1:
        {
            IF_OK (GEM_NONE != info.nGem1 && GEM_HOLE != info.nGem1)
            {
                pItem->SetInt(ITEMDATA_GEM1, GEM_HOLE);
                // correct dur
                OBJID idGemType = 700000 + info.nGem1;
                int nDurEffect = pItem->GetGemDurEffect(idGemType);
                if (nDurEffect != 0)
                {
                    int nDur = pItem->GetInt(ITEMDATA_AMOUNTLIMIT);
                    pItem->SetInt(ITEMDATA_AMOUNTLIMIT, __max(0, nDur * 100 / (100 + nDurEffect)));
                    if (pItem->GetInt(ITEMDATA_AMOUNT) > pItem->GetInt(ITEMDATA_AMOUNTLIMIT))
                    {
                        pItem->SetInt(ITEMDATA_AMOUNT, pItem->GetInt(ITEMDATA_AMOUNTLIMIT));
                    }
                }
                pItem->SaveInfo();
            }
        }
        break;
    case 2:
        {
            IF_OK (GEM_NONE != info.nGem2 && GEM_HOLE != info.nGem2)
            {
                pItem->SetInt(ITEMDATA_GEM2, GEM_HOLE);
                // correct dur
                OBJID idGemType = 700000 + info.nGem2;
                int nDurEffect = pItem->GetGemDurEffect(idGemType);
                if (nDurEffect != 0)
                {
                    int nDur = pItem->GetInt(ITEMDATA_AMOUNTLIMIT);
                    pItem->SetInt(ITEMDATA_AMOUNTLIMIT, __max(0, nDur * 100 / (100 + nDurEffect)));
                    if (pItem->GetInt(ITEMDATA_AMOUNT) > pItem->GetInt(ITEMDATA_AMOUNTLIMIT))
                    {
                        pItem->SetInt(ITEMDATA_AMOUNT, pItem->GetInt(ITEMDATA_AMOUNTLIMIT));
                    }
                }
                pItem->SaveInfo();
            }
        }
        break;
    default:
        {
            ::LogSave("invalid pos:%u in gem take out.", nPos);
            return;
        }
        break;
    }
    // update item info at client
    CMsgItemInfo msg;
    IF_OK (msg.Create(pItem, ITEMINFO_UPDATE))
    this->SendMsg(&msg);
}

//////////////////////////////////////////////////////////////////////
void CUser::UpdateWeight()
{
    m_bUpdateWeight	= true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::Court	(OBJID idTarget)
{
    if (this->IsMarried())
    {
        return false;
    }
    CUser* pTargetUser = this->FindAroundUser(idTarget);
    if (!pTargetUser)
    {
        return false;
    }
    if (this->GetSex() == pTargetUser->GetSex())
    {
        return false;
    }
    this->SetApply(CUser::APPLY_MARRY, idTarget);
    CMsgInteract msg;
    IF_OK (msg.Create(INTERACT_COURT, this->GetID(), idTarget, 0, 0))
    pTargetUser->SendMsg(&msg);
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::Marry	(OBJID idTarget)
{
    if (this->IsMarried())
    {
        return false;
    }
    CUser* pTargetUser = this->FindAroundUser(idTarget);
    if (!pTargetUser)
    {
        return false;
    }
    if (pTargetUser->FetchApply(CUser::APPLY_MARRY) != this->GetID())
    {
        return false;
    }
    this->UpdateMate(pTargetUser->GetName(), true);
    pTargetUser->UpdateMate(this->GetName(), true);
    // boradcast msg
    MSGBUF szMsg;
    sprintf(szMsg, STR_MARRY, this->GetName(), pTargetUser->GetName());
    CMsgTalk msg;
    IF_OK_ (msg.Create(SYSTEM_NAME, ALLUSERS_NAME, szMsg, NULL, 0xff0000, _TXTATR_GM))
    UserManager()->BroadcastMsg(&msg);
    // fireworks
    {
        CMsgItem msg;
        IF_OK (msg.Create(this->GetID(), ITEMACT_FIREWORKS))
        this->BroadcastRoomMsg(&msg, true);
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
bool CUser::Divorce	(void)
{
    if (!this->IsMarried())
    {
        return false;
    }
    CUser* pMate = UserManager()->GetUser(this->GetMate());
    IDatabase* pDb = Database();
    IF_OK (pDb)
    {
        char szSQL[1024] = "";
        sprintf(szSQL, "INSERT INTO %s SELECT 0,%u,user.id,user.id,0,0,0,%u,0,0,0,0,0,0 FROM %s AS user where user.name='%s'",
                _TBL_ITEM,
                TYPE_DIVOICEITEM, ITEMPOSITION_BACKPACK, _TBL_USER, this->GetMate());
        IF_NOT (pDb->ExecuteSQL(szSQL))
        return false;
    }
    MSGBUF szMsg;
    sprintf(szMsg, STR_DIVORCE, this->GetName(), this->GetMate());
    this->BroadcastRoomMsg(szMsg, true);
    if (pMate)
    {
        pMate->UpdateMate(NOMATE_NAME, true);
    }
    else
    {
        char szSQL[1024] = "";
        sprintf(szSQL, "UPDATE %s SET mate='%s' WHERE name='%s' LIMIT 1", _TBL_USER, NOMATE_NAME, this->GetMate());
        pDb->ExecuteSQL(szSQL);
    }
    this->UpdateMate(NOMATE_NAME, true);
    return true;
}

//////////////////////////////////////////////////////////////////////
void CUser::UpdateMate	(const char* pszName, bool bUpdate/* = true*/)
{
    IF_NOT (pszName && strlen(pszName) < _MAX_NAMESIZE)
    return;
    m_data.SetMate(pszName, true);
    if (bUpdate)
    {
        CMsgName msg;
        IF_OK (msg.Create(NAMEACT_MATE, pszName, this->GetID()))
        this->SendMsg(&msg);
    }
}

//////////////////////////////////////////////////////////////////////
bool CUser::IsMarried(void)
{
    const char* pszMate = this->GetMate();
    return (0 != strcmp(pszMate, NOMATE_NAME));
}

//////////////////////////////////////////////////////////////////////
bool CUser::IsMate(CUser* pUser)
{
    IF_NOT (pUser)
    return false;
    return (0 == strcmp(this->GetMate(), pUser->GetName()));
}

//////////////////////////////////////////////////////////////////////
void CUser::UpdateEquipmentDurability(void)
{
    for(int i = ITEMPOSITION_EQUIPBEGIN; i < ITEMPOSITION_EQUIPEND; i++)
    {
        if (i == ITEMPOSITION_RINGR ||
                i == ITEMPOSITION_RINGL ||
                i == ITEMPOSITION_WEAPONR ||
                i == ITEMPOSITION_WEAPONL)
        {
            continue;
        }
        this->AddEquipmentDurability(i, -1);
    }
}

//////////////////////////////////////////////////////////////////////
void CUser::AddEquipmentDurability(int nPosition, int nAddValue)
{
    ASSERT(nAddValue < 0);
    CItemPtr& refpEquip = this->GetEquipItemRef(nPosition);
    if (!refpEquip || refpEquip->IsExpend() || refpEquip->IsNeverWear())
    {
        return;
    }
    int nOldDur	= refpEquip->GetInt(ITEMDATA_AMOUNT);
    int nDurability = __max(0, refpEquip->GetInt(ITEMDATA_AMOUNT) + nAddValue);
    refpEquip->SetInt(ITEMDATA_AMOUNT, nDurability);
    if (nDurability < 100)
    {
        if ((nDurability % 10) == 0)
        {
            this->SendSysMsg(_TXTATR_GM, STR_DAMAGED_REPAIR, refpEquip->GetStr(ITEMDATA_NAME));
        }
    }
    else if (nDurability < 200)
    {
        if ((nDurability % 20) == 0)
        {
            this->SendSysMsg(_TXTATR_GM, STR_DURABILITY_REPAIR, refpEquip->GetStr(ITEMDATA_NAME));
        }
    }
    if (nOldDur / 100 != nDurability / 100
            || nDurability <= 0)	// should update info to client (nDurability%100) == 0
    {
        CMsgItem msg;	// if nDurability equal zero, the client will auto delete this equipment
        if (msg.Create(nPosition, ITEMACT_DURABILITY, nDurability))
        {
            this->SendMsg(&msg);
        }
        if (nDurability == 0 && !refpEquip->IsMount()) // this equipment is damaged
        {
            ::MyLogSave("gmlog/wareout_item", "%s(%u) wear out item:[id=%u, type=%u], dur=%d, max_dur=%d",
                        this->GetName(),
                        this->GetID(),
                        refpEquip->GetID(),
                        refpEquip->GetInt(ITEMDATA_TYPE),
                        refpEquip->GetInt(ITEMDATA_AMOUNT),
                        refpEquip->GetInt(ITEMDATA_AMOUNTLIMIT));
            EraseEquip(nPosition, SYNCHRO_TRUE);
            /*
            			CMsgItem msg;
            			if (msg.Create(refpEquip->GetID(), ITEMACT_DROPEQUIPMENT, nPosition))
            				this->SendMsg(&msg);

            			refpEquip->DeleteRecord();
            			refpEquip->ReleaseByOwner();
            			refpEquip = NULL;

            			CMsgPlayer msgPlayer;
            			if (msgPlayer.Create(this->QueryRole()))
            				this->BroadcastRoomMsg(&msgPlayer, EXCLUDE_SELF);
            */
        }
        else
        {
            refpEquip->SaveInfo();
        }
    }
}

//////////////////////////////////////////////////////////////////////
int CUser::GetWeight()
{
    if (m_bUpdateWeight)
    {
        m_nAllWeight = 0;
        if (m_pHelmet)
        {
            m_nAllWeight	+= m_pHelmet->GetWeight();
        }
        if (m_pNecklace)
        {
            m_nAllWeight	+= m_pNecklace->GetWeight();
        }
        if (m_pArmor)
        {
            m_nAllWeight	+= m_pArmor->GetWeight();
        }
        if (m_pWeaponR)
        {
            m_nAllWeight	+= m_pWeaponR->GetWeight();
        }
        if (m_pWeaponL)
        {
            m_nAllWeight	+= m_pWeaponL->GetWeight();
        }
        if (m_pRingR)
        {
            m_nAllWeight	+= m_pRingR->GetWeight();
        }
        if (m_pRingL)
        {
            m_nAllWeight	+= m_pRingL->GetWeight();
        }
        if (m_pShoes)
        {
            m_nAllWeight	+= m_pShoes->GetWeight();
        }
        if (m_pMount)
        {
            m_nAllWeight	+= m_pMount->GetWeight();
        }
        if (m_pSprite)
        {
            m_nAllWeight	+= m_pSprite->GetWeight();
        }
        if (m_pMantle)
        {
            m_nAllWeight	+= m_pMantle->GetWeight();
        }
        m_nAllWeight	+= m_pPackage->GetWeight();
        m_bUpdateWeight = false;
    }
    return m_nAllWeight;
}

//////////////////////////////////////////////////////////////////////
bool CUser::PickMapItem(OBJID idItem)
{
    CMapItem* pMapItem = MapManager()->QueryMapItem(idItem);
    if (!pMapItem)
    {
        return false;
    }
    if (this->GetDistance(pMapItem->QueryMapThing()) > PICKMAPITEMDIST_LIMIT)
    {
        this->SendSysMsg(STR_FAR_CANNOT_PICK);
        return false;
    }
    if (!pMapItem->IsMoney() &&
            this->IsItemFull(pMapItem->GetWeight(), pMapItem->GetType(), pMapItem->GetInfo()->nAmountLimit))
    {
        if (CItem::IsGhostGem(pMapItem->GetType()))
        {
            this->SendSysMsg(STR_GEMBAG_FULL);
        }
        else
        {
            this->SendSysMsg(STR_FULL_CANNOT_PICK);
        }
        return false;
    }
    OBJID idOwner = pMapItem->GetPlayerID();
    if (pMapItem->IsPriv() && idOwner != GetID())
    {
        CUser* pOwner = UserManager()->GetUser(idOwner);
        if (!(pOwner && this->IsMate(pOwner)))
        {
            CTeam* pTeam = this->GetTeam();
            if (pTeam && pTeam->IsTeamMember(idOwner)
                    && (pMapItem->IsMoney() && !pTeam->IsCloseMoney() || !pMapItem->IsMoney() && !pTeam->IsCloseItem() || pMapItem->IsGem() && !pTeam->IsCloseGemAccess())
                    && !(pMapItem->GetType() == TYPE_DRAGONBALL || pMapItem->GetType() == TYPE_SHOOTINGSTAR) )
            {
            }
            else
            {
                SendSysMsg(STR_OTHERS_ITEM);
                return false;
            }
        }
    }
    if (pMapItem->IsMoney())
    {
        int	nMoney = pMapItem->GetAmount();
        if (this->GetMoney() + nMoney > _MAX_MONEYLIMIT)
        {
            SendSysMsg(STR_TOOMUCH_MONEY);
            return false;
        }
        MapManager()->QueryMapItemSet()->DelObj(pMapItem->GetID());		//??? 先删除，以防作弊
        GainMoney(nMoney, SYNCHRO_TRUE);
        if (nMoney >= PICK_MORE_MONEY)
        {
            CMsgAction	msg;
            IF_OK(msg.Create(GetID(), GetPosX(), GetPosY(), GetDir(), actionGetMoney, nMoney))
            SendMsg(&msg);
        }
        this->SendSysMsg(STR_PICK_MONEY, nMoney);
    }
    else
    {
        CItem* pItem = NULL;
        ItemInfoStruct info;
        memcpy(&info, pMapItem->GetInfo(), sizeof(info));
        info.idOwner	= GetID();
        info.idPlayer	= GetID();
        info.nPosition	= this->QueryPackage()->GetItemPositionByType(info.idType);
        DEBUG_TRY
        {
            MapManager()->QueryMapItemSet()->DelObj(pMapItem->GetID());		//??? 先删除，以防作弊
            pItem = this->AwardItem(&info, SYNCHRO_TRUE);
        } DEBUG_CATCH("CUser::PickMapItem")
        if (pItem)
        {
            if (pItem->IsNonsuchItem())
            {
                ::MyLogSave("gmlog/pick_item", "%s(%u) pick item:[id=%u, type=%u], dur=%d, max_dur=%d",
                            this->GetName(),
                            this->GetID(),
                            pItem->GetID(),
                            pItem->GetInt(ITEMDATA_TYPE),
                            info.nAmount,
                            info.nAmountLimit);
            }
            if (pItem->IsNeedIdent())
            {
                this->SendSysMsg(STR_GOT_IDENT_ITEM, pItem->GetStr(ITEMDATA_NAME));
            }
            else
            {
                this->SendSysMsg(STR_GOT_ITEM, pItem->GetStr(ITEMDATA_NAME));
            }
        }
    }
    // perform action of pick
    CMsgMapItem msg;
    IF_OK (msg.Create(this->GetID(), this->GetPosX(), this->GetPosY(), MSGMAPITEM_PICK))
    this->BroadcastRoomMsg(&msg, EXCLUDE_SELF);
    return true;
}

/////////////////////////////////////////////////////////////////////////////
// squear deal
/////////////////////////////////////////////////////////////////////////////
bool CUser::CreateSquareDeal(CUser* pTarget)
{
    ISquareDeal* pSquareDeal = CSquareDeal::CreateNew();
    CHECKF(pSquareDeal);
    if (pSquareDeal->Create(&m_ifaceSquareDeal, &pTarget->m_ifaceSquareDeal))
    {
        return true;
    }
    pSquareDeal->Release();
    return false;
}

/////////////////////////////////////////////////////////////////////////////
void CUser::XUserSquareDeal::CheckDistance()
{
    if (!m_pSquareDeal)
    {
        return ;
    }
    if (m_pSquareDeal->GetTarget()->GetMapID() == This()->GetMapID()
            && m_pSquareDeal->GetTarget()->GetDistance(This()->GetPosX(), This()->GetPosY()) <= CELLS_PER_VIEW)
    {
        return ;
    }
    m_pSquareDeal->Release();		// 关闭交易系统
    return ;
}

/////////////////////////////////////////////////////////////////////////////
bool CUser::XUserSquareDeal::Create(ISquareDeal* pSquareDeal)
{
    if (m_pSquareDeal)
    {
        return false;
    }
    m_pSquareDeal = pSquareDeal;
    // msg
    CMsgTrade	msg;
    CHECKF(msg.Create(_TRADE_OPEN, m_pSquareDeal->GetTarget()->GetID()));
    This()->SendMsg(&msg);
    return true;
}

/////////////////////////////////////////////////////////////////////////////
ULONG CUser::XUserSquareDeal::Release()
{
    CHECKF(m_pSquareDeal);
    // msg
    m_pSquareDeal = NULL;
    return 1;
}

/////////////////////////////////////////////////////////////////////////////
bool CUser::XUserSquareDeal::AddItem(CItem* pItem, int nAmount, int nAllWeight)
{
    CHECKF(m_pSquareDeal);
    CHECKF (pItem);
    IUserPackage* pPackage = This()->QueryPackage();
    if (!pPackage)
    {
        return false;
    }
    int nPosition = pPackage->GetItemPosition(pItem);
    if (!This()->IsBackPackSpare(nAmount, nAllWeight, ID_NONE, nPosition))
    {
        return false;
    }
    // msg
    CMsgItemInfo	msg;
    CHECKF(msg.Create(pItem, ITEMINFO_TRADE));
    This()->SendMsg(&msg);
    return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CUser::XUserSquareDeal::AddMoney(int nAllMoney)
{
    CHECKF(m_pSquareDeal);
    if (This()->GetMoney() + nAllMoney > _MAX_MONEYLIMIT)
    {
        return false;
    }
    // msg
    CMsgTrade	msg;
    CHECKF(msg.Create(_TRADE_MONEYALL, nAllMoney));
    This()->SendMsg(&msg);
    return true;
}

/////////////////////////////////////////////////////////////////////////////
void CUser::XUserSquareDeal::ClickOK()
{
    CHECK(m_pSquareDeal);
    // msg
    CMsgTrade	msg;
    CHECK(msg.Create(_TRADE_OK, 0));
    This()->SendMsg(&msg);
    return ;
}
int CUser::GetAuctionPackageAmount(OBJID idNpc, int nPosition/* = ITEMPOSITION_AUCTION_STORAGE*/)
{
    if (!m_pStorage)
    {
        m_pStorage	= CPackage::CreateNew();
    }
    CHECKF(m_pStorage);
    //    idNpc	= STORAGE_AUCTION_NPC_ID;
    CHECKF(m_pStorage->Create(nPosition, GetID(), GetID(), Database()));
    int count = 0;
    for(int i = 0; i < m_pStorage->GetAmount(); i++)
    {
        if (m_pStorage->GetItemByIndex(i) && m_pStorage->GetItemByIndex(i)->GetInt(ITEMDATA_TYPE) != AUCTION_CHIP)
        {
            count++;
        }
        if (count >= PACKAGE_AUCTION_LIMIT)
        {
            break;
        }
    }
    return count;
}
/////////////////////////////////////////////////////////////////////////////
// IStorage
/////////////////////////////////////////////////////////////////////////////
bool CUser::SendStorageInfo(OBJID idNpc, int nPosition /*= ITEMPOSITION_STORAGE*/)
{
    if (!m_pStorage)
    {
        m_pStorage	= CPackage::CreateNew();
    }
    CHECKF(m_pStorage);
    if (nPosition == ITEMPOSITION_AUCTION_STORAGE)
    {
        idNpc = GetID();
    }
    else
    {
        idNpc	= STORAGE_NPC_ID;    //? 信仰仓库互通
    }
    CHECKF(m_pStorage->Create(nPosition, idNpc, GetID(), Database()));
    return m_pStorage->SendInfo(this);
}
//LW为了将拍买物品从Ｎｐｃ拍买仓库放入玩家拍买仓库
bool CUser::AuctionGoodsBackOff(CItem* pItem, int nPosition)
{
    if (!m_pStorage)
    {
        m_pStorage	= CPackage::CreateNew();
    }
    CHECKF(m_pStorage);
    CHECKF(m_pStorage->Create(nPosition, GetID(), GetID(), Database()));
    return m_pStorage->CheckIn(pItem);
}

/////////////////////////////////////////////////////////////////////////////
bool CUser::CheckIn(OBJID idNpc, CItem* pItem, bool bUpdate /*= true*/, int nPosition/*= ITEMPOSITION_STORAGE*/)
{
    if (!m_pStorage)
    {
        m_pStorage	= CPackage::CreateNew();
    }
    CHECKF(m_pStorage);
    idNpc	= STORAGE_NPC_ID;		//? 信仰仓库互通
    CHECKF(m_pStorage->Create(nPosition, idNpc, GetID(), Database()));
    return m_pStorage->CheckIn(this, pItem, bUpdate);
}


/////////////////////////////////////////////////////////////////////////////
CItem* CUser::CheckOut(OBJID idNpc, OBJID idItem, bool bUpdate, int nPosition/*= ITEMPOSITION_STORAGE*/)
{
    CHECKF(m_pStorage);
    if (nPosition == ITEMPOSITION_AUCTION_STORAGE)
    {
        idNpc	= GetID();
    }
    else
    {
        idNpc   = STORAGE_NPC_ID;    //? 信仰仓库互通
    }
    CHECKF(m_pStorage->Create(nPosition, idNpc, GetID(), Database()));
    return m_pStorage->CheckOut(this, idItem, bUpdate);
}


/////////////////////////////////////////////////////////////////////////////
OBJID CUser::ChkUpEqQuality	(CItem* pItem, bool bSendHint/*=false*/)
{
    if (!(pItem && pItem->IsEquipment()))
    {
        return ID_NONE;
    }
    //if (pItem->GetItemSubType() == 112)
    //	return ID_NONE;
    if (pItem->GetInt(ITEMDATA_AMOUNT) < pItem->GetInt(ITEMDATA_AMOUNTLIMIT))
    {
        if (bSendHint)
        {
            this->SendSysMsg(STR_REPAIR_THEN_IMPROVE);
        }
        return ID_NONE;
    }
    int nQuality = pItem->GetQuality();
    if (nQuality < 3 || nQuality >= 9)
    {
        if (bSendHint)
        {
            this->SendSysMsg(STR_SUPERIOR_ALREADY);
        }
        return ID_NONE;
    }
    OBJID idType = pItem->GetInt(ITEMDATA_TYPE);
    if (nQuality < 5)
    {
        idType		= (idType / 10) * 10 + 5;
    }
    OBJID idNewType = idType + 1;
    if (!ItemType()->QueryItemType(idNewType))
    {
        if (bSendHint)
        {
            this->SendSysMsg(STR_CONNOT_IMPROVE);
        }
        return ID_NONE;
    }
    return idNewType;
}

/////////////////////////////////////////////////////////////////////////////
OBJID CUser::ChkUpEqLevel	(CItem* pItem, bool bSendHint/*=false*/)
{
    if (!(pItem && pItem->IsEquipment()))
    {
        return ID_NONE;
    }
    switch(pItem->GetItemSubType())
    {
    case 112:
    case 135:
    case 136:
    case 137:
    case 138:
    case 139:
        {
            if (bSendHint)
            {
                this->SendSysMsg(STR_UPGRADE_NOMORE);
            }
            return ID_NONE;
        }
        break;
    }
    int nQuality = pItem->GetQuality();
    if (nQuality < 3)
    {
        return ID_NONE;
    }
    if (pItem->GetInt(ITEMDATA_AMOUNT) / 100 < pItem->GetInt(ITEMDATA_AMOUNTLIMIT) / 100)
    {
        if (bSendHint)
        {
            this->SendSysMsg(STR_REPAIR_THEN_UPGRADE);
        }
        return ID_NONE;
    }
    OBJID idType	= pItem->GetInt(ITEMDATA_TYPE);
    OBJID idNewType = idType;
    if (pItem->IsShield() || pItem->IsArmor() || pItem->IsHelmet())
    {
        if ((idType / 10) % 10 >= 9)
        {
            if (pItem->IsArmor())
            {
                // 千位加5
                idNewType += 5000;
            }
            else if (pItem->IsHelmet())
            {
                // 10位＝千位，千位＝2
                int nThousand = (idType / 1000) % 10;
                idNewType -= nThousand * 1000;
                idNewType += 2000;
                int nTen = (idType / 10) % 10;
                idNewType -= nTen * 10;
                idNewType += nThousand * 10;
            }
            // change detection...
            if (idType == idNewType)
            {
                // no change
                if (bSendHint)
                {
                    this->SendSysMsg(STR_UPGRADE_NOMORE);
                }
                return ID_NONE;
            }
            else
            {
                // got new to uplev
                if (ItemType()->QueryItemType(idNewType))
                {
                    // new type found
                    return idNewType;
                }
                else
                {
                    if (bSendHint)
                    {
                        this->SendSysMsg(STR_UPGRADE_NOMORE);
                    }
                    return ID_NONE;
                }
            }
        }
        while(true)
        {
            idNewType += 10;
            if (ItemType()->QueryItemType(idNewType))
            {
                break;
            }
            // not found this type
            if ((idNewType / 10) % 10 >= 9)
            {
                if (bSendHint)
                {
                    this->SendSysMsg(STR_UPGRADE_NOMORE);
                }
                return ID_NONE;
            }
        }
    }
    else
    {
        if ((idType / 10) % 100 >= 22)
        {
            if (bSendHint)
            {
                this->SendSysMsg(STR_UPGRADE_NOMORE);
            }
            return ID_NONE;
        }
        while (true)
        {
            idNewType += 10;
            if (ItemType()->QueryItemType(idNewType))
            {
                break;
            }
            // not found
            if ((idNewType % 1000) / 10 >= 22)
            {
                if (bSendHint)
                {
                    this->SendSysMsg(STR_UPGRADE_NOMORE);
                }
                return ID_NONE;
            }
        }
    }
    return idNewType;
}

/////////////////////////////////////////////////////////////////////////////
bool CUser::GetUpEpQualityInfo	(CItem* pItem, int& nChance, OBJID& idNewType, bool bSendHint/*=false*/)
{
    idNewType = this->ChkUpEqQuality(pItem, bSendHint);
    if (ID_NONE == idNewType)
    {
        return false;
    }
    // changce
    nChance = 100;
    switch(pItem->GetQuality())
    {
    case 6:
        nChance = 50;
        break;
    case 7:
        nChance = 33;
        break;
    case 8:
        nChance = 20;
        break;
    default:
        nChance = 100;
        break;
    }
    int nFactor = pItem->GetInt(ITEMDATA_REQ_LEVEL);
    if (nFactor > 70 )
    {
        nChance = nChance * (100 - (nFactor - 70) * 1.0) / 100;
    }
    nChance = __max(1, nChance);
    return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CUser::GetUpEpLevelInfo	(CItem* pItem, int& nChance, OBJID& idNewType, bool bSendHint/*=false*/)
{
    idNewType = this->ChkUpEqLevel(pItem, bSendHint);
    if (ID_NONE == idNewType)
    {
        return false;
    }
    OBJID idType = pItem->GetInt(ITEMDATA_TYPE);
    nChance = 100;
    if (pItem->IsShield() || pItem->IsArmor() || pItem->IsHelmet())
    {
        int nLev = (idType % 100) / 10;
        switch(nLev)
        {
        case 5:
            nChance = 50;
            break;
        case 6:
            nChance = 40;
            break;
        case 7:
            nChance = 30;
            break;
        case 8:
        case 9:
            nChance = 20;
            break;
        default:
            nChance = 500;
            break;
        }
        int nQuality = idType % 10;
        switch(nQuality)
        {
        case 6:
            nChance = nChance * 90 / 100;
            break;
        case 7:
            nChance = nChance * 70 / 100;
            break;
        case 8:
            nChance = nChance * 30 / 100;
            break;
        case 9:
            nChance = nChance * 10 / 100;
            break;
        default:
            break;
        }
    }
    else
    {
        int nLev = (idType % 1000) / 10;
        switch(nLev)
        {
        case 11:
            nChance = 95;
            break;
        case 12:
            nChance = 90;
            break;
        case 13:
            nChance = 85;
            break;
        case 14:
            nChance = 80;
            break;
        case 15:
            nChance = 75;
            break;
        case 16:
            nChance = 70;
            break;
        case 17:
            nChance = 65;
            break;
        case 18:
            nChance = 60;
            break;
        case 19:
            nChance = 55;
            break;
        case 20:
            nChance = 50;
            break;
        case 21:
            nChance = 45;
            break;
        case 22:
            nChance = 40;
            break;
        default:
            nChance = 500;
            break;
        }
        int nQuality = idType % 10;
        switch(nQuality)
        {
        case 6:
            nChance = nChance * 90 / 100;
            break;
        case 7:
            nChance = nChance * 70 / 100;
            break;
        case 8:
            nChance = nChance * 30 / 100;
            break;
        case 9:
            nChance = nChance * 10 / 100;
            break;
        default:
            break;
        }
    }
    // change range
    nChance = __min(100, __max(1, nChance));
    return true;
}


/////////////////////////////////////////////////////////////////////////////
// return -1 means some thing wrong
int	CUser::GetChanceUpEpQuality	(int nPosition, bool bSendHint/*=false*/)
{
    CItem* pItem = GetEquipItemByPos(nPosition);
    if (!pItem)
    {
        return -1;
    }
    OBJID idNewType = ID_NONE;
    int nChance = 0;
    if (this->GetUpEpQualityInfo(pItem, nChance, idNewType, bSendHint))
    {
        return nChance;
    }
    else
    {
        return -1;
    }
}

/////////////////////////////////////////////////////////////////////////////
int CUser::GetChanceUpEpLevel	(int nPosition, bool bSendHint/* = false*/)
{
    CItem* pItem = GetEquipItemByPos(nPosition);
    if (!pItem)
    {
        return -1;
    }
    OBJID idNewType = ID_NONE;
    int nChance = 0;
    if (this->GetUpEpLevelInfo(pItem, nChance, idNewType, bSendHint))
    {
        return nChance;
    }
    else
    {
        return -1;
    }
}

/////////////////////////////////////////////////////////////////////////////
bool CUser::UpEquipmentQuality		(int nPosition)
{
    CItem* pItem = GetEquipItemByPos(nPosition);
    if (!pItem)
    {
        return false;
    }
    // get up quality info
    OBJID idNewType = ID_NONE;
    int nChance = 0;
    if (!this->GetUpEpQualityInfo(pItem, nChance, idNewType, true))
    {
        return false;
    }
    // gem cost
    int nGemCost = 100 / nChance + 1;
    if (!this->MultiCheckItem(TYPE_DRAGONBALL, TYPE_DRAGONBALL, nGemCost))
    {
        return false;
    }
    {
        // change type
        IF_NOT (pItem->ChangeType(idNewType))
        return false;
        // take gem for quality lev up
        this->MultiDelItem(TYPE_DRAGONBALL, TYPE_DRAGONBALL, nGemCost);
        // durability, 2 percent chance to increase 100-300 point
        int nAmountLimit = pItem->GetInt(ITEMDATA_AMOUNTLIMIT_ORIGINAL);
        if (::RandGet(100) < 2)
        {
            nAmountLimit += (::RandGet(3) + 1) * 100;
        }
        // durabal gem effect
        nAmountLimit = nAmountLimit * (100 + pItem->GetGemDurEffect()) / 100;
        // update durability
        pItem->SetInt(ITEMDATA_AMOUNTLIMIT, nAmountLimit, UPDATE_FALSE);
        pItem->SetInt(ITEMDATA_AMOUNT, nAmountLimit, UPDATE_FALSE);
        // gem hole, 1 percent chance to make a hole
        if (::RandGet(100) < 1)
        {
            if (GEM_NONE == pItem->GetInt(ITEMDATA_GEM1))
            {
                pItem->SetInt(ITEMDATA_GEM1, GEM_HOLE);
            }
            else if (GEM_NONE == pItem->GetInt(ITEMDATA_GEM2))
            {
                pItem->SetInt(ITEMDATA_GEM2, GEM_HOLE);
            }
        }
        // save to db
        pItem->SaveInfo();
        // inform client
        /*	CMsgItemInfo msg;
        IF_OK (msg.Create(pItem, ITEMINFO_UPDATE))
        	this->SendMsg(&msg); */
        CMsgItemInfoEx msg;
        IF_OK (msg.Create(pItem, this->GetID(), 0, ITEMINFOEX_EQUIPMENT))
        this->BroadcastRoomMsg(&msg, true);
    }
    return true;
}

/////////////////////////////////////////////////////////////////////////////
void CUser::UpEquipmentQuality	(OBJID idEquipment, OBJID idTreasure)
{
    CItem* pTreasure = this->GetItem(idTreasure);
    IF_NOT (pTreasure)
    return;
    if (pTreasure->GetInt(ITEMDATA_TYPE) != TYPE_DRAGONBALL)
    {
        this->SendSysMsg(STR_DRAGONBALL);
        return;
    }
    CItem* pItem = this->GetItem(idEquipment);
    IF_NOT (pItem)
    return;
    // quality
    int nQuality = pItem->GetQuality();
    // get info
    OBJID idNewType = ID_NONE;
    int nChance = 0;
    if (!this->GetUpEpQualityInfo(pItem, nChance, idNewType, true))
    {
        return;
    }
    // show time
    this->EraseItem(idTreasure, SYNCHRO_TRUE);
    // backup item info
    ItemInfoStruct infoItem;
    pItem->GetInfo(&infoItem);
    if (::RandGet(100) < nChance)
    {
        // succeed
        // delete old item
        this->EraseItem(idEquipment, SYNCHRO_TRUE);
        // award new item
        CItem* pItem = this->AwardItem(idNewType, SYNCHRO_FALSE, IDENT_OK, NO_COMBINE);
        IF_NOT (pItem)
        return;
        // restore info
        pItem->SetInt(ITEMDATA_GEM1, infoItem.nGem1, UPDATE_FALSE);
        pItem->SetInt(ITEMDATA_GEM2, infoItem.nGem2, UPDATE_FALSE);
        pItem->SetInt(ITEMDATA_MAGIC1, infoItem.nMagic1, UPDATE_FALSE);
        pItem->SetInt(ITEMDATA_MAGIC2, infoItem.nMagic2, UPDATE_FALSE);
        pItem->SetInt(ITEMDATA_MAGIC3, infoItem.nMagic3, UPDATE_FALSE);
        pItem->SetInt(ITEMDATA_DATA, infoItem.nData, UPDATE_FALSE);
        // durability
        int nAmountLimit = pItem->GetInt(ITEMDATA_AMOUNTLIMIT);
        if (::RandGet(100) < 2)
        {
            nAmountLimit += (::RandGet(3) + 1) * 100;
            pItem->SetInt(ITEMDATA_AMOUNTLIMIT, nAmountLimit, UPDATE_FALSE);
        }
        int nDurEffect = pItem->GetGemDurEffect(700000 + pItem->GetInt(ITEMDATA_GEM1));
        nDurEffect += pItem->GetGemDurEffect(700000 + pItem->GetInt(ITEMDATA_GEM2));
        if (nDurEffect > 0)
        {
            nAmountLimit = nAmountLimit * (100 + nDurEffect) / 100;
            pItem->SetInt(ITEMDATA_AMOUNTLIMIT, nAmountLimit, UPDATE_FALSE);
            pItem->SetInt(ITEMDATA_AMOUNT, nAmountLimit, UPDATE_FALSE);
        }
        // gem hole
        if (::RandGet(100) < 1)
        {
            if (GEM_NONE == pItem->GetInt(ITEMDATA_GEM1))
            {
                pItem->SetInt(ITEMDATA_GEM1, GEM_HOLE);
            }
            else if (GEM_NONE == pItem->GetInt(ITEMDATA_GEM2))
            {
                pItem->SetInt(ITEMDATA_GEM2, GEM_HOLE);
            }
        }
        // save to db
        pItem->SaveInfo();
        // inform client
        CMsgItemInfo msg;
        IF_OK (msg.Create(pItem))
        this->SendMsg(&msg);
    }
    else
    {
        // failed
        if (8 == nQuality)
        {
            // if (::RandGet(100) < 50)
            /*
            if (false)
            {
            	// delete old item
            	this->EraseItem(idEquipment, SYNCHRO_TRUE);

            	// award new item
            	OBJID idNewType = idType-1;
            	CItem* pItem = this->AwardItem(idNewType, SYNCHRO_FALSE, IDENT_OK, NO_COMBINE);
            	IF_NOT (pItem)
            		return;

            	// restore info
            	pItem->SetInt(ITEMDATA_GEM1, infoItem.nGem1, UPDATE_FALSE);
            	pItem->SetInt(ITEMDATA_GEM2, infoItem.nGem2, UPDATE_FALSE);

            	pItem->SetInt(ITEMDATA_MAGIC1, infoItem.nMagic1, UPDATE_FALSE);
            	pItem->SetInt(ITEMDATA_MAGIC2, infoItem.nMagic2, UPDATE_FALSE);
            	pItem->SetInt(ITEMDATA_MAGIC3, infoItem.nMagic3, UPDATE_FALSE);

            	pItem->SetInt(ITEMDATA_DATA, infoItem.nData, UPDATE_FALSE);

            	// decrease durability
            	int nAmountLimit = pItem->GetInt(ITEMDATA_AMOUNTLIMIT);
            	pItem->SetInt(ITEMDATA_AMOUNT, nAmountLimit/2);

            	// save to db
            	pItem->SaveInfo();

            	// inform client
            	CMsgItemInfo msg;
            	IF_OK (msg.Create(pItem))
            		this->SendMsg(&msg);
            }
            else
            */
            {
                // decrease durability
                int nAmountLimit = pItem->GetInt(ITEMDATA_AMOUNTLIMIT);
                if (::RandGet(100) < PERCENT_DECDUR)
                {
                    if (nAmountLimit / 100 > 1)
                    {
                        nAmountLimit -= 100;
                        pItem->SetInt(ITEMDATA_AMOUNTLIMIT, nAmountLimit);
                    }
                }
                int nAmountNew = nAmountLimit;
                int nOrgAmountLimit = pItem->GetInt(ITEMDATA_AMOUNTLIMIT_ORIGINAL);
                nAmountNew -= nOrgAmountLimit / 2;
                if (nAmountNew > pItem->GetInt(ITEMDATA_AMOUNTLIMIT))
                {
                    nAmountNew = pItem->GetInt(ITEMDATA_AMOUNTLIMIT) / 2;
                }
                pItem->SetInt(ITEMDATA_AMOUNT, nAmountNew);
                // save to db
                pItem->SaveInfo();
                // inform client
                CMsgItemInfo msg;
                IF_OK (msg.Create(pItem, ITEMINFO_UPDATE))
                this->SendMsg(&msg);
            }
        }
        else// if (7 == nQuality)
        {
            //if (::RandGet(100) < 50)
            /*
            if (false)
            {
            	// delete old item
            	this->EraseItem(idEquipment, SYNCHRO_TRUE);

            	// award new item
            	OBJID idNewType = idType-1;
            	CItem* pItem = this->AwardItem(idNewType, SYNCHRO_FALSE, IDENT_OK, NO_COMBINE);
            	IF_NOT (pItem)
            		return;

            	// restore info
            	pItem->SetInt(ITEMDATA_GEM1, infoItem.nGem1, UPDATE_FALSE);
            	pItem->SetInt(ITEMDATA_GEM2, infoItem.nGem2, UPDATE_FALSE);

            	pItem->SetInt(ITEMDATA_MAGIC1, infoItem.nMagic1, UPDATE_FALSE);
            	pItem->SetInt(ITEMDATA_MAGIC2, infoItem.nMagic2, UPDATE_FALSE);
            	pItem->SetInt(ITEMDATA_MAGIC3, infoItem.nMagic3, UPDATE_FALSE);

            	pItem->SetInt(ITEMDATA_DATA, infoItem.nData, UPDATE_FALSE);

            	// decrease durability
            	int nAmountLimit = pItem->GetInt(ITEMDATA_AMOUNTLIMIT);
            	pItem->SetInt(ITEMDATA_AMOUNT, nAmountLimit/2);

            	// save to db
            	pItem->SaveInfo();

            	// inform client
            	CMsgItemInfo msg;
            	IF_OK (msg.Create(pItem))
            		this->SendMsg(&msg);
            }
            */
            // decrease durability
            int nAmountLimit = pItem->GetInt(ITEMDATA_AMOUNTLIMIT);
            if (::RandGet(100) < PERCENT_DECDUR)
            {
                if (nAmountLimit / 100 > 1)
                {
                    nAmountLimit -= 100;
                    pItem->SetInt(ITEMDATA_AMOUNTLIMIT, nAmountLimit);
                }
            }
            int nAmountNew = nAmountLimit;
            int nOrgAmountLimit = pItem->GetInt(ITEMDATA_AMOUNTLIMIT_ORIGINAL);
            nAmountNew -= nOrgAmountLimit / 2;
            if (nAmountNew > pItem->GetInt(ITEMDATA_AMOUNTLIMIT))
            {
                nAmountNew = pItem->GetInt(ITEMDATA_AMOUNTLIMIT) / 2;
            }
            pItem->SetInt(ITEMDATA_AMOUNT, nAmountNew);
            // save to db
            pItem->SaveInfo();
            // inform client
            CMsgItemInfo msg;
            IF_OK (msg.Create(pItem, ITEMINFO_UPDATE))
            this->SendMsg(&msg);
        }
    }
}


/////////////////////////////////////////////////////////////////////////////
bool CUser::UpEquipmentLevel		(int nPosition)
{
    CItem* pItem = GetEquipItemByPos(nPosition);
    if (!pItem)
    {
        return false;
    }
    // get info
    OBJID idNewType = ID_NONE;
    int nChance = 0;
    if (!this->GetUpEpLevelInfo(pItem, nChance, idNewType, true))
    {
        return false;
    }
    // check lev
    CItemTypeData* pType = ItemType()->QueryItemType(idNewType);
    if (!pType)
    {
        return false;
    }
    if (pType->GetInt(ITEMTYPEDATA_REQ_LEVEL) > this->GetLev())
    {
        this->SendSysMsg(_TXTATR_NORMAL, STR_NEXTEQP_OVERLEV);
        return false;
    }
    // gem cost
    int nGemCost = (100 / nChance + 1) * 12 / 10;
    if (!this->MultiCheckItem(TYPE_SHOOTINGSTAR, TYPE_DIVOICEITEM, nGemCost))
    {
        return false;
    }
    {
        // change type of this item
        IF_NOT (pItem->ChangeType(idNewType))
        return false;
        // take the gem cost
        this->MultiDelItem(TYPE_SHOOTINGSTAR, TYPE_DIVOICEITEM, nGemCost);
        // durability, 2 percent chance to increase 100-300 point
        int nAmountLimit = pItem->GetInt(ITEMDATA_AMOUNTLIMIT_ORIGINAL);
        if (::RandGet(100) < 2)
        {
            nAmountLimit += (::RandGet(3) + 1) * 100;
        }
        // durabal gem effect
        nAmountLimit = nAmountLimit * (100 + pItem->GetGemDurEffect()) / 100;
        // update durability
        pItem->SetInt(ITEMDATA_AMOUNTLIMIT, nAmountLimit, UPDATE_FALSE);
        pItem->SetInt(ITEMDATA_AMOUNT, nAmountLimit, UPDATE_FALSE);
        // gem hole, 1 percent chance to make a hole
        if (::RandGet(CHANCE_MAKEHOLE) < 1)
        {
            if (GEM_NONE == pItem->GetInt(ITEMDATA_GEM1))
            {
                pItem->SetInt(ITEMDATA_GEM1, GEM_HOLE);
            }
            else if (GEM_NONE == pItem->GetInt(ITEMDATA_GEM2))
            {
                pItem->SetInt(ITEMDATA_GEM2, GEM_HOLE);
            }
        }
        // save to db
        pItem->SaveInfo();
        // inform client
        /*
        CMsgItemInfo msg;
        IF_OK (msg.Create(pItem, ITEMINFO_UPDATE))
        	this->SendMsg(&msg);
        	*/
        CMsgItemInfoEx msg;
        IF_OK (msg.Create(pItem, this->GetID(), 0, ITEMINFOEX_EQUIPMENT))
        this->BroadcastRoomMsg(&msg, true);
    }
    return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CUser::DownEquipmentLevel		(int nPosition, int nUserLevel)
{
    CItem* pEquip = this->GetEquipItemByPos(nPosition);
    CHECKF(pEquip);
    int nOldLevel = pEquip->GetInt(ITEMDATA_REQ_LEVEL);
    int nNewLevel = 0;
    // TODO
    ::MyLogSave("sys\\rebirth.log", "User[%s][%d], equip[%d], level[%d] down to Level[%d]",
                this->GetName(), this->GetID(), pEquip->GetID(), nOldLevel, nNewLevel);
    return true;
}

/////////////////////////////////////////////////////////////////////////////
void CUser::UpEquipmentLevel (OBJID idEquipment, OBJID idTreasure)
{
    CItem* pTreasure = this->GetItem(idTreasure);
    IF_NOT (pTreasure)
    return;
    if (pTreasure->GetInt(ITEMDATA_TYPE) != TYPE_SHOOTINGSTAR
            && pTreasure->GetInt(ITEMDATA_TYPE) != TYPE_DIVOICEITEM)
    {
        this->SendSysMsg(STR_METEOR);
        return;
    }
    CItem* pItem = this->GetItem(idEquipment);
    IF_NOT(pItem && pItem->IsEquipment())
    return;
    // get info
    OBJID idNewType = ID_NONE;
    int nChance = 0;
    if (!this->GetUpEpLevelInfo(pItem, nChance, idNewType, true))
    {
        return;
    }
    // divoice item effect
    if (pTreasure->GetInt(ITEMDATA_TYPE) == TYPE_DIVOICEITEM)
    {
        nChance = nChance * 130 / 100;
    }
    // show time
    this->EraseItem(idTreasure, SYNCHRO_TRUE);
    // backup item info
    ItemInfoStruct infoItem;
    pItem->GetInfo(&infoItem);
    if (::RandGet(100) < nChance)
    {
        // succeed
        // delete old item
        this->EraseItem(idEquipment, SYNCHRO_TRUE);
        // award new item
        CItem* pItem = this->AwardItem(idNewType, SYNCHRO_FALSE, IDENT_OK, NO_COMBINE);
        IF_NOT (pItem)
        return;
        // restore info
        pItem->SetInt(ITEMDATA_GEM1, infoItem.nGem1, UPDATE_FALSE);
        pItem->SetInt(ITEMDATA_GEM2, infoItem.nGem2, UPDATE_FALSE);
        pItem->SetInt(ITEMDATA_MAGIC1, infoItem.nMagic1, UPDATE_FALSE);
        pItem->SetInt(ITEMDATA_MAGIC2, infoItem.nMagic2, UPDATE_FALSE);
        pItem->SetInt(ITEMDATA_MAGIC3, infoItem.nMagic3, UPDATE_FALSE);
        pItem->SetInt(ITEMDATA_DATA, infoItem.nData, UPDATE_FALSE);
        // durability
        int nAmountLimit = pItem->GetInt(ITEMDATA_AMOUNTLIMIT);
        if (::RandGet(100) < 2)
        {
            nAmountLimit += (::RandGet(3) + 1) * 100;
            pItem->SetInt(ITEMDATA_AMOUNTLIMIT, nAmountLimit, UPDATE_FALSE);
        }
        int nDurEffect = pItem->GetGemDurEffect(700000 + pItem->GetInt(ITEMDATA_GEM1));
        nDurEffect += pItem->GetGemDurEffect(700000 + pItem->GetInt(ITEMDATA_GEM2));
        if (nDurEffect > 0)
        {
            nAmountLimit = nAmountLimit * (100 + nDurEffect) / 100;
            pItem->SetInt(ITEMDATA_AMOUNTLIMIT, nAmountLimit, UPDATE_FALSE);
            pItem->SetInt(ITEMDATA_AMOUNT, nAmountLimit, UPDATE_FALSE);
        }
        // gem hole
        if (::RandGet(CHANCE_MAKEHOLE) < 1)
        {
            if (GEM_NONE == pItem->GetInt(ITEMDATA_GEM1))
            {
                pItem->SetInt(ITEMDATA_GEM1, GEM_HOLE);
            }
            else if (GEM_NONE == pItem->GetInt(ITEMDATA_GEM2))
            {
                pItem->SetInt(ITEMDATA_GEM2, GEM_HOLE);
            }
        }
        // save to db
        pItem->SaveInfo();
        // inform client
        CMsgItemInfo msg;
        IF_OK (msg.Create(pItem, ITEMINFO_ADDITEM))
        this->SendMsg(&msg);
    }
    else
    {
        // failed
        // decrease durability
        int nAmountLimit = pItem->GetInt(ITEMDATA_AMOUNTLIMIT);
        if (::RandGet(100) < PERCENT_DECDUR)
        {
            if (nAmountLimit / 100 > 1)
            {
                nAmountLimit -= 100;
                pItem->SetInt(ITEMDATA_AMOUNTLIMIT, nAmountLimit);
            }
        }
        int nOrgAmountLimit = pItem->GetInt(ITEMDATA_AMOUNTLIMIT_ORIGINAL);
        nAmountLimit -= nOrgAmountLimit / 2;
        pItem->SetInt(ITEMDATA_AMOUNT, nAmountLimit);
        // save to db
        pItem->SaveInfo();
        // inform client
        CMsgItemInfo msg;
        IF_OK (msg.Create(pItem, ITEMINFO_UPDATE))
        this->SendMsg(&msg);
    }
}

/////////////////////////////////////////////////////////////////////////////
bool CUser::RecoverEquipmentDur(int nPos)
{
    // check
    CItem* pItem = this->GetEquipItemByPos(nPos);
    if (!pItem)
    {
        return false;
    }
    // take cost
    int nRecoverCost = pItem->GetRecoverDurCost();
    IF_NOT (nRecoverCost > 0)
    return false;
    if (!this->SpendMoney(nRecoverCost, SYNCHRO_TRUE))
    {
        return false;
    }
    // recover dur
    if (!pItem->RecoverDur())
    {
        return false;
    }
    // save to db
    pItem->SaveInfo();
    // inform client
    CMsgItemInfo msg;
    IF_OK (msg.Create(pItem, ITEMINFO_UPDATE))
    this->SendMsg(&msg);
    return true;
}

/////////////////////////////////////////////////////////////////////////////
void CUser::Mine(IRole* pRole)
{
    if (!this->IsAlive())
    {
        return;
    }
    if (m_tMine.IsActive())
    {
        return;
    }
    if (m_pTransformation)
    {
        return;
    }
    if (!m_pWeaponR || !m_pWeaponR->IsPick())
    {
        this->SendSysMsg(STR_MINE_WITH_PECKER);
        return;
    }
    CHECK (pRole);
    CNpc* pTarget = NULL;
    CHECK (pRole->QueryObj(OBJ_NPC, IPP_OF(pTarget)));
    if (GetDistance(pTarget->QueryMapThing()) > _MINE_RANGE)
    {
        return;
    }
    CRole::DetachStatus(this->QueryRole(), STATUS_HIDDEN);
    m_tMine.Startup(3000);
    m_pMineTarget	= pTarget;
    m_nMineCount = 0;
}

/////////////////////////////////////////////////////////////////////////////
void CUser::StopMine(void)
{
    m_tMine.Clear();
    m_pMineTarget = NULL;
    m_nMineCount = 0;
}

/////////////////////////////////////////////////////////////////////////////
void CUser::ProcessMineTimer(void)
{
    if (!this->IsAlive())
    {
        this->StopMine();
        return;
    }
    if (!m_pMineTarget || m_pMineTarget->GetType() != _ROLE_MINE_NPC)
    {
        this->StopMine();
        return;
    }
    // equipment check
    if (!m_pWeaponR || !m_pWeaponR->IsPick())
    {
        this->SendSysMsg(STR_NEED_PICK);
        this->StopMine();
        return;
    }
    // TODO: 以下公式需要修改
    enum { ORE_IRON = 1040000, ORE_COPPER = 1040010, ORE_SILVER = 1040020, ORE_GOLD = 1040030 };
    enum
    {
        NONE	= 0,
        IRON	= 1,
        COPPER	= 2,
        SILVER	= 3,
        GOLD	= 4,
        MAX_TYPE,
    };
    // 机会为万分之几？
    //	int nChance[4][4] = { 1000,	0000, 0000, 0017,
    //						  0250, 7500, 0000, 0017,
    //						  0000, 0250, 0750, 0017,
    //						  0000, 0000, 0000, 0000 };
    OBJID	setOreType[MAX_TYPE] =
    {
        ID_NONE,
        ORE_IRON,
        ORE_COPPER,
        ORE_SILVER,
        ORE_GOLD,
    };
    // factor
    int nFactor = (500 - __max(0, m_nMineCount - 100)) / 500;
    // ore item
    OBJID idType = ID_NONE;
    // init rand
    ::RandGet(100, true);
    // ....
    int nRand = (10000 - 1) -::RandGet(10000);
    bool bStopMine = false;
    for (int i = 0; i < 4; i++)
    {
        DWORD dwData = m_pMineTarget->GetInt((NPCDATA)(NPCDATA_DATA0 + i));
        if (dwData == 0)
        {
            continue;
        }
        int nType = dwData / 1000000;				// 百万位表示类型
        IF_NOT (nType > 0 && nType < MAX_TYPE)		// 校验类型的有效性
        {
            LOGERROR("Invalid NPC data value [NPC ID=%u, data%d=%u]", m_pMineTarget->GetID(), i, dwData);
            continue;
        }
        int nMinQuality = (dwData / 100000) % 10;	// 十万位表示最低品质
        int nChance = dwData % 100000;				// 个位~万位表示概率
        nChance *= nFactor;			// 根据挖掘次数调整概率
        if (nRand < nChance)
        {
            OBJID idType = setOreType[nType];
            idType += __min(9, (nMinQuality + ::RandGet(10 - nMinQuality)));
            if (!this->AwardItem(idType, SYNCHRO_TRUE))
            {
                POINT	pos;
                pos.x	= this->GetPosX();
                pos.y	= this->GetPosY();
                const int DROP_MINE_RANGE	= 1;
                if (this->GetMap() && this->GetMap()->FindDropItemCell(DROP_MINE_RANGE, &pos))
                {
                    CMapItem* pMapItem = CMapItem::CreateNew();
                    if (pMapItem)
                    {
                        IF_OK(pMapItem->Create(MapManager()->SpawnMapItemID(), GetMap(), pos, idType, this->GetID()))
                        {
                            MapManager()->QueryMapItemSet()->AddObj(pMapItem);
                        }
                        else
                        {
                            pMapItem->ReleaseByOwner();
                        }
                    }
                }
                else
                {
                    bStopMine = true;
                }
            }
            switch (nType)
            {
            case IRON:
                this->SendSysMsg(STR_IRON_ORE);
                break;
            case COPPER:
                this->SendSysMsg(STR_COPPER_ORE);
                break;
            case SILVER:
                this->SendSysMsg(STR_SILVER_ORE);
                break;
            case GOLD:
                this->SendSysMsg(STR_GOLD_ORE);
                break;
            }
            break;
        }
    }
    // inc mine count
    m_nMineCount++;
    // broadcast mine msg
    CMsgInteract msg;
    IF_OK (msg.Create(INTERACT_MINE, this->GetID(), m_pMineTarget->GetID(), this->GetPosX(), this->GetPosY(), this->GetDir()))
    this->BroadcastRoomMsg(&msg, INCLUDE_SELF);
    if (bStopMine)
    {
        StopMine();
    }
}

/////////////////////////////////////////////////////////////////////////////
bool CUser::MultiDelItem(OBJID idTypeFirst, OBJID idTypeLast, int nNum)
{
    int nItems	= m_pPackage->GetAmount();
    if (nItems > 0)
    {
        int nCount = 0;
        OBJID* id = new OBJID[nItems];
        nCount = m_pPackage->MultiGetItem(idTypeFirst, idTypeLast, nNum, id, nItems);
        if (nCount >= nNum)
        {
            for (int i = 0; i < nCount; i++)
            {
                this->EraseItem(id[i], SYNCHRO_TRUE);
            }
            delete [] id;
            return true;
        }
        else
        {
            delete [] id;
        }
    }
    return false;
}

/////////////////////////////////////////////////////////////////////////////
bool CUser::MultiCheckItem(OBJID idTypeFirst, OBJID idTypeLast, int nNum)
{
    return m_pPackage->MultiCheckItem(idTypeFirst, idTypeLast, nNum);
}

/////////////////////////////////////////////////////////////////////////////
int CUser::CountCard(void)
{
    SQLBUF	szSQL;
    sprintf(szSQL, "SELECT id FROM %s WHERE account_id=%u", _TBL_CARD, this->GetAccountID());
    return __max(0, Database()->CountRecord(szSQL));
}

/////////////////////////////////////////////////////////////////////////////
bool CUser::CheckInCard(void)
{
    // search card item
    CItem* pItem = this->GetItemByType(CARD_FEE_POINT);
    if (!pItem)
    {
        return false;
    }
    // delete card item first
    OBJID idCard = pItem->GetID();
    IF_NOT (this->EraseItem(idCard, SYNCHRO_TRUE))
    return false;
    // log
    ::MyLogSave("gmlog/card_item", "[%s][id=%u] delete card item[id=%u]", this->GetName(), this->GetID(), idCard);
    // database check
    IDatabase* pDb = Database();
    IF_NOT (pDb)
    {
        ::MyLogSave("gmlog/card_item", "Error: [%s][id=%u] db failed.", this->GetName(), this->GetID());
        return false;
    }
    // exe sql
    SQLBUF	szSQL;
    sprintf(szSQL, "INSERT INTO %s SET type=%u,account_id=%u", _TBL_CARD, CARD_FEE_POINT, this->GetAccountID());
    IF_NOT (pDb->ExecuteSQL(szSQL))
    {
        ::MyLogSave("gmlog/card_item", "Error: [%s][id=%u] exe insert sql[%s] failed.", this->GetName(), this->GetID(), szSQL);
        return false;
    }
    // log
    ::MyLogSave("gmlog/card_item", "[%s][id=%u] suc insert card record[id=%u]",
                this->GetName(),
                this->GetID(),
                pDb->GetInsertId());
    return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CUser::CheckOutCard(void)
{
    // check item pack first
    //if (m_setItem.size() >= _MAX_USERITEMSIZE)
    if (m_pPackage->IsPackFull(ITEMPOSITION_BACKPACK))
    {
        this->SendSysMsg(STR_ITEMBAG_FULL);
        return false;
    }
    // database check
    IDatabase* pDb = Database();
    IF_NOT (pDb)
    {
        ::MyLogSave("gmlog/card_item", "Error: [%s][id=%u] db failed.", this->GetName(), this->GetID());
        return false;
    }
    // count card
    SQLBUF	szSQL;
    sprintf(szSQL, "SELECT id FROM %s WHERE account_id=%u", _TBL_CARD, this->GetAccountID());
    int nCards = pDb->CountRecord(szSQL);
    if (-1 == nCards)
    {
        ::MyLogSave("gmlog/card_item", "Error: [%s][id=%u] db failed when count.", this->GetName(), this->GetID());
        return false;
    }
    else if (0 == nCards)
    {
        ::MyLogSave("gmlog/card_item", "Error: [%s][id=%u] no card exist.", this->GetName(), this->GetID());
        return false;
    }
    // delete a card record
    sprintf(szSQL, "DELETE FROM %s WHERE account_id=%u LIMIT 1", _TBL_CARD, this->GetAccountID());
    IF_NOT (pDb->ExecuteSQL(szSQL))
    {
        ::MyLogSave("gmlog/card_item", "Error: [%s][id=%u] exe delete sql[%s] failed.", this->GetName(), this->GetID(), szSQL);
        return false;
    }
    // log
    ::MyLogSave("gmlog/card_item", "[%s][id=%u] delete a card record.", this->GetName(), this->GetID());
    // award a card item
    CItem* pItem = this->AwardItem(CARD_FEE_POINT, SYNCHRO_TRUE, IDENT_OK, NO_COMBINE);
    if (!pItem)
    {
        ::MyLogSave("gmlog/card_item", "Error: [%s][id=%u] award card item [%u] failed.", this->GetName(), this->GetID(), CARD_FEE_POINT);
        return false;
    }
    // log
    ::MyLogSave("gmlog/card_item", "[%s][id=%u] award card item [%u] [id=%u] succed.",
                this->GetName(),
                this->GetID(),
                CARD_FEE_POINT,
                pItem->GetID());
    return true;
}

/////////////////////////////////////////////////////////////////////////////
int CUser::CountCard2(void)
{
    SQLBUF	szSQL;
    sprintf(szSQL, "SELECT id FROM %s WHERE account_id=%u", _TBL_CARD2, this->GetAccountID());
    return __max(0, Database()->CountRecord(szSQL));
}

/////////////////////////////////////////////////////////////////////////////
bool CUser::CheckInCard2(void)
{
    // search card item
    CItem* pItem = this->GetItemByType(CARD_FEE_POINT2);
    if (!pItem)
    {
        return false;
    }
    // delete card item first
    OBJID idCard = pItem->GetID();
    IF_NOT (this->EraseItem(idCard, SYNCHRO_TRUE))
    return false;
    // log
    ::MyLogSave("gmlog/card2_item", "[%s][id=%u] delete card item[id=%u]", this->GetName(), this->GetID(), idCard);
    // database check
    IDatabase* pDb = Database();
    IF_NOT (pDb)
    {
        ::MyLogSave("gmlog/card2_item", "Error: [%s][id=%u] db failed.", this->GetName(), this->GetID());
        return false;
    }
    // exe sql
    SQLBUF	szSQL;
    sprintf(szSQL, "INSERT INTO %s SET type=%u,account_id=%u", _TBL_CARD2, CARD_FEE_POINT2, this->GetAccountID());
    IF_NOT (pDb->ExecuteSQL(szSQL))
    {
        ::MyLogSave("gmlog/card2_item", "Error: [%s][id=%u] exe insert sql[%s] failed.", this->GetName(), this->GetID(), szSQL);
        return false;
    }
    // log
    ::MyLogSave("gmlog/card2_item", "[%s][id=%u] suc insert card record[id=%u]",
                this->GetName(),
                this->GetID(),
                pDb->GetInsertId());
    return true;
}

/////////////////////////////////////////////////////////////////////////////
bool CUser::CheckOutCard2(void)
{
    // check item pack first
    //if (m_setItem.size() >= _MAX_USERITEMSIZE)
    if (m_pPackage->IsPackFull(ITEMPOSITION_BACKPACK))
    {
        this->SendSysMsg(STR_ITEMBAG_FULL);
        return false;
    }
    // database check
    IDatabase* pDb = Database();
    IF_NOT (pDb)
    {
        ::MyLogSave("gmlog/card2_item", "Error: [%s][id=%u] db failed.", this->GetName(), this->GetID());
        return false;
    }
    // count card
    SQLBUF	szSQL;
    sprintf(szSQL, "SELECT id FROM %s WHERE account_id=%u", _TBL_CARD2, this->GetAccountID());
    int nCards = pDb->CountRecord(szSQL);
    if (-1 == nCards)
    {
        ::MyLogSave("gmlog/card2_item", "Error: [%s][id=%u] db failed when count.", this->GetName(), this->GetID());
        return false;
    }
    else if (0 == nCards)
    {
        ::MyLogSave("gmlog/card2_item", "Error: [%s][id=%u] no card exist.", this->GetName(), this->GetID());
        return false;
    }
    // delete a card record
    sprintf(szSQL, "DELETE FROM %s WHERE account_id=%u LIMIT 1", _TBL_CARD2, this->GetAccountID());
    IF_NOT (pDb->ExecuteSQL(szSQL))
    {
        ::MyLogSave("gmlog/card2_item", "Error: [%s][id=%u] exe delete sql[%s] failed.", this->GetName(), this->GetID(), szSQL);
        return false;
    }
    // log
    ::MyLogSave("gmlog/card2_item", "[%s][id=%u] delete a card record.", this->GetName(), this->GetID());
    // award a card item
    CItem* pItem = this->AwardItem(CARD_FEE_POINT2, SYNCHRO_TRUE, IDENT_OK, NO_COMBINE);
    if (!pItem)
    {
        ::MyLogSave("gmlog/card2_item", "Error: [%s][id=%u] award card item [%u] failed.", this->GetName(), this->GetID(), CARD_FEE_POINT2);
        return false;
    }
    // log
    ::MyLogSave("gmlog/card2_item", "[%s][id=%u] award card item [%u] [id=%u] succed.",
                this->GetName(),
                this->GetID(),
                CARD_FEE_POINT2,
                pItem->GetID());
    return true;
}
