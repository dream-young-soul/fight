
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGTXTACTION_H__FC7766D6_CB92_4300_9154_7CED9F5DE545__INCLUDED_)
#define AFX_DLGTXTACTION_H__FC7766D6_CB92_4300_9154_7CED9F5DE545__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTxtAction.h : header file
//
#include "MyList.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgTxtAction dialog

class CDlgTxtAction : public CDialog
{
    // Construction
public:
    void Show();
    CDlgTxtAction(CWnd* pParent = NULL);   // standard constructor
    BOOL		m_bShow ;
    // Dialog Data
    //{{AFX_DATA(CDlgTxtAction)
    enum { IDD = IDD_DIALOG_TXTACTION };
    CMyList	m_ListTxtAction;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgTxtAction)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:
    BOOL		m_bLoad ;
    CPoint		m_Pnt ;

    // Generated message map functions
    //{{AFX_MSG(CDlgTxtAction)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTXTACTION_H__FC7766D6_CB92_4300_9154_7CED9F5DE545__INCLUDED_)
