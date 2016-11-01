
//**********************************************************
// 代码编辑器
//**********************************************************

// User.h: interface for the CUser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USER_H__BF5C20F1_5DBE_4E65_9099_E96EFAD7ECBA__INCLUDED_1)
#define AFX_USER_H__BF5C20F1_5DBE_4E65_9099_E96EFAD7ECBA__INCLUDED_1

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)
#include "GameBlock.h"
#include "UserData.h"
#include "Friend.h"
#include "Tutor.h"
#include "Item.h"
#include "Timeout.h"
#include "SquareDeal.h"
#include "gamemap.h"
#include "battleSystem.h"
#include "UserWeaponSkill.h"
#include "Myheap.h"
#include "T_SingleObjSet2.h"
#include "T_SingleMap2.h"
#include "team.h"
#include "magic.h"
#include "math.h"
#include "Package.h"
#include "SynAttr.h"
#include "protocol.h"
#include "booth.h"
#include "autolink.h"
#include "Enemy.h"
#include "AutoPtr.h"
#include "UserPackage.h"
#include "Role.h"

#include <vector>
#include <deque>
using namespace std;

#include "UserTable.h"		// 各种表定义
#include "define.h"	// Added by ClassView

////////////////////////////////////////////////////////////////

const int	_MAX_USERFRIENDSIZE				= 50;
const int	_MAX_TUTORSIZE					= 15;
const int	_MAX_MONEYLIMIT					= 1000000000;
const int	USER_PACKAGE_SIZE				= 20;							// 仓库最多存多少东西
const int	USERDROPITEM_RANGE				= 5;							// 自动掉
const int	USERDROPITEM_RANGE2				= 9;							// 手动扔
//const int	ARROW_FLY_ALT					= 60;							// 箭支飞行高度
const int	JUMP_ALT						= 200;							// 跳的最高高度
const int	FLY_ALT							= 100;							// 飞的最高高度
const int	PERCENT_DECDUR					= 5;							// 装备升级失败后渐耐久的几率
const int	CRIME_KEEP_SECS					= 30;							// 犯罪维持时间
const int   SELF_DEFENCE_SECS               = 20;                           // 正当防卫持续时间
const int	SYNCRIME_KEEP_SECS				= 30;							// 帮派犯罪维持时间
const int	MAX_MENUTASKSIZE				= 20;							// 菜单系统中"回答TASK"的最大数量

const int	PKVALUE_HALFKILL				= 2;
const int	PKVALUE_ONEKILL					= 20;                           // 每杀死一个白名/红名玩家，增加PK值   // add huang 2004.1.11
const int	PKVALUE_REDNAME					= 21;							// 红名玩家最低PK值
const int	PKVALUE_BLACKNAME				= 101;							// 黑名玩家最低PK值

const int	CHANCE_MAKEHOLE					= 300;

const int	SUPERMAN_KILLMONSTER_INCTIME		= 800;				// 无双状态每杀一个怪物的加时(MS)
const int	SUPERMAN_KILLMONSTER_TIMELIMIT		= 120 * 1000;			// 无双状态延长的时间限制(MS)
const int	TORNADO_KILLMONSTER_INCTIME			= 1000;				// 旋风状态每杀一个怪物的加时(MS)
const int	TORNADO_KILLMONSTER_TIMELIMIT		= 120 * 1000;			// 旋风状态延长的时间限制(MS)

const int	MASK_CHANGELOOK					= 10000000;						// 变身时LOCKFACE的掩码，face=face + new_face*10000000
const int	MAX_SIZEADD						= 2;							// 最大的SizeAdd，用于魔法区域的放大搜索(注意该数据不要太大)

const int	CHGMAP_LOCK_SECS				= 10;							// 进入地图后的保护时间
const int	SYNWAR_NOMONEY_DAMAGETIMES		= 10;							// 帮派没钱时增加的伤害倍数
const int	PICK_MORE_MONEY					= 1000;							// 捡钱多时，发ACTION消息
const int	SYNCHRO_SECS					= 5;							// 同步消息5秒内同步

const int	IDENT_FEE						= 100;							// 鉴定的价格
const int	UNIDENT_SALE_FEE				= 1;							// 卖未鉴定的价格

const int	MAX_USER_XP						= 100;			// 最大XP值
const int	ADD_XP_SECS						= 30;			// 每隔30秒增加一次XP
const int	ADD_XP_VALUE					= 15;			// 每次增加15点XP
const int	MAX_XPFULL_SECS					= 60;			// XP满最多持续的时间
//const int	ADD_XPFULL_SECS_KILLMONSTER		= 1;			// 每杀死一只怪物多持续1秒钟

const int	ADJUST_EXPFORMAT_LEVEL			= 70;			// 70级以上调整经验存储格式为/10

/////////////////////////////////////////////////////////////////////////
// 导师系统相关
const int	ADDSTUDENT_TUTOREXP				= 10;			// 招收徒弟需要消耗的导师经验
const int	DELSTUDENT_TUTOREXP				= 1000;			// 开除学员扣的导师经验
const int	LEAVETUTOR_LEVEL				= 10;			// 10级以上玩家叛师要扣经验
const int	_MIN_GIVEEXP_LEV				= 30;			// 学员为导师提供奖励经验的最低等级
const int	TUTOREXP_PERCENT				= 40;			// 一级学员奖励给导师的经验比例
const int	TUTOREXP2_PERCENT				= 10;			// 二级学员奖励给导师的经验比例
const int	GETEXP_PER_TUTOREXP0			= 500;			// 初级导师消耗1点导师经验能够获得的学员贡献经验
const int	GETEXP_PER_TUTOREXP1			= 700;			// 中级导师消耗1点导师经验能够获得的学员贡献经验
const int	GETEXP_PER_TUTOREXP2			= 1000;			// 高级导师消耗1点导师经验能够获得的学员贡献经验


//---jinggy---帮派---begin
const int	SYNREPUTE_SECS					= 6 * 60;		// 玩家每在线6分钟，加10点军团 声望和贡献度。
const int	SYNREPUTE_VAL_PEREACH			= 10;
const int	SYNREPUTE_VAL_PROFFER			= 1;		// 玩家每在线6分钟，加一点军团贡献度。

#define		SYNMEM_TIMEONLINE_INTERVAL		15*60			//每15分钟更新一次帮派成员在线时间
#define		SYNMEM_TIMEONLINE_INCREPUTE		10 //③、	军团成员每人每在线6分钟，军团声望增加10点
//---jinggy---帮派---end
//---jinggy---铸造---begin
const int	EQUIPMENT_EXP_ADD_SECS			= 10 * 60;						// 每10分钟增加一次小于9级的武器经验
const int	EQUIPMENT_EXP_ADD_SECS_AFTER9	= 15 * 60;						// 每15分钟增加一次9级的武器经验
const int	WEAPON_UPEXP_MAXVALUE			= 8;
const int	ARMOR_UPEXP_MAXVALUE			= 10;
const int	NECKLACE_UPEXP_MAXVALUE			= 12;
const int	SHOE_UPEXP_MAXVALUE			= 14;
const int	BANGLE_UPEXP_MAXVALUE		= 16;
const int	MAX_LEVEL_WARLEVEL	= 9;
const int   DEC_WARGHOSTEXP_FORREPAIR = 3 ;
//---jinggy---铸造---end

const int	PROFFER_BEKILL					= 100;					// 每被敌对帮派玩家杀死一次减少的帮派贡献度
const int	SYNREPUTE_BEKILL				= 100;					// 每次被敌对帮派玩家杀死减少的帮派声望

const int	NEWBIE_LEVEL					= 40;			// 新手的等级
const int	_MIN_TUTOR_LEVEL				= 50;			// 导师最低等级
enum { FLYMAP_ERROR = -1, FLYMAP_NORMAL, FLYMAP_MAPGROUP };
enum { ATTACK_TIME = false, BEATTACK_TIME = true, BY_WEAPON = false, BY_MAGIC = true };

////////////////////////////////////////////////////////////////
// sex define
enum {SEX_NONE = 0, SEX_MAN, SEX_WOMAN, SEX_ASEXUAL, SEX_ALL};

// requre sex define
const unsigned char SEX_REQ_MAN			=	0x01;// 男的可用
const unsigned char SEX_REQ_WOMAN		=	0x02;// 女的可用
const unsigned char SEX_REQ_EUNUCH		=	0x04;// 太监可用


////////////////////////////////////////////////////////////////
const int	MASTER_USERLEV				= 120;
const int	MAX_USERLEV					= 130;
//const int	MAX_EUDEMONLEV				= 130;

typedef	IGameObjSet<CUserWeaponSkill>		IUserWeaponSkillSet;
typedef	CGameObjSet<CUserWeaponSkill>		CUserWeaponSkillSet;

////////////////////////////////////////////////////////////////
enum
{
    RACE_NONE	= 0, // 未知
    RACE_HUMAN	= 1, // 人类
    RACE_SPRITE = 2, // 精灵
    RACE_DARKSPRITE = 3, // 暗精灵
};

////////////////////////////////////////////////////////////////
enum {	PROFSORT_MAGICIAN	= 1,
        PROFSORT_SOLDIER	= 2,
        PROFSORT_ARCHER		= 3,

