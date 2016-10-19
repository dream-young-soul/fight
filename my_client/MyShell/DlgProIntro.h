
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGPROINTRO_H__A59147E1_2FCB_4D98_8350_F75E406F1161__INCLUDED_)
#define AFX_DLGPROINTRO_H__A59147E1_2FCB_4D98_8350_F75E406F1161__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgProIntro.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgProIntro dialog
#include "MyStatic.h"
#include "MyButton.h"

class CDlgProIntro : public CDialog
{
    // Construction
public:
    void Show();
    CDlgProIntro(CWnd* pParent = NULL);   // standard constructor
    BOOL				m_bShow ;
private:
    CPoint				m_Pnt ;
public:
    // Dialog Data
    //{{AFX_DATA(CDlgProIntro)
    enum { IDD = IDD_DIALOG_PROINTRO };
    CMyButton	m_BtnClose;
    CMyStatic	m_Sta1;
    CMyStatic	m_Sta2;
    CMyStatic	m_Sta3;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgProIntro)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgProIntro)
    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnMove(int x, int y);
    afx_msg void OnBtnClose();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPROINTRO_H__A59147E1_2FCB_4D98_8350_F75E406F1161__INCLUDED_)
