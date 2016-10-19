
//**********************************************************
// 代码编辑器
//**********************************************************

// Magic.h: interface for the CMagic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAGIC_H__B2689767_419D_487F_B0F9_FBB04C4619AA__INCLUDED_)
#define AFX_MAGIC_H__B2689767_419D_487F_B0F9_FBB04C4619AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "define.h"
#include "GameObj.h"
#include "GameData.h"
#include "UserTable.h"
#include "MagicType.h"
#include "T_SingleObjSet2.h"
#include "Myheap.h"
#include "MagicData.h"

#pragma warning(disable:4786)
#include <vector>



char	szMagicTable[];

//////////////////////////////////////////////////////////////////////
enum
{
    MAGICSORT_ATTACK			= 1,
    MAGICSORT_RECRUIT			= 2,			// support auto active.
    MAGICSORT_CROSS				= 3,
    MAGICSORT_FAN				= 4,			// support auto active(random).
    MAGICSORT_BOMB				= 5,
    MAGICSORT_ATTACHSTATUS		= 6,
    MAGICSORT_DETACHSTATUS		= 7,
    MAGICSORT_SQUARE			= 8,
    MAGICSORT_JUMPATTACK		= 9,			// move, a-lock
    MAGICSORT_RANDOMTRANS		= 10,			// move, a-lock
    MAGICSORT_DISPATCHXP		= 11,
    MAGICSORT_COLLIDE			= 12,			// move, a-lock & b-synchro
    MAGICSORT_SERIALCUT			= 13,			// auto active only.
    MAGICSORT_LINE				= 14,			// support auto active(random).
    MAGICSORT_ATKRANGE			= 15,			// auto active only, forever active.
    MAGICSORT_ATKSTATUS			= 16,			// support auto active, random active.
    MAGICSORT_CALLTEAMMEMBER	= 17,
    MAGICSORT_RECORDTRANSSPELL	= 18,
    MAGICSORT_TRANSFORM			= 19,
    MAGICSORT_ADDMANA			= 20,			// support self target only.
    MAGICSORT_LAYTRAP			= 21,
    MAGICSORT_DANCE				= 22,			// 跳舞(only use for client)
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
};

//////////////////////////////////////////////////////////////////////
const int	WEAPONMAGIC_OFFSET				= 10000;	// 武器上带的魔法的最低ID值，同时是ITEMTYPE中MAGICx字段加此偏移等于MAGICTYPE的ID
const int	WEAPONMAGIC_MIN					= WEAPONMAGIC_OFFSET;
const int	WEAPONMAGIC_MAX					= WEAPONMAGIC_OFFSET + 256;
const int	WEAPONMAGIC_LEVEL	= 0;					// 缺省的武器魔法等级
const int	WEAPONR_MAGICID		= 4200000001;			// 右手武器的魔法ID
const int	WEAPONL_MAGICID		= 4200000002;			// 左手武器的魔法ID

//////////////////////////////////////////////////////////////////////
const int	MAGICDAMAGE_ALT					= 26;		// 法术效果的高差限制
const int	AUTOLEVELUP_EXP					= -1;		// 自动升级的标志
const int	DISABLELEVELUP_EXP				= 0;		// 不升级的标志
const int	AUTOMAGICLEVEL_PER_USERLEVEL	= 10;		// 每10级，法术等级加一级
const int	USERLEVELS_PER_MAGICLEVEL		= 10;		// 玩家等级必须是法术等级的10倍

const int	KILLBONUS_PERCENT				= 5;		// 杀死怪物的额外EXP奖励
const int	HAVETUTORBONUS_PERCENT			= 10;		// 有导师的情况下额外EXP奖励
const int	WITHTUTORBONUS_PERCENT			= 20;		// 和导师一起练的情况下EXP奖励

