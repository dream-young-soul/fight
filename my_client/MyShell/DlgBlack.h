
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGBLACK_H__51FAF5B7_1A75_4DD0_934C_92D672D30652__INCLUDED_)
#define AFX_DLGBLACK_H__51FAF5B7_1A75_4DD0_934C_92D672D30652__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgBlack.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgBlack dialog
#include "MyButton.h"
#include "MyList.h"
#include "MyEdit.h"

class CDlgBlack : public CDialog
{
    // Construction
public:
    void Show();
    CDlgBlack(CWnd* pParent = NULL);   // standard constructor
    BOOL		m_bShow ;
private:
    CPoint		m_Pnt ;
public:
    void SetBlack();
    // Dialog Data
    //{{AFX_DATA(CDlgBlack)
    enum { IDD = IDD_DIALOG_BLACK };
    CMyEdit	m_BlackEdtName;
    CMyButton	m_BtnHelp;
    CMyList	m_BlackLstData;
    CMyButton	m_BlackCloseb;
    CMyButton	m_BlackBtnDel;
    CMyButton	m_BlackBtnAdd;
    CMyButton	m_BlackBtnCloses;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgBlack)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgBlack)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnBlackBtnCloses();
    afx_msg void OnBtnHelp();
    afx_msg void OnBlackCloseb();
    afx_msg void OnBlackBtnDel();
    afx_msg void OnBlackBtnAdd();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGBLACK_H__51FAF5B7_1A75_4DD0_934C_92D672D30652__INCLUDED_)
