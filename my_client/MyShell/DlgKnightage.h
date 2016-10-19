
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGKNIGHTAGE_H__F393BD90_38F7_433B_8F97_112591833FCB__INCLUDED_)
#define AFX_DLGKNIGHTAGE_H__F393BD90_38F7_433B_8F97_112591833FCB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgKnightage.h : header file
//
#include "mybutton.h"
#include "myCheck.h"
#include "DlgKnightagebase.h"
#include "DlgKnightageenemy.h"
#include "DlgKnightageMemList.h"
#include "DlgKnightageBillboard.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgKnightage dialog

class CDlgKnightage : public CDialog
{
    // Construction
public:
    void HideCurDlg(BOOL flag);
    void ShowChildDlg();
    void FlashKnightage();
    CDlgKnightageBase		m_DlgKnightageBase;
    CDlgKnightageEnemy		m_DlgKnightageEnemy;
    CDlgKnightageMemList	m_DlgKnightageMemList;
    CDlgKnightageBillBoard	m_DlgKnightageBillBoard;
    BOOL m_bShow;
    void Show();
    int m_PntX;
    int m_iCurDlg;
    CPoint m_Pnt;
    CDlgKnightage(CWnd* pParent = NULL);   // standard constructor
    // show the frame
    DWORD				m_dwLastShowTime ;
    float				m_fFrameStep ;
    int					m_nFrameMode ;
    // Dialog Data
    //{{AFX_DATA(CDlgKnightage)
    enum { IDD = IDD_DIALOG_KNIGHTAGE };
    CMyCheck	m_ChkMember;
    CMyCheck	m_ChkEnemy;
    CMyCheck	m_ChkBillBoard;
    CMyCheck	m_ChkBase;
    CMyButton	m_CloseBtn;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgKnightage)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgKnightage)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnBtnClose();
    afx_msg void OnChkBase();
    afx_msg void OnChkBillboard();
    afx_msg void OnChkEnemy();
    afx_msg void OnChkMember();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGKNIGHTAGE_H__F393BD90_38F7_433B_8F97_112591833FCB__INCLUDED_)
