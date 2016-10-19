
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// BoothNpc.h: interface for the CBoothNpc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BOOTHNPC_H__479DBB01_3CF7_4BE1_8CA6_5A17CFC7BA18__INCLUDED_)
#define AFX_BOOTHNPC_H__479DBB01_3CF7_4BE1_8CA6_5A17CFC7BA18__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "3DTaskNpc.h"

class CBoothNpc  : public C3DTaskNpc
{
public:
    CBoothNpc();
    virtual ~CBoothNpc();
private:
    DWORD m_dwItem0;
    DWORD m_dwItem1;

public:
    static  IRoleView* CreateNewView();
    virtual BOOL HitTest ( int nHitScreenX, int nHitScreenY, int nMapX, int nMapY );
    virtual BOOL Draw ( int nMapX, int nMapY );
    virtual BOOL SetVariable(int nVariableType, DWORD dwData);

};
const int BoothMaskOffset[8][5][2] =
{
    {{ -1, 0}, {0, 0}, {0, 0}, {0, 0}, {1, 0}},
    {{ -1, 0}, {0, -1}, {0, 0}, {1, 0}, {0, 1}},
    {{0, 1}, {0, 0}, {0, 0}, {0, 0}, {0, -1}},
    {{0, 1}, { -1, 0}, {0, 0}, {0, -1}, {1, 0}},
    {{ -1, 0}, {0, 0}, {0, 0}, {0, 0}, {1, 0}},
    {{ -1, 0}, {0, -1}, {0, 0}, {1, 0}, {0, 1}},
    {{0, 1}, {0, 0}, {0, 0}, {0, 0}, {0, -1}},
    {{0, 1}, { -1, 0}, {0, 0}, {0, -1}, {1, 0}}
};
#endif // !defined(AFX_BOOTHNPC_H__479DBB01_3CF7_4BE1_8CA6_5A17CFC7BA18__INCLUDED_)
