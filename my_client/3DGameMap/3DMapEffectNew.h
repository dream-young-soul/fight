
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// 3DMapEffectNew.h: interface for the C3DMapEffect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_3DMAPEFFECTNEW_H__4DCD9A12_136D_4BEF_B80D_2204A58B7FBE__INCLUDED_)
#define AFX_3DMAPEFFECTNEW_H__4DCD9A12_136D_4BEF_B80D_2204A58B7FBE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "3DMapEffect.h"

class C3DMapEffectNew 	:	public C3DMapEffect
{
private:
    float	m_fScaleX;
    float	m_fScaleY;
    float	m_fScaleZ;

public:
    static  C3DMapEffectNew* CreateNew(CMyPos posWorld, char* pszIndex, BOOL bDelSelf = true, BOOL bSave = false);

    virtual int GetObjType() {return MAP_3DEFFECTNEW;}
    virtual void Show(void* pInfo);

    void	SetScale(float x, float y, float z);

    void	LoadDataObj(FILE* fp);
    void	LoadTextObj(FILE* fp);
    void	SaveDataObj(FILE* fp);
    void	SaveTextObj(FILE* fp);
};

#endif
