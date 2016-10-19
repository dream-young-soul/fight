
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGKNIGHTAGEENEMY_H__1274FD72_E75D_4D66_9AB0_6A7FB4C9AEFA__INCLUDED_)
#define AFX_DLGKNIGHTAGEENEMY_H__1274FD72_E75D_4D66_9AB0_6A7FB4C9AEFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgKnightageEnemy.h : header file
//
#include "mylist.h"
#include "MyButton.h"
#include "MyCheck.h"
#include "DlgKnightageState.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgKnightageEnemy dialog

class CDlgKnightageEnemy : public CDialog
{
    // Construction
public:
    void SetFriendList();
    void SetEnemyList();
    void Show(int x, int y);
    CPoint m_Pnt;
    BOOL   m_bShow;
    int	   m_ListMode;
    int    m_nCurSelIndex;
    CDlgKnightageEnemy(CWnd* pParent = NULL);   // standard constructor
    CDlgKnightageState m_DlgKnightageState;

    // Dialog Data
    //{{AFX_DATA(CDlgKnightageEnemy)
    enum { IDD = IDD_DIALOG_KNIGHTAGE_ENEMY };
    CMyCheck	m_ChkFriendList;
    CMyCheck	m_ChkEnemyList;
    CMyButton	m_BtnDelEnemy;
    CMyButton	m_BtnAddEnemy;
    CMyList	m_EnemyList;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgKnightageEnemy)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgKnightageEnemy)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnBtnAddenemy();
    afx_msg void OnBtnDelenemy();
    afx_msg void OnChkEnemylist();
    afx_msg void OnChkFriendlist();
    afx_msg void OnSetfocusListEnemy();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGKNIGHTAGEENEMY_H__1274FD72_E75D_4D66_9AB0_6A7FB4C9AEFA__INCLUDED_)
