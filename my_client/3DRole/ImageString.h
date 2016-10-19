
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// ImageString.h: interface for the CImageString class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGESTRING_H__DC21F97B_4D4E_418B_B86A_9DC845158E28__INCLUDED_)
#define AFX_IMAGESTRING_H__DC21F97B_4D4E_418B_B86A_9DC845158E28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "basefunc.h"

class CImageString
{
public:
    CImageString();
    virtual ~CImageString();

    static CImageString* CreateNew(const char* pszString, CMyPos posShow);

    void	SetScalePercent (DWORD dwPercent)	{ m_dwScalePercent = dwPercent; }
    void	SetShowPos		(CMyPos posShow)	{ m_posShow = posShow; }
    void	SetString		(const char* pszString);
    void	SetColor		(DWORD dwColor, DWORD dwAlpha)	{ m_dwColor = dwColor, m_dwAlpha = dwAlpha; }
    void	Show			(void);

private:
    CHAR	m_szString[16];
    CMyPos	m_posShow;
    DWORD	m_dwColor, m_dwAlpha;
    DWORD	m_dwScalePercent;
};

#endif // !defined(AFX_IMAGESTRING_H__DC21F97B_4D4E_418B_B86A_9DC845158E28__INCLUDED_)
