
//**********************************************************
// 代码编辑器
//**********************************************************

#include "basefunc.H"
#include "hero.H"
#include "gamemsg.H"
#include "allmsg.H"
#include "GamePlayerSet.h"
#include "Shop.h"
#include "3DGameMap.h"
#include "GameDataSet.h"

//--------------------------------------------------------------
CItem* CHero::GetItem(OBJID id, BOOL bExcludeEquipment)
{
    if (m_pPackageNormal && m_pPackageNormal->QueryItemByID(id))
    {
        return (CItem*)m_pPackageNormal->QueryItemByID(id);
    }
    if (m_pPackageEgg && m_pPackageEgg->QueryItemByID(id))
    {
        return (CItem*)m_pPackageEgg->QueryItemByID(id);
    }
    if (m_pPackagePet && m_pPackagePet->QueryItemByID(id))
    {
        return (CItem*)m_pPackagePet->QueryItemByID(id);
    }
    if (m_pPackageSoul && m_pPackageSoul->QueryItemByID(id))
    {
        return (CItem*)m_pPackageSoul->QueryItemByID(id);
    }
    if (!bExcludeEquipment)
    {
        for (int i = 0; i < _MAX_EQUIPMENT; i++)
        {
            if (m_pEquipment[i] && m_pEquipment[i]->GetID() == id)
            {
                return m_pEquipment[i];
            }
        }
    }
    return NULL;
}
//--------------------------------------------------------------
int	CHero::GetItemAmount(int nPackageType)
{
    switch(nPackageType)
    {
    case PACKAGE_BAG:
        return this->QueryPackageBag()->GetItemAmount();
    case PACKAGE_PET:
        return this->QueryPackagePet()->GetItemAmount();
    case PACKAGE_EGG:
        return this->QueryPackageEgg()->GetItemAmount();
    case PACKAGE_SOUL:
        return this->QueryPackageSoul()->GetItemAmount();
    default:
        return 0;
    }
}
//--------------------------------------------------------------
BOOL CHero::AddItem(CItem* pItem)
{
    int nPackageType = pItem->GetPackageType();
    switch(nPackageType)
    {
    case PACKAGE_BAG:
        return this->QueryPackageBag()->AddItem(pItem);
    case PACKAGE_PET:
        return this->QueryPackagePet()->AddItem(pItem);
    case PACKAGE_EGG:
        return this->QueryPackageEgg()->AddItem(pItem);
    case PACKAGE_SOUL:
        return this->QueryPackageSoul()->AddItem(pItem);
    default:
        return false;
    }
}
//--------------------------------------------------------------
CItem* CHero::GetItemByIndex(UINT uIndex, int nPackageType)
{
    switch(nPackageType)
    {
    case PACKAGE_BAG:
        return (CItem*)this->QueryPackageBag()->QueryItemByIndex(uIndex);
    case PACKAGE_PET:
        return (CItem*)this->QueryPackagePet()->QueryItemByIndex(uIndex);
    case PACKAGE_EGG:
        return (CItem*)this->QueryPackageEgg()->QueryItemByIndex(uIndex);
    case PACKAGE_SOUL:
        return (CItem*)this->QueryPackageSoul()->QueryItemByIndex(uIndex);
    default:
        return NULL;
    }
}

//--------------------------------------------------------------

BOOL CHero::DelItem(OBJID id, BOOL bDelete)
{
    if (this->QueryPackageBag()->DeleteItemByID(id, bDelete))
    {
        return true;
    }
    if (this->QueryPackagePet()->DeleteItemByID(id, bDelete))
    {
        return true;
    }
    if (this->QueryPackageEgg()->DeleteItemByID(id, bDelete))
    {
        return true;
    }
    if (this->QueryPackageSoul()->DeleteItemByID(id, bDelete))
    {
        return true;
    }
    int nItemCount = g_objHero.m_objBoothManager.GetItemAmount();
    for(int i = 0; i < nItemCount; i ++)
    {
        CItem* pItem = g_objHero.m_objBoothManager.GetItemByIndex(i);
        if (pItem && pItem->GetID() == id)
        {
            if (bDelete)
            {
                SAFE_DELETE(pItem);
            }
            g_objHero.m_objBoothManager.m_dequeItem.erase(g_objHero.m_objBoothManager.m_dequeItem.begin() + i);
        }
    }
    return false;
}

//--------------------------------------------------------------
void CHero::ClearItem(void)
{
    // item in backpack
    this->QueryPackageBag()->Clear(true);
    this->QueryPackagePet()->Clear(true);
    this->QueryPackageEgg()->Clear(true);
    this->QueryPackageSoul()->Clear(true);
    // equipment
    for (int i = 0; i < _MAX_EQUIPMENT; i++)
    {
        SAFE_DELETE(m_pEquipment[i]);
    }
    // auction item
    SAFE_DELETE(m_AuctionItem);
}


//--------------------------------------------------------------
CItem* CHero::SearchItem(OBJID id)
{
    /*	if (m_pItemWeapon && m_pItemWeapon->GetID() == id)
    		return m_pItemWeapon;

    	if (m_pItemArmor && m_pItemArmor->GetID() == id)
    		return m_pItemArmor;

    	if (m_pItemTreasure && m_pItemTreasure->GetID() == id)
    		return m_pItemTreasure;

    	return this->GetItem(id);*/
    return NULL;
}

