
//**********************************************************
// 代码编辑器
//**********************************************************

// ItemType.h: interface for the CItemType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(ITEMTYPE_H)
#define ITEMTYPE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)
#include "define.h"
#include "ConstGameData.h"
#include "GameData.h"
#include "GameObj.h"
#include "Myheap.h"
#include "T_SingleMap2.h"
#include <vector>
using namespace std;


//////////////////////////////////////////////////////////////////////
struct ItemInfoStruct
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
    //---jinggy---2004-11-19---begin
    DWORD	dwWarGhostExp; //战魂经验值
    DWORD	dwGemAtkType; //攻击类宝石的类型
    DWORD	dwAvailabeTime;//攻击类宝石可用次数
    //---jinggy---2004-11-19---end

    char	szName[_MAX_NAMESIZE];	// 由于幻兽可改名，因此掉落的物品名字有可能不是标准物品名字
};

//////////////////////////////////////////////////////////////////////
enum ITEMTYPEDATA
{
    ITEMTYPEDATA_ID_ = 0,				//道具ID
    ITEMTYPEDATA_NAME = 1,				//道具名称
    ITEMTYPEDATA_REQ_PROF,				//所需职业
    ITEMTYPEDATA_LEVEL,					// 武器技能
    ITEMTYPEDATA_REQ_LEVEL,				ITEMTYPEDATA_SPRITE_LEVEL = ITEMTYPEDATA_REQ_LEVEL,//所需等级
    ITEMTYPEDATA_REQ_SEX,				//所需性别
    ITEMTYPEDATA_REQ_FORCE,				//力量
    ITEMTYPEDATA_REQ_DEX,				//速度
    ITEMTYPEDATA_REQ_HEALTH,			//健康
    ITEMTYPEDATA_REQ_SOUL,				//精神
    ITEMTYPEDATA_MONOPOLY,				//独占物品标识
    ITEMTYPEDATA_WEIGHT,				//物品重量
    ITEMTYPEDATA_PRICE,					//价格
    ITEMTYPEDATA_ACTION,				//动作
    ITEMTYPEDATA_ATTACK_MAX,			ITEMTYPEDATA_GROWTH_ORIGINAL = ITEMTYPEDATA_ATTACK_MAX,//最大攻击
    ITEMTYPEDATA_ATTACK_MIN,			ITEMTYPEDATA_LEVEXP = ITEMTYPEDATA_ATTACK_MIN,	//最小攻击
    ITEMTYPEDATA_DEFENSE,				//防御
    ITEMTYPEDATA_DEXTERITY,				//敏捷
    ITEMTYPEDATA_LUCK,					//幸运		
    ITEMTYPEDATA_LIFE,					//生命
    ITEMTYPEDATA_MANA,					//魔法力
    ITEMTYPEDATA_AMOUNT,				//数量
    ITEMTYPEDATA_AMOUNT_LIMIT,			//数量
    ITEMTYPEDATA_IDENT,					
    ITEMTYPEDATA_GEM1,					//宝石1
    ITEMTYPEDATA_GEM2,					//宝石2
    ITEMTYPEDATA_MAGIC1,				//附加魔法
    ITEMTYPEDATA_MAGIC2,				//附加魔法1
    ITEMTYPEDATA_MAGIC3,				//附加魔法2
    ITEMTYPEDATA_MAGICATK,				//魔攻
    ITEMTYPEDATA_MAGICDEF,				//魔防
    ITEMTYPEDATA_ATKRANGE,				//攻击范围
    ITEMTYPEDATA_ATKSPEED,				//攻击速度
    ITEMTYPEDATA_HITRATE,				//命中
    ITEMTYPEDATA_MONSTERTYPE,			//
    ITEMTYPEDATA_TARGET,				//使用对象
};

char	szItemTypeTable[];
#if	defined(PALED_DEBUG)
typedef	CConstGameData<ITEMTYPEDATA, szItemTypeTable, ITEMTYPEDATA_ID_>	CItemTypeData;
#else
typedef	CGameData<ITEMTYPEDATA, szItemTypeTable, szID>	CItemTypeData;
#endif
typedef	ISingleMap2<CItemTypeData>	IItemTypeSet;
typedef	CSingleMap2<CItemTypeData>	CItemTypeSet;

class IDatabase;
class CItemType
{
protected:
    CItemType();
    virtual ~CItemType();
public:
    static CItemType*	CreateNew	()		{ return new CItemType; }
    ULONG	Release			()				{ delete this; return 0; }
    bool	Create			(IDatabase* pDb);
    CItemTypeData*	QueryItemType(OBJID idType);
    bool	GetInfo(OBJID idType, ItemInfoStruct* pInfo);

protected:
    IItemTypeSet*	m_setType;

    MYHEAP_DECLARATION(s_heap)
};



#endif // !defined(ITEMTYPE_H)
