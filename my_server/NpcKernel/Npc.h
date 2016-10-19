
//**********************************************************
// 代码编辑器
//**********************************************************

// Npc.h: interface for the CNpc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NPC_H__6093FC28_BAC7_4C65_AF21_721F11F1C975__INCLUDED_)
#define AFX_NPC_H__6093FC28_BAC7_4C65_AF21_721F11F1C975__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameObj.h"
#include "Timeout.h"
#include "NpcType.h"
#include "I_MapData.h"
#include "I_Role.h"
#include "MsgWalk.h"
#include "NpcGenerator.h"
#include "AutoPtr.h"
#include "item.h"

//////////////////////////////////////////////////////////////////////
//const int	ITEMSORT_WEAPON2				=	5;	// 双手武器
//const int	DWEAPON_BOW					=	00000;// 弓

//////////////////////////////////////////////////////////////////////
const int	THINK_SECS					= 1;		// 休息状态时每3秒侦察一下周围
const int	DORMANCY_SECS				= 60;		// 周围无PLAYER时，60秒休眠
const int	SHORTWEAPON_RANGE_LIMIT		= 2;		// 近身武器的最大攻击范围(以此范围是否弓箭手)
const int	NPC_CLIMBCAP				= 26;		// NPC爬山的能力(高度差)
const int	GUARD_LEAVEDISTANCE			= 48;		// 卫兵放弃追杀的距离
const int	PKKILLER_JUMPDISTANCE		= 6;		// 触发卫兵跳的距离
const int	RANDOMMOVE_RATE				= 5;		// NPC休闲时随机走动的频度为1/20
const int	GUARD_RANDOMMOVE_RATE		= 5;		// 卫兵休闲时随机走动的频度为1/10
const int	SIMPLEMAGIC_RECRUIT			= 1020;		// 加血魔法

//////////////////////////////////////////////////////////////////////
enum {	ATKUSER_LEAVEONLY				= 0,				// 只会逃跑
        ATKUSER_PASSIVE					= 0x0001,				// 被动攻击
        ATKUSER_ACTIVE					= 0x0002,				// 主动攻击
        ATKUSER_RIGHTEOUS				= 0x0004,				// 正义的(卫兵或玩家召唤和控制的怪物)
        ATKUSER_GUARD					= 0x0008,				// 卫兵
        ATKUSER_PPKER					= 0x0010,				// 追杀黑名
        ATKUSER_JUMP					= 0x0020,				// 会跳
        ATKUSER_FIXED					= 0x0040,				// 不会动的
        ATKUSER_FASTBACK				= 0x0080,				// 速归
        ATKUSER_LOCKUSER				= 0x0100,				// 锁定攻击指定玩家，玩家离开自动消失
        ATKUSER_LOCKONE					= 0x0200,				// 锁定攻击首先攻击自己的玩家
        ATKUSER_ADDLIFE					= 0x0400,				// 自动加血
        ATKUSER_EVIL_KILLER				= 0x0800,				// 白名杀手
        ATKUSER_WING					= 0x1000,				// 飞行状态
        ATKUSER_NEUTRAL					= 0x2000,				// 中立的
        ATKUSER_ROAR					= 0x4000,				// 出生时全地图怒吼
        ATKUSER_NOESCAPE				= 0x8000,				// 不会逃跑
        ATKUSER_EQUALITY				= 0x00010000,				// 不藐视
     };
const int	ATKUSER_NO_DORMANCY				= ATKUSER_LOCKUSER;

//////////////////////////////////////////////////////////////////////
enum { OWNER_NONE, OWNER_USER, OWNER_SYN, OWNER_NPC };

