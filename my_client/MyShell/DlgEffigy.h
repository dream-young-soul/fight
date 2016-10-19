
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGEFFIGY_H__C153EBDC_E8E1_4C95_886B_628A985CEEE3__INCLUDED_)
#define AFX_DLGEFFIGY_H__C153EBDC_E8E1_4C95_886B_628A985CEEE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgEffigy.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgEffigy dialog
#include "MyButton.h"

class CDlgEffigy : public CDialog
{
    // Construction
public:
    void Show();
    CDlgEffigy(CWnd* pParent = NULL);   // standard constructor
    BOOL				m_bShow ;
private:
    CPoint				m_Pnt ;
    int 				m_nPntX ;
    BOOL				m_bPlay ;
    int					m_nDir ;

    // show the frame
    DWORD				m_dwLastShowTime ;
    float				m_fFrameStep ;
    int					m_nFrameMode ;
public:
    // Dialog Data
    //{{AFX_DATA(CDlgEffigy)
    enum { IDD = IDD_DIALOG_EFFIGY };
    CMyButton	m_EffigyRight;
    CMyButton	m_EffigyPandP;
    CMyButton	m_EffigyOk;
    CMyButton	m_EffigyLeft;
    CMyButton	m_Effigy_CLoseS;
    CMyButton	m_Effigy_Close;
    CMyButton	m_BtnHelp;
    CMyButton	m_ActBtn1;
    CMyButton	m_ActBtn2;
    CMyButton	m_ActBtn3;
    CMyButton	m_ActBtn4;
    CMyButton	m_ActBtn5;
    CMyButton	m_ActBtn6;
    CMyButton	m_ActBtn7;
    CMyButton	m_ActBtn8;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgEffigy)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgEffigy)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnBtnHelp();
    afx_msg void OnEffigyClose();
    afx_msg void OnEffigyCloses();
    afx_msg void OnActBtn1();
    afx_msg void OnActBtn2();
    afx_msg void OnActBtn3();
    afx_msg void OnActBtn4();
    afx_msg void OnActBtn5();
    afx_msg void OnActBtn6();
    afx_msg void OnActBtn7();
    afx_msg void OnActBtn8();
    afx_msg void OnEffigyPandp();
    afx_msg void OnEffigyLeft();
    afx_msg void OnEffigyRight();
    afx_msg void OnEffigyOk();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGEFFIGY_H__C153EBDC_E8E1_4C95_886B_628A985CEEE3__INCLUDED_)
