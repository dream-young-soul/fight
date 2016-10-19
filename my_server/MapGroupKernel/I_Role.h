
//**********************************************************
// 代码编辑器
//**********************************************************

// RoleManager.h: interface for the CRoleManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(I_ROLE_HEADER)
#define I_ROLE_HEADER

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <math.h>
#include "define.h"
#include "GameObjSet.h"
#include "TimeOut.h"

enum { POSE_STAND = 0, };

const int	ADJUST_PERCENT			= 30000;						// ADJUST时，>=30000 表示百分数
const int	ADJUST_SET				= -30000;						// ADJUST时，<=-30000 表示等于(-1*num - 30000)
const int	ADJUST_FULL				= -32768;						// ADJUST时，== -32768 表示填满
const int	DEFAULT_DEFENCE2		= 10000;						// 数据库缺省值

//帮派等级需要的人数的最低限度
const int	SYNMEMSUM_LOWERLIMIT_TWO = 60;
const int	SYNMEMSUM_LOWERLIMIT_THREE = 150;
const int	SYNMEMSUM_LOWERLIMIT_FOUR = 380;
//帮派级别
const int SYNRANK_FOUR	= 4;
const int SYNRANK_THREE	= 3;
const int SYNRANK_TWO	= 2;
const int SYNRANK_ONE	= 1;

const DWORD TIME_FOOTPRINT	= 1000;

const int	TEAM_STATUS_REQ_ROLES	= 3;		// 结界状态生效需要的最少组队人数

const int	MAX_FRENZY_LIFE_PERCENT	= 70;		// 生命超过这个百分比就会取消狂暴状态


///////////////////////////////////////
// 修改魔法系统需要的
const int	ARROW_FLY_ALT					= 60;							// 箭支飞行高度


//---帮派---begin
//////////////////////////////////////////////////////////////////////
enum
{
    RANK_LEADER			= 100,			//军团团长
    RANK_SUBLEADER		= 80,			//分团长
    RANK_TEAMLEADER		= 60,			//队长
    RANK_NEWBIE			= 20,			//普通团员
    RANK_RESERVED		= 10,			//预备团员
    RANK_NONE			= 0,

    RANK_SUB		= RANK_LEADER - RANK_SUBLEADER,
    //		RANK_PRE		= -1,//RANK_SUBLEADER - RANK_PRESUBLEADER,
    //		RANK_TEAM		= RANK_SUBLEADER - RANK_TEAMLEADER,
};

enum
{
    SYNMEM_LEVEL_ONE		= 1,
    SYNMEM_LEVEL_TWO		= 2,
    SYNMEM_LEVEL_THREE		= 3,
    SYNMEM_LEVEL_FOUR		= 4,
    SYNMEM_LEVEL_FIVE		= 5,
};

//---帮派---end
//////////////////////////////////////////////////////////////////////
enum
{
    DIE_NORMAL			= 1,
    DIE_CRITICALHIT		= 2,
    DIE_MAGIC			= 3,
    DIE_FLOAT			= 4,		// only for client
    DIE_ARROWSHOOT		= 5,
};

//////////////////////////////////////////////////////////////////////
enum { OWNER_NONE, OWNER_USER, OWNER_SYN, OWNER_NPC };

//////////////////////////////////////////////////////////////////////
// 外部状态，需要通过MsgPlayer通知其它玩家的状态。与客户端同步
const I64		KEEPEFFECT_NORMAL			= 0x00000000; 	// 无特殊状态

const I64		KEEPEFFECT_TEAMLEADER		= 0x00000001;   // 队长

const I64		KEEPEFFECT_DIE				= 0x00000002;	// 死亡
const I64		KEEPEFFECT_GHOST			= 0x00000004;   // 灵魂状态
const I64		KEEPEFFECT_DISAPPEARING		= 0x00000008;   // 尸体消失状态


const I64		KEEPEFFECT_CRIME			= 0x00000010;	// 犯罪 // 闪蓝色
const I64		KEEPEFFECT_RED				= 0x00000020;	// 红名
const I64		KEEPEFFECT_DEEPRED			= 0x00000040;	// 黑名
const I64		KEEPEFFECT_SYNCRIME			= 0x00000080;	// 帮派犯罪

const I64		KEEPEFFECT_POISON			= 0x00000100;   // 中毒
const I64		KEEPEFFECT_HIDDEN			= 0x00000200;   // 隐身
const I64		KEEPEFFECT_FREEZE			= 0x00000400;   // 冰冻
const I64		KEEPEFFECT_SUPERSOLDIER		= 0x00000800;   // 无双
const I64		KEEPEFFECT_LURKER			= 0x00001000;   // 潜伏

const I64		KEEPEFFECT_DEFENCE1			= 0x00002000;   // 防御提高1
const I64		KEEPEFFECT_DEFENCE2			= 0x00004000;   // 防御提高2
const I64		KEEPEFFECT_DEFENCE3			= 0x00008000;   // 防御提高3

const I64		KEEPEFFECT_ATTACK			= 0x00010000;   // 攻击提升
const I64		KEEPEFFECT_ATKSPEED			= 0x00020000;   // 攻击速度提高

