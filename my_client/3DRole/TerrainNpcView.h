
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// TerrainNpcView.h: interface for the CTerrainNpcView class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TERRAINNPCVIEW_H__D0E4FF55_742F_4CB2_9F98_42B48A6BCECA__INCLUDED_)
#define AFX_TERRAINNPCVIEW_H__D0E4FF55_742F_4CB2_9F98_42B48A6BCECA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IRoleView.h"

class CTerrainNpcView  : public IRoleView
{
public:
    CTerrainNpcView();
    virtual ~CTerrainNpcView();
private:
    OBJID m_idNpc;

public:
    virtual void	SetPos ( int nX, int nY, int nHeight, int nRotate, float fScale );
    virtual void	SetLightOffset( int type, float x, float y, float z, float a, float r, float g, float b );
    virtual void	SetRGBA ( float alpha, float red, float green, float blue );
    virtual BOOL	HitTest ( int nHitScreenX, int nHitScreenY, int nMapX, int nMapY );
    virtual BOOL	Draw ( int nMapX, int nMapY );
    virtual DWORD	GetVariable(int nVariableType);
    virtual int		SetAction(int nActionType, BOOL bResetMotion = true, BOOL bPlayerSound = false, int nWorldX = 0, int nWorldY = 0, BOOL bSetEffect = false);

    virtual BOOL	SetVariable(int nVariableType, DWORD dwData);
    static  IRoleView* CreateNewView(int nLook, OBJID idNpc, CMyPos posCell);
public:
    BOOL	Create(OBJID idNpc);
};

#endif // !defined(AFX_TERRAINNPCVIEW_H__D0E4FF55_742F_4CB2_9F98_42B48A6BCECA__INCLUDED_)
