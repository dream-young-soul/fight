
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// AlphaMask.h: interface for the CAlphaMask class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALPHAMASK_H__4E68F95F_86D7_465A_84E8_8704C8ADBA42__INCLUDED_)
#define AFX_ALPHAMASK_H__4E68F95F_86D7_465A_84E8_8704C8ADBA42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseFunc.h"

class CAlphaMask
{
public:
    CAlphaMask();
    virtual ~CAlphaMask();

private:
    BYTE*	m_pBuffer;
    CMySize	m_sizeInfo;

private:
    BOOL CreateBuffer();

public:
    BOOL IsTransparent(CMyPos posPoint);
    BOOL Load(CMySize sizeBmp, char* pszFileName);
    BOOL Load(CMySize sizeBmp, void* pMaskData);
};

#endif // !defined(AFX_ALPHAMASK_H__4E68F95F_86D7_465A_84E8_8704C8ADBA42__INCLUDED_)
