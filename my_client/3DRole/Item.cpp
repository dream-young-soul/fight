
//**********************************************************
// 代码编辑器
//**********************************************************

//////////////////////////////////////////////////////////////////////
#include "basefunc.h"
#include "item.h"
#include "3DRoleData.h"
#include "3DGameMap.h"
#include "Hero.h"

CItemData CItem::s_ItemData;

//-----------------------------------------------------------------
CItem::CItem()
{
    m_nMaxAttackModify		= 0;
    m_nMinAttackModify		= 0;
    m_nDefenceModify		= 0;
    m_nDexterityModify		= 0;
    m_nDodgeModify			= 0;
    m_nMagicAttackModify	= 0;
    m_nMagicDefenceModify	= 0;
    m_nFrameIndex			= 0;
    m_dwWarGhostExp			= 0;	// 战魂经验 //幻兽当前生命
    m_dwGemAtkType			= 0;		// 宝石类型	//幻兽忠诚
    m_dwAvailabeTime		= 0;	// 使用次数
    m_dwGrow				= 0;			//成长率
    m_bMonsterDie			= FALSE;
    strcpy(m_szMonsterName, "");
}
//-----------------------------------------------------------------
CItem::~CItem()
{
}
//-----------------------------------------------------------------
unsigned short	CItem::GetWeight()
{
    if (this->GetTypeID() == _ARROW || this->GetTypeID() == _DART)
    {
        return (this->GetAmount() - 1) * m_infoType.usWeight / 100 + 1;
    }
    if (this->GetSort() == ITEMSORT_EXPEND)
    {
        return m_infoType.usWeight * m_infoType.usAmount;
    }
    return m_infoType.usWeight;
}
//-----------------------------------------------------------------

BOOL CItem::Create(OBJID idItem, OBJID idItemType)
{
    if (idItem == ID_NONE || idItemType == ID_NONE)
    {
        return false;
    }
    if (!s_ItemData.GetItemTypeInfo(idItemType, m_infoType))
    {
        return false;
    }
    m_idItem = 	idItem;
    m_usOriginalAmountLimit = this->GetAmountLimit();
    m_usOriginalAmount = this->GetAmount();
    strcpy(m_szMonsterName, "");
    return true;
}
//-----------------------------------------------------------------
int	CItem::GetSort()
{
    return CItem::ItemGetSort(m_infoType.uID);
}

const char* CItem::GetSexRequiriedStr()
{
	return this->GetSexRequried() == 1 ? g_objGameDataSet.GetStr(10031): g_objGameDataSet.GetStr(10030);

}

//-----------------------------------------------------------------
int	CItem::GetSubType()
{
    return CItem::ItemGetSubType(m_infoType.uID);
}
//-----------------------------------------------------------------
DWORD CItem::GetSellPrice()
{
    if (this->GetAmountLimit() == 0)
    {
        return 0;
    }
    int nAmount = this->GetAmount();
    if (nAmount > this->GetAmountLimit())
    {
        nAmount = this->GetAmountLimit();
    }
    DWORD dwPrice = this->GetPrice();
    dwPrice = dwPrice + MulDiv(dwPrice, this->GetQuality() * 5, 100)
              + MulDiv(dwPrice, (this->GetAddition() / 10) * 5, 100)
              + MulDiv(dwPrice, this->GetLucky() * 5, 100);
    dwPrice = MulDiv(dwPrice / 3, nAmount, this->GetOriginalAmount());
    const int UNIDENT_SALE_FEE = 1;
    if (this->TestStatus(_ITEM_STATUS_NOT_IDENT))
    {
        dwPrice = UNIDENT_SALE_FEE;
    }
    if (this->IsArrow())
    {
        dwPrice = 0;
    }
    return dwPrice;
}

