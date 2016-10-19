
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGKNIGHTAGESTATE_H__44EF6233_1110_4EF7_AA49_BEC3553155AD__INCLUDED_)
#define AFX_DLGKNIGHTAGESTATE_H__44EF6233_1110_4EF7_AA49_BEC3553155AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgKnightageState.h : header file
//
#include "mystatic.h"
#include "mybutton.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgKnightageState dialog

class CDlgKnightageState : public CDialog
{
    // Construction
public:
    CWnd* m_ParentWnd;
    void SetParentWnd(CWnd* pParentWnd);
    void SetStateData();
    void Show();
    CDlgKnightageState(CWnd* pParent = NULL);   // standard constructor
    CPoint m_Pnt;
    BOOL   m_bShow;
    // Dialog Data
    //{{AFX_DATA(CDlgKnightageState)
    enum { IDD = IDD_DIALOG_KNIGHTAGE_STATE };
    CMyStatic	m_StaSynLev;
    CMyButton	m_CloseBtn;
    CMyStatic	m_StaNumber;
    CMyStatic	m_StaName;
    CMyStatic	m_StaLevel;
    CMyStatic	m_StaHeadName;
    CMyStatic	m_StaFamor;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgKnightageState)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgKnightageState)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnBtnClose();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGKNIGHTAGESTATE_H__44EF6233_1110_4EF7_AA49_BEC3553155AD__INCLUDED_)