//////////////////////////////////////////////////////////////////////
class CNetMsg;
class CNpc : public CGameObj, INpc
{
public:
    CNpc();
    virtual ~CNpc();
    static CNpc*	CreateNew()			{ return new CNpc; }
    void			OnTimer();
    virtual bool	Create(OBJID idNpc, int nType, OBJID idMap, int nPosX, int nPosY, OBJID idGenerator, OBJID idOwner, int nOwnerType, int nData);
    void		ReleaseByOwner()		{ delete this; }
    IRole*		QueryRole()				{ return (IRole*)this; }
    IRoleAttr*	QueryRoleAttr()			{ return static_cast<IRoleAttr*>(this); }
    virtual CAutoLink<IRole>&	QueryLink(const CAutoLink<IRole>&)				{ return m_lnkThis; }
    template<class T>
    T*		QueryInterface(T** ppT = NULL)	{ if (ppT) { *ppT = this; } return this; }
    virtual void*	QueryInterface(int id, void** ppT)	{ void* p = NULL; if (id == GUID_OF(CNpc)) { p = this; } if (ppT) { *ppT = p; } return p; }

public: // get info
    OBJID		GetID()				{ return m_id; }
    OBJID		GetType()			{ return m_pType->GetID(); }
    LPCTSTR		GetStr(NPCTYPEDATA idx)	{ return m_pType->GetStr(idx); }
    int			GetInt(NPCTYPEDATA idx)	{ return m_pType->GetInt(idx); }
public: // get
    int			GetLife()			{ return m_nCurrLife; }
    int			GetPower()			{ return m_nCurrPower; }
    OBJID		GetMapID();
    CGameMap*	GetMap()			{ return m_pMap; }
    int			GetPosX()			{ return m_nPosX; }
    int			GetPosY()			{ return m_nPosY; }
    int			GetDir()			{ return m_nDir; }
    IGenerator* 	GetGenerator()			{ return m_pGenerator; }
    int			GetAttackRange(int nSizeAdd)			{ return GetInt(NPCTYPEDATA_ATKRANGE) + (GetSizeAdd() + nSizeAdd + 1) / 2; }
    int			GetSizeAdd()		{ return GetInt(NPCTYPEDATA_SIZEADD); }
    I64			GetEffect()			{ return m_i64Effect; }
    int			GetOwnerID()		{ return m_idOwner; }
public:
    bool		IsMonster()			{ return !IsSynPet() && !IsGuard() && !IsPkKiller() && !IsEvilKiller(); }
    bool		IsSynPet()			{ return m_nOwnerType == OWNER_SYN; }
    bool		IsOpposedSyn(OBJID idSyn);
    bool		IsLurker()			{ return 0; }
    bool		IsDormancy()		{ return (m_nActMode == MODE_DORMANCY); }
    bool		IsAlive()			{ return !m_bDie && m_nCurrLife > 0 && (GetEffect()&KEEPEFFECT_DIE) == 0; }
    bool		IsFarWeapen()		{ return GetInt(NPCTYPEDATA_ATKRANGE) > SHORTWEAPON_RANGE_LIMIT; }
    bool		IsRighteous()		{ return (GetInt(NPCTYPEDATA_ATKUSER) & ATKUSER_RIGHTEOUS) != 0; }
    bool		IsEvil()			{ return (GetInt(NPCTYPEDATA_ATKUSER) & (ATKUSER_RIGHTEOUS | ATKUSER_NEUTRAL)) == 0; }
    bool		IsGuard()			{ return (GetInt(NPCTYPEDATA_ATKUSER) & ATKUSER_GUARD) != 0; }
    bool		IsPkKiller()		{ return (GetInt(NPCTYPEDATA_ATKUSER) & ATKUSER_PPKER) != 0; }
    bool		IsActiveEnable()	{ return (GetEffect() & (KEEPEFFECT_NOT_ACTIVE)) == 0; }	// 可以动作(非冻结)
    bool		IsMoveEnable()		{ return IsActiveEnable() && (IsWalkEnable() || IsJumpEnable()); }
    bool		IsWalkEnable()		{ return (GetInt(NPCTYPEDATA_ATKUSER) & ATKUSER_FIXED) == 0; }
    bool		IsJumpEnable()		{ return (GetInt(NPCTYPEDATA_ATKUSER) & ATKUSER_JUMP) != 0; }
    bool		IsFastBack()		{ return (GetInt(NPCTYPEDATA_ATKUSER) & ATKUSER_FASTBACK) != 0; }
    bool		IsLockUser()		{ return (GetInt(NPCTYPEDATA_ATKUSER) & ATKUSER_LOCKUSER) != 0; }
    bool		IsLockOne()			{ return (GetInt(NPCTYPEDATA_ATKUSER) & ATKUSER_LOCKONE		) != 0; }
    //	bool		IsFighterFirst()	{ return (GetInt(NPCTYPEDATA_ATKUSER) & ATKUSER_FIGHTER_FIRST) != 0; }
    //	bool		IsWizardFirst()		{ return (GetInt(NPCTYPEDATA_ATKUSER) & ATKUSER_WIZARD_FIRST) != 0; }
    //	bool		IsBowmanFirst()		{ return (GetInt(NPCTYPEDATA_ATKUSER) & ATKUSER_BOWMAN_FIRST) != 0; }
    bool		IsAddLife()			{ return (GetInt(NPCTYPEDATA_ATKUSER) & ATKUSER_ADDLIFE		) != 0; }
    bool		IsEvilKiller()		{ return (GetInt(NPCTYPEDATA_ATKUSER) & ATKUSER_EVIL_KILLER	) != 0; }
    bool		IsDormancyEnable()	{ return (GetInt(NPCTYPEDATA_ATKUSER) & ATKUSER_NO_DORMANCY	) == 0; }
    bool		IsWing()			{ return (GetInt(NPCTYPEDATA_ATKUSER) & ATKUSER_WING		) != 0; }
    bool		IsBowman()			{ int nType = GetInt(NPCTYPEDATA_WEAPONR_TYPE); return ((nType % 10000000) / 100000) == ITEMSORT_WEAPON2 && ((nType % 100000) / 1000) * 1000 == DWEAPON_BOW; }
    bool		IsSimpleMagicAtk()	{ return GetInt(NPCTYPEDATA_MAGIC_TYPE) != ID_NONE && GetInt(NPCTYPEDATA_MAGIC_TYPE) != SIMPLEMAGIC_RECRUIT; }
    bool		IsCloseAttack()		{ return !IsBowman() && !IsSimpleMagicAtk(); }
    virtual bool		IsBlockNpc()								{ return (GetEffect() & KEEPEFFECT_NOT_BLOCKNPC) == 0; }
    //	bool		IsAttackEnable(IRole* pRole);
    bool		IsEscapeEnable()	{ return (GetInt(NPCTYPEDATA_ATKUSER) & (ATKUSER_NOESCAPE)) == 0; }
public: // modify
    void		JumpPos(int nPosX, int nPosY, int nDir);
    //	void		SetStatus(int nStatus)			{ m_nStatus = nStatus; }
    void		SetDir(int nDir)				{ m_nDir = nDir; }
    bool		IsAttackEnable(IRole* pTarget);

public: // set (call by msguserattrib)
    void		SetLife(int nLife)				{ m_nCurrLife = nLife; }
    void		SetPower(int nPower)			{ m_nCurrPower = nPower; }
    void		SetEffect(I64 i64Effect)		{ m_i64Effect = i64Effect; }
    void		SetSizeAdd(int nSizeAdd)		{}		// can't change npc size_add
    void		SetDie()						{ if (m_bDie) { return; } m_bDie = true; m_pGenerator->DeleteNpc(this->GetID()); }

public: // application
    bool		PathMove(int nMode);
    bool		MoveForward(int nDir, int nMode);
    bool		SynchroPos(int nSourX, int nSourY, int nTargX, int nTargY);
    bool		KickBack(int nTargX, int nTargY);
    void		SetPos(int nPosX, int nPosY);
    void		Attack();
    void		AttackOnce()					{ m_bAttackFlag = true; }
    bool		CheckTarget();				// 检查当前TARGET是否有效。同时修改m_idActTarget
    bool		FindNewTarget();
    bool		JumpBlock(int x, int y, int dir);
    bool		FarJump(int x, int y, int dir);
    void		LockMove(int nMilliSecs);

public: // change mode
    bool		ChangeMode(int nMode);			// return false : 失败
    void		WakeUp()						{ m_tDormancy.Update(); if (IsDormancy()) { ChangeMode(MODE_IDLE); } }
    void		BeAttack(IRole* pRole);
    void		BeKill(IRole* pRole)			{ ChangeMode(CNpc::MODE_IDLE); }
    bool		ProcessEscape();
    bool		ProcessAttack();
    bool		ProcessForward();
    bool		ProcessIdle();
    bool		CheckDormancy();

public: // socket
    bool		SendMsg(CNetMsg* pMsg);
    bool		SendMsg(LPCTSTR szMsg);

public: // static
    int			Distance(int x, int y)			{ return __max(abs(x - GetPosX()), abs(y - GetPosY())); }

public:	// others
    virtual bool		IsActive();
    virtual bool		Lock(bool bLock)							{ m_bLocked	= bLock; return true; }
    virtual bool		IsLocked()									{ return m_bLocked; }
protected:
    bool		m_bLocked;

