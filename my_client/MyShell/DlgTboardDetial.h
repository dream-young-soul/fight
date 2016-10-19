
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGTBOARDDETIAL_H__B1025286_FE20_4F8B_91FD_B8AD5C8A297D__INCLUDED_)
#define AFX_DLGTBOARDDETIAL_H__B1025286_FE20_4F8B_91FD_B8AD5C8A297D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTboardDetial.h : header file
//
#include "MyStatic.h"
#include "MyButton.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgTboardDetial dialog

class CDlgTboardDetial : public CDialog
{
    // Construction
public:
    void SetTboardInfo();
    void SetInfoMode(int nMode);
    BOOL m_bShow;
    void Show();
    CDlgTboardDetial(CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
    //{{AFX_DATA(CDlgTboardDetial)
    enum { IDD = IDD_DIALOG_TBOARDDETIAL };
    CMyStatic	m_StaTitle;
    CMyStatic	m_StaTDegree;
    CMyStatic	m_StaName;
    CMyStatic	m_StaLevel;
    CMyStatic	m_StaJob;
    CMyStatic	m_StaInfo;
    CMyButton	m_BtnHelp;
    CMyButton	m_BtnClose;
    CMyButton	m_BtnCancelBoard;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgTboardDetial)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:
    int m_nViewMode;
    CPoint m_Pnt;

    // Generated message map functions
    //{{AFX_MSG(CDlgTboardDetial)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnBtnCancelboard();
    afx_msg void OnBtnClose();
    afx_msg void OnBtnHelp();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTBOARDDETIAL_H__B1025286_FE20_4F8B_91FD_B8AD5C8A297D__INCLUDED_)
