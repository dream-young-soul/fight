
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

#include "MultiObjSet.h"
//#include "RoleManager.h"
#include "Myheap.h"
#include "I_Role.h"

typedef	IMultiObjSet<IMapThing>		IThingSet;
typedef	CMultiObjSet<IMapThing>		CThingSet;

class CGameBlock
{
public:
    CGameBlock();
    virtual ~CGameBlock();
public:
    static CGameBlock*	CreateNew() { return new CGameBlock; }
    bool	Create();
    virtual ULONG	Release() { delete this; return 0; }

public: // interface
    IThingSet*	QuerySet()		{ return m_pSet; }

public: // dormancy
    bool		IsDormancy()		{ return m_bDormancy; }
    void		SetDormancy(bool b)	{ m_bDormancy = b; }

protected:
    IThingSet*		m_pSet;

protected:
    bool			m_bDormancy;

public:
    //	MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_GAMEBLOCK_H__B39AA177_958B_4948_A75C_EC068FEB0968__INCLUDED_)
