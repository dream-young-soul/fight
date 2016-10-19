
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGLAY_H__047CB3D8_492F_40E4_80D7_5E268DC3EC56__INCLUDED_)
#define AFX_DLGLAY_H__047CB3D8_492F_40E4_80D7_5E268DC3EC56__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgLay.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgLay dialog
#include "MyButton.h"

class CDlgLay : public CDialog
{
    // Construction
public:
    CDlgLay(CWnd* pParent = NULL);   // standard constructor
    BOOL				m_bShow ;
private:
    CPoint				m_Pnt ;
    int 				m_nPntX ;
    int					m_nDir ;

    // show the frame
    DWORD				m_dwLastShowTime ;
    float				m_fFrameStep ;
    int					m_nFrameMode ;
public:
    void Show();
    // Dialog Data
    //{{AFX_DATA(CDlgLay)
    enum { IDD = IDD_DIALOG_LAY };
    CMyButton	m_LayRight;
    CMyButton	m_LayLeft;
    CMyButton	m_LayCloseB;
    CMyButton	m_LayBtnCloseS;
    CMyButton	m_BtnOk;
    CMyButton	m_BtnHelp;
    CMyButton	m_BtnCancel;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgLay)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgLay)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnLayBtnCloses();
    afx_msg void OnBtnHelp();
    afx_msg void OnLayCloseb();
    afx_msg void OnLayLeft();
    afx_msg void OnLayRight();
    afx_msg void OnBtnOk();
    afx_msg void OnBtnCancel();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGLAY_H__047CB3D8_492F_40E4_80D7_5E268DC3EC56__INCLUDED_)
