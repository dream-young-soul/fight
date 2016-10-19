
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// MagicItem.h: interface for the CMagicItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAGICITEM_H__D96FDA8E_53C7_44A1_9C2D_CF20E80FEA21__INCLUDED_)
#define AFX_MAGICITEM_H__D96FDA8E_53C7_44A1_9C2D_CF20E80FEA21__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
using namespace std;

#include "BaseFunc.h"
#include "IRoleView.h"
#include "Game3DEffectEx.h"

class CMagicItem  : public IRoleView
{
public:
    CMagicItem();
    virtual ~CMagicItem();
private:
    char m_szStart[_MAX_STRING];
    char m_szLast[_MAX_STRING];
    char m_szEnd[_MAX_STRING];

    DWORD m_dwTimeBegin;
    int	  m_nMoment;

    vector<CGame3DEffectEx*> m_setEffect;
private:
    BOOL	Load(int nLook);
    void	Init(int );
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

#endif // !defined(AFX_MAGICITEM_H__D96FDA8E_53C7_44A1_9C2D_CF20E80FEA21__INCLUDED_)