const I64		KEEPEFFECT_MAGICDAMAGE		= 0x00040000;   // 魔法伤害提高
const I64		KEEPEFFECT_MAGICDEFENCE 	= 0x00080000;   // 魔法防御提高

const I64		KEEPEFFECT_REFLECT			= 0x00100000;	// 攻击反射
const I64		KEEPEFFECT_REFLECTMAGIC		= 0x00200000;   // 魔法反射

const I64		KEEPEFFECT_SLOWDOWN1		= 0x00400000;   // 减速状态 50%
const I64		KEEPEFFECT_SLOWDOWN2		= 0x00800000;	// 减速状态 // 血少于一半时开始减速 // 50%

const I64		KEEPEFFECT_TEAM_HEALTH		= 0x01000000;	// 医疗结界状态
const I64		KEEPEFFECT_TEAM_ATTACK		= 0x02000000;	// 攻击结界状态
const I64		KEEPEFFECT_TEAM_DEFENCE		= 0x04000000;	// 护体结界状态
const I64		KEEPEFFECT_TEAM_SPEED		= 0x08000000;	// 速度结界状态
const I64		KEEPEFFECT_TEAM_EXP			= 0x10000000;	// 修炼结界状态
const I64		KEEPEFFECT_TEAM_SPIRIT		= 0x20000000;	// 心灵结界状态
const I64		KEEPEFFECT_TEAM_CLEAN		= 0x40000000;	// 净化结界状态

const I64		KEEPEFFECT_SMOKE			= 0x80000000;			// 烟雾效果
const I64		KEEPEFFECT_DARKNESS			= 0x0000000100000000;	// 黑暗效果
const I64		KEEPEFFECT_PALSY			= 0x0000000200000000;	// 麻痹效果
const I64		KEEPEFFECT_MAXLIFE			= 0x0000000400000000;	// 最大生命增加/减少
const I64		KEEPEFFECT_MAXENERGY		= 0x0000000800000000;	// 最大体力增加/减少

const I64		KEEPEFFECT_ADD_EXP			= 0x0000001000000000;	// 战斗经验增加
const I64		KEEPEFFECT_ATTRACT_MONSTER	= 0x0000002000000000;	// 吸引怪物
const I64		KEEPEFFECT_XPFULL			= 0x0000004000000000;	// XP满

const I64		KEEPEFFECT_ADJUST_DODGE			= 0x0000008000000000;	// 调节总的躲避值
const I64		KEEPEFFECT_ADJUST_XP			= 0x0000010000000000;	// 调节每次增加XP值
const I64		KEEPEFFECT_ADJUST_DROPMONEY		= 0x0000020000000000;	// 调节怪物每次掉钱

const I64		KEEPEFFECT_PK_PROTECT			= 0x0000040000000000;	// pk保护状态
const I64		KEEPEFFECT_PELT					= 0x0000080000000000;	// 疾行状态
const I64		KEEPEFFECT_ADJUST_EXP			= 0x0000100000000000;	// 战斗获得经验调整

const I64		KEEPEFFECT_HEAL					= 0x0000200000000000;	// 治愈状态
const I64		KEEPEFFECT_FAINT				= 0x0000400000000000;	// 晕
const I64		KEEPEFFECT_TRUCULENCE			= 0x0000800000000000;	// 野蛮
const I64		KEEPEFFECT_DAMAGE				= 0x0001000000000000;	// 调整受到的伤害
const I64		KEEPEFFECT_ATKER_DAMAGE			= 0x0002000000000000;	// 调整对目标造成的伤害
const I64		KEEPEFFECT_CONFUSION			= 0x0004000000000000;	// 混乱
const I64		KEEPEFFECT_FRENZY				= 0x0008000000000000;	// 狂暴
const I64		KEEPEFFECT_EXTRA_POWER			= 0x0010000000000000;	// 神力
const I64		KEEPEFFECT_TRANSFER_SHIELD		= 0x0020000000000000;	// 护盾
const I64		KEEPEFFECT_SORB_REFLECT			= 0x0040000000000000;	// 吸收反射
const I64		KEEPEFFECT_FRENZY2				= 0X0080000000000000;	// 另一种狂暴状态

//////////////////////////////////////////////////////////////////////
// 综合状态
//enum {
const I64		KEEPEFFECT_NOT_ACTIVE		= KEEPEFFECT_DIE | KEEPEFFECT_FREEZE | KEEPEFFECT_PALSY;
const I64		KEEPEFFECT_NOT_CLEAR		= KEEPEFFECT_TEAMLEADER | KEEPEFFECT_RED | KEEPEFFECT_DEEPRED;		// 玩家死亡不清此状态
// 结界系统所有状态
const I64		KEEPEFFECT_TEAM_ALL			= KEEPEFFECT_TEAM_HEALTH | KEEPEFFECT_TEAM_ATTACK | KEEPEFFECT_TEAM_DEFENCE
        | KEEPEFFECT_TEAM_SPEED | KEEPEFFECT_TEAM_EXP | KEEPEFFECT_TEAM_SPIRIT | KEEPEFFECT_TEAM_CLEAN;
