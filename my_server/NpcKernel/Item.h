
//**********************************************************
// 代码编辑器
//**********************************************************

// Item.h: interface for the CItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEM_H__CA7CE885_1DCF_4B96_B308_FB7F40CCF4BA__INCLUDED_)
#define AFX_ITEM_H__CA7CE885_1DCF_4B96_B308_FB7F40CCF4BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyHeap.h"
#include "GameObj.h"
#include "ItemType.h"

//////////////////////////////////////////////////////////////////////
const int	ITEMREPAIR_PERCENT		= 200;				// 修理费为原价的200%
const int	ITEMIDENT_PERCENT		= 10;				// 鉴定费10%

// treasure
const OBJID TYPE_DRAGONBALL		= 1088000;
const OBJID TYPE_SHOOTINGSTAR	= 1088001;
const OBJID TYPE_DIVOICEITEM	= 1088002;

// gems
const OBJID GEM_WPNEXP_LOW	= 700051;
const OBJID GEM_WPNEXP_MID	= 700052;
const OBJID GEM_WPNEXP_HGT	= 700053;

const OBJID GEM_MGCEXP_LOW	= 700061;
const OBJID GEM_MGCEXP_MID	= 700062;
const OBJID GEM_MGCEXP_HGT	= 700063;

const OBJID GEM_EXP_LOW	= 700031;
const OBJID GEM_EXP_MID	= 700032;
const OBJID GEM_EXP_HGT	= 700033;

const OBJID GEM_DUR_LOW	= 700041;
const OBJID GEM_DUR_MID	= 700042;
const OBJID GEM_DUR_HGT	= 700043;

const OBJID GEM_DMG_LOW	= 700011;
const OBJID GEM_DMG_MID	= 700012;
const OBJID GEM_DMG_HGT	= 700013;

const OBJID GEM_HIT_LOW	= 700021;
const OBJID GEM_HIT_MID	= 700022;
const OBJID GEM_HIT_HGT	= 700023;

const OBJID GEM_MATK_LOW	= 700001;
const OBJID GEM_MATK_MID	= 700002;
const OBJID GEM_MATK_HGT	= 700003;

const OBJID	NORMAL_ARROW_TYPE	= 1050000;			// normal arrow itemtype

enum ITEMDATA
{
    ITEMDATA_ID_ = 0,
    ITEMDATA_TYPE = 1,
    ITEMDATA_OWNERID,
    ITEMDATA_PLAYERID,
    ITEMDATA_AMOUNT,
    ITEMDATA_AMOUNTLIMIT,
    ITEMDATA_IDENT,
    ITEMDATA_POSITION,
    ITEMDATA_GEM1,
    ITEMDATA_GEM2,
    ITEMDATA_MAGIC1,
    ITEMDATA_MAGIC2,
    ITEMDATA_MAGIC3,
    ITEMDATA_DATA,
    ITEMDATA_WARGHOSTEXP,		//圣战中的战魂经验值
    ITEMDATA_GEMTYPE,			//圣战中的技能（注入不同状态攻击类宝石实现）
    ITEMDATA_AVAILABLETIME,		//圣战中的技能 可利用的次数（注入不同状态攻击类宝石实现）
    ITEMDATA_DB_END,

