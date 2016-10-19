
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// 3DSimpleRole.h: interface for the C3DSimpleRole class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_3DSIMPLEROLE_H__A5F50D6D_99F0_418C_8C83_B00F713AC6B8__INCLUDED_)
#define AFX_3DSIMPLEROLE_H__A5F50D6D_99F0_418C_8C83_B00F713AC6B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "3DSimpleObj.h"
#include "ndsound.h"
#include "3DRole.h"

const int _MALE_ROLE_A = 1;
const int _MALE_ROLE_B = 2;
const int _MALE_ROLE_C = 3;
const int _FEMALE_ROLE_A = 4;
const int _FEMALE_ROLE_B = 5;
const int _FEMALE_ROLE_C = 6;

class C3DSimpleRole
{
public:
    C3DSimpleRole();
    virtual ~C3DSimpleRole();
private:
    int		m_nIndex;
    int		m_nLook;
    DWORD	m_dwBegin;
    DWORD   m_dwFlashDelayTime;
    OBJID	m_idSimpleObj;
    int		m_nZoomPercent;
    int		m_XPos;
    int		m_YPos;
    BOOL    m_bSetFlash;
    OBJID	m_idMotionStandBy;
    OBJID	m_idMotionBlaze;
    BOOL	m_bBlaze;
    BOOL	m_bSalute;
    CSound* m_pSound;
    C3DSimpleObj	m_obj3DSimpleObj;
    CGame3DEffectEx m_objEffectF;
    CGame3DEffectEx m_objEffectB;
    C3DRole			m_obj3DRole;
    DWORD			m_dwFrameAmount;
    DWORD			m_nSimpRoleAction;
    int				m_nSimpRoleZoom;
    int				m_nSimpRoleRotate;
    BOOL			m_bResetSalute;
    CMyPos			m_SimpleRolePos;
public:
    BOOL  ShowPet(int nLook, CMyPos posScr,  int nRotateX = 0, int  bFocus = 0);
    BOOL  ShowRole(int nLook, CMyPos posScr, int nZoomPercent = 100, int nRotateX = 0, int  bFocus = 0);
    void Show(int nIndex, CMyPos posScr, int nZoomPercent = 100, int nRotateX = 0, int nStatus = 0);
    void Blaze();
    void SetFlashEffect();
    void SetHead(OBJID idHead);
    void SetLook(int nLook);
    void Reset() {m_nIndex = -1;	}
};

#endif // !defined(AFX_3DSIMPLEROLE_H__A5F50D6D_99F0_418C_8C83_B00F713AC6B8__INCLUDED_)
