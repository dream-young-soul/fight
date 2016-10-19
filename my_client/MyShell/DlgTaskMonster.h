
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGTASKMONSTER_H__A7A6E852_511E_4087_8EA0_438CBE141527__INCLUDED_)
#define AFX_DLGTASKMONSTER_H__A7A6E852_511E_4087_8EA0_438CBE141527__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTaskMonster.h : header file
//
#include "Mybutton.h"
#include "MyList.h"
#include "MyStatic.h"
#include "MyEdit.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgTaskMonster dialog

class CDlgTaskMonster : public CDialog
{
    // Construction
public:
    BOOL IsSetOk();
    void ReSetDlg();
    CPoint m_Pnt;
    BOOL m_bShow;
    void Show();
    CDlgTaskMonster(CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
    //{{AFX_DATA(CDlgTaskMonster)
    enum { IDD = IDD_DIALOG_TASKMONSTER };
    CMyEdit	m_EditMonsterType;
    CMyEdit	m_EditMonsterLev;
    CMyEdit	m_EditMonsterGrow;
    CMyButton	m_BtnSetOk;
    CMyButton	m_BtnSetCancel;
    CMyButton	m_BtnClose;
    int		m_nMonsterGrow;
    int		m_nMonsterLev;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgTaskMonster)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:
    BOOL m_bSetOk;

    // Generated message map functions
    //{{AFX_MSG(CDlgTaskMonster)
    afx_msg void OnBtnClose();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnBtnSetcancel();
    afx_msg void OnBtnSetok();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTASKMONSTER_H__A7A6E852_511E_4087_8EA0_438CBE141527__INCLUDED_)