        // no use
        PROFSORT_WIZARD		= 4,
        PROFSORT_DAOIST		= 10,
        PROFSORT_DAOISTGOLD	= 11,
        PROFSORT_DAOISTWOOD	= 12,
        PROFSORT_DAOISTWATER = 13,
        PROFSORT_DAOISTFIRE	= 14,
        PROFSORT_DAOISTEARTH = 15,
     };

////////////////////////////////////////////////////////////////
enum
{
    NOBILITY_RANK_NONE		= 0,			// 无爵位
    NOBILITY_RANK_BARON		= 1,			// 爵士
    NOBILITY_RANK_VISCOUNT	= 2,			// 子爵
    NOBILITY_RANK_EARL		= 3,			// 伯爵
    NOBILITY_RANK_MARQUESS	= 4,			// 侯爵
    NOBILITY_RANK_DUKE		= 5,			// 公爵
};

////////////////////////////////////////////////////////////////
class IUserStorage
{
public:
    virtual bool	SendStorageInfo(OBJID idNpc, int nPosition = ITEMPOSITION_STORAGE)						PURE_VIRTUAL_FUNCTION_0
    virtual bool	CheckIn(OBJID idNpc, CItem* pItem, bool bUpdate, int nPosition = ITEMPOSITION_STORAGE)	PURE_VIRTUAL_FUNCTION_0
    virtual CItem*	CheckOut(OBJID idNpc, OBJID idItem, bool bUpdate, int nPosition = ITEMPOSITION_STORAGE)	PURE_VIRTUAL_FUNCTION_0
    virtual int 	GetStorageCount()																		PURE_VIRTUAL_FUNCTION_0
    virtual int 	GetWeight(OBJID idItem)																	PURE_VIRTUAL_FUNCTION_0
    virtual OBJID	GetItemType(OBJID idItem)																PURE_VIRTUAL_FUNCTION_0
    virtual DWORD	GetAmountLimit(OBJID idItem)															PURE_VIRTUAL_FUNCTION_0
};

const int	MASTER_WEAPONSKILLLEVEL	= 12;
const int	MAX_WEAPONSKILLLEVEL	= 20;
const int	nWeaponSkillLeveUpNeedExp[20] =
{
    0,
    1200,
    68000,
    250000,
    640000,
    1600000,
    4000000,
    10000000,
    22000000,
    40000000,
    90000000,
    190000000 / 2,
    285000000 / 2,
    427500000 / 2,
    641250000 / 2,
    961875000 / 2,
    1442812500 / 2,
    2164218750 / 2,
    3246328125 / 2,
    4200000000 / 2
};

//////////////////////////////////////////////////////////////////////
enum INFO_ID
{
    INFO_NONE = 0,
    INFO_ITEM,
    INFO_FRIEND,
    INFO_ENEMY,
    INFO_WEAPONSKILL,
    INFO_MAGIC,
    INFO_SYNATTR,
    INFO_STATUS,
    INFO_TEAM,
};

////////////////////////////////////////////////////////////////
enum REMOTE_CALL
{
    REMOTE_CALL_DROP_LEADER,
    REMOTE_CALL_LEAVE_SYN,
    REMOTE_CALL_TEAM_CLOSE_MONEY,
    REMOTE_CALL_TEAM_CLOSE_ITEM,
    REMOTE_CALL_TEAM_CLOSE_GEM_ACCESS,
    REMOTE_CALL_TEAM_HUNTKILL,
};

////////////////////////////////////////////////////////////////
class	IChangeTeam
{
public:
    virtual bool AddTeamMember(OBJID idUser)				PURE_VIRTUAL_FUNCTION_0
    virtual bool DelTeamMember(OBJID idUser)				PURE_VIRTUAL_FUNCTION_0
    virtual bool DelTeam()									PURE_VIRTUAL_FUNCTION_0
    virtual bool SendInfo(SOCKET_ID idSocket, OBJID idAgent)				PURE_VIRTUAL_FUNCTION_0
};

//////////////////////////////////////////////////////////////////////
// extern int RankOfIndex(int nPos);

////////////////////////////////////////////////////////////////
class CNetMsg;
class CPackage;
class CTransformation;
class CAgent;
class CTaskDetail;
class CUser : public CGameObj, public CRole, IUserStorage, IChangeTeam
{
protected:
    CUser();
    virtual ~CUser();
public:
    static		CUser*		CreateNew	()				{ return new CUser; }
    void		ReleaseByOwner			()				{ delete this; }
    void		OnTimer					(time_t tCurr);
    static		CUser*	CreateNewUser	(PROCESS_ID idProcess, SOCKET_ID idSocket, UserInfoStruct* pInfo);
    bool		Create				(PROCESS_ID idProcess, SOCKET_ID idSocket, OBJID idUser);
    bool		Init					();		// login 和 change map_group都要执行的操作

    virtual		bool		FindAroundObj	(OBJID idObjType, OBJID idObj, void** pp)		{ return GetMap()->QueryObj(GetPosX(), GetPosY(), idObjType, idObj, pp); }
    virtual		IRole*		FindAroundRole	(OBJID id)		{ return GetMap()->QueryRole(GetPosX(), GetPosY(), id); }
    virtual		CUser*		FindAroundUser	(OBJID id);
protected: // interface
    virtual		bool		QueryRole		(void** ppv)	{ return *ppv = (IRole*)this, true; }
    virtual		bool		QueryObj		(OBJID idObjType, void** ppv)			{ if (idObjType == OBJ_USER) { return *ppv = this, true; } return *ppv = NULL, false; }
    virtual		CUser*		QueryOwnerUser	()				{ return this; }
public:
    virtual		IMapThing*	QueryMapThing	()				{ return (IMapThing*)this; }
    IRole*					QueryRole		()				{ return (IRole*)this; }
    IChangeTeam*			QueryChangeTeam	()				{ return (IChangeTeam*)this; }
protected:
    virtual		IUserWeaponSkillSet*	QueryWeapenSkill()	{ return m_pUserWeaponSkillSet; }

public: // IMapThing
    //	virtual		OBJID		GetID();
    //	virtual		int			GetPosX();
    //	virtual		int			GetPosY();
    virtual		int			GetObjType		()			{ return CGameObj::GetObjType(); }

    virtual		void		SendShow		(IRole* pRole);

public: // IRole
    virtual OBJID	GetID			()				{ return m_data.m_Info.id; }
    virtual LPCTSTR	GetName			()				{ return m_data.m_Info.szName; }
    virtual LPCTSTR	GetMate			()				{ return m_data.m_Info.szMate; }
    virtual int		GetSex			()				{ if (m_data.GetLookFace() % 2 == 0) { return SEX_WOMAN; } return SEX_MAN; }
    virtual DWORD	GetLookFace		();
    virtual DWORD 	GetHair()						{ return m_data.m_Info.dwHair; }
    virtual char	GetLength		()				{ return m_data.m_Info.cLength; }
    virtual char	GetFat			()				{ return m_data.m_Info.cFat; }
    virtual int		GetPosX			()				{return m_nPosX;}
    virtual int		GetPosY			()				{return m_nPosY;}
    virtual int 	GetDir			();
    virtual void	SetDir			(int nDir)		{ m_nDir = nDir % 8; }
    virtual int		GetPose			();
    virtual void	SetPose			(int nPose)		{ m_nPose = nPose; }
    virtual void	SetEmotion		(int nEmotion)	{ m_nEmotin = nEmotion; }
    virtual DWORD	GetMoney		()				{ return m_data.m_Info.dwMoney; }
    virtual DWORD	GetMoneySaved	()				{ return m_data.m_Info.dwMoney_saved; }
    //	virtual DWORD	GetAuctionMoney	()				{ return m_data.m_Info.dwAuction_Money; }//LW

    virtual OBJID	GetMateID		();
    //	virtual bool	AddItem			(CItem* pItem);

    void	SetLife(int nLife, BOOL bUpdate = false);
    virtual DWORD	GetLife				();
    virtual DWORD	GetMaxLife			();
    virtual DWORD	GetMana				()			{ return m_data.GetMana(); }	// not over
    virtual DWORD	GetMaxMana			();

    virtual DWORD	GetMinAtk			();
    virtual DWORD	GetMaxAtk			();
    virtual DWORD	GetMgcMinAtk		();
    virtual DWORD	GetMgcMaxAtk		();
    virtual DWORD	GetAttack			();
    virtual DWORD	GetMagicAttack		();
    virtual DWORD	GetAtkHitRate		();

    virtual DWORD	GetDef				();
    virtual DWORD	GetDefence			();
    virtual DWORD	GetDefence2			();

    virtual DWORD	GetDex				();
    virtual DWORD	GetDexterity		();

    virtual DWORD	GetDdg				();
    virtual DWORD	GetDodge			();

    //	virtual DWORD	GetMagicAtk			();
    virtual DWORD	GetMagicDef			();

    virtual DWORD	GetInterAtkRate		();
    virtual DWORD	GetIntervalAtkRate	();
    virtual int		GetAttackRange		(int nTargetSizeAdd);
    virtual	int		GetSizeAdd			();
    virtual	int		GetLuck				();

