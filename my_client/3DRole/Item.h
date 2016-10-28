
//**********************************************************
// 代码编辑器
//**********************************************************

#ifndef _ITEM_H
#define _ITEM_H

#include "basefunc.h"
#include "ItemData.h"
//#include "3DSimpleObj.h"
#include "2DMapItem.h"
#include "IItemPackage.h"

const int _ARROW	=	170001;	// 箭
const int _DART		=	171001;	// 弩箭

// monopoly define 独占物品定义
const int _NOT_MONOPOLY_NOT_DISAPPEAR	=	0;//0：非独占，一般物品
const int _NOT_MONOPOLY_DISAPPEAR		=	1;//1：非独占，离开身体消失 可以寄存,不可交易
const int _MONOPOLY_NOT_DISAPPEAR		=	2;//2：独占－不可以寄存，离开身体不消失
const int _MONOPOLY_DISAPPEAR			=	3;//3：独占－不可以寄存，离开身体消失
const int _MONOPOLY_DROPCONFIRM			=   4;//4: 丢弃提示-丢弃时客户端跳出确认窗口，提示内容：请确定要丢弃
const int _MONOPOLY_SELLCONFIRM			=   8;//8:出售提示-出售给商店时客户端跳出确认窗口，提示内容：请确定要出售
const int _MONOPOLY_NOTDROP				=  16;//16:死亡不会掉落
const int _MONOPOLY_NOT_SELL			=  32;//32:不可出售

const int _MAX_EQUIPMENT				=	11;	// 不能修正成7，否则位置错乱

// requre sex define
const unsigned char _MAN_USEABLE		=	0x01;// 男的可用
const unsigned char _WOMAN_USEABLE		=	0x02;// 女的可用
const unsigned char _EUNUCH_USEABLE		=	0x04;// 太监可用

// Item Position define
//const int	ITEMPOSITION_BACKPACK	=	0;	// 背包
const int	ITEMPOSITION_HELMET		=	1;	// 头盔
const int	ITEMPOSITION_NECKLACE	=	2;	// 项链
const int	ITEMPOSITION_ARMOR		=	3;	// 盔甲
const int	ITEMPOSITION_WEAPONR	=	4;	// 右武器
const int	ITEMPOSITION_WEAPONL	=	5;	// 左武器
const int	ITEMPOSITION_RINGR		=	6;	// 右戒指
const int	ITEMPOSITION_TREASURE	=	7;	// 宝物
const int	ITEMPOSITION_SHOES		=	8;	// 鞋子
const int	ITEMPOSITION_MOUNT		=	9;	// 坐骑
const int	ITEMPOSITION_SPRITE		=	10;	// 精灵
const int ITEMPOSITION_MANTLE		=	11; // 披风

const int ITEMPOSITION_BACKPACK			=	50; // 普通物品背包
const int ITEMPOSITION_GHOSTGEM_PACK	=	51; // 魔魂宝石背包
const int ITEMPOSITION_EUDEMONEGG_PACK	=	52; // 幻兽蛋背包
const int ITEMPOSITION_EUDEMON_PACK		=	53; // 幻兽背包


const int	ITEMPOSITION_USER_LIMIT	=	199;// 身上物品分界
const int	ITEMPOSITION_STORAGE	=	201;// 寄存处
const int	ITEMPOSITION_TRUNK		=	202;// 箱子
const int	ITEMPOSITION_CHEST		=	203;// 百宝囊
const int   ITEMPOSITION_AUCTION_STORAGE = 207; //拍卖

const int	ITEMPOSITION_GROUND		=	254;// 地上
const int	ITEMPOSITION_NONE		=	255;// 非法位置

