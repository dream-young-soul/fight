
//**********************************************************
// 代码编辑器
//**********************************************************

// AINpc.h: interface for the CMonster class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AINPC_H__52682938_5D82_4101_B888_6A4A3F30EB51__INCLUDED_)
#define AFX_AINPC_H__52682938_5D82_4101_B888_6A4A3F30EB51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "define.h"
#include "gameobj.h"
#include "GameBlock.h"
#include "Timeout.h"
#include "Item.h"
#include "battlesystem.h"
#include "Myheap.h"
#include "GameMap.h"
#include "StatusMore.h"
#include "protocol.h"
#include "UserTable.h"
#include "autolink.h"
#include "maptrap.h"
#include "Role.h"
#include "Magic.h"
//////////////////////////////////////////////////////////////////////
enum {	ATKUSER_LEAVEONLY				= 0,				// 只会逃跑
        ATKUSER_PASSIVE					= 0x01,				// 被动攻击
        ATKUSER_ACTIVE					= 0x02,				// 主动攻击
        ATKUSER_RIGHTEOUS				= 0x04,				// 正义的(卫兵或玩家召唤和控制的怪物)
        ATKUSER_GUARD					= 0x08,				// 卫兵(无事回原位置)
        ATKUSER_PPKER					= 0x10,				// 追杀黑名
        ATKUSER_JUMP					= 0x20,				// 会跳
        ATKUSER_FIXED					= 0x40,				// 不会动的
        ATKUSER_FASTBACK				= 0x0080,				// 速归
        ATKUSER_LOCKUSER				= 0x0100,				// 锁定攻击指定玩家，玩家离开自动消失
        ATKUSER_LOCKONE					= 0x0200,				// 锁定攻击首先攻击自己的玩家
        ATKUSER_ADDLIFE					= 0x0400,				// 自动加血
        ATKUSER_EVIL_KILLER				= 0x0800,				// 白名杀手
        ATKUSER_WING					= 0x1000,				// 飞行状态
        ATKUSER_NEUTRAL					= 0x2000,				// 中立
        ATKUSER_ROAR					= 0x4000,				// 出生时全地图怒吼
        ATKUSER_NOESCAPE				= 0x8000,				// 不会逃跑
        ATKUSER_EQUALITY				= 0x00010000,				// 不藐视
     };

//////////////////////////////////////////////////////////////////////
const int	MONSTERDROPITEM_RANGE		= 2;		// 2*2+1的4次方=625次搜索
const int	NPCDIEDELAY_SECS			= 10;		// MONSTER死亡多少秒后删除
const int	SIMPLEMAGIC_RECRUIT			= 1005;
const OBJID	TYPE_TASK_MONSTER			= 3000;
const bool	DEL_NOW						= true;


const int	DEFAULT_EUDEMON_POTENTIAL	= 50;		// 幻兽的缺省潜力值
const int	MAX_EUDEMON_POTENTIAL		= 100;		// 幻兽最大潜能值
const int	ADD_POTENTIAL_KILLNUM		= 100;		// 每杀死100个目标增加潜力值
const int	ADD_POTENTIAL_PER_KILLNUM	= 1;
const int	ADD_POTENTIAL_LEVUP			= 5;

const int	ADD_RELATIONSHIP_KILLNUM	= 100;		// 每杀死100个目标增加关系值
const int	ADD_RELATIONSHIP_PER_TIME	= 1;		// 每次增减的关系值

//////////////////////////////////////////////////////////////////////
// 幻兽攻击类型
enum EATK_MODE
{
    EATK_MODE_NONE	= 0,	// 无类型
    EATK_MODE_ATK,			// 攻击
    EATK_MODE_ATKDEF,		// 攻防
    EATK_MODE_DEF,			// 防御

    EATK_MODE_LIMIT,
};