    ITEMTYPEDATA_OFFSET = 100,		// 控制量，请勿使用
    ITEMDATA_NAME = 101,
    ITEMDATA_REQ_PROF,
    ITEMDATA_REQ_SKILL,
    ITEMDATA_REQ_LEVEL,
    ITEMDATA_REQ_SEX,
    ITEMDATA_REQ_FORCE,
    ITEMDATA_REQ_SPEED,
    ITEMDATA_REQ_HEALTH,
    ITEMDATA_REQ_SOUL,
    ITEMDATA_MONOPOLY,
    ITEMDATA_WEIGHT,
    ITEMDATA_PRICE,
    ITEMDATA_ACTION,
    ITEMDATA_ATTACK_MAX,
    ITEMDATA_ATTACK_MIN,
    ITEMDATA_DEFENSE,
    ITEMDATA_DEXTERITY,
    ITEMDATA_DODGE,
    ITEMDATA_LIFE,
    ITEMDATA_MANA,
    ITEMDATA_AMOUNT_ORIGINAL,
    ITEMDATA_AMOUNTLIMIT_ORIGINAL,
    ITEMDATA_IDENT_ORIGINAL,
    ITEMDATA_GEM1_ORIGINAL,
    ITEMDATA_GEM2_ORIGINAL,
    ITEMDATA_MAGIC1_ORIGINAL,
    ITEMDATA_MAGIC2_ORIGINAL,
    ITEMDATA_MAGIC3_ORIGINAL,
    ITEMDATA_MAGICATK,
    ITEMDATA_MAGICDEF,
    ITEMDATA_ATKRANGE,
    ITEMDATA_ATKSPEED,
};

//////////////////////////////////////////////////////////////////////
typedef int		ItemInfoStruct[ITEMDATA_DB_END];
/*struct ItemInfoStruct
{
	OBJID	id;
	OBJID	idType;
	OBJID	idOwner;
	OBJID	idPlayer;
	int		nAmount;
	int		nAmountLimit;
	int		nIdent;
	int		nPosition;
	int		nData;
	int		nGem1;
	int		nGem2;
	int		nMagic1;
	int		nMagic2;
	int		nMagic3;
};*/

//////////////////////////////////////////////////////////////////////
//	ITEMDATA_MONOPOLY,
const DWORD	MONOPOLY_MASK			= 0x01;
const DWORD	STORAGE_MASK			= 0x02;

const DWORD	IDENT_MASK				= 0x01;

const int	MAGIC_NONE				= 0;
const int	GEM_NONE				= 0;
const int	GEM_HOLE				= 255;

//////////////////////////////////////////////////////////////////////

const int	ITEMSORT_INVALID				=	-1;
const int	ITEMSORT_EXPEND					=	0;	// 易耗品
const int	ITEMSORT_HELMET					=	1;	// 头盔
const int	ITEMSORT_NECKLACE				=	2;	// 项链
const int	ITEMSORT_ARMOR					=	3;	// 盔甲
const int	ITEMSORT_WEAPON1				=	4;	// 单手武器
const int	ITEMSORT_WEAPON2				=	5;	// 双手武器
const int	ITEMSORT_SHIELD					=	6;	// 盾牌
const int	ITEMSORT_RING					=	7;	// 戒指
const int	ITEMSORT_SHOES					=	8;	// 鞋子
const int	ITEMSORT_OTHER					=	9;	// 其他, 不能直接使用

//////////////////////////////////////////////////////////////////////
const int	ITEMTYPE_INVALID			=	-1;	  // 非法
const int	ITEMTYPE_MEDICINE			=	00000;// 补药
const int	ITEMTYPE_POISON				=	10000;// 毒药
const int	ITEMTYPE_DART				=	20000;// 暗器
const int	ITEMTYPE_BOOK				=	30000;// 秘笈
const int	ITEMTYPE_NOTUSE				=	40000;// 不用
const int	ITEMTYPE_ARROW				=	50000;// 箭矢
const int	ITEMTYPE_SPELL				=	60000;// 符咒类
const int	ITEMTYPE_NOT_DIRECTUSE		=	70000;// 不可以双击使用的（如任务物品，宝石，木材等资源类，千位继续分类）
const int	ITEMTYPE_EX0				=	80000;// 扩展0
const int	ITEMTYPE_EX1				=	90000;// 扩展1

//////////////////////////////////////////////////////////////////////
const int	ITEMTYPE_GEM				=	00000;// 宝石物品
const int	ITEMTYPE_TASKITEM			=	10000;// 任务物品
const int	ITEMTYPE_ACTIONITEM			=	20000;// Action物品

