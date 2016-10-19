
//**********************************************************
// 代码编辑器
//**********************************************************

// Item.cpp: implementation of the CItem class.
//
//////////////////////////////////////////////////////////////////////

#include "windows.h"
#include "define.h"
#include "Item.h"
#include "ItemType.h"
#include "NpcType.h"
#include "NpcWorld.h"
#include "i_mydb.h"

MYHEAP_IMPLEMENTATION(CItem, s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CItem::CItem()
{
    SetObjType(OBJ_ITEM);
    m_pType	= NULL;
}

//////////////////////////////////////////////////////////////////////
CItem::~CItem()
{
}

//////////////////////////////////////////////////////////////////////
bool CItem::Create(ItemInfoStruct* pInfo, int nPrice/*=0*/)
{
    memcpy(&m_info, pInfo, sizeof(ItemInfoStruct));
    return true;
}

/*/////////////////////////////////////////////////////////////////////
int	CItem::CalcRepairMoney()
{
	int nQualityParam = 10;
	int nQuality = this->GetInt(ITEMDATA_TYPE)%10;
	switch (nQuality)
	{
	case 6:
		nQualityParam = 11;
		break;

	case 7:
		nQualityParam = 12;
		break;

	case 8:
		nQualityParam = 13;
		break;

	case 9:
		nQualityParam = 15;
		break;

	default:
		nQualityParam = 10;
		break;
	}

	// the highest price is about million.
	int nRecoverDurability = __max(0, GetInt(ITEMDATA_AMOUNTLIMIT) - GetInt(ITEMDATA_AMOUNT));
	int nRepairCost = ::MulDiv(GetInt(ITEMDATA_PRICE), nRecoverDurability*3, 4*GetInt(ITEMDATA_AMOUNTLIMIT));
	nRepairCost = ::MulDiv(nRepairCost, nQualityParam, 10);

	return nRepairCost;
}

//////////////////////////////////////////////////////////////////////
DWORD CItem::GetSellPrice()
{
	if (GetInt(ITEMDATA_AMOUNTLIMIT) == 0)
		return 0;
	CHECKF(GetInt(ITEMDATA_AMOUNTLIMIT) > 0);
	DWORD dwPrice = MulDiv(GetInt(ITEMDATA_PRICE)/3, GetInt(ITEMDATA_AMOUNT), GetInt(ITEMDATA_AMOUNTLIMIT));
	return dwPrice;
}
*/
//////////////////////////////////////////////////////////////////////
int	CItem::GetItemSort()
{
    //取百万+十万位
    int nType = ITEMSORT_INVALID;
    switch((GetInt(ITEMDATA_TYPE) % 10000000) / 100000)
    {
    case 10:
        nType = ITEMSORT_EXPEND;
        break;
    case 1:
        {
            ////取十万位+万位
            switch((GetInt(ITEMDATA_TYPE) % 1000000) / 10000)
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
    case 7:
        nType = ITEMSORT_OTHER;
        break;
    case 9:
        nType = ITEMSORT_SHIELD;
        break;
    }
    return nType;
}

//////////////////////////////////////////////////////////////////////
int	CItem::GetItemType()
{
    int nType = ITEMTYPE_INVALID;
    //取百万+十万位
    switch((GetInt(ITEMDATA_TYPE) % 10000000) / 100000)
    {
    case 7:
    case 10:
        nType = ((GetInt(ITEMDATA_TYPE) % 100000) / 10000) * 10000;
        break;
    case 4:
    case 5:
        nType = ((GetInt(ITEMDATA_TYPE) % 100000) / 1000) * 1000;
        break;
    default:
        break;
    }
    return nType;
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
    if (this->IsExpend())
    {
        return false;
    }
    // high quality
    if ((GetInt(ITEMDATA_TYPE) % 10) >= 8)
    {
        return true;
    }
    // dragon ball
    if (TYPE_DRAGONBALL == this->GetInt(ITEMDATA_TYPE))
    {
        return true;
    }
    // precious gem
    if (this->IsGem() && (GetInt(ITEMDATA_TYPE) % 10) >= 2)
    {
        return true;
    }
    // ....
    int nGem1 = this->GetInt(ITEMDATA_GEM1) % 10;
    int nGem2 = this->GetInt(ITEMDATA_GEM2) % 10;
    bool bIsNonsuch = false;
    switch(this->GetItemSort())
    {
    case ITEMSORT_WEAPON1:
    case ITEMSORT_WEAPON2:
        {
            if ((nGem1 != GEM_HOLE && nGem1 >= 2)
                    || (nGem2 != GEM_HOLE && nGem2 >= 2))
            {
                bIsNonsuch = true;
            }
        }
        break;
    case ITEMSORT_HELMET:
    case ITEMSORT_NECKLACE:
    case ITEMSORT_ARMOR:
    case ITEMSORT_SHIELD:
    case ITEMSORT_RING:
    case ITEMSORT_SHOES:
        {
            if (nGem1 != GEM_NONE || nGem2 != GEM_NONE)
            {
                bIsNonsuch = true;
            }
        }
        break;
    default:
        break;
    }
    return bIsNonsuch;
}

/*/////////////////////////////////////////////////////////////////////
bool CItem::RecoverDur	(void)
{
	int nMaxDur = this->GetInt(ITEMDATA_AMOUNTLIMIT_ORIGINAL);
	nMaxDur += nMaxDur*this->GetGemDurEffect()/100;

	this->SetInt(ITEMDATA_AMOUNTLIMIT, nMaxDur, true);
	return true;
}

//////////////////////////////////////////////////////////////////////
int CItem::GetRecoverDurCost	(void)
{
	int nRealMaxDur = this->GetInt(ITEMDATA_AMOUNTLIMIT)*100/(100+this->GetGemDurEffect());
	if (nRealMaxDur >= this->GetInt(ITEMDATA_AMOUNTLIMIT_ORIGINAL))
		return 0;

	int nRepairDur = this->GetInt(ITEMDATA_AMOUNTLIMIT_ORIGINAL)-nRealMaxDur;
	int nCost = 0;
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

	return nCost;
}
*/
bool CItem::UpdateInfo(ItemInfoStruct* pInfo)
{
    CHECKF(pInfo && (*pInfo)[ITEMDATA_ID_] == GetInt(ITEMDATA_ID_) && (*pInfo)[ITEMDATA_POSITION] == GetInt(ITEMDATA_POSITION));
    memcpy(&m_info, pInfo, sizeof(ItemInfoStruct));
    return true;
}

int CItem::GetTypeInt(ITEMDATA idx)
{
    if (!m_pType)
    {
        m_pType = NpcWorld()->GetItemType()->QueryItemType(m_info[ITEMDATA_TYPE]);
    }
    CHECKF(m_pType);
    return m_pType->GetInt((ITEMTYPEDATA)(idx - ITEMTYPEDATA_OFFSET));
}
