
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGNORMSG_H__6F6E951C_5BB9_4716_9856_FC52BB4892AB__INCLUDED_)
#define AFX_DLGNORMSG_H__6F6E951C_5BB9_4716_9856_FC52BB4892AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgNorMsg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgNorMsg dialog
#include "MyPackButton.h"

const int _MSGDLG_SETRECT = 25;

class CDlgNorMsg : public CDialog
{
    // Construction
public:
    void ResetDlgRect();
    void Show();
    CDlgNorMsg(CWnd* pParent = NULL);   // standard constructor
    BOOL m_bShow;
    CPoint m_Pnt;
    int m_nMsgUpDown;
    // Dialog Data
    //{{AFX_DATA(CDlgNorMsg)
    enum { IDD = IDD_DIALOG_NORMSG };
    CMyPackButton	m_btnChange;

    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgNorMsg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgNorMsg)
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGNORMSG_H__6F6E951C_5BB9_4716_9856_FC52BB4892AB__INCLUDED_)
