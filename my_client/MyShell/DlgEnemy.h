
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGENEMY_H__E4498A99_19F7_4CEE_9D80_068A76748954__INCLUDED_)
#define AFX_DLGENEMY_H__E4498A99_19F7_4CEE_9D80_068A76748954__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgEnemy.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgEnemy dialog
#include "MyButton.h"
#include "MyCheck.h"
#include "MyList.h"
#include "DlgEnmState.h"
#include "DlgSchoolList.h"

class CDlgEnemy : public CDialog
{
    // Construction
public:
    void Show();
    void SetEnemyState();
    void SetEnemyList ();

    CDlgEnemy(CWnd* pParent = NULL);   // standard constructor
    BOOL				m_bShow ;
private:
    int					m_nTsStatus;
    CPoint				m_Pnt ;
    int 				m_nPntX ;
    // show the frame
    DWORD				m_dwLastShowTime ;
    float				m_fFrameStep ;
    int					m_nFrameMode ;
public:
    void DeleteFriend();
    void DeleteEnemy();
    void SetTeacherBtn();
    void PopCurListChk();
    void SetTsList();
    int m_iShowMode;
    void SetFriendState();
    void SetFriendList();
    // the Enemy state dialog
    CDlgEnmState		m_DlgEnmState ;
    CDlgSchoolList		m_DlgSchoolList;
    int					m_nCurrentQueryIndex ;

    // Dialog Data
    //{{AFX_DATA(CDlgEnemy)
    enum { IDD = IDD_DIALOG_ENEMY };
    CMyButton	m_TeacherBtn;
    CMyButton	m_DelStuBtn;
    CMyCheck	m_TeacherListChk;
    CMyCheck	m_EnemyListChk;
    CMyCheck	m_FriendListChk;
    CMyButton	m_AddBtn;
    CMyList	m_EnemyLstData;
    CMyButton	m_EnemyBtnClose;
    CMyButton	m_BtnHelp;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgEnemy)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:
    void SetTeacherStatus();

    // Generated message map functions
    //{{AFX_MSG(CDlgEnemy)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnEnemyBtnClose();
    afx_msg void OnBtnHelp();
    afx_msg void OnEnemyBtnFriend();
    afx_msg void OnEnemyBtnAdd();
    afx_msg void OnEnemyBtnEnemy();
    afx_msg void OnEnemyBtnTeacher();
    afx_msg void OnBtnTeacher();
    afx_msg void OnBtnDelstudent();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGENEMY_H__E4498A99_19F7_4CEE_9D80_068A76748954__INCLUDED_)