//////////////////////////////////////////////////////////////////////
enum PETDATA
{
    PETDATA_OWNERID = 1,
    PETDATA_OWNERTYPE,
    PETDATA_GENID,
    PETDATA_TYPE,
    PETDATA_NAME,
    PETDATA_LIFE,
    PETDATA_MANA,
    PETDATA_MAPID,
    PETDATA_RECORD_X,
    PETDATA_RECORD_Y,
    PETDATA_DATA,						// 可用于神兽记录NPCID
};

//////////////////////////////////////////////////////////////////////
enum MONSTER_NAME_TYPE
{
    NAME_GREEN,			// 绿名怪
    NAME_WHITE,			// 白名怪
    NAME_RED,			// 红名怪
    NAME_BLACK,			// 黑名怪
};

//////////////////////////////////////////////////////////////////////
enum MONSTER_MASKDATA
{
    MASK_NONE				= 0,
    MASK_DISABLE_SORB_SOUL	= 1,	// 禁止吸魂
    MASK_DISABLE_STEAL_MONEY = 2,	// 禁止偷窃金钱
};

//////////////////////////////////////////////////////////////////////
char	szPetTable[];
typedef		CGameData<PETDATA, szPetTable, szID>	CPetData;

//////////////////////////////////////////////////////////////////////
class CUser;
class CMonster : public CGameObj, public CRole
{
protected:
    CMonster();
    virtual ~CMonster();
public:
    static CMonster*	CreateNew() 	{ return new CMonster; }

    bool	Create(PROCESS_ID idProcess, CNpcType* pType, const struct ST_CREATENEWNPC* pInfo, LPCTSTR pszName = NULL);
    bool	Create(PROCESS_ID idProcess, CNpcType* pType, const struct ST_CREATENEWNPC* pInfo, CUser* pUser, OBJID idItem = ID_NONE);
    void	ReleaseByOwner()					{ delete this; }

    virtual		bool		FindAroundObj	(OBJID idObjType, OBJID idObj, void** pp)		{ return GetMap()->QueryObj(GetPosX(), GetPosY(), idObjType, idObj, pp); }
    virtual		IRole*		FindAroundRole	(OBJID id)		{ return GetMap()->QueryRole(GetPosX(), GetPosY(), id); }

    virtual CUser*		QueryOwnerUser		()				{ if (IsCallPet() || IsEudemon()) { return m_pOwner; } return NULL; }	//@@@ 暂略 if (m_pData && m_pData->GetInt(PETDATA_OWNERTYPE) == OWNER_USER) return m_pData->GetInt(PETDATA_OWNERID);
protected:
    virtual bool		QueryRole(void** ppv)	{ return *ppv = (IRole*)this, true; }
    virtual bool		QueryObj(OBJID idObjType, void** ppv)			{ if (idObjType == OBJ_MONSTER) { return *ppv = this, true; } return *ppv = NULL, false; }
public:
    virtual IMapThing*	QueryMapThing()			{ return (IMapThing*)this; }
    IRole*				QueryRole()				{ return (IRole*)this; }

public: // IRole
    //	virtual OBJID	GetID();
    virtual LPCTSTR	GetName();	//							{ return m_pType->GetStr(NPCTYPEDATA_NAME); }
    //	virtual LPCTSTR	GetMate()							{ return NOMATE_NAME; }
    virtual LPCTSTR	GetTitle()							{ return TITILE_NONE; }

    virtual DWORD 	GetHair				()				{ return 0; }
    virtual int 	GetPosX				()				{ return m_nPosX; }
    virtual int 	GetPosY				()				{ return m_nPosY; }
    virtual int 	GetDir				()				{ return m_nDir; }
    //	virtual	BOOL	CheckStatus			(int nStatus);
    virtual I64		GetEffect			()				{ return m_i64Effect; }
    virtual int 	GetPose				()				{ return m_nPose; }
    // 幻兽不使用monstertype表中的等级属性字段 -- zlong 2004-02-24
    virtual DWORD	GetLev				()				{ if (IsEudemon() && m_pEudemonItem) { return m_pEudemonItem->GetEudemonLevel(); } return m_pType->GetInt(NPCTYPEDATA_LEVEL); }

