
//**********************************************************
// 代码编辑器
//**********************************************************

// Agent.h: interface for the CAgent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AGENT_H__71627ADA_0012_4C5F_ACD6_21928AA84DC5__INCLUDED_)
#define AFX_AGENT_H__71627ADA_0012_4C5F_ACD6_21928AA84DC5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "define.h"
#include "I_MapData.h"
#include "I_Role.h"
#include "mycom.h"
#include "autoptr.h"
#include "timeout.h"
#include "GameObjSet.h"

#include "I_AiCenter.h"
#include "ItemPack.h"
#include "Team.h"
#include "Body.h"
#include "Sense.h"
#include "RoleMove.h"
#include "RoleFight.h"

//////////////////////////////////////////////////////////////////////
const int	AGENT_VIEW_RANGE					= CELLS_PER_VIEW;
//////////////////////////////////////////////////////////////////////
//是否自动分配点数
enum {_NOT_AUTOALLOT = 0, _AUTOALLOT = 1};

struct	UserInfoStruct
{
    OBJID			id;

    NAMESTR			szName;
    NAMESTR			szMate;

    DWORD			dwLookface;		//int(4) unsigned
    DWORD			dwHair;
    DWORD			dwMoney;			//int(4) unsigned
    UCHAR			ucLevel;			//tinyint(1) unsigned
    int				nExp;			//int(4) ?? 为什么不用unsigned?

    int				nPotential;
    USHORT			usForce;		//smallint(2) unsigned
    USHORT			usConstitution;	//smallint(2) unsigned			体质
    USHORT			usDexterity;	//
    USHORT			usSpeed;		//smallint(2) unsigned
    USHORT			usHealth;		//smallint(2) unsigned
    USHORT			usSoul;			//smallint(2) unsigned

    USHORT			usAdditional_point; //smallint(2) unsigned
    UCHAR			ucAuto_allot;		//tinyint(1) unsigned
    USHORT			usLife;
    USHORT			usMaxLife;
    USHORT			usMana;			//smallint(2) unsigned
    UCHAR			ucProfession;	//tinyint(2) unsigned
    //	int				nDeed;			//int(4)
    SHORT			sPk;			//smallint(2)
    UCHAR			ucNobility;		//tinyint(1) unsigned
    DWORD			dwMedal;			//int(4) unsigned
    DWORD			dwMedal_select;	//int(4) unsigned
    UCHAR			ucMetempsychosis;	//tinyint(1) unsigned

    // other
    OBJID			idSyndicate;	//int(4) unsigned
    //	OBJID			idRecordmap;	//int(4)
    USHORT			usPosX;
    USHORT			usPosY;		//int(4) unsigned
    DWORD			dwVirtue;

    // 玩家临时状态
    I64				i64Effect;
    USHORT			usEnergy;
    USHORT			usMaxEnergy;

    DWORD			dwMonsterType;		//? only use for just change mapgroup
    DWORD			dwMonsterSecs;		//? only use for just change mapgroup
};

//////////////////////////////////////////////////////////////////////
class CNetMsg;
class CMsgAction;
class CMsgInteract;
class CAgent : INpc, IAiCenterOwner
{
    COM_STYLE(CAgent)
protected:
    CAgent();
    virtual ~CAgent();

public: // interface
    template<class T>
    T*			QueryInterface(T** ppT)							{ if (ppT) { *ppT = this; } return this; }
    virtual void*	QueryInterface(int id, void** ppT)				{ void* p = NULL; if (id == GUID_OF(CAgent)) { p = this; } if (ppT) { *ppT = p; } return p; }
    DEFINE_QI(IAiCenter, m_pAiCenter)
    DEFINE_QI(CSense, m_pSense)
    DEFINE_QI(CBody, m_pBody)
    DEFINE_QI(CTeam, m_pTeam)
    DEFINE_QI(CItemPack, m_pItemPack)
    DEFINE_QI(CRoleMove, m_pMove)
    DEFINE_QI(CRoleFight, m_pFight)

public: // construction
    bool		Create(OBJID idAgent, int nTimeOfLife);
    bool		AppendInfo(const UserInfoStruct& info);

    // IRole //////////////////////////////////////////////
    virtual IRoleAttr*	QueryRoleAttr()								{ return static_cast<IRoleAttr*>(this); }
    virtual CAutoLink<IRole>&	QueryLink(const CAutoLink<IRole>&)				{ return m_lnkThis; }

public: // get info
    virtual OBJID		GetID()										{ return m_info.id; }
    virtual OBJID		GetMapID();
    virtual int			GetPosX()									{ return m_pMove->GetPosX(); }
    virtual int			GetPosY()									{ return m_pMove->GetPosY(); }
    virtual int			GetDir()									{ return m_nDir; }
    virtual int			GetAttackRange(int nSizeAdd)				{ return m_pItemPack->GetWeaponRange() + (GetSizeAdd() + nSizeAdd + 1) / 2; }
    virtual int			GetSizeAdd()								{ return m_nSizeAdd; }

