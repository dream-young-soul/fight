
//**********************************************************
// 代码编辑器
//**********************************************************

// ItemData.h: interface for the CItemData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMDATA_H__B6A8F74E_31AA_46B4_BF3C_03AF450B1E90__INCLUDED_)
#define AFX_ITEMDATA_H__B6A8F74E_31AA_46B4_BF3C_03AF450B1E90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseFunc.h"
#include <map>
#include <string>
#include <vector>

using namespace std;
enum // 附加属性类型枚举
{
    _SPRITE_ADDITION_PATK	= 0,	//	火元素精灵--增加物理攻击力
    _SPRITE_ADDITION_PDEF	= 1,	//  土元素精灵--增加物理防御力
    _SPRITE_ADDITION_MATK	= 2,	//  风元素精灵--增加魔法攻击力
    _SPRITE_ADDITION_MDEF	= 3,	//  水元素精灵--增加魔法防御力
    _SPRITE_ADDITION_SOUL	= 4,	//  暗元素精灵--增加精神
};
typedef struct
{
    unsigned int	uID;					//0 item type 的 ID		//  精灵的lookface根据itemtype 定义
    char			szName[_MAX_NAMESIZE];	//1 物品名字
    unsigned char	ucRequiredProfession;	//2 要求职业			附加属性类型
    unsigned char	ucRequiredWeaponSkill;	//3 要求的武器技能
    unsigned char	ucRequiredLevel;		//4 要求的等级			// 精灵等级 装备精灵的玩家的等级必须不能少于精灵的等级×2。
    unsigned char	ucRequiredSex;			//5 要求的性别
    unsigned short	usRequiredForce;		//6 要求力量
    unsigned short	usRequiredSpeed;		//7 要求速度
    unsigned short	usRequiredHealth; 		//8 要求健康
    unsigned short	usRequiredSoul; 		//9 要求精神
    unsigned char	ucMonopoly;				//10 独占物品标志
    unsigned short	usWeight;				//11 物品重量
    unsigned int	uPrice;					//12 价格
    int				uLuck;					//13 幸运值				//	升级到下一级需要的经验值
    unsigned short	usMaxAttack;			//14 最大攻击
    unsigned short	usMinAttack;			//15 最小攻击
    short			usDefense;				//16 防御
    short			usDexterity;			//17 敏捷				// 最小魔攻击
    short			usDodge;				//18 闪避				// add soul
    short			sLife;					//19 生命
    short			sMana;					//20 魔法力
    unsigned short	usAmount;				//21 数量				//	精灵亲密度=忠诚
    unsigned short	usAmountLimit;			//22 数量				//  精灵附加属性 // 显示时应当 /100
    unsigned char	ucStatus;				//23 状态
    unsigned char	ucGem1;					//24 镶嵌宝石1
    unsigned char	ucGem2;					//25 镶嵌宝石2
    unsigned char	ucMagic1;				//26 附加魔法1
    unsigned char	ucMagic2;				//27 幸运
    unsigned char	ucMagic3;				//28 追加
    unsigned short	usMagicAttack;			//29 魔攻				// 最大魔攻
    unsigned short	usMagicDefence;			//30 魔防
    unsigned short  usRange;				//31 攻击范围
    unsigned short  usAttackSpeed;			//32 攻击速度
    int		     	nHitarte;				//33 命中
    unsigned short  usTarget;				//34 使用对象
    char			szDesc[128];			//35 描述
} ItemTypeInfo;

typedef struct
{
    int nLife;
    int nAttackMax;
    int nAttackMin;
    int nDefense;
    int nMAttackMax;
    int nMAttackMin;
    int nMDef;
    int nDexterity;
    int nDodge;
} ItemAdditionInfo;

typedef map<OBJID, ItemTypeInfo> MAP_ITEMINFO;
typedef map<__int64, ItemAdditionInfo> MAP_ITEMADDITIONINFO;

class CItemData
{
public:
    CItemData();
    virtual ~CItemData();


  
	void LoadAllItemInfo();
	void ExportItemData(); //导出道具数据库
private:
    BOOL	CreateIndexTable();
    void	CreateAdditionalInfo();
	DWORD m_dwRecordAmount;
public:
    BOOL	GetItemTypeInfo(OBJID idType, ItemTypeInfo& infoItemType);
    BOOL	GetItemAdditionInfo(OBJID idType, int nLevel, ItemAdditionInfo& infoAdditional);
    BOOL	CheckItem(const char* pszItemName);
    static MAP_ITEMINFO s_mapItemType;
    MAP_ITEMADDITIONINFO m_mapItemAdditional;
    std::vector<string> m_setItemName;
};

#endif // !defined(AFX_ITEMDATA_H__B6A8F74E_31AA_46B4_BF3C_03AF450B1E90__INCLUDED_)
