
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// 3DArmet.h: interface for the C3DArmet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_3DARMET_H__5BAC122F_698E_4485_B470_E131551D751B__INCLUDED_)
#define AFX_3DARMET_H__5BAC122F_698E_4485_B470_E131551D751B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "3DRolePart.h"

class C3DArmet	:	public C3DRolePart
{
public:
    C3DArmet();
    virtual ~C3DArmet();

public:
    static C3DArmet*	CreateNew(OBJID idType);

public:
    BOOL	Create(OBJID idType);
};

#endif // !defined(AFX_3DARMET_H__5BAC122F_698E_4485_B470_E131551D751B__INCLUDED_)
