
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// SynStone.h: interface for the CSynStone class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYNSTONE_H__843B0963_1EB0_4F78_B54A_0078A69F2617__INCLUDED_)
#define AFX_SYNSTONE_H__843B0963_1EB0_4F78_B54A_0078A69F2617__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "IRoleView.h"
#include "3DSimpleObj.h"

const int _TARGET_LOOK_AMOUNT = 64;
class CSynStone  : public IRoleView
{
public:
    CSynStone();
    virtual ~CSynStone();
private:
    void SetLook(int nLook);
    void SetTrueLook(int nLook);
    void SetLife(DWORD dwLife);
    void SetMaxLife(DWORD dwLife);
private:
    int m_nLook;
    int m_nDir;
    char  m_szName[_MAX_NAMESIZE];
    OBJID m_idSimpleObj;
    C3DSimpleObj m_obj3DSimpleObj;

    int		m_nAmount;
    int		m_nVarSet[_TARGET_LOOK_AMOUNT];
    int		m_nLookSet[_TARGET_LOOK_AMOUNT];
    DWORD	m_dwMaxLife;
    DWORD	m_dwLife;

public:
    virtual void	SetPos ( int nX, int nY, int nHeight, int nRotate, float fScale );
    virtual void	SetLightOffset( int type, float x, float y, float z, float a, float r, float g, float b );
    virtual void	SetRGBA ( float alpha, float red, float green, float blue );
    virtual BOOL	HitTest ( int nHitScreenX, int nHitScreenY, int nMapX, int nMapY );
    virtual BOOL	Draw ( int nMapX, int nMapY );
    virtual DWORD	GetVariable(int nVariableType);
    virtual int		SetAction(int nActionType, BOOL bResetMotion = true, BOOL bPlayerSound = false, int nWorldX = 0, int nWorldY = 0, BOOL bSetEffect = false);

    virtual BOOL	SetVariable(int nVariableType, DWORD dwData);
    static  IRoleView* CreateNewView();
};

#endif // !defined(AFX_SYNSTONE_H__843B0963_1EB0_4F78_B54A_0078A69F2617__INCLUDED_)
