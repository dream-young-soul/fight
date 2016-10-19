
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// 3DMount.h: interface for the C3DMount class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_3DMOUNT_H__F9CDC05C_2392_4A64_BE05_55D3C2632720__INCLUDED_)
#define AFX_3DMOUNT_H__F9CDC05C_2392_4A64_BE05_55D3C2632720__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "3DRolePart.h"

class C3DMount	:	public C3DRolePart
{
public:
    C3DMount();
    virtual ~C3DMount();

public:
    static C3DMount*	CreateNew(OBJID idType);

public:
    BOOL Create(OBJID idType);
};

#endif // !defined(AFX_3DMOUNT_H__F9CDC05C_2392_4A64_BE05_55D3C2632720__INCLUDED_)