//////////////////////////////////////////////////////////////////////
//2003-01-13 10:36:24  黄宇航
// Necklace type define
const int	ITEMTYPE_NECKLACE			=	00000;// 项链
const int	ITEMTYPE_SACHET				=	10000;// 香袋
const int	ITEMTYPE_AMULET				=	20000;// 护身符

//2003-01-13 10:46:37  黄宇航
// Ring type define
const int	ITEMTYPE_RING				=	00000;// 戒指
const int	ITEMTYPE_THUMB_RING			=	10000;// 扳指
const int	ITEMTYPE_BANGLE				=	20000;// 手镯

//2003-01-13 11:05:03  黄宇航
// Singlehand Weapon define
const int	SWEAPON_NONE				=	00000;// 空手
const int	SWEAPON_BLADE				=	10000;// 刀
const int	SWEAPON_AXE					=	20000;// 斧
const int	SWEAPON_HAMMER				=	30000;// 锤
const int	SWEAPON_HOOK				=	40000;// 钩
const int	SWEAPON_CLUB				=	50000;// 棒/杵
const int	SWEAPON_SWORD				=	60000;// 剑
const int	SWEAPON_CRUTCH				=	70000;// 拐
const int	SWEAPON_SCOURGE				=	80000;// 鞭/锏
const int	SWEAPON_SHORT				=	90000;// 短兵器

// Doublehand Weapon define
const int	DWEAPON_BOW					=	00000;// 弓
const int	DWEAPON_BLADE				=	10000;// 刀
const int	DWEAPON_AXE					=	20000;// 斧
const int	DWEAPON_HAMMER				=	30000;// 锤
const int	DWEAPON_HOOK				=	40000;// 钩
const int	DWEAPON_STAFF				=	50000;// 棍
const int	DWEAPON_SHOVEL				=	60000;// 铲
const int	DWEAPON_HALBERD				=	70000;// 戟
const int	DWEAPON_FORK				=	80000;// 叉
const int	DWEAPON_SPEAR				=	90000;// 枪

//////////////////////////////////////////////////////////////////////
//const int	ITEMPOSITION_BACKPACK	= 0;
const int	ITEMPOSITION_EQUIPBEGIN		= 1;
const int	ITEMPOSITION_HELMET		= 1;
const int	ITEMPOSITION_NECKLACE	= 2;
const int	ITEMPOSITION_ARMOR		= 3;
const int	ITEMPOSITION_WEAPONR	= 4;
const int	ITEMPOSITION_WEAPONL	= 5;
const int	ITEMPOSITION_RINGR		= 6;
const int	ITEMPOSITION_RINGL		= 7;
const int	ITEMPOSITION_SHOES		= 8;
const int	ITEMPOSITION_MOUNT		= 9;
const int	ITEMPOSITION_SPRITE		= 10;	// 精灵  -- add by zlong 2003-11-27
const int	ITEMPOSITION_MANTLE		= 11;	// 披风 -- zlong 2004-02-04
const int	ITEMPOSITION_EQUIPEND		= 12;

const int	ITEMPOSITION_PACK_BEGIN				= 50;
const int	ITEMPOSITION_BACKPACK			= 50;	// 普通物品背包
const int	ITEMPOSITION_GHOSTGEM_PACK		= 51;	// 魔魂宝石背包
const int	ITEMPOSITION_EUDEMONEGG_PACK	= 52;	// 幻兽蛋背包
const int	ITEMPOSITION_EUDEMON_PACK		= 53;	// 幻兽背包
const int	ITEMPOSITION_PACK_END				= 54;
const int	ITEMPOSITION_PACK_LIMIT			= 70;

