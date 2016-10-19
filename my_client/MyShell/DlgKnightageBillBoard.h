
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGKNIGHTAGEBILLBOARD_H__36723DED_3709_43F2_BD03_7E30CC56A7BD__INCLUDED_)
#define AFX_DLGKNIGHTAGEBILLBOARD_H__36723DED_3709_43F2_BD03_7E30CC56A7BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgKnightageBillBoard.h : header file
//
#include "myedit.h"
#include "MyStatic.h"
#include "MyButton.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgKnightageBillBoard dialog

class CDlgKnightageBillBoard : public CDialog
{
    // Construction
public:
    void SetBillBoard();
    BOOL m_bShow;
    CPoint m_Pnt;
    void Show(int x, int y);
    CDlgKnightageBillBoard(CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
    //{{AFX_DATA(CDlgKnightageBillBoard)
    enum { IDD = IDD_DIALOG_KNIGHTAGE_BILLBOARD };
    CMyStatic	m_StaWriter;
    CMyStatic	m_StaTime;
    CMyButton	m_BtnCancel;
    CMyButton	m_BtnBillBoard;
    CMyEdit	m_BillBoardEdt;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgKnightageBillBoard)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgKnightageBillBoard)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnBtnBillboard();
    afx_msg void OnBtnCancel();
    afx_msg void OnSetfocusEdtBillboard();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGKNIGHTAGEBILLBOARD_H__36723DED_3709_43F2_BD03_7E30CC56A7BD__INCLUDED_)
