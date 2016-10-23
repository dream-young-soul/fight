
//**********************************************************
// 代码编辑器
//**********************************************************

#ifndef _ROLE_H
#define _ROLE_H

#include "basefunc.h"
#include "Path.h"
#include "MapObj.h"
#include "IRoleView.h"
#include "AniEffect.h"
#include "SayHello.h"
#include "FloatingStr.h"
#include "Game3DEffectEx.h"
#include "ZoomNum.h"
#include <vector>
const int _MAX_LEVEL			= 100;
//----------------------------------------------------------------------------
// ID空间
enum
{
    _PLAYER_ID_NONE		= 0,
    _PLAYER_ID_MONSTER	= 1,
    _PLAYER_ID_PET		= 2,
    _PLAYER_ID_SYSNPC	= 3,
    _PLAYER_ID_DYNNPC	= 4,
    _PLAYER_ID_CALLPET	= 5,
    _PLAYER_ID_PLAYER	= 6,
};
// ID空间
/////////////////////////////////////////////////////////////////////
// ID空间
const OBJID	SCENEID_FIRST		= 000001;			// SCENE的第一个ID
const OBJID	SYSNPCID_FIRST		= 00001;			// SYSNPC的第一个ID
const OBJID	SYSNPCID_LAST		= 99999;			// SYSNPC的最后一个ID
const OBJID	DYNANPCID_FIRST		= 100001;			// DYNANPC的第一个ID
const OBJID	DYNANPCID_LAST		= 199999;			// DYNANPC的最后一个ID
const OBJID	SCENEID_LAST		= 299999;			// SCENE的最后一个ID

const OBJID	NPCSERVERID_FIRST	= 400001;			// NPC的第一个ID
const OBJID	MONSTERID_FIRST		= 400001;			// MONSTER的第一个ID
const OBJID	MONSTERID_LAST		= 499999;			// MONSTER的最后一个ID
const OBJID	PETID_FIRST			= 500001;			// PET的第一个ID
const OBJID	PETID_LAST			= 599999;			// PET的最后一个ID
const OBJID	NPCSERVERID_LAST	= 899999;			// NPC的最后一个ID

const OBJID TRAPID_FIRST		= 900001;			// 魔法陷阱的第一个ID
const OBJID MAGICTRAPID_FIRST	= 900001;			// 玩家魔法的第一个ID
const OBJID MAGICTRAPID_LAST	= 989999;			// 玩家魔法的最后一个ID
const OBJID SYSTRAPID_FIRST		= 990001;			// 系统陷阱的第一个ID
const OBJID SYSTRAPID_LAST		= 999999;			// 系统陷阱的最后一个ID
const OBJID TRAPID_LAST			= 999999;			// 魔法陷阱的最后一个ID

const OBJID PLAYER_ID_FIRST		= 1000000;			// Player的第一个ID
const OBJID PLAYER_ID_LAST		= 1999999999;		// Player的最后一个ID
const OBJID	CALLPETID_FIRST		= 2000000000;		// CALLPET的第一个ID
const OBJID	CALLPETID_LAST		= 3999999999;		// CALLPET的最后一个ID

const OBJID CLIENT_ID_FIRST		= 4000000000;		// 为客户端保留的ID空间
const OBJID MOUSE_PLAYER_ID		= 4000000001;		// 放NPC时在鼠标上显示

const OBJID _IDMAX_NPC	= 999999;

//----------------------------------------------------------------------------
// 功能类型
enum
{
    ROLE_ATTRIBUTE_TASK			= 1,			// 任务类
    ROLE_ATTRIBUTE_RECYCLE		= 2,			// 可回收类
    ROLE_ATTRIBUTE_SCENE		= 4,			// 场景类
};
//----------------------------------------------------------------------------
// 行为类型
enum
{
    _ROLE_NPC_NONE				= 0,			// 非法NPC
    _ROLE_SHOPKEEPER_NPC		= 1,			// 商店NPC
    _ROLE_TASK_NPC				= 2,			// 任务NPC
    _ROLE_STORAGE_NPC			= 3,			// 寄存处NPC
    _ROLE_TRUNCK_NPC			= 4,			// 箱子NPC
    _ROLE_FACE_NPC				= 5,			// 变头像NPC
    _ROLE_FORGE_NPC				= 6,			// 锻造NPC
    _ROLE_EMBED_NPC				= 7,			// 镶嵌NPC

    _ROLE_STATUARY_NPC			= 9,			// 雕像NPC
    _ROLE_SYNFLAG_NPC			= 10,			// 帮派标记NPC