    virtual void	AwardBattleExp		(int nExp, bool bGemEffect = true);
    virtual int 	AdjustHitRate		(int nHitRate);
    virtual int 	AdjustAttack		(int nAtk);
    virtual int 	AdjustDefence		(int nDef);
    virtual int 	AdjustMagicAtk		(int nAtk);
    virtual int 	AdjustMagicDef		(int nDef);
    virtual int 	AdjustBowDefence	(int nDef);
    virtual int 	AdjustWeaponDamage	(int nDamage);
    virtual int 	AdjustMagicDamage	(int nDamage);
    virtual void	SetFightPause		(int nInterval)			{ m_nFightPause = nInterval; }

    virtual int		GetDistance			(IMapThing* pTarget)	{ return __max(abs(GetPosX() - pTarget->GetPosX()), abs(GetPosY() - pTarget->GetPosY())); }
    virtual int		GetDistance			(int x, int y)			{ return __max(abs(GetPosX() - x), abs(GetPosY() - y)); }

    virtual int 	GetExp				()						{ return m_data.m_Info.nExp; }
    virtual void	SetStatus			(int nStatus, bool bSynchro = true);
    virtual void	ClsStatus			(int nStatus, bool bSynchro = true);
    virtual DWORD	GetStatus			()						{ return m_data.m_Info.dwStatus; }
    virtual void	ClearAllStatus		();
    //	virtual BOOL	CheckStatus			(int nStatus);
    virtual I64 	GetEffect			();

    virtual DWORD	GetLev				();
    virtual void	IncLev				(int nLev);
    virtual void	AllotPoint			();

    virtual void	ClrAttackTarget		()						{ SetAttackTarget(NULL); QueryMagic()->BreakAutoAttack(); }
    virtual bool	SetAttackTarget		(IRole* pTarget);
    virtual OBJID	GetAttackTargetID	()						{ if (m_pBattleSystem) { return m_pBattleSystem->GetTargetID(); } return ID_NONE; }
    virtual bool	AutoSkillAttack		(IRole* pTarget)		{ return QueryMagic()->AutoAttackTarget(pTarget); }
    virtual bool	AdditionMagic(int nLifeLost, int nDamage)				{ return false; }
    virtual int		Attack				(IRole* pTarget);
    virtual bool	BeAttack			(bool bMagic, IRole* pTarget, int nPower, bool bReflectEnable = true);
    virtual bool	MagicAttack			(int nType, OBJID idTarget, int x, int y);
    virtual void	Kill				(IRole* pTarget, DWORD dwDieWay);
    virtual void	BeKill				(IRole* pTarget = NULL);
    virtual bool	BroadcastTeamLife	(bool bMaxLife = false);
    virtual bool	IsAtkable			(IRole* pTarget, bool bSendHint = false);
    virtual bool	IsBeAtkable			();
    virtual void	SendDamageMsg		(OBJID idTarget, int nDamage);
    virtual void	ProcessPk			(CUser* pTargetUser);
    virtual bool	TransferShield		(bool bMagic, IRole* pAtker, int nDamage);

    virtual int		GetHelmetTypeID		()			{ if (m_pHelmet) { return m_pHelmet->GetInt(ITEMDATA_TYPE); } return ID_NONE; }
    virtual int		GetArmorTypeID		()			
	{ 
		if (m_nSynDressArmorType) 
		{ 
			return m_nSynDressArmorType; 
		} 
		if(m_pFashion)
		{

			return m_pFashion->GetInt(ITEMDATA_TYPE); 
		}
		if (m_pArmor) 
		{ 
			return m_pArmor->GetInt(ITEMDATA_TYPE); 
		}  
		return ID_NONE;
	}
	
	virtual int		GetWeaponRTypeID	()			{ if (m_pWeaponR) { return m_pWeaponR->GetInt(ITEMDATA_TYPE); } return ID_NONE; }
    virtual int		GetWeaponLTypeID	()			{ if (m_pWeaponL) { return m_pWeaponL->GetInt(ITEMDATA_TYPE); } return ID_NONE; }
    virtual int		GetNecklaceTypeID	()			{ if (m_pNecklace) { return m_pNecklace->GetInt(ITEMDATA_TYPE); } return ID_NONE; }
    virtual int		GetRingRTypeID		()			{ if (m_pRingR) { return m_pRingR->GetInt(ITEMDATA_TYPE); } return ID_NONE; }
    virtual int		GetRingLTypeID		()			{ if (m_pRingL) { return m_pRingL->GetInt(ITEMDATA_TYPE); } return ID_NONE; }
    virtual int		GetShoesTypeID		()			{ if (m_pShoes) { return m_pShoes->GetInt(ITEMDATA_TYPE); } return ID_NONE; }
    virtual int		GetMountTypeID		()			{ if (m_pMount) { return m_pMount->GetInt(ITEMDATA_TYPE); } return ID_NONE; }
    //virtual int		GetSpriteTypeID		()			{ if (m_pSprite) return m_pSprite->GetInt(ITEMDATA_TYPE); return ID_NONE; }
    virtual int		GetMantleTypeID		()			{ if (m_pMantle) { return m_pMantle->GetInt(ITEMDATA_TYPE); } return ID_NONE; }

    virtual CItem*	GetWeaponR			()			{ return m_pWeaponR; }
    virtual CItem*	GetWeaponL			()			{ return m_pWeaponL; }

    virtual char*	GetMedal			()			{ return m_data.m_Info.setMedal; }
    virtual DWORD	GetMedalSelect		()			{ return m_data.m_Info.dwMedal_select; }
    //	virtual void	SetMedalSelect		(int nData)	{m_data.SetMedalSelect(nData);}
    virtual char*	GetTitle			()			{ return m_data.m_Info.setTitle; }
    //	virtual DWORD	GetTitleSelect		()			{ return m_data.m_Info.dwTitle_select; }
    //	virtual void	SetTitleSelect		(int nData)	{m_data.SetTitleSelect(nData);}
    virtual DWORD	GetPotential		()			{ return m_data.m_Info.nPotential; }	// 取得潜能值
    virtual DWORD	GetMetempsychosis	()			{ return m_data.m_Info.ucMetempsychosis; }

    virtual bool	IsSimpleMagicAtk	();
    virtual int 	GetSimpleMagicHitRate();

    virtual int		GetTutorExp			()			{ return m_data.m_Info.nTutorExp; }
    virtual UCHAR	GetTutorLevel		()			{ return m_data.m_Info.ucTutorLevel; }
    virtual UCHAR	GetMercenaryRank	()			{ return m_data.m_Info.ucMercenaryRank; }
    virtual DWORD	GetMercenaryExp		()			{ return m_data.m_Info.dwMercenaryExp; }
    virtual DWORD	GetExploit			()			{ return m_data.m_Info.dwExploit; }
    virtual DWORD   GetAllTutorLevel    ();

    virtual bool	IsAlive				();
    virtual bool	IsTalkEnable		()			{ return true; }	// not over
    virtual bool	IsGM				()			{ return (strstr(GetName(), "[GM]") || strstr(GetName(), "[PM]")); }
    virtual bool	IsPM				()			{ return (strstr(GetName(), "[PM]") != NULL); }
    virtual bool	IsMonster			()			{ return false; }
    //	virtual bool	IsScene				()			{ return false; }
    virtual bool	IsDelThis			()			{ return false; }
    virtual bool	IsVirtuous()					{ return GetPk() < PKVALUE_REDNAME && !QueryStatus(STATUS_CRIME); }
    virtual bool	IsEvil()						{ return GetPk() >= PKVALUE_REDNAME || QueryStatus(STATUS_CRIME); }
    virtual bool	IsBowman			()			{ return m_pWeaponR && m_pWeaponR->IsBowSort(); }
    virtual bool	IsShieldEquip		()			{ return m_pWeaponL && m_pWeaponL->IsShield(); }
    virtual bool	IsWing				()			{ return NULL; }	//QueryStatus(STATUS_WING) != NULL; }
    virtual bool	IsSendBlockInfo		()			{ return true; }
    virtual bool	IsAutoAllot			(void)		{ return (m_data.m_Info.ucAuto_allot != 0); }
    virtual bool	IsNewbie			()			{ return GetLev() < NEWBIE_LEVEL; }

    virtual bool	SendMsg				(CNetMsg* pMsg);
    virtual void	BroadcastRoomMsg	(CNetMsg* pMsg, bool bSendSelf);
    virtual void	BroadcastRoomMsg	(LPCTSTR szMsg, bool bSendSelf);
    virtual void	BroadcastMapMsg		(CNetMsg* pMsg, bool bSendSelf);
    virtual void	BroadcastMapMsg		(LPCTSTR szMsg, bool bSendSelf);
    virtual bool	SendSysMsg			(const char* fmt, ...);
    virtual bool	SendSysMsg	(int nChannel, const char* fmt, ...);