//--------------------------------------------------------------
BOOL CHero::DropItem(OBJID id, int x, int y)
{
    if (g_objGameMap.GetType() & MAPTYPE_FAMILY)
    {
        OBJID idCanNotDrop = 100146;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idCanNotDrop));
        return false;
    }
    if (!this->GetItem(id))
    {
        OBJID idItemError = 100044;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idItemError));
        return false;
    }
    CMyPos posCell;
    g_objGameMap.Screen2Cell(x, y, posCell.x, posCell.y);
    CMyPos posHero;
    g_objHero.GetPos(posHero);
    int nDir = g_objGameMap.GetDirection(posHero.x, posHero.y, posCell.x, posCell.y);
    posCell.x = posHero.x + _DELTA_X[nDir];
    posCell.y = posHero.y + _DELTA_Y[nDir];
    CellInfo* pInfo = g_objGameMap.GetCell(posCell.x, posCell.y);
    OBJID idItem = g_objGameMap.Get3DMapItem(posCell);
    if (!pInfo || g_objGameMap.Get3DMapItem(posCell) || g_objGameMap.GetGroundMask(pInfo) != 0)
    {
        // 八格随机方向
        int nX[8] = {1, -1, 0, 1, 1, -1, -1, 0};
        int nY[8] = {1, -1, 1, 0, -1, 1, 0, -1};
        int i = 0;
        for(; i < 8; i ++)
        {
            CMyPos posItem;
            posItem.x = posHero.x + nX[i];
            posItem.y = posHero.y + nY[i];
            CellInfo* pInfo = g_objGameMap.GetCell(posItem.x, posItem.y);
            if (pInfo)
            {
                if (!g_objGameMap.Get3DMapItem(posItem) && g_objGameMap.GetGroundMask(pInfo) == 0 && !g_objPlayerSet.GetPlayer(posItem.x, posItem.y))
                {
                    posCell = posItem;
                    break;
                }
            }
        }
        if (i >= 8)
        {
            return false;
        }
    }
    /*	// Play the sound
    	CItem *pItem = g_objHero.GetItem( id ) ;
    	if ( pItem != NULL )
    	{
    		switch ( pItem->GetSort() )
    		{
    		case ITEMSORT_WEAPON_SINGLE_HAND:
    		case ITEMSORT_WEAPON_DOUBLE_HAND:
    			DXPlaySound( "Sound/Dropweapon.wav" ) ;
    			break ;
    		case ITEMSORT_HELMET:
    		case ITEMSORT_ARMOR:
    		case ITEMSORT_SHIELD:
    		case ITEMSORT_SHOES:
    			DXPlaySound( "Sound/Droploricae.wav" ) ;
    			break ;
    		case ITEMSORT_NECKLACE:
    		case ITEMSORT_RING:
    			DXPlaySound( "Sound/Dropaccouterment.wav" ) ;
    			break ;
    		default:
    			DXPlaySound( "Sound/Dropdrug.wav" ) ;
    			break ;
    		}
    	}
    	*/
    CMsgItem msg;
    if (msg.Create(id, posCell.x, posCell.y, ITEMACT_DROP))
    {
        msg.Send();
    }
    return true;
}

//--------------------------------------------------------------
void CHero::PickUpItem(OBJID id, int x, int y)
{
    if (this->IsMagicLock())
    {
        return;
    }
    if (id == ID_NONE)
    {
        return;
    }
    if (m_objBoothManager.IsActive())
    {
        return;
    }
    // here got player already
    CPlayer* pPlayer = g_objPlayerSet.GetPlayer(x, y);
    if (pPlayer && !pPlayer->IsDead())
    {
        return;
    }
    CCommand cmd;
    cmd.iType		= _COMMAND_PICKUP;
    cmd.iStatus		= _CMDSTATUS_BEGIN;
    cmd.idTarget	= id;
    cmd.posTarget.x = x;
    cmd.posTarget.y = y;
    this->SetCommand(&cmd);
}

//--------------------------------------------------------------
void CHero::BuyItem(OBJID idType)
{
    int nSubType = CItem::ItemGetSubType(idType);
    if ((nSubType / 10000) * 10000 == ITEMTYPE_GHOSTGEM)
    {
        bool bCanBuy = FALSE;
        if (g_objHero.GetItemAmount (PACKAGE_SOUL) < g_objHero.GetPackageLimit (PACKAGE_SOUL))
        {
            bCanBuy = TRUE;
        }
        else
        {
            int nAmount = g_objHero.GetItemAmount(PACKAGE_SOUL);
            for(int i = 0 ; i < nAmount ; i++)
            {
                CItem* pOldItem = g_objHero.GetItemByIndex(i, PACKAGE_SOUL);
                if (pOldItem && pOldItem->GetTypeID() == idType
                        && pOldItem->GetAmount() < pOldItem->GetAmountLimit())
                {
                    bCanBuy = TRUE;
                    break;
                }
            }
        }
        if (!bCanBuy)
        {
            OBJID idPackageFull = 100200;		//100200=宝石背包已经满了，不能再购买了
            g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idPackageFull));
            return;
        }
    }
    else if ((nSubType / 10000 ) * 10000 == ITEMTYPE_EUDEMON)
    {
        bool bCanBuy = FALSE;
        if (g_objHero.GetItemAmount (PACKAGE_PET) < g_objHero.GetPackageLimit (PACKAGE_PET))
        {
            bCanBuy = TRUE;
        }
        else
        {
            g_objGameMsg.AddMsg ( (char*)g_objGameDataSet.GetStr(10698) ) ;
            return;
        }
    }
    else if ((nSubType / 10000) * 10000 == ITEMTYPE_EUDEMONEGG)
    {
        bool bCanBuy = FALSE;
        if (g_objHero.GetItemAmount (PACKAGE_EGG) < g_objHero.GetPackageLimit (PACKAGE_EGG))
        {
            bCanBuy = TRUE;
        }
        else
        {
            g_objGameMsg.AddMsg ( (char*)g_objGameDataSet.GetStr(10699) ) ;
            return;
        }
    }
    else
    {
        bool bCanBuy = FALSE;
        if (g_objHero.GetItemAmount (PACKAGE_BAG) < g_objHero.GetPackageLimit (PACKAGE_BAG))
        {
            bCanBuy = TRUE;
        }
        else
        {
            int nAmount = g_objHero.GetItemAmount(PACKAGE_BAG);
            for(int i = 0 ; i < nAmount ; i++)
            {
                CItem* pOldItem = g_objHero.GetItemByIndex(i, PACKAGE_BAG);
                if (pOldItem && pOldItem->GetTypeID() == idType
                        && pOldItem->GetAmount() < pOldItem->GetAmountLimit())
                {
                    bCanBuy = TRUE;
                    break;
                }
            }
        }
        if (!bCanBuy)
        {
            OBJID idPackageFull = 100045 ;		//=错误：物品栏已满。
            g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idPackageFull));
            return;
        }
    }
    CMsgItem msg;
    if (msg.Create(g_objShop.GetShopID(), idType, ITEMACT_BUY))
    {
        msg.Send();
    }
}

