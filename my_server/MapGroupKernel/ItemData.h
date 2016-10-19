
//**********************************************************
// 代码编辑器
//**********************************************************

// ItemData.h: interface for the CItemData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMDATA_H__28794933_A926_4C9A_AACF_1A95340DA94B__INCLUDED_)
#define AFX_ITEMDATA_H__28794933_A926_4C9A_AACF_1A95340DA94B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameObj.h"
#include "ItemType.h"

//////////////////////////////////////////////////////////////////////
const int MAX_INTIMACY		= 255;
//////////////////////////////////////////////////////////////////////

enum // 附加属性类型枚举
{
    _SPRITE_ADDITION_PATK = 0, // 火元素精灵--增加物理攻击力
    _SPRITE_ADDITION_PDEF = 1, // 土元素精灵--增加物理防御力
    _SPRITE_ADDITION_MATK = 2, // 风元素精灵--增加魔法攻击力
    _SPRITE_ADDITION_MDEF = 3, // 水元素精灵--增加魔法防御力
    _SPRITE_ADDITION_SOUL = 4, // 暗元素精灵--增加精神
};

enum ITEMDATA
{
    ITEMDATA_ID_ = 0,			// use for insert
    ITEMDATA_TYPE = 1,
    ITEMDATA_OWNERID,
    ITEMDATA_PLAYERID,
    ITEMDATA_AMOUNT,			//ITEMDATA_EXP = ITEMDATA_AMOUNT,				// 精灵经验 -- 复用表示幻兽亲密度
    ITEMDATA_AMOUNTLIMIT,		//ITEMDATA_ATTRIB = ITEMDATA_AMOUNTLIMIT,		// 精灵附加属性 ATTRIB100 -- 幻兽等级
    ITEMDATA_IDENT,
    ITEMDATA_POSITION,

    ITEMDATA_GEM1,
    ITEMDATA_GEM2,
    ITEMDATA_MAGIC1,

    ITEMDATA_MAGIC2,			ITEMDATA_LUCK = ITEMDATA_MAGIC2,		// 幸运
    ITEMDATA_MAGIC3,			ITEMDATA_ADDITION = ITEMDATA_MAGIC3,	// 追加
    ITEMDATA_DATA,				//ITEMDATA_GROWTH = ITEMDATA_DATA,			// 精灵成长率 -- 幻兽成长率系数

    //---jinggy---2004-11-19---圣战魔域---铸造系统需求增加的字段---begin
    ITEMDATA_WARGHOSTEXP,    //圣战中的战魂经验值							// 复用表示幻兽生命值
    ITEMDATA_GEMTYPE,	     //圣战中的技能（注入不同状态攻击类宝石实现）	// 幻兽经验
    ITEMDATA_AVAILABLETIME,
    //---jinggy---2004-11-19---圣战魔域---铸造系统需求增加的字段---end

    /////////////////////////////////////
    ITEMDATA_INTIMACY = 80,				//? for mount, NOTE: no this field

    // for sprite -- add by zlong 2003-12-05
    ITEMDATA_EXP	= 90,					// --> 幻兽经验
    ITEMDATA_ATTRIB	= 91,		ITEMDATA_EUDEMON_LEVEL = 91,	// --> 幻兽等级
    ITEMDATA_GROWTH	= 92,					// --> 幻兽成长率系数
    ITEMDATA_EUDEMON_LIFE	= 93,			// 幻兽生命
    ITEMDATA_FIDELITY		= 94,			// 幻兽亲密度


