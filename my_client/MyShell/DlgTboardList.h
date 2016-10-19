
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGTBOARDLIST_H__C520B296_5AF2_4907_8DAB_DA908A61F70A__INCLUDED_)
#define AFX_DLGTBOARDLIST_H__C520B296_5AF2_4907_8DAB_DA908A61F70A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTboardList.h : header file
//
#include "MyList.h"
#include "MyButton.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgTboardList dialog

class CDlgTboardList : public CDialog
{
    // Construction
public:
    void SetTboardList();
    void Show();
    BOOL m_bShow;
    CDlgTboardList(CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
    //{{AFX_DATA(CDlgTboardList)
    enum { IDD = IDD_DIALOG_TBOARDLIST };
    CMyList	m_ListTboard;
    CMyButton	m_BtnUppage;
    CMyButton	m_BtnDownPage;
    CMyButton	m_BtnClose;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgTboardList)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:
    CPoint m_Pnt;

    // Generated message map functions
    //{{AFX_MSG(CDlgTboardList)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnBtnClose();
    afx_msg void OnBtnDownpage();
    afx_msg void OnBtnUppage();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTBOARDLIST_H__C520B296_5AF2_4907_8DAB_DA908A61F70A__INCLUDED_)
