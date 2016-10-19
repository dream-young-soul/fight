
//**********************************************************
// 代码编辑器
//**********************************************************

// Item.cpp: implementation of the CItem class.
//
//////////////////////////////////////////////////////////////////////

#include "Item.h"
#include "ItemType.h"
#include "MapGroup.h"
#include "I_mydb.h"
#include "User.h"
#include "MsgEudemonAttrib.h"
//---jinggy---2004-11-12---Begin
#include "netmsg.h"
#include "AllMsg.h"

#define WARGHOSTVALUE_LIMIT  1024	//战魂经验值的界限
//---jinggy---2004-11-12---End

MYHEAP_IMPLEMENTATION(CItem, s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CItem::CItem()
{
    SetObjType(OBJ_ITEM);
}

//////////////////////////////////////////////////////////////////////
CItem::~CItem()
{
}

//////////////////////////////////////////////////////////////////////
bool CItem::CreateItemInfo(ItemInfoStruct& info, DWORD dwValue, CNpcType* pType, int nQuality/*=0*/)
{
    if (!pType)
    {
        return false;
    }
    // item quality
    DWORD dwItemQuality = 0;
    if (nQuality == 0)
    {
        // auto create this param
        int nRand = ::RandGet(100);
        if (nRand >= 0 && nRand < 30)
        {
            dwItemQuality = 5;
        }
        else if (nRand >= 30 && nRand < 70)
        {
            dwItemQuality = 4;
        }
        else
        {
            dwItemQuality = 3;
        }
    }
    else
    {
        dwItemQuality = nQuality;
    }
    // item sort & item color
    DWORD dwItemSort	= 0;
    DWORD dwItemColor	= 0;
    DWORD dwItemLev		= 0;
    int nRand = ::RandGet(1200);
    if (nRand >= 0 && nRand < 20)
    {
        // shoes
        dwItemSort	= 160;
        dwItemLev	= pType->GetInt(NPCTYPEDATA_DROP_SHOES);
    }
    else if (nRand >= 20 && nRand < 50)
    {
        // necklace
        DWORD dwSort[2] = {120, 121};
        dwItemSort = dwSort[::RandGet(2)];
        dwItemLev	= pType->GetInt(NPCTYPEDATA_DROP_NECKLACE);
    }
    else if (nRand >= 50 && nRand < 100)
    {
        // ring
        DWORD dwSort[3] = {150, 151, 152};
        dwItemSort = dwSort[::RandGet(3)];
        dwItemLev	= pType->GetInt(NPCTYPEDATA_DROP_RING);
    }
    else if (nRand >= 100 && nRand < 400)
    {
        // armet
        DWORD dwSort[6] = {111, 112, 113, 114, 117, 118};
        dwItemSort = dwSort[::RandGet(6)];
        if (dwItemSort <= 118)	// have color
        {
            dwItemColor = ::RandGet(7) + 3;
        }
        dwItemLev	= pType->GetInt(NPCTYPEDATA_DROP_ARMET);
    }
    else if (nRand >= 400 && nRand < 700)
    {
        // armor
        DWORD dwSort[5] = {130, 131, 132, 133, 134};
        dwItemSort	= dwSort[::RandGet(5)];
        dwItemColor = ::RandGet(7) + 3;
        dwItemLev	= pType->GetInt(NPCTYPEDATA_DROP_ARMOR);
    }
    else
    {
        // weapon & shield
        int nRate = ::RandGet(100);
        if (nRate >= 0 && nRate < 20)		// 20% for sword of Taoist
        {
            dwItemSort = 421;
        }
        else if (nRate >= 20 && nRate < 40)	// 20% for archer
        {
            // dwItemSort = 500;
            return false;
        }
        else if (nRate >= 40 && nRate < 80)	// 40% for single hand weapon
        {
            DWORD dwSort[11] = {410, 420, 421, 430, 440, 450, 460, 480, 481, 490, 500};
            dwItemSort = dwSort[::RandGet(11)];
        }
        else	// 20% for two hand weapon
        {
            DWORD dwSort[6] = {510, 530, 560, 561, 580, 900};
            dwItemSort = dwSort[::RandGet(6)];
            if (900 == dwItemSort)	// shield
            {
                dwItemColor = ::RandGet(7) + 3;
            }
        }
        if (900 == dwItemSort)	// shield
        {
            dwItemColor = ::RandGet(7) + 3;
            dwItemLev	= pType->GetInt(NPCTYPEDATA_DROP_SHIELD);
        }
        else // weapon
        {
            dwItemLev	= pType->GetInt(NPCTYPEDATA_DROP_WEAPON);
        }
    }
    // item lev
    if (dwItemLev == 99)
    {
        return false;
    }
    {
        int nRate = ::RandGet(100);
        if (nRate < 50)		// 50% down one lev
        {
            DWORD dwLev = dwItemLev;
            dwItemLev = ::RandGet(dwLev / 2) + dwLev / 3;
            if (dwItemLev >= 1)
            {
                dwItemLev--;
            }
        }
        else if (nRate >= 80)	// 20% up one lev
        {
            if ((dwItemSort >= 110 && dwItemSort <= 114) ||
                    (dwItemSort >= 130 && dwItemSort <= 134) ||
                    (dwItemSort >= 900 && dwItemSort <= 999))
            {
                // item with color
                dwItemLev = __min(dwItemLev + 1, 9);
            }
            else
            {
                dwItemLev = __min(dwItemLev + 1, 23);
            }
        }
        // 50% do nothing
    }
    // item type
    OBJID idItemType = dwItemSort * 1000 + dwItemColor * 100 + dwItemLev * 10 + dwItemQuality;
    CItemTypeData* pItemType = ItemType()->QueryItemType(idItemType);
    if (!pItemType)
    {
        return false;
    }
    // fill item info
    memset(&info, 0L, sizeof(info));
    info.id			= ID_NONE;
    info.idOwner	= ID_NONE;
    info.idPlayer	= ID_NONE;
    info.idType		= idItemType;
    info.nAmountLimit	= pItemType->GetInt(ITEMTYPEDATA_AMOUNT_LIMIT)*::RandomRateGet(0.3);
    if (info.nAmountLimit < 1)
    {
        info.nAmountLimit = 1;
    }
    if (dwItemQuality > 5)
    {
        info.nAmount = info.nAmountLimit * (50 +::RandGet(50)) / 100;
    }
    else
    {
        DWORD dwPrice = pItemType->GetInt(ITEMTYPEDATA_PRICE);
        if (dwPrice <= 0)
        {
            dwPrice = 1;
        }
        info.nAmount = 3 * info.nAmountLimit * dwValue / dwPrice;
        if (info.nAmount >= info.nAmountLimit)
        {
            info.nAmount = info.nAmountLimit;
        }
        if (info.nAmount < 1)
        {
            info.nAmount = 1;
            //return false;
        }
    }
    // gem hole
    if (idItemType >= 400000 && idItemType < 599999)
    {
        // is weapon
        int nRate = ::RandGet(100);
        if (nRate < 5)			// 5% got 2 holes
        {
            info.nGem1 = 0xff;
            info.nGem2 = 0xff;
        }
        else if (nRate < 20)	// 15% got 1 hole
        {
            info.nGem1 = 0xff;
        }
        // 80% do nothing
    }
    // magic
    // to do...
    return true;
}

//////////////////////////////////////////////////////////////////////
int	CItem::CalcRepairMoney()
{
    // the highest price is about million.
    int nRecoverDurability = __max(0, GetInt(ITEMDATA_AMOUNTLIMIT) - GetInt(ITEMDATA_AMOUNT));
    if (nRecoverDurability == 0)
    {
        return 0;
    }
    // 修理价格=(持久上限-当前持久)/持久上限*商店出售价格*50%
    // 案子中没有说明当amount_limit==0的时候怎么处理，这里只好当作修理价格为0处理了
    int nRepairCost	= 0;
    if (GetInt(ITEMDATA_AMOUNTLIMIT) > 0)
    {
        nRepairCost	= GetInt(ITEMDATA_PRICE) * nRecoverDurability / GetInt(ITEMDATA_AMOUNTLIMIT) / 2;
    }
    return __max(1, nRepairCost);	// 最低修理价格为1
}

//////////////////////////////////////////////////////////////////////
DWORD CItem::GetSellPrice()
{
    if (GetInt(ITEMDATA_AMOUNTLIMIT) == 0)
    {
        return 0;
    }
    int nAmount = this->GetInt(ITEMDATA_AMOUNT);
    if (nAmount > this->GetInt(ITEMDATA_AMOUNTLIMIT))
    {
        nAmount = this->GetInt(ITEMDATA_AMOUNTLIMIT);
    }
    DWORD dwPrice = GetInt(ITEMDATA_PRICE);
    dwPrice	= dwPrice + MulDiv(dwPrice, GetQuality() * 5, 100) /* + MulDiv(dwPrice, (GetInt(ITEMDATA_ADDITION)/10)*5, 100)*/ + MulDiv(dwPrice, GetInt(ITEMDATA_LUCK) * 5, 100);
    dwPrice = MulDiv(dwPrice / 3, nAmount, GetInt(ITEMDATA_AMOUNT_ORIGINAL));
    // ident
    if (IsNeedIdent())
    {
        dwPrice = UNIDENT_SALE_FEE;
    }
    if (IsArrowSort())
    {
        dwPrice = 0;
    }
    return dwPrice;
}

//////////////////////////////////////////////////////////////////////
CItem*	CItem::Split(int nNum)
{
    CHECKF(IsPileEnable());
    int nCurrNum = GetInt(ITEMDATA_AMOUNT);
    CHECKF(nNum > 0 && nNum < nCurrNum);
    DEBUG_TRY ////////
    CItemPtr pItem = Clone();
    CHECKF(pItem);
    pItem->SetInt(ITEMDATA_AMOUNT, nNum, UPDATE_TRUE);
    this->SetInt(ITEMDATA_AMOUNT, nCurrNum - nNum, UPDATE_TRUE);
    return pItem;
    DEBUG_CATCH("item split") //////////
    return NULL;
}

//////////////////////////////////////////////////////////////////////
CItem*	CItem::Clone()		// 新物品存数据库
{
    CItemPtr pItem = CreateNew();
    IF_NOT(pItem)
    return NULL;
    ItemInfoStruct	info;
    GetInfo(&info);
    info.id		= ID_NONE;		// new record
    if (this->IsEudemon())
    {
        ASSERT (m_pEudemonData);
        if (pItem->Create(m_pData->GetRecord(), &info, m_pEudemonData->GetRecord(), true))		// true: insert
        {
            return pItem;
        }
    }
    else
    {
        if (pItem->Create(m_pData->GetRecord(), &info, NULL, true))		// true: insert
        {
            return pItem;
        }
    }
    pItem->ReleaseByOwner();
    return NULL;
}

/*/////////////////////////////////////////////////////////////////////
int	CItem::GetItemSort()
{
	//取百万+十万位
	int nType = ITEMSORT_INVALID;
	switch((GetInt(ITEMDATA_TYPE)%10000000)/100000)
	{
	case 10:
		nType = ITEMSORT_EXPEND;
		break;

	case 1:
		{
			////取十万位+万位
			switch((GetInt(ITEMDATA_TYPE)%1000000)/10000)
			{
			case 11:
				nType = ITEMSORT_HELMET;
				break;
			case 12:
				nType = ITEMSORT_NECKLACE;
				break;
			case 13:
				nType = ITEMSORT_ARMOR;
				break;
			case 15:
				nType = ITEMSORT_RING;
				break;
			case 16:
				nType = ITEMSORT_SHOES;
				break;
			}
		}
		break;

	case 4:
		nType = ITEMSORT_WEAPON1;
		break;

	case 5:
		nType = ITEMSORT_WEAPON2;
		break;

	case 6:
		nType = ITEMSORT_MOUNT;
		break;

	case 7:
		nType = ITEMSORT_OTHER;
		break;

	case 9:
		nType = ITEMSORT_SHIELD;
		break;
	}

	return nType;
}
//*/
/*/////////////////////////////////////////////////////////////////////
int	CItem::GetItemType()
{
	int nType = ITEMTYPE_INVALID;
	//取百万+十万位
	switch((GetInt(ITEMDATA_TYPE)%10000000)/100000)
	{
	case 7:
	case 10:
		nType = ((GetInt(ITEMDATA_TYPE)%100000)/10000)*10000;	// 返回万位*万
		break;

	case 4:
	case 5:
		nType = ((GetInt(ITEMDATA_TYPE)%100000)/1000)*1000;		// 返回（万位，千位）*千
		break;

	default:
		break;
	}

	return nType;
}
//*/
//////////////////////////////////////////////////////////////////////
int	CItem::GetItemType(OBJID idType)
{
    if (GetItemSort(idType) == ITEMSORT_WEAPON1 || GetItemSort(idType) == ITEMSORT_WEAPON2) // || GetItemSort(idType) == ITEMSORT_EXPEND)
    {
        return  ((idType % 100000) / 1000) * 1000;    // 返回（万位，千位）*千
    }
    else
    {
        return  ((idType % 100000) / 10000) * 10000;    // 返回万位*万
    }
}
//////////////////////////////////////////////////////////////////////
int	CItem::GetItemSubType()
{
    // 取十万，万，千位
    return (GetInt(ITEMDATA_TYPE) % 1000000) / 1000;
}

//////////////////////////////////////////////////////////////////////
int	CItem::GetLevel(void)
{
    if (this->IsShield() || this->IsArmor() || this->IsHelmet())
    {
        return (GetInt(ITEMDATA_TYPE) % 100) / 10;
    }
    else
    {
        return (GetInt(ITEMDATA_TYPE) % 1000) / 10;
    }
}

//////////////////////////////////////////////////////////////////////
/*
 对表 cq_item 中的 type, 改变 type 的末尾就是改变装备的品质 ;
 通过 type 和 magic3 就可定位到  表 cq_itemaddition (不同品质及追加属性的装备对应的其他属性)
*/
bool CItem::ChangeType	(OBJID idNewType) //
{
    // type
    CItemTypeData* pType = ItemType()->QueryItemType(idNewType);
    if (!pType)
    {
        LOGERROR("ChangeType类型[%d]错误!", idNewType);
        return false;
    }
    m_pType = pType;
    this->SetInt(ITEMDATA_TYPE, idNewType);
    // TODO:这里暂时不考虑从非幻兽物品到幻兽物品的type变换
    // 否则需要读入或创建eudemon表数据
    // zlong 2005-03-02
    m_pAddition = ItemAddition()->QueryItemAddition(idNewType, GetInt(ITEMDATA_ADDITION));
    return true;
}
//////////////////////////////////////////////////////////////////////
/*
对表 cq_item 中的 magic3, 改变 type 的末尾就是改变装备的追加等级属性
*/
bool CItem::ChangeAddition(int nLevel)
{
    CItemAdditionData* pAddition = NULL;
    if (nLevel > 0)
    {
        pAddition = ItemAddition()->QueryItemAddition(this->GetInt(ITEMDATA_TYPE), nLevel);
        if (!pAddition)
        {
            return false;
        }
    }
    m_pAddition	= pAddition;
    this->SetInt(ITEMDATA_ADDITION, nLevel, true);
    return true;
}

//////////////////////////////////////////////////////////////////////
int	CItem::GetGemDmgEffect()
{
    int nEffect = 0;
    int nGem[2] = { this->GetInt(ITEMDATA_GEM1), this->GetInt(ITEMDATA_GEM2) };
    for (int i = 0; i < 2; i++)
    {
        if (nGem[i] != GEM_NONE && nGem[i] != GEM_HOLE)
        {
            OBJID idGemType = 700000 + nGem[i];
            switch(idGemType)
            {
            case GEM_DMG_LOW:
                nEffect += 5;
                break;
            case GEM_DMG_MID:
                nEffect += 10;
                break;
            case GEM_DMG_HGT:
                nEffect += 15;
                break;
            }
        }
    }
    return nEffect;
}

//////////////////////////////////////////////////////////////////////
int	CItem::GetGemMgcAtkEffect()
{
    int nEffect = 0;
    int nGem[2] = { this->GetInt(ITEMDATA_GEM1), this->GetInt(ITEMDATA_GEM2) };
    for (int i = 0; i < 2; i++)
    {
        if (nGem[i] != GEM_NONE && nGem[i] != GEM_HOLE)
        {
            OBJID idGemType = 700000 + nGem[i];
            switch(idGemType)
            {
            case GEM_MATK_LOW:
                nEffect += 5;
                break;
            case GEM_MATK_MID:
                nEffect += 10;
                break;
            case GEM_MATK_HGT:
                nEffect += 15;
                break;
            }
        }
    }
    return nEffect;
}

//////////////////////////////////////////////////////////////////////
int	CItem::GetGemHitRateEffect()
{
    int nEffect = 0;
    int nGem[2] = { this->GetInt(ITEMDATA_GEM1), this->GetInt(ITEMDATA_GEM2) };
    for (int i = 0; i < 2; i++)
    {
        if (nGem[i] != GEM_NONE && nGem[i] != GEM_HOLE)
        {
            OBJID idGemType = 700000 + nGem[i];
            switch(idGemType)
            {
            case GEM_HIT_LOW:
                nEffect += 5;
                break;
            case GEM_HIT_MID:
                nEffect += 10;
                break;
            case GEM_HIT_HGT:
                nEffect += 15;
                break;
            }
        }
    }
    return nEffect;
}

//////////////////////////////////////////////////////////////////////
int	CItem::GetGemExpEffect()
{
    int nEffect = 0;
    int nGem[2] = { this->GetInt(ITEMDATA_GEM1), this->GetInt(ITEMDATA_GEM2) };
    for (int i = 0; i < 2; i++)
    {
        if (nGem[i] != GEM_NONE && nGem[i] != GEM_HOLE)
        {
            OBJID idGemType = 700000 + nGem[i];
            switch(idGemType)
            {
            case GEM_EXP_LOW:
                nEffect += 10;
                break;
            case GEM_EXP_MID:
                nEffect += 15;
                break;
            case GEM_EXP_HGT:
                nEffect += 25;
                break;
            }
        }
    }
    return nEffect;
}

//////////////////////////////////////////////////////////////////////
int	CItem::GetGemWpnExpEffect()
{
    int nEffect = 0;
    int nGem[2] = { this->GetInt(ITEMDATA_GEM1), this->GetInt(ITEMDATA_GEM2) };
    for (int i = 0; i < 2; i++)
    {
        if (nGem[i] != GEM_NONE && nGem[i] != GEM_HOLE)
        {
            OBJID idGemType = 700000 + nGem[i];
            switch(idGemType)
            {
            case GEM_WPNEXP_LOW:
                nEffect += 30;
                break;
            case GEM_WPNEXP_MID:
                nEffect += 50;
                break;
            case GEM_WPNEXP_HGT:
                nEffect += 100;
                break;
            }
        }
    }
    return nEffect;
}

//////////////////////////////////////////////////////////////////////
int	CItem::GetGemMgcExpEffect()
{
    int nEffect = 0;
    int nGem[2] = { this->GetInt(ITEMDATA_GEM1), this->GetInt(ITEMDATA_GEM2) };
    for (int i = 0; i < 2; i++)
    {
        if (nGem[i] != GEM_NONE && nGem[i] != GEM_HOLE)
        {
            OBJID idGemType = 700000 + nGem[i];
            switch(idGemType)
            {
            case GEM_MGCEXP_LOW:
                nEffect += 15;
                break;
            case GEM_MGCEXP_MID:
                nEffect += 30;
                break;
            case GEM_MGCEXP_HGT:
                nEffect += 50;
                break;
            }
        }
    }
    return nEffect;
}

//////////////////////////////////////////////////////////////////////
int CItem::GetGemDurEffect(OBJID idGemType)
{
    int nEffect = 0;
    // default...
    if (ID_NONE == idGemType)
    {
        int nGem[2] = { this->GetInt(ITEMDATA_GEM1), this->GetInt(ITEMDATA_GEM2) };
        for (int i = 0; i < 2; i++)
        {
            if (nGem[i] != GEM_NONE && nGem[i] != GEM_HOLE)
            {
                OBJID idGemType = 700000 + nGem[i];
                switch(idGemType)
                {
                case GEM_DUR_LOW:
                    nEffect += 50;
                    break;
                case GEM_DUR_MID:
                    nEffect += 100;
                    break;
                case GEM_DUR_HGT:
                    nEffect += 200;
                    break;
                }
            }
        }
    }
    else
    {
        switch(idGemType)
        {
        case GEM_DUR_LOW:
            nEffect += 50;
            break;
        case GEM_DUR_MID:
            nEffect += 100;
            break;
        case GEM_DUR_HGT:
            nEffect += 200;
            break;
        }
    }
    return nEffect;
}

//////////////////////////////////////////////////////////////////////
bool CItem::IsNonsuchItem(void)
{
    // treasure
    DWORD dwType = this->GetInt(ITEMDATA_TYPE);
    switch(dwType)
    {
    case TYPE_DRAGONBALL:
    case TYPE_SHOOTINGSTAR:
    case TYPE_DIVOICEITEM:
        return true;
        break;
    default:
        break;
    }
    // precious gem
    if (this->IsGem() && (dwType % 10) >= 2)
    {
        return true;
    }
    // game card
    if (this->IsGameCard())
    {
        return true;
    }
    // chest item
    if (this->IsChestItem())
    {
        return true;
    }
    // other type
    if (CItem::GetItemSort(dwType) == ITEMSORT_OTHER || CItem::GetItemSort(dwType) >= ITEMSORT_EXPEND)
    {
        return false;
    }
    // high quality
    if (this->GetQuality() >= 8)
    {
        return true;
    }
    // ....
    int nGem1 = this->GetInt(ITEMDATA_GEM1) % 10;
    int nGem2 = this->GetInt(ITEMDATA_GEM2) % 10;
    bool bIsNonsuch = false;
    if (IsWeapon())
    {
        if ((nGem1 != GEM_HOLE && nGem1 >= 2)
                || (nGem2 != GEM_HOLE && nGem2 >= 2))
        {
            bIsNonsuch = true;
        }
    }
    else if (IsFinery() || IsShield())
    {
        if (nGem1 != GEM_NONE || nGem2 != GEM_NONE)
        {
            bIsNonsuch = true;
        }
    }
    return bIsNonsuch;
}

//////////////////////////////////////////////////////////////////////
bool CItem::ChangeColor(int nColor)
{
    if (!(nColor >= 3 && nColor <= 9))
    {
        return false;
    }
    if (IsHelmet() || IsArmor() || IsShield())
    {
        DWORD idType = this->GetInt(ITEMDATA_TYPE);
        DWORD idNewType = idType - ((idType / 100) % 10) * 100 + nColor * 100;
        return this->ChangeType(idNewType);
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
bool CItem::RecoverDur	(void)
{
    int nMaxDur = this->GetInt(ITEMDATA_AMOUNTLIMIT_ORIGINAL);
    nMaxDur += nMaxDur * this->GetGemDurEffect() / 100;
    this->SetInt(ITEMDATA_AMOUNTLIMIT, nMaxDur, true);
    return true;
}

//////////////////////////////////////////////////////////////////////
int CItem::GetRecoverDurCost	(void)
{
    int nRealMaxDur = this->GetInt(ITEMDATA_AMOUNTLIMIT) * 100 / (100 + this->GetGemDurEffect());
    if (nRealMaxDur >= this->GetInt(ITEMDATA_AMOUNTLIMIT_ORIGINAL))
    {
        return 0;
    }
    int nRepairDur = this->GetInt(ITEMDATA_AMOUNTLIMIT_ORIGINAL) - nRealMaxDur;
    int nCost = 0;
    CHECKF(this->GetInt(ITEMDATA_AMOUNTLIMIT_ORIGINAL));
    switch(this->GetQuality())
    {
    case 9:
        nCost = ::MulDiv(5000000, nRepairDur, this->GetInt(ITEMDATA_AMOUNTLIMIT_ORIGINAL));
        nCost = __max(nCost, 500000);
        break;
    case 8:
        nCost = ::MulDiv(3500000, nRepairDur, this->GetInt(ITEMDATA_AMOUNTLIMIT_ORIGINAL));
        nCost = __max(nCost, 350000);
        break;
    default:
        nCost = ::MulDiv(1500000, nRepairDur, this->GetInt(ITEMDATA_AMOUNTLIMIT_ORIGINAL));
        nCost = __max(nCost, 150000);
        break;
    }
    return __max(1, nCost);
}
//////////////////////////////////////////////////////////////////////
int CItem::GetWeight(OBJID idType, int nAmount/*=0*/)		// nAmount: amount 字段值
{
    CItemTypeData* pType = ItemType()->QueryItemType(idType);
    IF_NOT(pType)
    return ERROR_WEIGHT;
    if (!nAmount)
    {
        nAmount	= pType->GetInt(ITEMTYPEDATA_AMOUNT);
    }
    if (IsArrowSort(idType))
    {
        return (pType->GetInt(ITEMTYPEDATA_AMOUNT) - 1) * pType->GetInt(ITEMTYPEDATA_WEIGHT) / ARROW_PILE_WEIGHT + 1;
    }
    if (CItem::IsExpend(idType))
    {
        return pType->GetInt(ITEMTYPEDATA_WEIGHT) * nAmount;
    }
    else
    {
        return pType->GetInt(ITEMTYPEDATA_WEIGHT);
    }
}
//////////////////////////////////////////////////////////////////////

int	CItem::AwardExp(int nExp, int nLuck /* = 0*/)
{
    if (nExp <= 0)
    {
        return nExp;
    }
    if (this->IsSprite())
    {
        int nLevupExp = this->GetInt(ITEMDATA_LEVEXP);
        if (nLevupExp <= 0)
        {
            return 0;
        }
        int nOrgProgress = ::MulDiv(this->GetExp(), 1000, nLevupExp);
        int nNewExp = nExp + this->GetExp();	// 先取原来经验
        if (nNewExp >= nLevupExp)
        {
            int nTypeId = this->GetInt(ITEMDATA_TYPE);
            // 升级：由于不同的等级是由不同的itemtype来区分的，因此每次升级实际上需要更新itemtype
            if (!this->ChangeType(nTypeId + 1))
            {
                return 0;
            }
            this->SetInt(ITEMDATA_EXP, 0);			// 经验清0
            this->GrowUp(nLuck);					// 增长属性
            this->SaveInfo();
        }
        else
        {
            this->SetInt(ITEMDATA_EXP, nNewExp);	// 增加经验
            int nCurProgress = ::MulDiv(this->GetExp(), 1000, nLevupExp);
            if (nOrgProgress / 200 != nCurProgress / 200)	//每隔1/5升级经验存一次
            {
                this->SaveInfo();
            }
        }
        return nNewExp;
    }
    return nExp;
}
//////////////////////////////////////////////////////////////////////

void CItem::GrowUp(int nLuck /* = 0*/)
{
    // 根据Luck调整实际成长率
    int nGrowth = __max(0, this->GetGrowth100() + nLuck);
    if (this->GetSpriteType() == _SPRITE_ADDITION_SOUL)
    {
        nGrowth = __min(nGrowth, MAX_SOUL_SPRITE_GROWTH);
    }
    else
    {
        nGrowth = __min(nGrowth, MAX_SPRITE_GROWTH);
    }
    int nAttrib = this->GetInt(ITEMDATA_ATTRIB);
    this->SetInt(ITEMDATA_ATTRIB, nAttrib + nGrowth);
}

//////////////////////////////////////////////////////////////////////
int CItem::AddGrowth(int nGrowth)
{
    int nNewGrowth = __max(0, this->GetGrowth100() + nGrowth);
    if (this->GetSpriteType() == _SPRITE_ADDITION_SOUL)
    {
        nNewGrowth = __min(nNewGrowth, MAX_SOUL_SPRITE_GROWTH);
    }
    else
    {
        nNewGrowth = __min(nNewGrowth, MAX_SPRITE_GROWTH);
    }
    this->SetInt(ITEMDATA_GROWTH, nNewGrowth, true);
    return nNewGrowth;
}

//////////////////////////////////////////////////////////////////////
int CItem::AdjustOriginalGrowth()
{
    int nGrowth = 0;
    int nRate = ::RandGet(100);
    if (this->GetSpriteType() == _SPRITE_ADDITION_SOUL)
    {
        if (nRate < 15)
        {
            nGrowth = ::RandGet(20);
        }
        else if (nRate < 35)
        {
            nGrowth = ::RandGet(20) + 20;
        }
        else if (nRate < 75)
        {
            nGrowth = 40;
        }
        else if (nRate < 95)
        {
            nGrowth = ::RandGet(19) + 41;
        }
        else
        {
            nGrowth = ::RandGet(20) + 60;
        }
    }
    else
    {
        if (nRate < 15)
        {
            nGrowth = ::RandGet(50);
        }
        else if (nRate < 35)
        {
            nGrowth = ::RandGet(50) + 50;
        }
        else if (nRate < 75)
        {
            nGrowth = 100;
        }
        else if (nRate < 95)
        {
            nGrowth = ::RandGet(49) + 101;
        }
        else
        {
            nGrowth = ::RandGet(50) + 150;
        }
    }
    this->SetInt(ITEMDATA_GROWTH, nGrowth, true);
    return nGrowth;
}

//////////////////////////////////////////////////////////////////////
int CItem::GetAddition(ITEMADDITIONDATA idx)
{
    if (!m_pAddition)
    {
        return 0;
    }
    return m_pAddition->GetInt(idx);
}

//////////////////////////////////////////////////////////////////////
bool CItem::IsEvolveEnable()
{
    return ((GetInt(ITEMDATA_TYPE) % 10 == 0)
            && (GetEudemonLevel() >= EUDEMON_EVOLVE_LEVEL1)
            && this->IsAliveEudemon());
}

//////////////////////////////////////////////////////////////////////
bool CItem::IsEvolve2Enable()
{
    // 还未经进行过一次进化
    if (GetInt(ITEMDATA_TYPE) % 10 == 0)
    {
        return false;
    }
    return (((GetInt(ITEMDATA_TYPE) / 10) % 10 == 0)
            && (GetEudemonLevel() >= EUDEMON_EVOLVE_LEVEL2)
            && this->IsAliveEudemon());
}

//////////////////////////////////////////////////////////////////////
UCHAR CItem::GetRelationShip(OBJID idDivine)
{
    CHECKF (idDivine >= MIN_DIVINE_ID && idDivine <= MAX_DIVINE_ID);
    CHECKF (m_pEudemonData);
    DWORD dwRelationShip = m_pEudemonData->GetInt(EUDEMONDATA_RELATIONSHIP);
    // dwRelationShip中从十进制个位到千万位分别表示与神识编号1~8的8种幻兽的关系
    // 只要取其中指定的十进制位返回
    return (dwRelationShip / (DWORD)pow(10.f, (int)idDivine - 1) % 10);
}

//////////////////////////////////////////////////////////////////////
bool CItem::AddRelationShip(OBJID idDivine, int nValue)
{
    CHECKF (idDivine >= MIN_DIVINE_ID && idDivine <= MAX_DIVINE_ID);
    CHECKF (m_pEudemonData);
    DWORD dwRelationShip = m_pEudemonData->GetInt(EUDEMONDATA_RELATIONSHIP);
    DWORD dwPow = (DWORD)pow(10.f, (int)idDivine - 1);
    int nRelationShip = dwRelationShip / dwPow % 10;
    nRelationShip = __min(9, __max(0, nRelationShip + nValue));	// 关系取值范围为0 ~ 9
    dwRelationShip = (dwRelationShip / (dwPow * 10) * dwPow * 10) + nRelationShip * dwPow + dwRelationShip % dwPow;
    m_pEudemonData->SetInt(EUDEMONDATA_RELATIONSHIP, dwRelationShip);
    m_pEudemonData->Update();
    return true;
}

//////////////////////////////////////////////////////////////////////
/*
bool CItem::SendEudemonAttrib(CUser* pUser)
{
	CHECKF(pUser);
	CHECKF(this->IsEudemon());

	int nLevel	= this->GetEudemonLevel();
	int nGrowth	= this->GetInt(ITEMDATA_GROWTH);

	ST_EUDEMON_ATTRIB	setEudemonAttrib[8];
	setEudemonAttrib[0].dwAttributeType		= _EUDEMONATTRIB_ATK_MAX;
	setEudemonAttrib[0].dwAttributeData		= this->GetInt(ITEMDATA_ATTACK_MAX_) * nLevel * nGrowth / 100;

	setEudemonAttrib[1].dwAttributeType		= _EUDEMONATTRIB_ATK_MIN;
	setEudemonAttrib[1].dwAttributeData		= this->GetInt(ITEMDATA_ATTACK_MIN_) * nLevel * nGrowth / 100;

	setEudemonAttrib[2].dwAttributeType		= _EUDEMONATTRIB_MAGICATK_MAX;
	setEudemonAttrib[2].dwAttributeData		= this->GetInt(ITEMDATA_MAGICATK_MAX_) * nLevel * nGrowth / 100;

	setEudemonAttrib[3].dwAttributeType		= _EUDEMONATTRIB_MAGICATK_MIN;
	setEudemonAttrib[3].dwAttributeData		= this->GetInt(ITEMDATA_MAGICATK_MIN_) * nLevel * nGrowth / 100;

	setEudemonAttrib[4].dwAttributeType		= _EUDEMONATTRIB_DEF;
	setEudemonAttrib[4].dwAttributeData		= this->GetInt(ITEMDATA_DEFENSE_) * nLevel * nGrowth / 100;

	setEudemonAttrib[5].dwAttributeType		= _EUDEMONATTRIB_MAGICDEF;
	setEudemonAttrib[5].dwAttributeData		= this->GetInt(ITEMDATA_MAGICDEF_) * nLevel * nGrowth / 100;

	setEudemonAttrib[6].dwAttributeType		= _EUDEMONATTRIB_ATKSPEED;
	setEudemonAttrib[6].dwAttributeData		= this->GetInt(ITEMDATA_ATKSPEED) * nLevel * nGrowth / 100;

	setEudemonAttrib[7].dwAttributeType		= _EUDEMONATTRIB_LIFE;
	setEudemonAttrib[7].dwAttributeData		= this->GetInt(ITEMDATA_LIFE) * nLevel * nGrowth / 100;

	CMsgEudemonAttrib	msg;
	if (msg.Create(_EA_ACTION_ATTRIB, this->GetID(), 8, setEudemonAttrib))
		pUser->SendMsg(&msg);

	return true;
}
*/
//////////////////////////////////////////////////////////////////////
bool CItem::WarGhostLevelReset()
{
    if (!IsEquipment())
    {
        return false;
    }
    if (GetInt(ITEMDATA_WARGHOSTEXP) < WARGHOSTVALUE_LIMIT)
    {
        return false;
    }
    SetInt(ITEMDATA_WARGHOSTEXP, WARGHOSTVALUE_LIMIT, true);
    return true;
}
//////////////////////////////////////////////////////////////////////

//升级战魂等级-------jinggy------2004-11-12
void CItem::WarGhostLevelUpgrade(CUserPtr pUser)
{
    if (!IsEquipment())
    {
        return ;
    }
    if (this->GetWarGhostLevel() >= MAX_LEVEL_WARLEVEL)
    {
        return;
    }
    //得到,设置该装备的 战魂经验值
    DWORD dwExp = GetInt(ITEMDATA_WARGHOSTEXP) ;
    SetInt(ITEMDATA_WARGHOSTEXP, ++dwExp, true);
    //通知客户端 设备属性更新
    CMsgItemInfo msg;
    IF_OK (msg.Create(this))
    pUser->SendMsg(&msg);
}
//判断是否是状态攻击类宝石
bool CItem::IsGemActiveAtk()
{
    DWORD dwType = GetInt(ITEMDATA_TYPE) % 100000 / 1000 * 1000;
    return (this->IsGhostGem() && (dwType == ITEMTYPE_GHOSTGEM_ACTIVE_ATK));
}

//是否是用来升级等级的宝石
bool CItem::IsGemForUpLevel()
{
    return GetInt(ITEMDATA_TYPE) == ITEMTYPE_GHOSTGEM_UPGRADE_EQUIPLEVEL;
}
//品质宝石
bool CItem::IsGemForUpQuality()
{
    return GetInt(ITEMDATA_TYPE) == ITEMTYPE_GHOSTGEM_FORQUALITY;
}
//追加宝石
bool CItem::IsGemForUpSuperAddition()
{
    return GetInt(ITEMDATA_TYPE) == ITEMTYPE_GHOSTGEM_FORGHOSTLEVEL;
}
//嵌入装备的宝石
bool CItem::IsGemForEmbedEquip()
{
    DWORD dwType = GetInt(ITEMDATA_TYPE) % 100000 / 1000 * 1000;
    return (this->IsGhostGem() && (dwType == ITEMTYPE_GHOSTGEM_EMBEDEQUIP));
}
//升级装备或武器的等级
bool CItem::UpLevel()
{
    if (this->IsShield() || this->IsArmor() || this->IsHelmet())
    {
        if (GetLevel() > MAX_LEVEL_EQUIP1)
        {
            return false;
        }
        if (!ChangeType(GetInt(ITEMDATA_TYPE) + 10))
        {
            return false;
        }
    }
    else
    {
        if (GetLevel() > MAX_LEVEL_EQUIP2)
        {
            return false;
        }
        if (!ChangeType(GetInt(ITEMDATA_TYPE) + 10))
        {
            return false;
        }
    }
    return true;
}
//升级品质
bool CItem::UpQuality()
{
    if (this->IsEquipment() || this->IsWeapon())
    {
        int nQuality = this->GetQuality() ;
        if (++nQuality > MAX_LEVEL_QUALITYANDADDITION)
        {
            return false;
        }
        int nType = GetInt(ITEMDATA_TYPE);
        IF_NOT( ChangeType(  ++ nType ) )
        {
            return false ;
        }
        SaveInfo();
        return true;
    }
    return false;
}
//升级追加
bool CItem::UpSuperAddition()
{
    if (this->IsEquipment() || this->IsWeapon())
    {
        int nEquipAddition = GetInt(ITEMDATA_ADDITION) ;
        int nEquipAdditionNew = nEquipAddition + 1 ;
        if (nEquipAdditionNew > MAX_LEVEL_QUALITYANDADDITION)
        {
            return false;
        }
        IF_NOT(ChangeAddition(nEquipAdditionNew) )
        return false ;
        SaveInfo();
        return true;
    }
    return false;
}
//降低 装备追加属性
bool CItem::DecSuperAddition()
{
    if (this->IsEquipment() || this->IsWeapon())
    {
        int nEquipAddition = GetInt(ITEMDATA_ADDITION) ;
        int nEquipAdditionNew = nEquipAddition - 1 ;
        if (nEquipAdditionNew < 0)
        {
            return false;
        }
        IF_NOT(ChangeAddition(nEquipAdditionNew) )
        return false ;
        SaveInfo();
        return true;
    }
    return false;
}
//得到战魂等级
int CItem::GetWarGhostLevel()
{
    DWORD dwExp = GetInt(ITEMDATA_WARGHOSTEXP) ;
    float fLevel =  powf((float)dwExp / (float)2, (float)1 / (float)3);
    return (int)fLevel;
}
//得到追加属性
int CItem::GetSuperAddition()
{
    return  GetInt(ITEMDATA_ADDITION) ;
}

void CItem::WarGhostLevelDec(int nDec)
{
    if (!IsEquipment())
    {
        return ;
    }
    //得到,设置该装备的 战魂经验值
    LONG  lExp = GetInt(ITEMDATA_WARGHOSTEXP) ;
    if (lExp <= WARGHOSTVALUE_LIMIT)
    {
        return ;
    }
    lExp -= nDec;
    if (lExp < WARGHOSTVALUE_LIMIT)
    {
        lExp = WARGHOSTVALUE_LIMIT;
    }
    SetInt(ITEMDATA_WARGHOSTEXP, lExp, true);
}