    virtual DWORD	GetSynID			()				{ return QuerySynAttr()->GetSynID(); }
    virtual DWORD	GetMasterSynID		()				{ if (QuerySynAttr()->GetSynID() == ID_NONE) { return ID_NONE; } return this->GetSyndicate()->GetMasterSynID(); }
    virtual DWORD	GetSynRank			()				{ if (QuerySynAttr()->GetSynID() == ID_NONE) { return RANK_NONE; } return QuerySynAttr()->GetInt(SYNATTRDATA_RANK); }
    virtual DWORD	GetSynRankShow		()				{ if (QuerySynAttr()->GetSynID() == ID_NONE) { return RANK_NONE; } return QuerySynAttr()->GetSynRankShow(); }

    virtual UCHAR	GetNobilityRank		()			    { return m_data.GetNobilityRank(); }  //add huang 2003.12.29
    virtual void	SetNobilityRank		(DWORD dwNobilityRank, bool bUpdate = true)	{ m_data.SetNobilityRank(dwNobilityRank, bUpdate); }
public:	// game card system -------------------
    bool		CheckInCard	(void);
    bool		CheckOutCard(void);
    int			CountCard	(void);

    bool		CheckInCard2	(void);
    bool		CheckOutCard2	(void);
    int			CountCard2		(void);

public:	// mining -------------------
    void		Mine	(IRole* pRole);
    void		StopMine(void);

private:
    void		ProcessMineTimer(void);
    int			m_nMineCount;
    CTimeOutMS	m_tMine;
    CNpc*		m_pMineTarget;

public:	// equipment improve -------------------
    OBJID		ChkUpEqQuality	(CItem* pItem, bool bSendHint = false);
    OBJID		ChkUpEqLevel	(CItem* pItem, bool bSendHint = false);

    bool		GetUpEpQualityInfo		(CItem* pItem, int& nChance, OBJID& idNewType, bool bSendHint = false);
    bool		GetUpEpLevelInfo		(CItem* pItem, int& nChance, OBJID& idNewType, bool bSendHint = false);

    int			GetChanceUpEpQuality	(int nPosition, bool bSendHint = false);
    int			GetChanceUpEpLevel		(int nPosition, bool bSendHint = false);

    bool		UpEquipmentQuality		(int nPosition);	// for action use
    bool		UpEquipmentLevel		(int nPosition);	// for action use
    bool		DownEquipmentLevel		(int nPosition, int nUserLevel);	// for action use

    void		UpEquipmentQuality		(OBJID idEquipment, OBJID idTreasure);
    void		UpEquipmentLevel		(OBJID idEquipment, OBJID idTreasure);
    void		EmbedGem				(OBJID idItem, OBJID idGem, int nPos);
    void		TakeOutGem				(OBJID idItem, int nPos);

    bool		RecoverEquipmentDur		(int nPos);

    bool		IsAllNonsuchEquip		(void);

public:	// gem effect  -------------------
    int			GetGemMgcExpEffect	(void);
    int			GetGemWpnExpEffect	(void);
    int			GetGemExpEffect		(void);
    int			GetGemAtkEffect		(void);
    int			GetGemMgcAtkEffect	(void);

    void		SendGemEffect		(void);

public:	// sprite  -- add by zlong 2003-12-05
    int			GetSpriteFace		()			{ if (m_pSprite) { return (m_pSprite->GetSpriteLevel() * 5 + m_pSprite->GetSpriteType()); } return 0xFF; }

public: // lock key ------------------------------
    bool		SetLock(DWORD dwKey);
    bool		OpenLock(DWORD dwKey);
    bool		ClearLock(DWORD dwKey);
    bool		CheckLock(bool bSynchro = true);

public: // move
    //	virtual		void	TransPos		(int nPosX, int nPosY);		// 传送，完全的新坐标
    //	virtual		void	JumpPos			(int nPosX, int nPosY);		// 跳，半屏以内
    //	virtual		bool	MoveToward		(int nDir);
    //	virtual		bool	SyncTrackTo		(int nPosX, int nPosY, int nDir, DWORD dwAction);

    void		SetSynchro				(bool bSynchro)		{ if (bSynchro) { m_tSynchro.Clear(); } else { m_tSynchro.Startup(SYNCHRO_SECS); } }
    bool		IsSynchro				()					{ return !(m_tSynchro.IsActive() && !m_tSynchro.TimeOver()); }
    bool		SynPosition			(int nPosX, int nPosY, int nMaxDislocation = 8);
    void		ProcessOnAttack		();
    void		ProcessOnMove		(int nMoveMode);
    void		ProcessAfterMove	();
    void		GetFootPrint	(int& nPosX, int& nPosY);
    bool		KickBack();
    bool		Synchro();
    void		StandTurnon()								{ ASSERT(!m_bStand); m_tStand.Clear(); m_bStand = true; /*GetMap()->IncRole(GetPosX(), GetPosY());*/ }
    void		StandTurnoff()								{ m_tStand.Clear(); if (m_bStand) { m_bStand = false; /*GetMap()->DecRole(GetPosX(), GetPosY());*/ }}
    void		StandRestart()								{ StandTurnoff(); m_tStand.Update(); }
protected:
    CTimeOut	m_tSynchro;			// invalid: wait for client actionSynchro.
    DWORD		m_tFootPrint;
    POINT		m_posFootPrint;
    auto_long	m_bRun;
    CTimeOutMS	m_tStand;
    auto_long	m_bStand;
    CTimeOut	m_tAutoReborn;		// 复活宝石使用定时器

public:	// map ----------------------
    OBJID		GetMapID						();
    OBJID		GetRecordMapID					();
    bool		ChangeMap						();
    //	enum { FLYMAP_ERROR=-1, FLYMAP_NORMAL, FLYMAP_MAPGROUP };
    int 		FlyMap							(OBJID idMap, int nPosX, int nPosY);		// return false: can't fly
    void		GotoJail						(void);
    virtual bool		SendSelfToBlock			();

public: // main -------------------------------------
    SOCKET_ID	GetSocketID			()						{ return m_idSocket; }
    virtual bool	IsAgent			()						{ return false; }			// agent overload
    virtual CAgent*	QueryAgent		()						{ ASSERT(!"QueryAgent"); return NULL; }			// agent overload
    void		SaveInfo			();		// save all info
    void		RemoteCall			(REMOTE_CALL0* pInfo);
    OBJID		GetAccountID		()						{ return m_data.m_Info.idAccount; }

public: // const -------------------------------------

public: // leave word ---------------------------------------
    bool		IsMoreLeaveWord()							{ return m_bHaveLeaveWord; }
    void		SetLeaveWord(bool nFlag)					{ m_bHaveLeaveWord = nFlag; }
    bool		AddLeaveWord(LPCTSTR szRecv, LPCTSTR szWords);
protected:
    bool		m_bHaveLeaveWord;
    //	CTimeOut	m_tLeaveWord;

public: // bonus
    bool	DoBonus			(void);
    void	SendBonusInfo	(void);

public: // common -------------------------------------
    //	static int	AdjustData			(int nData, int nAdjust, int nMaxData=0);
    static int	GetLook(DWORD dwLookFace)					{ return dwLookFace % 1000; }
    static int	GetFace(DWORD dwLookFace)					{ return (dwLookFace / 10000) % 1000; }
    //	static int	GetSex(DWORD dwLookFace)					{ return (m_pInfo->dwLookFace/1000)%10; }
    static int	GetTransFormLook(DWORD dwLookFace)			{ return dwLookFace / 10000000; }

public: // IUserStorage ---------------------------------------------------
    IUserStorage*	QueryStorage()							{ return (IUserStorage*)this; }
protected:
    virtual bool	SendStorageInfo(OBJID idNpc, int nPosition = ITEMPOSITION_STORAGE);
    virtual bool	CheckIn(OBJID idNpc, CItem* pItem, bool bUpdate = true, int nPosition = ITEMPOSITION_STORAGE);
    virtual CItem*	CheckOut(OBJID idNpc, OBJID idItem, bool bUpdate, int nPosition = ITEMPOSITION_STORAGE);
    virtual int 	GetStorageCount()						{ IF_OK(m_pStorage) return m_pStorage->GetAmount(); return 1234567890; }
    virtual int		GetWeight(OBJID idItem)					{ IF_OK(m_pStorage) return m_pStorage->GetWeight(idItem); return ERROR_WEIGHT; }
    virtual OBJID	GetItemType(OBJID idItem)				{ IF_OK(m_pStorage) return m_pStorage->GetItemTypeByID(idItem); return ID_NONE; }
    virtual DWORD	GetAmountLimit(OBJID idItem)			{ IF_OK(m_pStorage) return m_pStorage->GetItemAmountLimitByID(idItem); return 0; }

protected:
    CPackage*		m_pStorage;
    //LW 拍买相关-----------------------------------------------------------------
public:
    int  GetAuctionPackageAmount(OBJID idNpc, int nPosition = ITEMPOSITION_AUCTION_STORAGE);
    bool AuctionGoodsBackOff(CItem* pItem, int nPosition = ITEMPOSITION_AUCTION_STORAGE);
    //----------------------------------------------------------------------------
public:	// modify attrib ------------------------------
    int			GetAddPoint			();

    int			GetAutoAllot		()					{return m_data.GetAutoAllot();}
    void		SetAutoAllot		(int nAutoAllot)	{m_data.SetAutoAllot(nAutoAllot);}

