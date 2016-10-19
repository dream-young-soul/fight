
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// IRoleView.h: interface for the IRoleView class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IROLEVIEW_H__875A2D30_E0E8_4A84_A715_44F2C7BAC746__INCLUDED_)
#define AFX_IROLEVIEW_H__875A2D30_E0E8_4A84_A715_44F2C7BAC746__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "BaseFunc.h"
enum
{
    _VAR_LOOK				= 0,
    _VAR_HAIR				= 1,
    _VAR_TRANSFORM			= 2,
    _VAR_ARMOR				= 3,
    _VAR_ARMET				= 4,
    _VAR_RWEAPON			= 5,
    _VAR_LWEAPON			= 6,
    _VAR_MOUNT				= 7,
    _VAR_FRAME				= 8,
    _VAR_ROTATEX			= 9,
    _VAR_RWEAPON_ID			= 10,
    _VAR_WEAPON_ACTION_DATA	= 11,
    _VAR_DIR				= 12,
    _VAR_DIRECT_DIR			= 13,
    _VAR_MAX_LIFE			= 14,
    _VAR_LIFE				= 15,
    _VAR_ITEM0				= 16,
    _VAR_ITEM1				= 17,
    _VAR_LOOKCLONE			= 18,
    _VAR_MOMENT				= 19,
    _VAR_DICE0				= 20,
    _VAR_DICE1				= 21,
    _VAR_DICE2				= 22,
    _VAR_MANTLE				= 23,
    _VAR_HEAD				= 24,
    _VAR_ACTION_DATA		= 25,
    _VAR_CLICK				= 26,
    _VAR_STATUS				= 27,
};
class IRoleView
{
public:
    IRoleView();
    virtual ~IRoleView();
public:
    virtual BOOL	SetVariable(int nVariableType, DWORD dwData) {return false;}
    virtual int		SetAction(int nActionType, BOOL bResetMotion = true, BOOL bPlayerSound = false, int nWorldX = 0, int nWorldY = 0, BOOL bSetEffect = false, DWORD dwTerrain = 0) {return 0;}
    virtual void	SetPos ( int nX, int nY, int nHeight, int nRotate, float fScale ) {return;}
    virtual void	SetLightOffset( int type, float x, float y, float z, float a, float r, float g, float b ) {return;}
    virtual void	SetRGBA ( float alpha, float red, float green, float blue ) {return;}
    virtual BOOL	HitTest ( int nHitScreenX, int nHitScreenY, int nMapX, int nMapY ) {return false;}
    virtual BOOL	Draw ( int nMapX, int nMapY ) {return false;}
    virtual DWORD	GetVariable(int nVariableType) {return 0;}
};

#endif // !defined(AFX_IROLEVIEW_H__875A2D30_E0E8_4A84_A715_44F2C7BAC746__INCLUDED_)
