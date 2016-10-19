
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// 3DArmor.h: interface for the C3DArmor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_3DARMOR_H__8EB91B55_EAD9_4433_8195_F51C9CBABB3D__INCLUDED_)
#define AFX_3DARMOR_H__8EB91B55_EAD9_4433_8195_F51C9CBABB3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "3DRolePart.h"

class C3DArmor	:	public C3DRolePart
{
public:
    C3DArmor();
    virtual ~C3DArmor();

public:
    static C3DArmor* CreateNew(OBJID idType);

public:
    BOOL Create(OBJID idType);
};

#endif // !defined(AFX_3DARMOR_H__8EB91B55_EAD9_4433_8195_F51C9CBABB3D__INCLUDED_)