    //	int			GetMetempsychosis		()					{return m_data.GetMetempsychosis();}
    void		SetMetempsychosis		(int nMetempsychosis)	{m_data.SetMetempsychosis(nMetempsychosis);}
    void		SetMaxLifePercent	(int nPercent, BOOL bUpdate = TRUE);
    //checked ok
    bool		AddAttrib			(int idxAttr, __int64 i64Data, int nSynchro);
    void		SetAttrib			(int idxAttr, __int64 i64Data, int nSynchro);

public:	// marriage ------------------------------
    void		UpdateMate			(const char* pszName, bool bUpdate = true);
    bool		IsMarried			(void);
    bool		IsMate				(CUser* pUser);
    bool		Court				(OBJID idTarget);
    bool		Marry				(OBJID idTarget);
    bool		Divorce				(void);

public: // master & Prentic ----------------------------------
    bool		IsMasterPrentice	(CUser* pUser)		{ return false; }

public: // mount
protected:
    CTimeOut	m_tMount;

public:	// sprite -- add by zlong 2003-11-28
protected:
    CTimeOut			m_tSprite;

public: // cheat --------------------------------------------------------
    enum { _TYPE_MAGIC = 0, _TYPE_WS = 1, _TYPE_FY = 2, _TYPE_USE_LIFE = 3, _TYPE_USE_MANA = 4, }; // 无双// 风云	// 用红药// 用蓝药
    bool		SetCheatLevel(int nLevel);
    void		KickoutCheat(OBJID idKind);
    int			GetCheatLevel()						{ return m_nCheatLevel; }
    bool		IsCheater(OBJID idKind);
    bool		IsMagicAtkCheat	(OBJID idTarget, int nTargetX, int nTargetY, DWORD dwTimeStamp);
    void		SetCheatMark(OBJID idCheatKind, DWORD dwCheatMark);
    void		DoCheaterPunish	(int nData = 0);

    DWORD		m_dwLastJump;
    DWORD		m_dwMsgCount;

    bool		CheckTimeStamp(DWORD dwTimeStamp)				{ if (dwTimeStamp - m_dwLastTimeStamp > 50) { m_dwLastTimeStamp = dwTimeStamp; return true; } return false; }

protected:
    void		SetCheat(int nType, int nLevel);
protected:
    auto_long	m_nCheatLevel;
    auto_long	m_nCheatCount;
    auto_long	m_maskCheat;

    deque<DWORD>	m_setActionTime;

    auto_long	m_dwLastTimeStamp;

protected:
    virtual		I64		SetEffect			(I64 i64Effect)	{ m_data.m_Info.i64Effect |= i64Effect; return m_data.m_Info.i64Effect; }
    virtual		I64		ResetEffect			(I64 i64Effect = 0xFFFFFFFFFFFFFFFF)	{ m_data.m_Info.i64Effect &= (~i64Effect); return m_data.m_Info.i64Effect; }

public:
    UserInfoStruct*	GetInfo	(void)		{return &m_data.m_Info;}

    virtual DWORD			GetWeightLimit	();
    //	virtual CGameMap*		GetMap			()			{ CHECKF(m_pMap); return m_pMap; }


public: // apply ---------------------------------------------
    enum
    {
        APPLY_NONE = 0,
        APPLY_FRIEND,
        APPLY_MARRY,
        APPLY_TRADE,
        APPLY_TEAMAPPLY,
        APPLY_TEAMINVIT,
        APPLY_JOIN_SYN,
        APPLY_INVITE_JOIN_SYN,
        APPLY_ALLY_SYN,

        APPLY_STUDENTAPPLY,		// 申请收徒弟
        APPLY_TEACHERAPPLY,		// 申请拜师
    };

    void	SetApply		(int nApply, OBJID idTarget)	{ m_nApply = nApply; m_idApplyTarget = idTarget; }
    OBJID	FetchApply		(int nApply)					{ if (nApply == m_nApply) { m_nApply = APPLY_NONE; return m_idApplyTarget; } return ID_NONE; }
    bool	FetchApply		(int nApply, OBJID idTarget)	{ if (nApply == m_nApply && idTarget == m_idApplyTarget) { m_nApply = APPLY_NONE; return true; } return false; }
protected:
    int		m_nApply;
    OBJID	m_idApplyTarget;

public:	//-------- 不知道的类型 -----------------------
    //	void		InitWalkStep		()				{ m_nWalkStep = 0; }

    void		SetForce			(DWORD dwForce);
    DWORD		GetForce			()				{ return m_data.GetForce();}

    void		SetDexterity		(DWORD dwDex);

    void		SetSpeed			(DWORD dwSpeed);
    DWORD		GetSpeed			()				{ return m_data.GetSpeed();}
    // 根据状态调整速度
    DWORD		AdjustSpeed			(int nSpeed);

    void		SetHealth			(DWORD dwHealth);
    DWORD		GetHealth			()				{ return m_data.GetHealth();}

    void		SetSoul				(DWORD dwSoul);
    DWORD		GetSoul				()				{ return m_data.GetSoul();}
    DWORD		GetSoulSum			();

    void		SetFace				(DWORD dwFace);
    DWORD		GetFace				()				{ return (m_data.GetLookFace() / 10000) % 1000; }
    void		SetLook				(DWORD dwLook);

    void		ClsXpVal			(bool bSynchro = SYNCHRO_TRUE);
    void		ProcXpVal			();
    bool		IsFullXp			()				{ return m_dwXP >= MAX_USER_XP; }
    bool		BurstXp				();
    void		SetXp				(int nXp);
    void		AddXp				(int nXp);

    OBJID		GetHomeID			()				{ return m_data.GetHomeID(); }
    OBJID		GetMateHomeID();

    int			GetRace				()				{ if (GetLook(GetLookFace()) > 6) { return RACE_NONE; } return (GetLook(GetLookFace()) + 1) / 2; }

