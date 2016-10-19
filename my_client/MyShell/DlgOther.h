
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGOTHER_H__E1A93628_95E0_466C_A1B1_198D41DE3933__INCLUDED_)
#define AFX_DLGOTHER_H__E1A93628_95E0_466C_A1B1_198D41DE3933__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgOther.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgOther dialog
#include "MyButton.h"

class CDlgOther : public CDialog
{
    // Construction
public:
    void Show();
    CDlgOther(CWnd* pParent = NULL);   // standard constructor
    BOOL				m_bShow ;
private:
    CPoint				m_Pnt ;
    // Dialog Data
    //{{AFX_DATA(CDlgOther)
    enum { IDD = IDD_DIALOG_OTHER };
    CMyButton	m_OtherBtnLay;
    CMyButton	m_OtherBtnDel;
    CMyButton	m_BtnCloseB;
    CMyButton	m_BtnClose;
    CMyButton	m_BtnHelp;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgOther)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgOther)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnBtnCloseb();
    afx_msg void OnBtnClose();
    afx_msg void OnBtnHelp();
    afx_msg void OnOtherBtnLay();
    afx_msg void OnOtherBtnDel();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGOTHER_H__E1A93628_95E0_466C_A1B1_198D41DE3933__INCLUDED_)