//--------------------------------------------------------------
void CHero::SellItem(OBJID id)
{
    if (!this->GetItem(id))
    {
        OBJID idItemError = 100044;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idItemError), _TXTATR_SHOP);
        return;
    }
    CMsgItem msg;
    if (msg.Create(g_objShop.GetShopID(), id, ITEMACT_SELL))
    {
        msg.Send();
    }
}

//--------------------------------------------------------------
void CHero::RepairItem(OBJID id)
{
    if (id == ID_NONE)
    {
        return;
    }
    CMsgItem msg;
    if (msg.Create(id, 0, ITEMACT_REPAIR))
    {
        msg.Send();
    }
}

//--------------------------------------------------------------
void CHero::UseItem(OBJID id, OBJID idTarget)
{
    if (this->IsDead())
    {
        return;
    }
    CItem* pItem = this->GetItem(id);
    if (!pItem)
    {
        OBJID idItemError = 100044;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idItemError));
        return;
    }
    if (!this->TryItem(pItem))
    {
        return;
    }
    BOOL bIsMed = false;
    if ( pItem->GetSort() == ITEMSORT_EXPEND && ( pItem->GetLife() > 0 || pItem->GetMana() > 0 ))
    {
        bIsMed = true;
    }
    if (this->TestStatus(USERSTATUS_CANNOTMOVE) && !bIsMed)
    {
        return;
    }
    int nPosition = 0;
    if (pItem->GetTypeID() == _ARROW && this->GetRWeapon() / 1000 % 1000 != 500)
    {
        return;
    }
    else if (pItem->GetTypeID() == _ARROW && this->GetRWeapon() / 1000 % 1000 == 500)
    {
        nPosition = ITEMPOSITION_WEAPONL;
        CMsgItem msg;
        if (msg.CreateForUseItem(id, nPosition, ITEMACT_USE, idTarget))
        {
            msg.Send();
        }
        return;
    }
    if (pItem->GetTypeID() == _DART && this->GetRWeapon() / 1000 % 1000 != 501)
    {
        return;
    }
    else if (pItem->GetTypeID() == _DART && this->GetRWeapon() / 1000 % 1000 == 501)
    {
        nPosition = ITEMPOSITION_WEAPONL;
        CMsgItem msg;
        if (msg.CreateForUseItem(id, nPosition, ITEMACT_USE, idTarget))
        {
            msg.Send();
        }
        return;
    }
    switch(pItem->GetSort())
    {
    case ITEMSORT_EXPEND:
        if (!this->TryItemTarget(pItem, idTarget))
        {
            g_objGameMsg.AddMsg((char*)g_objGameDataSet.GetStr(100060));	//错误对象!
            return;
        }
        if (pItem->GetLife() > 0)
        {
            m_objCheck.AddLife();
            int n = this->GetMaxLife();
            int k = this->GetLifeForeCast();
            int j = pItem->GetLife();
            this->SetLifeForeCast(this->GetLifeForeCast() + pItem->GetLife());
            int l = this->GetLifeForeCast();
        }
        else if (pItem->GetMana() > 0)
        {
            m_objCheck.AddMana();
        }
        break;
    case ITEMSORT_HELMET:
        nPosition = ITEMPOSITION_HELMET;
        break;
    case ITEMSORT_NECKLACE:
        nPosition = ITEMPOSITION_NECKLACE;
        break;
    case ITEMSORT_ARMOR:
        nPosition = ITEMPOSITION_ARMOR;
        break;
    case ITEMSORT_WEAPON_SINGLE_HAND:
        nPosition = ITEMPOSITION_WEAPONR;
        break;
    case ITEMSORT_WEAPON_DOUBLE_HAND:
        nPosition = ITEMPOSITION_WEAPONR;
        break;
    case ITEMSORT_TREASURE:
        nPosition = ITEMPOSITION_TREASURE;
        break;
    case ITEMSORT_SHIELD:
        {
            CItem* pItem = this->GetEquipment(ITEMPOSITION_WEAPONR);
            if (!pItem)
            {
                return;
            }
            if (pItem && ITEMSORT_WEAPON_DOUBLE_HAND == pItem->GetSort())
            {
                const OBJID idEqumipError = 100048;
                g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idEqumipError));
                return;
            }
        }
        nPosition = ITEMPOSITION_WEAPONL;
        break;
    case ITEMSORT_RING:
        nPosition = ITEMPOSITION_RINGR;
        break;
    case ITEMSORT_SHOES:
        nPosition = ITEMPOSITION_SHOES;
        break;
    case ITEMSORT_MOUNT:
        nPosition = ITEMPOSITION_MOUNT;
        break;
    default:
        break;
    }
    /*	if (nPosition != 0)
    	{
    		CCommand cmd;
    		cmd.iType	=_COMMAND_STANDBY;
    		cmd.iStatus	=_CMDSTATUS_BEGIN;

    		g_objHero.SetCommand(&cmd);
    		g_objHero.ResetActionData();
    	}*/
    CMsgItem msg;
    if (msg.CreateForUseItem(id, nPosition, ITEMACT_USE, idTarget))
    {
        msg.Send();
    }
}
//--------------------------------------------------------------
BOOL CHero::TryItemTarget(CItem* pItem, OBJID idTarget)
{
    if (!pItem)
    {
        return false;
    }
    if (pItem->TestTarget(TARGET_NONE))	// 不限定条件
    {
        return true;
    }
    if (pItem->TestTarget(TARGET_FORBIDDEN))	// 禁止对任何目标使用
    {
        return false;
    }
    CPlayer* pPlayer = g_objPlayerSet.GetPlayer(idTarget, true);
    if (!pPlayer)
    {
        return false;
    }
    else
    {
        if (pItem->TestTarget(TARGET_SELF)) //对自己使用
        {
            if (pPlayer->GetID() != g_objHero.GetID()
                    && !pPlayer->IsMyPet())
            {
                return false;
            }
        }
        if (pItem->TestTarget(TARGET_OTHERS)) //对他人使用
        {
            if (pPlayer->GetID() == g_objHero.GetID()
                    || pPlayer->IsMyPet())
            {
                return false;
            }
        }
        if (pItem->TestTarget(TARGET_BODY)) //对尸体使用
        {
            if (!pPlayer->IsDead())
            {
                return false;
            }
        }
        if (pItem->TestTarget(TARGET_CHK_PKMODE))	//PK模式检查
        {
            if (pPlayer->GetID() == g_objHero.GetID()
                    && pPlayer->IsMyPet())
            {
                return false;
            }
            if (g_objHero.GetPkMode() == PKMODE_MONSTERONLY)  //安全
            {
                if (pPlayer->IsPlayer() || pPlayer->IsPet())
                {
                    return false;
                }
            }
            else if (g_objHero.GetPkMode() == PKMODE_TMEXCLUDE) //组队
            {
                if (pPlayer->IsPlayer())
                {
                    if (g_objHero.GetTeamMemberInfoByID(pPlayer->GetID()))
                    {
                        return false;
                    }
                    if (g_objHero.GetFriend(pPlayer->GetID()))
                    {
                        return false;
                    }
                    if (strcmp(pPlayer->GetName(), g_objHero.GetMate()) == 0)
                    {
                        return false;
                    }
                    if (pPlayer->GetSydicateID() == g_objHero.GetSydicateID())
                    {
                        return false;
                    }
                }
                else if (pPlayer->IsPet())
                {
                    if (g_objHero.GetTeamMemberInfoByID(pPlayer->GetPetOwnerId()))
                    {
                        return false;
                    }
                    if (g_objHero.GetFriend(pPlayer->GetPetOwnerId()))
                    {
                        return false;
                    }
                    CPlayer* pOwnerPlayer = g_objPlayerSet.GetPlayer(pPlayer->GetPetOwnerId());
                    if (pOwnerPlayer && pOwnerPlayer->GetSydicateID() == g_objHero.GetSydicateID())
                    {
                        return false;
                    }
                    if (pOwnerPlayer && strcmp(pPlayer->GetName(), g_objHero.GetMate()) == 0)
                    {
                        return false;
                    }
                }
            }
            else if (g_objHero.GetPkMode() == PKMODE_ARRESTMENT) //捕杀
            {
                if (pPlayer->IsPlayer())
                {
                    if (!pPlayer->TestStatus(USERSTATUS_CRIME)
                            && !pPlayer->TestStatus(USERSTATUS_DARKRED))
                    {
                        return false;
                    }
                }
                else if (pPlayer->IsPet())
                {
                    CPlayer* pOwnerPlayer = g_objPlayerSet.GetPlayer(pPlayer->GetPetOwnerId());
                    if (pOwnerPlayer
                            && !pOwnerPlayer->TestStatus(USERSTATUS_CRIME)
                            && !pOwnerPlayer->TestStatus(USERSTATUS_DARKRED))
                    {
                        return false;
                    }
                }
            }
            //			else if (g_objHero.GetPkMode() == PKMODE_FREE) //自由pk 无限制
        }
        //目标类型检测
        if (!pItem->TestTarget(TARGET_USER)		//不可以用于玩家
                && pPlayer->IsPlayer())
        {
            return false;
        }
        if (!pItem->TestTarget(TARGET_MONSTER)  //不可以用于怪物
                && pPlayer->IsMonster())
        {
            return false;
        }
        if (!pItem->TestTarget(TARGET_EUDEMON)  //不可以用于幻兽
                && pPlayer->IsPet())
        {
            return false;
        }
    }
    return true;
}