    void		ProcSlowHealLifeUp	();
    void		ProcSlowHealManaUp	();
    void		ProcAutoHealLife	();		// 每隔5秒自动补血，跑跳不补血

#ifdef _DEBUG
    void		SetName	(const char* pszName)	{ m_data.SetName(pszName); }
#endif

public: // profession
    DWORD		GetProfession		()	{ return m_data.GetProfession(); }
    DWORD		GetProfessionSort	()	{ return (m_data.GetProfession() % 1000) / 10; }
    DWORD		GetProfessionLevel	()	{ return m_data.GetProfession() % 10; }

public:	//---------- USER专有的战斗函数 ----------------------
    void		IncAddPoint			(int nAddPoint);
    void		SetExp				(int nExp)				{m_data.SetExp(nExp);}
    void		CalcFightRate		()						{ m_nFightRate = GetInterAtkRate(); }
    void		ProcessAutoAttack	();
    int			GetDelay			()						{ return m_nFightPause; }
    void		SetDelay			(int nDelay)			{ m_nFightPause = nDelay; }
    int			AdjustExp			(IRole* pTarget, int nRawExp, bool bNewbieBonusMsg = false);
    bool		CheckCrime			(IRole* pRole);
    bool		IsImmunity			(IRole* pRole);

public:	//--------- 队伍管理 --------------------------------
    BOOL		CreateTeam			();
    BOOL		AcceptTeamMember	(CUser* pMemeber);
    BOOL		AcceptTeamInvit		(CUser* pLeader);
    BOOL		QuitTeam			();
    BOOL		DismissTeam			();
    BOOL		KickOut				(OBJID idMember);
    void		SetTeam				(CTeam* pTeam);
    void		ClrTeam				();
    CTeam*		GetTeam				();
    void		ProcessTeamTimer	();
    void		SendShow			(CUser* pNewMember);
    void		AppendTeam			(TeamInfoStruct* pInfo);
protected: // IChangeTeam
    virtual bool AddTeamMember(OBJID idUser);
    virtual bool DelTeamMember(OBJID idUser);
    virtual bool DelTeam();
    virtual bool SendInfo(SOCKET_ID idSocket, OBJID idAgent);		// agent overload

private:
    CTeam*		m_pTeam;
    CTimeOut	m_tTeam;



public: //--------- 玩家行为 --------------------------
    int			GetPk				()				{ return m_data.GetPk(); }
    int			IncPk				()				{ return AddPk(PKVALUE_ONEKILL); }
    int			GetPkMode			()				{ return m_data.m_Info.ucPkMode; }
    void		SetPkMode			(int nMode)		{ m_data.m_Info.ucPkMode = nMode; }
    bool		IsPkEnable			(IRole* pRole);
    bool		IsGhost				()				{ return m_bGhost; }
    void		Reborn				(bool bChgPos = true);
    void		TransformGhost		();
    int			CalcExpLostOfDeath	(IRole* pKiller);

protected:
    bool		m_bGhost;

public:	// 可用于任务系统
    int			AddPk				(int nAdd);
    void		ProcessPkTimer		(void);			// auto synchro
    void		SendSystemBBS		(LPCTSTR szText);

public: // item ------------------------------------------
    enum { IDENT_OK = true, NO_COMBINE = false, };
    CItem*	AwardItem		(int nItemType, bool bSynchro, bool bIdentOK = false, bool bAutoCombine = true);		// 同时操作数据库
    CItem*	AwardItem		(ItemInfoStruct* pInfo, bool bSynchro, bool bAutoCombine = true);		// 同时操作数据库
    bool	EraseItem		(OBJID idItem, bool bSynchro);			// 同时操作数据库
    bool	EraseEquip		(int nPosition, bool bSynchro);			// 同时操作数据库
    CItem*	GetItem			(OBJID idItem);
    CItem*	GetItemByType	(OBJID idItemType);
    CItem*	GetEquipItem	(OBJID idItem);
    CItemPtr*	GetEquipItemPtr	(int nPosition);
    CItemPtr&	GetEquipItemRef	(int nPosition)						{ CItemPtr* ppEquip = GetEquipItemPtr(nPosition); ASSERT(ppEquip); return *ppEquip; }
    CItem*	GetEquipItemByPos	(int nPosition);
    bool	AddItem			(CItem* pItem, bool bSynchro, bool bUpdate = true);
    bool	DelItem			(OBJID idItem, bool bSynchro);
    bool	DelItemPt		(OBJID idItem);							// 只删除指针
    CItem*	PopItem			(OBJID idItem, bool bSynchro, bool bUpdate);			// 立即清除OWNER
    bool	TryItem			(OBJID idItem, int nPosition);
    bool	ChkUseItem		(CItem* pItem, IRole* pTarget);
    bool	UseItem			(OBJID idItem, int nPosition, bool bSynchro);
    bool	UseItemTo		(OBJID idTarget, OBJID idItem);			// 对别人使用物品 -- zlong 2004.09.23
    bool	UserItemToByType(OBJID idTarget, OBJID idType);			// 根据物品类型对别人使用物品——用于武器附带的魔魂宝石攻击效果
    bool	UnEquipItem		(int nPosition, bool bSynchro);
    bool	SplitItem		(OBJID idItem, int nNum);
    bool	CombineItem		(OBJID idItem, OBJID idOther);
    CItem*	FindCombineItem	(OBJID idType);
    CItem*	CombineNewItem	(CItem* pItem, ItemInfoStruct* pInfo, bool bSynchro);
    bool	PickMapItem		(OBJID idItem);
    bool	DropItem		(OBJID idItem, int x, int y);
    bool	DropMoney		(DWORD dwMoney, int x, int y);
    bool	RepairItem		(OBJID idItem, bool bSynchro);
    bool	RepairAll		(bool bSynchro);
    bool	IdentItem		(OBJID idItem, bool bSynchro);
    CItem*	GetTaskItem		(LPCTSTR szItemName);
    CItem*	GetTaskItemByType		(OBJID idItemType, int nAmount = 0);
    bool	IsItemFull		(int nWeight, OBJID idItemType, DWORD dwData);	//					{ return !IsBackPackSpare(1, nWeight); }
    bool	IsBackPackSpare	(int nNum, int nWeight, OBJID idItemType, DWORD dwData)		{ return m_pPackage->IsPackSpare(nNum, idItemType, dwData); }		//{ if (nWeight && GetWeight()+nWeight > GetWeightLimit() || nNum && m_setItem.size()+nNum > _MAX_USERITEMSIZE) return false; return true; }
    int		GetWeight		();
    CItem*	FindSpaceTransSpell();
    void	AddEquipmentDurability		(int nPosition, int nAddValue);
    bool	MultiDelItem(OBJID idTypeFirst, OBJID idTypeLast, int nNum);
    bool	MultiCheckItem(OBJID idTypeFirst, OBJID idTypeLast, int nNum);
    bool	DecEquipmentDurability(bool bBeAttack, bool bMagic, int bDurValue = 1);
    bool	SpendItemByType	(OBJID idType, bool bSynchro);
    bool	SpendEquipItem	(int nSubType, int nNum, bool bSynchro);
    virtual bool	SpendArrow();
    bool	SpendItem(CItem* pItem, int nNum = 1, int nPosition = ITEMPOSITION_BACKPACK, bool bSynchro = true);	// may be erase
protected:
    bool	EquipItem		(CItem* pItem, int nPosition, bool bSynchro);
    CItem*	UnEquipOnly		(int nPosition);
    void	UpdateWeight	();
    void	UpdateEquipmentDurability	();
protected:
    bool	CreateAllItem	(IDatabase* pDb);
    void	SaveItemInfo	();
    void	DeleteAllItem	();
    void	SendAllItemInfo	();
public: // msgitem
    void	SendAllEquipInfoTo(CUser* pTarget);
    void	BuyItem			(OBJID idNpc, OBJID idType);
    void	SellItem		(OBJID idNpc, OBJID idItem);
protected: // member
    bool	m_bUpdateWeight;		// false: m_nAllWeight值无效，需要重新计算
    int		m_nAllWeight;			// 当前总负重量的CACHE
    int		m_idxLastArrow;

public: // task & action -------------------------------------------------------------------------------
    void	ClearTaskID		()									{ m_setTaskID.clear(); m_setIterID.clear(); }
    void	SetTaskNpcID	(OBJID idNpc)						{ m_idTaskNpc = idNpc; }
    void	SetTaskItemID	(OBJID idItem)						{ m_idTaskItem = idItem; }
    OBJID	GetTaskItemID	()									{ return m_idTaskItem; }
    OBJID	GetTaskItemActionID()								{ return m_idTaskItemAction; }
    void	SetTaskItemActionID(OBJID idAction)					{ m_idTaskItemAction = idAction; }
    bool	TestTask		(CTaskData* pTask);
    bool	ProcessTask		(int idx, LPCTSTR pszAccept);
    bool	ProcessClientTask(OBJID idTask, LPCTSTR pszAccept);
    //	bool	ProcessAction	(OBJID idAction, IRole* pRole = NULL, CItem* pItem = NULL, LPCTSTR pszAccept = NULL);
    bool	DebugShowAction	()									{ m_bDebugAction = !m_bDebugAction; return m_bDebugAction; }
    int&	TaskIterator	()									{ return m_nTaskIterator; }
    //	void	ReplaceAttrStr		(char* pszTarget, const char* pszSource, IRole* pRole = NULL);
    int		PushTaskID			(OBJID idTask)					{ if (idTask != ID_NONE && m_setTaskID.size() < MAX_MENUTASKSIZE) { return m_setTaskID.push_back(idTask), m_setTaskID.size() - 1; } return INDEX_NONE; }

    int		PushTaskID			(OBJID idTask, OBJID idIter);
    CUserData* QueryDataForAction()								{ return &m_data; }
    void 	SetRecordPos	(OBJID idMap, DWORD dwX, DWORD dwY, BOOL bUpdate = false);

    void	AddTaskMask		(int idx);
    void	ClrTaskMask		(int idx);
    bool	ChkTaskMask		(int idx);

    // ========= 佣兵任务相关 ============
    bool	AcceptMercenaryTask(OBJID idTask);
    bool	CancelMercenaryTask(OBJID idTask);
    bool	CompleteMercenaryTask(OBJID idTask, OBJID idItem);
    // ===================================

protected:
    OBJID	GetTaskID			(int idx)						{ if (idx >= 0 && idx < m_setTaskID.size()) { return m_setTaskID[idx]; } return ID_NONE; }
    bool	CheckItem			(CTaskData* pTask);
    void	SetIterID			(int idx)						{ if (idx >= 0 && idx < m_setIterID.size()) { m_nTaskIterator = m_setIterID[idx]; } }

protected:
    vector<OBJID>			m_setTaskID;
    vector<OBJID>			m_setIterID;
    OBJID					m_idTaskNpc;
    OBJID					m_idTaskItemAction;		// for idTaskItem
    OBJID					m_idTaskItem;
    bool					m_bDebugAction;				// 是否给GM显示每条ACTION的执行细节
    int						m_nTaskIterator;			// 用于在任务系统中叠代

public:	// wanted
    WantedInfoStruct& WantedInfo(void)		{ return m_data.GetInfo().infoWanted; }
    void	ResetWantedInfo(void)	{ memset(&m_data.GetInfo().infoWanted, 0L, sizeof(WantedInfoStruct)); }
    OBJID&	PoliceWantedID()	{ return m_data.GetInfo().idPoliceWanted; }
    void	TeamHuntKill		(OBJID idTarget, int nTargetLev, const char* pszTargetName);
    bool	IsMyWantedTarget	(CUser* pTarget);
    bool	IsPoliceWantedTarget(CUser* pTarget);

public: // magic ------------------------------------------------------------------------------------
    bool		SendAllMagicInfo()		{ return m_pMagic->SendInfoToClient(); }
    CMagic*		QueryMagic()			{ CHECKF(m_pMagic); return m_pMagic; }
    static bool	IsInCircle(POINT pos, POINT posCenter, int nRange)	{ return sqrt((double)(posCenter.x - pos.x) * (posCenter.x - pos.x) + (double)(posCenter.y - pos.y) * (posCenter.y - pos.y)) < nRange + .9; }
    //	bool		IsInFan(POINT pos, POINT posSource, int nRange, int nWidth, POINT posMidline);
    bool		IsArrowPass(int x, int y, int nAlt = ARROW_FLY_ALT);
    bool		IsJumpPass(int x, int y, int nAlt);
    bool		EquipMagicItem(CItem* pItem, int nPosition);
    bool		UnequipMagicItem(int nPosition);
    void		CalcSupermanKills();
    void		SynchroSupermanOrder(int nOrder);
    int			GetSupermanAddSecs()			{ return m_nSupermanOrder; }
    void		QuerySupermanSecs();
    void		QuerySupermanList(int nIndex, OBJID idAction, int nNumber);
protected:
    CMagic*		m_pMagic;
    int			m_nCurrSupermanKills;
    int			m_nTopSupermanKills;
    int			m_nSupermanOrder;

public: // Transformation ----------------------------------------
    bool		Transform(DWORD dwType, int nKeepSecs, bool bSynchro = true);
    CTransformation* QueryTransformation()		{ return m_pTransformation; }
    void		AbortTransform();
    bool		SynchroTransform();
protected:
    CTransformation*		m_pTransformation;
    CTimeOut				m_tTransformation;

public: // call pet -----------------------------------------------
    bool		CallPet(OBJID idMonsterType, int x, int y, int nKeepSecs = 0);
    CMonster*	QueryCallPet()			{ return m_pCallPet; }
    void		KillCallPet(bool bNow = false);
    OBJID		GetCallPetID();
protected:
    CAutoLink<CMonster>		m_pCallPet;			// may be null
    CTimeOut				m_tCallPet;			// no active for keep alive

