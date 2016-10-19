
//**********************************************************
// ´úÂë±à¼­Æ÷
//**********************************************************

#if !defined(AFX_DLGKNIGHTAGEBASE_H__13BFB863_C310_48A1_98BB_A285B516EEBC__INCLUDED_)
#define AFX_DLGKNIGHTAGEBASE_H__13BFB863_C310_48A1_98BB_A285B516EEBC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgKnightageBase.h : header file
//
#include "mystatic.h"
#include "MyButton.h"
#include "MyEdit.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgKnightageBase dialog

class CDlgKnightageBase : public CDialog
{
    // Construction
public:
    void SetKnightBaseData();
    BOOL m_bShow;
    CPoint m_Pnt;
    void Show(int x, int y);
    CDlgKnightageBase(CWnd* pParent = NULL);   // standard constructor
    int  m_nSkip;
    int  m_nEdtNum;
    // Dialog Data
    //{{AFX_DATA(CDlgKnightageBase)
    enum { IDD = IDD_DIALOG_KNIGHTAGE_BASE };
    CMyButton	m_BtnGiveOk;
    CMyButton	m_BtnGiveCancel;
    CMyEdit	m_EditMoney;
    CMyStatic	m_StaHeroTeam;
    CMyStatic	m_StaHeroName;
    CMyStatic	m_StaHeroLev;
    CMyButton	m_BtnGiveMoney;
    CMyButton	m_BtnWantJoin;
    CMyStatic	m_StaOffer;
    CMyStatic	m_StaKnightNum;
    CMyStatic	m_StaKnightName;
    CMyStatic	m_StaKnightMoney;
    CMyStatic	m_StaKnightLev;
    CMyStatic	m_StaKnightFamor;
    CMyStatic	m_StaJob;
    CMyStatic	m_StaHeadName;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgKnightageBase)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CDlgKnightageBase)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnBtnAdd();
    afx_msg void OnBtnGive();
    afx_msg void OnChangeEditMoney();
    afx_msg void OnBtnGiveok();
    afx_msg void OnBtnGivecancel();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGKNIGHTAGEBASE_H__13BFB863_C310_48A1_98BB_A285B516EEBC__INCLUDED_)