//-----------------------------------------------------------------
DWORD CItem::GetRepairCost()
{
    if ( this->GetAmountLimit() == this->GetAmount())
    {
        return 0;
    }
    int nRecoverDurability = __max(0,  this->GetAmountLimit() - this->GetAmount());
    int nRepairCost	= 0;
    if (this->GetAmountLimit() > 0)
    {
        nRepairCost	= this->GetPrice() * nRecoverDurability / this->GetAmountLimit() / 2;
    }
    return __max(1, nRepairCost);	// 最低修理价格为1
}

//-----------------------------------------------------------------
DWORD CItem::GetWeaponSkillType ()
{
    if (ITEMSORT_WEAPON_SINGLE_HAND != this->GetSort()
            && ITEMSORT_WEAPON_DOUBLE_HAND != this->GetSort()
            && ITEMSORT_SHIELD != this->GetSort())
    {
        return 0;
    }
    return (this->GetTypeID() % 1000000) / 1000;
}

//-----------------------------------------------------------------
int CItem::ItemGetSort (OBJID idType)
{
    //取百万+十万位
    int nType = ITEMSORT_INVALID;
    switch((idType % 10000000) / 100000)
    {
    case 10:	
        nType = ITEMSORT_EXPEND;
        break;
    case 1:
        {
            ////取十万位+万位
            switch((idType % 1000000) / 10000)
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
       /*     case 14:
                nType = ITEMSORT_TREASURE;*/
                break;
            case 15:
                nType = ITEMSORT_RING;
                break;
            case 16:
                nType = ITEMSORT_SHOES;
                break;
			case 19:
				{
					nType = ITEMSORT_TREASURE;
					break;
				}
            }
        }
        break;
    case 4:
        nType = ITEMSORT_WEAPON_SINGLE_HAND;
        break;
    case 5:
        nType = ITEMSORT_WEAPON_DOUBLE_HAND;
        break;
    case 6: // 曾松要求的改动
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

//-----------------------------------------------------------------
int CItem::ItemGetSubType(OBJID idType)
{
    int nType = EXPEND_INVALID;
    switch((idType % 10000000) / 100000)
    {
    case 7:
    case 10:
        nType = (idType % 100000);
        break;
    case 4:
    case 5:
        nType = (idType % 100000);
        break;
    }
    return nType;
}

//-----------------------------------------------------------------
unsigned short CItem::GetMagicDefence()
{
    return m_infoType.usMagicDefence;
}
//-----------------------------------------------------------------
char* CItem::GetDesc()
{
    return m_infoType.szDesc;
}
//-----------------------------------------------------------------
void	CItem::SetBoothSellPrice(DWORD dwPrice)
{
    m_dwSellPrice = dwPrice;
}
//-----------------------------------------------------------------
DWORD	CItem::GetBoothSellPrice()
{
    return m_dwSellPrice;
}
//-----------------------------------------------------------------
void	CItem::SetOwnerID(OBJID idOwner)
{
    m_idOwner = idOwner;
}
//-----------------------------------------------------------------
OBJID	CItem::GetOwnerID()
{
    return m_idOwner;
}
//-----------------------------------------------------------------
int		CItem::GetMaxAttackModify()
{
    return m_nMaxAttackModify;
}
//-----------------------------------------------------------------
int		CItem::GetMinAttackModify()
{
    return m_nMinAttackModify;
}
//-----------------------------------------------------------------
int		CItem::GetDefenceModify()
{
    return m_nDefenceModify;
}
//-----------------------------------------------------------------
int		CItem::GetDexterityModify()
{
    return m_nDexterityModify;
}
//-----------------------------------------------------------------
int		CItem::GetDodgeModify()
{
    return m_nDodgeModify;
}
//-----------------------------------------------------------------
int		CItem::GetMagicAttackModify()
{
    return m_nMagicAttackModify;
}
//-----------------------------------------------------------------
int		CItem::GetMagicDefenceModify()
{
    return m_nMagicDefenceModify;
}
//-----------------------------------------------------------------
BOOL	CItem::IsNonsuchItem(OBJID idItemType)
{
    if ((idItemType / 100000 ) < 10 && !( idItemType >= 700000 && idItemType <= 800000) && idItemType % 10 == 9)
    {
        return true;
    }
    return false;
}
//-----------------------------------------------------------------
void	CItem::SetStatus(unsigned char ucStatus)
{
    m_infoType.ucStatus = ucStatus;
}
//-----------------------------------------------------------------
unsigned char	CItem::GetStatus()
{
    return m_infoType.ucStatus;
}
//-----------------------------------------------------------------
BOOL	CItem::TestStatus(unsigned char ucStatus)
{
    if ( m_infoType.ucStatus == 0  && ucStatus == 0)
    {
        return true;
    }
    return m_infoType.ucStatus & ucStatus;
}
//-----------------------------------------------------------------
BOOL  CItem::TestTarget(unsigned short usTarget)
{
    BOOL bFlag = FALSE;
    if (usTarget == TARGET_NONE)
    {
        if (this->GetTarget() == TARGET_NONE)
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        return ((this->GetTarget()&usTarget) == usTarget);
    }
}
//-----------------------------------------------------------------
BOOL CItem::TestMonopolyItem(unsigned char ucMonopoly)
{
    BOOL bFlag = FALSE;
    if (ucMonopoly == 0)
    {
        if (this->GetMonopoly() == 0)
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        return ((this->GetMonopoly()&ucMonopoly) == ucMonopoly);
    }
}
//-----------------------------------------------------------------
void	CItem::ShowMiniItem(int nX, int nY , BOOL bScale)
{
    char szAni[256] = "";
    sprintf(szAni, "%u", this->GetTypeID());
    CAni* pAni = g_objGameDataSet.GetItemMinIcon(this->GetTypeID());
    if (!pAni)
    {
        return;
    }
    CMyBitmap* pBmp = pAni->GetFrame(0);
    if (!pBmp)
    {
        return;
    }
    CMySize sizeBmp;
    pBmp->GetSize(sizeBmp.iWidth, sizeBmp.iHeight);
    if (bScale)
    {
        pBmp->ShowEx(nX, nY , NULL, sizeBmp.iWidth / 2, sizeBmp.iHeight / 2);
    }
    else
    {
        pBmp->ShowEx(nX, nY , NULL, sizeBmp.iWidth, sizeBmp.iHeight);
    }
}
//-----------------------------------------------------------------
int	CItem::GetQuality(void)
{
    return (this->GetTypeID() / 1000000000) * 10 + (this->GetTypeID() % 10);
}
//-----------------------------------------------------------------
BOOL CItem::WantCombin()
{
    OBJID idSet[11] =
    {
        1010000,
        1010010,
        1010020,
        1010030,
        1010040,
        1011000,
        1011010,
        1011020,
        1011030,
        1011040,
        1020001,
    };
    for(int i = 0; i < 11; i ++)
    {
        if (idSet[i] == this->GetTypeID())
        {
            return true;
        }
    }
    return false;
}
//-----------------------------------------------------------------
int	CItem::GetPackageType()
{
    if (this->GetSort() == ITEMSORT_EXPEND)
    {
        if (this->GetSubType() / 1000 * 1000 == ITEMTYPE_GHOSTGEM_ACTIVE_ATK ||
                this->GetSubType() / 1000 * 1000 == ITEMTYPE_GHOSTGEM_PASSIVE_ATK ||
                this->GetSubType() / 1000 * 1000 == ITEMTYPE_GHOSTGEM_EUDEMON ||
                this->GetSubType() / 1000 * 1000 == ITEMTYPE_GHOSTGEM_RELEASE ||
                this->GetSubType() / 1000 * 1000 == ITEMTYPE_GHOSTGEM_TRACE ||
                this->GetSubType() / 1000 * 1000 == ITEMTYPE_GHOSTGEM_PROTECTIVE ||
                this->GetSubType() / 1000 * 1000 == ITEMTYPE_GHOSTGEM_SPECIAL	||
                this->GetSubType() / 1000 * 1000 == ITEMTYPE_GHOSTGEM_INVITE)
        {
            return PACKAGE_SOUL;
        }
        if ((this->GetSubType() / 10000 ) * 10000 == ITEMTYPE_EUDEMON)
        {
            return PACKAGE_PET;
        }
        else if ((this->GetSubType() / 10000) * 10000 == ITEMTYPE_EUDEMONEGG)
        {
            // 蛋...
            return PACKAGE_EGG;
        }
        else
        {
            return PACKAGE_BAG;
        }
    }
    else
    {
        return PACKAGE_BAG;
    }
}
//-----------------------------------------------------------------
int	CItem::GetMonsterMaxAtk()
{
    if (this->GetAddition())
    {
        ItemAdditionInfo pInfo ;
        if (CItem::s_ItemData.GetItemAdditionInfo(this->GetTypeID(), this->GetAddition(), pInfo))
        {
            return this->GetAmountLimit() * (this->GetMagicDefence() * this->GetMonsterGrow() + pInfo.nAttackMax * 100) / 10000;
        }
    }
    return this->GetAmountLimit() * this->GetMaxAttack() * this->GetMonsterGrow() / 10000;
}

int	CItem::GetMonsterMinAtk()
{
    if (this->GetAddition())
    {
        ItemAdditionInfo pInfo ;
        if (CItem::s_ItemData.GetItemAdditionInfo(this->GetTypeID(), this->GetAddition(), pInfo))
        {
            return this->GetAmountLimit() * (this->GetMagicDefence() * this->GetMonsterGrow() + pInfo.nAttackMin * 100) / 10000;
        }
    }
    return this->GetAmountLimit() * this->GetMinAttack() * this->GetMonsterGrow() / 10000;
}

int	CItem::GetMonsterMagicMaxAtk()
{
    if (this->GetAddition())
    {
        ItemAdditionInfo pInfo ;
        if (CItem::s_ItemData.GetItemAdditionInfo(this->GetTypeID(), this->GetAddition(), pInfo))
        {
            return this->GetAmountLimit() * (this->GetMagicDefence() * this->GetMonsterGrow() + pInfo.nMAttackMax * 100) / 10000;
        }
    }
    return this->GetAmountLimit() * this->GetMaxMagicAttack() * this->GetMonsterGrow() / 10000;
}

int	CItem::GetMonsterMagicMinAtk()
{
    if (this->GetAddition())
    {
        ItemAdditionInfo pInfo ;
        if (CItem::s_ItemData.GetItemAdditionInfo(this->GetTypeID(), this->GetAddition(), pInfo))
        {
            return this->GetAmountLimit() * (this->GetMagicDefence() * this->GetMonsterGrow() + pInfo.nMAttackMin * 100) / 10000;
        }
    }
    return this->GetAmountLimit() * this->GetMinMagicAttack() * this->GetMonsterGrow() / 10000;
}

int	CItem::GetMonsterMagicDef()
{
    if (this->GetAddition())
    {
        ItemAdditionInfo pInfo ;
        if (CItem::s_ItemData.GetItemAdditionInfo(this->GetTypeID(), this->GetAddition(), pInfo))
        {
            return this->GetAmountLimit() * (this->GetMagicDefence() * this->GetMonsterGrow() + pInfo.nMDef * 100) / 10000;
        }
    }
    return this->GetAmountLimit() * this->GetMagicDefence() * this->GetMonsterGrow() / 10000;
}

int	CItem::GetMonsterDef()
{
    if (this->GetAddition())
    {
        ItemAdditionInfo pInfo ;
        if (CItem::s_ItemData.GetItemAdditionInfo(this->GetTypeID(), this->GetAddition(), pInfo))
        {
            return this->GetAmountLimit() * (this->GetMagicDefence() * this->GetMonsterGrow() + pInfo.nDefense * 100) / 10000;
        }
    }
    return this->GetAmountLimit() * this->GetDefense() * this->GetMonsterGrow() / 10000;
}

int	CItem::GetMonsterMaxLife()
{
    if (this->GetAddition())
    {
        ItemAdditionInfo pInfo ;
        if (CItem::s_ItemData.GetItemAdditionInfo(this->GetTypeID(), this->GetAddition(), pInfo))
        {
            return this->GetAmountLimit() * (this->GetMagicDefence() * this->GetMonsterGrow() + pInfo.nLife * 100) / 10000;
        }
    }
    return this->GetAmountLimit() * (this->GetLife() * this->GetMonsterGrow() + this->GetAddition() * 100) / 10000;
}

char* CItem::GetMonsterName()
{
    if (m_szMonsterName && strlen(m_szMonsterName) > 0)
    {
        return m_szMonsterName;
    }
    else
    {
        return this->GetName();
    }
}

DWORD CItem::IsMonsterCanEvolve(int nMode)
{
    if (this->GetPackageType() != PACKAGE_PET)
    {
        return 0;
    }
    ItemTypeInfo pItemInfo;
    DWORD idTypeAfter = 0;
    switch(nMode)
    {
    case 1:		//一次进化 --圣兽
        if (this->GetTypeID() % 100 == 0 && s_ItemData.GetItemTypeInfo(this->GetTypeID() + 1, pItemInfo))
        {
            idTypeAfter = this->GetTypeID() + 1;
        }
        break;
    case 2:		//一次进化 --魔兽
        if (this->GetTypeID() % 100 == 0 && s_ItemData.GetItemTypeInfo(this->GetTypeID() + 2, pItemInfo))
        {
            idTypeAfter = this->GetTypeID() + 2;
        }
        break;
    case 3:		//二次进化 --圣兽
        if (this->GetTypeID() % 100 == 1 && s_ItemData.GetItemTypeInfo(this->GetTypeID() + 10, pItemInfo))
        {
            idTypeAfter = this->GetTypeID() + 10;
        }
        break;
    case 4:
        if (this->GetTypeID() % 100 == 1 && s_ItemData.GetItemTypeInfo(this->GetTypeID() + 20, pItemInfo))
        {
            idTypeAfter = this->GetTypeID() + 20;
        }
        break;
    case 5:
        if (this->GetTypeID() % 100 == 1 && s_ItemData.GetItemTypeInfo(this->GetTypeID() + 30, pItemInfo))
        {
            idTypeAfter = this->GetTypeID() + 30;
        }
        break;
    case 6:
        if (this->GetTypeID() % 100 == 1 && s_ItemData.GetItemTypeInfo(this->GetTypeID() + 40, pItemInfo))
        {
            idTypeAfter = this->GetTypeID() + 40;
        }
        break;
    case 7:		//二次进化 --魔兽
        if (this->GetTypeID() % 100 == 2 && s_ItemData.GetItemTypeInfo(this->GetTypeID() + 50, pItemInfo))
        {
            idTypeAfter = this->GetTypeID() + 50;
        }
        break;
    case 8:
        if (this->GetTypeID() % 100 == 2 && s_ItemData.GetItemTypeInfo(this->GetTypeID() + 60, pItemInfo))
        {
            idTypeAfter = this->GetTypeID() + 60;
        }
        break;
    case 9:
        if (this->GetTypeID() % 100 == 2 && s_ItemData.GetItemTypeInfo(this->GetTypeID() + 70, pItemInfo))
        {
            idTypeAfter = this->GetTypeID() + 70;
        }
        break;
    case 10:
        if (this->GetTypeID() % 100 == 2 && s_ItemData.GetItemTypeInfo(this->GetTypeID() + 80, pItemInfo))
        {
            idTypeAfter = this->GetTypeID() + 80;
        }
        break;
    default:
        break;
    }
    return idTypeAfter;
}