    virtual	DWORD	GetLife				();	//			{ return m_nCurrLife; }
    virtual DWORD	GetMaxLife			();	//				{ return (m_AddInfo.nMaxLifeAdd + m_pType->GetInt(NPCTYPEDATA_LIFE)); }
    //	virtual DWORD	GetMana				()				{ return m_nCurrMana; }
    //	virtual DWORD	GetMaxMana			()				{ return m_pType->GetInt(NPCTYPEDATA_MANA); }
    virtual DWORD	GetMinAtk			();
    virtual DWORD	GetMaxAtk			();
    virtual DWORD	GetMgcMinAtk			();
    virtual DWORD	GetMgcMaxAtk			();
    virtual DWORD	GetAttack			()				{ return (GetMaxAtk() + GetMinAtk()) / 2; }	//{ return (m_pType->GetInt(NPCTYPEDATA_ATTACKMIN)+m_pType->GetInt(NPCTYPEDATA_ATTACKMAX))/2; }
    virtual DWORD	GetAtkHitRate		()				{ return (m_pType->GetInt(NPCTYPEDATA_ATK_HITRATE)); }
    virtual DWORD	GetDef				();
    virtual DWORD	GetDefence			()				{ return m_pType->GetInt(NPCTYPEDATA_DEFENCE); }
    virtual DWORD	GetDefence2			()				{ return m_pType->GetInt(NPCTYPEDATA_DEFENCE2); }
    virtual DWORD	GetDex				();
    virtual DWORD	GetDexterity		()				{ return m_pType->GetInt(NPCTYPEDATA_DEXTERITY); }
    virtual DWORD	GetDdg				();
    virtual DWORD	GetDodge			()				{ return m_pType->GetInt(NPCTYPEDATA_DODGE); }
    //	virtual DWORD	GetMagicAtk			()				{ return 0; }
    virtual DWORD	GetMagicDef			();
    virtual	DWORD	GetInterAtkRate		();
    virtual	DWORD	GetIntervalAtkRate	()				{ return m_pType->GetInt(NPCTYPEDATA_ATKSPEED); }

    virtual DWORD	GetPotential		()				{ return m_nPotential; }

    virtual int		AdjustExp(IRole* pTarget, int nRawExp, bool bNewbieBonusMsg = false);
    virtual void	AwardBattleExp		(int nExp, bool bGemEffect = true, bool bIncludeOwner = true);
    virtual int 	AdjustHitRate		(int nHitRate)	{ /*IStatus* pStatus = QueryStatus(STATUS_HITRATE); if (pStatus) return ::CutTrail(0, AdjustData(nHitRate, pStatus->GetPower()));*/ return nHitRate; }
    virtual int 	AdjustAttack		(int nAtk);
    virtual int 	AdjustDefence		(int nDef);
    virtual int 	AdjustMagicAtk		(int nAtk)		{ IStatus* pStatus = QueryStatus(STATUS_SUPER_MATK); if (pStatus) { return ::CutTrail(0, AdjustData(nAtk, pStatus->GetPower())); } return nAtk; }
    virtual int 	AdjustMagicDef		(int nDef);
    virtual int 	AdjustBowDefence	(int nDef)		{ /*IStatus* pStatus = QueryStatus(STATUS_BOWDEFENCE); if (pStatus) return ::CutTrail(0, AdjustData(nDef, pStatus->GetPower()));*/ return nDef; }
    virtual int 	AdjustWeaponDamage	(int nDamage);
    virtual int 	AdjustMagicDamage	(int nDamage);
    virtual void	SetFightPause		(int nInterval)			{ m_nFightPause = nInterval; }

    int				AdjustFrenzy2Dmg	(int nDamage);

    virtual DWORD	GetLookFace()						{ return m_pType->GetInt(NPCTYPEDATA_LOOKFACE); }
    virtual char	GetLength			()				{ return m_pType->GetInt(NPCTYPEDATA_LENGTH); }
    virtual char	GetFat				()				{ return m_pType->GetInt(NPCTYPEDATA_FAT); }

