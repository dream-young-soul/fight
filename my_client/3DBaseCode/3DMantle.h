
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// 3DMantle.h: interface for the C3DMantle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_3DMANTLE_H__78314DBA_4D46_4DBF_AD08_B3F2A2612397__INCLUDED_)
#define AFX_3DMANTLE_H__78314DBA_4D46_4DBF_AD08_B3F2A2612397__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "3DRolePart.h"

class C3DMantle    :	public C3DRolePart
{
public:
    C3DMantle();
    virtual ~C3DMantle();

public:
    static C3DMantle*	CreateNew(OBJID idType);

public:
    BOOL		Create(OBJID idType);
};

#endif // !defined(AFX_3DMANTLE_H__78314DBA_4D46_4DBF_AD08_B3F2A2612397__INCLUDED_)
