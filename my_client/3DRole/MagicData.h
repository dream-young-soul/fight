
//**********************************************************
// 代码编辑器
//**********************************************************

// MagicData.h: interface for the CMagicData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAGICDATA_H__B1F17B6C_5DC6_4DC8_ACBC_598255551440__INCLUDED_)
#define AFX_MAGICDATA_H__B1F17B6C_5DC6_4DC8_ACBC_598255551440__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseFunc.h"

enum
{
    MAGIC_SORT_ATTACK_SINGLE_HP			= 1,	// 攻击单人，对方去血
    MAGIC_SORT_RECOVER_SINGLE_HP		= 2,	// 回复单人，对方加血
    MAGIC_SORT_ATTACK_CROSS_HP			= 3,	// 十字型区域目标去血
    MAGIC_SORT_ATTACK_SECTOR_HP			= 4,	// 扇形区域目标去血
    MAGIC_SORT_ATTACK_ROUND_HP			= 5,	// 圆形区域目标去血
    MAGIC_SORT_ATTACK_SINGLE_STATUS		= 6,	// 攻击单人，对方加状态
    MAGIC_SORT_RECOVER_SINGLE_STATUS	= 7,	// 回复单人，对方去状态
    MAGICSORT_SQUARE					= 8,	// 方形攻击
    MAGICSORT_JUMPATTACK				= 9,	// 跳砍
    MAGICSORT_RANDOMTRANS				= 10,	// 随机传送
    MAGICSORT_DISPATCHXP				= 11,	// 怒吼
    MAGICSORT_COLLIDE					= 12,	// 冲击
    MAGICSORT_SERIALCUT					= 13,	// 连击
    MAGICSORT_LINE						= 14,	// 方向魔法
    MAGICSORT_ATKRANGE					= 15,	// 增加攻击范围
    MAGICSORT_ATKSTATUS					= 16,	// ~!@#$%^&*()_+
    MAGICSORT_CALL_TEAMMEMBER			= 17,	// 召唤队员
    MAGICSORT_RECORDTRANSSPELL			= 18,	// record map position to trans spell.
    MAGICSORT_TRANSFORM					= 19,	// 变身
    MAGICSORT_ADDMANA					= 20,	// 加Mana// support self target only.
    MAGICSORT_LAYTRAP					= 21,
    MAGICSORT_DANCE						= 22,	// 跳舞
    MAGICSORT_CALLPET			= 23,			// 召唤兽
    MAGICSORT_VAMPIRE			= 24,			// 吸血，power is percent award. use for call pet
    MAGICSORT_INSTEAD			= 25,			// 替身. use for call pet
    MAGICSORT_DECLIFE			= 26,			// 扣血(当前血的比例)
    MAGICSORT_GROUNDSTING		= 27,			// 地刺
    MAGICSORT_REBORN			= 28,			// 复活 -- zlong 2004.5.14
    MAGICSORT_TEAM_MAGIC		= 29,			// 界结魔法—— 与MAGICSORT_ATTACHSTATUS相同处理，
    //				这里独立分类只是为了方便客户端识别
    MAGICSORT_BOMB_LOCKALL		= 30,			// 与MAGICSORT_BOMB处理相同，只是锁定全部目标
    MAGICSORT_SORB_SOUL			= 31,			// 吸魂魔法
    MAGICSORT_STEAL				= 32,			// 偷盗，随机从目标身上偷取power个物品
    MAGICSORT_LINE_PENETRABLE	= 33,			// 攻击者轨迹可以穿人的线性攻击

    //////////////////////////////////////////////
    // 新增幻兽魔法类型
    MAGICSORT_BLAST_THUNDER			= 34,			// 魔雷
    MAGICSORT_MULTI_ATTACHSTATUS	= 35,			// 群体施加状态
    MAGICSORT_MULTI_DETACHSTATUS	= 36,			// 群体解除状态
    MAGICSORT_MULTI_CURE			= 37,			// 群体补血
    MAGICSORT_STEAL_MONEY			= 38,			// 偷钱
    MAGICSORT_KO					= 39,			// 必杀技，目标血小于15%自动触发
    MAGICSORT_ESCAPE				= 40,			// 逃跑/救助
    //		MAGICSORT_FLASH_ATTACK			= 41,			// 移形换位
    MAGICSORT_ATTRACK_MONSTER		= 42,			// 吸引怪物
};

enum
{
    MAGIC_SHAKENORMAL		  = 0x00000001, // 震动效果
    MAGIC_COLORNORMAL		  = 0x00000002, // 变暗效果
    MAGIC_SCALENORMAL		  = 0x00000004, // 缩放效果
};

