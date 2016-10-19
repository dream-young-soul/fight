
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// 3DRolePart.h: interface for the C3DRolePart class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_3DROLEPART_H__709EC8A9_1A3B_4C9A_898F_380DC5124782__INCLUDED_)
#define AFX_3DROLEPART_H__709EC8A9_1A3B_4C9A_898F_380DC5124782__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ND_Bitmap.h"
#include "Game3DEffect.h"
#include "Game3DEffectEx.h"

#include <deque>
using namespace std;

typedef struct
{
    OBJID	idMesh;
    OBJID	idTexture;
    int		nType;
    C3DObj*	p3DMesh;
    OBJID	idTexture2;
    float	fSpeedX;
    float	fSpeedY;
    float	fCurrentX;
    float	fCurrentY;
} C3DObjPartInfo;
//typedef deque<C3DObjPartInfo*> DEQUE_3DOBJPARTINFO;

class C3DRolePart
{
public:
    C3DRolePart();
    virtual ~C3DRolePart();

public:
    inline virtual void			Destroy			(void);
    inline virtual void			Move			(float x, float y, float z);
    inline virtual void			Rotate			(float x, float y, float z);
    inline virtual void			Scale			(float x, float y, float z);
    inline virtual void			SetARGB			(float alpha, float red, float green, float blue);
    inline virtual void			NextFrame		(int nStep);
    inline virtual void			SetFrame		(DWORD dwFrame);
    inline virtual void			Draw			(int type, float lightx, float lighty, float lightz, float sa, float sr, float sg, float sb);
    inline virtual void			DrawAlpha		(int type, float lightx, float lighty, float lightz, float sa, float sr, float sg, float sb, float height);
    inline virtual C3DObjPartInfo*	GetInfo			(int nIndex);
    inline virtual	int				GetInfoAmount	();
    inline virtual void			Clear3DEffect	(char* szIndex = NULL);
    inline virtual void			AddEffect		(char* pszIndex, bool bOnlyOnce = false);
    inline virtual void			SetMotion		(C3DMotion* pMotion);
    inline virtual void			SetTexture		(void);
    inline virtual void			Set3DEffect		(int nLook, int nAction, int nVariable = 0, CGame3DEffectEx* pEffect = NULL, char* pszVmesh = NULL);

    void		SetVirtualMotion	(C3Motion* pMotion);
    void		SetDefaultMotion	(void);
    CGame3DEffectEx* QueryGame3DEffectEx() {return &this->m_objGame3DEffectEx;}

    void	MuliplyPhy			(_D3DXMATRIXA16* pobjMatrix);
    void	ClearMatrix			(void);
    void	TransformCoord		(D3DXVECTOR3* pobjMin, D3DXVECTOR3* pobjMax, _D3DXMATRIXA16* pobjMatrix);
    void	TransformCoordforSimpleObj(D3DXVECTOR3* pobjMin, D3DXVECTOR3* pobjMax, _D3DXMATRIXA16* pobjMatrix);
    C3Motion*	GetVirtualMotion	(const char* pszName);

    OBJID		GetTypeID(void)	{return m_idType;}
    void		ProcessLight();
    void		CreateLight(OBJID idLight);
protected:
    OBJID	m_idType;
    int		m_nAction;
    CGame3DEffectEx		m_objGame3DEffectEx;

private:


    // for light ...
private:
    int				m_nLightIndex;
    DWORD			m_dwTotalCost;
    unsigned char	m_ucSAlpha;
    unsigned char	m_ucSRed;
    unsigned char	m_ucSGreen;
    unsigned char	m_ucSBlue;

    unsigned char	m_ucAlpha;
    unsigned char	m_ucRed;
    unsigned char	m_ucGreen;
    unsigned char	m_ucBlue;


public:
    C3DObjPartInfo m_infoPart;
};

#endif // !defined(AFX_3DROLEPART_H__709EC8A9_1A3B_4C9A_898F_380DC5124782__INCLUDED_)
