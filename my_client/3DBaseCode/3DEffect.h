
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// 3DEffect.h: interface for the C3DEffect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_3DEFFECT_H__E643C394_B705_4DAF_A473_BA9A68205F64__INCLUDED_)
#define AFX_3DEFFECT_H__E643C394_B705_4DAF_A473_BA9A68205F64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseFunc.h"
#include "ND_Bitmap.h"
#include "c3_shape.h"
#include "c3_ptcl.h"

// constant ...
const int _EFFECT_PART_MAX_	 = 24;
class C3DEffect
{
public:
    C3DEffect();
    virtual ~C3DEffect();

private:
    DWORD		m_dwShapes;
    C3Shape*		m_Shape[_EFFECT_PART_MAX_];
    C3SMotion*	m_SMotion[_EFFECT_PART_MAX_];

    DWORD		m_dwPtcls;
    C3Ptcl*		m_Ptcl[_EFFECT_PART_MAX_];

    DWORD		m_dwPhys;
    C3Phy*		m_Phy[_EFFECT_PART_MAX_];
    C3Motion*	m_Motion[_EFFECT_PART_MAX_];

    _D3DXMATRIXA16	matrix;

public:
    BOOL Load(char* lpFilename);
    void Unload(void);
    void Bind(C3DObj* lpObj);
    void Bind( _D3DXMATRIXA16* matrix);
    void Bind(C3DObj* lpObj, char* pszPartName);

    void Draw(BOOL bNextFrame = true, int nAsb = 5, int nAdb = 6);
    void SetTexture(C3DTexture* pTexture);
    void ResetShape(void);

    void Move(float x, float y, float z);
    void Rotate(float x, float y, float z);
    void Scale(float x, float y, float z);

    DWORD GetPtclAmount() {return m_dwPtcls;}
    DWORD GetPhyAmount() {return m_dwPhys;}
    void SetPtclCurFrame ( DWORD dwIndex, DWORD dwFrame );
    int GetPtclFrames ( DWORD dwIndex );
    void SetPhyCurFrame ( DWORD dwIndex, DWORD dwFrame );
    int GetPhyFrames ( DWORD dwIndex );
    DWORD GetFrames();
    void SetFrames(DWORD dwFrame);

    DWORD GetShapeAmount() {return m_dwShapes;}

    ShapeBackupInfo* CreateShapeInfo	(void);
    void	BackupShapeInfo		(ShapeBackupInfo* pBuf);
    void	RestoreShapeInfo	(ShapeBackupInfo* pBuf);

    int				m_nShapeAir;

    static void	DestroyShapeInfo	(ShapeBackupInfo*& pBuf, int nShapes);
};
#ifdef _ANALYSIS_ON
extern DWORD g_dw3DEffectLoadAmount;
extern DWORD g_dw3DEffectLoadTime;
extern DWORD g_dw3DEffectLoadTimeMax;
extern DWORD g_dw3DEffectLoadTimeFrame;
#endif

#endif // !defined(AFX_3DEFFECT_H__E643C394_B705_4DAF_A473_BA9A68205F64__INCLUDED_)
