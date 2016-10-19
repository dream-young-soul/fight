
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGPLAYERTASKGET_H__4D520B0F_5A42_409E_AD0D_81DB1AB4221C__INCLUDED_)
#define AFX_DLGPLAYERTASKGET_H__4D520B0F_5A42_409E_AD0D_81DB1AB4221C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPlayerTaskGet.h : header file
//
#include "MyEdit.h"
#include "MyButton.h"
#include "MyGrid.h"
#include "MyStatic.h"
#include "DlgListBox.h"
#include "DlgTaskItem.h"
#include "DlgTaskMonster.h"
#include "DlgTaskKiller.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgPlayerTaskGet dialog

class CDlgPlayerTaskGet : public CDialog
{
    // Construction
public:
    void ShowSetDlg();
    BOOL m_bShow;
    void Show();
    CDlgPlayerTaskGet(CWnd* pParent = NULL);   // standard constructor
public:
    void SetTaskSetOk(int nMode);
    BOOL CheckItem(OBJID idItem);
    int m_nReqLevel;
    void ResetTaskData();
    int m_uCurSetDlg;
    void CloseCurSetDlg();
    CDlgTaskItem	m_DlgTaskItem;
    CDlgTaskMonster m_DlgTaskMonster;
    CDlgTaskKiller  m_DlgTaskKiller;
    // Dialog Data
    //{{AFX_DATA(CDlgPlayerTaskGet)
    enum { IDD = IDD_DIALOG_PLAYERTASKGET };
    CMyList	m_ListReqLev;
    CMyStatic	m_StaReqLev;
    CMyEdit	m_EditMoney;
    CMyButton	m_BtnCloseD;
    CMyGrid	m_GridTaskItem;
    CMyEdit	m_EditTaskName;
    CMyEdit	m_EditTaskInfo;
    CMyButton	m_BtnSetMonster;
    CMyButton	m_BtnSetKiller;
    CMyButton	m_BtnSetItem;
    CMyButton	m_BtnTaskGetOk;
    CMyButton	m_BtnReqLev;
    CMyButton	m_BtnClose;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgPlayerTaskGet)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation

protected:
    CPoint m_Pnt;
    // Generated message map functions
    //{{AFX_MSG(CDlgPlayerTaskGet)
    afx_msg void OnBtnClose();
    afx_msg void OnBtnReqlev();
    afx_msg void OnBtnSetitem();
    afx_msg void OnBtnSetkiller();
    afx_msg void OnBtnSetmonster();
    afx_msg void OnBtnTaskgetok();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnBtnClosed();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPLAYERTASKGET_H__4D520B0F_5A42_409E_AD0D_81DB1AB4221C__INCLUDED_)