    //---任务系统---begin
    CTaskDetail*		m_pTaskDetail;
    //---任务系统---end

public: // eudemon -- zlong 2004-02-05  -----------------------------------------------
    bool		CreateEudemon(CItem* pItem, int x, int y);
    void		CallBackEudemon(OBJID idItem, bool bNow = true);
    //========
    int			GetEudemonAmount()			{ return m_setEudemon.size(); }
    CMonster*	QueryEudemonByIndex(int nIdx);
    CMonster*	QueryEudemonByID(OBJID idEudemon);
    CMonster*	QueryEudemon(OBJID idItem);
    //========

    bool		HatchEudemon(CItem* pItem);
    bool		EvolveEudemon(OBJID idEudemonItem, int nType);
    bool		Evolve2Eudemon(OBJID idEudemonItem, int nType);
    bool		RebornEudemon(OBJID idEudemonItem, OBJID idGem);
    bool		EnhanceEudemon(OBJID idEudemonItem, OBJID idGem);

    void		AwardEudemonExp(OBJID idItem, int nExp, bool bGemEffect = true);

    bool		AttachEudemon(OBJID idItem);
    void		DetachEudemon(CItem* pItem = NULL);

protected:
    void		CallBackAllEudemon(bool bNow = true);

protected:
    //========
    struct ST_EUDEMON
    {
        CAutoLink<CMonster>		pEudemon;
        CItemPtr				pEudemonItem;
        CTimeOut				tEudemon;
    };
    typedef std::deque<ST_EUDEMON*>	EUDEMON_SET;
    EUDEMON_SET		m_setEudemon;

    OBJID		m_idLinkEudemonItem;
    OBJID		m_idLinkEudemonType;
    int			m_nLinkValue;
    typedef std::vector<int>	LINKMAGIC_SET;
    LINKMAGIC_SET	m_setLinkMagic;

protected:
    ST_EUDEMON*	QueryEudemonSt(OBJID idItem);
public: // syndicate ---------------------------------------------------------------------------------
    bool		SendAllSynInfo()		{ return m_pSyn->SendInfoToClient(); }
    CSynAttr*	QuerySynAttr()			{ CHECKF(m_pSyn); return m_pSyn; }		//? 调用时注意检查是否有帮派
    void		AwardSynWarScore(CNpc* pNpc, int nScore);
    bool		IsHostileSynMember	(CUser* pUser);
    CSyndicate* GetSyndicate	(void);
    int			QuerySynProffer	(void)	{ if (m_pSyn) { return m_pSyn->GetProffer(); } return 0; }
    void		SetSynProffer	(int nData)		{ if (m_pSyn) { m_pSyn->SetProffer(nData); } }
    int			QuerySynRank	(void)	{ if (m_pSyn) { return m_pSyn->GetSynRankShow(); } return RANK_NONE; }
    bool		IsMemberOfSyn	(OBJID idSyn);

protected: // data
    CSynAttr*		m_pSyn;
    CTimeOut		m_tIncRepute;
    CTimeOut		m_tIncPotential;

public: // user status --------------------------------------------------------------------------------
    //	IStatus*	QueryStatus(OBJID idType)		{ CHECKF(m_setStatus); return m_setStatus->GetObj(idType); }
    //	IStatusSet*	QueryStatusSet()				{ CHECKF(m_setStatus); return m_setStatus; }
    void		SetCrimeStatus(int nStatus = STATUS_CRIME, int nTime = CRIME_KEEP_SECS);
    void        SetSelfDefStatus(int nTime = SELF_DEFENCE_SECS);    //add huang 2004.1.14
protected: // data
    //	IStatusSet*	m_setStatus;

public: // friend --------------------------------
    int			GetFriendAmount	()					{ return m_setFriend.size(); }
    CFriend*	GetFriendByIndex(int idx)			{ CHECKF(idx >= 0 && idx < m_setFriend.size()); return m_setFriend[idx]; }
    CFriend*	GetFriend		(OBJID idFriend);
    bool		AddFriend		(OBJID idFriend, LPCTSTR szFriendName);		// 未检查重复添加
    bool		DelFriend		(OBJID idFriend);			// 同时删除记录
    void		BroadcastFriendsMsg	(CNetMsg* pMsg);
protected:
    bool		CreateAllFriend	(IDatabase* pDb);
    void		DeleteAllFriend	();
    void		SendFriendInfo	();

public:	// tutor&student ------------------------------------
    // tutor
    CTutor*		GetTutor			()					{ return m_pTutor; }
    bool		AddTutor			(OBJID idTutor, LPCTSTR szName);
    bool		DelTutor			();

    void		AwardTutorExp		(int nTutorExp);	// 获得导师经验
    bool		TakeExpFromStudent	(int nExpTake);		// 取得学员贡献的经验

    // student
    int			GetStudentAmount	()					{ return m_setStudent.size(); }
    CTutor*		GetStudentByIndex	(int idx)			{ CHECKF(idx >= 0 && idx < m_setStudent.size()); return m_setStudent[idx]; }
    CTutor*		GetStudent			(OBJID idStudent);
    bool		DelStudent			(OBJID idStudent);
    bool		IsStudentFull		()					{ return (GetStudentAmount() >= (GetTutorLevel() + 1) * 5); }

    void		SendTutorInfo		();
    void		NotifyTutorStatus	(int nStatus);

protected:
    bool		DelTutorRef			();
    bool		AddStudentRef		(const TutorInfoStruct* pInfo);
    bool		DelStudentRef		(OBJID idStudent);

    bool		CreateAllTutor	(IDatabase* pDb);
    void		DeleteAllTutor	();

public: // enemy ------------------------------------
    CEnemy*		QueryEnemy()						{ return m_pEnemy; }
protected:
    CJavaObj<CEnemy>	m_pEnemy;

public:	//-------------- 武器熟练度 --------------------
    void		SendAllWeaponSkillInfo	();
    void		AddWeaponSkillExp		(int nSkillType, int nExp);
    bool		CheckWeaponSubType		(int nSubType, int nNum = 0);

    BOOL		AwardWeaponSkill	(int nSkillType, int nLev);
    CUserWeaponSkill* GetWeaponSkill(int nSkillType);

    DWORD		GetWeaponSkillLev	(OBJID idWeaponType);
    bool		UnlearnAllSkill		();

protected:
    BOOL		CreateWeaponSkill	(IDatabase* pDb);
    bool		SaveAllWeaponSkillInfo	();

public: // money --------------------------------
    virtual bool	SpendMoney	(DWORD nMoney, bool bSynchro);
    virtual bool	GainMoney	(DWORD nMoney, bool bSynchro);
    bool			SaveMoney	(DWORD nMoney, bool bSynchro = false);
    bool			DrawMoney	(DWORD nMoney, bool bSynchro = false);
public: // tick --------------------------------
    void			ProcessTick	(DWORD dwTime);
protected:
    deque<DWORD>	m_dequeServerTick;
    DWORD			m_dwFirstServerTick;
    DWORD			m_dwLastServerTick;

    DWORD			m_dwFirstClientTick;
    DWORD			m_dwLastClientTick;

