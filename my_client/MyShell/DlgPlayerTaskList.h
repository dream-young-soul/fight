
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGPLAYERTASKLIST_H__0C47EE28_D613_4711_A160_F433B80A78DF__INCLUDED_)
#define AFX_DLGPLAYERTASKLIST_H__0C47EE28_D613_4711_A160_F433B80A78DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPlayerTaskList.h : header file
//
#include "MyList.h"
#include "MyButton.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgPlayerTaskList dialog

class CDlgPlayerTaskList : public CDialog
{
    // Construction
public:
    int m_nListKind;
    void ResetTaskList();
    void GetTaskList();
    void SetTaskListKind(int nKind);
    BOOL m_bShow;
    void Show();
    CDlgPlayerTaskList(CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
    //{{AFX_DATA(CDlgPlayerTaskList)
    enum { IDD = IDD_DIALOG_PLAYERTASKLIST };
    CMyButton	m_BtnListUp;
    CMyButton	m_BtnListDown;
    CMyList	m_ListPlayerTask;
    CMyButton	m_BtnListClose;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgPlayerTaskList)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:
    bool m_bListUp;
    bool m_bListDown;
    int m_nListUnPage;
    int m_nListAcPage;
    int m_nListMyPage;
    void SetLastPageTaskID(int nKind , OBJID idTask);
    OBJID m_LastPageidUnTask[64];
    OBJID m_LastPageidAcTask[64];
    OBJID m_LastPageidMyTask[64];
    CPoint m_Pnt;
    // Generated message map functions
    //{{AFX_MSG(CDlgPlayerTaskList)
    afx_msg void OnBtnListclose();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnBtnListup();
    afx_msg void OnBtnLisdown();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPLAYERTASKLIST_H__0C47EE28_D613_4711_A160_F433B80A78DF__INCLUDED_)