//--------------------------------------------------------------
BOOL CHero::TryItem(CItem* pItem)
{
    if (!pItem)
    {
        return false;
    }
    // task item
    if (pItem->IsTaskItem())
    {
        return false;
    }
    // level check
    if (pItem->GetLevelRequired() != 0 &&
            this->GetLev() < pItem->GetLevelRequired())
    {
        return false;
    }
    // attribute check
    if (pItem->GetForceRequired() != 0 &&
            this->GetForce() < pItem->GetForceRequired())
    {
        return false;
    }
    if (pItem->GetSpeedRequired() != 0 &&
            this->GetSpeed() < pItem->GetSpeedRequired())
    {
        return false;
    }
    if (pItem->GetHealthRequired() != 0 &&
            this->GetHealth() < pItem->GetHealthRequired())
    {
        return false;
    }
    if (pItem->GetSoulRequired() != 0 &&
            this->GetOriginalSoul() < pItem->GetSoulRequired())
    {
        return false;
    }
    if (pItem->GetSexRequried() == SEX_MALE || pItem->GetSexRequried() == SEX_FEMALE)
        if (this->GetSex() != pItem->GetSexRequried())
        {
            return false;
        }
    if (pItem->TestStatus(_ITEM_STATUS_NOT_IDENT))
    {
        return false;
    }
    // profession check
    int nReqProfession = pItem->GetProfessionRequired ();
    int nProfession = this->GetProfession();
    if ( nReqProfession != 0 )
    {
        int nRequireProfSort	= nReqProfession / 10;
        int nRequireProfLevel	= nReqProfession % 10;
        if (nRequireProfSort != (nProfession % 1000) / 10)
        {
            return false;
        }
        if (nProfession % 10 < nRequireProfLevel)
        {
            return false;
        }
    }
    return true;
}
//--------------------------------------------------------------
BOOL	CHero::TryItem(OBJID idItem, int nPosition)
{
    CItem* pItem = g_objHero.GetItem(idItem);
    if (!pItem)
    {
        return false;
    }
    if (!this->TryItem(pItem))
    {
        return false;
    }
    if (pItem->GetTypeID() == _ARROW)
    {
        if (this->GetRWeapon() / 1000 % 1000 == 500 && nPosition == ITEMPOSITION_WEAPONL)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    if (pItem->GetTypeID() == _DART)
    {
        if (this->GetRWeapon() / 1000 % 1000 == 501 && nPosition == ITEMPOSITION_WEAPONL)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    switch(pItem->GetSort())
    {
    case ITEMSORT_EXPEND:
        return false;
        break;
    case ITEMSORT_TREASURE:
        return (nPosition == ITEMPOSITION_TREASURE);
    case ITEMSORT_HELMET:
        return (nPosition == ITEMPOSITION_HELMET);
    case ITEMSORT_NECKLACE:
        return (nPosition == ITEMPOSITION_NECKLACE);
    case ITEMSORT_ARMOR:
        return (nPosition == ITEMPOSITION_ARMOR);
    case ITEMSORT_WEAPON_SINGLE_HAND:
        return (nPosition == ITEMPOSITION_WEAPONR);
    case ITEMSORT_WEAPON_DOUBLE_HAND:
        {
            if (this->GetEquipment(ITEMPOSITION_WEAPONL))
            {
                return false;
            }
            else
            {
                return true;
            }
        }
    case ITEMSORT_SHIELD:
        {
            CItem* pItem = this->GetEquipment(ITEMPOSITION_WEAPONR);
            if (!pItem)
            {
                return false;
            }
            if (pItem && ITEMSORT_WEAPON_DOUBLE_HAND == pItem->GetSort())
            {
                return false;
            }
            else
            {
                return true;
            }
        }
    case ITEMSORT_RING:
        return (nPosition == ITEMPOSITION_RINGR);
    case ITEMSORT_SHOES:
        return (nPosition == ITEMPOSITION_SHOES);
    default:
        return false;
    }
}
//--------------------------------------------------------------
void CHero::SetEquipment (int nPosition, CItem* pItem)
{
    //if (!pItem)
    //	return;
    MYASSERT(nPosition - 1 >= 0 && nPosition - 1 < _MAX_EQUIPMENT);
    int nType = nPosition - 1;
    switch(nPosition)
    {
    case ITEMPOSITION_HELMET:
        m_pEquipment[nType] = pItem;
        /*
        		if (pItem)
        			this->SetArmet(pItem->GetTypeID());
        		else
        			this->SetArmet(ID_NONE);
        */
        break;
    case ITEMPOSITION_NECKLACE:
        m_pEquipment[nType] = pItem;
        break;
    case ITEMPOSITION_ARMOR:
        m_pEquipment[nType] = pItem;
        if (pItem)
        {
            this->SetArmor(pItem->GetTypeID());
        }
        else
        {
            this->SetArmor(ID_NONE);
        }
        break;
    case ITEMPOSITION_WEAPONR:
        m_pEquipment[nType] = pItem;
        if (pItem)
        {
            this->SetRWeapon(pItem->GetTypeID());
            /*if (pItem->GetSort() == ITEMSORT_WEAPON_DOUBLE_HAND || ITEMSORT_WEAPON_SINGLE_HAND == pItem->GetSort())
            {
            	CItem* pLItem = g_objHero.GetEquipment(ITEMPOSITION_WEAPONL);
            	if (pItem->IsBow() && pLItem && pLItem->IsArrow())
            	{
            	}
            	else
            	{
            		this->AddItem(m_pEquipment[nPosition]);
            		m_pEquipment[nPosition] = NULL;
            		this->SetLWeapon(ID_NONE);
            	}
            }*/
        }
        else
        {
            this->SetRWeapon(ID_NONE);
        }
        break;
    case ITEMPOSITION_WEAPONL:
        m_pEquipment[nType] = pItem;
        if (pItem)
        {
            this->SetLWeapon(pItem->GetTypeID());
        }
        else
        {
            this->SetLWeapon(ID_NONE);
        }
        break;
    case ITEMPOSITION_RINGR:
        m_pEquipment[nType] = pItem;
        break;
    case ITEMPOSITION_TREASURE:
        m_pEquipment[nType] = pItem;
        break;
    case ITEMPOSITION_SHOES:
        m_pEquipment[nType] = pItem;
        break;
    case ITEMPOSITION_MOUNT:
        m_pEquipment[nType] = pItem;
        this->SetMount(pItem->GetTypeID());
        break;
    case ITEMPOSITION_SPRITE:
        m_pEquipment[nType] = pItem;
        break;
    }
    ::PostCmd(CMD_FLASHEQUIPMENT);
}

//--------------------------------------------------------------
BOOL CHero::EquipItem (int nPosition, OBJID id)
{
    if (this->IsDead())
    {
        return false;
    }
    CCommand* pCommand = this->GetCommand();
    if (pCommand
            && pCommand->iType != _COMMAND_STANDBY
            && pCommand->iType != _COMMAND_WALK
            && pCommand->iType != _COMMAND_RUN)
    {
        return false;
    }
    CItem* pItem	= this->GetItem(id);
    if (!pItem)
    {
        return false;
    }
    if (!this->TryItem(pItem))
    {
        return false;
    }
    if (pItem->GetTypeID() == _ARROW && pItem->GetTypeID() == _DART)
    {
        if (pItem->GetTypeID() == _ARROW && this->GetRWeapon() / 1000 % 1000 != 500)
        {
            return false;
        }
        else if (pItem->GetTypeID() == _ARROW && nPosition == ITEMPOSITION_WEAPONL)
        {
            CMsgItem msg;
            if (msg.Create(id, nPosition, ITEMACT_USE))
            {
                msg.Send();
            }
            return true;
        }
        if (pItem->GetTypeID() == _DART && this->GetRWeapon() / 1000 % 1000 != 501)
        {
            return false;
        }
        else if (pItem->GetTypeID() == _DART && nPosition == ITEMPOSITION_WEAPONL)
        {
            CMsgItem msg;
            if (msg.Create(id, nPosition, ITEMACT_USE))
            {
                msg.Send();
            }
            return true;
        }
        return false;
    }
    switch(pItem->GetSort())
    {
    case ITEMSORT_HELMET:
        if (nPosition != ITEMPOSITION_HELMET)
        {
            return false;
        }
        break;
    case ITEMSORT_NECKLACE:
        if (nPosition != ITEMPOSITION_NECKLACE)
        {
            return false;
        }
        break;
    case ITEMSORT_ARMOR:
        if (nPosition != ITEMPOSITION_ARMOR)
        {
            return false;
        }
        break;
    case ITEMSORT_WEAPON_SINGLE_HAND:
        {
            if (nPosition != ITEMPOSITION_WEAPONR)
            {
                return false;
            }
        }
        break;
    case ITEMSORT_WEAPON_DOUBLE_HAND:
        {
            if (nPosition != ITEMPOSITION_WEAPONR)
            {
                const OBJID idEqumipError = 100049;
                g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idEqumipError));
                return false;
            }
            if (this->GetEquipment(ITEMPOSITION_WEAPONL))
            {
                const OBJID idEqumipError = 100047;
                g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idEqumipError));
                return false;
            }
        }
        break;
    case ITEMSORT_SHIELD:
        {
            if (nPosition != ITEMPOSITION_WEAPONL)
            {
                return false;
            }
            CItem* pEquipment = this->GetEquipment(ITEMPOSITION_WEAPONR);
            if (!pEquipment)
            {
                return false;
            }
            if (pEquipment && ITEMSORT_WEAPON_DOUBLE_HAND == pEquipment->GetSort())
            {
                const OBJID idEqumipError = 100048;
                g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idEqumipError));
                return false;
            }
        }
        break;
    case ITEMSORT_RING:
        if (nPosition != ITEMPOSITION_RINGR)
        {
            return false;
        }
        break;
    case ITEMSORT_SHOES:
        if (nPosition != ITEMPOSITION_SHOES)
        {
            return false;
        }
        break;
    }
    CMsgItem msg;
    if (msg.Create(id, nPosition, ITEMACT_USE))
    {
        msg.Send();
    }
    return true;
}