    DWORD			m_dwLastRcvClientTick;

public: // weather ----------------------------------
    OBJID			GetWeatherID()					{ return m_idWeather; }
    void			SetWeatherID(OBJID id)			{ m_idWeather = id; }
    bool			SetWeather(int nType, int nIntensity, int nDir, DWORD nColor, DWORD nKeepSecs, DWORD nSpeedSecs);
protected:
    OBJID			m_idWeather;

public: // energy -------------------------------------
    void		SetEnergy			(int nEnergy)	{ m_data.m_Info.usEnergy = nEnergy; }
    int			GetEnergy			()				{ return m_data.m_Info.usEnergy; }
    int			GetMaxEnergy		()				{ IStatus* pStatus = QueryStatus(STATUS_MAXENERGY); if (pStatus) { return ::CutTrail(0, AdjustData(m_data.m_Info.usMaxEnergy, pStatus->GetPower(), 100)); } return m_data.m_Info.usMaxEnergy; }
    void		AddEnergy			(int nEnergy)	{ m_data.m_Info.usEnergy = ::CutRange(m_data.m_Info.usEnergy + nEnergy, 0, GetMaxEnergy()); }
    void		ResetEnergyInc		()				{ m_tEnergy.Update(); }
protected:
    CTimeOut		m_tEnergy;

public: // light --------------------------------------
    void		SetLight			(int dwRGB, int nSecs = 0);
    void		SendLight			();

public: // CBooth -------------------------------------
    CBooth*		QueryBooth()						{ return m_pBooth; }
    bool		CreateBooth(int nPosX, int nPosY, int nDir)	{ if (m_pBooth) { return false; } return (m_pBooth = CBooth::CreateNew(m_idProcess, this, nPosX, nPosY, nDir)) != NULL; }
    void		DestroyBooth()						{ if (m_pBooth) { m_pBooth->ReleaseByOwner(); } m_pBooth = NULL; }
protected:
    CBooth*			m_pBooth;		// NULL: no in booth

public: // login, return false: logout_user
    bool			EnterMap		();
    bool			LeaveMap		();
    bool			SendItemSet		();
    bool			SendGoodFriend	();

public: // rebirth ---------------------------------------
    bool		Rebirth(int nProf, int nLook);
protected:
    bool		CheckRebirth(int nProf, CRebirthData* pData);
    void		ResetUserAttrib(int nOldProf, int nOldLevel, int nNewProf, int nNewLevel, int nNewLook);
    void		ResetEquipLevel(int nNewLevel);
    int			GetRebirthAddPoint(int nOldProf, int nOldLevel);

public: // mapgroup ---------------------------------
    bool			ChangeMapGroup	(PROCESS_ID idProcess, OBJID idMap, int nPosX, int nPosY);	// out
    bool			JoinMap			(OBJID idMap, int nPosX, int nPosY);		// in
    bool			AppendObjInfo	(INFO_ID nType, void* pInfo);
protected:
    bool			AppendItem		(ItemInfoStruct* pInfo);
    bool			AppendFriend	(FriendInfoStruct* pInfo);
    bool			AppendWeaponSkill	(UserWeaponSkillInfoStruct* pInfo);
    bool			AppendStatus	(StatusInfoStruct* pInfo);
    bool			LeaveMapGroup	();

public: // common ------------------------------------------
    CAutoLink<CUser>&	QueryLink	()					{ return m_link; }

public: // IUserSquareDeal ------------------------------------------
    bool	CreateSquareDeal		(CUser* pTarget);
    ISquareDeal*	QuerySquareDeal	()		{ m_ifaceSquareDeal.CheckDistance(); return m_ifaceSquareDeal.m_pSquareDeal; }

    class XUserSquareDeal : IUserSquareDeal			// SquareDeal 接口
    {
    public: // constructor
        XUserSquareDeal()			{ m_pSquareDeal = NULL; }
        ~XUserSquareDeal()			{ if (m_pSquareDeal) { m_pSquareDeal->Release(); } }
        void	CheckDistance();

    public:
        virtual bool	Create		(ISquareDeal* pSquareDeal);
        virtual ULONG	Release		();
        virtual CUser*	QueryUser	()					{ return This(); }

    public: // application
        // return false: too weight
        virtual bool	AddItem		(CItem* pItem, int nAmount, int nAllWeight);
        // return false: too more
        virtual bool	AddMoney	(int nAllMoney);
        virtual void	ClickOK		();

    protected:
        ISquareDeal*		m_pSquareDeal;

    private:
        CUser*		This()	{ return m_pParent; }
        CUser*		m_pParent;
        friend class CUser;
    }	m_ifaceSquareDeal;

    friend class CUser::XUserSquareDeal;

protected: // 登录控制量
    enum { STEP_NONE = 0, STEP_LOGIN, STEP_ENTERMAP };
    int				m_nStep;

protected: // 玩家管理的对象
    typedef vector<CFriend*>	FRIEND_SET;
    FRIEND_SET	m_setFriend;

    typedef vector<CTutor*>		TUTOR_SET;
    TUTOR_SET	m_setStudent;
    CTutor*		m_pTutor;
    DWORD		m_dwExpToTutor;

    IUserPackage*	m_pPackage;	// 背包
public:
    IUserPackage*	QueryPackage()		{ ASSERT(m_pPackage); return m_pPackage; }

protected:

    CItemPtr	m_pHelmet;
    CItemPtr	m_pNecklace;
    CItemPtr	m_pArmor;
    CItemPtr	m_pWeaponR;
    CItemPtr	m_pWeaponL;
    CItemPtr	m_pRingR;
    CItemPtr	m_pRingL;
    CItemPtr	m_pShoes;
    CItemPtr	m_pMount;
    CItemPtr	m_pSprite;	// 精灵 -- add by zlong 2003-11-27
    CItemPtr	m_pMantle;	// 披风 -- zlong 2004-02-04
	CItemPtr	m_pFashion;	//时装
    //---jinggy---2004-11-19---装备经验值---begin
    CTimeOut    m_arrTimeOutForEquipExp[ITEMPOSITION_EQUIPEND-ITEMPOSITION_EQUIPBEGIN];
    int			m_arrEquipIncExp[ITEMPOSITION_EQUIPEND-ITEMPOSITION_EQUIPBEGIN];//设备战魂经验值的增量
    CTimeOut    m_tEquipWarGhostExp; //检测装备的战魂间隔时间
    bool		m_bIsEnableWarGhostExp;

    CTimeOut	m_tTimeOnline; //帮派成员在线时间
    //---jinggy---2004-11-19---装备经验值---end

    CUserData	m_data;

protected: // 玩家状态
    //	int			m_nPosX;
    //	int			m_nPosY;
    //	int			m_nDir;
    int			m_nPose;
    int			m_nEmotin;
    int			m_dwXP;
    int			m_nSynDressArmorType;
    Array<int>	m_setSlowHealUp2Life;
    Array<int>	m_setSlowHealUp2Mana;
    //	CGameMap*	m_pMap;

protected:
    void UpdateEquipmentExp_Init();
    void UpdateEquipmentExp_Startup(DWORD dwPos, int nSecs);
    void UpdateEquipmentExp_Stop(DWORD dwPos);
    void UpdateEquipmentExp();
public:
    //---jinggy---2004-11-12---用武器上嵌入的攻击类宝石攻击对方
    bool UseSkillAtk(OBJID idDes);
public:
    CTaskDetail* GetTaskDetail();
    void SetSynDressArmorType(int nArmorType);
    //	void NotifyChatRoomOffline();
    bool ApplyAttackSyn(CNpc* pNpc);
    int GetAvailableSynFund();
    bool FlyToRandomPos();
    bool FlyToMainMap();
    bool IsEmbedGemType(int nGemType);
    DWORD GetLastLogin();
    void AddMoney(DWORD dwMoney);
    bool IsSynLeader();
    bool CheckBadge(int nItemType = 0);
    bool DelAllBadge();
    bool AddBadge();
    void ProcessMsgAction(int nAction, int nData, OBJID id);
    DWORD		m_dwLastCoolShow;

private:
    IUserWeaponSkillSet*	m_pUserWeaponSkillSet;

protected: // fight
    CBattleSystem*		m_pBattleSystem;
    CTimeOutMS			m_tFight;
    CTimeOut			m_tXp;
    CTimeOut			m_tPK;
    CTimeOutMS			m_tSlowHealLife;
    CTimeOutMS			m_tSlowHealMana;
    CTimeOut			m_tAutoHealLife;					// 每几秒自动补血。跑、跳不给补血。
    CTimeOut			m_tEquipmentDurability;				// 装备耐久度损耗
    CTimeOut			m_tAutoHealMaxLife;					// 每分钟自动恢复最大血量
    int					m_nFightRate;						// 攻击的间隔毫秒数
    int					m_nFightPause;						// 僵硬状态时，一次性增加的间隔毫秒数

protected: // 应用控制量
    //	int			m_nWalkStep;
    //	CTimeOut	m_tLock;
    CAutoLink<CUser>	m_link;

protected: // 控制量
    SOCKET_ID	m_idSocket;
    //	PROCESS_ID	m_idProcess;
protected:
    //---jinggy---2004-11-17---升级装备经验值---已经穿在身上的装备的map---begin
    map<OBJID, CTimeOut*> m_mapEquipmentsUsed ;
    typedef pair <OBJID, CTimeOut*> PairID;
    //---jinggy---2004-11-17---升级装备经验值---已经穿在身上的装备的map---end

    MYHEAP_DECLARATION(s_heap)
};



#endif // !defined(AFX_USER_H__BF5C20F1_5DBE_4E65_9099_E96EFAD7ECBA__INCLUDED_1)
