
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// 3DTaskNpc.h: interface for the C3DTaskNpc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_3DTASKNPC_H__383DB301_CBCA_4B9F_892E_FA78F4E7CF85__INCLUDED_)
#define AFX_3DTASKNPC_H__383DB301_CBCA_4B9F_892E_FA78F4E7CF85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IRoleView.h"
#include "3DSimpleObj.h"

class C3DTaskNpc   : public IRoleView
{
public:
    C3DTaskNpc();
    virtual ~C3DTaskNpc();

protected:
    int m_nLook;
    int m_nDir;
    BOOL  m_bBlaze;
    OBJID m_idStandByMotion;
    OBJID m_idRestMotion;
    OBJID m_idBlazeMotion;
    OBJID m_idSimpleObj;
    OBJID m_idCurrentMotion;
    char  m_szName[_MAX_NAMESIZE];
    int	  m_nZoomPercent;
    C3DSimpleObj m_obj3DSimpleObj;
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

#endif // !defined(AFX_3DTASKNPC_H__383DB301_CBCA_4B9F_892E_FA78F4E7CF85__INCLUDED_)