    _ROLE_PLAYER				= 11,			// 其他玩家
    _ROLE_HERO					= 12,			// 自己
    _ROLE_MONSTER				= 13,			// 怪物

    _ROLE_BOOTH_NPC				= 14,			// 摊位NPC
    _SYNTRANS_NPC				= 15,			// 帮派传送NPC(用于00:00收费)(LINKID为固定NPC的ID，与其它使用LINKID的互斥)
    _ROLE_BOOTH_FLAG_NPC		= 16,			// 摊位标志NPC
    _ROLE_MOUSE_NPC				= 17,			// 鼠标上的NPC
    _ROLE_MAGICITEM				= 18,			// 陷阱火墙

    _ROLE_DICE_NPC				= 19,			// 骰子NPC

    _ROLE_WEAPONGOAL_NPC		= 21,			// 近身攻击NPC
    _ROLE_MAGICGOAL_NPC			= 22,			// 魔法攻击靶子NPC
    _ROLE_BOWGOAL_NPC			= 23,			// 弓箭靶子NPC

    _ROLE_TARGET_NPC			= 24,			// 挨打，不触发任务

    _ROLE_FURNITURE_NPC			= 25,			// 家具NPC
    _ROLE_CITY_GATE_NPC			= 26,			// 城门NPC
    _ROLE_NEIGHBOR_DOOR			= 27,			// 邻居的门
    _ROLE_CALL_PET				= 28,			// 召唤兽

    _TRAINPLACE_NPC				= 29,			// 幻兽驯养所
    _AUCTION_NPC				= 30,			// 拍卖NPC
    //		_ROLE_WOOD_NPC				= 30,			// 木头矿
    _ROLE_STONE_MINE			= 31,			// 石头矿
    //		_ROLE_METAL_MINE			= 32,			// 金属矿


    _ROLE_PKGAME_NPC			= 32,			// PK比赛用NPC
    _ROLE_3DFURNITURE_NPC		= 101,			// 3D家具NPC

    _ROLE_CITY_WALL_NPC			= 102,			// 城墙NPC
    _ROLE_CITY_MOAT_NPC			= 103,			// 护城河NPC

    SYN_NPC_WARLETTER			= 110,			//增加新的ＮＰＣ类型　专门用来　下战书的　帮派ＮＰＣ
};

//----------------------------------------------------------------------------
// 视图类型
enum
{
    _ROLE_VIEW_NONE			= 0,
    _ROLE_VIEW_ROLE			= 1,
    _ROLE_VIEW_STATUARY		= 2,
    _ROLE_VIEW_TARGET		= 3,
    _ROLE_VIEW_TASKNPC		= 4,
    _ROLE_VIEW_SYNSTONE		= 5,
    _ROLE_VIEW_BOOTH		= 6,
    _ROLE_VIEW_MAPMAGICOBJ	= 7,
    _ROLE_VIEW_DICENPC		= 8,
    _ROLE_VIEW_TERRAIN		= 9,
};
//----------------------------------------------------------------------------
// command define
const int _COMMAND_NULL			= 0;
const int _COMMAND_STOP			= 1;
const int _COMMAND_STANDBY		= 2;
const int _COMMAND_WALK			= 3;
const int _COMMAND_RUN			= 4;
const int _COMMAND_JUMP         = 5;
const int _COMMAND_EMOTION      = 6;
const int _COMMAND_ACTION		= 7;
const int _COMMAND_ATTACK		= 8;
const int _COMMAND_FOLLOW		= 9;
const int _COMMAND_SHITHAPPEN	= 10;
const int _COMMAND_DIE			= 11;
const int _COMMAND_SPATTACK		= 12;
const int _COMMAND_FAINT		= 13;
const int _COMMAND_RUSHATK		= 14;
const int _COMMAND_WOUND		= 15;
const int _COMMAND_DEFEND		= 16;
const int _COMMAND_WALKFORWARD	= 17;
const int _COMMAND_RUNFORWARD	= 18;
const int _COMMAND_LOCKATK		= 19;
const int _COMMAND_PICKUP		= 20;
const int _COMMAND_TEAMFOLLOW	= 21;
const int _COMMAND_POSE			= 22;
const int _COMMAND_INTONE		= 23;
const int _COMMAND_BUMP			= 24;
const int _COMMAND_MINE			= 25;
const int _COMMAND_SHOOT		= 26;
const int _COMMAND_LOCKBOOTH	= 27;