const int	MAGIC_DELAY						= 1000;		// 魔法DELAY
const int	MAGIC_DECDELAY_PER_LEVEL		= 100;		// 每个“法术等级”减少的魔法DELAY
const int	RANDOMTRANS_TRY_TIMES			= 10;		// 随机瞬移的尝试次数
const int	DISPATCHXP_NUMBER				= 20;		// 加XP的数量
const int	COLLIDE_POWER_PERCENT			= 80;		// 冲撞增加攻击力的百分比
const int	COLLIDE_SHIELD_DURABILITY		= 3;		// 冲撞要减少的盾牌寿命
const int	LINE_WEAPON_DURABILITY			= 2;		// 直线攻击要减少的武器寿命
const int	MAX_SERIALCUTSIZE				= 10;		// 顺势斩的数量
const int	AWARDEXP_BY_TIMES				= 1;		// 按次数加经验值
const int	AUTO_MAGIC_DELAY_PERCENT		= 150;		// 连续魔法攻击时增加的DELAY
const int	BOW_SUBTYPE						= 500;		// 弓的SUBTYPE
const int	POISON_MAGIC_TYPE				= 1501;		// use for more status
const int	DEFAULT_MAGIC_FAN				= 120;		//
//const int	STUDENTBONUS_PERCENT			= 5;		// 导师杀死一只怪物徒弟得到的经验百分比

const int	MAGIC_KO_LIFE_PERCENT			= 15;		// 必杀技能去血上限
const int	MAGIC_ESCAPE_LIFE_PERCENT		= 15;		// 逃跑技能有效的生命上限

//////////////////////////////////////////////////////////////////////
typedef	CGameData<MAGICDATA, szMagicTable, szID>	CMagicDataBase;
class	CMagicData : public IMagicData
{
protected:
    CMagicData()			{ m_pData = NULL; }
    virtual ~CMagicData();
public: // overload
    static IMagicData* CreateNew()		{ return new CMagicData; }
    virtual void	ReleaseByOwner()	{ delete this; }
    virtual bool	Create(IRecordset* pRecordset);
    virtual bool	Create(IRecordset* pDefaultRecord, OBJID idNewKey);
    virtual bool	Create(ST_MAGIC_DATA* pMagicData);
    virtual int		GetInt(MAGICDATA nDataName);
    virtual void	SetInt(MAGICDATA idx, int nData);
    virtual bool	FindType();		//? 创建后，需要调用此函数手工同步

    virtual OBJID	GetID()				{ CHECKF(m_pData); return m_pData->GetID(); }
    virtual bool	Update()			{ CHECKF(m_pData); return m_pData->Update(); }
    virtual void	ClearUpdateFlags()	{ CHECK(m_pData); m_pData->ClearUpdateFlags(); }
    virtual OBJID	InsertRecord()		{ CHECKF(m_pData); return m_pData->InsertRecord(); }
    virtual bool	DeleteRecord()		{ CHECKF(m_pData); return m_pData->DeleteRecord(); }

protected:
    CMagicDataBase*	m_pData;

protected: // ctrl
    MYHEAP_DECLARATION(s_heap)
};

//////////////////////////////////////////////////////////////////////
typedef	ISingleObjSet2<IMagicData>		IMagicSet;
typedef	CSingleObjSet2<IMagicData>		CMagicSet;

//////////////////////////////////////////////////////////////////////
struct MagicInfoStruct
{
    OBJID	id;
    OBJID	idOwner;
    int		nType;
    int		nLevel;
    DWORD	dwExp;
};

////////////////////////////////////////////////////////////////
enum
{
    TYPE_MAGIC		= 0, // 魔法
    TYPE_XPSKILL	= 1, // xp技
    TYPE_KONGFU		= 2, // 武功招式
};

//////////////////////////////////////////////////////////////////////
enum
{
    MAGICSTATE_NONE		= 0,		// 无状态
    MAGICSTATE_INTONE	= 1,		// 吟唱状态
    MAGICSTATE_LAUNCH	= 2,		// 施展状态
    MAGICSTATE_DELAY	= 3,		// 延迟状态
};

//////////////////////////////////////////////////////////////////////
enum AUTO_ACTIVE
{
    AUTOACTIVE_KILL			= 0x01,
    AUTOACTIVE_FOREVER		= 0x02,
    AUTOACTIVE_RANDOM		= 0x04,
    AUTOACTIVE_HIDDEN		= 0x08,		// 包含此掩码的魔法不显示在客户端
    AUTOACTIVE_EXPDISABLE	= 0x10,		// 包含此掩码的魔法使用过程中不能得到经验
};

//////////////////////////////////////////////////////////////////////
enum THROW_WEAPON
{
    THROW_WERPONR_TARGET			= 1,
};

