
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGLOGO_H__72D2DD02_CA8F_4BE6_BBF7_5AE4F03CAB56__INCLUDED_)
#define AFX_DLGLOGO_H__72D2DD02_CA8F_4BE6_BBF7_5AE4F03CAB56__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgLogo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgLogo dialog

class CDlgLogo : public CDialog
{
    // Construction
public:
    CDlgLogo(CWnd* pParent = NULL);   // standard constructor
    HBRUSH			m_Hbr ;
    // Dialog Data
    //{{AFX_DATA(CDlgLogo)
    enum { IDD = IDD_DIALOG_LOGO };
    // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgLogo)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgLogo)
    virtual BOOL OnInitDialog();
    afx_msg void OnDestroy();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGLOGO_H__72D2DD02_CA8F_4BE6_BBF7_5AE4F03CAB56__INCLUDED_)
