
//**********************************************************
// 代码编辑器
//**********************************************************

// Role.h: interface for the CRole class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROLE_H__A27D6633_2132_4F63_AA77_4EB7740D2D67__INCLUDED_)
#define AFX_ROLE_H__A27D6633_2132_4F63_AA77_4EB7740D2D67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)
#include <list>
#include <windows.h>
#include "common.h"
#include "define.h"
#include "I_Role.h"
#include "gameMap.h"


//////////////////////////////////////////////////////////////////////
class CRole : public IRole, public IMapThing
{
protected:
    CRole();
    virtual ~CRole();

public: // get attrib
    virtual OBJID	GetID				()						PURE_VIRTUAL_FUNCTION_0
    virtual LPCTSTR	GetName				()						PURE_VIRTUAL_FUNCTION_0

    virtual DWORD	GetLookFace			()						PURE_VIRTUAL_FUNCTION_0
    virtual DWORD	GetHair				()						PURE_VIRTUAL_FUNCTION_0
    virtual char	GetLength			()						PURE_VIRTUAL_FUNCTION_0		// 取得角色高矮 -- zlong 2004-02-03
    virtual char	GetFat				()						PURE_VIRTUAL_FUNCTION_0		// 取得角色胖瘦 -- zlong 2004-02-03
    virtual int 	GetPosX				()						PURE_VIRTUAL_FUNCTION_0
    virtual int 	GetPosY				()						PURE_VIRTUAL_FUNCTION_0
    virtual int 	GetDir				()						PURE_VIRTUAL_FUNCTION_0
    virtual I64		GetEffect			()						PURE_VIRTUAL_FUNCTION_0
    virtual int 	GetPose				()						PURE_VIRTUAL_FUNCTION_0
    virtual DWORD	GetLev				()						PURE_VIRTUAL_FUNCTION_0

    virtual DWORD	GetLife				()						{ return 0; }
    virtual DWORD	GetMaxLife			()						{ return 0; }

    virtual DWORD	GetMinAtk			()						{ return 0; }
    virtual DWORD	GetMaxAtk			()						{ return 0; }
    virtual DWORD	GetMgcMinAtk		()						{ return 0; }
    virtual DWORD	GetMgcMaxAtk		()						{ return 0; }
    virtual DWORD	GetAttack			()						{ return 0; }
    virtual DWORD	GetAtkHitRate		()						PURE_VIRTUAL_FUNCTION_0

    virtual DWORD	GetDef				()						{ return 0; }
    virtual DWORD	GetDefence			()						{ return 0; }

    virtual DWORD	GetDex				()						{ return 0; }
    virtual DWORD	GetDexterity		()						{ return 0; }

    virtual DWORD	GetDdg				()						{ return 0; }
    virtual DWORD	GetDodge			()						{ return 0; }

    virtual DWORD	GetMagicDef			()						{ return 0; }

    virtual	DWORD	GetInterAtkRate		()						{ return 0; }
    virtual DWORD	GetIntervalAtkRate	()						{ return 0; }

    virtual int		GetHelmetTypeID		()						{ return 0; }
    virtual int		GetArmorTypeID		()						{ return 0; }
    virtual int		GetWeaponRTypeID	()						{ return 0; }
    virtual int 	GetWeaponLTypeID	()						{ return 0; }
    virtual int 	GetMountTypeID		()						{ return 0; }
    virtual int		GetMantleTypeID		()						{ return 0; }

    //	virtual DWORD 	GetMedalSelect		()						{ return 0; }
    //	virtual DWORD	GetTitleSelect		()						{ return 0; }

    virtual int		GetDistance			(int x, int y)			{ return __max(abs(GetPosX() - x), abs(GetPosY() - y)); }
    virtual int		GetDistance			(IMapThing* pTarget)	{ return __max(abs(GetPosX() - pTarget->GetPosX()), abs(GetPosY() - pTarget->GetPosY())); }