// Type Define
const int	ITEMSORT_INVALID				=	-1;
const int	ITEMSORT_EXPEND					=	0;	// 易耗品
const int	ITEMSORT_HELMET					=	1;	// 头盔
const int	ITEMSORT_NECKLACE				=	2;	// 项链
const int	ITEMSORT_ARMOR					=	3;	// 盔甲
const int	ITEMSORT_WEAPON_SINGLE_HAND		=	4;	// 单手武器
const int	ITEMSORT_WEAPON_DOUBLE_HAND		=	5;	// 双手武器
const int	ITEMSORT_SHIELD					=	6;	// 盾牌
const int	ITEMSORT_RING					=	7;	// 戒指
const int	ITEMSORT_SHOES					=	8;	// 鞋子
const int	ITEMSORT_OTHER					=	9;	// 其他, 不能直接使用
const int	ITEMSORT_MOUNT					=	10;	// 坐骑
const int	ITEMSORT_SPRITE					=	11;	// 精灵
const int	ITEMSORT_TREASURE				=	12; // 珠宝

// money define
const int	ITEMTYPEMONEY_MIN			= 1090000;	// 最小堆钱
const int	ITEMTYPEMONEY_MAX			= 1091020;	// 最大堆钱

const int	SMALLHEAPMONEY_LIMIT		= 10;		// 小堆钱
const int	SMALLHEAPMONEY_TYPE			= 1090000;	// 小堆钱ITEMTYPE
const int	MIDDLEHEAPMONEY_LIMIT		= 100;		// 中堆钱
const int	MIDDLEHEAPMONEY_TYPE		= 1090010;	// 中堆钱ITEMTYPE
const int	BIGHEAPMONEY_LIMIT			= 1000;		// 大堆钱
const int	BIGHEAPMONEY_TYPE			= 1090020;	// 大堆钱ITEMTYPE

const int	SMALLHEAPGOLD_LIMIT			= 2000;		// 小堆金子
const int	SMALLHEAPGOLD_TYPE			= 1091000;	// 小堆金子ITEMTYPE
const int	MIDDLEHEAPGOLD_LIMIT		= 5000;		// 中堆金子
const int	MIDDLEHEAPGOLD_TYPE			= 1091010;	// 中堆金子ITEMTYPE
const int	BIGHEAPGOLD_LIMIT			= 10000;	// 大堆金子
const int	BIGHEAPGOLD_TYPE			= 1091020;	// 大堆金子ITEMTYPE

//	1050000 ~ 1059999都是精灵/精灵蛋
const int	ITEMTYPE_SPRITE		=	50000;// 精灵、精灵蛋

// Other type define
const int	OTHER_GEM					=	00000;
const int	OTHER_TASKITEM				=	10000;
const int	OTHER_ACTIONITEM			=	20000;

// Expend type define
const int	EXPEND_INVALID				=	-1;	  // 非法
const int	EXPEND_MEDICINE				=	00000;// 补药
const int	EXPEND_POISON				=	10000;// 毒药
const int	EXPEND_DART					=	20000;// 暗器
const int	EXPEND_BOOK					=	30000;// 秘笈
const int	EXPEND_NOTUSE				=	40000;// 迷信，不用
const int	EXPEND_ARROW				=	50000;// 箭矢
const int	EXPEND_SPELL				=	60000;// 符咒类
const int	EXPEND_NOT_DIRECTUSE		=	70000;// 不可以双击使用的（如任务物品，宝石，木材等资源类，千位继续分类）

// Necklace type define
const int	NECKLACE_NECKLACE			=	00000;// 项链
const int	NECKLACE_SACHET				=	10000;// 香袋
const int	NECKLACE_AMULET				=	20000;// 护身符

// Ring type define
const int	RING_RING				=	00000;// 戒指
const int	RING_THUMB_RING			=	10000;// 扳指
const int	RING_BANGLE				=	20000;// 手镯

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

const int ITEMTYPE_GHOSTGEM = 30000; // 魔魂宝石
const int ITEMTYPE_GHOSTGEM_ACTIVE_ATK = 31000; // 状态攻击类
const int ITEMTYPE_GHOSTGEM_PASSIVE_ATK = 32000; // 状态被动类
const int ITEMTYPE_GHOSTGEM_EUDEMON = 33000; // 幻兽类
const int ITEMTYPE_GHOSTGEM_RELEASE = 34000; // 解除类
const int ITEMTYPE_GHOSTGEM_TRACE = 35000; // 追杀类
const int ITEMTYPE_GHOSTGEM_PROTECTIVE = 36000; // 护身类
const int ITEMTYPE_GHOSTGEM_SPECIAL = 37000; // 特殊类
const int ITEMTYPE_GHOSTGEM_INVITE = 38000; //锻造类