const int _COMMAND_TAKEOFF		= 28;
const int _COMMAND_LANDING		= 29;
const int _COMMAND_FLOAT		= 30;
const int _COMMAND_FLOAT_DIE	= 31;

const int _COMMAND_CHARGEUP			= 32;
const int _CPMMAND_CHARGEUP_SHOOT	= 33;

const int _COMMAND_KONGFU		= 100;

// command status define
const int _CMDSTATUS_BEGIN		= 0;
const int _CMDSTATUS_DEPART		= 1;
const int _CMDSTATUS_PROGRESS	= 2;
const int _CMDSTATUS_CONTINUE	= 3;
const int _CMDSTATUS_WAITING	= 4;
const int _CMDSTATUS_ACCOMPLISH	= 9;

// user status
const __int64				USERSTATUS_NORMAL			= 0x00000000;		 	// 无特殊状态
const __int64				USERSTATUS_TEAMLEADER		= 0x00000001;			// 队长
const __int64				USERSTATUS_DIE				= 0x00000002;			// 死亡
const __int64				USERSTATUS_GHOST			= 0x00000004;			// 灵魂状态
const __int64				USERSTATUS_DISAPPEARING		= 0x00000008;			// 尸体消失状态
const __int64				USERSTATUS_CRIME			= 0x00000010;			// 犯罪 // 闪蓝色
const __int64				USERSTATUS_RED				= 0x00000020;			// 红名
const __int64				USERSTATUS_DARKRED			= 0x00000040;			// 黑名
const __int64				USERSTATUS_SYNCRIME			= 0x00000080;			// 帮派犯罪
const __int64				USERSTATUS_POISON			= 0x00000100;			// 中毒
const __int64				USERSTATUS_INVISIBLE		= 0x00000200;			// 隐身
const __int64				USERSTATUS_CANNOTMOVE		= 0x00000400;			// 冰冻
const __int64				USERSTATUS_LURKER			= 0x00001000;			// 潜伏
const __int64				USERSTATUS_DEFENCE1			= 0x00002000;			// 防御提高1
const __int64				USERSTATUS_DEFENCE2			= 0x00004000;			// 防御提高2
const __int64				USERSTATUS_DEFENCE3			= 0x00008000;			// 防御提高3
const __int64				USERSTATUS_ATTACK			= 0x00010000;			// 攻击提升
const __int64				USERSTATUS_ATKSPEED			= 0x00020000;			// 攻击速度提高
const __int64				USERSTATUS_MAGICDAMAGE		= 0x00040000;			// 魔法伤害提高
const __int64				USERSTATUS_MAGICDEFENCE 	= 0x00080000;			// 魔法防御提高
const __int64				USERSTATUS_REFLECT			= 0x00100000;			// 攻击反射
const __int64				USERSTATUS_REFLECTMAGIC		= 0x00200000;			// 魔法反射
const __int64				USERSTATUS_SLOWDOWN1		= 0x00400000;			// 减速状态 50%
const __int64				USERSTATUS_SLOWDOWN2		= 0x00800000;			// 减速状态 // 血少于一半时开始减速 // 50%
const __int64				USERSTATUS_TEAM_HEALTH		= 0x01000000;			// 医疗结界状态
const __int64				USERSTATUS_TEAM_ATTACK		= 0x02000000;			// 攻击结界状态
const __int64				USERSTATUS_TEAM_DEFENCE		= 0x04000000;			// 护体结界状态
const __int64				USERSTATUS_TEAM_SPEED		= 0x08000000;			// 速度结界状态
const __int64				USERSTATUS_TEAM_EXP			= 0x10000000;			// 修炼结界状态
const __int64				USERSTATUS_TEAM_SPIRIT		= 0x20000000;			// 心灵结界状态
const __int64				USERSTATUS_TEAM_CLEAN		= 0x40000000;			// 净化结界状态
const __int64				USERSTATUS_SMOKE			= 0x80000000;			// 烟雾效果
const __int64				USERSTATUS_DARKNESS			= 0x0000000100000000;	// 黑暗效果
const __int64				USERSTATUS_PALSY			= 0x0000000200000000;	// 麻痹效果
const __int64				USERSTATUS_MAXLIFE			= 0x0000000400000000;	// 最大生命增加/减少
const __int64				USERSTATUS_MAXENERGY		= 0x0000000800000000;	// 最大体力增加/减少
const __int64				USERSTATUS_ADD_EXP			= 0x0000001000000000;	// 战斗经验增加
const __int64				USERSTATUS_ATTRACT_MONSTER	= 0x0000002000000000;	// 吸引怪物
const __int64				USERSTATUS_XPFULL			= 0x0000004000000000;	// XP满
const __int64				USERSTATUS_ADJUST_DODGE		= 0x0000008000000000;	// 调节总的躲避值
const __int64				USERSTATUS_ADJUST_XP		= 0x0000010000000000;	// 调节每次增加XP值
const __int64				USERSTATUS_ADJUST_DROPMONEY	= 0x0000020000000000;	// 调节怪物每次掉钱
const __int64				USERSTATUS_PK_PROTECT		= 0x0000040000000000;	// PK保护
const __int64				USERSTATUS_PELT				= 0x0000080000000000;	// 疾行状态

