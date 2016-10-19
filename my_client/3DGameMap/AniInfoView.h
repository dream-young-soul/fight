
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

// AniInfoView.h: interface for the CAniInfoView class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANIINFOVIEW_H__0FA8CF41_6628_4C0B_8C59_9FA84863E317__INCLUDED_)
#define AFX_ANIINFOVIEW_H__0FA8CF41_6628_4C0B_8C59_9FA84863E317__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IInfoView.h"
#include "BaseFunc.h"

class CAniInfoView : public IInfoView
{
public:
    CAniInfoView();
    virtual ~CAniInfoView();
private:
    char	m_szFile[_MAX_STRING];
    char	m_szIndex[_MAX_STRING];
    CMyPos	m_posShow;
    int		m_nFrame;
public:
    virtual void Show();

public:
    static CAniInfoView* CreateNew(char* pszFile, char* pszIndex, CMyPos posShow, int nFrame);
    BOOL Create(char* pszFile, char* pszIndex, CMyPos posShow, int nFrame);
};

#endif // !defined(AFX_ANIINFOVIEW_H__0FA8CF41_6628_4C0B_8C59_9FA84863E317__INCLUDED_)