const int ITEMTYPE_EUDEMON = 70000; //幻兽
const int ITEMTYPE_EUDEMONEGG = 80000;//幻兽蛋

enum
{
    _ITEM_STATUS_NONE			=	0,	//	无
    _ITEM_STATUS_NOT_IDENT		=	1,	//	未鉴定
    _ITEM_STATUS_CANNOT_REPAIR	=	2,	//	不可修复
    _ITEM_STATUS_NEVER_DAMAGE	=	4,	//	永不磨损
    _ITEM_STATUS_MAGIC_ADD		=	8,	//	魔法追加

};

// 当target字段为0的时候，不限制使用条件
const unsigned short TARGET_NONE = 0x0000; // 不限定条件
// 目标类型
// 以下三条可以组合出现，以控制允许对多种目标使用
const unsigned short TARGET_USER = 0x0001; // 可以对玩家使用
const unsigned short TARGET_MONSTER = 0x0002; // 可以对怪物使用
const unsigned short TARGET_EUDEMON = 0x0004; // 可以对幻兽使用
// TARGET_SELF和TARGET_OTHERS是互斥关系，可以都不指定，但是不允许同时出现
// 如果指定了TARGET_BODY，则只能对尸体使用，否则只能对活着的目标使用
const unsigned short TARGET_SELF = 0x0010; // 限制只能对自己或属于自己的目标
const unsigned short TARGET_OTHERS = 0x0020; // 限制只能对别人或属于别人的目标
const unsigned short TARGET_BODY = 0x0040; // 限制只能对尸体使用
// 其他类型检查
const unsigned short TARGET_CHK_PKMODE = 0x0100; // 需要检查pk模式
const unsigned short TARGET_FORBIDDEN = 0x0200; // 禁止对任何目标使用

const OBJID MONEYITEM_TYPE = 720000;   //筹码物品类型

//declare class here
class CItem	:	public IItem
{
public:
    CItem();
    virtual ~CItem();

    static	CItemData s_ItemData;
    static  int ItemGetSort		(OBJID idType);
    static  int ItemGetSubType	(OBJID idTyep);

protected:
    OBJID			m_idItem;
    OBJID			m_idOwner;
    ItemTypeInfo	m_infoType;
    unsigned char	m_ucPosition;
    DWORD			m_dwSellPrice;
    int				m_nMaxAttackModify;
    int				m_nMinAttackModify;
    int				m_nDefenceModify;
    int				m_nDexterityModify;
    int				m_nDodgeModify;
    int				m_nMagicAttackModify;
    int				m_nMagicDefenceModify;
    int				m_nFrameIndex;
    unsigned short	m_usPercent;
    unsigned short  m_usOriginalAmount;
    unsigned short  m_usOriginalAmountLimit;
    DWORD			m_dwWarGhostExp;	// 战魂经验 //幻兽当前生命
    DWORD			m_dwGemAtkType;		// 宝石类型	//幻兽经验
    DWORD			m_dwAvailabeTime;	// 使用次数
    DWORD			m_dwGrow;			//成长率	//筹码的兑换价格
    char			m_szMonsterName[16];	//幻兽名字
    BOOL			m_bMonsterDie;		//幻兽是否死亡
public:
    void			SetMonsterDie(BOOL bDie) {m_bMonsterDie = bDie;}
    BOOL			IsMonsterDie() {return m_bMonsterDie;}

    DWORD			GetWarGhostExp() {return m_dwWarGhostExp;}
    int				GetWarGhostLevel() {return (int)powf((float)m_dwWarGhostExp / (float)2, (float)1 / (float)3);}

    void			SetMonsterGrow(DWORD dwGrow) {m_dwGrow = dwGrow;}
    int				GetMonsterGrow() {return m_dwGrow;}

    int				GetMonsterMaxAtk();
    int				GetMonsterMinAtk();
    int				GetMonsterMagicMaxAtk();
    int				GetMonsterMagicMinAtk();
    int				GetMonsterMagicDef();
    int				GetMonsterDef();
    int				GetMonsterMaxLife();

