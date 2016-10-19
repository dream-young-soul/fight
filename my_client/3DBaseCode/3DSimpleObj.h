
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// 3DSimpleObj.h: interface for the C3DSimpleObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_3DSIMPLEOBJ_H__EACBD297_70EA_458F_BC6E_9C6F7771C3C5__INCLUDED_)
#define AFX_3DSIMPLEOBJ_H__EACBD297_70EA_458F_BC6E_9C6F7771C3C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ND_Bitmap.h"
#include "3DRolePart.h"

#include <vector>
using namespace std;

class C3DSimpleObj	: public C3DRolePart
{
public:
    C3DSimpleObj();
    virtual ~C3DSimpleObj();

public:
    int			m_nPosX;
    int			m_nPosY;
    int			m_nPosH;
    int			m_nRotate;
    float		m_fScale;
    D3DXVECTOR3	m_LightOffset;
    int			m_nShadowType;
    float		m_a;
    float		m_r;
    float		m_g;
    float		m_b;
    BOOL		m_bOblique;
public:
    static C3DSimpleObj* CreateNew(OBJID idType);

public:
    BOOL Create(OBJID idType);
    void SetPos(int nX, int nY, int nHeight, int nRotate, float fScale);
    BOOL Draw2BG(int nMapX, int nMapY);
    BOOL HitTest(int nHitScreenX, int nHitScreenY, int nMapX, int nMapY);
    void SetLightOffset(int type, float x, float y, float z, float a, float r, float g, float b);
    void SetOblique(BOOL bOblique) {m_bOblique = bOblique;}
    void SetShadowType(int nShadowType) {m_nShadowType = nShadowType;}
    void Create(OBJID idMesh, OBJID idTexture);
};

#endif // !defined(AFX_3DSIMPLEOBJ_H__EACBD297_70EA_458F_BC6E_9C6F7771C3C5__INCLUDED_)