//};

//////////////////////////////////////////////////////////////////////
// 内部状态，不需要通过MsgPlayer通知其它玩家的状态。服务器内部使用(cq_magictype使用)
enum
{
    STATUS_NORMAL		= 0,
    STATUS_DIE			= 1,										// 死亡
    STATUS_CRIME		= 2,										// 犯罪闪蓝状态
    STATUS_POISON		= 3,										// 中毒
    STATUS_TEAMLEADER	= 4,										// 队长
    STATUS_PKVALUE		= 5,										// PK状态
    STATUS_DETACH_BADLY	= 6,										// 清除所有不良状态
    STATUS_DETACH_ALL	= 7,										// 清除所有魔法状态
    STATUS_VAMPIRE		= 8,										// ATKSTATUS中吸血
    STATUS_DISAPPEARING	= 9,										// 尸体消失状态
    STATUS_MAGICDEFENCE	= 10,									// 魔法防御提升/下降
    STATUS_SUPER_MDEF	= 11,									// 超级魔防
    STATUS_ATTACK		= 12,									// 攻击提升/下降
    STATUS_REFLECT		= 13,									// 攻击反射
    STATUS_HIDDEN		= 14,									// 隐身
    STATUS_MAGICDAMAGE	= 15,									// 魔法伤害提升/下降
    STATUS_ATKSPEED		= 16,									// 攻击速度提升/下降
    STATUS_LURKER		= 17,									// user only			// 潜行，此状态下不受NPC攻击，对玩家无效
    STATUS_SYNCRIME		= 18,									// 帮派犯罪
    STATUS_REFLECTMAGIC	= 19,									// 魔法反射
    STATUS_SUPER_DEF	= 20,									// 超级防御
    STATUS_SUPER_ATK	= 21,									// self only	// 超级攻击
    STATUS_SUPER_MATK	= 22,		 							// self only	// 超级魔攻
    STATUS_STOP			= 23,
    STATUS_DEFENCE1		= 24,									// 防御提高/降低1
    STATUS_DEFENCE2		= 25,									// 防御提高/降低2
    STATUS_DEFENCE3		= 26,									// 防御提高/降低3
    STATUS_FREEZE		= 27,									// 冰冻状态
    STATUS_SMOKE		= 28,									// 烟雾效果
    STATUS_DARKNESS		= 29,									// 黑暗效果
    STATUS_PALSY		= 30,									// 麻痹效果

    STATUS_TEAM_BEGIN	= 31,
    STATUS_TEAMHEALTH	= 31,									// 医疗结界
    STATUS_TEAMATTACK	= 32,									// 攻击结界
    STATUS_TEAMDEFENCE	= 33,									// 护体结界
    STATUS_TEAMSPEED	= 34,									// 速度结界
    STATUS_TEAMEXP		= 35,									// 修炼结界
    STATUS_TEAMSPIRIT 	= 36,									// 心灵结界
    STATUS_TEAMCLEAN	= 37,									// 净化结界
    STATUS_TEAM_END		= 37,

    STATUS_SLOWDOWN1	= 38,									// 移动速度提升/下降
    STATUS_SLOWDOWN2	= 39,									// 降低速度（仅在生命低于一半的时候。客户端表现）
    STATUS_MAXLIFE		= 40,									// 最大生命增加/降低
    STATUS_MAXENERGY	= 41,									// 最大体力增加/降低
    STATUS_DEF2ATK		= 42,									// 防御转换为攻击(power=被转换的防御百分比)
    STATUS_ADD_EXP		= 43,						// 战斗经验增加 -- 只能对队长使用，类似修炼结界效果
    STATUS_DMG2LIFE		= 44,									// 每次攻击伤害部分转换为自己的生命(power=被转换的百分比)
    STATUS_ATTRACT_MONSTER	= 45,								// 吸引怪物
    STATUS_XPFULL		= 46,									// XP满
    //---jinggy---begin
    STATUS_ADJUST_DODGE	= 47,			//调节总的躲避值
    STATUS_ADJUST_XP	= 48,			//调节每次增加XP值
    STATUS_ADJUST_DROPMONEY	= 49,		//调节怪物每次掉钱
    //---jinggy---end
    STATUS_PK_PROTECT	= 50,									// PK保护状态
    STATUS_PELT			= 51,									// 疾行状态
    STATUS_ADJUST_EXP	= 52,									// 战斗获得经验调整——允许对任何人使用

    /////////////////////////////////
    // 幻兽技能需要用到的状态
    STATUS_HEAL				= 100,			// 治愈
    STATUS_FAINT			= 101,			// 打晕
    STATUS_TRUCULENCE		= 102,			// 野蛮
    STATUS_DAMAGE			= 103,			// 调整受到的伤害
    STATUS_ATKER_DAMAGE		= 104,			// 调整对目标造成的伤害
    STATUS_CONFUSION		= 105,			// 混乱
    STATUS_FRENZY			= 106,			// 狂暴
    STATUS_EXTRA_POWER		= 107,			// 神力
    STATUS_TRANSFER_SHIELD	= 108,			// 护盾
    STATUS_SORB_REFLECT		= 109,			// 吸收反射
    STATUS_FRENZY2			= 110,			// 另外一种狂暴状态

