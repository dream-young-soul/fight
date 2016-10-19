
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGTITLEBAR_H__60E1197D_AD00_428D_98F3_81251D8EA922__INCLUDED_)
#define AFX_DLGTITLEBAR_H__60E1197D_AD00_428D_98F3_81251D8EA922__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTitleBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgTitleBar dialog

class CDlgTitleBar : public CDialog
{
    // Construction
public:
    void ResetBk( int nMode );
    CPoint GetOffset();
    CDlgTitleBar(CWnd* pParent = NULL);   // standard constructor
    BOOL	m_bLBtnDown ;
    CRect   m_InitDlgRect ;
    CPoint	m_InitDlgPnt ;
    CPoint  m_MouseMovePnt ;
    HBRUSH	m_Hbr ;
    CWnd*	m_pParentWnd ;
    // Dialog Data
    //{{AFX_DATA(CDlgTitleBar)
    enum { IDD = IDD_DIALOG_TITLEBAR };
    CButton	m_5;
    CButton	m_BtnMin;
    CButton	m_BtnClose;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgTitleBar)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgTitleBar)
    virtual BOOL OnInitDialog();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnBtnClose();
    afx_msg void OnBtnMin();
    afx_msg void OnDestroy();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTITLEBAR_H__60E1197D_AD00_428D_98F3_81251D8EA922__INCLUDED_)