    ITEMTYPEDATA_OFFSET = 100,		// 控制量，请勿使用
    ITEMDATA_NAME = 101,
    ITEMDATA_REQ_PROF,			// 职业需求
    ITEMDATA_LEVEL,
    ITEMDATA_REQ_LEVEL,			// 等级需求				ITEMDATA_SPRITE_LEVEL
    ITEMDATA_REQ_SEX,			// 性别需求
    ITEMDATA_REQ_FORCE,			// 力量需求
    ITEMDATA_REQ_DEX,			// 敏捷需求
    ITEMDATA_REQ_HEALTH,		// 耐力需求
    ITEMDATA_REQ_SOUL,			// 精神需求
    ITEMDATA_MONOPOLY,
    ITEMDATA_WEIGHT_,				// no immediacy
    ITEMDATA_PRICE,
    ITEMDATA_ACTION,
    ITEMDATA_ATTACK_MAX_,		//ITEMDATA_GROWTH_ORIGINAL
    ITEMDATA_ATTACK_MIN_,		//ITEMDATA_LEVEXP
    ITEMDATA_DEFENSE_,
    ITEMDATA_MAGICATK_MIN_,
    ITEMDATA_DODGE,					//	ITEMDATA_ADD_SOUL = ITEMDATA_DODGE,
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
    ITEMDATA_MAGICATK_MAX_,
    ITEMDATA_MAGICDEF_,
    ITEMDATA_ATKRANGE,
    ITEMDATA_ATKSPEED,
    ITEMDATA_HITRATE,					// 命中率
    ITEMDATA_MONSTERTYPE,				// 幻兽类型id
    ITEMDATA_TARGET,

    ITEMDATA_SPRITE_LEVEL = 200,		// 精灵等级 装备精灵的玩家的等级必须不能少于精灵的等级×2。
    ITEMDATA_GROWTH_ORIGINAL = 201,		// 精灵原始成长率
    ITEMDATA_LEVEXP = 202,				// 精灵升级到下一级需要的经验值
};

//////////////////////////////////////////////////////////////////////
//	ITEMDATA_MONOPOLY,
const DWORD	MONOPOLY_MASK				= 0x01;
const DWORD	STORAGE_MASK				= 0x02;
const DWORD	DROP_HINT_MASK				= 0x04;
const DWORD	SELL_HINT_MASK				= 0x08;
const DWORD	NEVER_DROP_WHEN_DEAD_MASK	= 0x10;
const DWORD	SELL_DISABLE_MASK			= 0x20;


const int	MAGIC_NONE				= 0;
const int	GEM_NONE				= 0;
const int	GEM_HOLE				= 255;

//////////////////////////////////////////////////////////////////////
// 物品大的分类
const int	ITEMSORT_INVALID				=	-1;
const int	ITEMSORT_EXPEND					=	10;	// 易耗品
const int	IETMSORT_FINERY					=	1;	// 服饰
const int	ITEMSORT_WEAPON1				=	4;	// 单手武器（武器）
const int	ITEMSORT_MOUNT					=	6;	// 坐骑
const int	ITEMSORT_OTHER					=	7;	// 其他, 不能直接使用

// 以下分类已经不使用
const int	ITEMSORT_WEAPON2				=	-1;//5;	// 双手武器
const int	ITEMSORT_SHIELD					=	-1;//9;	// 盾牌

//////////////////////////////////////////////////////////////////////
// ITEMSORT_FINERY 类别物品
const int	ITEMTYPE_HELMET				=	10000;	// 头盔
const int	ITEMTYPE_NECKLACE			=	20000;	// 项链
const int	ITEMTYPE_ARMOR				=	30000;	// 盔甲
const int	ITEMTYPE_BANGLE				=	40000;	// 手镯
const int	ITEMTYPE_MANTLE				=	50000;	// 披风
const int	ITEMTYPE_SHOES				=	60000;	// 鞋子

// 以下分类已经不使用
const int	ITEMTYPE_RING				=	-1;//50000;	// 戒指

//////////////////////////////////////////////////////////////////////
// ITEMSORT_EXPEND 类别物品
const int	ITEMTYPE_INVALID			=	-1;	  // 非法