//新增加
const __int64				USERSTATUS_ADJUST_EXP		= 0x0000100000000000;	// 战斗获得经验调整

const __int64				USERSTATUS_HEAL				= 0x0000200000000000;	// 治愈状态
const __int64				USERSTATUS_FAINT			= 0x0000400000000000;	// 晕
const __int64				USERSTATUS_TRUCULENCE		= 0x0000800000000000;	// 野蛮
const __int64				USERSTATUS_DAMAGE			= 0x0001000000000000;	// 调整受到的伤害
const __int64				USERSTATUS_ATKER_DAMAGE		= 0x0002000000000000;	// 调整对目标造成的伤害
const __int64				USERSTATUS_CONFUSION		= 0x0004000000000000;	// 混乱
const __int64				USERSTATUS_FRENZY			= 0x0008000000000000;	// 狂暴
const __int64				USERSTATUS_EXTRA_POWER		= 0x0010000000000000;	// 神力
const __int64				USERSTATUS_TRANSFER_SHIELD	= 0x0020000000000000;	// 护盾
const __int64				USERSTATUS_SORB_REFLECT		= 0x0040000000000000;	// 吸收反射
const __int64				USERSTATUS_FRENZY2			= 0X0080000000000000;	// 另一种狂暴状态

enum {ANIEFFECT_NORMAL = 0, ANIEFFECT_CASTMAGIC = 1, ANIEFFECT_BLASTMAGIC = 2};
enum {ROLE_PLAYER = 0, ROLE_MONSTER = 1};
enum {SEX_NONE = 0, SEX_MALE, SEX_FEMALE, SEX_ASEXUAL, SEX_ALL};
enum
{
    RACE_NONE	= 0, // 未知
    RACE_HUMAN	= 2, // 人类
    RACE_SPRITE = 3, // 精灵
    RACE_DARKSPRITE = 1, // 暗精灵
};
enum
{
    PROFESSIONAL_ENCHANTER	=	10, // 法师
    PROFESSIONAL_WARRIOR	=	20, // 战士
    PROFESSIONAL_KORADJI	=	30, // 异能者
};

// others
const int OFFSET_NAMESHOW	= 120;
const int OFFSET_SYNDICATESHOW	= 102;
const DWORD TIME_INBATTLE	= 10000;

// type define
class C3DObj;
class C3DMotion;
class C3DRoleData;
typedef struct
{
    OBJID	idPlayer;
    DWORD	dwSenderCommandIndex;
    int		nAction;
    int		nFrameIndex;
    int		nLife;
    DWORD	dwColor;
} BeAttackCmdInfo;

class CCommand
{
public:
    CCommand(void)
    {
        memset(this, 0L, sizeof(CCommand));
        bHeroSendMsg = true;
        strcpy(szString0, "");
        strcpy(szString1, "");
        bAddUp = false;
		bUseFrameInterval = false;
    }
	~CCommand()
    {}

	void Format(const char* fmt, ...) { vsprintf( szCmd, fmt, (char*) ((&fmt) + 1) ); }

public:
    union
    {
        char	szCmd[256];

        struct
        {
            OBJID	idTarget;
            CMyPos	posTarget;
            int		nTargetZ;
            int		nDir;
            char	szString[_MAX_NAMESIZE];
            DWORD	dwData;
            int		nData;
            BOOL	bData;
            BOOL	bHeroSendMsg;
            char	szString0[_MAX_STRING];
            char	szString1[_MAX_STRING];
            int		nFrameStart;
            int		nFrameEnd;
            int		nFrameInterval; //用于不确定总帧数的情况绘制动画
			int		timeActionTime; //其余得到总帧数的动作按这个变量来 技能的不确定的帧数按nFrameInterval来
            BOOL	bAddUp;
            DWORD	dwIndex;
			BOOL    bUseFrameInterval;	//是否使用nFrameInterval
        };
    };
	