    virtual int		GetHelmetTypeID		()				{ return m_pType->GetInt(NPCTYPEDATA_HELMET_TYPE); }
    virtual int		GetArmorTypeID		()				{ return m_pType->GetInt(NPCTYPEDATA_ARMOR_TYPE); }
    virtual int 	GetWeaponRTypeID	()				{ return m_pType->GetInt(NPCTYPEDATA_WEAPONR_TYPE); }
    virtual int 	GetWeaponLTypeID	()				{ return m_pType->GetInt(NPCTYPEDATA_WEAPONL_TYPE); }

    virtual void	BroadcastRoomMsg(CNetMsg* pMsg, bool bSendSelf = false);
    virtual void	BroadcastRoomMsg(LPCTSTR szMsg, bool bSendSelf = false);
    virtual void	BroadcastMapMsg(CNetMsg* pMsg, bool bSendSelf = false);
    virtual void	BroadcastMapMsg(LPCTSTR szMsg, bool bSendSelf = false);
    virtual bool	SendMsg(CNetMsg* pMsg);
    virtual bool	SendSysMsg(const char* fmt, ...);
    virtual void	OnTimer(time_t tCurr);

    virtual DWORD	GetSynID			()				{ return m_pData && m_pData->GetInt(PETDATA_OWNERTYPE) == OWNER_SYN ? m_pData->GetInt(PETDATA_OWNERID) : ID_NONE; }
    virtual DWORD	GetSynRank			()				{ return RANK_NONE; }
    virtual DWORD	GetSynRankShow		()				{ return RANK_NONE; }

    virtual bool	IsSimpleMagicAtk	()				{ return m_pType->GetInt(NPCTYPEDATA_MAGIC_TYPE) != ID_NONE && m_pType->GetInt(NPCTYPEDATA_MAGIC_TYPE) != SIMPLEMAGIC_RECRUIT && m_pType->GetInt(NPCTYPEDATA_MAGIC_HITRATE) != 0; }
    virtual int 	GetSimpleMagicHitRate()				{ return m_pType->GetInt(NPCTYPEDATA_MAGIC_HITRATE); }
    int 			GetRecruitMagicPercent()			{ return m_pType->GetInt(NPCTYPEDATA_MAGIC_HITRATE); }

    virtual CMagic*	QueryMagic()						{ return m_pMagic; }
    virtual DWORD	GetProfession()						{ return m_pType->GetInt(NPCTYPEDATA_PROFESSION); }

public: // map
    bool	EnterMapGroup();
    bool	LeaveMapGroup();
    //	virtual bool	MoveToward(int nDir);
    //	virtual void	TransPos(int nPosX, int nPosY);		// 传送，完全的新坐标
    //	virtual void	JumpPos(int nPosX, int nPosY);		// 跳，半屏以内
    //	virtual bool	SyncTrackTo(int nPosX, int nPosY, int nDir, DWORD dwAction);	// 魔法轨迹同步
    virtual bool	SendSelfToBlock();
    bool	SendLeaveFromBlock();

    bool	KickBack();

public: // foot print
    void	GetFootPrint	(int& nPosX, int& nPosY);

protected:
    DWORD	m_tFootPrint;
    POINT	m_posFootPrint;

public: // get
    virtual OBJID	GetID()			{ return m_idNpc; }
    virtual int		GetObjType()	{ return CGameObj::GetObjType(); }
    //virtual OBJID	GetMapID()		{ return m_idMap; }
    virtual void	SendShow(IRole* pRole);

public: // get attrib  ------------------------------------
    virtual int		GetType			()		{ return m_pType->GetID(); }
    //	virtual int		GetSex			()		{ return 0; }
    virtual bool	IsAlive			();
    virtual bool	IsTalkEnable	()		{ return m_dwStatus == STATUS_NORMAL; }
    virtual bool	IsMonster		()		{ return IsMonsterID(GetID()) && !IsGuard() && !IsPkKiller(); }
    virtual bool	IsDelThis		()		{ return IsDeleted() && m_tDie.IsTimeOut(); }		// ASSERT(!IsAlive());
    virtual bool	IsVirtuous()			{ return IsGuard() || IsPkKiller(); }
    virtual bool	IsEvil()				{ return ((m_pType->GetInt(NPCTYPEDATA_ATKUSER)&ATKUSER_RIGHTEOUS) == 0 || QueryStatus(STATUS_CRIME)); }
    virtual bool	IsBowman()				{ return CItem::IsBowSort(GetWeaponRTypeID()); }
    virtual bool	IsShieldEquip()			{ return CItem::IsShield(GetWeaponLTypeID()); }
    virtual bool	IsWing()				{ return (m_pType->GetInt(NPCTYPEDATA_ATKUSER)&ATKUSER_WING) != 0; }
    virtual bool	IsSendBlockInfo()		{ return (!IsCallPet() && !IsEudemon()); }