//--------------------------------------------------------------
BOOL CHero::UnequipItem(int nPosition)
{
    if (this->IsDead())
    {
        return false;
    }
    if (nPosition == ITEMPOSITION_WEAPONR)
    {
        if (this->GetEquipment(ITEMPOSITION_WEAPONL))
        {
            const OBJID idUnequipLeftWeaponFirst = 100053;
            g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idUnequipLeftWeaponFirst));
            return false;
        }
    }
    CItem* pItem = this->GetEquipment(nPosition);
    if (!pItem)
    {
        return false;
    }
    CMsgItem msg;
    if (msg.Create(pItem->GetID(), nPosition, ITEMACT_UNEQUIP))
    {
        msg.Send();
    }
    return true;
}

//--------------------------------------------------------------
CItem* CHero::SearchEquipment(OBJID idItem)
{
    if (ID_NONE == idItem)
    {
        return NULL;
    }
    for (int i = 0; i < _MAX_EQUIPMENT; i++)
    {
        CItem* pEquip = m_pEquipment[i];
        if (pEquip && pEquip->GetID() == idItem)
        {
            return pEquip;
        }
    }
    return NULL;
}

//--------------------------------------------------------------
BOOL CHero::CanEquip(int nPosition, CItem* pItem) const
{
    return true;
}

