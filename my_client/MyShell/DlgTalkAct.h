
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGTALKACT_H__3EEB533D_3E4F_428C_9F15_536BF845EECF__INCLUDED_)
#define AFX_DLGTALKACT_H__3EEB533D_3E4F_428C_9F15_536BF845EECF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTalkAct.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgTalkAct dialog
#include "MyButton.h"
const int ACTION_CHOOSE = 5006 ;

class CDlgTalkAct : public CDialog
{
    // Construction
public:
    void SetParentCWnd ( CWnd* ParentCWnd );
    void Show();
    CDlgTalkAct(CWnd* pParent = NULL);   // standard constructor
    BOOL		m_bShow ;
private:
    CPoint		m_Pnt ;
    int			m_nSpace ;
    int			m_nCurIndex ;
    int			m_nCurAmount ;
    CWnd*		m_pParentCWnd ;
    // Dialog Data
    //{{AFX_DATA(CDlgTalkAct)
    enum { IDD = IDD_DIALOG_TALKACT };
    CMyButton	m_TalkActBtnPageUp;
    CMyButton	m_TalkActBtnPageDown;
    CMyButton	m_TalkActBtnHelp;
    CMyButton	m_TalkActBtnCloseB;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgTalkAct)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgTalkAct)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnTalkActBtnHelp();
    afx_msg void OnTalkActBtnCloseB();
    afx_msg void OnTalkActBtnPageUp();
    afx_msg void OnTalkActBtnPageDown();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTALKACT_H__3EEB533D_3E4F_428C_9F15_536BF845EECF__INCLUDED_)