    virtual CGameMap*	GetMap			()						{ CHECKF(m_pMap); return m_pMap; }

    virtual DWORD	GetSynID			()						{ return ID_NONE; }
    virtual DWORD	GetSynRank			()						{ return RANK_NONE; }
    virtual DWORD	GetSynRankShow		()						{ return RANK_NONE; }

    virtual bool	IsSimpleMagicAtk	()						{ return 0; }
    virtual int 	GetSimpleMagicHitRate()						{ return 0; }

    virtual int		GetTutorExp			()						{ return 0; }
    virtual UCHAR	GetTutorLevel		()						{ return 0; }
    virtual UCHAR	GetMercenaryRank	()						{ return 0; }
    virtual DWORD	GetMercenaryExp		()						{ return 0; }
    virtual UCHAR	GetNobilityRank		()						{ return 0; }
    virtual DWORD	GetExploit			()						{ return 0; }
public: // const

    virtual bool	IsAlive				()						PURE_VIRTUAL_FUNCTION_0
    virtual bool	IsTalkEnable		()						{ return false; }
    virtual bool	IsGM				()						{ return false; }
    virtual bool	IsPM				()						{ return false; }
    virtual bool	IsMonster			()						{ return false; }
    virtual bool	IsDelThis			()						PURE_VIRTUAL_FUNCTION_0
    virtual bool	IsVirtuous			()						{ return false; }
    virtual bool	IsEvil				()						{ return true; }
    virtual bool	IsBowman			()						{ return false; }
    virtual bool	IsShieldEquip		()						{ return false; }
    virtual bool	IsWing				()						{ return false; }
    virtual bool	IsSendBlockInfo		()						{ return false; }

    // return true: ppv返回对象指针
    virtual bool	QueryObj			(OBJID idObjType, void** ppv)	PURE_VIRTUAL_FUNCTION_0
    virtual IMapThing*	QueryMapThing	()						{ return (IMapThing*)this; }
    virtual IRole*	FindAroundRole		(OBJID id)				{ return GetMap()->QueryRole(GetPosX(), GetPosY(), id); }
    virtual CUser*	QueryOwnerUser		()						{ return NULL; }

public:	// modify attrib ------------------------------
    virtual void	SetDir				(int nDir)							{}
    virtual void	SetPose				(int nPose)							{}
    virtual	void	SetStatus			(int nStatus, bool bSynchro = true)	PURE_VIRTUAL_FUNCTION
    virtual	void	ClsStatus			(int nStatus, bool bSynchro = true)	PURE_VIRTUAL_FUNCTION
    virtual bool	AddAttrib			(int idxAttr, __int64 i64Data, int bSynchro)			PURE_VIRTUAL_FUNCTION_0
    virtual void	ProcessOnMove		(int nMoveMode)						PURE_VIRTUAL_FUNCTION
    virtual void	ProcessAfterMove	()									PURE_VIRTUAL_FUNCTION
    virtual void	GetFootPrint		(int& x, int& y)					PURE_VIRTUAL_FUNCTION

    virtual bool	SpendArrow			()									{ return true; }

    virtual	IStatus*	QueryStatus(OBJID idType)							{ CHECKF(m_setStatus); return m_setStatus->GetObj(idType); }
    virtual	IStatusSet*	QueryStatusSet()									{ CHECKF(m_setStatus); return m_setStatus; }