//--------------------------------------------------------------
DWORD CHero::GetCarryingCapacity()
{
    int nStep = 0;
    int nBaseWeight = 0;
    switch(this->GetProfessionSort())
    {
    case PROFSORT_SOLDIER:
        nStep = 3;
        nBaseWeight = 30;
        break;
    case PROFSORT_ARCHER:
        nStep = 2;
        nBaseWeight = 20;
        break;
    case PROFSORT_MAGICIAN:
        nStep = 2;
        nBaseWeight = 20;
        break;
    }
    int nWeight = nBaseWeight + this->GetLev() * nStep - nStep;
    return __max(0, nWeight);
}
//--------------------------------------------------------------
DWORD CHero::GetItemTotalWeight()
{
    // 圣战不走负重
    return 0;
}
//--------------------------------------------------------------
BOOL CHero::IsPackageFull(CItem* pItem)
{
    if (pItem)
    {
        int nPackageType = pItem->GetPackageType();
        if ( g_objHero.GetItemAmount (nPackageType) < g_objHero.GetPackageLimit (nPackageType) )
        {
            return FALSE;
        }
        if (pItem->GetAmount() >= pItem->GetAmountLimit())
        {
            if ( g_objHero.GetItemAmount (nPackageType) < g_objHero.GetPackageLimit (nPackageType) )
            {
                return FALSE;
            }
        }
        else
        {
            int nAmount = g_objHero.GetItemAmount(nPackageType);
            for(int i = 0 ; i < nAmount ; i++)
            {
                CItem* pOldItem = g_objHero.GetItemByIndex(i, nPackageType);
                if (pOldItem && pOldItem->GetTypeID() == pItem->GetTypeID()
                        && (pItem->GetAmount() + pOldItem->GetAmount() <= pItem->GetAmountLimit()))
                {
                    return FALSE;
                }
            }
        }
    }
    return TRUE;
}
//--------------------------------------------------------------
int CHero::GetAtkRange()
{
    /*	if (this->HaveBow())
    		return this->GetBowShootRange();*/
    if (this->IsTransform())
    {
        return 1;
    }
    int nRange = 1, nRangeR = 0, nRangeL = 0;
    CItem* pRWeapon = this->GetEquipment(ITEMPOSITION_WEAPONR);
    if (pRWeapon)
    {
        nRangeR = pRWeapon->GetAtkRange();
    }
    CItem* pLWeapon = this->GetEquipment(ITEMPOSITION_WEAPONL);
    if (pLWeapon)
    {
        nRangeL = pLWeapon->GetAtkRange();
    }
    if (nRangeR && nRangeL)
    {
        nRange = (nRangeR + nRangeL) / 2;
    }
    else if (nRangeR)
    {
        nRange = nRangeR;
    }
    else if (nRangeL)
    {
        nRange = nRangeL;
    }
    // weapon skill factor
    int nAmount = m_setMagic.size();
    for(int i = 0; i < nAmount; i ++)
    {
        CMagic* pMagic = m_setMagic[i];
        if (pMagic && (MAGICSORT_ATKRANGE == pMagic->m_infoMagicType.dwActionSort))
        {
            CItem* pWeaponR = this->GetEquipment(ITEMPOSITION_WEAPONR);
            CItem* pWeaponL = this->GetEquipment(ITEMPOSITION_WEAPONL);
            if ((pWeaponR && pWeaponR->GetWeaponSkillType() == pMagic->GetWeaponRequired())
                    || (pWeaponL && pWeaponL->GetWeaponSkillType() == pMagic->GetWeaponRequired()))
            {
                nRange += pMagic->GetPower();
            }
        }
    }
    return nRange;
}