const int	ITEMTYPE_PHYSIC				=	10000;//药品
const int	ITEMTYPE_MEDICINE_HP			=	10000;//补血药
const int	ITEMTYPE_MEDICINE_MP			=	11000;//补魔法药
const int	ITEMTYPE_POISON					=	12000;//毒药
const int	ITEMTYPE_SCROLL				=	20000;//卷轴
const int	ITEMTYPE_SCROLL_SPECIAL			=	20000;//特殊卷轴，如：回城卷、祝福卷轴等
const int	ITEMTYPE_SCROLL_MSKILL			=	21000;//魔法师技能卷轴
const int	ITEMTYPE_SCROLL_SSKILL			=	22000;//战士技能卷轴
const int	ITEMTYPE_SCROLL_BSKILL			=	23000;//弓箭手技能卷轴 <== 改为异能者

const int	ITEMTYPE_GHOSTGEM			=	30000;		// 魔魂宝石
const int	ITEMTYPE_GHOSTGEM_ACTIVE_ATK	=	31000;	// 状态攻击类
const int	ITEMTYPE_GHOSTGEM_PASSIVE_ATK	=	32000;	// 状态被动类
const int	ITEMTYPE_GHOSTGEM_EUDEMON		=	33000;	// 幻兽类
const int	ITEMTYPE_GHOSTGEM_RELEASE		=	34000;	// 解除类
const int	ITEMTYPE_GHOSTGEM_TRACE			=	35000;	// 追杀类
const int	ITEMTYPE_GHOSTGEM_PROTECTIVE	=	36000;	// 护身类
const int	ITEMTYPE_GHOSTGEM_SPECIAL		=	37000;	// 特殊类
const int   ITEMTYPE_GHOSTGEM_EMBEDEQUIP	=	38000;	// 用于嵌入装备的宝石---jinggy
//---jinggy 2004-11-9 --begin---圣战铸造系统用到的
const int   ITEMTYPE_GHOSTGEM_FORQUALITY = 1037160; //提高装备品质的宝石 （灵魂晶石）
const int   ITEMTYPE_GHOSTGEM_FORGHOSTLEVEL = 1037150;//升级装备魔魂等级的宝石
const int   ITEMTYPE_GHOSTGEM_UPGRADE_EQUIPLEVEL = 1037170;//幻魔晶石（幻魔晶石type：1037170）来升级武器或装备的等级。
//---jinggy 2004-11-9 --end---圣战铸造系统用到的


const int	ITEMTYPE_NOT_DIRECTUSE		=	40000;//不可以双击使用的 -- 木材、矿石等资源类

//===================================
const int	ITEMTYPE_SPECIAL_USE		=	50000;	// 大于此编号以上的为其他用途的物品

const int	ITEMTYPE_SPRITE				=	50000;// 精灵
const int	ITEMTYPE_SPRITE_PATK		=		50000;// 火元素精灵--增加物理攻击力
const int	ITEMTYPE_SPRITE_PDEF		=		51000;// 土元素精灵--增加物理防御力
const int	ITEMTYPE_SPRITE_MATK		=		52000;// 风元素精灵--增加魔法攻击力
const int	ITEMTYPE_SPRITE_MDEF		=		53000;// 水元素精灵--增加魔法防御力
const int	ITEMTYPE_SPRITE_SOUL		=		54000;// 暗元素精灵--增加精神

const int	ITEMTYPE_SPECIAL			=	60000;//特殊物品
const int	ITEMTYPE_SPECIAL_VALUABLES		=	60000;//特殊贵重物品。如异次元袋
const int	ITEMTYPE_SPECIAL_UNREPAIRABLE	=	61000;//不可修复的贵重物品

// 幻兽 -- zlong 2004-02-03
const int	ITEMTYPE_EUDEMON			=	70000;	//幻兽
const int	ITEMTYPE_EUDEMON_SPEED			=	71000;	// 速度型
const int	ITEMTYPE_EUDEMON_PATK			=	72000;	// 攻击型
const int	ITEMTYPE_EUDEMON_DEF			=	73000;	// 防御型
const int	ITEMTYPE_EUDEMON_MATK			=	74000;	// 魔法攻击型
const int	ITEMTYPE_EUDEMON_BOMB			=	75000;	// 爆破型
const int	ITEMTYPE_EUDEMON_PROTECTIVE		=	76000;	// 保护型
const int	ITEMTYPE_EUDEMON_ATTACH			=	77000;	// 附身型
const int	ITEMTYPE_EUDEMON_VARIATIONAL	=	78000;	// 变异型