//////////////////////////////////////////////////////////////////////
inline bool		IsWeaponMagic(int nType)			{ return nType >= WEAPONMAGIC_MIN && nType < WEAPONMAGIC_MAX; }
inline int		MagicType(int idx)					{ int nType = idx + WEAPONMAGIC_OFFSET; CHECKF(IsWeaponMagic(nType)); return nType; }
inline OBJID	POSITION2MAGIC_ID(int nPosition)	{ if (nPosition == ITEMPOSITION_WEAPONR) { return WEAPONR_MAGICID; } else { return WEAPONL_MAGICID; } }
void	DDALine(int x0, int y0, int x1, int y1, int nRange, vector<POINT>& vctPoint);

//////////////////////////////////////////////////////////////////////
class CMagic
{
protected:
    CMagic(PROCESS_ID idProcess, IRole* pRole);
    virtual ~CMagic();

private:
    CMagic() { m_bAutoAttack = false;}

public:
    static CMagic* CreateNew(PROCESS_ID idProcess, IRole* pRole);
    ULONG	ReleaseByOwner()	{ delete this; return 0; }
    void	OnTimer(DWORD tCurr);

public: // login
    bool	CreateAll();
    bool	SendInfoToClient();

public: // MapGroup
    bool	SaveInfo();
    bool	GetInfoByIndex(MagicInfoStruct* pInfo, int idx);
    bool	AppendMagic(const MagicInfoStruct* pInfo);

public: // application
    IMagicSet*	QuerySet()		{ CHECKF(m_setData); return m_setData; }
    bool		MagicAttack(int nType, OBJID idTarget, int x, int y, UCHAR ucAutoActive = 0);		// return false: NO BROADCAST
    bool		AbortMagic(bool bSynchro = false);
    void		OwnerUpLevel(int nOwnerLev);

    bool		AutoLearnMagic();

    void		UserWeaponSkillUpLevel(int nWeaponSkillType, int nWeaponSkillLevel);
    bool		AutoLearnWeaponSkill(int nWeaponSkillType, int nWeaponSkillLevel, int nWeaponSkillExp = 0);
    bool		UserKillTarget(IRole* pTarget);
    bool		AutoAttackTarget(IRole* pTarget);
    IMagicData*	FindMagic(int nType);
    int			GetExtendAtkRange	(CItem* pWeaponR, CItem* pWeaponL);
    bool		AwardExp(int nType, int nBattleExp, int nExp);
    bool		AwardExp(int nBattleExp, int nExp = AWARDEXP_BY_TIMES, bool bIgnoreFlag = false);		// 同时支持外调用，但bAddBattleExp应该为0。用于变身法术
    void		BreakAutoAttack();
    void		DebugReloadAll()
    {
        for(int i = 0; i < m_setData->GetAmount(); i++)
        {
            m_setData->GetObjByIndex(i)->FindType();
        }
    }

    bool		IsIntone()		{ return m_nMagicState == MAGICSTATE_INTONE; }	//return (m_tIntone.IsActive() && !m_tIntone.IsTimeOut()); }
    bool		IsInLaunch()	{ return m_nMagicState == MAGICSTATE_LAUNCH; }	//return (m_tApply.IsActive() && !m_tApply.IsTimeOut()); }
    bool		IsActive()		{ return (m_nMagicState != MAGICSTATE_NONE); }

public: // static
    static CMagicTypeData*	FindMagicType(int nType, int nLevel);
    static int 	CalcMagicPower(IRole* pAtker, IRole* pTarget, int* pAdjustAtk = 0);

protected:
    bool		AwardExpOfLife(IRole* pRole, int nLoseLife, BOOL bMagicRecruit = false);
    void		ShowMiss();
    int			Attack(int nType, IRole* pRole);		// return lose life
    int			Attack(int nType, int x, int y);		// return lose life
    int			Launch();
    bool		UpLevel(bool bSynchro = true);
    bool		CheckCondition(IMagicData* pData, OBJID idTarget, int x, int y);
    bool		CheckCrime(const vector<IRole*>* pSet);
    bool		CheckCrime(IRole* pRole);

    bool		LockTarget(bool bLock);
    //	bool		AdjustUserPos();
    //	bool		AdjustTargetPos();
    bool		AdjustPosByTrack(int nStepIndex, bool bAtkerPenetrable = false, bool bTargetPenetrable = false);

