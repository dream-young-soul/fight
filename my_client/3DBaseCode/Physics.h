
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// Physics.h: interface for the CPysics class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PHYSICS_H__9293353F_A570_448C_A16E_B040AB70D000__INCLUDED_)
#define AFX_PHYSICS_H__9293353F_A570_448C_A16E_B040AB70D000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Basefunc.h"

class CPhysics
{
public:
    CPhysics();
    virtual ~CPhysics();
    // 2D Space
    static DWORD  S2DGetDistance(CMyPos posBegin, CMyPos posEnd);

    // 2D Space Beeline Movement
    static BOOL S2DCMGetPos(int nV, float fA, float fT, float fRadii, CMyPos posBegin, CMyPos posEnd, CMyPos& posCurrent, BOOL bSide);

    // 2D space curve movement
    static BOOL S2DBMGetPos(int nV, float fA, float fT, CMyPos posBegin, CMyPos posEnd, CMyPos& posCurrent);

    // 3D space get dir
    static void S3Get3DDir(CMy3DPos posBegin, CMy3DPos posEnd, CMy3DDir& dir3D);

    // Is point in rect
    static BOOL IsPointInRect(CMyPos posPoint, CMyRect rect);

    // 2D space rotate in 45 view point
    static void S2DR45VP(CMyPos posTarget, CMyPos& posPoint);
};

#endif // !defined(AFX_PHYSICS_H__9293353F_A570_448C_A16E_B040AB70D000__INCLUDED_)
