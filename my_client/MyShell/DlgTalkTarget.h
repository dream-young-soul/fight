
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGTALKTARGET_H__4653F5D5_AED7_4B2E_894E_3AA097870924__INCLUDED_)
#define AFX_DLGTALKTARGET_H__4653F5D5_AED7_4B2E_894E_3AA097870924__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTalkTarget.h : header file
//
#include "MyList.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgTalkTarget dialog

class CDlgTalkTarget : public CDialog
{
    // Construction
public:
    BOOL m_bShow;
    void Show();
    CDlgTalkTarget(CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
    //{{AFX_DATA(CDlgTalkTarget)
    enum { IDD = IDD_DIALOG_TALKTARGET };
    CMyList	m_ListBox;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgTalkTarget)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:
    CPoint m_Pnt;

    // Generated message map functions
    //{{AFX_MSG(CDlgTalkTarget)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    BOOL m_bLoad;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTALKTARGET_H__4653F5D5_AED7_4B2E_894E_3AA097870924__INCLUDED_)
