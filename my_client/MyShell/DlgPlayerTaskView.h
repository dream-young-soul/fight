
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGPLAYERTASKVIEW_H__A67BA3FC_FA1A_4F7E_A72C_6432D61FAF40__INCLUDED_)
#define AFX_DLGPLAYERTASKVIEW_H__A67BA3FC_FA1A_4F7E_A72C_6432D61FAF40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPlayerTaskView.h : header file
//
#include "MyStatic.h"
#include "MyGrid.h"
#include "MyButton.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgPlayerTaskView dialog

class CDlgPlayerTaskView : public CDialog
{
    // Construction
public:
    void SetPlayerTask();
    void SetPlayerTaskViewMode(int nMode);
    CPoint m_Pnt;
    BOOL m_bShow;
    void Show();
    CDlgPlayerTaskView(CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
    //{{AFX_DATA(CDlgPlayerTaskView)
    enum { IDD = IDD_DIALOG_PLAYERTASKVIEW };
    CMyButton	m_BtnClose;
    CMyGrid	m_GridItem;
    CMyStatic	m_StaTaskTitle;
    CMyStatic	m_StaTaskReqInfo;
    CMyStatic	m_StaTaskMoney;
    CMyStatic	m_StaTaskInfo;
    CMyStatic	m_StaReqLev;
    CMyButton	m_BtnTaskOk;
    CMyButton	m_BtnTaskExit;
    CMyButton	m_BtnTaskCancel;
    CMyButton	m_BtnTaskBack;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgPlayerTaskView)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:
    int m_nViewMode;

    // Generated message map functions
    //{{AFX_MSG(CDlgPlayerTaskView)
    afx_msg void OnBtnTaskback();
    afx_msg void OnBtnTaskcancel();
    afx_msg void OnBtnTaskexit();
    afx_msg void OnBtnTaskok();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnBtnClose();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPLAYERTASKVIEW_H__A67BA3FC_FA1A_4F7E_A72C_6432D61FAF40__INCLUDED_)
