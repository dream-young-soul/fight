
//**********************************************************
// 代码编辑器
//**********************************************************

// MapTrap.h: interface for the CMapTrap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPTRAP_H__1A117AC2_A313_4179_8F93_3D308C29F0EB__INCLUDED_)
#define AFX_MAPTRAP_H__1A117AC2_A313_4179_8F93_3D308C29F0EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include "common.h"
#include "gameObj.h"
#include "define.h"
#include "Role.h"
#include "mycom.h"
#include "i_mydb.h"
#include "timeout.h"
#include "GameData.h"
#include "T_SingleMap2.h"
#include "gameMap.h"
#include "UserTable.h"
#include "autolink.h"

/*enum TRAPTYPEDATA
{
	TRAPTYPEDATA_SORT=1,
	TRAPTYPEDATA_LOOK,
	TRAPTYPEDATA_ACTION,
	TRAPTYPEDATA_LEVEL,
	TRAPTYPEDATA_ATK_MAX,
	TRAPTYPEDATA_ATK_MIN,
	TRAPTYPEDATA_DEX,
	TRAPTYPEDATA_SPEED,
	TRAPTYPEDATA_TIMES,
	TRAPTYPEDATA_MAGICTYPE,
	TRAPTYPEDATA_MAGICHITRATE,
	TRAPTYPEDATA_SIZE,
	TRAPTYPEDATA_ATKMODE,
};*/
//////////////////////////////////////////////////////////////////////
struct TRAP_INFO
{
    OBJID		id;
    USHORT		usType;
    USHORT		usLook;
    OBJID		idOwner;
    OBJID		idMap;
    USHORT		usPosX;
    USHORT		usPosY;
};

//////////////////////////////////////////////////////////////////////
enum TRAPDATA
{
    TRAPDATA_ID_ = 0,
    TRAPDATA_TYPE = 1,
    TRAPDATA_LOOK,					// with new dir, look%10 is dir
    TRAPDATA_OWNERID,
    TRAPDATA_MAPID,
    TRAPDATA_POS_X,
    TRAPDATA_POS_Y,
    TRAPDATA_DATA,
};
//typedef	CGameData<TRAPDATA>		CTrapData;

enum TRAP_SORT
{
    TRAPSORT_SYSTRAP	= 1,					// 系统的陷阱，多次有效
    TRAPSORT_TRAPSORT_	= 10,				// 陷阱类
    // ---------------------

    TRAPSORT_FIREWALL	= 10,				// 火墙, 可由系统放置
};

enum ATK_MODE
{
    ATKMODE_USER		= 1,
    ATKMODE_MONSTER		= 2,
    ATKMODE_NOATTACK	= 4,				// 不攻击对象，只触发ACTION
};

class CMapTrap : public CGameObj, public CRole
{
    COM_STYLE(CMapTrap)
protected:
    CMapTrap();
    virtual ~CMapTrap();
    bool	Init();

public:
    static CMapTrap* CreateNew(PROCESS_ID idProcess, TRAP_INFO* pInfo, CUser* pUser = NULL);			// 玩家放火墙类
    static CMapTrap* CreateNew(PROCESS_ID idProcess, IRecord* pRes);			// 系统类
    bool		ChangeType(int nNewType);
    bool		ChangeLook(int nNewLook);

    IRole*			QueryRole()									{ return (IRole*)this; }
    virtual		bool		FindAroundObj	(OBJID idObjType, OBJID idObj, void** pp)
    { return GetMap()->QueryObj(GetPosX(), GetPosY(), idObjType, idObj, pp); }
public: // IRole ================================================================================
    virtual OBJID	GetID()										{ return m_info.id; }
    virtual int 	GetObjType()								{ return CGameObj::GetObjType(); }
    virtual int		GetPosX()									{ return m_info.usPosX; }
    virtual int		GetPosY()									{ return m_info.usPosY; }
    virtual void	SendShow(IRole* pRole);							// 向玩家发送物件外形数据

public: // get attrib
    virtual LPCTSTR	GetName				()						{ return "trap"; }

    virtual DWORD	GetLookFace			()						{ return m_info.usLook; }
    virtual DWORD	GetHair				()						{ return 0; }
    virtual int 	GetDir				()						{ return GetLookFace() % MAX_DIRSIZE; }
    virtual I64		GetEffect			()						{ return 0; }
    virtual int 	GetPose				()						{ return 0; }
    virtual DWORD	GetLev				()						{ return m_pType->GetInt(TRAPTYPEDATA_LEVEL); }

    virtual DWORD	GetMinAtk			()						{ return m_pType->GetInt(TRAPTYPEDATA_ATK_MIN); }
    virtual DWORD	GetMaxAtk			()						{ return m_pType->GetInt(TRAPTYPEDATA_ATK_MAX); }
    virtual DWORD	GetMgcMinAtk			()						{ return m_pType->GetInt(TRAPTYPEDATA_ATK_MIN); }
    virtual DWORD	GetMgcMaxAtk			()						{ return m_pType->GetInt(TRAPTYPEDATA_ATK_MAX); }
    virtual DWORD	GetAttack			()						{ return GetMinAtk() + ::RandGet(GetMaxAtk() - GetMinAtk()); }
    virtual DWORD	GetAtkHitRate		()						{ return 100; }

    virtual DWORD	GetDex				()						{ return m_pType->GetInt(TRAPTYPEDATA_DEX); }
    virtual DWORD	GetDexterity		()						{ return m_pType->GetInt(TRAPTYPEDATA_DEX); }

    virtual	DWORD	GetInterAtkRate		()						{ return m_pType->GetInt(TRAPTYPEDATA_SPEED); }
    virtual DWORD	GetIntervalAtkRate	()						{ return m_pType->GetInt(TRAPTYPEDATA_SPEED); }