    int		iType;
    int		iStatus;
};

class CPath;

class CRoleInfo
{
public:
	CRoleInfo()
	{
		Reset();
	}

	void Reset()
	{
		id				= 0;
		idMonsterType	= 0;
		usLook			= 0;
		usHair			= 0;
		usFace			= 0;
		usPose			= 0;
		ucSprite		= 0;
		idArmorType		= 0;
		idRWeaponType	= 0;
		idLWeaponType	= 0;
		idMountType		= 0;
		idArmetType		= 0;
		idMantleType	= 0;

		iSize			= 0;
		iAddSize		= 0;
		nSex			= 0;
		nTransformLook	= 0;
		nRace			= 0;
		iDirection		= 0;
		iAngle			= 0;
		iZoomPercent	= 0;

		//posRush			= 0;
		//posMap			= 0;
		//posWorld		= 0;
		//posScr			= 0;
		//posNext			= 0;
		//posActionBegin	= 0;
		//posActionEnd	= 0;	// world pos
		//
		//posDir			= 0;
		iActType		= 0;
		iActFrameIndex	= 0;
		iActFrameAmount	= 0;
		timeFirstFrame	= 0;
		timeFrameInterval	= 0;
		timeActionTime = 0;
		//objPath			= 0;
		//cmdProc			= 0;
		//sizeBase		= 0;
		nFlyOffset		= 0;
		nFlyOffsetGoal	= 0;
		bAdjustDir		= 0;
		bAdjustRun		= 0;
		nNextStep		= 0;
		dwSpeed			= 0;

		memset(szName, 0, sizeof(szName));
	}

    OBJID	id;
    OBJID   idMonsterType;
    USHORT	usLook, usHair, usFace, usPose;
    UCHAR	ucSprite;
    OBJID	idArmorType, idRWeaponType, idLWeaponType, idMountType, idArmetType, idMantleType;
    char	szName[_MAX_NAMESIZE];

    int		iSize, iAddSize, nSex, nTransformLook, nRace;
    int		iDirection, iAngle;
    int		iZoomPercent;

    CMyPos	posRush;
    CMyPos	posMap, posWorld, posScr, posNext;
    CMyPos	posActionBegin, posActionEnd;	// world pos
    int		nPosActionBeginZ, nPosActionEndZ;
    int		nZ;
    CMyPos	posDir;
    int		iActType;
    int		iActFrameIndex;
    int		iActFrameAmount;
    DWORD	timeFirstFrame;
    DWORD	timeFrameInterval;
	DWORD	timeActionTime;	// 改为计算动画所需时间，毫秒为单位
    CPath		objPath;
    CCommand	cmdProc;
    CMySize		sizeBase;
    int			nFlyOffset;
    int			nFlyOffsetGoal;
    BOOL		bAdjustDir;
    BOOL		bAdjustRun;
    int			nNextStep;
    DWORD		dwSpeed;
};


#include <list>
using namespace std;

typedef list<UINT> LIST_DIR;

class CRole : public CMapObj
{
private:
	// 重新计算动画所需要的总时间
	DWORD	GetActionTotalTime(int nActType);
    void	StaticAction(int nAction, int nDir = -1);

    BOOL	Step		(void);
    BOOL	Walk		(void);
    BOOL	WalkForward	(void);
    BOOL	Run			(void);
    BOOL	RunForward	(void);
    BOOL	Jump		(void);
    BOOL    Emotion		(void);
    BOOL	Attack		(void);
    BOOL	RushAtk		(void);
    BOOL	LockAtk		(void);
    BOOL	TeamFollow	(void);
    BOOL	Follow		(void);
    BOOL	Action		(void);
    BOOL	Die			(void);
    BOOL	DieNormal	(void);
    BOOL	DieDelay	(void);
    BOOL	DieFlying	(void);
    BOOL	DieFloat	(void);

    BOOL	PickUp		(void);
    BOOL	Pose		(void);
    BOOL	InTone		(void);
    BOOL	Bump		(void);
    BOOL	Mine		(void);
    BOOL	LockBooth	(void);
    BOOL	TakeOff		(void);
    BOOL	Landing		(void);
    BOOL	Float		(void);
    BOOL	Kongfu		(void);
protected:
    int		GetOffsetY();
    BOOL	StandBy		(void);

private:
    int		GetStepDir		(void);
    BOOL	IsMapReached	(void);
    int		GetJumpHeight	();
    int		GetJumpShadowHeight();
    DWORD	GetJumpFrameInterval();
    int     GetWalkHeight	();

public:
    CRole(void) {Init();}
    virtual ~CRole(void) {Destroy();}