    DWORD			IsMonsterCanEvolve(int nMode);	//幻兽是否能进化

    void			SetWarGhostExp(DWORD dwWarGhostExp) {m_dwWarGhostExp = dwWarGhostExp;}
    DWORD			GetGemAtkType() {return m_dwGemAtkType;}
    void			SetGemAtkType(DWORD dwGemAtkType) {m_dwGemAtkType = dwGemAtkType;}
    DWORD			GetAvailabeTime() {return m_dwAvailabeTime;}
    void			SetAvailabeTime(DWORD dwAvailabeTime) {m_dwAvailabeTime = dwAvailabeTime;}
    void			SetMonsterName(char* szName) {strcpy(m_szMonsterName, szName);}
    char*			GetMonsterName();

    unsigned short	GetPercent() {return m_usPercent;}
    BOOL			Create(OBJID idItem, OBJID idItemType);
    void			SetAmount(unsigned short usAmount) {m_infoType.usAmount = usAmount;}
    void			SetAmountLimit(unsigned short usAmountLimit) {m_infoType.usAmountLimit = usAmountLimit;}
    void			SetGem1(unsigned char ucGem1) {m_infoType.ucGem1 = ucGem1;}
    void			SetGem2(unsigned char ucGem2) {m_infoType.ucGem2 = ucGem2;}
    void			SetMagic1(unsigned char ucMagic1) {m_infoType.ucMagic1 = ucMagic1;}
    void			SetMagic2(unsigned char ucMagic2) {m_infoType.ucMagic2 = ucMagic2;}
    void			SetMagic3(unsigned char ucMagic3) {m_infoType.ucMagic3 = ucMagic3;}
    void			SetPosition(unsigned char ucPosition) {m_ucPosition = ucPosition;}

    void			SetStatus(unsigned char ucStatus);
    unsigned char	GetStatus();
    BOOL			TestStatus(unsigned char ucStatus);

    OBJID			GetID() {return m_idItem;}
    unsigned long	Release() {delete this; return 0;}
    int				GetSort();
    int				GetSubType();
    OBJID			GetTypeID() {return m_infoType.uID;}
    char*			GetName() {return m_infoType.szName;}
    unsigned char	GetProfessionRequired() {return m_infoType.ucRequiredProfession;}
    unsigned char	GetWeaponSkillRequired() {return m_infoType.ucRequiredWeaponSkill;}
    unsigned char	GetLevelRequired() {return m_infoType.ucRequiredLevel;}
    unsigned char	GetSexRequried() {return m_infoType.ucRequiredSex;}
    unsigned short	GetForceRequired() {return m_infoType.usRequiredForce;}
    unsigned short	GetSpeedRequired() {return m_infoType.usRequiredSpeed;}
    unsigned short	GetHealthRequired() {return m_infoType.usRequiredHealth;}
    unsigned short	GetSoulRequired() {return m_infoType.usRequiredSoul;}
    unsigned char	GetMonopoly() {return (unsigned char)m_infoType.ucMonopoly;}
    unsigned short	GetWeight();
    unsigned int	GetPrice() {return m_infoType.uPrice;}
    unsigned short	GetMaxAttack() {return m_infoType.usMaxAttack;}
    unsigned short	GetMinAttack() {return m_infoType.usMinAttack;}
    short			GetDefense() {return m_infoType.usDefense;}
    short			GetDexterity() {return m_infoType.usDexterity;}
    short			GetDodge() {return m_infoType.usDodge;}
    short			GetLife() {return m_infoType.sLife;}
    short			GetMana() {return m_infoType.sMana;}
    unsigned short	GetAmount() {return m_infoType.usAmount;}
    unsigned short	GetOriginalAmountLimit() {return m_usOriginalAmountLimit;}
    unsigned short	GetOriginalAmount() {return m_usOriginalAmount;}