    /////////////////////////////////

    STATUS_LOCK			= 251,									// 被锁定状态 -- only used in server

    STATUS_LIMIT		= 256,						// 角色状态不能超过这个值
};

//////////////////////////////////////////////////////////////////////
struct StatusInfoStruct;
class IStatus
{
public:
    virtual OBJID	GetID()							PURE_VIRTUAL_FUNCTION_0
    virtual ULONG	ReleaseByOwner()				PURE_VIRTUAL_FUNCTION_0

    virtual bool		IsValid()							PURE_VIRTUAL_FUNCTION_0
    virtual int			GetPower()							PURE_VIRTUAL_FUNCTION_0
    virtual bool		GetInfo(StatusInfoStruct* pInfo)	PURE_VIRTUAL_FUNCTION_0
    virtual void		OnTimer(DWORD tCurr)				PURE_VIRTUAL_FUNCTION
    virtual bool		ChangeData(int nPower, int nSecs, int nTimes = 0)		PURE_VIRTUAL_FUNCTION_0
    virtual bool		IncTime(int nMilliSecs, int nLimit)		PURE_VIRTUAL_FUNCTION_0
    virtual bool		ToFlash()							PURE_VIRTUAL_FUNCTION_0
};

typedef	IGameObjSet<IStatus>		IStatusSet;
typedef	CGameObjSet<IStatus>		CStatusSet;

////////////////////////////////////////////////////////////////
enum {	MOVEMODE_WALK = 0,					// PathMove()的模式
        MOVEMODE_RUN,
        MOVEMODE_SHIFT,						// to server only
        MOVEMODE_JUMP,
        MOVEMODE_TRANS,
        MOVEMODE_CHGMAP,
        MOVEMODE_JUMPMAGICATTCK,
        MOVEMODE_COLLIDE,
        MOVEMODE_SYNCHRO,					// to server only
        MOVEMODE_TRACK,

        MOVEMODE_RUN_DIR0 = 20,

        MOVEMODE_RUN_DIR7 = 27,
     };

////////////////////////////////////////////////////////////////
const int _DELTA_X[9] = { 0, -1, -1, -1, 0, 1, 1, 1, 0 };
const int _DELTA_Y[9] = { 1, 1, 0, -1, -1, -1, 0, 1, 0 };
const int MAX_DIRSIZE = 8;
inline int GetDirByPos(int nFromX, int nFromY, int nToX, int nToY)		// return MAX_DIRSIZE: error
{
    if (nFromX < nToX)
    {
        if (nFromY < nToY)
        {
            return 7;
        }
        else if (nFromY > nToY)
        {
            return 5;
        }
        else
        {
            return 6;
        }
    }
    else if (nFromX > nToX)
    {
        if (nFromY < nToY)
        {
            return 1;
        }
        else if (nFromY > nToY)
        {
            return 3;
        }
        else
        {
            return 2;
        }
    }
    else // if (nFromX == nToX)
    {
        if (nFromY < nToY)
        {
            return 0;
        }
        else if (nFromY > nToY)
        {
            return 4;
        }
    }
    return MAX_DIRSIZE;
}
inline int		MaskLook(int look)							{ return look / 10; }
inline int		MaskDir(int look)							{ return look % 10; }
inline DWORD	MakeLookFace(int nMaskLook, int nMaskDir)	{ return (nMaskLook * 10) + (nMaskDir % 10); }
inline float	GetRadian(float posSourX, float posSourY, float posTargetX, float posTargetY)
{
    CHECKF(posSourX != posTargetX || posSourY != posTargetY);
    float PI = static_cast<float>(3.1415926535);
    float fDeltaX = posTargetX - posSourX;
    float fDeltaY = posTargetY - posSourY;
    float fDistance	= sqrt(fDeltaX * fDeltaX + fDeltaY * fDeltaY);
    CHECKF(fDeltaX <= fDistance && fDistance > 0);
    float fRadian = asin(fDeltaX / fDistance);
    return fDeltaY > 0 ? (PI / 2 - fRadian) : (PI + fRadian + PI / 2);
}

//////////////////////////////////////////////////////////////////////
class IRole;
class IMapThing
{
public: // base
    virtual void	ReleaseByOwner()							PURE_VIRTUAL_FUNCTION
    virtual OBJID	GetID()										PURE_VIRTUAL_FUNCTION_0
    virtual int 	GetObjType()								PURE_VIRTUAL_FUNCTION_0
    virtual int		GetPosX()									PURE_VIRTUAL_FUNCTION_0
    virtual int		GetPosY()									PURE_VIRTUAL_FUNCTION_0
    virtual void	SendShow(IRole* pRole)						PURE_VIRTUAL_FUNCTION

    // return true: ppv返回对象指针
    virtual bool	QueryObj(OBJID idObjType, void** ppv)				PURE_VIRTUAL_FUNCTION_0
    virtual bool	QueryRole(void** ppv)						PURE_VIRTUAL_FUNCTION_0
};

