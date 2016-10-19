
//**********************************************************
// 代码编辑器
//**********************************************************

// RoleFight.h: interface for the CRoleFight class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROLEFIGHT_H__C4F4C4C7_E5AD_497F_91FE_372212DBD47A__INCLUDED_)
#define AFX_ROLEFIGHT_H__C4F4C4C7_E5AD_497F_91FE_372212DBD47A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mycom.h"
#include "AutoLink.h"
#include "I_Role.h"
#include "TimeOut.h"

const int	ATTACK_DELAY_MS					= 1500;			// 开始攻击到收到攻击命令的最大延时。(BUG，未考虑震晕)@@@

class CAgent;
class CRoleFight
{
    COM_STYLE(CRoleFight)
protected:
    CRoleFight();
    virtual ~CRoleFight();

public:
    bool		Create(CAgent* pOwner);
    void		OnTimer();

public: // const
    IRole*		QueryTarget()							{ return m_pTarget; }

public: // application
    bool Attack(IRole* pRole);
    void		AttackOnce()							{ m_tAttackDelay.Update(); }
protected:
    CTimeOutMS*	QueryTime()								{ return &m_tFight; }

protected:
    CAgent*			m_pOwner;

protected: // attr
    OBJID			m_idAttackMe;
    CAutoLink<IRole>		m_pTarget;			// may be null
    CTimeOutMS		m_tAttackDelay;
    CTimeOutMS		m_tFight;
};

#endif // !defined(AFX_ROLEFIGHT_H__C4F4C4C7_E5AD_497F_91FE_372212DBD47A__INCLUDED_)
