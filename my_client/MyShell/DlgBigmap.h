
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGBIGMAP_H__23D70966_C9CE_4C91_88F7_84881A0B5E9F__INCLUDED_)
#define AFX_DLGBIGMAP_H__23D70966_C9CE_4C91_88F7_84881A0B5E9F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgBigmap.h : header file
//
#include "MyButton.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgBigmap dialog

class CDlgBigmap : public CDialog
{
    // Construction
public:
    void Show();
    CDlgBigmap(CWnd* pParent = NULL);   // standard constructor
    BOOL		m_bShow;

private:
    CPoint				m_Pnt ;

    // Dialog Data
    //{{AFX_DATA(CDlgBigmap)
    enum { IDD = IDD_DIALOG_BIGMAP };
    CMyButton	m_BtnClose;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgBigmap)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation

    // Generated message map functions
    //{{AFX_MSG(CDlgBigmap)
    afx_msg void OnBtnClose();
    afx_msg void OnMove(int x, int y);
    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGBIGMAP_H__23D70966_C9CE_4C91_88F7_84881A0B5E9F__INCLUDED_)