//////////////////////////////////////////////////////////////////////
class IMapThing;
class IStatus;
class CTransformation;
class CTeam;
class CItem;
class CMagic;
class IRole
{
protected:
    IRole() {}
    ~IRole() {}
public:
    virtual void	ReleaseByOwner		()						PURE_VIRTUAL_FUNCTION
public: // get attrib
    virtual OBJID	GetID				()						PURE_VIRTUAL_FUNCTION_0
    virtual LPCTSTR	GetName				()						PURE_VIRTUAL_FUNCTION_0
    //	virtual LPCTSTR	GetMate				()						PURE_VIRTUAL_FUNCTION_0

    virtual DWORD	GetLookFace			()						PURE_VIRTUAL_FUNCTION_0
    //以后要分解成下面两个接口
    //virtual int	GetLook				()						PURE_VIRTUAL_FUNCTION_0
    //virtual int	GetFace				()						PURE_VIRTUAL_FUNCTION_0
    virtual DWORD	GetHair				()						PURE_VIRTUAL_FUNCTION_0
    virtual char	GetLength			()						PURE_VIRTUAL_FUNCTION_0		// 取得角色高矮 -- zlong 2004-02-03
    virtual char	GetFat				()						PURE_VIRTUAL_FUNCTION_0		// 取得角色胖瘦 -- zlong 2004-02-03
    virtual int 	GetPosX				()						PURE_VIRTUAL_FUNCTION_0
    virtual int 	GetPosY				()						PURE_VIRTUAL_FUNCTION_0
    virtual int 	GetDir				()						PURE_VIRTUAL_FUNCTION_0
    //	virtual	BOOL	CheckStatus			(int nStatus)			PURE_VIRTUAL_FUNCTION_0
    virtual I64		GetEffect			()						PURE_VIRTUAL_FUNCTION_0
    virtual int 	GetPose				()						PURE_VIRTUAL_FUNCTION_0
    //	virtual DWORD	GetMoney			()						PURE_VIRTUAL_FUNCTION_0
    //	virtual DWORD	GetMoneySaved		()						PURE_VIRTUAL_FUNCTION_0
    //	virtual int  	GetExp				()						PURE_VIRTUAL_FUNCTION_0
    virtual DWORD	GetLev				()						PURE_VIRTUAL_FUNCTION_0

    virtual DWORD	GetLife				()						PURE_VIRTUAL_FUNCTION_0
    //	virtual DWORD	GetMana				()						PURE_VIRTUAL_FUNCTION_0
    virtual DWORD	GetMaxLife			()						PURE_VIRTUAL_FUNCTION_0
    //	virtual DWORD	GetMaxMana			()						PURE_VIRTUAL_FUNCTION_0

    virtual DWORD	GetMinAtk			()						PURE_VIRTUAL_FUNCTION_0
    virtual DWORD	GetMaxAtk			()						PURE_VIRTUAL_FUNCTION_0
    virtual DWORD	GetMgcMinAtk			()						PURE_VIRTUAL_FUNCTION_0
    virtual DWORD	GetMgcMaxAtk			()						PURE_VIRTUAL_FUNCTION_0
    virtual DWORD	GetAttack			()						PURE_VIRTUAL_FUNCTION_0
    virtual DWORD	GetAtkHitRate		()						PURE_VIRTUAL_FUNCTION_0

    virtual DWORD	GetDef				()						PURE_VIRTUAL_FUNCTION_0
    virtual DWORD	GetDefence			()						PURE_VIRTUAL_FUNCTION_0

    virtual DWORD	GetDex				()						PURE_VIRTUAL_FUNCTION_0
    virtual DWORD	GetDexterity		()						PURE_VIRTUAL_FUNCTION_0

    virtual DWORD	GetDdg				()						PURE_VIRTUAL_FUNCTION_0
    virtual DWORD	GetDodge			()						PURE_VIRTUAL_FUNCTION_0

    //	virtual DWORD	GetMagicAtk			()						PURE_VIRTUAL_FUNCTION_0
    virtual DWORD	GetMagicDef			()						PURE_VIRTUAL_FUNCTION_0

    virtual	DWORD	GetInterAtkRate		()						PURE_VIRTUAL_FUNCTION_0
    virtual DWORD	GetIntervalAtkRate	()						PURE_VIRTUAL_FUNCTION_0

    virtual int		GetHelmetTypeID		()						PURE_VIRTUAL_FUNCTION_0
    virtual int		GetArmorTypeID		()						PURE_VIRTUAL_FUNCTION_0
    virtual int		GetWeaponRTypeID	()						PURE_VIRTUAL_FUNCTION_0
    virtual int 	GetWeaponLTypeID	()						PURE_VIRTUAL_FUNCTION_0
    //	virtual int		GetNecklaceTypeID	()						PURE_VIRTUAL_FUNCTION_0
    //	virtual int 	GetRingRTypeID		()						PURE_VIRTUAL_FUNCTION_0
    //	virtual int 	GetRingLTypeID		()						PURE_VIRTUAL_FUNCTION_0
    //	virtual int 	GetShoesTypeID		()						PURE_VIRTUAL_FUNCTION_0
    virtual int 	GetMountTypeID		()						PURE_VIRTUAL_FUNCTION_0
    virtual int		GetMantleTypeID		()						PURE_VIRTUAL_FUNCTION_0

