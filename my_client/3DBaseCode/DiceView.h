
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// DiceView.h: interface for the CDiceView class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DICEVIEW_H__3F5D2E71_9303_4347_96BB_3A751B401882__INCLUDED_)
#define AFX_DICEVIEW_H__3F5D2E71_9303_4347_96BB_3A751B401882__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "3DTaskNpc.h"

class CDiceView  : public C3DTaskNpc
{
public:
    CDiceView();
    virtual ~CDiceView();

protected:
    C3DSimpleObj m_objDesk;
    C3DSimpleObj m_objDiceRoll[3];
    C3DSimpleObj m_objDiceStatic[3];
    BOOL		 m_bRun;
    BOOL		 m_bDiceRun[3];
    DWORD		 m_dwDice[3];
    DWORD		 m_dwDiceEnd[3];
    DWORD		 m_dwDiceFrame[3];
    DWORD		 m_dwOffset[3];
    BOOL		 m_bUp[3];
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

#endif // !defined(AFX_DICEVIEW_H__3F5D2E71_9303_4347_96BB_3A751B401882__INCLUDED_)
