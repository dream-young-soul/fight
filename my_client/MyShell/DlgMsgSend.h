
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGMSGSEND_H__E0DC562A_93D9_42F1_86A7_1922BA5B68A9__INCLUDED_)
#define AFX_DLGMSGSEND_H__E0DC562A_93D9_42F1_86A7_1922BA5B68A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMsgSend.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgMsgSend dialog
#include "MyButton.h"
#include "MyStatic.h"
#include "MyEdit.h"

class CDlgMsgSend : public CDialog
{
    // Construction
public:
    void Show ();
    CDlgMsgSend(CWnd* pParent = NULL);   // standard constructor
    BOOL	m_bShow ;
private:
    CPoint				m_Pnt ;
    OBJID				m_FriendID ;
    // Dialog Data
public:
    void SetTargetFriend( const char* strName );
    //{{AFX_DATA(CDlgMsgSend)
    enum { IDD = IDD_DIALOG_MSGSEND };
    CMyEdit		m_MsgSendData;
    CMyButton	m_MsgSendSend;
    CMyStatic	m_MsgSendName;
    CMyButton	m_MsgSendClose;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgMsgSend)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgMsgSend)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnBtnHelp();
    afx_msg void OnMsgsendCloseb();
    afx_msg void OnMsgsendSend();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMSGSEND_H__E0DC562A_93D9_42F1_86A7_1922BA5B68A9__INCLUDED_)