    virtual void	AwardBattleExp		(int nExp, bool bGemEffect = true)	{}
    virtual int 	AdjustHitRate		(int nHitRate)						{ return nHitRate; }
    virtual int 	AdjustAttack		(int nAtk)							{ return nAtk; }
    virtual int 	AdjustDefence		(int nDef)							{ return nDef; }
    virtual int 	AdjustMagicAtk		(int nAtk)							{ return nAtk; }
    virtual int 	AdjustMagicDef		(int nDef)							{ return nDef; }
    virtual int 	AdjustBowDefence	(int nDef)							{ return nDef; }
    virtual int 	AdjustWeaponDamage	(int nDamage)						{ return nDamage; }
    virtual int 	AdjustMagicDamage	(int nDamage)						{ return nDamage; }
    virtual void	SetFightPause		(int nInterval)						{}
    virtual void	SaveInfo			()												PURE_VIRTUAL_FUNCTION

public: // system
    virtual void	BroadcastRoomMsg(CNetMsg* pMsg, bool bSendSelf = false);
    virtual void	BroadcastRoomMsg(LPCTSTR szMsg, bool bSendSelf = false)	PURE_VIRTUAL_FUNCTION
    virtual bool	SendMsg(CNetMsg* pMsg)									{ return true; }
    virtual bool	SendSysMsg(const char* fmt, ...)						{ return true; }
    virtual void	OnTimer(time_t tCurr)									PURE_VIRTUAL_FUNCTION

public: // map
    virtual bool	MoveToward(int nDir, bool bSync = true);
    virtual void	TransPos(int nPosX, int nPosY);
    virtual void	JumpPos(int nPosX, int nPosY);
    virtual bool	SyncTrackTo(int nPosX, int nPosY, int nDir, DWORD dwAction);
    virtual void	ActiveTransPos(int nPosX, int nPosY);
    virtual bool	SendSelfToBlock()										PURE_VIRTUAL_FUNCTION_0

public: // fight ------------------------------
    virtual void	ClrAttackTarget		()									PURE_VIRTUAL_FUNCTION
    virtual bool	SetAttackTarget(IRole* pTarget = NULL)					PURE_VIRTUAL_FUNCTION_0
    virtual bool	AutoSkillAttack(IRole* pTarget)							PURE_VIRTUAL_FUNCTION_0
    virtual bool	AdditionMagic(int nLifeLost, int nDamage)				PURE_VIRTUAL_FUNCTION_0
    virtual int		Attack(IRole* pTarget)									PURE_VIRTUAL_FUNCTION_0
    virtual bool	BeAttack(bool bMagic, IRole* pTarget, int nPower, bool bReflectEnable = true)					PURE_VIRTUAL_FUNCTION_0
    virtual bool	MagicAttack(int nType, OBJID idTarget, int x, int y)	PURE_VIRTUAL_FUNCTION_0
    virtual void	Kill(IRole* pTarget, DWORD dwDieWay)					PURE_VIRTUAL_FUNCTION
    virtual void	BeKill(IRole* pTarget = NULL)							PURE_VIRTUAL_FUNCTION
    virtual bool	IsAtkable(IRole* pTarget, bool bSendHint = false)			PURE_VIRTUAL_FUNCTION_0
    virtual bool	IsBeAtkable()											PURE_VIRTUAL_FUNCTION_0
    virtual int		GetAttackRange(int nTargetSizeAdd)						PURE_VIRTUAL_FUNCTION_0
    virtual int		GetSizeAdd()											PURE_VIRTUAL_FUNCTION_0
    virtual	int		GetLuck()												PURE_VIRTUAL_FUNCTION_0
    virtual void	SendDamageMsg(OBJID idTarget, int nDamage)				PURE_VIRTUAL_FUNCTION

    virtual bool	TransferShield(bool bMagic, IRole* pAtker, int nDamage)	{ return false; }

public:	// 为了修改魔法系统而增加的部分，等魔法系统修改完再做整理 -- zlong 2005-03-23
    virtual void	AwardSynWarScore(CNpc* pNpc, int nScore)				{}
    virtual bool	IsImmunity(IRole* pRole)								{ return false; }
    virtual DWORD	GetProfession()											{ return 0; }
    virtual int		GetGemExpEffect()										{ return 0; }
    virtual int		GetGemMgcExpEffect()									{ return 0; }
    virtual bool	SpendEquipItem(int nSubType, int nNum, bool bSynchro)	{ return false; }
    virtual int		AdjustExp(IRole* pTarget, int nRawExp, bool bNewbieBonusMsg = false)		{ return nRawExp; }
    virtual bool	DecEquipmentDurability(bool bBeAttack, bool bMagic, int bDurValue = 1)	{ return false; }
    virtual void	SendGemEffect()											{}
    virtual bool	IsEmbedGemType(int nGemType)							{ return false; }