    void	Init			(void);

    BOOL	Create	(UINT uLook = 1, int nViewType = _ROLE_VIEW_ROLE);
    BOOL	Create	(UINT uLook, int nViewType, OBJID id, CMyPos pos);
    void	Destroy	(void);

    virtual void	GetWorldPos	(int& iPosX, int& iPosY)	{iPosX = m_Info.posWorld.x, iPosY = m_Info.posWorld.y;}
    virtual void	GetScrPos	(int& iPosX, int& iPosY)	{iPosX = m_Info.posScr.x, iPosY = m_Info.posScr.y;}
    virtual void	GetPos		(int& iPosX, int& iPosY)	{iPosX = m_Info.posMap.x, iPosY = m_Info.posMap.y;}

    virtual void	GetSize		(int& iWidth, int& iHeight)	{iWidth = iHeight = m_Info.iSize;}
    virtual void	Show		(void);
    virtual void	Process		(void* pInfo);


    virtual BOOL	ProcessAction	(void);

    virtual BOOL	IsMouseFocus(int evtMouse = _MOUSE_NONE);
    virtual BOOL	IsVisible(void);

    virtual int		GetAtkRange	(void) { return 1; }

    void	SetActionData	(void);
    void	ResetActionData	(void);

    DWORD   GetFrameInterval(int nActType);
	BOOL    IsUseFrameInterval(); //是否使用帧间隔模式
    DWORD	GetDistance	(CRole* pRole);
    BOOL	IsMapAttainable	(int iMapX, int iMapY);
    BOOL	IsWalking	(void);
    BOOL	IsRunning	(void);
    BOOL	IsMoving	(void);
    BOOL	IsStandBy	(void);
    BOOL	IsIdle		(void);		// 空闲状态, 表示当前人物任务处在最低优先级别
    BOOL	IsBumpBackEnable	(void);

    DWORD	GetActionDelayInfoKey	(CRole* pTarget);
    DWORD	GetWeaponInfoKey		(CRole* pTarget);

    void	AddStepDir	(UINT uDir)	{m_lstStepDir.push_back(uDir % 8);}
    void	ResetStepDir();

    BOOL	SetPos		(int iPosX, int iPosY);
    void    SetNextPos  (int iPosX, int iPosY)  {m_Info.posNext.x = iPosX; m_Info.posNext.y = iPosY;}
    void	SetScrPos	(int iPosX, int iPosY)	{m_Info.posScr.x = iPosX; m_Info.posScr.y = iPosY;}
    void	SetRushPos	(int iPosX, int iPosY)	{m_Info.posRush.x = iPosX, m_Info.posRush.y = iPosY;}
    void	ClrRushPos	(void)	{m_Info.posRush.x = m_Info.posRush.y = 0;}

    void	ShowName	(void);
    void	Say			(const char* pszWords);

    DWORD	GetSpeed() const {return m_Info.dwSpeed;}
    void	SetSpeed(DWORD dwSpeed) {m_Info.dwSpeed = dwSpeed;}


    //	3d feature
    void	SetRGBA		(float alpha, float red, float green, float blue);
    void	SetArmor	(OBJID idType);
    void	SetArmet	(OBJID idType);
    void	SetRWeapon	(OBJID idType);
    void	SetLWeapon	(OBJID idType);
    void	SetMount	(OBJID idType);
    void	SetMantle	(OBJID idType);
    BOOL	HaveBow		();
    OBJID	GetArmor	() {return m_Info.idArmorType;}
    OBJID	GetArmet	() {return m_Info.idArmetType;}
    OBJID	GetRWeapon	() {return m_Info.idRWeaponType;}
    OBJID	GetLWeapon	() {return m_Info.idLWeaponType;}
    OBJID	GetMount	() {return m_Info.idMountType;}
    OBJID	GetMantle	() {return m_Info.idMantleType;}

    // static...
public:
    enum {SHADOW_NONE, SHADOW_SIMPLE, SHADOW_REAL};
    static int s_nShadowType;

public:
    int		GetActionType	(void)	{return m_Info.iActType;}
    BOOL	IsAttacking		(void);
    void	GetActionStartPos	(int& nWorldX, int& nWorldY)	{nWorldX = m_Info.posActionBegin.x, nWorldY = m_Info.posActionBegin.y;}
    void	GetActionEndPos		(int& nWorldX, int& nWorldY)	{nWorldX = m_Info.posActionEnd.x, nWorldY = m_Info.posActionEnd.y;}

