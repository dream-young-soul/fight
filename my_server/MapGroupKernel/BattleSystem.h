
//**********************************************************
// 代码编辑器
//**********************************************************

// BattleSystem.h: interface for the CBattleSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BATTLESYSTEM_H__291473E1_45B6_4A0A_8567_548852B93C33__INCLUDED_)
#define AFX_BATTLESYSTEM_H__291473E1_45B6_4A0A_8567_548852B93C33__INCLUDED_

#include "T_SingleObjSet2.h"

//////////////////////////////////////////////////////////////////////
enum LEVUPEXPDATA
{
    LEVUPEXPDATA_LEVEL		= 0,
    LEVUPEXPDATA_EXP,
    LEVUPEXPDATA_STU_EXP,	// 导师杀死怪，被带的学员这个等级最高可以得到的经验 --  zlong 2004.10.08
};
char	szZcxKey[];
char	szLevupexpTable[];
typedef	CGameData<LEVUPEXPDATA, szLevupexpTable, szZcxKey>	CLevupexpData;
typedef	ISingleObjSet2<CLevupexpData>	ILevupexpSet;
typedef	CSingleObjSet2<CLevupexpData>	CLevupexpSet;

const int	_EXP_TYPE	= 1000;
enum
{
    EXP_TYPE_USER		= 0,	// 人物升级经验
    EXP_TYPE_EUDEMON	= 1,	// 幻兽升级经验
};

enum
{
    MAGICTYPE_NONE		= 0,	// 必须为0(可能由false转换而来)
    MAGICTYPE_NORMAL	= 1,	// 必须为1(可能由true转换而来)
    MAGICTYPE_XPSKILL	= 2,
};

enum TUTOREXPDATA
{
    TUTOREXPDATA_ID			= 0,
    TUTOREXPDATA_TUTOR_LEV,
    TUTOREXPDATA_EXP,
    TUTOREXPDATA_TUTOREXP,
};
char	szTutorKey[];
char	szTutorexpTable[];
typedef CGameData<TUTOREXPDATA, szTutorexpTable, szTutorKey>	CTutorexpData;

class IRole;

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBattleSystem
{
public:
    CBattleSystem(PROCESS_ID idProcess, IRole* pOwner);
    virtual ~CBattleSystem();

public:
    // 一次交战终止或结束时调用
    void		ResetBattle				();
    BOOL		CreateBattle			(OBJID idTarget);
    IRole*		GetOwner				()					{return m_pOwner;}

    BOOL		ProcAttack_Hand2Hand	();
    BOOL		IsBattleMaintain		();
    OBJID		GetTargetID				()					{return m_idTarget;}
    BOOL		IsActived				()					{return m_idTarget != ID_NONE;}

public: // static
    static int	CalcAttackPower(IRole* pAtker, IRole* pTarget, int* pAdjustAtk = 0, int* pAdjustDef = 0);
    static int	AdjustDrop(int nDrop, int nAtkLev, int nDefLev);
    static int	CalcDamageUser2Monster(int nAtk, int nDef, int nAtkLev, int nDefLev);
    static int	CalcDamageMonster2User(int nAtk, int nDef, int nAtkLev, int nDefLev);
    static int	AdjustMinDamageUser2Monster(int nDamage, IRole* pAtker, IRole* pTarget);
    static int	AdjustMinDamageMonster2User(int nDamage, IRole* pAtker, IRole* pTarget);
    static int	CalcPower(int nMagic, IRole* pAtker, IRole* pTarget, int* pAdjustAtk = 0, bool bCanDodge = false);
    static int	CalcPower(int nMagic, IRole* pAtker, IRole* pTarget, int nAdjustAtk, bool bCanDodge = false)		{ return CalcPower(nMagic, pAtker, pTarget, &nAdjustAtk, bCanDodge); }

    static int	AdjustExp(int nExp, int nAtkLev, int nDefLev);

public:
    static BOOL	IsTargetDodged			(IRole* pAtker, IRole* pTarget);

private:
    OBJID			m_idTarget;
    IRole*			m_pOwner;

    PROCESS_ID		m_idProcess;

public: // ctrl
    MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_BATTLESYSTEM_H__291473E1_45B6_4A0A_8567_548852B93C33__INCLUDED_)