    //////////////////////////////////////////////////////////////////////////////////////////
protected: // info
    OBJID		m_id;
    CNpcType*	m_pType;

protected: // attrib
    int			m_nCurrLife;
    int			m_nCurrPower;
    CAutoLink<IRole>	m_lnkThis;

public:
    enum NPC_MODE { MODE_DORMANCY = 0, MODE_IDLE, MODE_WALK, MODE_GOTO, MODE_TALK, MODE_FOLLOW, MODE_ESCAPE, MODE_FORWARD, MODE_ATTACK,  };
protected: // action
    int			m_nActMode;		// enum { attack,escape,talk,follow,goto,idle,dormancy };
    // 设置dormancy时，同时设置地图CELL的dormancy标志。由玩家消息驱动清除标志。

    OBJID		m_idMoveTarget;	// 移动但不攻击的目标ID，为了“吸引”状态而增加的，
    // 如果m_idActTarget!=ID_NONE，则一定与m_idMoveTarget相同
    OBJID		m_idActTarget;	// 攻击目标ID
    CTimeOut	m_tAction;		// 用于IDLE和ATTACK时延时

protected: // state
    int			m_nPosX, m_nPosY;
    I64			m_i64Effect;			// enum { STATUS_NORMAL=0, STATUS_FREEZE, STATUS_DIE };
    int			m_nDir;
    int			m_nPose;
    CMapPtr		m_pMap;
    CTimeOut	m_tDormancy;

protected: // fight
    CTimeOutMS	m_tMove;			// 用于控制走路的频率
    OBJID		m_idAtkMe;			// 由对方填写，自己清除
    bool		m_bAtkFirst;		// 主动攻击
    bool		m_bAttackFlag;		// 游戏服务器返回了攻击动作

protected: // pet
    int			m_nOwnerType;
    OBJID		m_idOwner;

protected: // append attribute
    CAutoPtr<IGenerator> 	m_pGenerator;		// 指向NPC发生器，与发生器的计数器同步
    bool		m_bDie;

protected: // path ////////////////////////////////////////////////////////////////////////////
    bool		FindPath(int nEscapeSteps = 0);
    bool		FindPath(int x, int y, int nEscapeSteps = 0);
    bool		DetectPath(int nNoDir);		// -1: 全允许
    bool		TestPath(int nDir);
    void		ClearPath()				{ m_nPathLen = 0; }
protected:
    POINT		m_posTarget;
    bool		m_bAheadPath;
    int			m_nPathLen;
    char		m_bufPath[_MAX_PATHSTEP];
};


#endif // !defined(AFX_NPC_H__6093FC28_BAC7_4C65_AF21_721F11F1C975__INCLUDED_)