const int	ITEMPOSITION_USER_LIMIT	= 199;
const int	ITEMPOSITION_STORAGE	= 201;
const int	ITEMPOSITION_TRUNK		= 202;
const int	ITEMPOSITION_CHEST		= 203;
const int	ITEMPOSITION_GROUND		= 254;			// 不存数据库
const int	ITEMPOSITION_NONE		= 255;			// 非法位置

//////////////////////////////////////////////////////////////////////
class CNpcType;
class CItem : CGameObj
{
protected:
    CItem();
    virtual ~CItem();
public:
    static CItem* CreateNew()		{ return (new CItem); }
    void	ReleaseByOwner()		{ delete this; }

    bool	Create(ItemInfoStruct* pInfo, int nPrice = 0);

public: // 分类
    bool	IsNormal()		{ return GetItemSort() == ITEMSORT_EXPEND; }
    bool	IsHelmet()		{ return GetItemSort() == ITEMSORT_HELMET; }
    bool	IsNecklace()	{ return GetItemSort() == ITEMSORT_NECKLACE; }
    bool	IsArmor()		{ return GetItemSort() == ITEMSORT_ARMOR; }
    bool	IsWeapon1()		{ return GetItemSort() == ITEMSORT_WEAPON1; }		// single hand use
    bool	IsWeapon2()		{ return GetItemSort() == ITEMSORT_WEAPON2; }		// two hand use
    bool	IsWeapon()		{ return GetItemSort() == ITEMSORT_WEAPON1 || GetItemSort() == ITEMSORT_WEAPON2; }
    bool	IsShield()		{ return GetItemSort() == ITEMSORT_SHIELD; }
    bool	IsRing()		{ return GetItemSort() == ITEMSORT_RING; }
    bool	IsShoes()		{ return GetItemSort() == ITEMSORT_SHOES; }
    bool	IsOther()		{ return GetItemSort() == ITEMSORT_OTHER; }
    bool	IsMedicine()	{ return IsNormal() && GetItemType() == ITEMTYPE_MEDICINE; }
    bool	IsDart()		{ return IsNormal() && GetItemType() == ITEMTYPE_DART; }
    bool	IsArrow()		{ return IsNormal() && GetItemType() == ITEMTYPE_ARROW; }
    bool	IsSpell()		{ return IsNormal() && GetItemType() == ITEMTYPE_SPELL; }
    bool	IsExpend()		{ return IsNormal() && GetItemType() < ITEMTYPE_NOT_DIRECTUSE; }
    bool	IsEquipment()	{ return GetItemSort() >= ITEMSORT_HELMET && GetItemSort() <= ITEMSORT_SHOES; }
    bool	IsTransSpell()	{ return false; }				//? 作废 GetInt(ITEMDATA_TYPE) == 107000099;
    bool	IsBow()			{ return IsWeapon2() && GetItemType() == DWEAPON_BOW; }
    bool	IsActionItem()	{ return IsOther() && GetItemType() == ITEMTYPE_ACTIONITEM; }
    bool	IsTaskItem()	{ return IsOther() && GetItemType() == ITEMTYPE_TASKITEM; }
    bool	IsGem()			{ return IsOther() && GetItemType() == ITEMTYPE_GEM; }
    bool	IsNonsuch()		{ return GetQuality() == 9; }

public: // static
    static bool	IsBow(int nType)		{ return ((nType % 10000000) / 100000) == ITEMSORT_WEAPON2 && ((nType % 100000) / 1000) * 1000 == DWEAPON_BOW; }
    static bool IsNonsuch(int nType)	{ return (nType % 10) == 9; }
    static bool IsCool8(int nType)		{ return (nType % 10) == 8; }
    static bool IsWeapon2(int nType)		{ return (nType % 10000000) / 100000 == 5; }

public:
    bool	IsHoldEnable()		{ return IsWeapon1() || IsWeapon2() || IsShield() || IsArrow(); }
    bool	IsEquipEnable()		{ return GetItemSort() >= ITEMSORT_HELMET && GetItemSort() <= ITEMSORT_SHOES || IsArrow(); }
    bool	IsEatEnable()		{ return IsExpend() && (GetItemType() == ITEMTYPE_MEDICINE || GetItemType() == ITEMTYPE_POISON); }
    bool	IsPileEnable()		{ return IsExpend() && GetInt(ITEMDATA_AMOUNTLIMIT) > 1; }
    bool	IsRepairEnable()	{ return !IsExpend() && GetInt(ITEMDATA_AMOUNTLIMIT) > 100; }
    //?	bool	IsExchangeEnable()	{ return (GetInt(ITEMDATA_MONOPOLY) & MONOPOLY_MASK) == 0; }
    //?	bool	IsStorageEnable()	{ return (GetInt(ITEMDATA_MONOPOLY) & STORAGE_MASK) == 0; }
    bool	IsNeedIdent()		{ return (GetInt(ITEMDATA_IDENT) & IDENT_MASK) != 0; }
    bool	IsNonsuchItem();	//	{ return (GetInt(ITEMDATA_TYPE)%10) >= 8; }
    bool	IsNormalArrow()		{ return GetInt(ITEMDATA_TYPE) == NORMAL_ARROW_TYPE; }

public: // const
    OBJID	GetID()				{ return m_info[0]; }
    int		GetInt(ITEMDATA idx)	{ if (idx > ITEMTYPEDATA_OFFSET) { return GetTypeInt(idx); } ASSERT(idx >= 0 && idx < ITEMDATA_DB_END); return m_info[idx]; }
    void	SetInt(ITEMDATA idx, int nData, bool = 0)	{ ASSERT(idx >= 0 && idx < ITEMDATA_DB_END); m_info[idx] = nData; }
    int		CalcRepairMoney();
    DWORD	GetSellPrice();
    int		GetNumber()			{ if (IsExpend()) { return GetInt(ITEMDATA_AMOUNT); } return 1; }		// 物品的数量，非易耗品返回1
    int		GetTypeInt(ITEMDATA idx);

public: // modify attribute
    void	SetIdent(bool bNeedIdent, bool bUpdate)
    {
        if (bNeedIdent) { SetInt(ITEMDATA_IDENT, GetInt(ITEMDATA_IDENT) | IDENT_MASK, bUpdate); }
        else { SetInt(ITEMDATA_IDENT, GetInt(ITEMDATA_IDENT)&(~IDENT_MASK), bUpdate); }
    }

public: // gem effect
    int		GetGemDmgEffect		(void);
    int		GetGemMgcAtkEffect	(void);
    int		GetGemHitRateEffect	(void);
    int		GetGemExpEffect		(void);
    int		GetGemWpnExpEffect	(void);
    int		GetGemMgcExpEffect	(void);
    int		GetGemDurEffect		(OBJID idGemType = ID_NONE);

public: // application
    CItem*	Split(int nNum);
    bool	RecoverDur			(void);
    bool	ChangeType	(OBJID idNewType);
    bool	ChangeColor	(int nColor);
protected:
    CItem*	Clone();		// 新物品存数据库

public: // const
    bool UpdateInfo(ItemInfoStruct* pInfo);
    int		GetItemSubType();
    int		GetItemSort();
    int		GetLevel	(void);
    int		GetQuality	(void)	{ return (GetInt(ITEMDATA_TYPE) % 10); }
    int		GetRecoverDurCost	(void);
    //	bool	GetInfo(ItemInfoStruct* pInfo)		{ memcpy(pInfo, &m_info, sizeof(ItemInfoStruct)); return true; }
protected:
    int		GetItemType();

protected:
    ItemInfoStruct	m_info;
    CItemTypeData*	m_pType;

protected: // ctrl
    MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_ITEM_H__CA7CE885_1DCF_4B96_B308_FB7F40CCF4BA__INCLUDED_)
