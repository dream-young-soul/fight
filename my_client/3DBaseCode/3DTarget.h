
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// 3DTarget.h: interface for the C3DTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_3DTARGET_H__023C125F_48AC_4336_8992_A0DB85BA852B__INCLUDED_)
#define AFX_3DTARGET_H__023C125F_48AC_4336_8992_A0DB85BA852B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IRoleView.h"
#include "3DSimpleObj.h"
class C3DTarget  : public IRoleView
{
public:
    C3DTarget();
    virtual ~C3DTarget();

protected:
    int m_nLook;

    int m_nDir;
    OBJID m_idStandByMotion;
    OBJID m_idWoundMotion;
    OBJID m_idSimpleObj;
    OBJID m_idCurrentMotion;
    char  m_szName[_MAX_NAMESIZE];
    C3DSimpleObj m_obj3DSimpleObj;
    BOOL  m_bHit;
    DWORD m_dwTimer;
    int	  m_nFrameAmount;
protected:
    void SetLook(int nLook);

public:
    virtual BOOL	SetVariable(int nVariableType, DWORD dwData);
    virtual int		SetAction(int nActionType, BOOL bResetMotion = true, BOOL bPlayerSound = false, int nWorldX = 0, int nWorldY = 0, BOOL bSetEffect = false);
    virtual void	SetPos ( int nX, int nY, int nHeight, int nRotate, float fScale );
    virtual void	SetLightOffset( int type, float x, float y, float z, float a, float r, float g, float b );
    virtual void	SetRGBA ( float alpha, float red, float green, float blue );
    virtual BOOL	HitTest ( int nHitScreenX, int nHitScreenY, int nMapX, int nMapY );
    virtual BOOL	Draw ( int nMapX, int nMapY );
    virtual DWORD	GetVariable(int nVariableType);
    static  IRoleView* CreateNewView();
};

#endif // !defined(AFX_3DTARGET_H__023C125F_48AC_4336_8992_A0DB85BA852B__INCLUDED_)
