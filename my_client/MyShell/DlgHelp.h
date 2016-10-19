
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGHELP_H__87DF2D19_8209_4C10_BFFB_AF6ACACDE440__INCLUDED_)
#define AFX_DLGHELP_H__87DF2D19_8209_4C10_BFFB_AF6ACACDE440__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgHelp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgHelp dialog
#include "Webbrowser.h"

class CDlgHelp : public CDialog
{
    // Construction
public:
    void ResetBk ( int nMode );
    void OpenHelpDlg( char* strFileName );
    CDlgHelp(CWnd* pParent = NULL);   // standard constructor
    CWebBrowser		m_Web ;
    HBRUSH			m_Hbr ;
    // Dialog Data
    //{{AFX_DATA(CDlgHelp)
    enum { IDD = IDD_DIALOG_HELP };
    CButton	m_HelpMore;
    CButton	m_HelpClose;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgHelp)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgHelp)
    virtual BOOL OnInitDialog();
    afx_msg void OnDestroy();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnButtonClose();
    afx_msg void OnHelpMore();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGHELP_H__87DF2D19_8209_4C10_BFFB_AF6ACACDE440__INCLUDED_)
