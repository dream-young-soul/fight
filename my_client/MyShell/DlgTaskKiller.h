
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGTASKKILLER_H__D74E9912_DAA3_482C_B5F1_C44633830F77__INCLUDED_)
#define AFX_DLGTASKKILLER_H__D74E9912_DAA3_482C_B5F1_C44633830F77__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTaskKiller.h : header file
//
#include "MyEdit.h"
#include "MyButton.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgTaskKiller dialog

class CDlgTaskKiller : public CDialog
{
    // Construction
public:
    BOOL IsSetOk();
    BOOL m_bSetOk;
    char* GetWantKiller();
    void ReSetDlg();
    CPoint m_Pnt;
    BOOL m_bShow;
    void Show();
    CDlgTaskKiller(CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
    //{{AFX_DATA(CDlgTaskKiller)
    enum { IDD = IDD_DIALOG_TASKKILLER };
    CMyButton	m_BtnSetOk;
    CMyButton	m_BtnSetCancel;
    CMyEdit	m_EditKillName;
    CMyButton	m_BtnClose;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgTaskKiller)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:
    char m_szWantKiller[16];

    // Generated message map functions
    //{{AFX_MSG(CDlgTaskKiller)
    afx_msg void OnBtnClose();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnBtnSetcancel();
    afx_msg void OnBtnSetok();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTASKKILLER_H__D74E9912_DAA3_482C_B5F1_C44633830F77__INCLUDED_)