    static	bool	IsMonsterID(OBJID id)	{ return id >= MONSTERID_FIRST && id <= MONSTERID_LAST; }
    bool		IsGuard()			{ return (m_pType->GetInt(NPCTYPEDATA_ATKUSER)&ATKUSER_GUARD) != 0; }
    bool		IsPkKiller()		{ return (m_pType->GetInt(NPCTYPEDATA_ATKUSER)&ATKUSER_PPKER) != 0; }
    bool		IsDeleted()			{ return m_tDie.IsActive(); }
    void		DelMonster(bool bNow = false);			//? call this mast !IsDeleted()
    bool		IsSynPet()				{ return m_idNpc >= PETID_FIRST && m_idNpc <= PETID_LAST; }
    bool		IsSynMonster()			{ return (m_idNpc >= PETID_FIRST && m_idNpc <= PETID_LAST) && !m_pData; }
    bool		IsCallPet()				{ return m_idNpc >= CALLPETID_FIRST && m_idNpc <= CALLPETID_LAST; }
    OBJID		GetMasterID();
    int			GetData()			{ CHECKF(m_pData); return m_pData->GetInt(PETDATA_DATA); }
    OBJID		GetGenID()			{ return m_idGen; }
    bool		IsTaskMonster()		{ return this->GetType() >= TYPE_TASK_MONSTER; }
    bool		IsMagicAtk()			{ return m_pType->GetInt(NPCTYPEDATA_MAGIC_TYPE) != ID_NONE && m_pType->GetInt(NPCTYPEDATA_MAGIC_HITRATE) == 0; }
    OBJID		GetMagicType()			{ return m_pType->GetInt(NPCTYPEDATA_MAGIC_TYPE); }
    bool		IsMapRoar()				{ return (m_pType->GetInt(NPCTYPEDATA_ATKUSER)&ATKUSER_ROAR) != 0; }
    bool		IsEquality()			{ return (m_pType->GetInt(NPCTYPEDATA_ATKUSER)&ATKUSER_EQUALITY) != 0; }
    bool		IsEudemon()				{ return (m_idNpc >= EUDEMON_ID_FIRST && m_idNpc <= EUDEMON_ID_LAST); }
    static int	GetNameType(int nAtkerLev, int nMonsterLev);

public:	// modify attrib ------------------------------
    virtual void	SetDir(int nDir)			{ m_nDir = nDir % 8; }
    virtual void	SetPose(int nPose)			{ m_nPose = nPose;}
    //	virtual void	SetEmotion(int nEmotion)	{ }//m_nEmotin = nEmotion; }

    virtual	void	SetStatus(int nStatus, bool bSynchro = true);
    virtual	void	ClsStatus(int nStatus, bool bSynchro = true);