    //	virtual CGameMap*	GetMap			()						{ CHECKF(m_pMap); return m_pMap; }

    virtual bool	IsSimpleMagicAtk	()						{ return m_pType->GetInt(TRAPTYPEDATA_MAGICTYPE) != 0; }
    virtual int 	GetSimpleMagicHitRate()						{ return m_pType->GetInt(TRAPTYPEDATA_MAGICHITRATE); }

public: // const
    virtual bool	IsAlive				()						{ return !m_bDie; }
    virtual bool	IsDelThis			()						{ return m_bDie; }

    // return true: ppv返回对象指针
    virtual bool	QueryObj(OBJID idObjType, void** ppv)		{ if (idObjType == OBJ_TRAP) { return *ppv = this, true; } return *ppv = NULL, false; }
    virtual bool	QueryRole(void** ppv)						{ return *ppv = QueryRole(), true; }
    virtual CUser*	QueryOwnerUser		()						{ if (m_pUser) { return m_pUser; } return NULL; }

public:	// modify attrib ------------------------------
    virtual void	SetDir				(int nDir)							{}
    virtual void	SetPose				(int nPose)							{}
    virtual	void	SetStatus			(int nStatus, bool bSynchro = true)	{}
    virtual	void	ClsStatus			(int nStatus, bool bSynchro = true)	{}
    virtual bool	AddAttrib			(int idxAttr, __int64 i64Data, int bSynchro, DWORD* pdwDirty = NULL)	{ return false; }
    virtual void	ProcessOnMove		(int nMoveMode)						{ ASSERT(!"move"); }

    virtual	IStatus*	QueryStatus(OBJID idType)							{ return NULL; }
    //	virtual	IStatusSet*	QueryStatusSet()									{ return m_setStatus; }

    virtual int 	AdjustDodge			(int nDodge)						{ return nDodge; }

    virtual void	SaveInfo	();

public: // system
    virtual void	BroadcastRoomMsg(CNetMsg* pMsg, bool bSendSelf = false);
    virtual void	BroadcastRoomMsg(LPCTSTR szMsg, bool bSendSelf = false);
    virtual void	OnTimer(time_t tCurr);

public: // map
    virtual bool	MoveToward(int nDir)									{ return 0; }
    virtual void	TransPos(int nPosX, int nPosY)							{ ASSERT(!"error"); }
    virtual void	JumpPos(int nPosX, int nPosY)							{ ASSERT(!"error"); }
    virtual bool	SyncTrackTo(int nPosX, int nPosY, int nDir, DWORD dwAction)		{ return 0; }
    virtual void	GetFootPrint	(int& nPosX, int& nPosY)				{ nPosX = GetPosX(); nPosY = GetPosY(); }

public: // fight ------------------------------
    virtual void	ClrAttackTarget		()									{}
    virtual bool	SetAttackTarget(IRole* pTarget)							{ return true; }
    virtual bool	AutoSkillAttack(IRole* pTarget)							{ return false; }
    virtual bool	AdditionMagic(int nLifeLost, int nDamage)				{ return false; }
    virtual int		Attack(IRole* pTarget);
    virtual bool	BeAttack(bool bMagic, IRole* pTarget, int nPower, bool bReflectEnable = true)			{ ASSERT(!"error"); return 0; }
    virtual bool	MagicAttack(int nType, OBJID idTarget, int x, int y)	{ ASSERT(!"MagicAttack"); return 0; }
    virtual void	Kill(IRole* pTarget, DWORD dwDieWay);
    virtual void	BeKill(IRole* pTarget = NULL)							{ ASSERT(!"error"); }
    virtual bool	IsAtkable(IRole* pTarget, bool bSendHint = false);
    virtual bool	IsBeAtkable()											{ return false; }
    virtual int		GetAttackRange(int nTargetSizeAdd)						{ return 999; }
    virtual int		GetSizeAdd()											{ return 0; }
    virtual	int		GetLuck()												{ return 0; }
    virtual void	SendDamageMsg(OBJID idTarget, int nDamage);
    // IRole ================================================================================

public: // const
    bool			IsDeleted()							{ return m_bDie; }
    bool			IsTrapSort()						{ return m_pType->GetInt(TRAPTYPEDATA_SORT) < TRAPSORT_TRAPSORT_; }
    bool			IsAutoSort()						{ return !IsTrapSort(); }
    static bool		IsDynaTrapID(OBJID id)				{ return id >= MAGICTRAPID_FIRST && id <= MAGICTRAPID_LAST; }
    int				GetType()							{ return m_info.usType; }

public: // application
    void SetLifePeriod(UINT ulLifePeriod);
    bool	DelTrap();					// call this mast !IsDeleted()
    void	TrapAttack(IRole* pTarget);
protected:
    void	MagicAttack();

protected: // process user magic
    void	ProcessFireWall();

protected:
    //	PROCESS_ID		m_idProcess;
    TRAP_INFO		m_info;
    CTimeOutMS		m_tFight;
    CTimeOut		m_tLifePeriod;//生命存在的时间
    bool			m_bDie;
    int				m_nTimes;

    CAutoLink<CUser>	m_pUser;				// auto reset to NULL by CUser obj
    //	IStatusSet*		m_setStatus;
    //	CGameMap*		m_pMap;
    CTrapTypeData*	m_pType;
    CBattleSystem*	m_pBattleSystem;
    //	CTrapData*		m_pData;			//? may be NULL

    MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_MAPTRAP_H__1A117AC2_A313_4179_8F93_3D308C29F0EB__INCLUDED_)
