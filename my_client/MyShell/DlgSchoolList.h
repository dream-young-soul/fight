
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGSCHOOLLIST_H__22685B6A_C9A6_48C5_922E_D8AB60F8028A__INCLUDED_)
#define AFX_DLGSCHOOLLIST_H__22685B6A_C9A6_48C5_922E_D8AB60F8028A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSchoolList.h : header file
//
#include "MyButton.h"
#include "MyList.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgSchoolList dialog

class CDlgSchoolList : public CDialog
{
    // Construction
public:
    void SetSchoolList();
    void Show();
    BOOL m_bShow;
    CDlgSchoolList(CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
    //{{AFX_DATA(CDlgSchoolList)
    enum { IDD = IDD_DIALOG_SCHOOLLIST };
    CMyList	m_ListSecSchool;
    CMyButton	m_BtnClose;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgSchoolList)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:
    CPoint m_Pnt;

    // Generated message map functions
    //{{AFX_MSG(CDlgSchoolList)
    afx_msg void OnBtnClose();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSCHOOLLIST_H__22685B6A_C9A6_48C5_922E_D8AB60F8028A__INCLUDED_)