//--------------------------------------------------------------
void CHero::ImproveItem(OBJID idItem, OBJID idTreasure)
{
    CItem* pTreasure = this->GetItem(idTreasure);
    if (!pTreasure || !pTreasure->IsDragonBall())
    {
        return;
    }
    CItem* pItem = this->GetItem(idItem);
    if (!pItem || pItem->GetAmount() < pItem->GetAmountLimit())
    {
        return;
    }
    CMsgItem msg;
    IF_SUC (msg.Create(idItem, idTreasure, ITEMACT_IMPROVE))
    msg.Send();
}

//--------------------------------------------------------------
void CHero::UpLevelItem(OBJID idItem, OBJID idTreasure)
{
    CItem* pTreasure = this->GetItem(idTreasure);
    if (!pTreasure || !pTreasure->IsShootingStar())
    {
        return;
    }
    CItem* pItem = this->GetItem(idItem);
    if (!pItem || pItem->GetAmount() < pItem->GetAmountLimit())
    {
        return;
    }
    CMsgDataArray msgDataArray;
    long id[3];
    id[0] = idItem;
    id[1] = idTreasure;
    if (msgDataArray.Create(_DATAARRAY_ACTION_UPGRADE_EQUIPLEVEL, 2, id))
    {
        msgDataArray.Send();
    }
}

//--------------------------------------------------------------
void CHero::EmbedGem	(OBJID idItem, OBJID idGem, int nPos)
{
    // param check
    if (!this->GetItem(idItem))
    {
        return;
    }
    if (!this->GetItem(idGem))
    {
        return;
    }
    if (nPos != 1 && nPos != 2)
    {
        return;
    }
    // send msg
    CMsgGemEmbed msg;
    IF_OK (msg.Create(this->GetID(), idItem, idGem, nPos))
    msg.Send();
}

//--------------------------------------------------------------
void CHero::TakeOutGem	(OBJID idItem, int nPos)
{
    // param check
    if (!this->GetItem(idItem))
    {
        return;
    }
    if (nPos != 1 && nPos != 2)
    {
        return;
    }
    // send msg
    CMsgGemEmbed msg;
    IF_OK (msg.Create(this->GetID(), idItem, ID_NONE, nPos, GEM_TAKEOUT))
    msg.Send();
}

//--------------------------------------------------------------
BOOL CHero::IsMiner(void)
{
    CItem* pWeapon = this->GetEquipment(ITEMPOSITION_WEAPONR);
    if (!pWeapon)
    {
        return false;
    }
    OBJID idType = pWeapon->GetTypeID();
    if (ID_NONE == idType || 562 != (idType / 1000) % 1000)
    {
        return false;
    }
    return true;
}

//--------------------------------------------------------------
void CHero::Mine(void)
{
    if (this->IsTransform())
    {
        // 变身以后不可采矿...
        return;
    }
    // condition check
    CItem* pWeapon = this->GetEquipment(ITEMPOSITION_WEAPONR);
    if (!pWeapon)
    {
        OBJID idMineToolError = 100054;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idMineToolError));
        return;
    }
    OBJID idType = pWeapon->GetTypeID();
    if (ID_NONE == idType || 562 != (idType / 1000) % 1000)
    {
        OBJID idMineToolError = 100054;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idMineToolError));
        return;
    }
    // prevent repeat
    if (_COMMAND_MINE == this->GetCommandType())
    {
        return;
    }
    // set cmd now
    CCommand cmd;
    cmd.iType	= _COMMAND_MINE;
    cmd.iStatus	= _CMDSTATUS_BEGIN;
    this->SetCommand(&cmd);
    CMsgAction msg;
    IF_SUC (msg.Create(this->GetID(), 0, 0, this->GetDir(), actionMine))
    msg.Send();
}