    bool		CheckProfession(DWORD dwProf, DWORD dwNeedProf);

protected: // calc magic fight
    bool		CheckAwardExpEnable(DWORD dwProf);
    void		OtherMemberAwardExp(IRole* pTarget, int nRawExp);

public: // const
    enum { HIT_BY_MAGIC, HIT_BY_WEAPON, HIT_BY_ITEM };
    bool		IsImmunity(IRole* pRole);
    int			HitByMagic();
    bool		HitByWeapon();
    int			GetDieMode();
    bool		IsKeepBow()						{ return false; }	//m_pMagic && m_pMagic->GetInt(MAGICDATA_SORT) == MAGICSORT_ATTACHSTATUS && m_pMagic->GetInt(MAGICDATA_STATUS) == STATUS_KEEPBOW; }
    int			GetPower(int nStepIndex = -1);
    //	int			GetApplyMS(int nStepIndex = -1);

public: // task
    bool		CheckType(int nType);
    bool		CheckLevel(int nType, int nLevel);
    bool		UpLevelByTask(int nType);

public: // create
    void		ResetDelay();
    OBJID		LearnMagic(int nType, int nLevel = 0, bool bSaveMagic = true);
    bool		UnlearnMagic(int nType, bool bDrop);
    bool		EquipWeaponMagic(OBJID idMagic, int nType, int nLevel);
    bool		UnequipWeaponMagic(OBJID idMagic);

protected: // process
    bool		ProcessAttack();
    bool		ProcessAtkStatus();
    bool		ProcessRecruit();
    bool		ProcessCross();
    bool		ProcessLine();
    bool		ProcessFan();
    bool		ProcessBomb();
    bool		ProcessBombLockAll();
    bool		ProcessSquare();
    bool		ProcessAttach();
    bool		ProcessDetach();
    bool		ProcessJumpAttackFaild(int x, int y);
    bool		ProcessJumpAtk();
    bool		ProcessRandomTrans();
    bool		ProcessDispatchXP();
    bool		ProcessCollideFail(int x, int y, int nDir);
    bool		ProcessCollide();
    bool		ProcessSerialCut();
    bool		ProcessCallTeamMember();
    bool		ProcessRecordTransSpell();
    bool		ProcessTransform();
    bool		ProcessAddMana();
    bool		ProcessLayTrap();
    bool		ProcessCallPet();
    bool		ProcessDecLife();
    bool		ProcessGroundSting();
    bool		ProcessReborn();
    bool		ProcessSorbSoul();
    bool		ProcessSteal();

    ////////////////////////////////////////////////////////
    // 新增幻兽魔法类型
    bool		ProcessBlastThunder();
    bool		ProcessMultiAttach();
    bool		ProcessMultiDetach();
    bool		ProcessMultiCure();
    bool		ProcessStealMoney();
    bool		ProcessKO();
    bool		ProcessEscape();
    //	bool		ProcessFlashAttack();


    typedef std::vector<IRole*>	ROLE_SET;
    enum { LOCK_NONE, LOCK_ONE, LOCK_ALL };
    bool		CollectTargetSet_Bomb(ROLE_SET& setRole, POINT& pos, int nLockType = LOCK_NONE);
    bool		CollectTargetSet_Team(ROLE_SET& setRole);

protected: // spawn id
    static OBJID	SpawnMagicID()						{ return InterlockedIncrement(&m_lockNextID); }

protected: // data
    IMagicSet*		m_setData;

protected: // intone
    IMagicData*		m_pMagic;
    OBJID			m_idTarget;
    POINT			m_pos;
    int				m_nData;
    bool			m_bAutoAttack;
    int				m_nRawDelay;		// fast delay for trans map
    int				m_nDelay;			// normal delay

    int				m_nMagicState;		// 魔法状态	-- zlong 2004-11-30

    CTimeOutMS		m_tIntone;			// 吟唱延时
    CTimeOutMS		m_tApply;			// 每次效果延时
    int				m_nApplyTimes;		// 作用效果次数
    CTimeOutMS		m_tDelay;			// 施展结束延时

    vector<OBJID>	m_setTargetLocked;	// 魔法锁定，有轨迹的目标集
    bool			m_bTargetLocked;

private: // cheat --------
    OBJID			m_cheatTargetID;
    int				m_cheatCount;

    // XP add by arhun
public:
    BOOL HaveXpMagic();	// 是否学会XP技能

protected: // ctrl
    PROCESS_ID		m_idProcess;
    IRole*			m_pOwner;
    static long		m_lockNextID;
    MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_MAGIC_H__B2689767_419D_487F_B0F9_FBB04C4619AA__INCLUDED_)
