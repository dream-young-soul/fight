
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGKNIGHTAGEMEMLIST_H__F3726C6E_F297_4719_BA0B_2ED51F7BC93E__INCLUDED_)
#define AFX_DLGKNIGHTAGEMEMLIST_H__F3726C6E_F297_4719_BA0B_2ED51F7BC93E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgKnightageMemList.h : header file
//
#include "myedit.h"
#include "mybutton.h"
#include "mylist.h"
#include "mystatic.h"
#include "DlgFacState.h"
// Added by ClassView
/////////////////////////////////////////////////////////////////////////////
// CDlgKnightageMemList dialog

class CDlgKnightageMemList : public CDialog
{
    // Construction
public:
    void SetMemList();
    void SetMemData();
    BOOL GetMemberName(int nIndex, char* szName);
    void SetKgButtonStatus();
    void OpenFacState();
    CPoint m_Pnt;
    BOOL   m_bShow;
    void Show(int x, int y);
    CDlgKnightageMemList(CWnd* pParent = NULL);   // standard constructor
    int  m_nCurSelMember;
    CDlgFacState m_DlgFacState;
    // Dialog Data
    //{{AFX_DATA(CDlgKnightageMemList)
    enum { IDD = IDD_DIALOG_KNIGHTAGE_MEMBER };
    CMyButton	m_BtnRight;
    CMyButton	m_BtnLeft;
    CMyButton	m_BtnLeader;
    CMyButton	m_BtnKickOut;
    CMyButton	m_BtnAdd;
    CMyStatic	m_StaTeamHead1;
    CMyList	m_MemberList;
    CMyStatic	m_StaHeadName;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgKnightageMemList)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:
    int m_nMemberIndex;

    // Generated message map functions
    //{{AFX_MSG(CDlgKnightageMemList)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnBtnAdd();
    afx_msg void OnBtnKickout();
    afx_msg void OnBtnLeadernext();
    afx_msg void OnBtnLeft();
    afx_msg void OnBtnRight();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGKNIGHTAGEMEMLIST_H__F3726C6E_F297_4719_BA0B_2ED51F7BC93E__INCLUDED_)