    //	virtual DWORD 	GetMedalSelect		()						PURE_VIRTUAL_FUNCTION_0
    //	virtual DWORD	GetTitleSelect		()						PURE_VIRTUAL_FUNCTION_0

    virtual int		GetDistance			(int x, int y)			PURE_VIRTUAL_FUNCTION_0
    virtual int		GetDistance			(IMapThing* pTarget)	PURE_VIRTUAL_FUNCTION_0

    virtual CGameMap*	GetMap			()						PURE_VIRTUAL_FUNCTION_0

    virtual DWORD	GetSynID			()						PURE_VIRTUAL_FUNCTION_0
    virtual DWORD	GetSynRank			()						PURE_VIRTUAL_FUNCTION_0
    virtual DWORD	GetSynRankShow		()						PURE_VIRTUAL_FUNCTION_0

    virtual bool	IsSimpleMagicAtk	()						PURE_VIRTUAL_FUNCTION_0
    virtual int 	GetSimpleMagicHitRate()						PURE_VIRTUAL_FUNCTION_0

    virtual int		GetTutorExp			()						PURE_VIRTUAL_FUNCTION_0
    virtual UCHAR	GetTutorLevel		()						PURE_VIRTUAL_FUNCTION_0
    virtual UCHAR	GetMercenaryRank	()						PURE_VIRTUAL_FUNCTION_0
    virtual DWORD	GetMercenaryExp		()						PURE_VIRTUAL_FUNCTION_0
    virtual UCHAR	GetNobilityRank		()						PURE_VIRTUAL_FUNCTION_0
    virtual DWORD	GetExploit			()						PURE_VIRTUAL_FUNCTION_0
public: // const

    //	virtual int		GetSex				()						PURE_VIRTUAL_FUNCTION_0
    virtual bool	IsAlive				()						PURE_VIRTUAL_FUNCTION_0
    virtual bool	IsTalkEnable		()						PURE_VIRTUAL_FUNCTION_0
    virtual bool	IsGM				()						PURE_VIRTUAL_FUNCTION_0
    virtual bool	IsPM				()						PURE_VIRTUAL_FUNCTION_0
    virtual bool	IsMonster			()						PURE_VIRTUAL_FUNCTION_0
    //	virtual bool	IsScene				()						PURE_VIRTUAL_FUNCTION_0
    virtual bool	IsDelThis			()						PURE_VIRTUAL_FUNCTION_0
    virtual bool	IsVirtuous()								PURE_VIRTUAL_FUNCTION_0
    virtual bool	IsEvil()									PURE_VIRTUAL_FUNCTION_0
    virtual bool	IsBowman()									PURE_VIRTUAL_FUNCTION_0
    virtual bool	IsShieldEquip()								PURE_VIRTUAL_FUNCTION_0
    virtual bool	IsWing()									PURE_VIRTUAL_FUNCTION_0
    virtual bool	IsSendBlockInfo		()						PURE_VIRTUAL_FUNCTION_0

    // return true: ppv返回对象指针
    virtual bool	QueryObj			(OBJID idObjType, void** ppv)	PURE_VIRTUAL_FUNCTION_0
    virtual IMapThing*	QueryMapThing	()						PURE_VIRTUAL_FUNCTION_0
    virtual IRole*	FindAroundRole		(OBJID id)				PURE_VIRTUAL_FUNCTION_0
    virtual CUser*	QueryOwnerUser		()						PURE_VIRTUAL_FUNCTION_0

public:	// modify attrib ------------------------------
    virtual void	SetDir				(int nDir)							PURE_VIRTUAL_FUNCTION
    virtual void	SetPose				(int nPose)							PURE_VIRTUAL_FUNCTION
    //	virtual void	SetEmotion			(int nEmotion)						PURE_VIRTUAL_FUNCTION
    virtual	void	SetStatus			(int nStatus, bool bSynchro = true)						PURE_VIRTUAL_FUNCTION
    virtual	void	ClsStatus			(int nStatus, bool bSynchro = true)						PURE_VIRTUAL_FUNCTION
    virtual bool	AddAttrib			(int idxAttr, __int64 i64Data, int bSynchro)					PURE_VIRTUAL_FUNCTION_0
    virtual void	ProcessOnMove		(int nMoveMode)						PURE_VIRTUAL_FUNCTION
    virtual void	ProcessAfterMove	()									PURE_VIRTUAL_FUNCTION
    virtual void	GetFootPrint		(int& x, int& y)					PURE_VIRTUAL_FUNCTION

    virtual bool	SpendArrow			()									PURE_VIRTUAL_FUNCTION_0

