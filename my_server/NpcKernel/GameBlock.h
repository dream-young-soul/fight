
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// GameBlock.h: interface for the CGameBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMEBLOCK_H__B39AA177_958B_4948_A75C_EC068FEB0968__INCLUDED_)
#define AFX_GAMEBLOCK_H__B39AA177_958B_4948_A75C_EC068FEB0968__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UserManager.h"
#include "NpcManager.h"

class CGameBlock
{
public:
    CGameBlock();
    virtual ~CGameBlock();
    static CGameBlock*	CreateNew() { return new CGameBlock; }
    bool	Create();
    virtual ULONG	Release() { delete this; return 0; }

public: // interface
    IUserSet*	QueryUserSet()		{ return m_pUserSet; }
    INpcSet*	QueryNpcSet()		{ return m_pNpcSet; }

public: // dormancy
    bool		IsDormancy()		{ return m_bDormancy; }
    void		SetDormancy(bool b)	{ m_bDormancy = b; }

protected:
    IUserSet*		m_pUserSet;
    INpcSet*		m_pNpcSet;

protected:
    bool			m_bDormancy;
};

#endif // !defined(AFX_GAMEBLOCK_H__B39AA177_958B_4948_A75C_EC068FEB0968__INCLUDED_)
