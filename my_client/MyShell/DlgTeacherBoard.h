
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGTEACHERBOARD_H__27AE49FE_0352_4D42_9D93_B19AA96DB7DB__INCLUDED_)
#define AFX_DLGTEACHERBOARD_H__27AE49FE_0352_4D42_9D93_B19AA96DB7DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTeacherBoard.h : header file
//
#include "MyButton.h"
#include "MyEdit.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgTeacherBoard dialog

class CDlgTeacherBoard : public CDialog
{
    // Construction
public:
    void Show();
    BOOL m_bShow;
    CDlgTeacherBoard(CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
    //{{AFX_DATA(CDlgTeacherBoard)
    enum { IDD = IDD_DIALOG_TEACHERBOARD };
    CMyEdit	m_EditTitle;
    CMyEdit	m_EditInfo;
    CMyButton	m_BtnUpBoard;
    CMyButton	m_BtnHelp;
    CMyButton	m_BtnClose;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgTeacherBoard)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:
    CPoint m_Pnt;

    // Generated message map functions
    //{{AFX_MSG(CDlgTeacherBoard)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnBtnClose();
    afx_msg void OnBtnHelp();
    afx_msg void OnBtnUpboard();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTEACHERBOARD_H__27AE49FE_0352_4D42_9D93_B19AA96DB7DB__INCLUDED_)