    virtual bool	AddAttrib(int idxAttr, __int64 i64Data, int nSynchro);
    virtual	void	ProcessOnMove(int nMoveMode);
    virtual void	ProcessAfterMove();
public: // money --------------------------------
    //	virtual bool	SpendMoney(DWORD nMoney, bool bSynchro)		{ return true; }
    //	virtual bool	GainMoney(DWORD nMoney, bool bSynchro)		{ return true; }

public: // fight ------------------------------
    virtual void	ClrAttackTarget		()									{ SetAttackTarget(NULL); }
    virtual bool	SetAttackTarget(IRole* pTarget);
    virtual bool	AutoSkillAttack(IRole* pTarget);
    virtual bool	AdditionMagic(int nLifeLost, int nDamage);
    virtual int		Attack(IRole* pTarget);		// return : lose life
    virtual bool	BeAttack(bool bMagic, IRole* pTarget, int nPower, bool bReflectEnable = true);
    virtual bool	MagicAttack(int nType, OBJID idTarget, int x, int y)	{ if (m_pMagic) { return m_pMagic->MagicAttack(nType, idTarget, x, y); } return false; }
    virtual void	Kill(IRole* pTarget, DWORD dwDieWay);
    virtual void	BeKill(IRole* pRole = NULL);	//pTarget = NULL);						// 可能删除NPC对象
    virtual bool	IsAtkable(IRole* pTarget, bool bSendHint = false);
    virtual bool	IsBeAtkable();
    virtual int		GetAttackRange(int nTargetSizeAdd)		{ return m_pType->GetInt(NPCTYPEDATA_ATKRANGE) + (GetSizeAdd() + nTargetSizeAdd + 1) / 2; }
    virtual int		GetSizeAdd()							{ return m_pType->GetInt(NPCTYPEDATA_SIZEADD); }
    virtual	int		GetLuck				()					{ return 0; }
    virtual void	SendDamageMsg(OBJID idTarget, int nDamage);
    void	ProcessBomb();
    int 	GetDieMode();
    bool	IsInsteadMagic();
    bool	IsImmunity			(IRole* pRole);

public: // interactive
    //	virtual bool	AddItem		(CItem* pItem)				{ CHECKF(pItem); pItem->ReleaseByOwner(); return true; }
    //	virtual bool	IsItemFull	()							{ return false; }
    virtual bool	Freeze(int nSecs);							// return false: 解穴不成功

    //	virtual CGameMap*		GetMap			()			{ ASSERT(m_pMap); return m_pMap; }

public: // action
    bool	ProcessAction	(CUser* pUser, OBJID idAction, LPCTSTR pszAccept = NULL);

public: // status
public: // user status --------------------------------------------------------------------------------
    //	IStatus*	QueryStatus(OBJID idType)		{ CHECKF(m_setStatus); return m_setStatus->GetObj(idType); }
    //	IStatusSet*	QueryStatusSet()				{ CHECKF(m_setStatus); return m_setStatus; }
    void		ClearAllStatus	();
protected: // data
    //	IStatusSet*	m_setStatus;

public: // common -------------------------------------
    //	static int	AdjustData			(int nData, int nAdjust, int nMaxData=0);

public: // pet ----------------------------------------------------
    bool	Create(PROCESS_ID idProcess, IRecordset* pRes);
    void	SaveInfo();			//??? save all when close server!!!

public: // call pet ---------------------------------
    CAutoLink<CMonster>&	QueryLink()			{ return m_link; }
protected:
    CAutoLink<CUser>	m_pOwner;
    CAutoLink<CMonster>	m_link;
    CMagicTypeData*		m_pMagicType;			// may be null
    CTimeOut			m_tMagic;

    CMagic*				m_pMagic;				// may be null

public:
    DWORD	GetDropMoneyAvg();
    bool	DropMoney	(DWORD dwMoney, OBJID idOwner);
    // modifyed by zlong 2003-11-19
    // 增加设置magic2,magic3的默认参数
    bool	DropItem	(OBJID idItemType, OBJID idOwner, int nMagic2 = 0, int nMagic3 = 0, int nUserLuck = 0, bool bIsAllowDropUnident = false);

protected:
    CPetData*	m_pData;		// NULL: is not pet

protected:
    bool	DropMedicine	(OBJID idOwner);
    bool	DropEquipment	(DWORD dwValue, OBJID idOwner, DWORD dwQuality = 0);

protected: // attrib
    OBJID		m_idNpc;
    //	int			m_nPosX;
    //	int			m_nPosY;
    //	int			m_nDir;
    DWORD		m_dwStatus;
    I64			m_i64Effect;
    int			m_nPose;
    //	CGameMap*	m_pMap;