const int	ITEMTYPE_EUDEMON_EGG		=	80000;	// 幻兽蛋
//===================================

/*
const int	ITEMTYPE_MEDICINE			=	00000;// 补药
const int	ITEMTYPE_POISON				=	10000;// 毒药
const int	ITEMTYPE_DART				=	20000;// 矢
const int	ITEMTYPE_BOOK				=	30000;// 秘笈
const int	ITEMTYPE_NOTUSE				=	40000;// 不用
const int	ITEMTYPE_ARROW				=	50000;// 箭
const int	ITEMTYPE_SPELL				=	60000;// 符咒类
const int	ITEMTYPE_NOT_DIRECTUSE		=	70000;// 不可以双击使用的（如任务物品，宝石，木材等资源类，千位继续分类）
const int	ITEMTYPE_EX0				=	80000;// 扩展0
const int	ITEMTYPE_EX1				=	90000;// 扩展1
*/

//////////////////////////////////////////////////////////////////////
// ITEMSORT_OTHER 类别物品
const int	ITEMTYPE_GEM				=	00000;// 宝石物品
const int	ITEMTYPE_TASKITEM			=	10000;// 任务物品
const int	ITEMTYPE_ACTIONITEM			=	20000;// Action物品
const int	ITEMTYPE_GAMECARD			=	80000;// 点卡，月卡

/*/////////////////////////////////////////////////////////////////////
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
*/
// Doublehand Weapon define
const int	DWEAPON_BOW					=	00000;// 弓
const int	DWEAPON_CROSSBOW			=	 1000;// 弩
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

// add by zlong 2003-12-01
const int	MAX_SPRITE_GROWTH		= 500;	// 精灵的最大成长率*100
const int	MAX_SOUL_SPRITE_GROWTH	= 200;	// 加精神的精灵最大成长率*100

const int	EUDEMON_DEC_FIDELITY_WHEN_DIE		= 10;			// 幻兽死亡扣10点亲密度
const int	EUDEMON_DEC_FIDELITY_WHEN_DEAL		= 30;			// 幻兽转手要扣30点亲密度


//////////////////////////////////////////////////////////////////////
//const int	ITEMPOSITION_BACKPACK	= 0;	// 普通物品背包
//const int	ITEMPOSITION_NONE		= 0;
const int	ITEMPOSITION_EQUIPBEGIN		= 1;
const int	ITEMPOSITION_HELMET		= 1;	// 头盔
const int	ITEMPOSITION_NECKLACE	= 2;	// 项链
const int	ITEMPOSITION_ARMOR		= 3;	// 铠甲
const int	ITEMPOSITION_WEAPONR	= 4;	// 武器
const int	ITEMPOSITION_WEAPONL	= 5;
const int	ITEMPOSITION_RINGR		= 6;
const int	ITEMPOSITION_RINGL		= 7;	// 手镯
const int	ITEMPOSITION_SHOES		= 8;	// 鞋子
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
const int	ITEMPOSITION_PLAYERTASK	= 204;			// 佣兵任务奖品
const int	ITEMPOSITION_EUDEMON_BROODER	= 205;	// 幻兽孵化器
const int	ITEMPOSITION_EUDEMON_STORAGE	= 206;	// 幻兽存储仓库
const int	ITEMPOSITION_AUCTION_STORAGE    = 207;  // Npc拍买仓库（玩家物品）
const int   ITEMPOSITION_AUCTION_SYS_STORAGE = 208; // Npc拍买仓库（系统物品）
const int	ITEMPOSITION_GROUND		= 254;			// 不存数据库
const int	ITEMPOSITION_NONE		= 255;			// 非法位置

//////////////////////////////////////////////////////////////////////
char	szItemTable[];
typedef	CGameData<ITEMDATA, szItemTable, szID>		CGameItemData;