    void	GetPrePos	(int& nPosX, int& nPosY);
    void    GetNextPos  (int& nPosX, int& nPosY) {nPosX = m_Info.posNext.x; nPosY = m_Info.posNext.y;};
    int		GetDir		(CMyPos posTarget) const;
    int		GetDir		(CRole* pTarget) const;

    //------------- status -------------
public:
    void	SetStatus	(__int64 dwStatus);
    void	ClrStatus	(__int64 dwStatus);
    __int64	GetStatus	(void)	{return m_dwStatus;}
    BOOL	TestStatus	(__int64 dwStatus);

protected:
    __int64	m_dwStatus;

    //------------- ani effects -------------
public:
    BOOL	AddAniEffectEx (const char* pszEffectTitle,
                            int nTimes = 1,
                            DWORD dwInterval = 50,
                            int nShowWay = _SHOWWAY_NORMAL,
                            DWORD dwLoopInterval = 0,
                            int nOffsetX = 0,
                            int nOffsetY = 0,
                            int nExigence = 0,
                            DWORD dwDelay = 0);

    BOOL	AddAniEffect	(const char* pszEffect);
    void	ClrAniEffect	(const char* pszEffect = NULL);

    void	ShowAniEffect	(int nPosX, int nPosY);
    void	ProcessAniEffect();
    BOOL	TestAniEffect(const char* pszEffect);
private:
    DEQUE_EFFECT	m_dequeAniEffect;

    //------------- command -------------
public:
    virtual DWORD	SetCommand	(CCommand* pCommand);
    virtual BOOL	ProcessCommand	(void);

    void		InitCommand	(void);
    CCommand*	GetCommand	(void)	{return &(m_Info.cmdProc);}

    int			GetCommandType(void);
    BOOL		GetCommandFlag(void) {return m_Info.cmdProc.bData;}
    int			GetLastCommand(void)	{return m_Info.cmdProc.iType;}
    int			GetCommandStatus(void)	{return m_Info.cmdProc.iStatus;}

    //------------- info -------------
public:
    BOOL	IsNpc(void) {return GetID() <= _IDMAX_NPC;}
    BOOL	IsPlayer(void) {return (GetID() >= PLAYER_ID_FIRST && GetID() <= PLAYER_ID_LAST);}

    //	BOOL	IsMonster(void) {return (GetID() >= MONSTERID_FIRST && GetID() <= MONSTERID_LAST);}
    //ACTION创建的MONSTER也算是MONSTER   --2005 6- 13 --XUELIANG
    BOOL	IsMonster(void) {return (GetID() >= MONSTERID_FIRST && GetID() <= PETID_LAST);}

    BOOL	IsGuard(void);
    BOOL	IsPet(void) {return (GetID() >= CALLPETID_FIRST && GetID() <= CALLPETID_LAST);}
    BOOL	IsMyPet(void);
    void	SetID		(OBJID id)		{m_Info.id = id;}
    OBJID	GetID		(void) const	{return m_Info.id;}

    void    SetLook		(USHORT usLook);
    USHORT	GetLook		(void) const	{return m_Info.usLook;}

    void    SetHair		(USHORT usHair);
    USHORT	GetHair		(void) const	{return m_Info.usHair;}

    void    SetFace		(USHORT usFace);
    USHORT	GetFace		(void) const	{return m_Info.usFace;}

    void    SetPose		(USHORT usPose)	{m_Info.usPose = usPose;}
    USHORT	GetPose		(void) const	{return m_Info.usPose;}

    void	SetDir		(int nDir);
    int		GetDir		(void) const	{return m_Info.iDirection;}

    const char*	GetName	(void) const	{return m_Info.szName;}
    void	SetName		(const char* pszName);

    DWORD   GetMonsterTypeId(void) {return m_Info.idMonsterType;}
    void	SetMonsterTypeId(DWORD idType) {m_Info.idMonsterType = idType;}

    int		GetAddSize	(void) const	{return m_Info.iAddSize;}
    void	SetAddSize	(int nSize)		{m_Info.iAddSize = nSize;}

    int		GetZoomPercent	(void) const	{return m_Info.iZoomPercent;}
    void	SetZoomPercent	(int nPercent)	{m_Info.iZoomPercent = nPercent;}