    int			m_nCurrLife;
    int			m_nCurrMana;

    int			m_nPotential;		// 潜力值
    DWORD		m_dwMaskData;
public:
    DWORD	GetMaskData()		{ return m_dwMaskData; }
    void	SetMaskData(DWORD dwMask)		{ m_dwMaskData = dwMask; }
    void	AddMaskData(DWORD dwMask)		{ m_dwMaskData |= dwMask; }
    void	ClsMaskData(DWORD dwMask)		{ m_dwMaskData &= ~dwMask; }

public:	// eudemon -------------------------------
    bool DropTrap(TRAP_INFO& trapInfo, UINT ulLifePeriod);
    // 此函数只需要在创建幻兽的时候调用一次即可，由于功能已经改变，以后最好换个函数名
    void	SetEudemonAddi(CItem*	pEudemon);
    DWORD	GetFidelity()		{ IF_NOT (IsEudemon() && m_pEudemonItem) return 0; return m_pEudemonItem->GetInt(ITEMDATA_FIDELITY); }

    OBJID	GetDivineID()		{ IF_NOT (IsEudemon() && m_pEudemonItem) return 0; return m_pEudemonItem->GetDivineID(); }
    UCHAR	GetRelationShip(OBJID idDivine)					{ IF_NOT (IsEudemon() && m_pEudemonItem) return 0; return m_pEudemonItem->GetRelationShip(idDivine); }
    bool	AddRelationShip(OBJID idDivine, int nValue)		{ IF_NOT (IsEudemon() && m_pEudemonItem) return 0; return m_pEudemonItem->AddRelationShip(idDivine, nValue); }

    int		GetAtkMode()		{ ASSERT(IsEudemon()); return m_nAtkMode; }
    bool	ChgAtkMode(int nAtkMode);

protected:
    DWORD	GetEudemonLev()		{ IF_NOT (IsEudemon() && m_pEudemonItem) return 0; return m_pEudemonItem->GetEudemonLevel(); }

protected:
    // 幻兽各属性根据等级、成长计算出来的差值，需要加成到属性中的
    /*
    	struct {
    		int			nMaxAtkAdd;			// 物理攻击
    		int			nMinAtkAdd;
    		int			nMaxMAtkAdd;		// 魔法攻击
    		int			nMinMAtkAdd;
    		int			nDefAdd;			// 物理防御
    		int			nMDefAdd;			// 魔法防御
    		int			nMaxLifeAdd;		// 最大生命

    		int			nAtkSpeedAdd;		// 攻击速度
    		int			nHitRateAdd;		// 命中率
    		int			nDodgeAdd;			// 躲避率

    //		int			nEudemonLev;		// 等级
    	} m_AddInfo;
    */

    CItem*		m_pEudemonItem;		// refrence

protected: // fight
    CBattleSystem*		m_pBattleSystem;
    CTimeOutMS			m_tFight;
    int					m_nFightPause;						// 僵硬状态时，一次性增加的间隔毫秒数

    CTimeOut			m_tSorbReflect;
    int					m_nTotalDamage;

    int					m_nKillNum;							// 杀死的目标数
    int					m_nKillNum4Potential;				// 杀死目标数量——为了增加潜力值而做的统计
    int					m_nKillNum4RelationShip;			// 杀死目标数量——为了增加关系值而做的统计

    int					m_nAtkMode;		// 攻击类型

protected: // npctype
    CNpcType*	m_pType;		// refrence

protected: // application
    CTimeOut	m_tDie;
    auto_long	m_bLeaveMap;
    bool		m_bDecRole;

protected: // ctrl
    CTimeOut	m_tAddLife;
    CTimeOut	m_tIncLife;
    OBJID		m_idGen;
    //	PROCESS_ID	m_idProcess;

    MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_AINPC_H__52682938_5D82_4101_B888_6A4A3F30EB51__INCLUDED_)