//////////////////////////////////////////////////////////////////////
enum EUDEMONDATA
{
    EUDEMONDATA_ID = 0,				// 幻兽ID
    EUDEMONDATA_OWNERID,			// 幻兽物品ID
    EUDEMONDATA_NAME,				// 幻兽名字
    EUDEMONDATA_RELATIONSHIP,		// 与其它幻兽关系
};

char	szEudemonTable[];
typedef		CGameData<EUDEMONDATA, szEudemonTable, szID>	CEudemonData;

//////////////////////////////////////////////////////////////////////
#include "T_SingleObjSet2.h"
#include "T_SingleMap2.h"
#include "UserTable.h"
#include "ItemAddition.h"

//////////////////////////////////////////////////////////////////////
class IDatabase;
class IRecord;
class IRecordset;
class CItemData : public CGameObj
{
protected:
    CItemData();
    virtual ~CItemData();

public:
    bool	Create			(IRecord* pDefault, const ItemInfoStruct* pInfo, IRecord* pDefaultEudemonData, bool bInsert = true, OBJID idNew = ID_NONE);			// false: 不存数据库
    bool	Create			(IRecord* pDefault, OBJID idType, OBJID idUser, int nPosition, IRecord* pDefaultEudemonData = NULL);
    bool	Create			(OBJID idItem, IDatabase* pDb);
    bool	Create			(IRecordset* pRes, IDatabase* pDb);
    bool	DeleteRecord		();
    static bool		DeleteItemRecord	(OBJID id, IDatabase* pDb);

public: // info
    bool	GetInfo	(ItemInfoStruct* pInfo);
    bool	SaveInfo		(void);

public: // get set
    OBJID		GetID()					{ return m_pData->GetKey(); }

    int		GetInt(ITEMDATA idx);
    LPCTSTR	GetStr(ITEMDATA idx);
    void	SetInt(ITEMDATA idx, int nData, bool bUpdate = false);
    void	SetStr(ITEMDATA idx, LPCTSTR szData, int nSize, bool bUpdate = false);
protected:
    bool	IsMount()				{ return ((m_pData->GetInt(ITEMDATA_TYPE) % 10000000) / 100000) == ITEMSORT_MOUNT; }		// 用于GEM1和INTIMACY分类处理
    bool	IsSprite()				{ return ((m_pData->GetInt(ITEMDATA_TYPE) % 10000000) / 100000) == ITEMSORT_EXPEND && ((m_pData->GetInt(ITEMDATA_TYPE) % 100000) / 10000) * 10000 == ITEMTYPE_SPRITE; }
    bool	IsEudemon()				{ return ((m_pData->GetInt(ITEMDATA_TYPE) % 10000000) / 100000) == ITEMSORT_EXPEND && ((m_pData->GetInt(ITEMDATA_TYPE) % 100000) / 10000) * 10000 == ITEMTYPE_EUDEMON; }
    bool	IsEudemonEgg()			{ return ((m_pData->GetInt(ITEMDATA_TYPE) % 10000000) / 100000) == ITEMSORT_EXPEND && ((m_pData->GetInt(ITEMDATA_TYPE) % 100000) / 10000) * 10000 == ITEMTYPE_EUDEMON_EGG; }

    bool	LoadEudemonData(OBJID idOwner, IDatabase* pDb);
public:
    bool	CreateEudemonData(IRecord* pDefaultData, OBJID idOwner, const char* pszName, bool bInsert = true);

protected: // data & type
    CGameItemData*		m_pData;
    CItemTypeData*		m_pType;
    //	CMonsterTypeData*	m_pEudemonType;		// 幻兽类型数据引用 -- zlong 2004-02-05

    // 追加属性——允许为NULL
    CItemAdditionData*		m_pAddition;
    CEudemonData*		m_pEudemonData;		// 幻兽数据
};

#endif // !defined(AFX_ITEMDATA_H__28794933_A926_4C9A_AACF_1A95340DA94B__INCLUDED_)
