
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// 3DWeapon.h: interface for the C3DWeapon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_3DWEAPONDATA_H__CA333759_1F4D_4654_A22C_B9FE77E09B47__INCLUDED_)
#define AFX_3DWEAPON_H__CA333759_1F4D_4654_A22C_B9FE77E09B47__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "3DRolePart.h"

class C3DWeapon  :	public C3DRolePart
{
public:
    C3DWeapon();
    virtual ~C3DWeapon();

public:
    static C3DWeapon*	CreateNew(OBJID idType);

public:
    BOOL		Create(OBJID idType);
};

#endif // !defined(AFX_3DWEAPONDATA_H__CA333759_1F4D_4654_A22C_B9FE77E09B47__INCLUDED_)