    virtual bool		IsAlive()									{ return (m_info.i64Effect & KEEPEFFECT_DIE) == 0; }
    virtual bool		IsFarWeapen()								{ return 0; }
    virtual bool		IsEvil()									{ return (m_info.i64Effect & (KEEPEFFECT_DEEPRED | KEEPEFFECT_CRIME)) != 0; }
    virtual bool		IsRighteous()								{ return (m_info.i64Effect & KEEPEFFECT_NOT_VIRTUOUS) == 0; }
    virtual bool		IsLurker()									{ return (m_info.i64Effect & (KEEPEFFECT_LURKER | KEEPEFFECT_HIDDEN)) != 0; }
    virtual bool		IsWing()									{ return false; }	//{ return (m_info.dwEffect & KEEPEFFECT_WING) != 0; }
    virtual int			Distance(int x, int y)						{ return __max(abs(x - GetPosX()), abs(y - GetPosY())); }

public: // application
    virtual void		SetLife(int nData);
    virtual void		SetPower(int nData);
    virtual void		SetEffect(I64 i64Effect);
    virtual void		SetSizeAdd(int nData)						{ m_nSizeAdd = nData; }

    // INpc //////////////////////////////////////////////
public: // main
    virtual void		OnTimer();
    virtual IRole*		QueryRole()									{ return (IRole*)this; }

public: // fight
    virtual void		SetDie();
    virtual void		BeAttack(IRole* pRole);
    virtual void		BeKill(IRole* pRole);
    virtual void		AttackOnce();

public: // move
    virtual bool		SynchroPos(int nSourX, int nSourY, int nTargX, int nTargY);
    virtual bool		KickBack(int nTargX, int nTargY);
    virtual CGameMap*	GetMap()									{ return m_pMove->GetMap(); }
    virtual bool		IsBlockNpc()								{ return (GetEffect() & KEEPEFFECT_NOT_BLOCKNPC) == 0; }

public:	// others
    virtual bool		IsActive()									{ return (GetEffect() & KEEPEFFECT_FREEZE) == 0; }
    virtual bool		Lock(bool bLock)							{ m_bLocked	= bLock; return true; }
    virtual bool		IsLocked()									{ return m_bLocked; }
protected:
    bool		m_bLocked;

    // IAiCenterOwner //////////////////////////////////////////////
    virtual bool	CheckCondition(bool bLogicNot, int idxFactFunction, VarTypeSetRef setParam, ARGUMENT_SET* psetArgument)
    { return (bLogicNot == 0) == (m_pSense->CheckCondition(idxFactFunction, setParam, psetArgument) != 0); }
    virtual int		Priority2Durable(int nPriority);
    /////////////////////////////////////////////////////////////

public: // const
    int				GetDistance(int x, int y)						{ return m_pMove->GetDistance(x, y); }
    int				GetLife()										{ return m_info.usLife; }
    LPCTSTR			GetName()										{ return m_info.szName; }
    int				GetLookRange()									{ return AGENT_VIEW_RANGE; }
    bool			IsCloseTarget(IRole* pRole)						{ return GetDistance(pRole->GetPosX(), pRole->GetPosY()) <= pRole->GetAttackRange(GetSizeAdd()); }
    bool			IsLookTarget(IRole* pRole)						{ return GetDistance(pRole->GetPosX(), pRole->GetPosY()) <= GetLookRange(); }
protected:
    int				GetMaxLife();
    int				GetProfessionSort	()							{ return (m_info.ucProfession % 1000) / 10; }
    int				GetProfessionLevel	()							{ return m_info.ucProfession % 10; }
    I64				GetEffect()										{ return m_info.i64Effect; }
    CTimeOutMS&		QueryTime()										{ return m_tAction; }

public: // application
    bool Login();		// send born msg to game server
    void TalkToMe(LPCTSTR szSender, LPCTSTR szWords, LPCTSTR szEmotion);
    void PrivateTalkToMe(LPCTSTR szSender, LPCTSTR szWords, LPCTSTR szEmotion);
    bool			SendMsg(CNetMsg* pMsg);
    bool			SendMsg(LPCTSTR szMsg);
    void			LoginOK()										{ 	ASSERT(m_nStep == STEP_LOGIN); m_nStep = STEP_LOGIN_OK; }
protected:
    //	void			SetPos(int nPosX, int nPosY);

protected:
    UserInfoStruct	m_info;
    int				m_nDir;
    int				m_nTimeOfLife;
    CAutoLink<IRole>		m_lnkThis;
    int				m_nSizeAdd;

protected: // obj
    CJavaObj<IAiCenter>		m_pAiCenter;
    CJavaObj<CBody>			m_pBody;
    CJavaObj<CSense>		m_pSense;
    CJavaObj<CTeam>			m_pTeam;
    CJavaObj<CItemPack>		m_pItemPack;
    CJavaObj<CRoleMove>		m_pMove;
    CJavaObj<CRoleFight>	m_pFight;

protected: // ctrl
    enum { STEP_NONE, STEP_OFFLINE, STEP_LOGIN, STEP_LOGIN_OK };
    int				m_nStep;
    //	CTimeOut		m_tAttack;		// keep attack
    CTimeOutMS		m_tAction;
};

#endif // !defined(AFX_AGENT_H__71627ADA_0012_4C5F_ACD6_21928AA84DC5__INCLUDED_)
