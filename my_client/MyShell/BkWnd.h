
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_BKWND_H__2FBC627D_2672_4E56_B8D3_9F6CA01AAD5A__INCLUDED_)
#define AFX_BKWND_H__2FBC627D_2672_4E56_B8D3_9F6CA01AAD5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BkWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBkWnd window
#include "MyShellDlg.h"

class CBkWnd : public CWnd
{
    // Construction
public:
    CBkWnd();
    CMyShellDlg		m_MyShellDlg ;
    // Attributes
public:
    // Operations
public:

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CBkWnd)
    //}}AFX_VIRTUAL

    // Implementation
public:
    void Create();
    virtual ~CBkWnd();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    // Generated message map functions
protected:
    HICON m_hIcon;

    //{{AFX_MSG(CBkWnd)
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    afx_msg void OnClose();
    afx_msg void OnActivateApp(BOOL bActive, /*HTASK*/DWORD hTask);
    afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BKWND_H__2FBC627D_2672_4E56_B8D3_9F6CA01AAD5A__INCLUDED_)