    unsigned short	GetAmountLimit() {return m_infoType.usAmountLimit;}
    unsigned char	GetGem1() {return m_infoType.ucGem1;}
    unsigned char	GetGem2() {return m_infoType.ucGem2;}
    unsigned char	GetMagic1() {return m_infoType.ucMagic1;}
    unsigned char	GetLucky() {return m_infoType.ucMagic2;}
    unsigned char	GetAddition() {return m_infoType.ucMagic3;}
    unsigned char	GetPosition() {return m_ucPosition;}
    DWORD			GetSellPrice();
    DWORD			GetRepairCost();

    DWORD			GetWeaponSkillType();

    int				GetAtkRange () { return m_infoType.usRange; }
    int				GetHitarte() {return m_infoType.nHitarte;}

    unsigned short  GetTarget() {return m_infoType.usTarget;}
    BOOL			TestTarget(unsigned short usTarget) ;

    unsigned short	GetMinMagicAttack() {return m_infoType.usDexterity;}
    unsigned short	GetMaxMagicAttack() {return m_infoType.usMagicAttack;}
    unsigned short	GetSoul() {return m_infoType.usDodge;}
    unsigned short	GetMagicDefence();
    unsigned short  GetAttackSpeed() {return m_infoType.usAttackSpeed;}


    //升级品质宝石type：1037160
    //升级魔魂等级宝石type：1037150
    enum {TREASURE_DRAGONBALL = 1037160, TREASURE_SHOOTINGSTAR = 1037170, TREASURE_SOULSTONE = 1037150};

    BOOL	IsDragonBall(void)		{ return (TREASURE_DRAGONBALL == GetTypeID()); }
    BOOL	IsShootingStar(void)	{ return (TREASURE_SHOOTINGSTAR == GetTypeID()); }
    BOOL	IsSoulStone(void)		{return (TREASURE_SOULSTONE == GetTypeID()); }


    //复活幻兽宝石type: 1033020
    //强化幻兽宝石type: 1033030
    enum {SOULSTONE_ALIVE = 1033020, SOULSTONE_IMPROVE = 1033030};
    BOOL	IsAliveSoulStone(void)   {return (SOULSTONE_ALIVE == GetTypeID()); }
    BOOL	IsImproveSoulStone(void) {return (SOULSTONE_IMPROVE == GetTypeID()); }

    char*	GetDesc();

    BOOL	IsActionItem(void)	{ return ITEMSORT_OTHER == GetSort() && OTHER_ACTIONITEM == GetSubType(); }
    BOOL	IsTaskItem	(void)	{ return ITEMSORT_OTHER == GetSort() && OTHER_TASKITEM == GetSubType(); }
    BOOL	IsMonopolyItem(void) {return this->GetMonopoly() == _MONOPOLY_NOT_DISAPPEAR || this->GetMonopoly() == _MONOPOLY_DISAPPEAR; }
    BOOL	TestMonopolyItem(unsigned char ucMonopoly);
    BOOL	IsGem		(void)	{ return ITEMSORT_OTHER == GetSort() && OTHER_GEM == GetSubType(); }
    BOOL	IsArrow		(void)	{ return ITEMSORT_EXPEND == GetSort() && EXPEND_ARROW == GetSubType(); }
    BOOL	IsBow		(void)	{ return ITEMSORT_WEAPON_DOUBLE_HAND == GetSort() && DWEAPON_BOW == GetSubType(); }

    BOOL    IsMoneyItem() { return GetTypeID () == MONEYITEM_TYPE;}	//是不是筹码
    DWORD   GetMoneyItemPrice() {return m_dwGrow;}

    void	SetBoothSellPrice(DWORD dwPrice);
    DWORD	GetBoothSellPrice();
    void	SetOwnerID(OBJID idOwner);
    OBJID	GetOwnerID();

    int		GetMaxAttackModify();
    int		GetMinAttackModify();
    int		GetDefenceModify();
    int		GetDexterityModify();
    int		GetDodgeModify();
    int		GetMagicAttackModify();
    int		GetMagicDefenceModify();
    void	ShowMiniItem(int nX, int nY , BOOL bScale = true);
    static	BOOL	IsNonsuchItem(OBJID idItemType);
    int		GetQuality(void);
    BOOL	WantCombin();

    int		GetPackageType();
};

#include <deque>
using namespace std;
typedef deque<CItem*>DEQUE_ITEM;

//end class declaration
#endif