    virtual DWORD	GetMana()												{ return 0; }
    virtual DWORD	GetMaxMana()											{ return 0; }

    virtual int		GetEnergy()												{ return 0; }
    virtual bool	CheckWeaponSubType(int nSubType, int nNum = 0)			{ return false; }
    virtual DWORD	GetPotential()											{ return 0; }
    virtual CTransformation* QueryTransformation()							{ return NULL; }
    virtual bool	IsArrowPass(int x, int y, int nAlt = ARROW_FLY_ALT)		{ return true; }
    virtual CTeam*	GetTeam()												{ return NULL; }
    virtual CItem*	GetEquipItemByPos(int nPosition)						{ return NULL; }
    virtual bool	CheckCrime(IRole* pRole)								{ return false; }
    virtual void	SetDelay(int nDelay)									{}

    virtual CItem*	GetWeaponR()											{ return NULL; }
    virtual CItem*	GetWeaponL()											{ return NULL; }

    virtual bool	DropItem(OBJID idItem, int x, int y)					{ return false; }
    virtual DWORD	GetSoulSum()											{ return 0; }
    virtual bool	Synchro()												{ return true; }
    virtual CItem*	FindSpaceTransSpell()									{ return NULL; }
    virtual void	AddEquipmentDurability(int nPosition, int nAddValue)				{}
    virtual bool	SynPosition(int nPosX, int nPosY, int nMaxDislocation = 8)			{ return true; }
    virtual bool	Transform(DWORD dwType, int nKeepSecs, bool bSynchro = true)		{ return false; }
    virtual bool	AddItem (CItem* pItem, bool bSynchro, bool bUpdate = true)			{ return false; }


    virtual bool	IsInFan(POINT pos, POINT posSource, int nRange, int nWidth, POINT posCenter);
    virtual int		AdjustData(int nData, int nAdjust, int nMaxData = 0)	{ return CRole::AdjustDataEx(nData, nAdjust, nMaxData); }
    static int		AdjustDataEx(int nData, int nAdjust, int nMaxData = 0);

    virtual	CMagic*	QueryMagic()											{ return NULL; }

protected:
    IStatusSet*		m_setStatus;
    //	I64				m_i64Effect;

    PROCESS_ID	m_idProcess;

    //
    CGameMap*	m_pMap;
    int			m_nPosX;
    int			m_nPosY;
    int			m_nDir;

    // 广播对象集
    typedef std::list<OBJID>	BROADCAST_SET;
    BROADCAST_SET	m_setBCRole;
    BROADCAST_SET	m_setBCMapItem;

public:
    virtual bool	UpdateBroadcastSet(bool bClearSet = false);
    virtual void	ClrBroadcastSet();

public:
    virtual void	AddToBCRoleSet			(OBJID idRole, bool bSendMsg);
    virtual void	RemoveFromBCRoleSet		(OBJID idRole);
    virtual void	AddToBCMapItemSet		(OBJID idMapItem);
    virtual void	RemoveFromBCMapItemSet	(OBJID idMapItem);

public:
    static bool		AttachStatus(IRole* pRole, int nStatus, int nPower, int nSecs, int nTimes = 0);		// =0 : status once
    static bool		DetachStatus(IRole* pRole, int nStatus);
    static void		DetachWellStatus(IRole* pRole);
    static void		DetachBadlyStatus(IRole* pRole);
    static void		DetachAllStatus(IRole* pRole);
    static bool		IsWellStatus(int nStatus);
    static bool		IsBadlyStatus(int nStatus);

};

#endif // !defined(AFX_ROLE_H__A27D6633_2132_4F63_AA77_4EB7740D2D67__INCLUDED_)