    virtual	IStatus*	QueryStatus(OBJID idType)										PURE_VIRTUAL_FUNCTION_0
    virtual	IStatusSet*	QueryStatusSet()												PURE_VIRTUAL_FUNCTION_0
    virtual void	AwardBattleExp		(int nExp, bool bGemEffect = true)				PURE_VIRTUAL_FUNCTION
    virtual int 	AdjustHitRate		(int nHitRate)									PURE_VIRTUAL_FUNCTION_0
    virtual int 	AdjustAttack		(int nAtk)										PURE_VIRTUAL_FUNCTION_0
    virtual int 	AdjustDefence		(int nDef)										PURE_VIRTUAL_FUNCTION_0
    virtual int 	AdjustMagicAtk		(int nAtk)										PURE_VIRTUAL_FUNCTION_0
    virtual int 	AdjustMagicDef		(int nDef)										PURE_VIRTUAL_FUNCTION_0
    virtual int 	AdjustBowDefence	(int nDef)										PURE_VIRTUAL_FUNCTION_0
    virtual int 	AdjustWeaponDamage	(int nDamage)									PURE_VIRTUAL_FUNCTION_0
    virtual int 	AdjustMagicDamage	(int nDamage)									PURE_VIRTUAL_FUNCTION_0
    virtual void	SetFightPause		(int nInterval)									PURE_VIRTUAL_FUNCTION
    virtual void	SaveInfo			()												PURE_VIRTUAL_FUNCTION

public: // money --------------------------------
    //	virtual bool	SpendMoney			(DWORD nMoney, bool bSynchro)			PURE_VIRTUAL_FUNCTION_0
    //	virtual bool	GainMoney			(DWORD nMoney, bool bSynchro)			PURE_VIRTUAL_FUNCTION_0

public: // interactive
    //	virtual bool	AddItem				(CItem* pItem)								PURE_VIRTUAL_FUNCTION_0
    //	virtual bool	IsItemFull			()											PURE_VIRTUAL_FUNCTION_0

public: // system
    virtual void	BroadcastRoomMsg(CNetMsg* pMsg, bool bSendSelf = false)	PURE_VIRTUAL_FUNCTION
    virtual void	BroadcastRoomMsg(LPCTSTR szMsg, bool bSendSelf = false)	PURE_VIRTUAL_FUNCTION
    //	virtual void	BroadcastMapMsg(CNetMsg* pMsg, bool bSendSelf = false)	PURE_VIRTUAL_FUNCTION
    //	virtual void	BroadcastMapMsg(LPCTSTR szMsg, bool bSendSelf = false)	PURE_VIRTUAL_FUNCTION
    virtual bool	SendMsg(CNetMsg* pMsg)									PURE_VIRTUAL_FUNCTION_0
    virtual bool	SendSysMsg(const char* fmt, ...)						PURE_VIRTUAL_FUNCTION_0
    virtual void	OnTimer(time_t tCurr)									PURE_VIRTUAL_FUNCTION

public: // map
    virtual bool	MoveToward(int nDir, bool bSync = true)					PURE_VIRTUAL_FUNCTION_0
    virtual void	TransPos(int nPosX, int nPosY)							PURE_VIRTUAL_FUNCTION
    virtual void	JumpPos(int nPosX, int nPosY)							PURE_VIRTUAL_FUNCTION
    virtual bool	SyncTrackTo(int nPosX, int nPosY, int nDir, DWORD dwAction)				PURE_VIRTUAL_FUNCTION_0
    virtual void	ActiveTransPos(int nPosX, int nPosY)					PURE_VIRTUAL_FUNCTION
    virtual bool	SendSelfToBlock()										PURE_VIRTUAL_FUNCTION_0

    virtual void	AddToBCRoleSet			(OBJID idRole, bool bSendMsg)	PURE_VIRTUAL_FUNCTION
    virtual void	RemoveFromBCRoleSet		(OBJID idRole)					PURE_VIRTUAL_FUNCTION
    virtual void	AddToBCMapItemSet		(OBJID idMapItem)				PURE_VIRTUAL_FUNCTION
    virtual void	RemoveFromBCMapItemSet	(OBJID idMapItem)				PURE_VIRTUAL_FUNCTION

public: // fight ------------------------------
    virtual void	ClrAttackTarget		()									PURE_VIRTUAL_FUNCTION
    virtual bool	SetAttackTarget(IRole* pTarget = NULL)					PURE_VIRTUAL_FUNCTION_0
    virtual bool	AutoSkillAttack(IRole* pTarget)							PURE_VIRTUAL_FUNCTION_0
    virtual bool	AdditionMagic(int nLifeLost, int nDamage)				PURE_VIRTUAL_FUNCTION_0
    virtual int		Attack(IRole* pTarget)									PURE_VIRTUAL_FUNCTION_0
    virtual bool	BeAttack(bool bMagic, IRole* pTarget, int nPower, bool bReflectEnable = true)					PURE_VIRTUAL_FUNCTION_0
    virtual bool	MagicAttack(int nType, OBJID idTarget, int x, int y)	PURE_VIRTUAL_FUNCTION_0
    virtual void	Kill(IRole* pTarget, DWORD dwDieWay)									PURE_VIRTUAL_FUNCTION
    virtual void	BeKill(IRole* pTarget = NULL)							PURE_VIRTUAL_FUNCTION
    virtual bool	IsAtkable(IRole* pTarget, bool bSendHint = false)			PURE_VIRTUAL_FUNCTION_0
    virtual bool	IsBeAtkable()											PURE_VIRTUAL_FUNCTION_0
    virtual int		GetAttackRange(int nTargetSizeAdd)						PURE_VIRTUAL_FUNCTION_0
    virtual int		GetSizeAdd()											PURE_VIRTUAL_FUNCTION_0
    virtual	int		GetLuck				()									PURE_VIRTUAL_FUNCTION_0
    virtual void	SendDamageMsg(OBJID idTarget, int nDamage)				PURE_VIRTUAL_FUNCTION

