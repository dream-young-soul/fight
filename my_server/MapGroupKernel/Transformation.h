
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// Transformation.h: interface for the CTransformation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRANSFORMATION_H__6BCB231D_F76F_4731_A951_26F6648CA26E__INCLUDED_)
#define AFX_TRANSFORMATION_H__6BCB231D_F76F_4731_A951_26F6648CA26E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTransformation
{
protected:
    CTransformation();
    virtual ~CTransformation();

public:
    static CTransformation* CreateNew()				{ return new CTransformation; }
    void	Release()								{ delete this; }
    bool	Create(CUser* pUser, CNpcType* pMonster);
    void	OnTimer();

public: // constant
    DWORD	GetLife				()			{ return m_nLife; }
    DWORD	GetMaxLife			()			{ return m_pMonster->GetInt(NPCTYPEDATA_LIFE); }

    DWORD	GetMinAtk			()			{ return m_pMonster->GetInt(NPCTYPEDATA_ATTACKMIN); }
    DWORD	GetMaxAtk			()			{ return m_pMonster->GetInt(NPCTYPEDATA_ATTACKMAX); }
    DWORD	GetMgcMinAtk			()			{ return m_pMonster->GetInt(NPCTYPEDATA_ATTACKMIN); }
    DWORD	GetMgcMaxAtk			()			{ return m_pMonster->GetInt(NPCTYPEDATA_ATTACKMAX); }
    DWORD	GetAttack			()			{ return (GetMinAtk() + GetMaxAtk()) / 2; }

    DWORD	GetDef				()			{ return GetDefence(); }
    DWORD	GetDefence			()			{ return m_pMonster->GetInt(NPCTYPEDATA_DEFENCE); }
    DWORD	GetDefence2			()			{ return m_pMonster->GetInt(NPCTYPEDATA_DEFENCE2); }

    DWORD	GetDex				()			{ return GetDexterity(); }
    DWORD	GetDexterity		()			{ return m_pMonster->GetInt(NPCTYPEDATA_DEXTERITY); }

    DWORD	GetDdg				()			{ return GetDodge(); }
    DWORD	GetDodge			()			{ return m_pMonster->GetInt(NPCTYPEDATA_DODGE); }

    //	DWORD	GetMagicAtk			()			{ return (GetMinAtk()+GetMaxAtk()) / 2; }
    DWORD	GetMagicDef			()			{ return m_pMonster->GetInt(NPCTYPEDATA_MAGIC_DEF); }

    DWORD	GetInterAtkRate		();
    DWORD	GetIntervalAtkRate	()			{ return m_pMonster->GetInt(NPCTYPEDATA_ATKSPEED); }
    int		GetAttackRange		(int nTargetSizeAdd)			{ return m_pMonster->GetInt(NPCTYPEDATA_ATKRANGE) + (GetSizeAdd() + nTargetSizeAdd + 1) / 2; }
    int		GetSizeAdd			()			{ return m_pMonster->GetInt(NPCTYPEDATA_SIZEADD); }

    DWORD	GetMonsterType		()			{ return m_pMonster->GetID(); }
    OBJID	GetMagicType		()			{ return m_pMonster->GetInt(NPCTYPEDATA_MAGIC_TYPE); }
    int		GetMagicHitRate		()			{ return m_pMonster->GetInt(NPCTYPEDATA_MAGIC_HITRATE); }
    int		GetHitRate			()			{ return m_pMonster->GetInt(NPCTYPEDATA_ATK_HITRATE); }

    int		GetLook				()			{ return CUser::GetLook(m_pMonster->GetInt(NPCTYPEDATA_LOOKFACE)); }
    bool	IsJumpEnable		()			{ return (m_pMonster->GetInt(NPCTYPEDATA_ATKUSER)&ATKUSER_JUMP) != 0; }
    bool	IsMoveEnable		()			{ return (m_pMonster->GetInt(NPCTYPEDATA_ATKUSER)&ATKUSER_FIXED) == 0; }

public: // modify
    void	SetLife				(int nLife)	{ ASSERT(nLife > 0); m_nLife = nLife; }

protected:
    CUser*			m_pUser;
    CNpcType*		m_pMonster;
    int				m_nLife;

    MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_TRANSFORMATION_H__6BCB231D_F76F_4731_A951_26F6648CA26E__INCLUDED_)
