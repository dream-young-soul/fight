
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGPK_H__FA3BAF32_D9BC_493A_8640_C33B0469D863__INCLUDED_)
#define AFX_DLGPK_H__FA3BAF32_D9BC_493A_8640_C33B0469D863__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPk.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPk dialog
#include "MyButton.h"

class CDlgPk : public CDialog
{
    // Construction
public:
    void Show();
    CDlgPk(CWnd* pParent = NULL);   // standard constructor

    BOOL				m_bShow ;
private:
    CPoint				m_Pnt ;
    // Dialog Data
    //{{AFX_DATA(CDlgPk)
    enum { IDD = IDD_DIALOG_PK };
    CMyButton	m_PkArre;
    CMyButton	m_PkFree ;
    CMyButton	m_PkGroup ;
    CMyButton	m_PkSafe ;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgPk)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
public:

    // Generated message map functions
    //{{AFX_MSG(CDlgPk)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnPkFree();
    afx_msg void OnPkGroup();
    afx_msg void OnPkSafe();
    afx_msg void OnPkArrestment();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPK_H__FA3BAF32_D9BC_493A_8640_C33B0469D863__INCLUDED_)
