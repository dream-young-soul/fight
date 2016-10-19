
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGHAWK_H__21A6B2A7_5659_44D2_A72F_2CA0BB6402C4__INCLUDED_)
#define AFX_DLGHAWK_H__21A6B2A7_5659_44D2_A72F_2CA0BB6402C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgHawk.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgHawk dialog
#include "MyButton.h"
#include "MyEdit.h"

class CDlgHawk : public CDialog
{
    // Construction
public:
    void Show();
    CDlgHawk(CWnd* pParent = NULL);   // standard constructor
    BOOL				m_bShow ;
private:
    CPoint				m_Pnt ;
public:
    // Dialog Data
    //{{AFX_DATA(CDlgHawk)
    enum { IDD = IDD_DIALOG_HAWK };
    CMyEdit	m_EdtData;
    CMyButton	m_BtnClose;
    CMyButton	m_BtnAgree;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgHawk)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgHawk)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnHawkBtnCloses();
    afx_msg void OnHawkBtnAgree();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGHAWK_H__21A6B2A7_5659_44D2_A72F_2CA0BB6402C4__INCLUDED_)
