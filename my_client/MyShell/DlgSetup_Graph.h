
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGSETUP_GRAPH_H__953135C0_D886_4785_94CE_302C86F3FBD3__INCLUDED_)
#define AFX_DLGSETUP_GRAPH_H__953135C0_D886_4785_94CE_302C86F3FBD3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSetup_Graph.h : header file
//
#include "MyCheck.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgSetup_Graph dialog

class CDlgSetup_Graph : public CDialog
{
    // Construction
public:
    void Show(int x, int y);
    BOOL m_bShow;
    CDlgSetup_Graph(CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
    //{{AFX_DATA(CDlgSetup_Graph)
    enum { IDD = IDD_DIALOG_SETUP_GRAPH };
    CMyCheck	m_ChkWeather;
    CMyCheck	m_ChkShadow;
    CMyCheck	m_ChkRoleName;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgSetup_Graph)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:
    CPoint m_Pnt;

    // Generated message map functions
    //{{AFX_MSG(CDlgSetup_Graph)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnChkRolename();
    afx_msg void OnChkShadow();
    afx_msg void OnChkWeather();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSETUP_GRAPH_H__953135C0_D886_4785_94CE_302C86F3FBD3__INCLUDED_)