enum
{
    MAGIC_TARGET_SELF		  = 0x00000001,	// 可以对自己使用
    MAGIC_TARGET_NONE		  = 0x00000002, // 不需要点目标
    MAGIC_TARGET_TERRAIN      = 0x00000004, // 对地使用
    MAGIC_PASSIVE			  = 0x00000008, // 被动魔法，不能使用
    MAGIC_TARGET_BODY		  = 0x00000010, // 只能对未消失的尸体使用 //其他无效
};

enum
{
    TYPE_MAGIC		= 0, // 魔法
    TYPE_XPSKILL	= 1, // xp技
    TYPE_KONGFU		= 2, // 武功招式
};
#include <vector>
using namespace std;

typedef struct
{
    OBJID	idMagicType;			// 类型ID
    DWORD	dwActionSort;			// 行为分类
    char	szName[_MAX_NAMESIZE];	// 魔法名称
    DWORD	dwCrime;				// 是否引发犯罪
    DWORD	dwGround;				// 是否地效魔法
    DWORD	dwMulti;				// 是否多对象
    DWORD	dwTarget;				// 对象类型
    DWORD	dwLevel;				// 魔法等级
    DWORD	dwMpCost;				// 花费MP
    DWORD	dwPotential;			// 花费潜能
    DWORD	dwPower;				// 威力
    DWORD	dwIntoneDuration;		// 吟唱时间
    DWORD	dwHitPoint;				// 命中比率
    DWORD	dwDuration;				// 持续时间 // 毫秒
    DWORD	dwRange;				// 伤害范围 // X= %100 , y=%10000-x*100
    DWORD	dwDistance;				// 有效距离
    DWORD	dwStatus;				// 导致状态
    DWORD	dwProfessionalRequired;	// 要求职业
    DWORD	dwExpRequired;			// 要求经验值 // 升级 // 0不能升级
    DWORD	dwMonsterLevelRequired;	// 要求打击怪物等级 // 无用

    DWORD	dwXp;					// 是否使用XP
    DWORD	dwWeaponSubType;		// 需要的武器类型

    DWORD	dwActiveTime;			// 持续时间
    DWORD	dwAutoActive;			// 自动触发
    DWORD	dwFloorAttribute;		// 地板属性
    DWORD	dwAutoLearn;			// 自动学会
    DWORD	dwLearnLevel;			// 学习需要等级
    DWORD	dwDropWeapon;			// 扔武器
    DWORD	dwUsePP;				// 使用体力值
    DWORD	dwWeaponHit;			// 是否武器攻击
    OBJID	idUseItem;				// 消耗物品
    OBJID	idNextMagic;			// 下一魔法
    DWORD	dwMagicBreak;			// 魔法间隔

    DWORD	dwClientRepresent;		// 客户端表现
    DWORD	dwSenderAction;			// 施放法术者的动作
    DWORD   dwNeedLevel;			// 下级魔法需要的玩家等级 是否有下个魔法等级 0: 没有下一等级
    char	szDisc[64];				// 描述
    char	szIntoneEffect[64];			// 吟唱特效
    char	szIntoneSound[_MAX_PATH];	// 吟唱声效
    char	szSenderEffect[64];			// 攻击特效
    char	szSenderSound[_MAX_PATH];	// 攻击音效
    DWORD	dwTargetDelay;				// 受击延时
    char	szTargetEffect[64];			// 受击特效
    char	szTargetSound[_MAX_PATH];	// 受击音效
    char	szGroundEffect[64];			// 地效索引
    char	szTraceEffect[64];			// 轨迹特效
    DWORD	dwScreenRepresent;			// 屏幕表现
    DWORD	dwCanBeusedInMarket;		// 是否能够在市场使用// 0，不可用， 1，可用
    DWORD	dwTargetWoundDelay;			// 受击者受伤动作延时
    DWORD	dwDelay;					// 开始魔法表现到魔法延迟结束的时间
    BOOL    bTrack;						// 是否连招
    vector<int> setAction;
} MagicTypeInfo;


typedef vector<MagicTypeInfo* >VECTOR_MAGICTYPE;

class CMagicData
{
public:
    CMagicData();
    virtual ~CMagicData();
private:
    VECTOR_MAGICTYPE m_setMagicType;
private:
    void Create();
    void CreateAction();
    void Destroy();
public:
    BOOL GetMagicTypeInfo(OBJID dwIDType, DWORD dwLevel, MagicTypeInfo& infoMagic);
};

#endif // !defined(AFX_MAGICDATA_H__B1F17B6C_5DC6_4DC8_ACBC_598255551440__INCLUDED_)
