
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGMSG_H__2F09F147_268E_41EC_9B1C_B91079669EEF__INCLUDED_)
#define AFX_DLGMSG_H__2F09F147_268E_41EC_9B1C_B91079669EEF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMsg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgMsg dialog
#include "MyButton.h"
#include "MyCheck.h"
#include "MyScrollButton.h"

class CDlgMsg : public CDialog
{
    // Construction
public:
    void PopupCurCheck( BYTE btCurChannel );
    void Show();
    CDlgMsg(CWnd* pParent = NULL);   // standard constructor
    BOOL				m_bShow ;
private:
    CPoint				m_Pnt ;
    BYTE				m_btCurChannel ;
    int					m_nHeight ;
    int					m_nWidth ;
    BYTE				m_btCurShowLine ;
    BOOL				m_bLBtnDown ;
    CPoint				m_MousePnt ;
    // Dialog Data
    //{{AFX_DATA(CDlgMsg)
    enum { IDD = IDD_DIALOG_MSG };
    CMyButton	m_BtnResume;
    CMyScrollButton	m_BtnUp;
    CMyScrollButton	m_BtnDown;
    CMyButton	m_BtnClear;
    CMyCheck	m_BtnUpTeam;
    CMyCheck	m_BtnUpPrivate;
    CMyCheck	m_BtnUpKnight;
    CMyCheck	m_BtnUpFriend;
    CMyCheck	m_BtnUpCur;
    CMyCheck	m_BtnUpAll;
    CMyCheck	m_BtnTeam;
    CMyCheck	m_BtnPrivate;
    CMyCheck	m_BtnKnight;
    CMyCheck	m_BtnFriend;
    CMyCheck	m_BtnCur;
    CMyCheck	m_BtnAll;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgMsg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgMsg)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnBtnAll();
    afx_msg void OnBtnCur();
    afx_msg void OnBtnFriend();
    afx_msg void OnBtnTeam();
    afx_msg void OnBtnPrivate();
    afx_msg void OnBtnKnight();
    afx_msg void OnBtnClear();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnBtnUpall();
    afx_msg void OnBtnUpcur();
    afx_msg void OnBtnUpfriend();
    afx_msg void OnBtnUpteam();
    afx_msg void OnBtnUpprivate();
    afx_msg void OnBtnUpknight();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMSG_H__2F09F147_268E_41EC_9B1C_B91079669EEF__INCLUDED_)