//--------------------------------------------------------------
BOOL CHero::IsAllEquipmentNonsuch()
{
    BOOL bReturn = true;
    for (int i = 0; i < _MAX_EQUIPMENT; i++)
    {
        if (!m_pEquipment[i])
        {
            bReturn = false;
        }
        else
        {
            if (!CItem::IsNonsuchItem(m_pEquipment[i]->GetTypeID()))
            {
                bReturn = false;
            }
        }
    }
    return bReturn;
}
//--------------------------------------------------------------
BOOL CHero::SplitItem	(OBJID idItem, int nAmount)
{
    CItem* pItem = g_objHero.GetItem(idItem);
    if (!pItem)
    {
        return false;
    }
    if (nAmount <= 0)
    {
        return false;
    }
    if (pItem->GetSort() != ITEMSORT_EXPEND)
    {
        return false;
    }
    if (pItem->GetAmount() <= nAmount)
    {
        return false;
    }
    CMsgItem msg;
    if (msg.Create(idItem, nAmount, ITEMACT_SPLITITEM))
    {
        msg.Send();
    }
    return true;
}
//--------------------------------------------------------------
BOOL CHero::CombinItem	(OBJID idItemSource, OBJID idItemTarget)
{
    /*	CItem* pItemSource = g_objHero.GetItem(idItemSource);
    	CItem* pItemTarget = g_objHero.GetItem(idItemTarget);
    	if (!pItemSource || !pItemTarget)
    		return false;
    	if (pItemSource->GetSort() != ITEMSORT_EXPEND && pItemSource->GetSort() != ITEMSORT_EXPEND)
    		return false;*/
    CMsgItem msg;
    if (msg.Create(idItemSource, idItemTarget, ITEMACT_COMBINEITEM))
    {
        msg.Send();
    }
    return true;
}
//--------------------------------------------------------------
void CHero::IdentItem(OBJID idItem)
{
    CItem* pItem = this->GetItem(idItem);
    if (pItem && pItem->TestStatus(_ITEM_STATUS_NOT_IDENT))
    {
        CMsgItem msgItem;
        if (msgItem.Create(idItem, 0, ITEMACT_IDENT))
        {
            msgItem.Send();
        }
    }
    else
    {
        // 只有未鉴定物品才能被鉴定
        const OBJID idStr = 100158;
        g_objGameMsg.AddMsg(g_objGameDataSet.GetStr(idStr));
    }
}
//--------------------------------------------------------------
void	CHero::ImproveQuality(OBJID idItem, OBJID idGem)
{
    CMsgDataArray msgDataArray;
    long id[2];
    id[0] = idItem;
    id[1] = idGem;
    if (msgDataArray.Create(_DATAARRAY_ACTION_UPGRADE_QUALITY, 2, id))
    {
        msgDataArray.Send();
    }
}
//--------------------------------------------------------------
void	CHero::ImproveSoulDegree(OBJID idItem, OBJID idGem)
{
    CMsgDataArray msgDataArray;
    long id[2];
    id[0] = idItem;
    id[1] = idGem;
    if (msgDataArray.Create(_DATAARRAY_ACTION_UPGRADE_SUPERADDITION, 2, id))
    {
        msgDataArray.Send();
    }
}
//--------------------------------------------------------------
void	CHero::ImproveMagic(OBJID idItem, OBJID idGem)
{
    CMsgDataArray msgDataArray;
    long id[2];
    id[0] = idItem;
    id[1] = idGem;
    if (msgDataArray.Create(_DATAARRAY_ACTION_EMBEDGEM, 2, id))
    {
        msgDataArray.Send();
    }
}
//--------------------------------------------------------------

int		CHero::GetPackageLimit(int nPackageType)
{
    int nLimit = 0;
    switch(nPackageType)
    {
    case PACKAGE_BAG:
        nLimit =  _MAX_HEROITEM;
        break;
    case PACKAGE_PET:
        nLimit = _MAX_HEROPET;
        break;
    case PACKAGE_EGG:
        nLimit = _MAX_HEROEGG;
        break;
    case PACKAGE_SOUL:
        nLimit = _MAX_HEROITEM;
        break;
    default:
        break;
    }
    return nLimit;
}

int CHero::GetGhostGemAmount(int nSubType)
{
    int nReturn = 0;
    int nAmount = this->GetItemAmount(PACKAGE_SOUL)	;
    for(int i = 0; i < nAmount; i++)
    {
        CItem* pItem = this->GetItemByIndex(i, PACKAGE_SOUL);
        if (pItem != NULL)
        {
            if (nSubType / 1000 * 1000 == pItem->GetSubType() / 1000 * 1000)
            {
                nReturn++;
            }
        }
    }
    return nReturn;
}


void CHero::CompletePlayerTaskForItem(OBJID idTask, OBJID idItem)
{
    if (idTask)
    {
        CMsgItem msgItem;
        if (msgItem.Create(idTask, idItem, ITEMACT_COMPLETE_TASK))
        {
            msgItem.Send();
        }
    }
}

//-------------------------------------------------------------------
//拍卖物品
void CHero::AuctionAddItem(OBJID idNpc, OBJID idItem, int dwValue)
{
    if (idNpc != ID_NONE && idItem != ID_NONE)
    {
        CMsgAuction msg;
        if (msg.Create(idNpc, idItem, dwValue, AUCTION_ADD))
        {
            msg.Send();
        }
    }
}

void CHero::AuctionBid(OBJID idNpc, OBJID idItem, int dwValue)
{
    if (idNpc != ID_NONE && idItem != ID_NONE)
    {
        CMsgAuction msg;
        if (msg.Create(idNpc, idItem, dwValue, AUCTION_BID))
        {
            msg.Send();
        }
    }
}

void CHero::AuctionItemInfo(OBJID idNpc, OBJID idItem)
{
    if (idNpc != ID_NONE && idItem != ID_NONE)
    {
        CMsgAuction msg;
        if (msg.Create(idNpc, idItem, AUCTION_ITEM_INFO))
        {
            msg.Send();
        }
    }
}

CItem* CHero::GetAuctionItem()
{
    //temp code!
    if (m_AuctionItem != NULL)
    {
        return m_AuctionItem;
    }
    else
    {
        return NULL;
    }
}

void CHero::SetAuctionItem(CItem* pItem)
{
    //temp code!
    if (pItem != NULL)
    {
        m_AuctionItem = pItem;
    }
}

void CHero::DelAuctionItem ()
{
    m_AuctionItemId = 0;
    m_AuctionItemMoney = 0;		//拍卖价格
    strcpy(m_AuctionName, "");	//加价人
    strcpy(m_AuctionOwnerName, ""); //物品主人
    SAFE_DELETE(m_AuctionItem);	//拍卖物品
}