    virtual bool	TransferShield(bool bMagic, IRole* pAtker, int nDamage)	PURE_VIRTUAL_FUNCTION_0

public:	// 为了修改魔法系统而增加的部分，等魔法系统修改完再做整理
    virtual void	AwardSynWarScore(CNpc* pNpc, int nScore)				PURE_VIRTUAL_FUNCTION
    virtual bool	IsImmunity(IRole* pRole)								PURE_VIRTUAL_FUNCTION_0
    virtual DWORD	GetProfession()											PURE_VIRTUAL_FUNCTION_0
    virtual int		GetGemExpEffect()										PURE_VIRTUAL_FUNCTION_0
    virtual int		GetGemMgcExpEffect()									PURE_VIRTUAL_FUNCTION_0
    virtual bool	SpendEquipItem(int nSubType, int nNum, bool bSynchro)	PURE_VIRTUAL_FUNCTION_0
    virtual int		AdjustExp(IRole* pTarget, int nRawExp, bool bNewbieBonusMsg = false)		PURE_VIRTUAL_FUNCTION_0
    virtual bool	DecEquipmentDurability(bool bBeAttack, bool bMagic, int bDurValue = 1)	PURE_VIRTUAL_FUNCTION_0
    virtual void	SendGemEffect()											PURE_VIRTUAL_FUNCTION
    virtual bool	IsEmbedGemType(int nGemType)							PURE_VIRTUAL_FUNCTION_0

    virtual DWORD	GetMana()												PURE_VIRTUAL_FUNCTION_0
    virtual DWORD	GetMaxMana()											PURE_VIRTUAL_FUNCTION_0

    virtual int		GetEnergy()												PURE_VIRTUAL_FUNCTION_0
    virtual bool	CheckWeaponSubType(int nSubType, int nNum = 0)			PURE_VIRTUAL_FUNCTION_0
    virtual DWORD	GetPotential()											PURE_VIRTUAL_FUNCTION_0
    virtual CTransformation* QueryTransformation()							PURE_VIRTUAL_FUNCTION_0
    virtual bool	IsArrowPass(int x, int y, int nAlt = ARROW_FLY_ALT)		PURE_VIRTUAL_FUNCTION_0
    virtual CTeam*	GetTeam()												PURE_VIRTUAL_FUNCTION_0
    virtual CItem*	GetEquipItemByPos(int nPosition)						PURE_VIRTUAL_FUNCTION_0
    virtual bool	CheckCrime(IRole* pRole)								PURE_VIRTUAL_FUNCTION_0
    virtual void	SetDelay(int nDelay)									PURE_VIRTUAL_FUNCTION

    virtual CItem*	GetWeaponR			()									PURE_VIRTUAL_FUNCTION_0
    virtual CItem*	GetWeaponL			()									PURE_VIRTUAL_FUNCTION_0

    virtual bool	DropItem(OBJID idItem, int x, int y)					PURE_VIRTUAL_FUNCTION_0
    virtual DWORD	GetSoulSum()											PURE_VIRTUAL_FUNCTION_0
    virtual void	AddEquipmentDurability(int nPosition, int nAddValue)	PURE_VIRTUAL_FUNCTION
    virtual bool	SynPosition(int nPosX, int nPosY, int nMaxDislocation = 8)						PURE_VIRTUAL_FUNCTION_0
    virtual bool	Transform(DWORD dwType, int nKeepSecs, bool bSynchro = true)					PURE_VIRTUAL_FUNCTION_0
    virtual bool	IsInFan(POINT pos, POINT posSource, int nRange, int nWidth, POINT posCenter)	PURE_VIRTUAL_FUNCTION_0
    virtual bool	AddItem (CItem* pItem, bool bSynchro, bool bUpdate = true)						PURE_VIRTUAL_FUNCTION_0
    virtual bool	Synchro()												PURE_VIRTUAL_FUNCTION_0
    virtual int		AdjustData(int nData, int nAdjust, int nMaxData = 0)		PURE_VIRTUAL_FUNCTION_0
    virtual CItem*	FindSpaceTransSpell()									PURE_VIRTUAL_FUNCTION_0

    virtual	CMagic*	QueryMagic()											PURE_VIRTUAL_FUNCTION_0
};




#endif // !defined(I_ROLE_HEADER)