    void	AddFStr(char* pszStr, int nMaxHeight = 64, DWORD dwColor = 0x00ffffff, int nStepHeight = 3, int nDelayTime = 0);
    void	AddZoomNum		(int nNum, DWORD dwColor, DWORD dwDelay, int nRandOffset = 0);
    //int		GetType();
    void	SetActionCmd (int nAction, int nDir = -1, DWORD dwType = 0, BOOL bData = false);
    void	SetIntoneCmd (int nAction, int nDir = -1, DWORD dwType = 0, BOOL bData = false);
    void	Transform(int nLook);
    void	SetWorldPos(CMyPos posWorld) {m_Info.posWorld = posWorld;}
    int		GetFrameIndex() {return m_Info.iActFrameIndex;}
    void	ShootArrow(OBJID idTarget, int nHpLose);

    int		GetSex() {return m_Info.nSex;}
    void	SetSex(int nSex) {m_Info.nSex = nSex;}


    int		GetRace() {return m_Info.nRace;}
    void	SetRace(int nRace) {m_Info.nRace = nRace;}

    void	AllotRes();
    int		GetViewType() {return m_nViewType;}

    static  int		GetIDType(OBJID id);
    int		GetRoleType() {return m_nRoleType;}
    void	SetRoleType(int nRoleType) {m_nRoleType = nRoleType;}
    void	SetRoleAttribute(DWORD dwAttribute) {m_dwRoleAttribute = dwAttribute;}
    DWORD	GetRoleAttribute() {return m_dwRoleAttribute;}
    void	SetFlyOffset(int nOffset);
    int		GetFlyOffset();
    void	SetFlyOffsetGoal(int nOffset);
    int		GetFlyOffsetGoal();
    void	FlashLastAttackTime() {m_tmLastAtk = ::TimeGet();}
    BOOL	IsHypersensitive();
    void	AddStep(CMyPos posCell);
protected:
    CRoleInfo		m_Info;
    int				m_nType;
    int				m_nViewType;
    int				m_nRoleType;
    DWORD			m_dwRoleAttribute;
    DWORD			m_tmDisappear;
    DWORD			m_tmLastAtk;
    LIST_DIR		m_lstStepDir;
    int				m_nWhichLeg;
    CMyPos			m_posCellOffset;
public:
    void SetSpriteEffect( UCHAR ucSprite );
    IRoleView*		m_pIRoleView;
    CSayHello		m_objSayHello;
    CFloatingStr	m_objFStr;
    CGame3DEffectEx m_objEffect;
    CZoomNum		m_objZoomNum;

public:
    typedef struct
    {
        int		nAction;
        DWORD	dwFrameInterval;
        char	szEffect[128];
        char	cDir;
        std::deque<char> setStep;
    } KongfuAction;
    std::deque<KongfuAction> m_setKongfu;
    BOOL	LoadKongfu(const char* pszFileName);

public:
    DWORD m_dwDisappear;
    DWORD m_dwDisappearLast;
    void Disappear(DWORD dwLast);
public:
    static float s_fBaseScale;
    std::vector<CCommand*> m_setCmd;
    std::vector<int> m_setLife;
    std::deque<BeAttackCmdInfo*> m_setBeAttackCmdInfo;
    OBJID m_idLocker;
protected:
    DWORD m_dwSpeedRate;
protected:
    DWORD m_dwCommandIndex;
public:
    void SetSpeedRate(int nSpeedRate) {m_dwSpeedRate = nSpeedRate;}
    void AddBeAttackCommandInfo(OBJID idSender, int nSenderCommandIndex, int nAction, int nFrameIndex, int nLife, DWORD dwColor);
    void PerformBeAttackCmd(BeAttackCmdInfo* pInfo);
    DWORD	GetSpeedRate() {return m_dwSpeedRate;}
    //---------------for mapobj----------------
public:
    virtual int  GetObjType()               {return m_nType;}
    virtual BOOL IsFocus()                  {return false;}
    virtual void GetWorldPos(CMyPos& posWorld) {posWorld = m_Info.posWorld;}
    virtual void GetPos(CMyPos& posCell)	{posCell = m_Info.posMap;}
    virtual void SetPos(CMyPos posCell)		{SetPos(posCell.x, posCell.y);}
    virtual void GetBase(CMySize& infoSize)	{infoSize.iWidth = 1; infoSize.iHeight = 1;}
    virtual void SetBase(CMySize& infoSize)	{m_Info.sizeBase = infoSize;}
    //---------------for mapobj----------------
};



#endif
