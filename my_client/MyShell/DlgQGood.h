
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGQGOOD_H__F0D52391_A424_4576_9D82_0187632812BE__INCLUDED_)
#define AFX_DLGQGOOD_H__F0D52391_A424_4576_9D82_0187632812BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgQGood.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgQGood dialog

class CDlgQGood : public CDialog
{
    // Construction
public:
    BOOL m_bShow;
    void Show();
    CDlgQGood(CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
    //{{AFX_DATA(CDlgQGood)
    enum { IDD = IDD_DIALOG_QGOOD };
    // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgQGood)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:
    CPoint m_Pnt;
    // Generated message map functions
    //{{AFX_MSG(CDlgQGood)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGQGOOD_H__F0D52391_A424_4576_9D82_0187632812BE__INCLUDED_)
